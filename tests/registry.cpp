#include "lib/registry.hpp"

#include <cassert>
#include <string>

/**
 * @brief Test suite entrypoint.
 * @return int Exit code (0 if assertion passes)
 */
int main() {
    const std::string testKey = "UnitTestKey";
    const std::string testValue = "TestValue123";

    // delete the test key/value pair if they already exist
    std::string value;

    if (lib::Registry::Instance().read(testKey, value)) {
        HKEY hKey;

        if (RegOpenKeyExA(HKEY_CURRENT_USER, lib::Registry::Key.c_str(), 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
            RegDeleteValueA(hKey, testKey.c_str());
            RegCloseKey(hKey);
        }
    }

    // read and write tests
    std::string loadedValue;
    assert(lib::Registry::Instance().write(testKey, testValue));
    assert(lib::Registry::Instance().read(testKey, loadedValue));
    assert(loadedValue == testValue);

    // overwrite a key
    const std::string newValue = "OverwrittenValue";
    assert(lib::Registry::Instance().write(testKey, newValue));
    assert(lib::Registry::Instance().read(testKey, loadedValue));
    assert(loadedValue == newValue);

    // uint64 test
    uint64_t iNewValue = 1337;
    uint64_t iLoadedValue;
    assert(lib::Registry::Instance().write(testKey, iNewValue));
    assert(lib::Registry::Instance().read(testKey, iLoadedValue));

    // clean up
    HKEY hKey;

    if (RegOpenKeyExA(HKEY_CURRENT_USER, lib::Registry::Key.c_str(), 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        RegDeleteValueA(hKey, testKey.c_str());
        RegCloseKey(hKey);
    }

    return 0;
}
