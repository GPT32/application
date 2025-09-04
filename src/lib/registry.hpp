#pragma once
#include <windows.h>

#include <string>

#include "resource.hpp"

/**
 * @namespace lib::registry
 * @brief Read and write application data to the Windows registry.
 */
namespace lib::registry {
    /** @brief Path to the registry key. */
    static const std::string RegistryPath = std::string("Software\\") + APP_NAME;

    /** @brief Reusable registry key handle. */
    static HKEY hKey = nullptr;

    /**
     * @brief Close the registry key handle.
     */
    void close();

    /**
     * @brief Open the registry key handle.
     * @returns bool True on success, false on failure.
     */
    bool open();

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
     * @brief Reads a 64-bit unsigned int from the registry.
     * @param subKey The registry sub key.
     * @param output The output value.
     */
    bool read(const std::string& subKey, DWORD64& output);

    /**
     * @brief Reads a 32-bit long from the registry.
     * @param subKey The registry sub key.
     * @param output The output value.
     */
    bool read(const std::string& subKey, long& output);

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

    /**
     * @brief Writes a 64-bit unsigned int to the registry.
     * @param subKey    The registry sub key.
     * @param input     The input value.
     */
    bool write(const std::string& subKey, DWORD64 input);

    /**
     * @brief Writes a 32-bit long to the registry.
     * @param subKey    The registry sub key.
     * @param input     The input value.
     */
    bool write(const std::string& subKey, long input);
}
