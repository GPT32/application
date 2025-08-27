#pragma once
#include <string>

/**
 * @namespace lib::settings::apiKey
 * @brief Loads and saves the api key to settings.
 */
namespace lib::settings::apiKey {
    /** @brief Registry subkey. */
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
    /** @brief Registry subkey. */
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
    /** @brief Registry subkey. */
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
    /** @brief Registry subkey. */
    inline const char* const Key = "Model";

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
