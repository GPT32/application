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

    if (lib::registry::read(testKey, value)) {
        HKEY hKey;

        if (RegOpenKeyExA(HKEY_CURRENT_USER, lib::registry::RegistryPath.c_str(), 0, KEY_SET_VALUE, &hKey) ==
            ERROR_SUCCESS) {
            RegDeleteValueA(hKey, testKey.c_str());
            RegCloseKey(hKey);
        }
    }

    // read and write tests
    std::string loadedValue;
    assert(lib::registry::write(testKey, testValue));
    assert(lib::registry::read(testKey, loadedValue));
    assert(loadedValue == testValue);

    // overwrite a key
    const std::string newValue = "OverwrittenValue";
    assert(lib::registry::write(testKey, newValue));
    assert(lib::registry::read(testKey, loadedValue));
    assert(loadedValue == newValue);

    // clean up
    HKEY hKey;

    if (RegOpenKeyExA(HKEY_CURRENT_USER, lib::registry::RegistryPath.c_str(), 0, KEY_SET_VALUE, &hKey) ==
        ERROR_SUCCESS) {
        RegDeleteValueA(hKey, testKey.c_str());
        RegCloseKey(hKey);
    }

    return 0;
}
