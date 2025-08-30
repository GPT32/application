#include "keys.hpp"

#include "lib/settings.hpp"
#include "resource.hpp"

Keys& Keys::Instance() {
    static Keys instance;
    return instance;
}

INT_PTR CALLBACK Keys::DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM) {
    switch (message) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDCANCEL:
                    EndDialog(hDlg, IDCANCEL);
                    return 1;
                case IDOK:
                    Keys::Instance().Save(hDlg);
                    EndDialog(hDlg, IDOK);
                    return 1;
                default:
                    return 0;
            }
        case WM_INITDIALOG:
            Keys::Instance().Load(hDlg);
            Keys::Instance().LoadDialogIcon(hDlg);
            Keys::Instance().LayoutControls(hDlg);
            return Keys::Instance().CenterDialog(hDlg);
        default:
            return 0;
    }
}

INT_PTR Keys::CenterDialog(HWND hDlg) {
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

INT_PTR Keys::CreateDialogBox(HINSTANCE hInstance, HWND hWnd) {
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_API_KEYS_DIALOG), hWnd, Keys::DlgProc);
}

LRESULT Keys::LayoutControls(HWND hDlg) {
    RECT rcDlg = {};
    GetClientRect(hDlg, &rcDlg);

    HWND hTextApiKey = GetDlgItem(hDlg, IDC_STATIC_API_KEY);
    HWND hTextAdminKey = GetDlgItem(hDlg, IDC_STATIC_ADMIN_API_KEY);
    HWND hEditApiKey = GetDlgItem(hDlg, IDC_EDIT_API_KEY);
    HWND hEditAdminKey = GetDlgItem(hDlg, IDC_EDIT_ADMIN_API_KEY);
    HWND hButtonOk = GetDlgItem(hDlg, IDOK);
    HWND hButtonCancel = GetDlgItem(hDlg, IDCANCEL);

    const int textHeight = DIALOG_BUTTON_HEIGHT;
    const int buttonHeight = DIALOG_BUTTON_HEIGHT;
    const int editHeight = MAIN_WINDOW_STATIC_HEIGHT;

    int dlgWidth = rcDlg.right - rcDlg.left;
    int dlgHeight = rcDlg.bottom - rcDlg.top;
    int buttonWidth = dlgWidth / 3;

    SetWindowPos(hTextApiKey, nullptr, SPACING, SPACING, dlgWidth - PADDING, textHeight, SWP_NOZORDER);
    SetWindowPos(hEditApiKey, nullptr, SPACING, textHeight + SPACING, dlgWidth - PADDING, editHeight, SWP_NOZORDER);
    SetWindowPos(hTextAdminKey,
        nullptr,
        SPACING,
        textHeight + editHeight + PADDING,
        dlgWidth - PADDING,
        textHeight,
        SWP_NOZORDER);
    SetWindowPos(hEditAdminKey,
        nullptr,
        SPACING,
        (textHeight * 2) + editHeight + PADDING,
        dlgWidth - PADDING,
        editHeight,
        SWP_NOZORDER);
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

void Keys::Load(HWND hDlg) {
    std::string apiKey;
    std::string adminKey;

    if (lib::settings::apiKey::load(apiKey)) {
        SetDlgItemText(hDlg, IDC_EDIT_API_KEY, apiKey.c_str());
    }

    if (lib::settings::adminApiKey::load(adminKey)) {
        SetDlgItemText(hDlg, IDC_EDIT_ADMIN_API_KEY, adminKey.c_str());
    }
}

void Keys::LoadDialogIcon(HWND hDlg) {
    HICON hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON));
    SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
}

void Keys::Save(HWND hDlg) {
    // api key
    char cApiKey[256];
    GetDlgItemText(hDlg, IDC_EDIT_API_KEY, cApiKey, std::size(cApiKey));
    std::string sApiKey(cApiKey);

    if (!sApiKey.empty()) {
        lib::settings::apiKey::save(sApiKey);
    }

    // admin key
    char cAdminKey[256];
    GetDlgItemText(hDlg, IDC_EDIT_ADMIN_API_KEY, cAdminKey, std::size(cAdminKey));
    std::string sAdminKey(cAdminKey);

    if (!sAdminKey.empty()) {
        lib::settings::adminApiKey::save(sAdminKey);
    }
}
