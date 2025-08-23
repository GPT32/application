#include "api.hpp"

namespace lib::api {
    lib::requests::Headers authenticate(const std::string& apiKey) {
        std::wstring apiKeyW = toWideString(apiKey);
        std::wstring authHeader = L"Bearer " + apiKeyW;
        lib::requests::Headers headers;
        headers[L"Authorization"] = authHeader;
        return headers;
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

    std::wstring toWideString(const std::string& str) {
        int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
        std::wstring wstr(size - 1, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr.data(), size);
        return wstr;
    }

    lib::requests::Response usageCompletions(const std::string& adminApiKey) {
        auto headers = lib::api::authenticate(adminApiKey);
        std::wstring url =
            lib::api::BASE_URL + L"/organization/usage/completions?start_time=1755896042&bucket_width=1d";
        return lib::requests::get(url.c_str(), headers);
    }
}
