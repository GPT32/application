#include "controller.hpp"

#include <Richedit.h>
#include <shlobj.h>
#include <windowsx.h>

#include "lib/settings.hpp"
#include "lib/uuid.hpp"
#include "model.hpp"
#include "resource.hpp"
#include "views/about.hpp"
#include "views/chat.hpp"
#include "views/instructions.hpp"
#include "views/keys.hpp"
#include "views/model-selector.hpp"
#include "views/status-bar.hpp"

Controller& Controller::Instance() {
    static Controller instance;
    return instance;
}

HTREEITEM Controller::AddTreeItem(HWND hWnd, HTREEITEM hParent) {
    HWND hTreeView = GetDlgItem(hWnd, IDC_TREEVIEW);
    TVINSERTSTRUCT tvis = {};

    tvis.hParent = hParent ? hParent : TVI_ROOT;
    tvis.hInsertAfter = TVI_LAST;
    tvis.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    tvis.item.iImage = hParent ? icons.document : icons.folder;
    tvis.item.iSelectedImage = tvis.item.iImage;

    return TreeView_InsertItem(hTreeView, &tvis);
}

HTREEITEM Controller::AddTreeItem(HWND hWnd, const std::unique_ptr<lib::storage::Project>& project) {
    HWND hTreeView = GetDlgItem(hWnd, IDC_TREEVIEW);
    TVINSERTSTRUCT tvis = {};

    tvis.hParent = TVI_ROOT;
    tvis.hInsertAfter = TVI_LAST;
    tvis.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN | TVIF_PARAM;
    tvis.item.pszText = const_cast<char*>(project->name.c_str());
    tvis.item.iImage = icons.folder;
    tvis.item.iSelectedImage = tvis.item.iImage;
    tvis.item.cChildren = static_cast<int>(project->chats.size());
    tvis.item.lParam = reinterpret_cast<LPARAM>(project.get());

    return TreeView_InsertItem(hTreeView, &tvis);
}

HTREEITEM Controller::AddTreeItem(HWND hWnd, const std::unique_ptr<lib::storage::Chat>& chat, HTREEITEM hParent) {
    HWND hTreeView = GetDlgItem(hWnd, IDC_TREEVIEW);
    TVINSERTSTRUCT tvis = {};

    tvis.hParent = hParent;
    tvis.hInsertAfter = TVI_LAST;
    tvis.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
    tvis.item.pszText = const_cast<char*>(chat->name.c_str());
    tvis.item.iImage = icons.document;
    tvis.item.iSelectedImage = tvis.item.iImage;
    tvis.item.lParam = reinterpret_cast<LPARAM>(chat.get());

    return TreeView_InsertItem(hTreeView, &tvis);
}

LRESULT Controller::OnCommand(HWND hWnd, WPARAM wParam) {
    switch (LOWORD(wParam)) {
        case IDC_BUTTON_SEND: {
            Chat::Instance().SendUserChat(hWnd);
            break;
        }
        case IDC_COMBOBOX:
            return ModelSelector::Instance().OnCommand(hWnd, wParam);
        case IDC_EDIT: {
            if (HIWORD(wParam) == EN_CHANGE) {
                HWND hEdit = GetDlgItem(hWnd, IDC_EDIT);
                HWND hButtonSend = GetDlgItem(hWnd, IDC_BUTTON_SEND);

                int len = GetWindowTextLength(hEdit);
                EnableWindow(hButtonSend, len > 0);
            }
            break;
        }
        case IDM_EDIT_API_KEYS: {
            HINSTANCE hInstance = GetModuleHandle(nullptr);
            Keys::Instance().CreateDialogBox(hInstance, hWnd);
            break;
        }
        case IDM_EDIT_INSTRUCTIONS: {
            HINSTANCE hInstance = GetModuleHandle(nullptr);
            Instructions::Instance().CreateDialogBox(hInstance, hWnd);
            break;
        }
        case IDM_FILE_DELETE_CHAT: {
            HWND hTreeView = GetDlgItem(hWnd, IDC_TREEVIEW);
            HTREEITEM hSelected = TreeView_GetSelection(hTreeView);
            TVITEM tvi = { .mask = TVIF_PARAM, .hItem = hSelected };
            TreeView_GetItem(hTreeView, &tvi);

            auto* chat = reinterpret_cast<lib::storage::Chat*>(tvi.lParam);
            TreeView_DeleteItem(hTreeView, hSelected);
            lib::storage::deleteChat(*chat->parent, chat->name);
            lib::storage::save(Model::Instance().projects, lib::storage::FILENAME);
            break;
        }
        case IDM_FILE_DELETE_PROJECT: {
            HWND hTreeView = GetDlgItem(hWnd, IDC_TREEVIEW);
            HTREEITEM hSelected = TreeView_GetSelection(hTreeView);
            TVITEM tvi = { .mask = TVIF_PARAM, .hItem = hSelected };
            TreeView_GetItem(hTreeView, &tvi);

            auto* project = reinterpret_cast<lib::storage::Project*>(tvi.lParam);
            TreeView_DeleteItem(hTreeView, hSelected);
            lib::storage::deleteProject(Model::Instance().projects, project->name);
            lib::storage::save(Model::Instance().projects, lib::storage::FILENAME);
            break;
        }
        case IDM_FILE_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDM_FILE_NEW_CHAT: {
            HWND hTreeView = GetDlgItem(hWnd, IDC_TREEVIEW);
            HTREEITEM hProject = TreeView_GetSelection(hTreeView);
            HTREEITEM hChat = Controller::AddTreeItem(hWnd, hProject);
            TreeView_SelectItem(hTreeView, hChat);
            TreeView_EditLabel(hTreeView, hChat);
            break;
        }
        case IDM_FILE_NEW_PROJECT: {
            HTREEITEM hProject = Controller::AddTreeItem(hWnd, nullptr);
            HWND hTreeView = GetDlgItem(hWnd, IDC_TREEVIEW);
            TreeView_SelectItem(hTreeView, hProject);
            TreeView_EditLabel(hTreeView, hProject);
            break;
        }
        case IDM_HELP_ABOUT: {
            HINSTANCE hInstance = GetModuleHandle(nullptr);
            About::Instance().CreateDialogBox(hInstance, hWnd);
            break;
        }
        case IDM_HELP_API_LOGS: {
            std::string url = "https://platform.openai.com/logs?api=responses";
            ShellExecute(hWnd, "open", url.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
            break;
        }
        case IDM_HELP_API_USAGE: {
            std::string url = "https://platform.openai.com/settings/organization/usage";
            ShellExecute(hWnd, "open", url.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
            break;
        }
        case IDM_HELP_ISSUE: {
            std::string url = APP_REPO "/issues/new?template=bug_report.yml";
            ShellExecute(hWnd, "open", url.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
            break;
        }
        case IDM_HELP_FEATURE: {
            std::string url = APP_REPO "/issues?q=is%3Aopen+is%3Aissue+type%3Afeature";
            ShellExecute(hWnd, "open", url.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
            break;
        }
        case IDM_VIEW_ALWAYS_ON_TOP:
            bool alwaysOnTop;
            lib::settings::alwaysOnTop::load(alwaysOnTop);
            lib::settings::alwaysOnTop::save(!alwaysOnTop);
            SetWindowPos(hWnd, !alwaysOnTop ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            break;
        default:
            break;
    }

    return 0;
}

LRESULT Controller::OnCreate(HWND hWnd) {
    // load icons from shell32 to use in the tree view
    HICON hIcon = nullptr;
    HIMAGELIST hImageList = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, 2, 1);
    SHSTOCKICONINFO sii = { sizeof(sii) };

    if (SUCCEEDED(SHGetStockIconInfo(SIID_FOLDER, SHGSI_ICON | SHGSI_SMALLICON, &sii))) {
        if (SUCCEEDED(SHDefExtractIconW(sii.szPath, sii.iIcon, 0, &hIcon, nullptr, 16))) {
            icons.folder = ImageList_AddIcon(hImageList, hIcon);
        }
    }

    if (SUCCEEDED(SHGetStockIconInfo(SIID_DOCNOASSOC, SHGSI_ICON | SHGSI_SMALLICON, &sii))) {
        if (SUCCEEDED(SHDefExtractIconW(sii.szPath, sii.iIcon, 0, &hIcon, nullptr, 16))) {
            icons.document = ImageList_AddIcon(hImageList, hIcon);
            DestroyIcon(hIcon);
        }
    }

    HWND hTreeView = GetDlgItem(hWnd, IDC_TREEVIEW);
    TreeView_SetImageList(hTreeView, hImageList, TVSIL_NORMAL);

    // load projects into tree view
    Model::Instance().projects = lib::storage::load(lib::storage::FILENAME);

    for (const auto& project : Model::Instance().projects) {
        HTREEITEM hProject = Controller::AddTreeItem(hWnd, project);

        for (const auto& chat : project->chats) {
            Controller::AddTreeItem(hWnd, chat, hProject);
        }
    }

    // apply always on top setting
    bool alwaysOnTop;
    lib::settings::alwaysOnTop::load(alwaysOnTop);

    if (alwaysOnTop) {
        SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }

    return 0;
}

LRESULT Controller::OnInitMenuPopup(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    HMENU hMenu = reinterpret_cast<HMENU>(wParam);
    HWND hTreeView = GetDlgItem(hWnd, IDC_TREEVIEW);
    HTREEITEM hSelected = TreeView_GetSelection(hTreeView);
    HTREEITEM hParent = TreeView_GetParent(hTreeView, hSelected);

    switch (LOWORD(lParam)) {
        case 0: {
            // buttons to disable if a root node is not selected
            EnableMenuItem(hMenu, IDM_FILE_NEW_CHAT, hSelected && !hParent ? MF_ENABLED : MF_DISABLED);
            EnableMenuItem(hMenu, IDM_FILE_DELETE_PROJECT, hSelected && !hParent ? MF_ENABLED : MF_DISABLED);

            // buttons to disable if a child node is not selected
            EnableMenuItem(hMenu, IDM_FILE_DELETE_CHAT, hSelected && hParent ? MF_ENABLED : MF_DISABLED);
            break;
        }
        case 1: {
            EnableMenuItem(hMenu, IDM_EDIT_INSTRUCTIONS, hSelected && !hParent ? MF_ENABLED : MF_DISABLED);
        }
        case 2: {
            bool alwaysOnTop;
            lib::settings::alwaysOnTop::load(alwaysOnTop);
            CheckMenuItem(hMenu, IDM_VIEW_ALWAYS_ON_TOP, alwaysOnTop ? MF_CHECKED : MF_UNCHECKED);
            break;
        }
        default:
            break;
    }

    return 0;
}

LRESULT Controller::OnNotify(HWND hWnd, LPARAM lParam) {
    NMHDR* nmhdr = reinterpret_cast<NMHDR*>(lParam);

    // bail early if not a control we care about
    if (nmhdr->idFrom != IDC_TREEVIEW && nmhdr->idFrom != IDC_RICHEDIT) {
        return 0;
    }

    // what kind of message did windows send us
    switch (nmhdr->code) {
        case EN_LINK: {
            auto* pEnLink = reinterpret_cast<ENLINK*>(lParam);

            // we only open links when user clicks on them
            if (pEnLink->msg != WM_LBUTTONUP) {
                return 0;
            }

            const auto len = pEnLink->chrg.cpMax - pEnLink->chrg.cpMin;
            std::vector<wchar_t> hyperlink(len + 1, L'\0');
            TEXTRANGEW tr{ .chrg = pEnLink->chrg, .lpstrText = hyperlink.data() };
            SendMessage(nmhdr->hwndFrom, EM_GETTEXTRANGE, 0, reinterpret_cast<LPARAM>(&tr));
            ShellExecuteW(hWnd, L"open", hyperlink.data(), nullptr, nullptr, SW_SHOWNORMAL);

            return 0;
        }
        case NM_DBLCLK: {
            // grab the coords of where the message originated from
            DWORD pos = GetMessagePos();
            POINT pt = { GET_X_LPARAM(pos), GET_Y_LPARAM(pos) };
            ScreenToClient(nmhdr->hwndFrom, &pt);

            // which we use to check if the double-click happened on our tree node
            TVHITTESTINFO hit = { .pt = pt };
            TreeView_HitTest(nmhdr->hwndFrom, &hit);

            if (hit.flags & TVHT_ONITEMLABEL) {
                TreeView_EditLabel(nmhdr->hwndFrom, hit.hItem);
            }

            return 0;
        }
        case TVN_ENDLABELEDIT: {
            // bail early if nothing to edit
            LPNMTVDISPINFO ptvdi = (LPNMTVDISPINFO)lParam;

            if (!ptvdi->item.pszText) {
                return 0;
            }

            // fetch `lParam` from the edited item
            TVITEM& tvi = ptvdi->item;

            if (!(tvi.mask & TVIF_PARAM)) {
                TVITEM tempTvi = { .mask = TVIF_PARAM, .hItem = tvi.hItem };
                TreeView_GetItem(nmhdr->hwndFrom, &tempTvi);
                tvi.lParam = tempTvi.lParam;
                tvi.mask |= TVIF_PARAM;
            }

            // handle new nodes being added
            if (!tvi.lParam) {
                HTREEITEM hParent = TreeView_GetParent(nmhdr->hwndFrom, tvi.hItem);

                if (hParent) {
                    TVITEM tviParent = { .mask = TVIF_PARAM, .hItem = hParent };
                    TreeView_GetItem(nmhdr->hwndFrom, &tviParent);

                    auto* project = reinterpret_cast<lib::storage::Project*>(tviParent.lParam);
                    auto& chat = project->chats.emplace_back(std::make_unique<lib::storage::Chat>());
                    chat->parent = project;
                    tvi.lParam = reinterpret_cast<LPARAM>(chat.get());

                    tviParent.mask = TVIF_CHILDREN;
                    tviParent.cChildren = 1;
                    TreeView_SetItem(nmhdr->hwndFrom, &tviParent);
                } else {
                    auto& project = Model::Instance().projects.emplace_back(std::make_unique<lib::storage::Project>());
                    tvi.lParam = reinterpret_cast<LPARAM>(project.get());
                }
            }

            // update the tree node and projects struct
            TreeView_SetItem(nmhdr->hwndFrom, &tvi);

            for (auto& project : Model::Instance().projects) {
                if (reinterpret_cast<LPARAM>(project.get()) == tvi.lParam) {
                    project->name = ptvdi->item.pszText;
                    break;
                }

                for (auto& chat : project->chats) {
                    if (reinterpret_cast<LPARAM>(chat.get()) == tvi.lParam) {
                        chat->id = lib::uuid::generate();
                        chat->name = ptvdi->item.pszText;
                        break;
                    }
                }
            }

            // update our storage
            lib::storage::save(Model::Instance().projects, lib::storage::FILENAME);
            return 1;
        }
        case TVN_SELCHANGED: {
            LPNMTREEVIEW ptv = reinterpret_cast<LPNMTREEVIEW>(lParam);
            TVITEM tvi = { .mask = TVIF_PARAM, .hItem = ptv->itemNew.hItem };
            TreeView_GetItem(nmhdr->hwndFrom, &tvi);

            HTREEITEM hParent = TreeView_GetParent(nmhdr->hwndFrom, tvi.hItem);
            HWND hRichEdit = GetDlgItem(hWnd, IDC_RICHEDIT);
            HWND hEdit = GetDlgItem(hWnd, IDC_EDIT);
            HWND hButtonSend = GetDlgItem(hWnd, IDC_BUTTON_SEND);
            HWND hStaticIntro = GetDlgItem(hWnd, IDC_STATIC_INTRO);

            // we don't care about root nodes
            if (!hParent) {
                ShowWindow(hRichEdit, SW_HIDE);
                ShowWindow(hEdit, SW_HIDE);
                ShowWindow(hButtonSend, SW_HIDE);
                ShowWindow(hStaticIntro, SW_SHOW);

                // reset status bar usage stats
                SendMessage(hWnd, WM_USER_UPDATE_SESSION_USAGE, 0, 0);
                return 0;
            }

            // show rich edit controls
            ShowWindow(hRichEdit, SW_SHOW);
            ShowWindow(hEdit, SW_SHOW);
            ShowWindow(hButtonSend, SW_SHOW);
            ShowWindow(hStaticIntro, SW_HIDE);
            EnableWindow(hButtonSend, 0);

            // bail here if a new chat was added
            if (!tvi.lParam) {
                SetWindowText(hRichEdit, "");
                return 0;
            }

            // combine this chat's messages into a single rtf block
            auto* chat = reinterpret_cast<lib::storage::Chat*>(tvi.lParam);
            Chat::Instance().RenderMessages(hWnd, chat);

            // adjust send button text if this chat has any pending work
            auto worker = Model::Instance().tasks.find(chat->id);

            if (worker != Model::Instance().tasks.end()) {
                SetWindowText(hButtonSend, "Thinking...");
            } else {
                SetWindowText(hButtonSend, "Send");
            }

            // update status bar usage stats
            SendMessage(hWnd, WM_USER_UPDATE_SESSION_USAGE, 0, reinterpret_cast<LPARAM>(chat));
        }
        default:
            return 0;
    }
}

LRESULT Controller::OnUserMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_USER_ADMIN_API_RESPONSE:
            return StatusBar::Instance().OnAdminApiResponse(hWnd, wParam, lParam);
        case WM_USER_API_RESPONSE:
            return Chat::Instance().OnApiResponse(hWnd, wParam, lParam);
        case WM_USER_UPDATE_SESSION_USAGE:
            return StatusBar::Instance().OnUpdateSessionUsage(hWnd, wParam, lParam);
        default:
            return 0;
    }
}
