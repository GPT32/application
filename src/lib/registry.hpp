#pragma once
#include <windows.h>

#include <string>

#include "resource.hpp"

namespace lib {
    /**
     * @class Registry
     * @brief Windows registry wrapper.
     */
    class Registry {
       public:
        /** @brief Registry key. */
        inline static const std::string Key = std::string("Software\\") + APP_NAME;

        /** @brief Registry key handle. */
        inline static HKEY hKey;

        /**
         * @brief Gets the singleton instance.
         * @return Reference to the singleton instance.
         */
        static Registry& Instance();

        /** @brief Cleans up the handle when this class is destroyed. */
        ~Registry();

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

       private:
        /** @brief Prevents external instantiation of the class. */
        Registry() = default;
    };
}
