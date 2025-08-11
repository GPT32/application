#include "about.hpp"

#include "resource.hpp"

About& About::Instance() {
    static About instance;
    return instance;
}

INT_PTR CALLBACK About::DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM) {
    switch (message) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDCANCEL:
                    EndDialog(hDlg, IDCANCEL);
                    return 1;
                case IDOK:
                    EndDialog(hDlg, IDOK);
                    return 1;
                default:
                    return 0;
            }
        case WM_INITDIALOG:
            About::Instance().LayoutControls(hDlg);
            About::Instance().LoadDialogIcon(hDlg);
            About::Instance().CenterDialog(hDlg);
            return 1;
        default:
            return 0;
    }
}

INT_PTR About::CenterDialog(HWND hDlg) {
    HWND hParent = GetParent(hDlg);
    RECT rcDlg = {};
    RECT rcParent = {};

    if (!hParent) {
        hParent = GetDesktopWindow();
    }

    GetWindowRect(hDlg, &rcDlg);
    GetWindowRect(hParent, &rcParent);

    int dlgWidth = rcDlg.right - rcDlg.left;
    int dlgHeight = rcDlg.bottom - rcDlg.top;
    int parentWidth = rcParent.right - rcParent.left;
    int parentHeight = rcParent.bottom - rcParent.top;

    int x = rcParent.left + (parentWidth - dlgWidth) / 2;
    int y = rcParent.top + (parentHeight - dlgHeight) / 2;

    SetWindowPos(hDlg, nullptr, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    return 1;
}

INT_PTR About::CreateDialogBox(HINSTANCE hInstance, HWND hWnd) {
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUT_DIALOG), hWnd, About::DlgProc);
}

LRESULT About::LayoutControls(HWND hDlg) {
    RECT rcDlg = {};
    GetClientRect(hDlg, &rcDlg);

    HWND hIcon = GetDlgItem(hDlg, IDC_STATIC_ICON);
    HWND hText = GetDlgItem(hDlg, IDC_STATIC_ABOUT);
    HWND hButtonOk = GetDlgItem(hDlg, IDOK);

    const int iconSize = 50;
    const int buttonHeight = DIALOG_BUTTON_HEIGHT;

    int dlgWidth = rcDlg.right - rcDlg.left;
    int dlgHeight = rcDlg.bottom - rcDlg.top;
    int textWidth = dlgWidth - iconSize - PADDING;
    int textHeight = dlgHeight - buttonHeight - PADDING;
    int buttonWidth = dlgWidth / 3;

    SetWindowPos(hIcon, nullptr, PADDING, PADDING, iconSize, iconSize, SWP_NOZORDER);
    SetWindowPos(hText, nullptr, PADDING + SPACING + iconSize, PADDING, textWidth, textHeight, SWP_NOZORDER);
    SetWindowPos(hButtonOk,
        nullptr,
        dlgWidth - (buttonWidth - SPACING),
        dlgHeight - buttonHeight - SPACING,
        buttonWidth - PADDING,
        buttonHeight,
        SWP_NOZORDER);

    return 0;
}

void About::LoadDialogIcon(HWND hDlg) {
    HICON hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON));
    SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
}
