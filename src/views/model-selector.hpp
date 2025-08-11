#pragma once
#include <windows.h>

/**
 * @class ModelSelector
 * @brief Model selection combobox.
 */
class ModelSelector {
   public:
    /**
     * @brief Gets the singleton instance.
     * @return Reference to the singleton instance.
     */
    static ModelSelector& Instance();

    /**
     * @brief List of supported models.
     */
    const char* models[5] = {
        "gpt-5",
        "gpt-4.1-mini",
        "gpt-4.1",
        "gpt-4",
        "gpt-3.5-turbo",
    };

    /**
     * @brief Creates the control.
     * @param hWnd Handle to the window.
     * @return HWND Handle to the newly created window.
     */
    HWND CreateControl(HWND hWnd);

    /**
     * @brief `WM_COMMAND` Win32 message handler.
     * @param hWnd      Handle to the main window.
     * @param wParam    Additional message information.
     * @return LRESULT Result of message processing.
     */
    LRESULT OnCommand(HWND hWnd, WPARAM wParam);

   private:
    /** @brief Prevents external instantiation of the class. */
    ModelSelector() = default;
};
