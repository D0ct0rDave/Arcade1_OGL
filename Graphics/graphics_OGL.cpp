#include "graphics.h"

#include "OGL/OGL.h"
#include "OGL/OGL_TexDB.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <gl/gl.h>
#include <gl/glu.h>
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdarg.h>

// -----------------------------------------------------------------------------
// Create handles for three graphics, the screen, a buffer, and the graphics we have made ourself,

//-----------------------------------------------------------------------------
// Global data
//-----------------------------------------------------------------------------
bool						gbWindowed		= true;
HGLRC						gGLRC			= NULL;
POINT						goScrOfs;					// Screen offset
HWND						ghWnd			= NULL;		// Window handler
HDC							ghDC			= NULL;		// Display Context

COGL_TexDB					gOGLTexDB;

char* *						gszBMPResources = NULL;
int							GFRAMENUM		= 0;
//-----------------------------------------------------------------------------
#ifdef _DEBUG
static GLenum eErr = GL_NO_ERROR;
#define CHECKERROR() eErr=glGetError();
#else
#define CHECKERROR()
#endif
//-----------------------------------------------------------------------------
void GSetResourceTable(char** _pszResTab)
{
	gszBMPResources = _pszResTab;
}
//-----------------------------------------------------------------------------
int GInit(void *pParam)
{
	ghWnd = ((HWND)pParam);
	ghDC  = GetDC(ghWnd);

	gGLRC = OGL_CreateRenderingContext(ghWnd,ghDC,OP_DOUBLE_BUFFERED,32,0);
	CHECKERROR();

	OGL_ActivateRenderingContext(ghDC,gGLRC);
	CHECKERROR();

    goScrOfs.x = goScrOfs.y = 0;
    ClientToScreen( ghWnd, &goScrOfs);

	gOGLTexDB.Init(100);

	// Initialize Matrix Transforms

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(-1.0f,+1.0f,0.0f);
	glScalef    ( 2.0f/(float)SCREENWIDTH,-2.0f/(float)SCREENHEIGHT,1.0f);
	
	CHECKERROR();

	glEnable(GL_TEXTURE_2D);
	CHECKERROR();

	glEnable(GL_BLEND);
	CHECKERROR();

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	CHECKERROR();

	glEnable(GL_ALPHA_TEST);
	CHECKERROR();
	
	glAlphaFunc(GL_GREATER,0.0f);
	CHECKERROR();
	
	glClearColor(0,0,0,1);
    CHECKERROR();

	glClearDepth(1.0f);            
	CHECKERROR();
    
	glClearStencil(0);
	CHECKERROR();

	glDisable(GL_DEPTH_TEST);
	CHECKERROR();

    glDepthFunc(GL_ALWAYS);  
	CHECKERROR();

    glEnable(GL_NORMALIZE);
	CHECKERROR();

	glShadeModel(GL_SMOOTH); 
	CHECKERROR();

    glViewport(0, 0, SCREENWIDTH,SCREENHEIGHT);
	CHECKERROR();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	CHECKERROR();

	glCullFace(GL_BACK);
	CHECKERROR();

	glDisable(GL_LIGHTING);
	CHECKERROR();
	
	// Init the frame number
	GFRAMENUM = 0;
	return(1);
}
//-----------------------------------------------------------------------------
void GFinish()
{
	gOGLTexDB.Finish();
	
	OGL_DeactivateRenderingContext();
	OGL_DestroyRenderingContext(gGLRC);
}
// -----------------------------------------------------------------------------
void GBlit()
{
	glFinish();
	SwapBuffers( wglGetCurrentDC() );

	GFRAMENUM++;	// Inc the frame
}
// -----------------------------------------------------------------------------
void GClear()
{
	glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
// -----------------------------------------------------------------------------
void GSetBlend(int _iBlend)
{
	switch (_iBlend)
	{
		case BLENDCOPY:
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		break;

		case BLENDALPHA:
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		break;

		case BLENDADD:
		glBlendFunc(GL_ONE,GL_ONE);
		break;

		case BLENDALPHAADD:
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		break;
	}
}
// -----------------------------------------------------------------------------
void GPutSprite(BMPHandle _SrcHnd,int _iSrcX,int _iSrcY,int _iTX,int _iTY,int _iDstX,int _iDstY,int _iBlend)
{
	RECT				oSrc;
	RECT				oDst;

	Texture* pTex = gOGLTexDB.pGetTexture((int)_SrcHnd);

	if ((_iTX == BMPWIDTH) || (_iTY == BMPHEIGHT))
	{
		if (_iTX == BMPWIDTH)  _iTX = pTex->TX;
		if (_iTY == BMPHEIGHT) _iTY = pTex->TY;
	}

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
	GLuint iTex = gOGLTexDB.GetTexture( (int)_SrcHnd );
	glBindTexture(GL_TEXTURE_2D,iTex);
	CHECKERROR();

	float fIU = (float)oSrc.left  / (float)pTex->TX;
	float fIV = (float)oSrc.top   / (float)pTex->TY;
	float fFU = (float)oSrc.right / (float)pTex->TX;
	float fFV = (float)oSrc.bottom/ (float)pTex->TY;

	float fIX = oDst.left;
	float fIY = oDst.top;
	float fFX = oDst.right;
	float fFY = oDst.bottom;

	GSetBlend(_iBlend);
	
	glBegin(GL_QUADS);

		glColor4f(1,1,1,1);
		glTexCoord2f(fIU,fIV);
		glVertex3f(fIX,fIY,1.0f);
		
		glColor4f(1,1,1,1);
		glTexCoord2f(fIU,fFV);
		glVertex3f(fIX,fFY,1.0f);
		
		glColor4f(1,1,1,1);
		glTexCoord2f(fFU,fFV);
		glVertex3f(fFX,fFY,1.0f);

		glColor4f(1,1,1,1);
		glTexCoord2f(fFU,fIV);
		glVertex3f(fFX,fIY,1.0f);

	glEnd();
	CHECKERROR();
}
// -----------------------------------------------------------------------------
BMPHandle GLoadBitmap(int _iTX,int _iTY,int _iResourceID)
{
	if (! gszBMPResources) return((void*)-1);
	if (! gszBMPResources[_iResourceID])  return((void*)-1);


	char szStr[1024];
	sprintf(szStr,"%s.tga",gszBMPResources[_iResourceID]);
	
	int x, y, n;
	unsigned char *data = stbi_load(szStr, &x, &y, &n, 0);

	if (data == NULL)
	{
		return ((void*)-1);
	}

	Texture* oTex = (Texture*)malloc(sizeof(Texture));
	oTex->data = data;
	oTex->TX = x;
	oTex->TY = y;
	oTex->channels = n;

	int iRes = gOGLTexDB.iAddTexture(oTex);	

	return ( (BMPHandle)iRes);
}
// -----------------------------------------------------------------------------
void GFreeBitmap(BMPHandle _SrcHnd)
{
}
// -----------------------------------------------------------------------------
void GTextXY(char *str,int x,int y,GAlignment align,Boolean black)
{
}
// -----------------------------------------------------------------------------
unsigned int GGetPixel(BMPHandle _SrcHnd,int _iX,int _iY)
{
	Texture* pTex = gOGLTexDB.pGetTexture((int)_SrcHnd);
	unsigned int color = *(unsigned int *)(pTex->data + _iY * (pTex->TX * pTex->channels) + _iX);
	return(color);
}
// -----------------------------------------------------------------------------
void GDrawMesh(BMPHandle _SrcHnd,unsigned short* _pusIdx,float* _pfPoints,float* _pfUVs,unsigned int* _puiColors,unsigned int _uiPoints,int _iBlend)
{
	if (_pfUVs)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 2*sizeof(GLfloat), _pfUVs);
	}
	else
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    if (_puiColors)
    {
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_UNSIGNED_BYTE, 4, _puiColors);
	}
	else
		glDisableClientState(GL_COLOR_ARRAY);

    if (_pfPoints)
    {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 3*sizeof(GLfloat), _pfPoints);
	}
	else
		glDisableClientState(GL_VERTEX_ARRAY);

	// Blit
	GLuint iTex = gOGLTexDB.GetTexture( (int)_SrcHnd );
	glBindTexture(GL_TEXTURE_2D,iTex);
	CHECKERROR();

   	GSetBlend(_iBlend);

    if (_pusIdx)
		glDrawElements(GL_TRIANGLES, _uiPoints, GL_UNSIGNED_SHORT, _pusIdx);
	else
		glDrawArrays(GL_TRIANGLES,0,_uiPoints);		
}
// -----------------------------------------------------------------------------