#include "registry.hpp"

namespace lib::registry {
    void close() {
        if (!lib::registry::hKey) {
            return;
        }

        RegCloseKey(lib::registry::hKey);
        lib::registry::hKey = nullptr;
    }

    bool open() {
        if (lib::registry::hKey) {
            return true;
        }

        if (RegOpenKeyEx(HKEY_CURRENT_USER, RegistryPath.c_str(), 0, KEY_READ | KEY_WRITE, &lib::registry::hKey) ==
            ERROR_SUCCESS) {
            return true;
        }

        if (RegCreateKeyEx(HKEY_CURRENT_USER,
                RegistryPath.c_str(),
                0,
                nullptr,
                0,
                KEY_WRITE,
                nullptr,
                &lib::registry::hKey,
                nullptr) != ERROR_SUCCESS) {
            return false;
        }

        return true;
    }

    bool read(const std::string& subKey, std::string& output) {
        if (!lib::registry::open()) {
            return false;
        }

        char buffer[1024];
        DWORD size = sizeof(buffer);
        DWORD type = REG_SZ;

        if (RegQueryValueEx(lib::registry::hKey,
                subKey.c_str(),
                nullptr,
                &type,
                reinterpret_cast<BYTE*>(buffer),
                &size) != ERROR_SUCCESS) {
            return false;
        }

        output.assign(buffer, size - 1);
        lib::registry::close();
        return true;
    }

    bool read(const std::string& subKey, DWORD& output) {
        if (!lib::registry::open()) {
            return false;
        }

        DWORD size = sizeof(DWORD);
        DWORD type = REG_DWORD;

        if (RegQueryValueEx(lib::registry::hKey,
                subKey.c_str(),
                nullptr,
                &type,
                reinterpret_cast<BYTE*>(&output),
                &size) != ERROR_SUCCESS) {
            lib::registry::close();
            return false;
        }

        lib::registry::close();
        return true;
    }

    bool read(const std::string& subKey, long& output) {
        if (!lib::registry::open()) {
            return false;
        }

        DWORD size = sizeof(DWORD);
        DWORD type = REG_DWORD;

        if (RegQueryValueEx(lib::registry::hKey,
                subKey.c_str(),
                nullptr,
                &type,
                reinterpret_cast<BYTE*>(&output),
                &size) != ERROR_SUCCESS) {
            lib::registry::close();
            return false;
        }

        // @todo: convert to class and close on cleanup
        // lib::registry::close();
        return true;
    }

    bool read(const std::string& subKey, DWORD64& output) {
        if (!lib::registry::open()) {
            return false;
        }

        DWORD size = sizeof(DWORD64);
        DWORD type = REG_QWORD;

        if (RegQueryValueEx(lib::registry::hKey,
                subKey.c_str(),
                nullptr,
                &type,
                reinterpret_cast<BYTE*>(&output),
                &size) != ERROR_SUCCESS) {
            lib::registry::close();
            return false;
        }

        lib::registry::close();
        return true;
    }

    bool write(const std::string& subKey, const std::string& input) {
        if (!lib::registry::open()) {
            return false;
        }

        if (RegSetValueEx(lib::registry::hKey,
                subKey.c_str(),
                0,
                REG_SZ,
                reinterpret_cast<const BYTE*>(input.c_str()),
                static_cast<DWORD>(input.size() + 1)) != ERROR_SUCCESS) {
            return false;
        }

        lib::registry::close();
        return true;
    }

    bool write(const std::string& subKey, DWORD input) {
        if (!lib::registry::open()) {
            return false;
        }

        if (RegSetValueEx(lib::registry::hKey,
                subKey.c_str(),
                0,
                REG_DWORD,
                reinterpret_cast<const BYTE*>(&input),
                sizeof(DWORD)) != ERROR_SUCCESS) {
            return false;
        }

        lib::registry::close();
        return true;
    }

    bool write(const std::string& subKey, DWORD64 input) {
        if (!lib::registry::open()) {
            return false;
        }

        if (RegSetValueEx(lib::registry::hKey,
                subKey.c_str(),
                0,
                REG_QWORD,
                reinterpret_cast<const BYTE*>(&input),
                sizeof(DWORD64)) != ERROR_SUCCESS) {
            return false;
        }

        lib::registry::close();
        return true;
    }

    bool write(const std::string& subKey, long input) {
        if (!lib::registry::open()) {
            return false;
        }

        if (RegSetValueEx(lib::registry::hKey,
                subKey.c_str(),
                0,
                REG_DWORD,
                reinterpret_cast<const BYTE*>(&input),
                sizeof(DWORD)) != ERROR_SUCCESS) {
            return false;
        }

        // @todo: convert to class and close on cleanup
        // lib::registry::close();

        return true;
    }
}
