#pragma once
#include <windows.h>

#include <commctrl.h>

#include <memory>
#include <vector>

#include "lib/storage.hpp"

/** @brief Image list icon identifers. */
struct ImageListIdentifier {
    int document;
    int folder;
};

/**
 * @class Controller
 * @brief Manages the main application logic.
 */
class Controller {
   public:
    /**
     * @brief Gets the singleton instance.
     * @return Reference to the singleton instance.
     */
    static Controller& Instance();

    /**
     * @brief `WM_COMMAND` Win32 message handler.
     * @param hWnd      Handle to the main window.
     * @param wParam    Additional message information.
     * @return LRESULT Result of message processing.
     */
    LRESULT OnCommand(HWND hWnd, WPARAM wParam);

    /**
     * @brief `WM_CREATE` Win32 message handler.
     * @param hWnd Handle to the main window.
     * @return LRESULT Result of message processing.
     */
    LRESULT OnCreate(HWND hWnd);

    /**
     * @brief `WM_INITMENUPOPUP` Win32 message handler.
     * @param hWnd      Handle to the main window.
     * @param wParam    Additional message information.
     * @param lParam    Additional message information.
     * @return LRESULT Result of message processing.
     */
    LRESULT OnInitMenuPopup(HWND hWnd, WPARAM wParam, LPARAM lParam);

    /**
     * @brief `WM_NOTIFY` Win32 message handler.
     * @param hWnd      Handle to the main window.
     * @param lParam    Additional message information.
     * @return LRESULT Result of message processing.
     */
    LRESULT OnNotify(HWND hWnd, LPARAM lParam);

    /**
     * @brief `WM_EXITSIZEMOVE` Win32 message handler.
     * @param hWnd      Handle to the main window.
     * @param lParam    Additional message information.
     * @return LRESULT Result of message processing.
     */
    LRESULT OnSizeMove(HWND hWnd, LPARAM lParam);

    /**
     * @brief User-defined Win32 message handler.
     * @param hWnd     Handle to the window receiving the message.
     * @param message  The Win32 message identifier.
     * @param wParam   Additional message information.
     * @param lParam   Additional message information.
     * @return LRESULT Result of message processing.
     */
    LRESULT OnUserMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

   private:
    /** @brief Prevents external instantiation of the class. */
    Controller() = default;

    /** @brief Holds the indices of icons in the image list. */
    ImageListIdentifier icons;

    /**
     * @brief Adds a generic item to the tree view.
     * @param hWnd      Handle to the main window.
     * @param hParent   Handler to parent node.
     * @return HTREEITEM Handle to the tree item.
     */
    HTREEITEM AddTreeItem(HWND hWnd, HTREEITEM hParent);

    /**
     * @brief Adds a project to the tree view.
     * @param hWnd      Handle to the main window.
     * @param project   The project.
     * @return HTREEITEM Handle to the tree item.
     */
    HTREEITEM AddTreeItem(HWND hWnd, const std::unique_ptr<lib::storage::Project>& project);

    /**
     * @brief Adds a chat (or child node) to the tree view
     * @param hWnd      Handle to the main window.
     * @param chat      The chat.
     * @param hParent   Handler to parent node.
     * @return HTREEITEM Handle to the tree item.
     */
    HTREEITEM AddTreeItem(HWND hWnd, const std::unique_ptr<lib::storage::Chat>& chat, HTREEITEM hParent);
};
