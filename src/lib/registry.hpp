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
        /** @brief Registry key path. */
        inline static const std::string Path = std::string("Software\\") + APP_NAME;

        /** @brief Registry key handle. */
        inline static HKEY hKey;

        /**
         * @brief Gets the singleton instance.
         * @return Reference to the singleton instance.
         */
        static Registry& Instance();

        /** @brief Cleans up the handle when this class is destroyed. */
        ~Registry();

        /** @brief Close the registry key handle. */
        void close();

        /**
         * @brief Open the registry key handle.
         * @return bool Successful operation.
         */
        bool open();

        /**
         * @brief Reads a string from the registry.
         * @param path      The path to read from.
         * @param output    The output value.
         * @return bool Successful operation.
         */
        bool read(const std::string& path, std::string& output);

        /**
         * @brief Reads a 32-bit unsigned int from the registry.
         * @param path      The path to read from.
         * @param output    The output value.
         * @return bool Successful operation.
         */
        bool read(const std::string& path, DWORD& output);

        /**
         * @brief Reads a 64-bit unsigned int from the registry.
         * @param path      The path to read from.
         * @param output    The output value.
         * @return bool Successful operation.
         */
        bool read(const std::string& path, DWORD64& output);

        /**
         * @brief Reads a 32-bit long from the registry.
         * @param path      The path to read from.
         * @param output    The output value.
         * @return bool Successful operation.
         */
        bool read(const std::string& path, long& output);

        /**
         * @brief Writes a string to the registry.
         * @param path  The path to write to.
         * @param input The input value.
         * @return bool Successful operation.
         */
        bool write(const std::string& path, const std::string& input);

        /**
         * @brief Writes a 32-bit unsigned int to the registry.
         * @param path  The path to write to.
         * @param input The input value.
         * @return bool Successful operation.
         */
        bool write(const std::string& path, DWORD input);

        /**
         * @brief Writes a 64-bit unsigned int to the registry.
         * @param path  The path to write to.
         * @param input The input value.
         * @return bool Successful operation.
         */
        bool write(const std::string& path, DWORD64 input);

        /**
         * @brief Writes a 32-bit long to the registry.
         * @param path  The path to write to.
         * @param input The input value.
         * @return bool Successful operation.
         */
        bool write(const std::string& path, long input);

       private:
        /** @brief Prevents external instantiation of the class. */
        Registry() = default;
    };
}
