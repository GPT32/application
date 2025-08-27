#include "model-selector.hpp"

#include <CommCtrl.h>

#include <string>

#include "lib/settings.hpp"
#include "resource.hpp"

ModelSelector& ModelSelector::Instance() {
    static ModelSelector instance;
    return instance;
}

HWND ModelSelector::CreateControl(HWND hWnd) {
    HWND hCombobox = CreateWindowEx(0,
        WC_COMBOBOXEX,
        nullptr,
        WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST,
        0,
        0,
        0,
        200,
        hWnd,
        (HMENU)(INT_PTR)IDC_COMBOBOX,
        GetModuleHandle(nullptr),
        nullptr);

    // load selected model from registry
    std::string model;
    lib::settings::model::load(model);

    // load models into combobox
    for (const char* id : ModelSelector::models) {
        COMBOBOXEXITEM cbei = {};
        cbei.mask = CBEIF_TEXT;
        cbei.iItem = -1;
        cbei.pszText = const_cast<LPSTR>(id);
        LRESULT result = SendMessage(hCombobox, CBEM_INSERTITEM, 0, reinterpret_cast<LPARAM>(&cbei));

        if (id == model) {
            SendMessage(hCombobox, CB_SETCURSEL, result, 0);
        }
    }

    return hCombobox;
}

LRESULT ModelSelector::OnCommand(HWND hWnd, WPARAM wParam) {
    switch (LOWORD(wParam)) {
        case IDC_COMBOBOX: {
            if (HIWORD(wParam) != CBN_SELCHANGE) {
                break;
            }

            HWND hCombobox = GetDlgItem(hWnd, IDC_COMBOBOX);
            int sel = static_cast<int>(SendMessage(hCombobox, CB_GETCURSEL, 0, 0));

            if (sel == CB_ERR) {
                break;
            }

            CHAR buffer[256];
            COMBOBOXEXITEM cbei;
            cbei.mask = CBEIF_TEXT;
            cbei.iItem = sel;
            cbei.pszText = buffer;
            cbei.cchTextMax = sizeof(buffer);

            SendMessage(hCombobox, CBEM_GETITEM, 0, reinterpret_cast<LPARAM>(&cbei));

            std::string model(buffer);
            lib::settings::model::save(model);
        }
        default:
            break;
    }

    return 0;
}
