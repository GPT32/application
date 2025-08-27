#pragma once
#include <windows.h>

#include <string>

#include "resource.hpp"

/**
 * @namespace lib::registry
 * @brief Read and write data to the Windows registry.
 */
namespace lib::registry {
    /** @brief Path to the application registry key. */
    inline const std::string RegistryPath = std::string("Software\\") + APP_NAME;

    /**
     * @brief Reads a string from the registry.
     * @param subKey The registry sub key.
     * @param output The output value.
     */
    bool read(const std::string& subKey, std::string& output);

    /**
     * @brief Reads a 32-bit unsigned int from the registry.
     * @param subKey The registry sub key.
     * @param output The output value.
     */
    bool read(const std::string& subKey, DWORD& output);

    /**
     * @brief Writes a string to the registry.
     * @param subKey    The registry sub key.
     * @param input     The input value.
     */
    bool write(const std::string& subKey, const std::string& input);

    /**
     * @brief Writes a 32-bit unsigned int to the registry.
     * @param subKey    The registry sub key.
     * @param input     The input value.
     */
    bool write(const std::string& subKey, DWORD input);
}
