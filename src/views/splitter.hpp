#pragma once
#include <windows.h>

/** @brief Tracks mouse movement and splitter state. */
struct SplitterState {
    bool dragging;
    POINT offset;
    POINT origin;
};

/**
 * @class Splitter
 * @brief A subclass of a static control.
 */
class Splitter {
   public:
    /** @brief Track positioning information. */
    SplitterState state = { .dragging = false, .offset = { 0, 0 }, .origin = { 0, 0 } };

    /**
     * @brief Gets the singleton instance.
     * @return Reference to the singleton instance.
     */
    static Splitter& Instance();

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

   private:
    /** @brief Prevents external instantiation of the class. */
    Splitter() = default;
};
