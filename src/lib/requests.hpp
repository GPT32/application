#pragma once
#include <windows.h>

#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <winhttp.h>

/**
 * @namespace lib::requests
 * @brief Wrapper for WinHTTP.
 */
namespace lib::requests {
    /** @brief HTTP headers alias. */
    using Headers = std::unordered_map<std::wstring, std::wstring>;

    /** @brief HTTP response. */
    struct Response {
        int code;
        Headers headers;
        nlohmann::json body;
        std::optional<std::string> error;
    };

    /**
     * @brief Extracts the specified header from the request object.
     * @param hRequest  The request handler.
     * @param flag      The header flag.
     * @return std::wstring The header content.
     */
    std::wstring extractHeader(HINTERNET hRequest, DWORD headerFlag);

    /**
     * @brief Extracts the status code from the request object.
     * @param hRequest The request handler.
     * @return DWORD The status code.
     */
    DWORD extractStatusCode(HINTERNET hRequest);

    /**
     * @brief Sends an HTTP GET request.
     * @param url       The URL.
     * @param headers   HTTP headers.
     * @return Response The response object.
     */
    Response get(const wchar_t* url, const Headers& headers = {});

    /**
     * @brief Sends an HTTP POST request.
     * @param url       The URL.
     * @param headers   HTTP headers.
     * @param body      JSON body.
     * @return Response The response object.
     */
    Response post(const wchar_t* url, const Headers& headers, const std::optional<nlohmann::json>& body);

    /**
     * @brief Generic request handler.
     * @param method    The HTTP method.
     * @param url       The URL.
     * @param headers   HTTP headers.
     * @param body      JSON body.
     * @return Response The response object.
     */
    Response request(
        const wchar_t* method, const wchar_t* url, const Headers& headers, const std::optional<nlohmann::json>& body);
}
