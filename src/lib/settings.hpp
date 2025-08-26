#pragma once
#include <windows.h>

#include <string>

#include "resource.hpp"

/**
 * @namespace lib::settings
 * @brief Loads and saves application settings.
 */
namespace lib::settings {
    /** @brief Path to the application registry key. */
    inline const std::string RegistryPath = std::string("Software\\") + APP_NAME;

    /**
     * @brief Loads a string from the registry.
     * @param subKey The registry sub key.
     * @param output The output value.
     */
    bool load(const std::string subKey, std::string& output);

    /**
     * @brief Loads a 32-bit unsigned int from the registry.
     * @param subKey The registry sub key.
     * @param output The output value.
     */
    bool load(const std::string subKey, DWORD& output);

    /**
     * @brief Saves a string to the registry.
     * @param subKey    The registry sub key.
     * @param input     The input value.
     */
    bool save(const std::string subKey, const std::string& input);

    /**
     * @brief Saves a 32-bit unsigned int to the registry.
     * @param subKey    The registry sub key.
     * @param input     The input value.
     */
    bool save(const std::string& subKey, DWORD input);
}

/**
 * @namespace lib::settings::apiKey
 * @brief Loads and saves the api key to settings.
 */
namespace lib::settings::apiKey {
    /** @brief API key registry sub key. */
    inline const char* const Key = "ApiKey";

    /**
     * @brief Loads the setting from the registry.
     * @param output Reference to where the setting will be stored.
     */
    bool load(std::string& output);

    /**
     * @brief Saves the setting to the registry.
     * @param input The setting to save.
     */
    bool save(std::string& input);
}

/**
 * @namespace lib::settings::adminApiKey
 * @brief Loads and saves the admin api key to settings.
 */
namespace lib::settings::adminApiKey {
    /** @brief Admin API key registry sub key. */
    inline const char* const Key = "AdminApiKey";

    /**
     * @brief Loads the setting from the registry.
     * @param output Reference to where the setting will be stored.
     */
    bool load(std::string& output);

    /**
     * @brief Saves the setting to the registry.
     * @param input The setting to save.
     */
    bool save(std::string& input);
}

/**
 * @namespace lib::settings::alwaysOnTop
 * @brief Loads and saves always on top settings.
 */
namespace lib::settings::alwaysOnTop {
    /** @brief Always on top registry sub key. */
    inline const char* const Key = "AlwaysOnTop";

    /**
     * @brief Loads the setting from the registry.
     * @param output Reference to where the setting will be stored.
     */
    bool load(bool& output);

    /**
     * @brief Saves the setting to the registry.
     * @param input The setting to save.
     */
    bool save(bool input);
}

/**
 * @namespace lib::settings::model
 * @brief The selected ai model.
 */
namespace lib::settings::model {
    /** @brief Model registry sub key. */
    inline const char* const Key = "Model";

    /**
     * @brief Loads setting from the registry.
     * @param output Reference to where the setting will be stored.
     */
    bool load(std::string& output);

    /**
     * @brief Saves setting to the registry.
     * @param input The setting to save.
     */
    bool save(std::string& input);
}
