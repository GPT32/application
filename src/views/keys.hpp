#pragma once
#include <windows.h>

#include <string>

/**
 * @class Keys
 * @brief API and Admin API keys dialog box.
 */
class Keys {
   public:
    /**
     * @brief Gets the singleton instance.
     * @return Reference to the singleton instance.
     */
    static Keys& Instance();

    /**
     * @brief Processes messages sent to the dialog box.
     * @param hDlg     Handle to the dialog box receiving the message.
     * @param message  The Win32 message identifier.
     * @param wParam   Additional message information.
     * @param lParam   Additional message information.
     * @return INT_PTR Result of message processing.
     */
    static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

    /**
     * @brief Registers and creates the dialog box.
     * @param hInstance Handle to the current application instance.
     * @param hWnd      Handle to the window receiving the message.
     * @return INT_PTR Result of message processing.
     */
    INT_PTR CreateDialogBox(HINSTANCE hInstance, HWND hWnd);

   private:
    /** @brief Prevents external instantiation of the class. */
    Keys() = default;

    /**
     * @brief Centers the dialog box within its parent window.
     * @param hDlg Handle to the dialog box.
     * @return INT_PTR Result of message processing.
     */
    INT_PTR CenterDialog(HWND hDlg);

    /**
     * @brief Arranges controls within the dialog box.
     * @param hDlg      Handle to the dialog box.
     * @return LRESULT Result of message processing.
     */
    LRESULT LayoutControls(HWND hDlg);

    /**
     * @brief Loads the api keys from the registry into the edit controls.
     * @param hDlg Handle to the dialog box.
     */
    void Load(HWND hDlg);

    /**
     * @brief Loads application icon.
     * @param hDlg Handle to the dialog box.
     */
    void LoadDialogIcon(HWND hDlg);

    /**
     * @brief Saves the api keys to the registry.
     * @param hDlg Handle to the dialog box.
     */
    void Save(HWND hDlg);
};
