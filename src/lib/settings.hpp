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
     * @brief Loads a string setting from the registry.
     * @param subKey The registry sub key.
     * @param output The output string.
     */
    bool load(const std::string subKey, std::string& output);

    /**
     * @brief Loads a boolean from the registry.
     * @param subKey The registry sub key.
     * @param output The output boolean.
     */
    bool load(const std::string subKey, DWORD& output);

    /**
     * @brief Saves a string setting to the registry.
     * @param subKey    The registry sub key.
     * @param value     The content to save.
     */
    bool save(const std::string subKey, const std::string& value);

    /**
     * @brief Saves a boolean setting to the registry.
     * @param subKey    The registry sub key.
     * @param value     The content to save.
     */
    bool save(const std::string& subKey, DWORD value);
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
     * @param apiKey Reference to where the setting will be stored.
     */
    bool load(std::string& apiKey);

    /**
     * @brief Saves the setting to the registry.
     * @param apiKey The setting to save.
     */
    bool save(std::string& apiKey);
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
     * @param adminApiKey Reference to where the setting will be stored.
     */
    bool load(std::string& adminApiKey);

    /**
     * @brief Saves the setting to the registry.
     * @param adminApiKey The setting to save.
     */
    bool save(std::string& adminApiKey);
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
     * @param alwaysOnTop Reference to where the setting will be stored.
     */
    bool load(bool& alwaysOnTop);

    /**
     * @brief Saves the setting to the registry.
     * @param alwaysOnTop The setting to save.
     */
    bool save(bool alwaysOnTop);
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
     * @param model Reference to where the setting will be stored.
     */
    bool load(std::string& model);

    /**
     * @brief Saves setting to the registry.
     * @param model The setting to save.
     */
    bool save(std::string& model);
}
