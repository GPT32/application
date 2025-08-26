#include "api.hpp"

namespace lib::api {
    lib::requests::Headers authenticate(const std::string& apiKey) {
        std::wstring apiKeyW = toWideString(apiKey);
        std::wstring authHeader = L"Bearer " + apiKeyW;
        lib::requests::Headers headers;
        headers[L"Authorization"] = authHeader;
        return headers;
    }

    lib::requests::Response cost(const std::string& adminApiKey, const time_t startTime) {
        std::wstring url = lib::api::BASE_URL + L"/organization/costs?bucket_width=1d&limit=31&start_time=" +
                           std::to_wstring(startTime);
        return paginate(adminApiKey, url);
    }

    lib::requests::Response createResponse(const std::string& apiKey,
        const std::string& input,
        const std::string& model,
        const std::string prevId,
        const std::string instructions) {
        // build url and input data
        auto headers = lib::api::authenticate(apiKey);
        std::wstring url = lib::api::BASE_URL + L"/responses";
        nlohmann::json data = { { "model", model }, { "input", input } };

        // append previous response id
        if (!prevId.empty()) {
            data["previous_response_id"] = prevId;
        }

        // set tone or role
        if (!instructions.empty()) {
            data["instructions"] = instructions;
        }

        return lib::requests::post(url.c_str(), headers, data);
    }

    lib::requests::Response listModels(const std::string& apiKey) {
        auto headers = lib::api::authenticate(apiKey);
        std::wstring url = lib::api::BASE_URL + L"/models";
        return lib::requests::get(url.c_str(), headers);
    }

    lib::requests::Response paginate(const std::string& adminApiKey, const std::wstring& baseUrl) {
        auto headers = lib::api::authenticate(adminApiKey);

        // query the endpoint until no more pages are remaining
        // and aggregate the results into a single vector
        std::vector<nlohmann::json> data;
        std::wstring nextPage;
        bool hasMorePages = true;

        while (hasMorePages) {
            std::wstring url = baseUrl;

            if (!nextPage.empty()) {
                url += L"&page=" + nextPage;
            }

            auto r = lib::requests::get(url.c_str(), headers);

            // @todo: error handling
            if (r.body.contains("data") && r.body["data"].is_array()) {
                for (auto& bucket : r.body["data"]) {
                    data.push_back(bucket);
                }
            }

            hasMorePages = r.body.value("has_more", false);

            if (hasMorePages) {
                std::string nextPageStr = r.body.value("next_page", "");
                nextPage = toWideString(nextPageStr);
                Sleep(500);
            }
        }

        // build the response manually and inject
        // the aggregated results into the body
        lib::requests::Response r;
        r.code = HTTP_STATUS_OK;
        r.body = data;
        return r;
    }

    std::wstring toWideString(const std::string& str) {
        int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
        std::wstring wstr(size - 1, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr.data(), size);
        return wstr;
    }

    lib::requests::Response usage(const std::string& adminApiKey, const time_t startTime) {
        std::wstring url = lib::api::BASE_URL +
                           L"/organization/usage/completions?bucket_width=1d&limit=31&start_time=" +
                           std::to_wstring(startTime);
        return paginate(adminApiKey, url);
    }
}
