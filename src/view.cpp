#include "view.hpp"

#include "chat.hpp"
#include "controller.hpp"
#include "model-selector.hpp"
#include "resource.hpp"

View& View::Instance() {
    static View instance;
    return instance;
}

LRESULT View::CenterWindow(HWND hWnd) {
    RECT rcWindow;
    GetWindowRect(hWnd, &rcWindow);

    int winWidth = rcWindow.right - rcWindow.left;
    int winHeight = rcWindow.bottom - rcWindow.top;
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    SetWindowPos(hWnd,
        nullptr,
        (screenWidth - winWidth) / 2,
        (screenHeight - winHeight) / 2,
        0,
        0,
        SWP_NOZORDER | SWP_NOSIZE);

    return 0;
}

LRESULT View::CreateControls(HWND hWnd) {
    // create the model combobox
    CreateWindowEx(0,
        "BUTTON",
        "Choose a model",
        WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        0,
        0,
        0,
        0,
        hWnd,
        (HMENU)(INT_PTR)IDC_GROUPBOX_MODEL,
        GetModuleHandle(nullptr),
        nullptr);
    ModelSelector::Instance().CreateControl(hWnd);

    // create the tree view
    CreateWindowEx(WS_EX_CLIENTEDGE,
        WC_TREEVIEW,
        nullptr,
        WS_VISIBLE | WS_CHILD | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_EDITLABELS | TVS_SHOWSELALWAYS,
        0,
        0,
        0,
        0,
        hWnd,
        (HMENU)(INT_PTR)IDC_TREEVIEW,
        GetModuleHandle(nullptr),
        nullptr);

    // create the intro static control
    CreateWindowEx(0,
        "STATIC",
        "Create or select a project and chat to begin.",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        0,
        0,
        0,
        0,
        hWnd,
        (HMENU)(INT_PTR)IDC_STATIC_INTRO,
        GetModuleHandle(nullptr),
        nullptr);

    // create the chat control
    Chat::Instance().CreateControl(hWnd);

    // create edit control for input
    CreateWindowEx(WS_EX_CLIENTEDGE,
        "EDIT",
        nullptr,
        ES_MULTILINE | WS_CHILD | WS_VSCROLL | ES_LEFT | ES_AUTOVSCROLL,
        0,
        0,
        0,
        0,
        hWnd,
        (HMENU)(INT_PTR)IDC_EDIT,
        GetModuleHandle(nullptr),
        nullptr);

    // create send button
    CreateWindowEx(0,
        "BUTTON",
        "Send",
        WS_CHILD | BS_PUSHBUTTON | BS_DEFPUSHBUTTON | WS_DISABLED | WS_TABSTOP,
        0,
        0,
        0,
        0,
        hWnd,
        (HMENU)(INT_PTR)IDC_BUTTON_SEND,
        GetModuleHandle(nullptr),
        nullptr);

    // create status bar
    CreateWindowEx(0,
        STATUSCLASSNAME,
        nullptr,
        WS_VISIBLE | WS_CHILD | SBARS_SIZEGRIP,
        0,
        0,
        0,
        0,
        hWnd,
        (HMENU)(INT_PTR)IDC_STATUS_BAR,
        GetModuleHandle(nullptr),
        nullptr);

    return 0;
}

HWND View::CreateMainWindow(HINSTANCE hInstance) {
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_3DFACE + 1);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
    wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
    wcex.hInstance = hInstance;
    wcex.lpfnWndProc = View::WndProc;
    wcex.lpszClassName = APP_ID;
    wcex.lpszMenuName = MAKEINTRESOURCE(IDM_MENU);
    wcex.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClassEx(&wcex);

    return CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
        APP_ID,
        APP_NAME,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        MAIN_WINDOW_WIDTH,
        MAIN_WINDOW_HEIGHT,
        nullptr,
        nullptr,
        hInstance,
        nullptr);
}

LRESULT View::LayoutControls(HWND hWnd, LPARAM lParam) {
    HWND hButtonSend = GetDlgItem(hWnd, IDC_BUTTON_SEND);
    HWND hCombobox = GetDlgItem(hWnd, IDC_COMBOBOX);
    HWND hEdit = GetDlgItem(hWnd, IDC_EDIT);
    HWND hGroupboxModel = GetDlgItem(hWnd, IDC_GROUPBOX_MODEL);
    HWND hRichEdit = GetDlgItem(hWnd, IDC_RICHEDIT);
    HWND hStaticIntro = GetDlgItem(hWnd, IDC_STATIC_INTRO);
    HWND hTreeView = GetDlgItem(hWnd, IDC_TREEVIEW);
    HWND hStatusBar = GetDlgItem(hWnd, IDC_STATUS_BAR);

    RECT rcStatusBar = {};
    GetClientRect(hStatusBar, &rcStatusBar);

    const int buttonHeight = MAIN_WINDOW_BUTTON_HEIGHT;
    const int comboboxHeight = MAIN_WINDOW_BUTTON_HEIGHT;
    const int inputHeight = 120;
    const int staticIntroheight = MAIN_WINDOW_STATIC_HEIGHT;

    int windowWidth = LOWORD(lParam);
    int windowHeight = HIWORD(lParam);
    int leftPaneWidth = windowWidth / 6;
    int rightPaneWidth = windowWidth - leftPaneWidth;

    int groupBoxHeight = 50;
    int statusBarHeight = rcStatusBar.bottom - rcStatusBar.top;
    int richEditWidth = rightPaneWidth - SPACING;
    int richEditHeight = windowHeight - inputHeight - buttonHeight - statusBarHeight - (PADDING * 2);
    int staticIntroWidth = rightPaneWidth / 3;
    int treeViewWidth = leftPaneWidth - PADDING;
    int treeViewHeight = windowHeight - groupBoxHeight - statusBarHeight - PADDING - SPACING;

    SetWindowPos(hGroupboxModel, nullptr, SPACING, SPACING, treeViewWidth, groupBoxHeight, SWP_NOZORDER);
    SetWindowPos(hCombobox,
        nullptr,
        PADDING,
        ((groupBoxHeight - comboboxHeight) / 2) + PADDING + 2,
        treeViewWidth - PADDING,
        comboboxHeight,
        SWP_NOZORDER);
    SetWindowPos(hTreeView, nullptr, SPACING, PADDING + groupBoxHeight, treeViewWidth, treeViewHeight, SWP_NOZORDER);
    SetWindowPos(hStaticIntro,
        nullptr,
        (leftPaneWidth - SPACING) + ((rightPaneWidth - staticIntroWidth + SPACING) / 2),
        (windowHeight - staticIntroheight) / 2,
        staticIntroWidth,
        staticIntroheight,
        SWP_NOZORDER);
    SetWindowPos(hRichEdit, nullptr, leftPaneWidth, SPACING, richEditWidth, richEditHeight, SWP_NOZORDER);
    SetWindowPos(hEdit, nullptr, leftPaneWidth, richEditHeight + PADDING, richEditWidth, inputHeight, SWP_NOZORDER);
    SetWindowPos(hButtonSend,
        nullptr,
        leftPaneWidth,
        windowHeight - buttonHeight - statusBarHeight - SPACING,
        richEditWidth,
        buttonHeight,
        SWP_NOZORDER);
    SetWindowPos(hStatusBar, nullptr, 0, 0, windowWidth, 0, SWP_NOZORDER | SWP_NOMOVE);

    // layout the status bar parts
    int statusBarPartsNum = 2;
    int statusBarPartWidth = windowWidth / statusBarPartsNum;
    int statusBarRightEdge = statusBarPartWidth;
    std::vector<int> statusBarParts(statusBarPartsNum);

    for (int i = 0; i < statusBarPartsNum; i++) {
        statusBarParts[i] = statusBarRightEdge;
        statusBarRightEdge += statusBarPartWidth;
    }

    SendMessage(hStatusBar,
        SB_SETPARTS,
        static_cast<WPARAM>(statusBarPartsNum),
        reinterpret_cast<LPARAM>(statusBarParts.data()));

    return 0;
}

LRESULT View::ThemeControls(HWND hWnd) {
    // load default font
    NONCLIENTMETRICS ncm{ sizeof(NONCLIENTMETRICS) };
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0);
    HFONT hFont = CreateFontIndirect(&ncm.lfMessageFont);

    // apply the font to classic controls which do not
    // load the modern font from newer systems
    HWND hButtonSend = GetDlgItem(hWnd, IDC_BUTTON_SEND);
    HWND hEdit = GetDlgItem(hWnd, IDC_EDIT);
    HWND hStaticIntro = GetDlgItem(hWnd, IDC_STATIC_INTRO);
    HWND hGroupboxModel = GetDlgItem(hWnd, IDC_GROUPBOX_MODEL);
    SendMessage(hButtonSend, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), 1);
    SendMessage(hEdit, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), 1);
    SendMessage(hStaticIntro, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), 1);
    SendMessage(hGroupboxModel, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), 1);

    return 0;
}

LRESULT CALLBACK View::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_COMMAND:
            return Controller::Instance().OnCommand(hWnd, wParam);
        case WM_CREATE:
            View::Instance().CenterWindow(hWnd);
            View::Instance().CreateControls(hWnd);
            View::Instance().ThemeControls(hWnd);
            return Controller::Instance().OnCreate(hWnd);
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_INITMENUPOPUP:
            return Controller::Instance().OnInitMenuPopup(hWnd, wParam, lParam);
        case WM_NOTIFY:
            return Controller::Instance().OnNotify(hWnd, lParam);
        case WM_SIZE:
            return View::Instance().LayoutControls(hWnd, lParam);
        case WM_USER_API_RESPONSE:
            return Controller::Instance().OnUserMessage(hWnd, message, wParam, lParam);
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
