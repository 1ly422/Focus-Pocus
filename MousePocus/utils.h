#pragma once
#include <stdbool.h>

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef DWORD   COLORREF;

#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

bool MakeWindowTransparent(COLORREF colorKey);

bool AddIconToTaskBar(void* hwnd);
bool RemoveIconToTaskBar(void* hwnd);

void CaptureMousePosition(int* x, int* y);

void ListenToHotKey(bool* shouldPocus, bool* shouldExit);


