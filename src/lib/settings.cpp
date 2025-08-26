#include "settings.hpp"

namespace lib::settings {
    bool load(const std::string& subKey, std::string& output) {
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

    bool load(const std::string& subKey, DWORD& output) {
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

    bool save(const std::string& subKey, const std::string& input) {
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

    bool save(const std::string& subKey, DWORD input) {
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

namespace lib::settings::apiKey {
    bool load(std::string& output) {
        return lib::settings::load(Key, output);
    }
    bool save(std::string& input) {
        return lib::settings::save(Key, input);
    }
}

namespace lib::settings::adminApiKey {
    bool load(std::string& output) {
        return lib::settings::load(Key, output);
    }
    bool save(std::string& input) {
        return lib::settings::save(Key, input);
    }
}

namespace lib::settings::alwaysOnTop {
    bool load(bool& output) {
        DWORD value = 0;

        if (!lib::settings::load(Key, value)) {
            output = false;
            return false;
        }

        output = (value != 0);
        return true;
    }

    bool save(bool input) {
        return lib::settings::save(Key, input);
    }
}

namespace lib::settings::model {
    bool load(std::string& output) {
        return lib::settings::load(Key, output);
    }
    bool save(std::string& input) {
        return lib::settings::save(Key, input);
    }
}
