//-----------------------------------------------------------------------------
// File: Stretch.CPP
//
// Desc: This program demonstrated clipped blting and stretched clipped blting.
//       It is a non-exclusive mode application that displays a rotating donut
//       in a window.  Clipped blting can be demonstrated by moving another
//       window partially or completely in front of the stretch window.  The
//       rotating donut does not overwrite the clipping window.
//
//       The size of the rotating donut can be changed with menu selections.
//       Any other size than 1x1 demonstrates stretch blting.  The window can
//       also be resized by grabbing any one of the corners with the mouse.
//
//       Another menu option can be used to change the rate of rotation of the
//       donut.
//
//       This is not an exclusive mode application and so it is incapable of
//       setting the display mode.  Therefore, it must be executed on an 8 bit
//       per pixel display.  It will not work correctly with other pixel
//       depths.
//
// Copyright (c) 1995-1998 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <ddraw.h>
#include "resource.h"
#include "Levels/Entities/Player.h"
#include "graphics/graphics.h"
#include "graphics/gmap.h"
#include "input/input.h"
#include "winjoy.h"

#include "Menu/menu.h"
#include "Game/Game.h"
#include "Demo/Demo.h"
//-----------------------------------------------------------------------------
// Local definitions
//-----------------------------------------------------------------------------
#define NAME                "CeruleanProject"
#define TITLE               "Cerulean project"

//-----------------------------------------------------------------------------
// Default settings
//-----------------------------------------------------------------------------
int menu_is_activated=0; // true when menu is on - used to stop the clock from drawing over the menu
char szChivato[256];

int iPosX = 0;
int iPosY = 0;
int iDeltaX=0;
int iDeltaY=0;
int iOldX;
int iOldY;
int iOldPosX;
int iOldPosY;

// -----------------------------------------------------------------------------
// Function prototypes
// -----------------------------------------------------------------------------
void HandleInput();
void HandleEntities();
void DrawWorld();
void DrawMenu();

//-----------------------------------------------------------------------------
// Global data
//-----------------------------------------------------------------------------
/*
LPDIRECTDRAW4               g_pDD = NULL;        // DirectDraw object
LPDIRECTDRAWSURFACE4        g_pDDSPrimary = NULL;// DirectDraw primary surface
LPDIRECTDRAWSURFACE4        g_pDDSOne = NULL;    // Offscreen surface 1
LPDIRECTDRAWCLIPPER         g_pClipper = NULL;   // Clipper for primary
LPDIRECTDRAWPALETTE         g_pDDPal = NULL;     // The primary surface palette
*/
BOOL                        g_bActive = FALSE;   // Is application active?

//-----------------------------------------------------------------------------
// Local data
//-----------------------------------------------------------------------------
// Name of our bitmap resource.
static char					*szBitmap = "DONUT";
const float					fDeltaMult = 1.0f;
const DWORD					dwUpdateDelay = SCREENDELTAT/fDeltaMult;


#define APPSTATE_MENU				0
#define APPSTATE_GAME				1
#define APPSTATE_DEMO				2
#define APPSTATE_DEMORECORD			3

unsigned int guiAppState = APPSTATE_MENU;
// -----------------------------------------------------------------------------
void DrawMenu()
{
    /*
	VoidHand h;
    BitmapPtr p;

	GClear();
	
    h = DmGet1Resource('Tbmp', _TITLE);
    if (h != NULL)
    {
        p = (BitmapPtr) MemHandleLock(h);
        WinDrawBitmap(p, 0, 0);
        MemHandleUnlock(h);
        DmReleaseResource(h);
    }

    if ((GFRAMENUM >> 4) & 0x01)
	 	GTextXY("press any key",SCREENHWIDTH,SCREENHHEIGHT+32,Centre,1);

 	GBlit();
	*/
}


// -----------------------------------------------------------------------------
void DrawWorld_PlayerNotReady()
{
	GClear();

		// EntitiesDraw();  	
		GTextXY("GAME OVER",SCREENHWIDTH,SCREENHHEIGHT,Centre,1);

 	GBlit();
}
// -----------------------------------------------------------------------------
// 				StartApplication
// Modified 2/10/99 H.Tomlinson.
// -----------------------------------------------------------------------------
static int StartApplication(HINSTANCE _hInst,HWND _hWnd)
{
	InitDirectInput(_hInst,_hWnd );
	GInit((void*)_hWnd);
	
	return 0;
}

// -----------------------------------------------------------------------------
// 				StopApplication
// Modified 2/10/99 H.Tomlinson.
// -----------------------------------------------------------------------------
static void StopApplication(void)
{
	FreeDirectInput();
	GFinish();
}

//-----------------------------------------------------------------------------
// Name: UpdateFrame()
// Desc: Decide what needs to be blitted next, wait for flip to complete,
//       then flip the buffers.
//-----------------------------------------------------------------------------
void UpdateFrame(HWND hWnd)
{
    static DWORD                lastTickCount = 0;
    static int                  currentFrame = 0;
    static BOOL                 haveBackground = FALSE;
    DWORD                       thisTickCount;
	DWORD						deltaTicks;
    RECT                        rcRect;
    RECT                        destRect;
    HRESULT                     hRet;
    POINT                       pt;
	int							iRetState;
	
	// Update timer
	if (lastTickCount == 0)
		lastTickCount = GetTickCount();
		
    thisTickCount = GetTickCount();
	deltaTicks = thisTickCount - lastTickCount;
	 
    if (deltaTicks < dwUpdateDelay)
        return;
	
	#ifdef _DEBUG	
	lastTickCount = thisTickCount;
	#else
	lastTickCount = thisTickCount - (deltaTicks - dwUpdateDelay);
	#endif

	// ------------------
	// Game update
	// ------------------
	switch (guiAppState)
	{
		// -----------------------------------------------------------------
		case APPSTATE_MENU:
		{
			// Update Input
			UpdateInputState();

			iRetState = MENU_iLoop();

			if (iRetState == MENUSTATE_PLAYINGGAME)
				guiAppState  = APPSTATE_GAME;	    
		
		else if (iRetState == MENUSTATE_PLAYINGDEMO)
				guiAppState = APPSTATE_DEMO;

		else if (iRetState == MENUSTATE_RECORDDEMO)
				guiAppState = APPSTATE_DEMORECORD;
		}
		break;


		// -----------------------------------------------------------------
		case APPSTATE_GAME:
		{
			// Update Input
			UpdateInputState();

			iRetState = GAME_iLoop();
			
			if (iRetState == GAMESTATE_GAMEOVER)
				guiAppState = APPSTATE_MENU;
		}
		break;

		// -----------------------------------------------------------------
		case APPSTATE_DEMO:
		{
			iRetState = DEMO_iLoop();
			if (iRetState == DEMOSTATE_DEMOOVER)
				guiAppState = APPSTATE_MENU;
		}
		break;
		
		// -----------------------------------------------------------------
		case APPSTATE_DEMORECORD:
		{
			// Update Input
			UpdateInputState();

			iRetState = DEMO_iLoopRecord();
			if (iRetState == DEMOSTATE_DEMOOVER)
				guiAppState = APPSTATE_MENU;
		}
		break;
	}
	return;
}
//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The Main Window Procedure
//-----------------------------------------------------------------------------
long FAR PASCAL 
WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT                        rc;
    int                         xRight = -1;
    int                         yBottom = -1;
    MINMAXINFO                 *pMinMax;

    switch (message)
    {
        case WM_ACTIVATEAPP:
            // Pause if minimized or not the top window
            g_bActive = (wParam == WA_ACTIVE) || (wParam == WA_CLICKACTIVE);
            return 0L;

		case WM_ACTIVATE:   // sent when window changes active state
            
			// Set exclusive mode access to the joystick based on active state
			SetAcquire( wParam == WA_ACTIVE );
            return 1L;
            break;

        case WM_COMMAND:
            // Handle all menu and accelerator commands 
            switch (LOWORD(wParam))
            {
                case ID_ROTATION_STOP:
                    break;
                case ID_ROTATION_SLOW:                    
                    break;
                case ID_ROTATION_FAST:
                    break;
                case ID_FILE_EXIT:
                    PostMessage(hWnd, WM_CLOSE, 0, 0L);
                    return 0L;

                case ID_SIZE_1X1:
                    /*
					xRight  = SIZEX * 1;
                    yBottom = SIZEY * 1;
					*/
                    break;
                case ID_SIZE_2X1:
                    /*
					xRight = SIZEX * 2;
                    yBottom = SIZEY * 1;
					*/
                    break;
                case ID_SIZE_3X1:
                    /*
					xRight = SIZEX * 3;
                    yBottom = SIZEY * 1;
					*/
                    break;
                case ID_SIZE_1X2:
                    /*
					xRight = SIZEX * 1;
                    yBottom = SIZEY * 2;
					*/
                    break;
                case ID_SIZE_2X2:
                    /*
					xRight = SIZEX * 2;
                    yBottom = SIZEY * 2;
					*/
                    break;
                case ID_SIZE_3X2:
                    /*
					xRight = SIZEX * 3;
                    yBottom = SIZEY * 2;
					*/
                    break;
                case ID_SIZE_1X3:
                    /*
					xRight = SIZEX * 1;
                    yBottom = SIZEY * 3;
					*/
                    break;
                case ID_SIZE_2X3:
                    /*
					xRight = SIZEX * 2;
                    yBottom = SIZEY * 3;
					*/
                    break;
                case ID_SIZE_3X3:
                    /*
					xRight = SIZEX * 3;
                    yBottom = SIZEY * 3;
					*/
                    break;
            }
            if (xRight != -1)
            {
                // Change the window size if set
                SetRect(&rc, 0, 0, xRight, yBottom);
                AdjustWindowRectEx(&rc,
                                   GetWindowLong(hWnd, GWL_STYLE),
                                   GetMenu(hWnd) != NULL,
                                   GetWindowLong(hWnd, GWL_EXSTYLE));
                SetWindowPos(hWnd, NULL, 0, 0, rc.right - rc.left,
                             rc.bottom - rc.top,
                             SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
                return 0L;
            }
            break;

        case WM_DESTROY:
            // Clean up and close the app
            StopApplication();
            PostQuitMessage(0);
            return 0L;

        case WM_GETMINMAXINFO:
            // Fix the minimum size of the window to SIZEX x SIZEY
            /*
			pMinMax = (MINMAXINFO *)lParam;
            pMinMax->ptMinTrackSize.x = SIZEX + GetSystemMetrics(SM_CXSIZEFRAME)*2;
            pMinMax->ptMinTrackSize.y = SIZEY +
                                        GetSystemMetrics(SM_CYSIZEFRAME) * 2 +
                                        GetSystemMetrics(SM_CYCAPTION) +
                                        GetSystemMetrics(SM_CYMENU);
			*/
            return 0L;

        case WM_KEYDOWN:
            // Handle any non-accelerated key commands
            switch (wParam)
            {
                case VK_F12:
                    PostMessage(hWnd, WM_CLOSE, 0, 0);
                    return 0L;

                case VK_ESCAPE: oInput.uiPressed |= CMD_START;	break;
				case VK_UP:		oInput.uiPressed |= CMD_UP;		break;
				case VK_DOWN:	oInput.uiPressed |= CMD_DOWN;	break;
				case VK_LEFT:	oInput.uiPressed |= CMD_LEFT;	break;
				case VK_RIGHT:	oInput.uiPressed |= CMD_RIGHT;	break;
				case VK_SPACE:	oInput.uiPressed |= CMD_ABUTT;	break;

				case 'B':  
				case 'b':		oInput.uiPressed |= CMD_BBUTT;	break;

				case 'M':  
				case 'm':		oInput.uiPressed |= CMD_CBUTT;	break;

				case 'N':  
				case 'n':		oInput.uiPressed |= CMD_DBUTT;	break;
			}
            break;

        case WM_KEYUP:
            // Handle any non-accelerated key commands
            switch (wParam)
            {
                case VK_ESCAPE: oInput.uiPressed &= ~CMD_START;	break;
                case VK_UP:		oInput.uiPressed &= ~CMD_UP;	break;
				case VK_DOWN:	oInput.uiPressed &= ~CMD_DOWN;	break;
				case VK_LEFT:	oInput.uiPressed &= ~CMD_LEFT;	break;
				case VK_RIGHT:	oInput.uiPressed &= ~CMD_RIGHT;	break;
				case VK_SPACE:	oInput.uiPressed &= ~CMD_ABUTT;	break;

				case 'B':  
				case 'b':		oInput.uiPressed &= ~CMD_BBUTT;	break;

				case 'M':  
				case 'm':		oInput.uiPressed &= ~CMD_CBUTT;	break;

				case 'N':  
				case 'n':		oInput.uiPressed &= ~CMD_DBUTT;	break;
			}
            break;

        case WM_PALETTECHANGED:
            if ((HWND) wParam == hWnd)
                break;
            // Fall through to WM_QUERYNEWPALETTE
        case WM_QUERYNEWPALETTE:
            // Install our palette here
            /*
			if (g_pDDPal)
                g_pDDSPrimary->SetPalette(g_pDDPal);
            */
			// Reload the bitmap into the surface because the palette
            // has changed..
            // DDReLoadBitmap(g_pDDSOne, szBitmap);
            return 0L;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

//-----------------------------------------------------------------------------
// Name: InitApp()
// Desc: Do work required for every instance of the application:
//          Create the window, initialize data
//-----------------------------------------------------------------------------
static HRESULT
InitApp(HINSTANCE hInstance, int nCmdShow, HWND *phWnd)
{
    WNDCLASS            wc;
	unsigned int uiWndStyle = WS_POPUP | WS_CAPTION | WS_SYSMENU /*| WS_THICKFRAME*/;

    /*
     * set up and register window class
     */
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName = NULL; // MAKEINTRESOURCE(IDR_MENU);
    wc.lpszClassName = NAME;
    RegisterClass( &wc );


	// Get window rect to get a perfect client rect
	RECT ClientRect;
	ClientRect.left  = 0;
	ClientRect.top   = 0;
	ClientRect.right = SCREENWIDTH;
	ClientRect.bottom= SCREENHEIGHT;

	AdjustWindowRect(&ClientRect,uiWndStyle,false);

    
    /*
     * create a window
     */


    *phWnd = CreateWindowEx(
	0,
	NAME,
	TITLE,
	uiWndStyle,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	ClientRect.right - ClientRect.left,
	ClientRect.bottom - ClientRect.top,
	NULL,
	NULL,
	hInstance,
	NULL );

    if( !*phWnd )
    {
	return FALSE;
    }

    ShowWindow( *phWnd, nCmdShow );
    UpdateWindow( *phWnd );

	StartApplication(hInstance,*phWnd);
}

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Initialization, message loop
//-----------------------------------------------------------------------------
int PASCAL
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow)
{
    MSG                         msg;
    HWND                        hWnd;

    if (InitApp(hInstance, nCmdShow, &hWnd) != DD_OK)
        return FALSE;

    while (TRUE)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (!GetMessage(&msg, NULL, 0, 0))
                return msg.wParam;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else if (g_bActive)
        {
            UpdateFrame(hWnd);

        }
        else
        {
            // Make sure we go to sleep if we have nothing else to do
            WaitMessage();
        }
    }
}
// -----------------------------------------------------------------------------