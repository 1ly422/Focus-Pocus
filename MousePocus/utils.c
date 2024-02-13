#include "utils.h"

#define WIN32_MEAN_AND_LEAN
#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>
#include "main.h"
#define HOTKEY_LCTRLQ 1
#define HOTKEY_LCTRLE 2


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
    wcscpy_s(nid.szTip, 128, title);
    Shell_NotifyIcon(NIM_ADD, &nid);

    return false;
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
#include <stdio.h>
void RegisterHotKeys(void* hwnd) {
    if (RegisterHotKey(hwnd, HOTKEY_LCTRLQ, MOD_CONTROL | MOD_NOREPEAT, 'Q'))
    {
        printf("Hotkey 'CTRL+Q' registered, using MOD_NOREPEAT flag\n");
    }
    else {
        printf("Error, HotKey 'CTRL+Q' not registered\n");
        exit(0);
    }

    if (RegisterHotKey(hwnd, HOTKEY_LCTRLE, MOD_CONTROL | MOD_NOREPEAT, 'E'))
    {
        printf("Hotkey 'CTRL+E' registered, using MOD_NOREPEAT flag\n");
    }
    else {
        printf("Error, HotKey 'CTRL+E' not registered\n");
        exit(0);
    }
}

void DoPocus() {

    bool* shouldPocus = should_pocus();
    printf("CTRL+Q\n");
    *shouldPocus = !*shouldPocus;
}

void DoExit() {

    bool* shouldPocus = should_pocus();
    bool* shouldExit = should_exit();

    printf("CTRL+E\n");
    *shouldExit = true;
    *shouldPocus = false;

}

static LONG_PTR OldWinProc = 0;

static
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_COMMAND:
        // Handle WM_COMMAND message
        switch (LOWORD(wParam)) {
            // Process specific commands here
        }
        break;
    case WM_HOTKEY:
        printf("Imma called\n");
        switch (wParam)
        {
        case HOTKEY_LCTRLQ:
            DoPocus();
            break;
        case HOTKEY_LCTRLE:
            DoExit();
            break;

        }
        break;
    }
    return CallWindowProc((WNDPROC)OldWinProc, hwnd, uMsg, wParam, lParam);
}

void SetWindowProcHandle(void* hwnd) {
    OldWinProc = SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)WindowProc);
}
