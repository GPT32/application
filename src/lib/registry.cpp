#include "registry.hpp"

lib::Registry& lib::Registry::Instance() {
    static lib::Registry instance;
    return instance;
}

lib::Registry::~Registry() {
    lib::Registry::Instance().close();
}

void lib::Registry::close() {
    if (!lib::Registry::Instance().hKey) {
        return;
    }

    RegCloseKey(lib::Registry::Instance().hKey);
    lib::Registry::Instance().hKey = nullptr;
}

bool lib::Registry::open() {
    if (lib::Registry::Instance().hKey) {
        return true;
    }

    if (RegOpenKeyEx(HKEY_CURRENT_USER,
            lib::Registry::Path.c_str(),
            0,
            KEY_READ | KEY_WRITE,
            &lib::Registry::Instance().hKey) == ERROR_SUCCESS) {
        return true;
    }

    if (RegCreateKeyEx(HKEY_CURRENT_USER,
            lib::Registry::Path.c_str(),
            0,
            nullptr,
            0,
            KEY_WRITE,
            nullptr,
            &lib::Registry::Instance().hKey,
            nullptr) != ERROR_SUCCESS) {
        return false;
    }

    return true;
}

bool lib::Registry::read(const std::string& path, std::string& output) {
    if (!lib::Registry::Instance().open()) {
        return false;
    }

    char buffer[1024];
    DWORD size = sizeof(buffer);
    DWORD type = REG_SZ;

    if (RegQueryValueEx(lib::Registry::Instance().hKey,
            path.c_str(),
            nullptr,
            &type,
            reinterpret_cast<BYTE*>(buffer),
            &size) != ERROR_SUCCESS) {
        return false;
    }

    output.assign(buffer, size - 1);
    return true;
}

bool lib::Registry::read(const std::string& path, DWORD& output) {
    if (!lib::Registry::Instance().open()) {
        return false;
    }

    DWORD size = sizeof(DWORD);
    DWORD type = REG_DWORD;

    if (RegQueryValueEx(lib::Registry::Instance().hKey,
            path.c_str(),
            nullptr,
            &type,
            reinterpret_cast<BYTE*>(&output),
            &size) != ERROR_SUCCESS) {
        return false;
    }

    return true;
}

bool lib::Registry::read(const std::string& path, long& output) {
    if (!lib::Registry::Instance().open()) {
        return false;
    }

    DWORD size = sizeof(DWORD);
    DWORD type = REG_DWORD;

    if (RegQueryValueEx(lib::Registry::Instance().hKey,
            path.c_str(),
            nullptr,
            &type,
            reinterpret_cast<BYTE*>(&output),
            &size) != ERROR_SUCCESS) {
        return false;
    }

    return true;
}

bool lib::Registry::read(const std::string& path, DWORD64& output) {
    if (!lib::Registry::Instance().open()) {
        return false;
    }

    DWORD size = sizeof(DWORD64);
    DWORD type = REG_QWORD;

    if (RegQueryValueEx(lib::Registry::Instance().hKey,
            path.c_str(),
            nullptr,
            &type,
            reinterpret_cast<BYTE*>(&output),
            &size) != ERROR_SUCCESS) {
        return false;
    }

    return true;
}

bool lib::Registry::write(const std::string& path, const std::string& input) {
    if (!lib::Registry::Instance().open()) {
        return false;
    }

    if (RegSetValueEx(lib::Registry::Instance().hKey,
            path.c_str(),
            0,
            REG_SZ,
            reinterpret_cast<const BYTE*>(input.c_str()),
            static_cast<DWORD>(input.size() + 1)) != ERROR_SUCCESS) {
        return false;
    }

    return true;
}

bool lib::Registry::write(const std::string& path, DWORD input) {
    if (!lib::Registry::Instance().open()) {
        return false;
    }

    if (RegSetValueEx(lib::Registry::Instance().hKey,
            path.c_str(),
            0,
            REG_DWORD,
            reinterpret_cast<const BYTE*>(&input),
            sizeof(DWORD)) != ERROR_SUCCESS) {
        return false;
    }

    return true;
}

bool lib::Registry::write(const std::string& path, DWORD64 input) {
    if (!lib::Registry::Instance().open()) {
        return false;
    }

    if (RegSetValueEx(lib::Registry::Instance().hKey,
            path.c_str(),
            0,
            REG_QWORD,
            reinterpret_cast<const BYTE*>(&input),
            sizeof(DWORD64)) != ERROR_SUCCESS) {
        return false;
    }

    return true;
}

bool lib::Registry::write(const std::string& path, long input) {
    if (!lib::Registry::Instance().open()) {
        return false;
    }

    if (RegSetValueEx(lib::Registry::Instance().hKey,
            path.c_str(),
            0,
            REG_DWORD,
            reinterpret_cast<const BYTE*>(&input),
            sizeof(DWORD)) != ERROR_SUCCESS) {
        return false;
    }

    return true;
}
