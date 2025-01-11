//---------------------------------------------------------------------------
// #include <mem.h>
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "OGL.h"
#include <stdlib.h>

int FirstContext=1;
int LastPixelFormat=0;
//---------------------------------------------------------------------------
// Function prototypes
//---------------------------------------------------------------------------
void ReadImplementationProperties();
int SetupPixelFormat(HDC hDC,HWND hWnd);
//-----------------------------------------------------------------------------
#ifdef _DEBUG
static GLenum eErr = GL_NO_ERROR;
#define CHECKERROR(xxx) eErr=glGetError(); xxx
#else
#define CHECKERROR(xxx) xxx
#endif
//---------------------------------------------------------------------------
HGLRC OGL_CreateRenderingContext(HWND wnd,HDC dc,TRCOptions Options,int ColorDepth,unsigned char StencilBits)
{
	if (! SetupPixelFormat(dc,wnd))
	{	 
		// Error, can't find the correct pixel format
		PostQuitMessage(0);
	}

	HGLRC glrc = wglCreateContext(dc);
	wglMakeCurrent(dc, glrc);
	
	/*
	// Set the OpenGL properties required to draw to the given canvas and
	// create a rendering context for it.

	PIXELFORMATDESCRIPTOR PFDescriptor;
    int PixelFormat;
    static HGLRC Result=0;

    memset((void *)&PFDescriptor,0,sizeof(PIXELFORMATDESCRIPTOR ));

    PFDescriptor.nSize     = sizeof(PIXELFORMATDESCRIPTOR );
    PFDescriptor.nVersion  = 1;
    PFDescriptor.dwFlags   = PFD_SUPPORT_OPENGL;

    if (GetObjectType(DC) == OBJ_MEMDC)
    	OP_SetOption(PFDescriptor.dwFlags,PFD_DRAW_TO_BITMAP);
    else
    	OP_SetOption(PFDescriptor.dwFlags,PFD_DRAW_TO_WINDOW);

    if (OP_IsOption(Options,OP_DOUBLE_BUFFERED)) OP_SetOption(PFDescriptor.dwFlags,PFD_DOUBLEBUFFER);
    if (OP_IsOption(Options,OP_GDI))             OP_SetOption(PFDescriptor.dwFlags,PFD_SUPPORT_GDI);
    if (OP_IsOption(Options,OP_STEREO))          OP_SetOption(PFDescriptor.dwFlags,PFD_STEREO);

    PFDescriptor.iPixelType   = PFD_TYPE_RGBA;
    PFDescriptor.cColorBits   = ColorDepth;
    PFDescriptor.cDepthBits   = 32;
    PFDescriptor.cStencilBits = StencilBits;
    PFDescriptor.iLayerType   = char(PFD_MAIN_PLANE);

  	// just in case it didn't happen already
    PixelFormat = ChoosePixelFormat(DC,&PFDescriptor);
    SetPixelFormat(DC,PixelFormat,&PFDescriptor);

    // check the properties just set
    DescribePixelFormat(DC,PixelFormat,sizeof(PIXELFORMATDESCRIPTOR ),&PFDescriptor);
	Result = wglCreateContext(DC);
	CHECKERROR();

 	// read implementation properties
  	if (FirstContext && (Result > 0))
    {
    	wglMakeCurrent(DC,Result);
    	CHECKERROR();

    	ReadImplementationProperties();    	

    	PixelFormat = FirstContext = false;
    	wglMakeCurrent(0,0);
    	CHECKERROR();
    }
	*/

    return(glrc);
}
//------------------------------------------------------------------------------
void OGL_ActivateRenderingContext(HDC DC,HGLRC RC)
{
  	wglMakeCurrent(DC,RC);
	CHECKERROR();
}
//------------------------------------------------------------------------------
void OGL_DeactivateRenderingContext()
{
	wglMakeCurrent(0,0);
	CHECKERROR();
}

//------------------------------------------------------------------------------

void OGL_DestroyRenderingContext(HGLRC RC)
{
  if (RC) wglDeleteContext(RC);
}

//------------------------------------------------------------------------------
void ReadImplementationProperties()
{
    static const unsigned char *Buffer;
    unsigned char MajorVersion,    MinorVersion,   Separator;

    // determine version of implementation
    // -- first GL
  	Buffer = glGetString(GL_VERSION);
  	CHECKERROR();
}

int SetupPixelFormat(HDC hDC,HWND hWnd)
{
		if (! hWnd)
		{
			PIXELFORMATDESCRIPTOR *ppfd;
			PIXELFORMATDESCRIPTOR  pfd;			
			int  iPixelFormat;
 
			// get the current pixel format index 
			iPixelFormat = GetPixelFormat(hDC); 
 
			// obtain a detailed description of that pixel format 
			DescribePixelFormat(hDC, iPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd); 

			// 
			pfd.cStencilBits = 8; 
			pfd.dwFlags = PFD_DRAW_TO_WINDOW		// support window 
						  | PFD_DOUBLEBUFFER		// double buffered 
						  | PFD_SUPPORT_OPENGL;		// support OpenGL 
						  
			ppfd = &pfd;
 
			if ( (iPixelFormat = ChoosePixelFormat(hDC, ppfd)) == 0 ) 
			{ 
				MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
				return 0; 
			} 
 
				return 1; 
		}
		else
		{
			PIXELFORMATDESCRIPTOR *ppfd; 			
			int pixelformat; 
 
			PIXELFORMATDESCRIPTOR pfd = { 
			sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd 
			1,								// version number 

			PFD_DRAW_TO_WINDOW				// support window
			| PFD_DOUBLEBUFFER				// double buffered
			| PFD_SUPPORT_OPENGL,			// support OpenGL
			

			PFD_TYPE_RGBA,         // RGBA type 
			32,                    // 16-bit color depth 
			0, 0, 0, 0, 0, 0,      // color bits ignored 
			8,                     // no alpha buffer 
			0,                     // shift bit ignored 
			0,                     // no accumulation buffer 
			0, 0, 0, 0,            // accum bits ignored 
			32,                    // 0-bit z-buffer	  (2D MODE ON)
			8,                     // no stencil buffer 
			0,                     // no auxiliary buffer 
			PFD_MAIN_PLANE,        // main layer 
			0,                     // reserved 
			0, 0, 0                // layer masks ignored 
			}; 

			pfd.cColorBits = GetDeviceCaps(hDC,BITSPIXEL);
			pfd.cStencilBits = 8; 
			ppfd = &pfd;
 
			if ( (pixelformat = ChoosePixelFormat(hDC, ppfd)) == 0 ) 
			{ 
				MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
				return 0; 
			} 
 
			if (SetPixelFormat(hDC, pixelformat, ppfd) == FALSE) 
			{ 
				MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
				return 0; 
			}
		}

	    return 1;
}
//------------------------------------------------------------------------------
