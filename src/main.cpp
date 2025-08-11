#include <windows.h>

#include <commctrl.h>

#include <resource.hpp>
#include <shlobj.h>

#include "views/view.hpp"

/**
 * @brief Main entry point for the Win32 application.
 * @param hInstance     Handle to the current application instance.
 * @param hPrevInstance Reserved (always nullptr).
 * @param lpCmdLine     The command line for the application, excluding the program name.
 * @param nCmdShow      Controls how the window is to be shown.
 * @return Application exit code.
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    // initialize common controls
    INITCOMMONCONTROLSEX icex = {};
    icex.dwSize = sizeof(icex);
    icex.dwICC = ICC_TREEVIEW_CLASSES | ICC_STANDARD_CLASSES | ICC_USEREX_CLASSES;
    InitCommonControlsEx(&icex);
    LoadLibrary("Msftedit.dll");

    // create the main window
    HWND hWnd = View::Instance().CreateMainWindow(hInstance);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // initialize the message loop
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}
