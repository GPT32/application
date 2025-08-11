#pragma once
#include <windows.h>

#include "lib/storage.hpp"

/**
 * @class Chat
 * @brief A subclass of the `RichEdit50W` control that renders the chat.
 *
 * Allows fine-grained control into the `WM_PAINT` message in order
 * to auto-scroll to the bottom after rendering is completed.
 */
class Chat {
   public:
    /**
     * @brief Gets the singleton instance.
     * @return Reference to the singleton instance.
     */
    static Chat& Instance();

    /**
     * @brief Process messages sent to the control subclass.
     * @param hWnd          Handle to the window receiving the message.
     * @param message       The Win32 message identifier.
     * @param wParam        Additional message information.
     * @param lParam        Additional message information.
     * @param uIdSubClass   Subclass ID.
     * @param dwRefData     Reference data pointer.
     * @return LRESULT Result of message processing.
     */
    static LRESULT CALLBACK WndProc(
        HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubClass, DWORD_PTR dwRefData);

    /**
     * @brief Creates the control.
     * @param hWnd Handle to the window.
     * @return HWND Handle to the newly created window.
     */
    HWND CreateControl(HWND hWnd);

    /**
     * @brief `WM_USER_API_RESPONSE` Win32 message handler.
     * @param hWnd      Handle to the main window.
     * @param wParam    Additional message information.
     * @param lParam    Additional message information.
     * @return LRESULT Result of message processing.
     */
    LRESULT OnApiResponse(HWND hWnd, WPARAM wParam, LPARAM lParam);

    /**
     * @brief Renders chat messages to the rich edit control.
     * @param hWnd  Handle to the main window.
     * @param chat  The chat.
     */
    void RenderMessages(HWND hWnd, lib::storage::Chat* chat);

    /**
     * @brief Sends user text to chat.
     * @param hWnd Handle to the main window.
     */
    void SendUserChat(HWND hWnd);

   private:
    /** @brief Prevents external instantiation of the class. */
    Chat() = default;
};
