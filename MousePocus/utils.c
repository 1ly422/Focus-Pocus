#include "utils.h"

#define WIN32_MEAN_AND_LEAN
#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>


// Makes a window transparent by setting a transparency color.
bool MakeWindowTransparent(COLORREF colorKey) {

    HWND hWnd = GetWindowHandle();

    // Change window type to layered (https://stackoverflow.com/a/3970218/3357935)
    SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

    // Set transparency color
    return SetLayeredWindowAttributes(hWnd, colorKey, 0, LWA_COLORKEY);
}

enum TrayIcon {
    ID = 13, CALLBACKID = WM_APP + 1
};

bool AddIconToTaskBar(void* hwnd) {
    
    NOTIFYICONDATA nid;
    memset(&nid, 0, sizeof(nid));
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = CALLBACKID;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    const wchar_t* title = L"Focus Pocus";
    wcscpy_s(nid.szTip, 128 * sizeof(wchar_t), title);
    Shell_NotifyIcon(NIM_ADD, &nid);
}

bool RemoveIconToTaskBar(void* hwnd) {
    NOTIFYICONDATA nid;
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = /*TrayIcon::*/ID;
    return Shell_NotifyIcon(NIM_DELETE, &nid);
}

void CaptureMousePosition(int* x, int* y){
    POINT point;
    GetCursorPos(&point);
    *x = point.x;
    *y = point.y;
}

void RegisterAltB() {
    if (RegisterHotKey( NULL, 1, MOD_ALT | MOD_NOREPEAT, 0x42))  //0x42 is 'b'
    {
        printf("Hotkey 'ALT+b' registered, using MOD_NOREPEAT flag\n");
    }
    else {
        printf("STOP");
        exit(0);
    }

    if (RegisterHotKey(NULL, 2, MOD_CONTROL | MOD_NOREPEAT, VK_SPACE))
    {
        printf("Hotkey 'CTRL SPACE' registered, using MOD_NOREPEAT flag\n");
    }
    else {
        printf("STOP");
        exit(0);
    }
}

static MSG msg = {0};
void ListenToHotKey(bool* shouldPocus, bool* shouldExit) {
    
    if (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        switch (msg.message)
        {
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_KEYDOWN:
            // 
            // Perform any required cleanup. 
            // 
            printf("HTBTBT\n");
        case WM_HOTKEY:
        {
            printf("wParam: %lu\n", msg.wParam);
            if (msg.wParam == 1) {
                *shouldExit = true;
                *shouldPocus = false;
            }
            else if (msg.wParam == 2) {
                *shouldPocus = !*shouldPocus;
            }
        }
        break;

        }
        DispatchMessage(&msg);
    }
    /*
    if (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        if (msg.message == WM_HOTKEY) {
            printf("wParam: %lu\n", msg.wParam);
            if (msg.wParam == 1) {
                *shouldExit = true;
                *shouldPocus = false;
            }
            else if (msg.wParam == 2) {
                *shouldPocus = !*shouldPocus;
            }
        }
        //DispatchMessage(&msg);
    }
    */
}

// Function to handle messages
static LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_USER + 1:
        switch (lParam) {
        case WM_RBUTTONUP:
            POINT p;
            GetCursorPos(&p);
            HMENU hPopupMenu = CreatePopupMenu();
            InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, 1, L"Exit");
            SetForegroundWindow(hWnd);
            TrackPopupMenu(hPopupMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y, 0, hWnd, NULL);
            PostMessage(hWnd, WM_NULL, 0, 0);
            DestroyMenu(hPopupMenu);
            break;
        }
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1:
            //appRunning = false;
            break;
        }
        break;
    case WM_CLOSE:
        //appRunning = false;
        break;
    case WM_HOTKEY:
        switch (wParam) {
        case 1: // Hotkey for showing the window
            printf("Khello\n");
            //toggleWindowVisibility(hWnd, true);
            break;
        case 2: // Hotkey for hiding the window
            printf("Asta la vista!\n");
            //toggleWindowVisibility(hWnd, false);
            break;
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
