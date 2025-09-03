#include "splitter.hpp"

#include <CommCtrl.h>

#include <windowsx.h>

#include "lib/settings.hpp"
#include "resource.hpp"

Splitter& Splitter::Instance() {
    static Splitter instance;
    return instance;
}

HWND Splitter::CreateControl(HWND hWnd) {
    // using wide strings (utf-16) because that is required to be
    // able to sub-class controls using ComCtl32.dll version 6
    HWND hSplitter = CreateWindowExW(0,
        L"STATIC",
        nullptr,
        WS_CHILD | WS_VISIBLE | SS_NOTIFY,
        0,
        0,
        0,
        0,
        hWnd,
        (HMENU)(INT_PTR)IDC_SPLITTER,
        GetModuleHandle(nullptr),
        nullptr);

    SetWindowSubclass(hSplitter, Splitter::WndProc, 1, 0);
    return hSplitter;
}

LRESULT CALLBACK Splitter::WndProc(
    HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubClass, DWORD_PTR) {
    switch (message) {
        case WM_LBUTTONDOWN: {
            SetCapture(hWnd);

            // grab local coords
            RECT rc = {};
            GetWindowRect(hWnd, &rc);

            // translate to parent coords
            HWND hParent = GetParent(hWnd);
            POINT pt = { rc.left, rc.top };
            ScreenToClient(hParent, &pt);

            Splitter::Instance().state.dragging = true;
            Splitter::Instance().state.offset = { 0, 0 };
            Splitter::Instance().state.origin = { pt.x, pt.y };

            return 1;
        }
        case WM_LBUTTONUP: {
            if (!Splitter::Instance().state.dragging) {
                break;
            }

            ReleaseCapture();
            Splitter::Instance().state.dragging = false;

            // save new left pane width
            int leftPaneWidth = Splitter::Instance().state.origin.x + Splitter::Instance().state.offset.x;
            lib::settings::leftPaneWidth::save(leftPaneWidth);

            // update parent window
            HWND hParent = GetParent(hWnd);
            SendMessage(hParent, WM_USER_UPDATE_LAYOUT, 0, 0);
            return 1;
        }
        case WM_MOUSEMOVE: {
            if (!Splitter::Instance().state.dragging) {
                break;
            }

            // translate local mouse coords to parent coords
            HWND hParent = GetParent(hWnd);
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            MapWindowPoints(hWnd, hParent, &pt, 1);

            // update with delta
            Splitter::Instance().state.offset.x = pt.x - Splitter::Instance().state.origin.x;

            SetWindowPos(hWnd,
                nullptr,
                Splitter::Instance().state.origin.x + Splitter::Instance().state.offset.x,
                Splitter::Instance().state.origin.y,
                0,
                0,
                SWP_NOSIZE | SWP_NOZORDER);

            return 1;
        }
        case WM_NCDESTROY:
            RemoveWindowSubclass(hWnd, Splitter::WndProc, uIdSubClass);
            break;
        case WM_SETCURSOR:
            SetCursor(LoadCursor(nullptr, IDC_SIZEWE));
            return 1;
    }

    return DefSubclassProc(hWnd, message, wParam, lParam);
}
