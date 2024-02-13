#pragma once
#include <stdbool.h>

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef DWORD   COLORREF;

bool AddIconToTaskBar(void* hwnd);
bool RemoveIconToTaskBar(void* hwnd);

void CaptureMousePosition(int* x, int* y);

void SetWindowProcHandle(void* hwnd);
void RegisterHotKeys(void* hwnd);

void DoPocus();
void DoExit();

