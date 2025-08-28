#include "status-bar.hpp"

#include <CommCtrl.h>

#include <chrono>
#include <cmath>
#include <format>
#include <ranges>
#include <sstream>
#include <thread>

#include "lib/api.hpp"
#include "lib/settings.hpp"
#include "resource.hpp"

StatusBar& StatusBar::Instance() {
    static StatusBar instance;
    return instance;
}

LRESULT StatusBar::ConfigureParts(HWND hWnd) {
    // get the parent window coords
    HWND hStatusBar = GetDlgItem(hWnd, IDC_STATUS_BAR);
    RECT rcWindow = {};
    GetClientRect(hWnd, &rcWindow);

    // figure out how many status bar parts there are
    // going to be and how wide each one should be
    int windowWidth = rcWindow.right - rcWindow.left;
    int statusBarPartsNum = static_cast<int>(StatusBar::parts.size());
    int statusBarPartWidth = windowWidth / statusBarPartsNum;
    int statusBarRightEdge = statusBarPartWidth;
    std::vector<int> statusBarParts(statusBarPartsNum);

    // calculate the right edge coords for each part
    for (int i = 0; i < statusBarPartsNum; i++) {
        statusBarParts[i] = statusBarRightEdge;
        statusBarRightEdge += statusBarPartWidth;
    }

    SendMessage(hStatusBar,
        SB_SETPARTS,
        static_cast<WPARAM>(statusBarPartsNum),
        reinterpret_cast<LPARAM>(statusBarParts.data()));

    return 0;
}

HWND StatusBar::CreateControl(HWND hWnd) {
    HWND hStatusBar = CreateWindowEx(0,
        STATUSCLASSNAME,
        nullptr,
        WS_VISIBLE | WS_CHILD | SBARS_SIZEGRIP,
        0,
        0,
        0,
        0,
        hWnd,
        (HMENU)(INT_PTR)IDC_STATUS_BAR,
        GetModuleHandle(nullptr),
        nullptr);

    // parts must be configured now so that the api requests
    // which come before `WM_SIZE` can be called have the
    // indices ready to attach their data to
    StatusBar::ConfigureParts(hWnd);

    // load the async data and initial render
    StatusBar::Load(hWnd);
    StatusBar::RenderText(hWnd);
    return hStatusBar;
}

std::string StatusBar::FormatCurrency(const uint32_t number) {
    // currency formatting requires a string stream
    std::ostringstream currency;
    currency.imbue(StatusBar::locale);
    currency << std::showbase << std::put_money(std::to_string(number));
    return currency.str();
}

std::string StatusBar::FormatNumber(uint32_t number) {
    static constexpr const char* suffixes[] = { "", "K", "M", "B", "T" };
    static constexpr int numSuffixes = sizeof(suffixes) / sizeof(suffixes[0]);

    double value = static_cast<double>(number);
    int suffixIdx = 0;

    // reduce the value by 1000 until we reach
    // `< 1000` or run out of suffixes
    while (value >= 1000.0 && suffixIdx < numSuffixes - 1) {
        value /= 1000.0;
        ++suffixIdx;
    }

    // determine if we're a whole number or nearly zero (1e-6) and ignore
    // decimals; otherwise we only care about up to 2 decimal places
    std::string result;

    if (std::fmod(value, 1.0) < 1e-6) {
        result = std::format("{}", static_cast<uint64_t>(value));
    } else {
        result = std::format("{:.2f}", value);

        // remove trailing zeroes and decimal points
        result.erase(result.find_last_not_of('0') + 1);

        if (result.back() == '.') {
            result.pop_back();
        }
    }

    result += suffixes[suffixIdx];
    return result;
}

void StatusBar::Load(HWND hWnd) {
    // bail early if no api key configured
    std::string adminApiKey;
    lib::settings::adminApiKey::load(adminApiKey);

    if (adminApiKey.empty()) {
        return;
    }

    // load data from cache if we can
    auto now = std::chrono::system_clock::now();

    std::chrono::system_clock::time_point lastChecked;
    lib::settings::statLastChecked::load(lastChecked);

    if (now - lastChecked < std::chrono::days(1)) {
        // update cost
        uint32_t cost;
        lib::settings::statCost::load(cost);
        parts[0].second = StatusBar::FormatCurrency(cost);

        // update input and output tokens
        uint32_t inputTokens;
        uint32_t outputTokens;

        lib::settings::statInputTokens::load(inputTokens);
        lib::settings::statOutputTokens::load(outputTokens);

        parts[1].second = StatusBar::FormatNumber(inputTokens);
        parts[2].second = StatusBar::FormatNumber(outputTokens);

        // render the data
        StatusBar::RenderText(hWnd);
        return;
    }

    // get the timestamp for the start of the current month
    auto today = std::chrono::year_month_day(std::chrono::floor<std::chrono::days>(now));
    auto month = today.year() / today.month() / std::chrono::day(1);

    auto monthTimepoint = std::chrono::system_clock::time_point(std::chrono::sys_days(month));
    auto monthTimestamp = std::chrono::duration_cast<std::chrono::seconds>(monthTimepoint.time_since_epoch()).count();

    // monthly usage
    std::jthread([adminApiKey, hWnd, monthTimestamp]() {
        auto r = lib::api::usage(adminApiKey, monthTimestamp);
        auto* body = new std::string(r.body.dump());
        PostMessage(hWnd, WM_USER_ADMIN_API_RESPONSE, 0, reinterpret_cast<LPARAM>(body));
    }).detach();

    // load monthly cost
    std::jthread([adminApiKey, hWnd, monthTimestamp]() {
        auto r = lib::api::cost(adminApiKey, monthTimestamp);
        auto* body = new std::string(r.body.dump());
        PostMessage(hWnd, WM_USER_ADMIN_API_RESPONSE, 1, reinterpret_cast<LPARAM>(body));
    }).detach();
}

LRESULT StatusBar::OnAdminApiResponse(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    auto* r = reinterpret_cast<std::string*>(lParam);

    if (!r) {
        return 0;
    }

    // extract json from response
    auto json = nlohmann::json::parse(*r);

    if (!json.is_array()) {
        delete r;
        return 0;
    }

    // handle statistic type
    switch (wParam) {
        case 0: {
            uint32_t inputTokens = 0;
            uint32_t outputTokens = 0;

            for (auto& bucket : json) {
                if (!bucket.contains("results") || !bucket["results"].is_array()) {
                    continue;
                }

                for (auto& bucketItem : bucket["results"]) {
                    inputTokens += bucketItem["input_tokens"];
                    outputTokens += bucketItem["output_tokens"];
                }
            }

            // cache the values and update their status bar parts
            lib::settings::statInputTokens::save(inputTokens);
            lib::settings::statOutputTokens::save(outputTokens);
            parts[1].second = StatusBar::FormatNumber(inputTokens);
            parts[2].second = StatusBar::FormatNumber(outputTokens);
            break;
        }
        case 1: {
            uint32_t cost = 0;

            for (auto& bucket : json) {
                if (!bucket.contains("results") || !bucket["results"].is_array()) {
                    continue;
                }

                for (auto& bucketItem : bucket["results"]) {
                    // convert explicitly to double from json to preserve
                    // precision and convert from cents to dollar
                    auto cents = bucketItem["amount"]["value"].get<double>();
                    cost += static_cast<uint32_t>(cents * 100);
                }
            }

            // cache the value and update its status bar part
            lib::settings::statCost::save(cost);
            parts[0].second = StatusBar::FormatCurrency(cost);
            break;
        }
    }

    // update last checked timestamp
    auto now = std::chrono::system_clock::now();
    lib::settings::statLastChecked::save(now);

    // render the data
    StatusBar::RenderText(hWnd);

    // clean up
    delete r;
    return 0;
}

LRESULT StatusBar::OnUpdateSessionUsage(HWND hWnd, WPARAM wParam, LPARAM) {
    // extract the input and output tokens
    //
    // lower 16 bits = input tokens
    // upper 16 bits = output tokens
    uint32_t inputTokens = LOWORD(wParam);
    uint32_t outputTokens = HIWORD(wParam);

    // update the status bar parts
    parts[3].second = StatusBar::FormatNumber(inputTokens);
    parts[4].second = StatusBar::FormatNumber(outputTokens);

    // render the text
    StatusBar::RenderText(hWnd);
    return 0;
}

void StatusBar::RenderText(HWND hWnd) {
    HWND hStatusBar = GetDlgItem(hWnd, IDC_STATUS_BAR);

    for (auto const& [partIdx, part] : StatusBar::parts | std::views::enumerate) {
        auto text = std::format("{}: {}", part.first, part.second);
        SendMessage(hStatusBar, SB_SETTEXT, partIdx, reinterpret_cast<LPARAM>(text.c_str()));
    }
}
