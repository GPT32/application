#pragma once
#include <windows.h>

#include <chrono>
#include <string>

/**
 * @namespace lib::settings::apiKey
 * @brief Loads and saves the api key to settings.
 */
namespace lib::settings::apiKey {
    /** @brief Registry subkey. */
    inline const char* const SubKey = "ApiKey";

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
    inline const char* const SubKey = "AdminApiKey";

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
    inline const char* const SubKey = "AlwaysOnTop";

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
    inline const char* const SubKey = "Model";

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
 * @namespace lib::settings::statLastChecked
 * @brief Loads and saves last checked timestamp.
 */
namespace lib::settings::statLastChecked {
    /** @brief Registry subkey. */
    inline const char* const SubKey = "StatLastChecked";

    /**
     * @brief Loads the setting from the registry.
     * @param output Reference to where the setting will be stored.
     */
    bool load(std::chrono::system_clock::time_point& output);

    /**
     * @brief Saves the setting to the registry.
     * @param input The setting to save.
     */
    bool save(const std::chrono::system_clock::time_point& input);
}

/**
 * @namespace lib::settings::statCost
 * @brief Loads and saves api usage cost.
 */
namespace lib::settings::statCost {
    /** @brief Registry subkey. */
    inline const char* const SubKey = "StatCost";

    /**
     * @brief Loads the setting from the registry.
     * @param output Reference to where the setting will be stored.
     */
    bool load(uint32_t& output);

    /**
     * @brief Saves the setting to the registry.
     * @param input The setting to save.
     */
    bool save(uint32_t input);
}

/**
 * @namespace lib::settings::statInputTokens
 * @brief Loads and saves api usage input tokens.
 */
namespace lib::settings::statInputTokens {
    /** @brief Registry subkey. */
    inline const char* const SubKey = "StatInputTokens";

    /**
     * @brief Loads the setting from the registry.
     * @param output Reference to where the setting will be stored.
     */
    bool load(uint32_t& output);

    /**
     * @brief Saves the setting to the registry.
     * @param input The setting to save.
     */
    bool save(uint32_t input);
}

/**
 * @namespace lib::settings::statOutputTokens
 * @brief Loads and saves api usage output tokens.
 */
namespace lib::settings::statOutputTokens {
    /** @brief Registry subkey. */
    inline const char* const SubKey = "StatOutputTokens";

    /**
     * @brief Loads the setting from the registry.
     * @param output Reference to where the setting will be stored.
     */
    bool load(uint32_t& output);

    /**
     * @brief Saves the setting to the registry.
     * @param input The setting to save.
     */
    bool save(uint32_t input);
}

/**
 * @namespace lib::settings::leftPaneWidth
 * @brief Tracks left pane width.
 */
namespace lib::settings::leftPaneWidth {
    /** @brief Registry subkey. */
    inline const char* const SubKey = "LeftPaneWidth";

    /**
     * @brief Loads the setting from the registry.
     * @param output Reference to where the setting will be stored.
     */
    bool load(int& output);

    /**
     * @brief Saves the setting to the registry.
     * @param input The setting to save.
     */
    bool save(int input);
}

/**
 * @namespace lib::settings::windowPosition
 * @brief Persists window positioning across restarts.
 */
namespace lib::settings::windowPosition {
    /** @brief Registry subkey. */
    inline const char* const SubKey = "WindowPosition";

    /**
     * @brief Loads the setting from the registry.
     * @param output Reference to where the setting will be stored.
     */
    bool load(RECT& output);

    /**
     * @brief Saves the setting to the registry.
     * @param input The setting to save.
     */
    bool save(RECT input);
}

/**
 * @namespace lib::settings::viewLeftPanel
 * @brief Left panel visibility toggle.
 */
namespace lib::settings::viewLeftPanel {
    /** @brief Registry subkey. */
    inline const char* const SubKey = "ViewLeftPanel";

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
