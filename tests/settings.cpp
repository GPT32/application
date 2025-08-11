#include "lib/settings.hpp"

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

    if (lib::settings::load(testKey, value)) {
        HKEY hKey;

        if (RegOpenKeyExA(HKEY_CURRENT_USER, lib::settings::RegistryPath.c_str(), 0, KEY_SET_VALUE, &hKey) ==
            ERROR_SUCCESS) {
            RegDeleteValueA(hKey, testKey.c_str());
            RegCloseKey(hKey);
        }
    }

    // save and load tests
    std::string loadedValue;
    assert(lib::settings::save(testKey, testValue));
    assert(lib::settings::load(testKey, loadedValue));
    assert(loadedValue == testValue);

    // overwrite a key
    const std::string newValue = "OverwrittenValue";
    assert(lib::settings::save(testKey, newValue));
    assert(lib::settings::load(testKey, loadedValue));
    assert(loadedValue == newValue);

    // clean up
    HKEY hKey;

    if (RegOpenKeyExA(HKEY_CURRENT_USER, lib::settings::RegistryPath.c_str(), 0, KEY_SET_VALUE, &hKey) ==
        ERROR_SUCCESS) {
        RegDeleteValueA(hKey, testKey.c_str());
        RegCloseKey(hKey);
    }

    return 0;
}
