#pragma once
#include <windows.h>

#include <locale>
#include <string>
#include <vector>

/** @brief Status bar part. */
using StatusBarPart = std::pair<std::string, std::string>;

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
    std::vector<StatusBarPart> parts = {
        { "Monthly Spend", "0" },
        { "Monthly Input Tokens", "0" },
        { "Monthly Output Tokens", "0" },
        { "Session Input Tokens", "0" },
        { "Session Output Tokens", "0" },
    };

    /** @brief Reusable locale instance. */
    std::locale locale = std::locale("");

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

    /**
     * @brief `WM_USER_ADMIN_API_RESPONSE` Win32 message handler.
     * @param hWnd      Handle to the main window.
     * @param wParam    Additional message information.
     * @param lParam    Additional message information.
     * @return LRESULT Result of message processing.
     */
    LRESULT OnAdminApiResponse(HWND hWnd, WPARAM wParam, LPARAM lParam);

    /**
     * @brief `WM_USER_UPDATE_SESSION_USAGE` Win32 message handler.
     * @param hWnd      Handle to the main window.
     * @param wParam    Additional message information.
     * @param lParam    Additional message information.
     * @return LRESULT Result of message processing.
     */
    LRESULT OnUpdateSessionUsage(HWND hWnd, WPARAM wParam, LPARAM lParam);

   private:
    /** @brief Prevents external instantiation of the class. */
    StatusBar() = default;

    /**
     * @brief Formats number of cents to currency.
     * @param number The number to format.
     * @return string The formatted number.
     */
    std::string FormatCurrency(const uint32_t number);

    /**
     * @brief Formats a large number into human-readable string with suffixes.
     * @param number The number to shorten.
     * @return string The formatted number.
     */
    std::string FormatNumber(uint32_t number);

    /**
     * @brief Loads the async data from the backend.
     * @param hWnd Handle to the main window.
     */
    void Load(HWND hWnd);

    /**
     * @brief Renders the status bar parts text.
     * @param hWnd  Handle to the main window.
     */
    void RenderText(HWND hWnd);
};
