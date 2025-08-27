#include "registry.hpp"

namespace lib::registry {
    bool read(const std::string& subKey, std::string& output) {
        HKEY hKey;

        if (RegOpenKeyEx(HKEY_CURRENT_USER, RegistryPath.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
            return false;
        }

        char buffer[1024];
        DWORD size = sizeof(buffer);
        DWORD type = REG_SZ;

        if (RegQueryValueEx(hKey, subKey.c_str(), nullptr, &type, reinterpret_cast<BYTE*>(buffer), &size) !=
            ERROR_SUCCESS) {
            return false;
        }

        output.assign(buffer, size - 1);
        RegCloseKey(hKey);
        return true;
    }

    bool read(const std::string& subKey, DWORD& output) {
        HKEY hKey;

        if (RegOpenKeyEx(HKEY_CURRENT_USER, RegistryPath.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
            return false;
        }

        DWORD size = sizeof(DWORD);
        DWORD type = REG_DWORD;

        if (RegQueryValueEx(hKey, subKey.c_str(), nullptr, &type, reinterpret_cast<BYTE*>(&output), &size) !=
            ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return false;
        }

        RegCloseKey(hKey);
        return true;
    }

    bool write(const std::string& subKey, const std::string& input) {
        HKEY hKey;

        if (RegCreateKeyEx(HKEY_CURRENT_USER,
                RegistryPath.c_str(),
                0,
                nullptr,
                0,
                KEY_WRITE,
                nullptr,
                &hKey,
                nullptr) != ERROR_SUCCESS) {
            return false;
        }

        if (RegSetValueEx(hKey,
                subKey.c_str(),
                0,
                REG_SZ,
                reinterpret_cast<const BYTE*>(input.c_str()),
                static_cast<DWORD>(input.size() + 1)) != ERROR_SUCCESS) {
            return false;
        }

        RegCloseKey(hKey);
        return true;
    }

    bool write(const std::string& subKey, DWORD input) {
        HKEY hKey;

        if (RegCreateKeyEx(HKEY_CURRENT_USER,
                RegistryPath.c_str(),
                0,
                nullptr,
                0,
                KEY_WRITE,
                nullptr,
                &hKey,
                nullptr) != ERROR_SUCCESS) {
            return false;
        }

        if (RegSetValueEx(hKey, subKey.c_str(), 0, REG_DWORD, reinterpret_cast<const BYTE*>(&input), sizeof(DWORD)) !=
            ERROR_SUCCESS) {
            return false;
        }

        RegCloseKey(hKey);
        return true;
    }
}
