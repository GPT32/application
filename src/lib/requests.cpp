#include "requests.hpp"

namespace lib::requests {
    std::wstring extractHeader(HINTERNET hRequest, DWORD headerFlag) {
        DWORD size = 0;

        // get required buffer size
        WinHttpQueryHeaders(hRequest, headerFlag, WINHTTP_HEADER_NAME_BY_INDEX, nullptr, &size, nullptr);

        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER || size == 0) {
            return {};
        }

        // now fetch the header
        std::wstring buffer(size / sizeof(wchar_t), 0);

        if (WinHttpQueryHeaders(hRequest, headerFlag, WINHTTP_HEADER_NAME_BY_INDEX, buffer.data(), &size, nullptr)) {
            buffer.resize(wcslen(buffer.c_str()));
            return buffer;
        }

        return {};
    }

    DWORD extractStatusCode(HINTERNET hRequest) {
        DWORD statusCode = 0;
        DWORD size = sizeof(statusCode);

        WinHttpQueryHeaders(hRequest,
            WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
            WINHTTP_HEADER_NAME_BY_INDEX,
            &statusCode,
            &size,
            nullptr);

        return statusCode;
    }

    Response get(const wchar_t* url, const Headers& headers) {
        auto r = request(L"GET", url, headers, std::nullopt);
        return r;
    }

    Response post(const wchar_t* url, const Headers& headers, const std::optional<nlohmann::json>& body) {
        auto r = request(L"POST", url, headers, body);
        return r;
    }

    Response request(
        const wchar_t* method, const wchar_t* url, const Headers& headers, const std::optional<nlohmann::json>& body) {
        // grab url components
        URL_COMPONENTSW urlComp = {};
        wchar_t host[256];
        wchar_t path[2048];
        urlComp.dwStructSize = sizeof(urlComp);
        urlComp.lpszHostName = host;
        urlComp.dwHostNameLength = std::size(host);
        urlComp.lpszUrlPath = path;
        urlComp.dwUrlPathLength = std::size(path);

        if (!WinHttpCrackUrl(url, 0, 0, &urlComp)) {
            return {};
        }

        // initialize and connect to http session
        HINTERNET hSession = WinHttpOpen(L"api/1.0",
            WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
            WINHTTP_NO_PROXY_NAME,
            WINHTTP_NO_PROXY_BYPASS,
            0);

        if (!hSession) {
            return {};
        }

        HINTERNET hConnect = WinHttpConnect(hSession, urlComp.lpszHostName, INTERNET_DEFAULT_HTTPS_PORT, 0);

        if (!hConnect) {
            return {};
        }

        // create the request and configure headers
        HINTERNET hRequest = WinHttpOpenRequest(hConnect,
            method,
            urlComp.lpszUrlPath,
            nullptr,
            WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            WINHTTP_FLAG_SECURE);

        if (!hRequest) {
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            return {};
        }

        for (const auto& [key, value] : headers) {
            std::wstring header = key + L": " + value;
            WinHttpAddRequestHeaders(hRequest,
                header.c_str(),
                static_cast<DWORD>(-1L),
                WINHTTP_ADDREQ_FLAG_ADD | WINHTTP_ADDREQ_FLAG_REPLACE);
        }

        // send the request
        std::string body_str;

        if (body.has_value()) {
            body_str = body->dump();
        }

        bool bResult = WinHttpSendRequest(hRequest,
            WINHTTP_NO_ADDITIONAL_HEADERS,
            -1,
            body_str.empty() ? nullptr : const_cast<LPVOID>(static_cast<const void*>(body_str.data())),
            static_cast<DWORD>(body_str.size()),
            static_cast<DWORD>(body_str.size()),
            0);

        if (!bResult || !WinHttpReceiveResponse(hRequest, nullptr)) {
            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            return {};
        }

        // keep checking for data until nothing left
        std::string response;

        do {
            DWORD bytesAvailable = 0;

            if (!WinHttpQueryDataAvailable(hRequest, &bytesAvailable) || bytesAvailable == 0) {
                break;
            }

            std::string buffer(bytesAvailable, 0);
            DWORD bytesRead = 0;

            if (WinHttpReadData(hRequest, buffer.data(), bytesAvailable, &bytesRead)) {
                response.append(buffer, 0, bytesRead);
            }
        } while (true);

        // build the response
        Response r;
        r.code = static_cast<int>(extractStatusCode(hRequest));
        r.headers.emplace(L"Content-Type", extractHeader(hRequest, WINHTTP_QUERY_CONTENT_TYPE));

        if (r.headers[L"Content-Type"].contains(L"application/json")) {
            try {
                r.body = nlohmann::json::parse(response);
            } catch (const std::exception& e) {
                r.error = e.what();
            }
        }

        // return the response
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return r;
    }
}
