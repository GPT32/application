#include "chat.hpp"

#include <CommCtrl.h>

#include <Richedit.h>
#include <model.hpp>
#include <ranges>
#include <thread>

#include "lib/api.hpp"
#include "lib/markdown.hpp"
#include "lib/settings.hpp"
#include "resource.hpp"

Chat& Chat::Instance() {
    static Chat instance;
    return instance;
}

HWND Chat::CreateControl(HWND hWnd) {
    HWND hRichEdit = CreateWindowExW(WS_EX_CLIENTEDGE,
        MSFTEDIT_CLASS,
        nullptr,
        ES_MULTILINE | WS_CHILD | ES_READONLY | WS_VSCROLL,
        0,
        0,
        0,
        0,
        hWnd,
        (HMENU)(INT_PTR)IDC_RICHEDIT,
        GetModuleHandle(nullptr),
        nullptr);

    SetWindowSubclass(hRichEdit, Chat::WndProc, 1, 0);
    SendMessage(hRichEdit, EM_AUTOURLDETECT, AURL_ENABLEURL, 0);
    SendMessage(hRichEdit, EM_SETEVENTMASK, 0, ENM_LINK);

    return hRichEdit;
}

LRESULT Chat::OnApiResponse(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    auto* r = reinterpret_cast<std::string*>(lParam);

    if (!r) {
        return 0;
    }

    HWND hTreeView = GetDlgItem(hWnd, IDC_TREEVIEW);
    HWND hButtonSend = GetDlgItem(hWnd, IDC_BUTTON_SEND);
    HTREEITEM hSelected = reinterpret_cast<HTREEITEM>(wParam);

    if (!hSelected) {
        return 0;
    }

    TVITEM tvi = {};
    tvi.mask = TVIF_PARAM;
    tvi.hItem = hSelected;
    TreeView_GetItem(hTreeView, &tvi);

    // clean up chat from task list and restore send button
    auto* chat = reinterpret_cast<lib::storage::Chat*>(tvi.lParam);
    Model::Instance().tasks.erase(chat->id);
    SetWindowText(hButtonSend, "Send");

    // extract json from response
    std::string messageId;
    std::string messageContent;

    try {
        auto json = nlohmann::json::parse(*r);

        if (!json.empty()) {
            if (!json["error"].is_null()) {
                messageContent = json["error"].value("message", "");
            } else {
                messageId = json.value("id", "");
                messageContent = json["output"][0]["content"][0].value("text", "");
            }
        }
    } catch (const std::exception& e) {
        messageContent = e.what() + std::string("\n\n");
        messageContent += r->c_str();
    }

    // save changes to disk
    chat->messages.push_back(std::make_unique<lib::storage::Message>(messageId, messageContent));
    lib::storage::save(Model::Instance().projects, lib::storage::FILENAME);

    // only render if the user is still on the same
    // tree node that they sent the message from
    HTREEITEM hSelectedCurrent = TreeView_GetSelection(hTreeView);

    if (hSelectedCurrent == hSelected) {
        Chat::RenderMessages(hWnd, chat);
    }

    delete r;
    return 0;
}

void Chat::RenderMessages(HWND hWnd, lib::storage::Chat* chat) {
    // reset text
    HWND hRichEdit = GetDlgItem(hWnd, IDC_RICHEDIT);
    SetWindowText(hRichEdit, "");

    // combine this chat's messages into a single rtf block
    std::string rtf =
        R"({\urtf8\ansi\deff0{\fonttbl
            {\f0\fnil\fcharset0 Segoe UI Emoji;}
            {\f1\fmodern\fcharset0 Courier New;}
        }\f0\fs18 )";

    for (const auto& [idx, message] : chat->messages | std::views::enumerate) {
        rtf += idx % 2 == 0 ? R"({\pard\b You:\b0\par})" : R"({\pard\b Assistant:\b0\par})";
        rtf += lib::markdown::toRTF(message->content);
        rtf += R"(\par)";
    }

    rtf += "}";

    // before streaming the rtf content, an offset
    // must be persisted otherwise it will re-read
    // into the same buffer indefinitely
    struct StreamCookie {
        const std::string* text;
        size_t offset;
    };
    StreamCookie cookie = { &rtf, 0 };

    // stream the string data into the richedit buffer
    EDITSTREAM es = {};
    es.dwCookie = reinterpret_cast<DWORD_PTR>(&cookie);
    es.pfnCallback = [](DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb) -> DWORD {
        auto* p = reinterpret_cast<StreamCookie*>(dwCookie);
        size_t remaining = p->text->size() - p->offset;
        size_t len = std::min<size_t>(cb, remaining);

        if (len > 0) {
            memcpy(pbBuff, p->text->data() + p->offset, len);
            p->offset += len;
        }

        *pcb = static_cast<LONG>(len);
        return 0;
    };
    SendMessage(hRichEdit,
        EM_STREAMIN,
        static_cast<WPARAM>(CP_UTF8 << 16) | SF_USECODEPAGE | SF_RTF,
        reinterpret_cast<LPARAM>(&es));

    // scroll to the bottom after rendering the data
    PostMessage(hRichEdit, WM_USER_SCROLL_TO_BOTTOM, 0, 0);
}

void Chat::SendUserChat(HWND hWnd) {
    // extract from input and reset text
    HWND hEdit = GetDlgItem(hWnd, IDC_EDIT);
    HWND hButtonSend = GetDlgItem(hWnd, IDC_BUTTON_SEND);
    int len = GetWindowTextLength(hEdit);
    std::string text(len, '\0');

    if (len > 0) {
        GetWindowText(hEdit, text.data(), len + 1);
        SetWindowText(hEdit, "");

        EnableWindow(hButtonSend, 0);
        SetWindowText(hButtonSend, "Thinking...");
    }

    // extract chat structure from tree node
    HWND hTreeView = GetDlgItem(hWnd, IDC_TREEVIEW);
    HTREEITEM hSelected = TreeView_GetSelection(hTreeView);
    TVITEM tvi = {};
    tvi.mask = TVIF_PARAM;
    tvi.hItem = hSelected;
    TreeView_GetItem(hTreeView, &tvi);

    // add chat to rich edit control
    auto* chat = reinterpret_cast<lib::storage::Chat*>(tvi.lParam);
    chat->messages.push_back(std::make_unique<lib::storage::Message>("", text));
    Chat::RenderMessages(hWnd, chat);

    // grab project instructions
    std::string instructions = chat->parent->instructions;

    // grab previous message id
    std::string prevMessageId;

    if (chat->messages.size() >= 2) {
        const auto& last = chat->messages[chat->messages.size() - 2];
        prevMessageId = last->id;
    }

    // send non-blocking api request
    std::string apiKey, model;
    lib::settings::apiKey::load(apiKey);
    lib::settings::model::load(model);

    std::jthread([apiKey, hWnd, hSelected, text, model, prevMessageId, instructions]() {
        auto r = lib::api::createResponse(apiKey, text, model, prevMessageId, instructions);
        auto* body = new std::string(r.body.dump());
        PostMessage(hWnd, WM_USER_API_RESPONSE, reinterpret_cast<WPARAM>(hSelected), reinterpret_cast<LPARAM>(body));
    }).detach();

    // add chat to task list
    Model::Instance().tasks.insert(chat->id);

    // save changes to disk
    lib::storage::save(Model::Instance().projects, lib::storage::FILENAME);
}

LRESULT CALLBACK Chat::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubClass, DWORD_PTR) {
    // persist this across calls so we only scroll when recieving
    // new messages and not when the user initiates the scroll.
    static bool scrolling = false;

    switch (message) {
        case WM_NCDESTROY:
            RemoveWindowSubclass(hWnd, Chat::WndProc, uIdSubClass);
            break;
        case WM_PAINT: {
            if (scrolling) {
                SendMessage(hWnd, WM_VSCROLL, SB_BOTTOM, 0);
                scrolling = false;
            }
            break;
        }
        case WM_USER_SCROLL_TO_BOTTOM:
            scrolling = true;
            InvalidateRect(hWnd, nullptr, FALSE);
            break;
    }

    return DefSubclassProc(hWnd, message, wParam, lParam);
}
