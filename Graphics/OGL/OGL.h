//---------------------------------------------------------------------------
#ifndef OpenGLH
#define OpenGLH
//---------------------------------------------------------------------------
#include <windows.h>

typedef int TRCOptions;

#define OP_SetOption(variable,option) variable |= option
#define OP_IsOption(variable,option) (variable & option) 

// Opciones para el rendering context

#define OP_DOUBLE_BUFFERED 0x00000001
#define OP_GDI			   0x00000002
#define OP_STEREO          0x00000004

HGLRC OGL_CreateRenderingContext(HWND wnd,HDC dc,TRCOptions Options,int ColorDepth,unsigned char StencilBits);
void  OGL_DestroyRenderingContext(HGLRC RC);
void  OGL_ActivateRenderingContext(HDC DC,HGLRC RC);
void  OGL_DeactivateRenderingContext();

#endif
