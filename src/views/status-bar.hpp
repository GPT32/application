#pragma once
#include <windows.h>

#include <string>
#include <vector>

/** @brief Status bar key/value pair. */
using StatusBarField = std::pair<std::string, std::string>;

/** @brief Status bar part. */
using StatusBarPart = std::vector<StatusBarField>;

/**
 * @class StatusBar
 * @brief Wrapper for the Win32 status bar control.
 */
class StatusBar {
   public:
    /**
     * @brief Gets the singleton instance.
     * @return Reference to the singleton instance.
     */
    static StatusBar& Instance();

    /** @brief The status bar parts. */
    std::vector<StatusBarPart> parts = { { { "Total Spend", "0" }, { "Requests", "0" }, { "Tokens", "0" } },
        { { "Today Spend", "0" }, { "Requests", "0" }, { "Tokens", "0" } },
        { { "Input", "0" }, { "Output", "0" } } };

    /**
     * @brief Configures the status bar parts.
     * @param hWnd Handle to the window.
     * @return LRESULT Result of message processing.
     */
    LRESULT ConfigureParts(HWND hWnd);

    /**
     * @brief Creates the control.
     * @param hWnd Handle to the window.
     * @return HWND Handle to the newly created window.
     */
    HWND CreateControl(HWND hWnd);

   private:
    /** @brief Prevents external instantiation of the class. */
    StatusBar() = default;

    /**
     * @brief Renders the status bar parts text.
     * @param hWnd  Handle to the main window.
     */
    void RenderText(HWND hWnd);
};
