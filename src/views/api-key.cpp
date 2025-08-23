#include "api-key.hpp"

#include "lib/settings.hpp"
#include "resource.hpp"

ApiKey& ApiKey::Instance() {
    static ApiKey instance;
    return instance;
}

INT_PTR CALLBACK ApiKey::DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM) {
    switch (message) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDCANCEL:
                    EndDialog(hDlg, IDCANCEL);
                    return 1;
                case IDOK:
                    ApiKey::Instance().Save(hDlg);
                    EndDialog(hDlg, IDOK);
                    return 1;
                default:
                    return 0;
            }
        case WM_INITDIALOG:
            ApiKey::Instance().Load(hDlg);
            ApiKey::Instance().LoadDialogIcon(hDlg);
            ApiKey::Instance().LayoutControls(hDlg);
            return ApiKey::Instance().CenterDialog(hDlg);
        default:
            return 0;
    }
}

INT_PTR ApiKey::CenterDialog(HWND hDlg) {
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

INT_PTR ApiKey::CreateDialogBox(HINSTANCE hInstance, HWND hWnd) {
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_API_KEY_DIALOG), hWnd, ApiKey::DlgProc);
}

LRESULT ApiKey::LayoutControls(HWND hDlg) {
    RECT rcDlg = {};
    GetClientRect(hDlg, &rcDlg);

    HWND hText = GetDlgItem(hDlg, IDC_STATIC_API_KEY);
    HWND hEdit = GetDlgItem(hDlg, IDC_EDIT_API_KEY);
    HWND hButtonOk = GetDlgItem(hDlg, IDOK);
    HWND hButtonCancel = GetDlgItem(hDlg, IDCANCEL);

    const int textHeight = DIALOG_BUTTON_HEIGHT;
    const int buttonHeight = DIALOG_BUTTON_HEIGHT;
    const int editHeight = MAIN_WINDOW_STATIC_HEIGHT;

    int dlgWidth = rcDlg.right - rcDlg.left;
    int dlgHeight = rcDlg.bottom - rcDlg.top;
    int buttonWidth = dlgWidth / 3;

    SetWindowPos(hText, nullptr, SPACING, SPACING, dlgWidth - PADDING, textHeight, SWP_NOZORDER);
    SetWindowPos(hEdit, nullptr, SPACING, textHeight + SPACING, dlgWidth - PADDING, editHeight, SWP_NOZORDER);
    SetWindowPos(hButtonCancel,
        nullptr,
        dlgWidth - (buttonWidth * 2 - SPACING),
        dlgHeight - buttonHeight - SPACING,
        buttonWidth - PADDING,
        buttonHeight,
        SWP_NOZORDER);
    SetWindowPos(hButtonOk,
        nullptr,
        dlgWidth - (buttonWidth - SPACING),
        dlgHeight - buttonHeight - SPACING,
        buttonWidth - PADDING,
        buttonHeight,
        SWP_NOZORDER);

    return 0;
}

void ApiKey::Load(HWND hDlg) {
    std::string apiKey;

    if (!lib::settings::apiKey::load(apiKey)) {
        return;
    }

    SetDlgItemText(hDlg, IDC_EDIT_API_KEY, apiKey.c_str());
}

void ApiKey::LoadDialogIcon(HWND hDlg) {
    HICON hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON));
    SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
}

void ApiKey::Save(HWND hDlg) {
    char cApiKey[256];
    GetDlgItemText(hDlg, IDC_EDIT_API_KEY, cApiKey, std::size(cApiKey));
    std::string sApiKey(cApiKey);
    lib::settings::apiKey::save(sApiKey);
}
