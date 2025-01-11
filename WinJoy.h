#ifndef WinJoyH
#define WinJoyH

#include <windows.h>

HRESULT UpdateInputState();
HRESULT InitDirectInput(HINSTANCE _hInst,HWND _hWnd );
HRESULT FreeDirectInput();
HRESULT SetAcquire(bool _bAquire);

#endif