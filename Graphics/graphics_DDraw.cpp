#include "graphics.h"

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
#include "ddutil.h"

// -----------------------------------------------------------------------------
// Create handles for three graphics, the screen, a buffer, and the graphics we have made ourself,

//-----------------------------------------------------------------------------
// Global data
//-----------------------------------------------------------------------------
bool						gbWindowed		= true;
LPDIRECTDRAW4               gpoDD			= NULL;     // DirectDraw object
LPDIRECTDRAWSURFACE4        gpoDDSFront  	= NULL;		// DirectDraw primary surface
LPDIRECTDRAWSURFACE4        gpoDDSBack		= NULL;		// DirectDraw primary surface
LPDIRECTDRAWCLIPPER         gpoClipper		= NULL;		// Clipper for primary
POINT						goScrOfs;					// Screen offset
HWND						ghWnd			= NULL;		// Window handler
// -----------------------------------------------------------------------------

int			GFRAMENUM;

// -----------------------------------------------------------------------------
// draw a bitmap at x,y
// This function is not used directly, but we have made a 'wrapper' function to make it neater.
/*
void DrawBitmap(Int x, Int y, Int id)
{
    VoidHand h;
    BitmapPtr p;
    h = DmGet1Resource('Tbmp', id);
    if (h != NULL) {
        p = (BitmapPtr) MemHandleLock(h);
        WinDrawBitmap(p, x, y);
        MemHandleUnlock(h);
        DmReleaseResource(h);
    }
}
*/
//-----------------------------------------------------------------------------
void GSetResourceTable(char** _pszResTab)
{
	DDSetResourceTable(_pszResTab);
}
//-----------------------------------------------------------------------------
// Name: ReleaseAllObjects()
// Desc: Finished with all objects we use; release them
//-----------------------------------------------------------------------------
 static void ReleaseAllObjects()
 {
    if (gpoDD != NULL)
    {
        if (gpoDDSFront != NULL)
        {
            gpoDDSFront->Release();
            gpoDDSFront = NULL;
        }
        if (gpoDDSBack != NULL)
        {
            gpoDDSBack->Release();
            gpoDDSBack= NULL;
        }
        gpoDD->Release();
        gpoDD = NULL;
    }

}
//-----------------------------------------------------------------------------
// Name: InitFail()
// Desc: This function is called if an initialization function fails
//-----------------------------------------------------------------------------
HRESULT
InitFail(HWND hWnd, HRESULT hRet, LPCTSTR szError,...)
{
    char                        szBuff[128];
    va_list                     vl;

    va_start(vl, szError);
    vsprintf(szBuff, szError, vl);
    ReleaseAllObjects();
    MessageBox(hWnd, szBuff, "Graphics.cpp", MB_OK);
    DestroyWindow(hWnd);
    va_end(vl);
    return hRet;
}

// -----------------------------------------------------------------------------
int GInit(void *pParam)
{
    ///////////////////////////////////////////////////////////////////////////
    // Create the main DirectDraw object
    ///////////////////////////////////////////////////////////////////////////
    DDSURFACEDESC2              ddsd;
    HRESULT                     hRet;
    LPDIRECTDRAW                pDD;

	ghWnd = ((HWND)pParam);

    hRet  = DirectDrawCreate(NULL, &pDD, NULL);
    if (hRet != DD_OK)
        return InitFail( ghWnd , hRet, "DirectDrawCreate FAILED");

    // Fetch DirectDraw4 interface
    hRet = pDD->QueryInterface(IID_IDirectDraw4, (LPVOID *) & gpoDD);
    if (hRet != DD_OK)
        return InitFail( ghWnd , hRet, "QueryInterface FAILED");

    // Get normal mode
    hRet = gpoDD->SetCooperativeLevel(ghWnd, DDSCL_NORMAL);
    if (hRet != DD_OK)
        return InitFail( ghWnd, hRet, "SetCooperativeLevel FAILED");

    // Create the primary surface
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize			= sizeof(ddsd);
    ddsd.dwFlags		= DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    hRet = gpoDD->CreateSurface(&ddsd, &gpoDDSFront, NULL);
    if (hRet != DD_OK)
        return InitFail( ghWnd, hRet, "CreateSurface Front FAILED");
    
	// Create the backbuffer surface
    ddsd.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;    
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
    ddsd.dwWidth        = SCREENWIDTH;
    ddsd.dwHeight       = SCREENHEIGHT;
    hRet = gpoDD->CreateSurface(&ddsd, &gpoDDSBack, NULL);
    if (hRet != DD_OK)
        return InitFail( ghWnd, hRet, "CreateSurface Back FAILED");

    // Create a clipper for the primary surface	
    hRet = gpoDD->CreateClipper(0, &gpoClipper, NULL);
    if (hRet != DD_OK)
        return InitFail( ghWnd, hRet, "CreateClipper FAILED");

    hRet = gpoClipper->SetHWnd(0, ghWnd);
    if (hRet != DD_OK)
        return InitFail( ghWnd, hRet, "SetHWnd FAILED");

    hRet = gpoDDSFront->SetClipper(gpoClipper);
    if (hRet != DD_OK)
        return InitFail( ghWnd, hRet, "SetClipper FAILED");
	

    // Create a clipper for the backbuffer surface
    /*
	hRet = gpoDD->CreateClipper(0, &gpoClipper, NULL);
    if (hRet != DD_OK)
        return InitFail( ghWnd, hRet, "CreateClipper FAILED");

    hRet = gpoClipper->SetHWnd(0, NULL);
    if (hRet != DD_OK)
        return InitFail( ghWnd, hRet, "SetHWnd FAILED");

    hRet = gpoDDSBack->SetClipper(gpoClipper);
    if (hRet != DD_OK)
        return InitFail( ghWnd, hRet, "SetClipper FAILED");
	*/

    goScrOfs.x = goScrOfs.y = 0;
    ClientToScreen( ghWnd, &goScrOfs);


	// Init the frame number
	GFRAMENUM = 0;
	
	return(1);
}
// -----------------------------------------------------------------------------
void GFinish()
{
	// Delete the backbuffer
	// WinDeleteWindow(GBACKBUFFER,false);


}
// -----------------------------------------------------------------------------
void GBlit()
{
	// We are in windowed mode so perform a blt from the backbuffer 
    // to the primary, returning any errors like DDERR_SURFACELOST
    HRESULT hr;	
	RECT oDst,oSrc;

	// Recompute every frame new offset screen coordinates
	goScrOfs.x = goScrOfs.y = 0;
    ClientToScreen( ghWnd, &goScrOfs);

	GetClientRect(ghWnd,&oDst);	
	oDst.left	+= goScrOfs.x;
	oDst.top	+= goScrOfs.y;
	oDst.right	+= goScrOfs.x;
	oDst.bottom += goScrOfs.y;

	oSrc.left	= 0;
	oSrc.top	= 0;
	oSrc.right	= SCREENWIDTH;
	oSrc.bottom = SCREENHEIGHT;


    while( 1 ) 
    {
		if (gbWindowed)
			hr = gpoDDSFront->Blt( &oDst, gpoDDSBack,&oSrc, DDBLT_WAIT, NULL );
        else
            hr = gpoDDSFront->Flip( NULL, 0 );

        if( hr == DDERR_SURFACELOST )
        {
            gpoDDSFront->Restore();
            gpoDDSBack->Restore();
        }

        if( hr != DDERR_WASSTILLDRAWING )
		{			
			GFRAMENUM++;	// Inc the frame
			return;
		}
    }
}
// -----------------------------------------------------------------------------
void GTextXY(char *str,int x,int y,GAlignment align,Boolean black)
{
}
// -----------------------------------------------------------------------------
void GClear()
{
	// Fill the back buffer with black, ignoring errors until the flip    

    // Erase the background
    DDBLTFX ddbltfx;
    ZeroMemory( &ddbltfx, sizeof(ddbltfx) );
    ddbltfx.dwSize      = sizeof(ddbltfx);
    ddbltfx.dwFillColor = 0;

    gpoDDSBack->Blt( NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );
}
// -----------------------------------------------------------------------------
void GPutSprite(BMPHandle _SrcHnd,int _iSrcX,int _iSrcY,int _iTX,int _iTY,int _iDstX,int _iDstY,int _iBlend)
{
	RECT				oSrc;
	RECT				oDst;

	if ((_iTX == BMPWIDTH) || (_iTY == BMPHEIGHT))
	{
		DDSURFACEDESC2		DDSurfaceDesc;

		DDSurfaceDesc.dwSize = sizeof(DDSURFACEDESC2);

		HRESULT HRES = ((IDirectDrawSurface4*)_SrcHnd)->GetSurfaceDesc(&DDSurfaceDesc);
		if (HRES != DD_OK) 
			return;

		if (_iTX == BMPWIDTH)  _iTX = DDSurfaceDesc.dwWidth;
		if (_iTY == BMPHEIGHT) _iTY = DDSurfaceDesc.dwHeight;
	}

	// if (_iTY == BMPHEIGHT) _iTY = ((IDirectDrawSurface4*)_SrcHnd)->

	oSrc.left	= _iSrcX;
	oSrc.top	= _iSrcY;
	oSrc.right	= _iSrcX + _iTX;
	oSrc.bottom = _iSrcY + _iTY;

	oDst.left	= _iDstX;
	oDst.top	= _iDstY;
	oDst.right	= _iDstX + _iTX;
	oDst.bottom = _iDstY + _iTY;

	// Horizontal Clipping
	if (oDst.right < 0          ) return;	// left  < 0            also
	if (oDst.left  >=SCREENWIDTH) return;	// right >=SCREENWIDTH  also
	if (oDst.left  < 0)
	{
		oSrc.left += (-oDst.left);
		oDst.left  = 0;
	}
	if (oDst.right >= SCREENWIDTH)
	{
		oSrc.right -=  (oDst.right - (SCREENWIDTH-1));
		oDst.right = SCREENWIDTH-1;
	}

	// Vertical Clipping
	if (oDst.bottom<  0           ) return;	// bottom  < 0             also
	if (oDst.top   >= SCREENHEIGHT) return;	// top     >= SCREENHEIGHT also
	if (oDst.top   < 0)
	{
		oSrc.top += (-oDst.top);
		oDst.top  = 0;
	}
	if (oDst.bottom >= SCREENHEIGHT)
	{
		oSrc.bottom -=  (oDst.bottom - (SCREENHEIGHT-1));
		oDst.bottom = SCREENHEIGHT-1;
	}

	// Blit
	// gpoDDSBack->Blt(&oDst,(IDirectDrawSurface4*)_SrcHnd,&oSrc,0/*DDBLT_KEYSRCOVERRIDE  */,/*&oBlt*/NULL);

	// Blit our sprites with source color keys. 
	switch (_iBlend)
	{
		case BLENDCOPY:
		gpoDDSBack->BltFast(oDst.left,oDst.top,(IDirectDrawSurface4*)_SrcHnd,&oSrc,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		break;

		case BLENDALPHA:
		gpoDDSBack->BltFast(oDst.left,oDst.top,(IDirectDrawSurface4*)_SrcHnd,&oSrc,DDBLTFAST_WAIT);
		break;
	}
}
// -----------------------------------------------------------------------------
BMPHandle GLoadBitmap(int _iResourceID)
{
	return ( (BMPHandle)DDLoadBitmap(gpoDD,_iResourceID,0,0) );
}
// -----------------------------------------------------------------------------
void GFreeBitmap(BMPHandle _SrcHnd)
{
}
// -----------------------------------------------------------------------------
unsigned int GGetPixel(BMPHandle _SrcHnd,int _iX,int _iY)
{
    DDSURFACEDESC2      ddsd;
	DWORD				dw;
    HRESULT				HRES;

	ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize			= sizeof(ddsd);
	HRES = ((IDirectDrawSurface4*)_SrcHnd)->GetSurfaceDesc(&ddsd);

	if ((HRES == DD_OK) && (ddsd.lpSurface))
    {
        dw = *((DWORD *)  ((char*)ddsd.lpSurface + ddsd.lPitch*_iY + _iX*4));
    }

	return( dw );
}
// -----------------------------------------------------------------------------