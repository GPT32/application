#include "status-bar.hpp"

#include <CommCtrl.h>

#include <ranges>
#include <sstream>
#include <thread>

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

    // send non-blocking api request to load the stats data
    std::string adminApiKey;
    lib::settings::load(lib::settings::adminApiKey::Key, adminApiKey);

    std::jthread([=]() {
        // @todo
    }).detach();

    // @todo: move this to `Controller::onCreate`
    StatusBar::RenderText(hWnd);
    return hStatusBar;
}

void StatusBar::RenderText(HWND hWnd) {
    HWND hStatusBar = GetDlgItem(hWnd, IDC_STATUS_BAR);

    for (auto const& [partIdx, part] : StatusBar::parts | std::views::enumerate) {
        std::ostringstream oss;

        for (auto const& [fieldIdx, field] : part | std::views::enumerate) {
            oss << field.first << ": " << field.second;

            if (size_t(fieldIdx) < part.size() - 1) {
                oss << ", ";
            }
        }

        std::string text = oss.str();
        SendMessage(hStatusBar, SB_SETTEXT, partIdx, reinterpret_cast<LPARAM>(text.c_str()));
    }
}
