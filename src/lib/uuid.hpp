#pragma once
#include <windows.h>

#include <string>

/**
 * @namespace lib::uuid
 * @brief Wrapper for Win32 API's UUID functions.
 */
namespace lib::uuid {
    /**
     * @brief Generates a UUIDv4 string.
     */
    inline std::string generate() {
        GUID guid;
        RPC_CSTR guidStr;
        HRESULT hr = CoCreateGuid(&guid);
        std::string result = "";

        if (hr == S_OK && UuidToStringA(&guid, &guidStr) == RPC_S_OK) {
            result = reinterpret_cast<char*>(guidStr);
            RpcStringFreeA(&guidStr);
        }

        return result;
    }
}
