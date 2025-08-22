#pragma once
#include <windows.h>

/**
 * @class View
 * @brief Manages the main application view.
 */
class View {
   public:
    /**
     * @brief Gets the singleton instance.
     * @return Reference to the singleton instance.
     */
    static View& Instance();

    /**
     * @brief Process messages sent to the main window.
     * @param hWnd     Handle to the window receiving the message.
     * @param message  The Win32 message identifier.
     * @param wParam   Additional message information.
     * @param lParam   Additional message information.
     * @return LRESULT Result of message processing.
     */
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    /**
     * @brief Centers the window.
     * @param hWnd Handle to the window.
     * @return LRESULT Result of message processing.
     */
    LRESULT CenterWindow(HWND hWnd);

    /**
     * @brief Creates controls within the window.
     * @param hWnd Handle to the window.
     * @return LRESULT Result of message processing.
     */
    LRESULT CreateControls(HWND hWnd);

    /**
     * @brief Registers and creates the window.
     * @param hInstance Handle to the current application instance.
     * @return HWND Handle to the newly created window.
     */
    HWND CreateMainWindow(HINSTANCE hInstance);

    /**
     * @brief Arranges controls within the window.
     * @param hWnd      Handle to the window.
     * @param lParam    Additional message information.
     * @return LRESULT Result of message processing.
     */
    LRESULT LayoutControls(HWND hWnd, LPARAM lParam);

    /**
     * @brief Applies COMCTRL32v6 theming to classic controls.
     * @param hWnd Handle to the window.
     * @return LRESULT Result of message processing.
     */
    LRESULT ThemeControls(HWND hWnd);

   private:
    /** @brief Prevents external instantiation of the class. */
    View() = default;
};
