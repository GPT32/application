#include "instructions.hpp"

#include <CommCtrl.h>

#include <fstream>
#include <model.hpp>

#include "lib/storage.hpp"
#include "resource.hpp"

Instructions& Instructions::Instance() {
    static Instructions instance;
    return instance;
}

INT_PTR CALLBACK Instructions::DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM) {
    switch (message) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDCANCEL:
                    EndDialog(hDlg, IDCANCEL);
                    return 1;
                case IDOK:
                    Instructions::Instance().Save(hDlg);
                    EndDialog(hDlg, IDOK);
                    return 1;
                default:
                    return 0;
            }
        case WM_INITDIALOG:
            Instructions::Instance().LayoutControls(hDlg);
            Instructions::Instance().Load(hDlg);
            Instructions::Instance().LoadDialogIcon(hDlg);
            Instructions::Instance().CenterDialog(hDlg);
            return 1;
        default:
            return 0;
    }
}

INT_PTR Instructions::CenterDialog(HWND hDlg) {
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

INT_PTR Instructions::CreateDialogBox(HINSTANCE hInstance, HWND hWnd) {
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_INSTRUCTIONS_DIALOG), hWnd, Instructions::DlgProc);
}

LRESULT Instructions::LayoutControls(HWND hDlg) {
    RECT rcDlg = {};
    GetClientRect(hDlg, &rcDlg);

    HWND hText = GetDlgItem(hDlg, IDC_STATIC_INSTRUCTIONS);
    HWND hEdit = GetDlgItem(hDlg, IDC_EDIT_INSTRUCTIONS);
    HWND hButtonOk = GetDlgItem(hDlg, IDOK);
    HWND hButtonCancel = GetDlgItem(hDlg, IDCANCEL);

    const int textHeight = DIALOG_BUTTON_HEIGHT;
    const int buttonHeight = DIALOG_BUTTON_HEIGHT;

    int dlgWidth = rcDlg.right - rcDlg.left;
    int dlgHeight = rcDlg.bottom - rcDlg.top;
    int editHeight = dlgHeight - PADDING - buttonHeight - textHeight;
    int buttonWidth = dlgWidth / 3;

    SetWindowPos(hText, nullptr, SPACING, SPACING, dlgWidth - PADDING, textHeight, SWP_NOZORDER);
    SetWindowPos(hEdit, nullptr, SPACING, textHeight, dlgWidth - PADDING, editHeight, SWP_NOZORDER);
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

void Instructions::Load(HWND hDlg) {
    HWND hParent = GetParent(hDlg);
    HWND hTreeView = GetDlgItem(hParent, IDC_TREEVIEW);
    HTREEITEM hSelected = TreeView_GetSelection(hTreeView);
    TVITEM tviParent = { .mask = TVIF_PARAM, .hItem = hSelected };
    TreeView_GetItem(hTreeView, &tviParent);

    auto* project = reinterpret_cast<lib::storage::Project*>(tviParent.lParam);
    SetDlgItemText(hDlg, IDC_EDIT_INSTRUCTIONS, project->instructions.c_str());
}

void Instructions::LoadDialogIcon(HWND hDlg) {
    HICON hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON));
    SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
}

void Instructions::Save(HWND hDlg) {
    HWND hEdit = GetDlgItem(hDlg, IDC_EDIT_INSTRUCTIONS);
    int len = GetWindowTextLength(hEdit);
    std::string instructions(len, '\0');
    GetWindowText(hEdit, instructions.data(), len + 1);

    HWND hParent = GetParent(hDlg);
    HWND hTreeView = GetDlgItem(hParent, IDC_TREEVIEW);
    HTREEITEM hSelected = TreeView_GetSelection(hTreeView);
    TVITEM tviParent = { .mask = TVIF_PARAM, .hItem = hSelected };
    TreeView_GetItem(hTreeView, &tviParent);

    auto* project = reinterpret_cast<lib::storage::Project*>(tviParent.lParam);
    project->instructions = std::move(instructions);

    // save changes to disk
    lib::storage::save(Model::Instance().projects, lib::storage::FILENAME);
}
