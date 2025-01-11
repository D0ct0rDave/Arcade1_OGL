//-----------------------------------------------------------------------------------------
// ... After about a million of changes...
//	LOG N + 1:			MultiThread support
//	DATE 12-11-2004
//	AUTHOR: DMC
//	To support multithreads rendering contexts must be handled inside the display context
//-----------------------------------------------------------------------------------------


//## begin module%38EB1E6B0157.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%38EB1E6B0157.cm

//## begin module%38EB1E6B0157.cp preserve=no
//## end module%38EB1E6B0157.cp

//## Module: DC_COpenGL%38EB1E6B0157; Pseudo Package body
//## Source file: D:\ProRally\Realization\ENGINE3D\DC\DC_COpenGL.cpp

//## begin module%38EB1E6B0157.additionalIncludes preserve=no
//## end module%38EB1E6B0157.additionalIncludes

//## begin module%38EB1E6B0157.includes preserve=yes
//#pragma message ("windows compile delay")
#include <windows.h> // required by opengl for windows
#include <gl\gl.h>			// REQUIRES OPENGL1.1 / 1.2
#include <gl\glext.h>		// REQUIRES OPENGL1.1 / 1.2
// #define GL_GLEXT_PROTOTYPES

#include <assert.h>
#include "mmry.h"
#include "gMode.h"
//## end module%38EB1E6B0157.includes

// DC_COpenGL
#include "DC_COpenGL.h"
//## begin module%38EB1E6B0157.additionalDeclarations preserve=yes
char g_bPalettedTextures = 0;

DC_COpenGL* gOGLDC = NULL;

#ifdef _DEBUG
static GLenum eErr = GL_NO_ERROR;
#define CHECKERROR(xxx) eErr=glGetError(); xxx
#else
#define CHECKERROR(xxx) xxx
#endif

//-----------------------------------------------------------------------------------------
#ifdef WIN32
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
 
			/*
			if (SetPixelFormat(hDC, iPixelFormat, ppfd) == FALSE) 
			{ 
				DWORD dwError = GetLastError();

				char szBuffer[1024];
				char szBuffer2[1024];
				FormatMessage(0,
										0,
										dwError,
										0,
										szBuffer,
										1024,
										NULL);
	
				MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
			*/
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
#endif
//-----------------------------------------------------------------------------------------

void DC_COpenGL::Init(void *pParam)
{		
	#ifdef WIN32
	m_hWnd  = (HWND)pParam;
	m_hGLDC = GetDC(m_hWnd);

	RECT oR;
	GetClientRect(m_hWnd,&oR);

	SCR_WIDTH  = oR.right - oR.left;
	SCR_HEIGHT = oR.bottom - oR.top;

	if (!SetupPixelFormat(m_hGLDC,m_hWnd))
	{	 
		// Error, can't find the correct pixel format
		PostQuitMessage(0);
	}

	m_hGLRC = wglCreateContext(m_hGLDC);
	wglMakeCurrent(m_hGLDC, m_hGLRC);
	#endif

	// Setup Z-buffer Mechanism 
    glClearIndex((GLfloat) 0);
	CHECKERROR();
    
	glClearColor(0.1f,0.1f,1.0f,0);    
	CHECKERROR();
    
	glClearDepth(1.0f);            
	CHECKERROR();
    
	glClearStencil(0);
	CHECKERROR();

	glEnable(GL_DEPTH_TEST);
	CHECKERROR();

    glDepthFunc(GL_LEQUAL);  
	CHECKERROR();

    glEnable(GL_NORMALIZE);
	CHECKERROR();

    // Enable Gouraud Shading (GL_SMOOTH)
	glShadeModel(GL_SMOOTH); 
	CHECKERROR();

    // Initialize OpenGL Viewport
    glViewport(0, 0, SCR_WIDTH,SCR_HEIGHT);
	CHECKERROR();

    // Initialize Matrix Transforms
    glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); 
 	glLoadIdentity();
	CHECKERROR();

	// pre-allocate texture descriptors
	m_poTexDesc = new  OGL_TextureDesc[DC_COpenGL_MAXTEXTURES];	

	// pre-allocate clut descriptors
	m_poClutDesc = new  OGL_ClutDesc[DC_COpenGL_MAXCLUTS];	

	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	CHECKERROR();

	m_eCullingMode = E3D_ECullFaceMode_BACK;	
	glCullFace(GL_BACK);
	CHECKERROR();

#ifdef WIN32
	// Toni: testing palette support
 		PFNGLCOLORTABLEEXTPROC glColorTableEXT;
		glColorTableEXT = (PFNGLCOLORTABLEEXTPROC)
				wglGetProcAddress("glColorTableEXT");
		
		g_bPalettedTextures = 1;		
		if ((glColorTableEXT == NULL) && (m_hGLRC != NULL))
		{
			//MessageBox(NULL, "Paletted textures are not supported on this video card", "Warning", MB_OK); 
			g_bPalettedTextures = 0;
		}
#endif

	// <JSPG>
	SetTextureGroup(0);

	// DMC
	pLSFunc = NULL;
	glDisable(GL_LIGHTING);
	CHECKERROR();

	float fBlack[4]	= {0.0f,0.0f,0.0f,1.0f};

	glEnable(GL_COLOR_MATERIAL);								// Enable material properties driven by vertex color
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);	// The vertex color drives the diffuse and ambient material properties

	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,	fBlack );
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,	fBlack);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,	fBlack);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,fBlack);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,	fBlack);		
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fBlack);

	for (int i=0;i<MAX_LIGHTS;i++)	
	{
		glLightf(GL_LIGHT0+i,GL_CONSTANT_ATTENUATION,	1.0f);
		glLightf(GL_LIGHT0+i,GL_LINEAR_ATTENUATION,		0.0f);
		glLightf(GL_LIGHT0+i,GL_QUADRATIC_ATTENUATION,	0.0f);
	}

	bLightingEnabled = false;
	glBlendEquation = (PGLBLENDEQUATION)wglGetProcAddress("glBlendEquation");

	glBlendFunc(GL_ONE,GL_ZERO);
	m_eBlendMode = E3D_BM_COPY;
	CHECKERROR();
}

//## end module%38EB1E6B0157.additionalDeclarations


// Class DC_COpenGL 













DC_COpenGL::DC_COpenGL()
  //## begin DC_COpenGL::DC_COpenGL%.hasinit preserve=no
      : m_poTexDesc(NULL), m_iNbTex(0)
  //## end DC_COpenGL::DC_COpenGL%.hasinit
  //## begin DC_COpenGL::DC_COpenGL%.initialization preserve=yes
  //## end DC_COpenGL::DC_COpenGL%.initialization
{
  //## begin DC_COpenGL::DC_COpenGL%.body preserve=yes	






	// DMC: NINGUNA DE LAS INICIALIZACIONES DE OPENGL QUE SE REALIZABAN AQUI FUNCIONABAN
	// PORQUE EN EL MOMENTO DE CREAR EL DISPLAY CONTEXT NO SE CAPTURABA EL DC DE WINDOWS
	// NI SE HABIA CREADO RENDERING CONTEXT DE OPENGL. EN EL INIT SE SUPONE QUE YA SE HA 
	// CREADO EL HGLRC. SE DEBERIA TRASLADAR EL CODIGO DE CREACION DEL HGLRC DENTRO DE ESTE FICHERO
	// QUE ES DEPENDIENTE DE PLATAFORMA (WINDOWS/OPENGL), PERO NO LO HE HECHO POR POSIBLES
	// PROBLEMAS COLATARALES CON OTRAS APLICACIONES (SUR EDITOR/G VIEWER/CAR EDITOR/...)

	




  //## end DC_COpenGL::DC_COpenGL%.body
}


DC_COpenGL::~DC_COpenGL()
{
  //## begin DC_COpenGL::~DC_COpenGL%.body preserve=yes
	if (m_poTexDesc)
	{
		//DestroyTexture (m_poTexDesc);
		delete [] m_poTexDesc;
		delete [] m_poClutDesc;
	}

	#ifdef WIN32
	if (m_hGLRC) wglDeleteContext(m_hGLRC);
	ReleaseDC(m_hWnd,m_hGLDC);
	#endif

  //## end DC_COpenGL::~DC_COpenGL%.body
}



//## Other Operations (implementation)
void DC_COpenGL::LoadMatrix (MTH_CMat4x4& _rMat44)
{
  //## begin DC_COpenGL::LoadMatrix%955007271.body preserve=yes
    glLoadMatrixf(_rMat44.pfGetPointer());
  //## end DC_COpenGL::LoadMatrix%955007271.body
}

void DC_COpenGL::ClearViewport2d ()
{
  //## begin DC_COpenGL::ClearViewport2d%955007272.body preserve=yes
	   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
  //## end DC_COpenGL::ClearViewport2d%955007272.body
}

void DC_COpenGL::PushMatrix ()
{
  //## begin DC_COpenGL::PushMatrix%955007273.body preserve=yes
    glPushMatrix();
  //## end DC_COpenGL::PushMatrix%955007273.body
}

void DC_COpenGL::PopMatrix ()
{
  //## begin DC_COpenGL::PopMatrix%955007274.body preserve=yes
    glPopMatrix();
  //## end DC_COpenGL::PopMatrix%955007274.body
}

void DC_COpenGL::LoadIdentity ()
{
  //## begin DC_COpenGL::LoadIdentity%955007276.body preserve=yes
    glLoadIdentity();
  //## end DC_COpenGL::LoadIdentity%955007276.body
}

void DC_COpenGL::MultMatrix (MTH_CMat4x4& _rMatrix)
{
  //## begin DC_COpenGL::MultMatrix%955007277.body preserve=yes
    glMultMatrixf(_rMatrix.pfGetPointer());
  //## end DC_COpenGL::MultMatrix%955007277.body
}

// ------------------------------------------------------------------------------
void DC_COpenGL::LoadMatrix(DC_EMatrixMode _eMode,MTH_CMat4x4& _rMat44)
{
	DC_EMatrixMode eOldMode = m_eMatrixMode;
	m_eMatrixMode = _eMode;
	LoadMatrix(_rMat44);
	m_eMatrixMode = eOldMode;
}
// ------------------------------------------------------------------------------
void DC_COpenGL::PushMatrix(DC_EMatrixMode _eMode)
{
	DC_EMatrixMode eOldMode = m_eMatrixMode;
	m_eMatrixMode = _eMode;
	PushMatrix();
	m_eMatrixMode = eOldMode;
}
// ------------------------------------------------------------------------------
void DC_COpenGL::PopMatrix(DC_EMatrixMode _eMode)
{
	DC_EMatrixMode eOldMode = m_eMatrixMode;
	m_eMatrixMode = _eMode;
	PopMatrix();
	m_eMatrixMode = eOldMode;
}
// ------------------------------------------------------------------------------
void DC_COpenGL::MultMatrix(DC_EMatrixMode _eMode,MTH_CMat4x4& _rMatrix)
{
	DC_EMatrixMode eOldMode = m_eMatrixMode;
	m_eMatrixMode = _eMode;
	MultMatrix(_rMatrix);
	m_eMatrixMode = eOldMode;
}
// ------------------------------------------------------------------------------
void DC_COpenGL::LoadIdentity(DC_EMatrixMode _eMode)
{
	DC_EMatrixMode eOldMode = m_eMatrixMode;
	m_eMatrixMode = _eMode;
	LoadIdentity();
	m_eMatrixMode = eOldMode;
}
// ------------------------------------------------------------------------------
void DC_COpenGL::GetMatrix (DC_EMatrixMode _eMode,MTH_CMat4x4& _rMat)
{
	DC_EMatrixMode eOldMode = m_eMatrixMode;
	m_eMatrixMode = _eMode;
	GetCurrentMatrix(_rMat);
	m_eMatrixMode = eOldMode;
}
// ------------------------------------------------------------------------------
void DC_COpenGL::Rotate (float _fAngle, float _fX, float _fY, float _fZ)
{
  //## begin DC_COpenGL::Rotate%955007278.body preserve=yes
    glRotatef(_fAngle, _fX, _fY, _fZ);
  //## end DC_COpenGL::Rotate%955007278.body
}

void DC_COpenGL::Scale (float _fX, float _fY, float _fZ)
{
  //## begin DC_COpenGL::Scale%955007279.body preserve=yes
    glScalef(_fX, _fY, _fZ);
  //## end DC_COpenGL::Scale%955007279.body
}

void DC_COpenGL::Translate (float _fX, float _fY, float _fZ)
{
  //## begin DC_COpenGL::Translate%955007280.body preserve=yes
    glTranslatef(_fX, _fY, _fZ);
  //## end DC_COpenGL::Translate%955007280.body
}

void DC_COpenGL::SetViewport2d (E3D_CViewport2D& _rVp2d)
{
  //## begin DC_COpenGL::SetViewport2d%955007282.body preserve=yes
    m_viewport.Copy(_rVp2d);
	
	glEnable(GL_SCISSOR_TEST);
	
	glViewport(_rVp2d.iGetX(), _rVp2d.iGetY(), _rVp2d.iGetWidth(), _rVp2d.iGetHeight() );
	glScissor (_rVp2d.iGetX(), _rVp2d.iGetY(), _rVp2d.iGetWidth(), _rVp2d.iGetHeight() );
  //## end DC_COpenGL::SetViewport2d%955007282.body
}

void DC_COpenGL::PushState ()
{
  //## begin DC_COpenGL::PushState%955007283.body preserve=yes
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

  //## end DC_COpenGL::PushState%955007283.body
}

void DC_COpenGL::PopState ()
{
  //## begin DC_COpenGL::PopState%955007284.body preserve=yes
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
  //## end DC_COpenGL::PopState%955007284.body
}

void DC_COpenGL::GetMatrix (MTH_CMat4x4& _rMat)
{
  //## begin DC_COpenGL::GetMatrix%955007303.body preserve=yes
	GLint iMatrixMode;
	glGetIntegerv(GL_MATRIX_MODE, &iMatrixMode);
	static float s_a16fMatrix[16];
	switch (iMatrixMode)
	{
	case GL_MODELVIEW:
		glGetFloatv(GL_MODELVIEW_MATRIX, s_a16fMatrix);
		break;
	case GL_TEXTURE:
		glGetFloatv(GL_TEXTURE_MATRIX, s_a16fMatrix);
		break;
	case GL_PROJECTION:
		glGetFloatv(GL_PROJECTION_MATRIX, s_a16fMatrix);
		break;
	default:
		// invalid mode!
		assert(0);
		break;
	}    
    _rMat.SetAt(0,0, s_a16fMatrix[0]);
    _rMat.SetAt(1,0, s_a16fMatrix[1]);
    _rMat.SetAt(2,0, s_a16fMatrix[2]);
    _rMat.SetAt(3,0, s_a16fMatrix[3]);

    _rMat.SetAt(0,1, s_a16fMatrix[4]);
    _rMat.SetAt(1,1, s_a16fMatrix[5]);
    _rMat.SetAt(2,1, s_a16fMatrix[6]);
    _rMat.SetAt(3,1, s_a16fMatrix[7]);

    _rMat.SetAt(0,2, s_a16fMatrix[8]);
    _rMat.SetAt(1,2, s_a16fMatrix[9]);
    _rMat.SetAt(2,2, s_a16fMatrix[10]);
    _rMat.SetAt(3,2, s_a16fMatrix[11]);

    _rMat.SetAt(0,3, s_a16fMatrix[12]);
    _rMat.SetAt(1,3, s_a16fMatrix[13]);
    _rMat.SetAt(2,3, s_a16fMatrix[14]);
    _rMat.SetAt(3,3, s_a16fMatrix[15]);
  //## end DC_COpenGL::GetMatrix%955007303.body
}

void DC_COpenGL::SwapBuffers ()
{
  //## begin DC_COpenGL::SwapBuffers%955010534.body preserve=yes

	// glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	
	::SwapBuffers( wglGetCurrentDC() );

    // HDC hDC = GetDC((HWND)m_hWnd);
    // ::SwapBuffers(hDC);
    // ReleaseDC((HWND)m_hWnd, hDC);
  //## end DC_COpenGL::SwapBuffers%955010534.body
}

void DC_COpenGL::BeginStrip ()
{
  //## begin DC_COpenGL::BeginStrip%955091997.body preserve=yes
    glBegin(GL_TRIANGLE_STRIP);
  //## end DC_COpenGL::BeginStrip%955091997.body
}

void DC_COpenGL::EndStrip ()
{
  //## begin DC_COpenGL::EndStrip%955091998.body preserve=yes
    glEnd();
  //## end DC_COpenGL::EndStrip%955091998.body
}

void DC_COpenGL::BeginFan ()
{
  //## begin DC_COpenGL::BeginFan%955091999.body preserve=yes
    glBegin(GL_TRIANGLE_FAN);
  //## end DC_COpenGL::BeginFan%955091999.body
}

void DC_COpenGL::EndFan ()
{
  //## begin DC_COpenGL::EndFan%955092000.body preserve=yes
     glEnd();
  //## end DC_COpenGL::EndFan%955092000.body
}

void DC_COpenGL::Vertex (MTH_CPoint3 &_rPoint)
{
  //## begin DC_COpenGL::Vertex%955092001.body preserve=yes
    glVertex3f(_rPoint.fGetX(), _rPoint.fGetY(), _rPoint.fGetZ());
  //## end DC_COpenGL::Vertex%955092001.body
}

void DC_COpenGL::ClearBuffer (int _eBuffer)
{
  //## begin DC_COpenGL::ClearBuffer%955092002.body preserve=yes
	int iMask = 0;
	if(_eBuffer & E3D_EBuffer_COLOR)
		iMask |= GL_COLOR_BUFFER_BIT;
	if(_eBuffer & E3D_EBuffer_DEPTH)
		iMask |= GL_DEPTH_BUFFER_BIT;
	if(_eBuffer & E3D_EBuffer_ACCUM)
		iMask |= GL_ACCUM_BUFFER_BIT;
	if(_eBuffer & E3D_EBuffer_STENCIL)
		iMask |= GL_STENCIL_BUFFER_BIT;
	
   glClear(iMask);   
  //## end DC_COpenGL::ClearBuffer%955092002.body
}

void DC_COpenGL::SetWindow (/*HWND*/int _hWnd)
{
  //## begin DC_COpenGL::SetWindow%955099841.body preserve=yes
    m_hWnd = (HWND)_hWnd;
  //## end DC_COpenGL::SetWindow%955099841.body
}

void DC_COpenGL::Vertex (float _x, float _y, float _z)
{
  //## begin DC_COpenGL::Vertex%955100772.body preserve=yes
    glVertex3f(_x, _y, _z);
  //## end DC_COpenGL::Vertex%955100772.body
}

void DC_COpenGL::Frustum (float _fLeft, float _fRight, float _fBottom, float _fTop, float _fZnear, float _fZfar)
{
  //## begin DC_COpenGL::Frustum%955356746.body preserve=yes
	GLint iMatrixMode;
	glGetIntegerv(GL_MATRIX_MODE, &iMatrixMode);	
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(_fLeft, _fRight, _fBottom, _fTop, _fZnear, _fZfar);    	
	glMatrixMode(iMatrixMode);
  //## end DC_COpenGL::Frustum%955356746.body
}

void DC_COpenGL::Flush ()
{
  //## begin DC_COpenGL::Flush%955356749.body preserve=yes
    glFlush();
  //## end DC_COpenGL::Flush%955356749.body
}

void DC_COpenGL::BeginTriangle ()
{
  //## begin DC_COpenGL::BeginTriangle%955376251.body preserve=yes
    glBegin(GL_TRIANGLES);
  //## end DC_COpenGL::BeginTriangle%955376251.body
}

void DC_COpenGL::EndTriangle ()
{
  //## begin DC_COpenGL::EndTriangle%955376252.body preserve=yes
    glEnd();
  //## end DC_COpenGL::EndTriangle%955376252.body
}

void DC_COpenGL::GetViewport2d (E3D_CViewport2D& _rVp2d)
{
  //## begin DC_COpenGL::GetViewport2d%956848990.body preserve=yes
    _rVp2d.Copy(m_viewport);
  //## end DC_COpenGL::GetViewport2d%956848990.body
}

void DC_COpenGL::Color (E3D_Color &_roColor)
{
  //## begin DC_COpenGL::Color%956927699.body preserve=yes
    float fRed, fGreen, fBlue,fAlpha;
    _roColor.GetRGBA(&fRed, &fGreen, &fBlue,&fAlpha);
    glColor4f(fRed, fGreen, fBlue,fAlpha);
  //## end DC_COpenGL::Color%956927699.body
}

void DC_COpenGL::Color (float _fR,float _fG,float _fB,float _fA)
{
  //## begin DC_COpenGL::Color%956927699.body preserve=yes
    glColor4f(_fR,_fG,_fB,_fA);
  //## end DC_COpenGL::Color%956927699.body
}

void DC_COpenGL::Color (unsigned int _uiColor)
{
  //## begin DC_COpenGL::Color%956927699.body preserve=yes
    glColor4ubv((GLubyte*)&_uiColor);
  //## end DC_COpenGL::Color%956927699.body
}
void DC_COpenGL::SetVertexArray (float *_pfTable, int _iNbVertex, int _iStride)
{
  //## begin DC_COpenGL::SetVertexArray%956928906.body preserve=yes
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, _iStride, _pfTable);
    //glDisableClientState(GL_VERTEX_ARRAY);
  //## end DC_COpenGL::SetVertexArray%956928906.body
}

void DC_COpenGL::SetColorArray (float *_pfTable, int _iNbColor, int _iStride)
{
  //## begin DC_COpenGL::SetColorArray%956928907.body preserve=yes
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, _iStride, _pfTable);
    //glDisableClientState(GL_COLOR_ARRAY);
  //## end DC_COpenGL::SetColorArray%956928907.body
}


void DC_COpenGL::SetColorArray (int *_piTable, int _iNbColor, int _iStride)
{
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, _iStride, _piTable);
}

void DC_COpenGL::SetNormalArray (float *_pfTable, int _iNbNormal, int _iStride)
{
  //## begin DC_COpenGL::SetNormalArray%956928908.body preserve=yes
    glEnableClientState(GL_NORMAL_ARRAY);    
    glNormalPointer(GL_FLOAT, _iStride, _pfTable); 
    //glDisableClientState(GL_NORMAL_ARRAY);    
  //## end DC_COpenGL::SetNormalArray%956928908.body
}

void DC_COpenGL::SetTexCoordArray (float *_pfTable, int _iNbTexCoord, int _iStride)
{
  //## begin DC_COpenGL::SetTexCoordArray%956928909.body preserve=yes
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);    
    glTexCoordPointer(2, GL_FLOAT, _iStride, _pfTable);    
    //glDisableClientState(GL_TEXCOORD_ARRAY);    
  //## end DC_COpenGL::SetTexCoordArray%956928909.body
}

void DC_COpenGL::DrawIndexedVertexSet (unsigned short *_pwTableIdx, int _iNbVertexIdx, int _iStride, E3D_EFaceType _eFace, E3D_EBinding _eColorBind, E3D_EBinding _eNormBind)
{
  //## begin DC_COpenGL::DrawIndexedVertexSet%957256274.body preserve=yes
    switch (_eFace)
    {
    case E3D_EFaceType_TRISTRIP: // TRIANGLE STRIPS
        glDrawElements(GL_TRIANGLE_STRIP, _iNbVertexIdx, GL_UNSIGNED_SHORT, _pwTableIdx);
        break;
    case E3D_EFaceType_TRIFAN: // TRIANGLE FANS
        glDrawElements(GL_TRIANGLE_FAN, _iNbVertexIdx, GL_UNSIGNED_SHORT, _pwTableIdx);
        break;
    case E3D_EFaceType_TRIANGLE: // TRIANGLES
        glDrawElements(GL_TRIANGLES, _iNbVertexIdx, GL_UNSIGNED_SHORT, _pwTableIdx);
        break;
    case E3D_EFaceType_QUAD: // QUADS
        glDrawElements(GL_QUADS, _iNbVertexIdx, GL_UNSIGNED_SHORT, _pwTableIdx);
        break;
    };
  //## end DC_COpenGL::DrawIndexedVertexSet%957256274.body
}

void DC_COpenGL::GetCurrentMatrix (MTH_CMat4x4& _roMatrix)
{
  //## begin DC_COpenGL::GetCurrentMatrix%957454859.body preserve=yes
    static float s_a16fMatrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, s_a16fMatrix);
    _roMatrix.SetAt(0,0, s_a16fMatrix[0]);
    _roMatrix.SetAt(1,0, s_a16fMatrix[1]);
    _roMatrix.SetAt(2,0, s_a16fMatrix[2]);
    _roMatrix.SetAt(3,0, s_a16fMatrix[3]);

    _roMatrix.SetAt(0,1, s_a16fMatrix[4]);
    _roMatrix.SetAt(1,1, s_a16fMatrix[5]);
    _roMatrix.SetAt(2,1, s_a16fMatrix[6]);
    _roMatrix.SetAt(3,1, s_a16fMatrix[7]);

    _roMatrix.SetAt(0,2, s_a16fMatrix[8]);
    _roMatrix.SetAt(1,2, s_a16fMatrix[9]);
    _roMatrix.SetAt(2,2, s_a16fMatrix[10]);
    _roMatrix.SetAt(3,2, s_a16fMatrix[11]);

    _roMatrix.SetAt(0,3, s_a16fMatrix[12]);
    _roMatrix.SetAt(1,3, s_a16fMatrix[13]);
    _roMatrix.SetAt(2,3, s_a16fMatrix[14]);
    _roMatrix.SetAt(3,3, s_a16fMatrix[15]);
  //## end DC_COpenGL::GetCurrentMatrix%957454859.body
}

void DC_COpenGL::Frustum (float _fFov, float _fZnear, float _fZfar, float _fAspect)
{
  //## begin DC_COpenGL::Frustum%958140738.body preserve=yes
	MTH_CMat4x4 oViewMat;
	GLint iMatrixMode;
	glGetIntegerv(GL_MATRIX_MODE, &iMatrixMode);	
	oViewMat.Frustum(_fFov , _fZnear , _fZfar, _fAspect);    
    glMatrixMode(GL_PROJECTION);    
    glLoadMatrixf(oViewMat.pfGetPointer());     
	glMatrixMode(iMatrixMode);
  //## end DC_COpenGL::Frustum%958140738.body
}

void DC_COpenGL::DestroyTexture (E3D_TextureDesc* _poTexDesc)
{
  //## begin DC_COpenGL::DestroyTexture%958398199.body preserve=yes
	OGL_TextureDesc *poTexDesc;
	for (int i=0; i< DC_COpenGL_MAXTEXTURES; i++)
	{
			poTexDesc = &m_poTexDesc[i];
			if (poTexDesc == _poTexDesc)
			{
				poTexDesc->DestroyTexture();
				return;
			}
	}
  //## end DC_COpenGL::DestroyTexture%958398199.body
}

void DC_COpenGL::SetCurrentTexture (E3D_TextureDesc* _poTexDesc)
{
	if ( !g_bPalettedTextures  && (_poTexDesc->eGetPixelFormat() == E3D_EPixelFormat_PAL8) )
	{
		SetCurrentTexture(_poTexDesc->m_pTexDescRGBA32);
		return;
	}

    #ifdef DEBUG
	    assert( (_poTexDesc) && "Attempting to use a NULL texture descriptor!!!" );		
    #else
		if (!_poTexDesc) return;
    #endif
    
    assert (! _poTexDesc->bIsFree() && "Attempting to use a free texture descriptor");
	_poTexDesc->BindTexture();
}

void DC_COpenGL::TexCoord (float _u, float _v)
{
  //## begin DC_COpenGL::TexCoord%958472417.body preserve=yes
	glTexCoord2f(_u, _v);
  //## end DC_COpenGL::TexCoord%958472417.body
}

void DC_COpenGL::Normal (MTH_CPoint3& _roNorm)
{
  //## begin DC_COpenGL::Normal%958472418.body preserve=yes
	glNormal3f(_roNorm.fGetX(), _roNorm.fGetY(), _roNorm.fGetZ());
  //## end DC_COpenGL::Normal%958472418.body
}

void DC_COpenGL::Enable (E3D_ESetting _eSetting)
{
  //## begin DC_COpenGL::Enable%958549574.body preserve=yes
	switch (_eSetting)
	{
	case E3D_ESetting_TEXTURE:
		glEnable(GL_TEXTURE_2D);
		break;
	case E3D_ESetting_DEPTHTEST:
		glEnable(GL_DEPTH_TEST);
		break;
	case E3D_ESetting_BLEND:
		glEnable(GL_BLEND);
		break;
	case E3D_ESetting_CULLFACE:
            glPolygonMode(GL_FRONT, GL_FILL ); // SOLID
            glPolygonMode(GL_BACK,  GL_FILL ); // SOLID            
			glEnable(GL_CULL_FACE);
		break;
	case E3D_ESetting_LIGHTING:
		glEnable(GL_LIGHTING);
		break;
	case E3D_ESetting_TEXTURE_GEN_T:
		glEnable(GL_TEXTURE_GEN_T);
		break;
	case E3D_ESetting_TEXTURE_GEN_S:
		glEnable(GL_TEXTURE_GEN_S);
		break;
	case E3D_ESetting_FOG:
		glEnable(GL_FOG);
		break;
	case E3D_ESetting_ALPHA_TESTING:
		glEnable(GL_ALPHA_TEST);
		break;
	case E3D_ESetting_LINE_SMOOTH:
		glEnable(GL_LINE_SMOOTH);
		break;
	case E3D_ESetting_POINT_SMOOTH:
		glEnable(GL_POINT_SMOOTH);
		break;
	case E3D_ESetting_POLYGON_SMOOTH:
		glEnable(GL_POLYGON_SMOOTH);
		break;
	case E3D_ESetting_SCISSOR_TEST:
		glEnable(GL_SCISSOR_TEST);
		break;
	}
  //## end DC_COpenGL::Enable%958549574.body
}

void DC_COpenGL::Disable (E3D_ESetting _eSetting)
{
  //## begin DC_COpenGL::Disable%958549575.body preserve=yes
	switch (_eSetting)
	{
	case E3D_ESetting_TEXTURE:
		glDisable(GL_TEXTURE_2D);
		break;
	case E3D_ESetting_DEPTHTEST:
		glDisable(GL_DEPTH_TEST);
		break;
	case E3D_ESetting_BLEND:
		glDisable(GL_BLEND);
		break;
	case E3D_ESetting_CULLFACE:
		glDisable(GL_CULL_FACE);
		break;
	case E3D_ESetting_LIGHTING:
		glDisable(GL_LIGHTING);
		break;
	case E3D_ESetting_TEXTURE_GEN_T:
		glDisable(GL_TEXTURE_GEN_T);
		break;
	case E3D_ESetting_TEXTURE_GEN_S:
		glDisable(GL_TEXTURE_GEN_S);
		break;
	case E3D_ESetting_FOG:
		glDisable(GL_FOG);
		break;
	case E3D_ESetting_ALPHA_TESTING:
		glDisable(GL_ALPHA_TEST);
		break;
	case E3D_ESetting_LINE_SMOOTH:
		glDisable(GL_LINE_SMOOTH);
		break;
	case E3D_ESetting_POINT_SMOOTH:
		glDisable(GL_POINT_SMOOTH);
		break;
	case E3D_ESetting_POLYGON_SMOOTH:
		glDisable(GL_POLYGON_SMOOTH);
		break;
	case E3D_ESetting_SCISSOR_TEST:
		glDisable(GL_SCISSOR_TEST);
		break;
	}
  //## end DC_COpenGL::Disable%958549575.body
}

int DC_COpenGL::bIsEnabled (E3D_ESetting _eSetting)
{
  //## begin DC_COpenGL::bIsEnabled%958549576.body preserve=yes
	switch (_eSetting)
	{
	case E3D_ESetting_TEXTURE:
		return glIsEnabled(GL_TEXTURE_2D);
		break;
	case E3D_ESetting_DEPTHTEST:
		return glIsEnabled(GL_DEPTH_TEST);
		break;
	case E3D_ESetting_BLEND:
		return glIsEnabled(GL_BLEND);
		break;
	case E3D_ESetting_CULLFACE:
		return glIsEnabled(GL_CULL_FACE);
		break;	
	case E3D_ESetting_FOG:
		return glIsEnabled(GL_FOG);
		break;
	case E3D_ESetting_ALPHA_TESTING:
		return glIsEnabled(GL_ALPHA_TEST);
		break;
	case E3D_ESetting_LINE_SMOOTH:
		return glIsEnabled(GL_LINE_SMOOTH);
		break;
	case E3D_ESetting_POINT_SMOOTH:
		return glIsEnabled(GL_POINT_SMOOTH);
		break;
	case E3D_ESetting_POLYGON_SMOOTH:
		return glIsEnabled(GL_POLYGON_SMOOTH);
		break;
	case E3D_ESetting_SCISSOR_TEST:
		return glIsEnabled(GL_SCISSOR_TEST);
		break;
	case E3D_ESetting_LIGHTING:
		return glIsEnabled(GL_LIGHTING);
		break;
	}
	return 0;
  //## end DC_COpenGL::bIsEnabled%958549576.body
}

E3D_TextureDesc* DC_COpenGL::poCreateCLUT (int _iNbEntries, E3D_EPixelFormat _ePixel, unsigned char *_pucImage)
{
  //## begin DC_COpenGL::poCreateCLUT%959155152.body preserve=yes
	OGL_ClutDesc *poClutDesc;
	for (int i=0; i< DC_COpenGL_MAXCLUTS; i++)
	{
		poClutDesc = &m_poClutDesc[i];
		if (poClutDesc->bIsFree())
		{					
			poClutDesc->m_iGroupId = m_iActiveTextureGroup;
			poClutDesc->m_bIsValid = 1;
			poClutDesc->m_pucImage = _pucImage;
			poClutDesc->m_iWidth = 1;
			poClutDesc->m_iHeight = _iNbEntries;
			poClutDesc->m_bIsFree = 0;
			poClutDesc->m_ePixelFmt = _ePixel;

			return poClutDesc;
		}
	}
	
	return NULL;
	
  //## end DC_COpenGL::poCreateCLUT%959155152.body
}

void DC_COpenGL::BlendScaleFactors (E3D_EBlendScale _eSrc, E3D_EBlendScale _eDst)
{
  //## begin DC_COpenGL::BlendScaleFactors%961362827.body preserve=yes
	GLenum sFactor, dFactor;
	switch (_eSrc)
	{
	case E3D_EBlendScale_ONE:
		sFactor = GL_ONE;
		break;
	case E3D_EBlendScale_ZERO:
		sFactor = GL_ZERO;
		break;
	case E3D_EBlendScale_DST_COLOR:
		sFactor = GL_DST_COLOR;
		break;
	case E3D_EBlendScale_SRC_COLOR:
		sFactor = GL_SRC_COLOR;
		break;
	case E3D_EBlendScale_ONE_MINUS_SRC_COLOR:
		sFactor = GL_ONE_MINUS_SRC_COLOR;
		break;
	case E3D_EBlendScale_ONE_MINUS_DST_COLOR:
		sFactor = GL_ONE_MINUS_DST_COLOR;
		break;
	case E3D_EBlendScale_DST_ALPHA:
		sFactor = GL_DST_ALPHA;
		break;
	case E3D_EBlendScale_SRC_ALPHA:
		sFactor = GL_SRC_ALPHA;
		break;
	case E3D_EBlendScale_ONE_MINUS_DST_ALPHA:
		sFactor = GL_ONE_MINUS_DST_ALPHA;
		break;
	case E3D_EBlendScale_ONE_MINUS_SRC_ALPHA: 
		sFactor = GL_ONE_MINUS_SRC_ALPHA;
		break;
	case E3D_EBlendScale_SRC_ALPHA_SATURATE:
		sFactor = GL_SRC_ALPHA_SATURATE;
		break;
	}
	switch (_eDst)
	{
	case E3D_EBlendScale_ONE:
		dFactor = GL_ONE;
		break;
	case E3D_EBlendScale_ZERO:
		dFactor = GL_ZERO;
		break;
	case E3D_EBlendScale_DST_COLOR:
		dFactor = GL_DST_COLOR;
		break;
	case E3D_EBlendScale_SRC_COLOR:
		dFactor = GL_SRC_COLOR;
		break;
	case E3D_EBlendScale_ONE_MINUS_SRC_COLOR:
		dFactor = GL_ONE_MINUS_SRC_COLOR;
		break;
	case E3D_EBlendScale_ONE_MINUS_DST_COLOR:
		dFactor = GL_ONE_MINUS_DST_COLOR;
		break;
	case E3D_EBlendScale_DST_ALPHA:
		dFactor = GL_DST_ALPHA;
		break;
	case E3D_EBlendScale_SRC_ALPHA:
		dFactor = GL_SRC_ALPHA;
		break;
	case E3D_EBlendScale_ONE_MINUS_DST_ALPHA:
		dFactor = GL_ONE_MINUS_DST_ALPHA;
		break;
	case E3D_EBlendScale_ONE_MINUS_SRC_ALPHA: 
		dFactor = GL_ONE_MINUS_SRC_ALPHA;
		break;
	case E3D_EBlendScale_SRC_ALPHA_SATURATE:
		dFactor = GL_SRC_ALPHA_SATURATE;
		break;
	}
	glBlendFunc(sFactor, dFactor);
  //## end DC_COpenGL::BlendScaleFactors%961362827.body
}

void DC_COpenGL::SetMatrixMode (DC_EMatrixMode _eMode)
{
  //## begin DC_COpenGL::SetMatrixMode%975067194.body preserve=yes
	switch (_eMode)
	{
	case DC_EMatrixMode_MODELVIEW:
		glMatrixMode(GL_MODELVIEW);
		break;

	case DC_EMatrixMode_PROJECTION:
		glMatrixMode(GL_PROJECTION);
		break;

	case DC_EMatrixMode_TEXTURE:
		glMatrixMode(GL_TEXTURE);
		break;
	}
  //## end DC_COpenGL::SetMatrixMode%975067194.body
}

DC_EMatrixMode DC_COpenGL::eGetMatrixMode ()
{
  //## begin DC_COpenGL::eGetMatrixMode%975067195.body preserve=yes
	GLint iMatrixMode;
	glGetIntegerv(GL_MATRIX_MODE, &iMatrixMode);
	switch (iMatrixMode)
	{
	case GL_MODELVIEW:
		return DC_EMatrixMode_MODELVIEW;		

	case GL_PROJECTION:
		return DC_EMatrixMode_PROJECTION;

	case GL_TEXTURE:
		return DC_EMatrixMode_TEXTURE;
	}
	return DC_EMatrixMode_MODELVIEW;
  //## end DC_COpenGL::eGetMatrixMode%975067195.body
}

void DC_COpenGL::BeginLine ()
{
  //## begin DC_COpenGL::BeginLine%975067198.body preserve=yes
		glBegin(GL_LINES);   
  //## end DC_COpenGL::BeginLine%975067198.body
}

void DC_COpenGL::BeginLineFan ()
{
  //## begin DC_COpenGL::BeginLineFan%975067199.body preserve=yes
		glBegin(GL_LINES);   
  //## end DC_COpenGL::BeginLineFan%975067199.body
}

void DC_COpenGL::BeginLineStrip ()
{
  //## begin DC_COpenGL::BeginLineStrip%975067200.body preserve=yes
	glBegin(GL_LINE_STRIP); 
  //## end DC_COpenGL::BeginLineStrip%975067200.body
}

void DC_COpenGL::EndLine ()
{
  //## begin DC_COpenGL::EndLine%975067201.body preserve=yes
		glEnd();    
  //## end DC_COpenGL::EndLine%975067201.body
}

void DC_COpenGL::EndLineFan ()
{
  //## begin DC_COpenGL::EndLineFan%975067202.body preserve=yes
		glEnd();    
  //## end DC_COpenGL::EndLineFan%975067202.body
}

void DC_COpenGL::EndLineStrip ()
{
  //## begin DC_COpenGL::EndLineStrip%975067203.body preserve=yes
		glEnd();    
  //## end DC_COpenGL::EndLineStrip%975067203.body
}

void DC_COpenGL::EndPoint ()
{
  //## begin DC_COpenGL::EndPoint%975067204.body preserve=yes
		glEnd();    
  //## end DC_COpenGL::EndPoint%975067204.body
}

void DC_COpenGL::UpdateLight (char _cId)
{
  //## begin DC_COpenGL::UpdateLight%975067207.body preserve=yes
	GLfloat fLight_position[4] = { 0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat fLight_Ambient[4]  = { 0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat fLight_Diffuse[4]  = { 0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat fLight_Specular[4] = { 0.0f, 0.0f, 0.0f, 1.0f};
	MTH_CPoint3 oPosition;
	E3D_Color  oColor;
	float r,g,b,a;

	// Position of light	:	
	m_poPositionalLights[ _cId]->GetPosition(&oPosition);
	fLight_position[0] = oPosition.x;
	fLight_position[1] = oPosition.y;
	fLight_position[2] = oPosition.z;
				
	// Ambient component:
	m_poPositionalLights[ _cId]->GetAmbientComponent (&oColor);
	oColor.GetRGBA(&r,&g,&b,&a);
	fLight_Ambient[0] = r;
	fLight_Ambient[1] = g;
	fLight_Ambient[2] = b;
	
	// Diffuse component:
	m_poPositionalLights[ _cId]->GetDiffuseComponent (&oColor);
	oColor.GetRGBA(&r,&g,&b,&a);
	fLight_Diffuse[0] = r;
	fLight_Diffuse[1] = g;
	fLight_Diffuse[2] = b;

	// Specular component:
	m_poPositionalLights[ _cId]->GetSpecularComponent (&oColor);
	oColor.GetRGBA(&r,&g,&b,&a);
	fLight_Specular[0] = r;
	fLight_Specular[1] = g;
	fLight_Specular[2] = b;
				
	glLightfv (GL_LIGHT1+_cId, GL_POSITION, fLight_position);	
	glLightfv (GL_LIGHT1+_cId, GL_AMBIENT, fLight_Ambient);
	glLightfv (GL_LIGHT1+_cId, GL_DIFFUSE, fLight_Diffuse);
	glLightfv (GL_LIGHT1+_cId, GL_SPECULAR, fLight_Specular);
	
	// Is this light enable?
	if ( m_poPositionalLights[ _cId]->m_ucEnable)
		glEnable(GL_LIGHT1+_cId);	
	else
		glDisable(GL_LIGHT1+_cId);	
  //## end DC_COpenGL::UpdateLight%975067207.body
}

void DC_COpenGL::BeginPoint ()
{
  //## begin DC_COpenGL::BeginPoint%975067208.body preserve=yes
	glBegin(GL_POINTS);
  //## end DC_COpenGL::BeginPoint%975067208.body
}

void DC_COpenGL::GettLight (LIG_PositionalLight* _oPositionalLight, char _cId)
{
  //## begin DC_COpenGL::GettLight%975067209.body preserve=yes
		_oPositionalLight = m_poPositionalLights[ _cId  ];   
  //## end DC_COpenGL::GettLight%975067209.body
}

void DC_COpenGL::unBlindLight (char _cId)
{
  //## begin DC_COpenGL::unBlindLight%975067210.body preserve=yes
	  m_poPositionalLights[ _cId  ] = NULL; 
  //## end DC_COpenGL::unBlindLight%975067210.body
}

char DC_COpenGL::cBlindLight (LIG_PositionalLight* _oPositionalLight)
{
  //## begin DC_COpenGL::cBlindLight%975067211.body preserve=yes
	int i=5,t=0;
		
	while((t<4)&&(i==5))
	{
		if ( m_poPositionalLights[ t] == NULL)
  				i = t;  
		t++;
	}
	
	if (i!=5)
	{
		m_poPositionalLights[ i] = _oPositionalLight;
		UpdateLight ((char)i);
	}
	
	return (char)i;
  //## end DC_COpenGL::cBlindLight%975067211.body
}

void DC_COpenGL::SetTexGenModeS (DC_ETexGenMode _eTexGenMode, float _a4fParam[4])
{
  //## begin DC_COpenGL::SetTexGenModeS%975424988.body preserve=yes
	switch (_eTexGenMode)
	{
	case DC_ETexGenMode_OBJECTLINEAR:
		assert(_a4fParam!=NULL);
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenfv(GL_S, GL_OBJECT_PLANE, _a4fParam);
		break;
	case DC_ETexGenMode_SPHEREMAP:
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		break;
	case DC_ETexGenMode_EYELINEAR:
		assert(_a4fParam!=NULL);
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		glTexGenfv(GL_S, GL_EYE_PLANE, _a4fParam);
		break;
	default:
		// unsupported texture coordinates generation mode
		assert(0);		
		break;
	}
  //## end DC_COpenGL::SetTexGenModeS%975424988.body
}

void DC_COpenGL::SetTexGenModeT (DC_ETexGenMode _eTexGenMode, float _a4fParam[4])
{
  //## begin DC_COpenGL::SetTexGenModeT%975424989.body preserve=yes
	switch (_eTexGenMode)
	{
	case DC_ETexGenMode_OBJECTLINEAR:
		assert(_a4fParam!=NULL);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenfv(GL_T, GL_OBJECT_PLANE, _a4fParam);
		break;
	case DC_ETexGenMode_SPHEREMAP:
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		break;
	case DC_ETexGenMode_EYELINEAR:
		assert(_a4fParam!=NULL);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		glTexGenfv(GL_T, GL_EYE_PLANE, _a4fParam);
		break;
	default:
		// unsupported texture coordinates generation mode
		assert(0);		
		break;
	}
  //## end DC_COpenGL::SetTexGenModeT%975424989.body
}

E3D_TextureDesc* DC_COpenGL::poCreateTexture (int _iWidth, int _iHeight, E3D_EPixelFormat _ePixel, unsigned char *_pucImage, E3D_EWrapMode _eTWrapMode, E3D_EWrapMode _eSWrapMode, E3D_EMagFilter _eMagFilter, E3D_EMinFilter _eMinFilter,int _iNumLODs,unsigned char *_pucPal)
{ 
	OGL_TextureDesc *poTexDesc=NULL;
	for (int i=0; i< DC_COpenGL_MAXTEXTURES; i++)
	{
		poTexDesc = &m_poTexDesc[i];
		if (poTexDesc->bIsFree())
		{					
			poTexDesc->m_iGroupId = m_iActiveTextureGroup;
			
			poTexDesc->bTexImage2D (_ePixel,_iWidth,_iHeight,_pucImage,_iNumLODs);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
			poTexDesc->SetMagFilter(_eMagFilter);
			poTexDesc->SetMinFilter(_eMinFilter);
			poTexDesc->SetSWrapMode(_eSWrapMode);
			poTexDesc->SetTWrapMode(_eTWrapMode);			
			break;
		}
	}
	if (poTexDesc==NULL) return NULL;

	if ( !g_bPalettedTextures  && (_ePixel == E3D_EPixelFormat_PAL8) &&  (_pucPal!=NULL))
	{
		unsigned int *puiImg = (unsigned int *)pAlloc ( _iWidth * _iHeight * 4);
		
		for (int j = 0; j < _iHeight; j++)
			for (int i = 0; i < _iWidth; i++)
			{
				int iIdx = ( j * _iWidth + i);
				unsigned int iPalIdx = _pucImage[ iIdx ];
				puiImg[ iIdx ] = *((unsigned int*)&_pucPal[ iPalIdx*4 ]);
			}

		poTexDesc->m_pTexDescRGBA32=poCreateTexture (_iWidth, _iHeight, E3D_EPixelFormat_RGBA32, (unsigned char *)puiImg, _eTWrapMode, _eSWrapMode, _eMagFilter, _eMinFilter,1);
	}
	else
		poTexDesc->m_pTexDescRGBA32=NULL;

	return poTexDesc;
}

void DC_COpenGL::SetFogColor (E3D_Color &_roColor)
{
  //## begin DC_COpenGL::SetFogColor%983288695.body preserve=yes
	float fFogColor[4];


	 _roColor.GetRGBA(&fFogColor[0],&fFogColor[1],&fFogColor[2],&fFogColor[3]);
	fFogColor[3] = 1.0f;
	glFogfv(GL_FOG_COLOR, fFogColor);

  //## end DC_COpenGL::SetFogColor%983288695.body
}

void DC_COpenGL::SetFogDensity (float _fNear, float _fFar, float _fValue)
{
  //## begin DC_COpenGL::SetFogDensity%983288696.body preserve=yes
	glFogf(GL_FOG_START, _fNear);
	glFogf(GL_FOG_END, _fFar);
	glFogf(GL_FOG_DENSITY, _fValue);
  //## end DC_COpenGL::SetFogDensity%983288696.body
}

void DC_COpenGL::SetFogMode (E3D_EFogMode _eFogMode)
{
  //## begin DC_COpenGL::SetFogMode%983288699.body preserve=yes
	GLint iFogMode;
	switch ( _eFogMode)
	{
	case E3D_EFogMode_LINEAR: iFogMode = GL_LINEAR;
		break;
	case E3D_EFogMode_EXPONENTIAL: iFogMode = GL_EXP;
		break;
	case E3D_EFogMode_EXPONENTIAL_2: iFogMode = GL_EXP2;
		break;
	default:;
	}
	glFogi( GL_FOG_MODE,iFogMode);
	glHint( GL_FOG_HINT, GL_DONT_CARE);
  //## end DC_COpenGL::SetFogMode%983288699.body
}

void DC_COpenGL::SetBackgroundColor (E3D_Color &_roColor)
{
  //## begin DC_COpenGL::SetBackgroundColor%985613127.body preserve=yes
	float fRed,fGreen,fBlue,fAlpha;

	m_oBackgroundColor = _roColor;
	_roColor.GetRGBA(&fRed,&fGreen,&fBlue,&fAlpha);
	glClearColor(fRed,fGreen,fBlue,fAlpha);
  //## end DC_COpenGL::SetBackgroundColor%985613127.body
}

void DC_COpenGL::SetLineWidth (float _fWidth)
{
  //## begin DC_COpenGL::SetLineWidth%985613128.body preserve=yes
	m_fLineWidth = _fWidth;
	glLineWidth((float)m_fLineWidth);
  //## end DC_COpenGL::SetLineWidth%985613128.body
}

void DC_COpenGL::SetCullFaceMode (E3D_ECullFaceMode _eCullFaceMode)
{
  //## begin DC_COpenGL::SetCullFaceMode%990608643.body preserve=yes

	m_eCullingMode = _eCullFaceMode;
	switch ( _eCullFaceMode)
	{
	case E3D_ECullFaceMode_FRONT:
		glCullFace(GL_FRONT);
		break;
	case E3D_ECullFaceMode_BACK:
		glCullFace(GL_BACK);
		break;
	case E3D_ECullFaceMode_FRONT_AND_BACK:
		glCullFace(GL_FRONT_AND_BACK);
		break;
	default:;
	}
  //## end DC_COpenGL::SetCullFaceMode%990608643.body
}

E3D_ECullFaceMode DC_COpenGL::eGetCullFaceMode ()
{
  //## begin DC_COpenGL::eGetCullFaceMode%990623005.body preserve=yes
	return this->m_eCullingMode;
  //## end DC_COpenGL::eGetCullFaceMode%990623005.body
}

void DC_COpenGL::SetAlphaFunc (E3D_EAlphaFunc _eAlphaFunc, float _fRefValue)
{
  //## begin DC_COpenGL::SetAlphaFunc%994343011.body preserve=yes
	
	m_eAlphaFunc = _eAlphaFunc;
	GLenum gAlphaFunc;
	switch(_eAlphaFunc)
	{
	case E3D_EAlphaFunc_NEVER:
		gAlphaFunc = GL_NEVER;
		break;
	case E3D_EAlphaFunc_LESS:
		gAlphaFunc = GL_LESS;
		break;
	case E3D_EAlphaFunc_EQUAL:
		gAlphaFunc = GL_EQUAL;
		break;
	case E3D_EAlphaFunc_LEQUAL:
		gAlphaFunc = GL_LEQUAL;
		break;
	case E3D_EAlphaFunc_GREATER:
		gAlphaFunc = GL_GREATER;
		break;
	case E3D_EAlphaFunc_NOTEQUAL:
		gAlphaFunc = GL_NOTEQUAL;
		break;
	case E3D_EAlphaFunc_GEQUAL:
		gAlphaFunc = GL_GEQUAL;
		break;
	case E3D_EAlphaFunc_ALWAYS:
		gAlphaFunc = GL_ALWAYS;
		break;
	default:;
	}

	glAlphaFunc(gAlphaFunc,_fRefValue);
  //## end DC_COpenGL::SetAlphaFunc%994343011.body
}

E3D_EAlphaFunc DC_COpenGL::eGetAlphaFunc ()
{
  //## begin DC_COpenGL::eGetAlphaFunc%994343012.body preserve=yes
	return m_eAlphaFunc;
  //## end DC_COpenGL::eGetAlphaFunc%994343012.body
}

void DC_COpenGL::SetTWrapMode (E3D_EWrapMode _eWrapMode)
{
  //## begin DC_COpenGL::SetTWrapMode%994678138.body preserve=yes
	switch (_eWrapMode)
	{
	case E3D_EWrapMode_CLAMP:
		m_iTexWrapT = GL_CLAMP;
		break;
	case E3D_EWrapMode_CLAMP_TO_EDGE:
		m_iTexWrapT = GL_CLAMP;
		break;
	case E3D_EWrapMode_REPEAT:
		m_iTexWrapT = GL_REPEAT;
		break;
	default:
		assert(0);
		break;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_iTexWrapT);
  //## end DC_COpenGL::SetTWrapMode%994678138.body
}

void DC_COpenGL::SetSWrapMode (E3D_EWrapMode _eWrapMode)
{
  //## begin DC_COpenGL::SetSWrapMode%994678139.body preserve=yes
	switch (_eWrapMode)
	{
	case E3D_EWrapMode_CLAMP:
		m_iTexWrapS = GL_CLAMP;
		break;
	case E3D_EWrapMode_CLAMP_TO_EDGE:
		m_iTexWrapS = GL_CLAMP;
		break;
	case E3D_EWrapMode_REPEAT:
		m_iTexWrapS = GL_REPEAT;
		break;
	default:
		assert(0);
		break;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_iTexWrapS);
  //## end DC_COpenGL::SetSWrapMode%994678139.body
}

void DC_COpenGL::SetMagFilter (E3D_EMagFilter _eMagFilter)
{
  //## begin DC_COpenGL::SetMagFilter%994678140.body preserve=yes
	switch (_eMagFilter)
	{
	case E3D_EMagFilter_NEAREST:
		m_iMagFilter = GL_NEAREST;
		break;
	case E3D_EMagFilter_LINEAR:
		m_iMagFilter = GL_LINEAR;
		break;
	default:
		assert(0);
		break;
	}
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_iMagFilter);
  //## end DC_COpenGL::SetMagFilter%994678140.body
}

void DC_COpenGL::SetMinFilter (E3D_EMinFilter _eMinFilter)
{
  //## begin DC_COpenGL::SetMinFilter%994678141.body preserve=yes
	switch (_eMinFilter)
	{
	case E3D_EMinFilter_NEAREST:
		m_iMinFilter = GL_NEAREST;
		break;
	case E3D_EMinFilter_LINEAR:
		m_iMinFilter = GL_LINEAR;
		break;
	case E3D_EMinFilter_NEAREST_MIPMAP_LINEAR:
		m_iMinFilter = GL_NEAREST_MIPMAP_LINEAR;
		break;
	case E3D_EMinFilter_NEAREST_MIPMAP_NEAREST:
		m_iMinFilter = GL_NEAREST_MIPMAP_NEAREST;
		break;
		case E3D_EMinFilter_LINEAR_MIPMAP_LINEAR:
		m_iMinFilter = GL_LINEAR_MIPMAP_LINEAR;
		break;
	case E3D_EMinFilter_LINEAR_MIPMAP_NEAREST:
		m_iMinFilter = GL_LINEAR_MIPMAP_NEAREST;
		break;
	default:
		assert(0);
		break;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_iMinFilter);
  //## end DC_COpenGL::SetMinFilter%994678141.body
}

void DC_COpenGL::SetScissor (float _x0, float _y0, float _x1, float _y1)
{
  //## begin DC_COpenGL::SetScissor%994678144.body preserve=yes
	glScissor((int)_x0,(int)_y0,(int)_x1,(int)_y1);
  //## end DC_COpenGL::SetScissor%994678144.body
}

void DC_COpenGL::SetShadingModel (E3D_EShadingMode _eShadingMode)
{
  //## begin DC_COpenGL::SetShadingModel%994678146.body preserve=yes

	GLenum mode;
	switch (_eShadingMode)
	{
	case E3D_EShadingMode_FLAT:
		mode = GL_FLAT;
		break;
	case E3D_EShadingMode_SMOOTH:
		mode = GL_SMOOTH;
		break;
	default:;
	}
	glShadeModel(mode);
  //## end DC_COpenGL::SetShadingModel%994678146.body
}

void DC_COpenGL::Vertex2f (float _x, float _y)
{
  //## begin DC_COpenGL::Vertex2f%994678148.body preserve=yes
	glVertex3f(_x,_y,1.0f);
  //## end DC_COpenGL::Vertex2f%994678148.body
}

void DC_COpenGL::SetTexEnvf (E3D_ETextureEnvMode _eTextureEnvMode)
{
  //## begin DC_COpenGL::SetTexEnvf%994678150.body preserve=yes

	GLfloat param;

	switch(_eTextureEnvMode)
	{
		case E3D_ETextureEnvMode_MODULATE:
			param = GL_MODULATE;
			break;
		case E3D_ETextureEnvMode_DECAL:
			param = GL_DECAL;
			break;
		case E3D_ETextureEnvMode_REPLACE:
			param = GL_REPLACE;
			break;
		case E3D_ETextureEnvMode_BLEND:
			param = GL_BLEND;
			break;
		default:;
	}
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, param);
  //## end DC_COpenGL::SetTexEnvf%994678150.body
}

void DC_COpenGL::SetOrtho (float _fLeft, float _fRight, float _fBottom, float _fTop, float _fZNear, float _fZFar)
{
  //## begin DC_COpenGL::SetOrtho%994752933.body preserve=yes
	glOrtho(
		_fLeft,_fRight,
		_fBottom,_fTop,
		_fZNear,_fZFar);
  //## end DC_COpenGL::SetOrtho%994752933.body
}

void DC_COpenGL::SetViewport (int _iX, int _iY, int _iWidth, int _iHeight)
{
  //## begin DC_COpenGL::SetViewport%994752934.body preserve=yes
    m_viewport.Set(_iX,_iY,_iWidth,_iHeight);
	
	glEnable(GL_SCISSOR_TEST);
	
	glViewport(_iX,_iY,_iWidth,_iHeight);
	glScissor (_iX,_iY,_iWidth,_iHeight);

  //## end DC_COpenGL::SetViewport%994752934.body
}

void DC_COpenGL::SetPixelStorei (E3D_EPixelStoreMode _ePixelStoreMode)
{
  //## begin DC_COpenGL::SetPixelStorei%994752937.body preserve=yes
	GLenum pname;
	switch(_ePixelStoreMode)
	{
	case E3D_EPixelStoreMode_UNPACK_ALIGNMENT:
		pname = GL_UNPACK_ALIGNMENT;
		break;
	case E3D_EPixelStoreMode_UNPACK_SWAP_BYTES:
		pname = GL_UNPACK_SWAP_BYTES;
		break;
	default:;
	}
		glPixelStorei(pname,1);
  //## end DC_COpenGL::SetPixelStorei%994752937.body
}

void DC_COpenGL::BeginPrimitive (unsigned char _ucPrimitive)
{
  //## begin DC_COpenGL::BeginPrimitive%994758211.body preserve=yes
	glBegin(_ucPrimitive);
  //## end DC_COpenGL::BeginPrimitive%994758211.body
}

void DC_COpenGL::EndPrimitive (unsigned char _ucPrimitive)
{
  //## begin DC_COpenGL::EndPrimitive%994758212.body preserve=yes
	glEnd();
  //## end DC_COpenGL::EndPrimitive%994758212.body
}

void DC_COpenGL::DefineCurrentTexture (int _iWidth, int _iHeight, E3D_EPixelFormat _ePixel, unsigned char *_pucImage)
{
  //## begin DC_COpenGL::DefineCurrentTexture%995294306.body preserve=yes
/*	switch(_ePixel)
	{
	case E3D_EPixelFormat_RGB24:
			glTexImage2D( GL_TEXTURE_2D,0,GL_RGB,
			_iWidth,_iHeight,0, 
			GL_RGB, GL_UNSIGNED_BYTE,_pucImage);	
			break;
	case E3D_EPixelFormat_RGBA32:
			glTexImage2D( GL_TEXTURE_2D,0,GL_RGBA,
			_iWidth,_iHeight,0, 
			GL_RGBA, GL_UNSIGNED_BYTE,_pucImage);	
			break;
	default:;

	}
*/
  //## end DC_COpenGL::DefineCurrentTexture%995294306.body
}

void DC_COpenGL::SetCurrentPalette (E3D_TextureDesc* _poTexDesc)
{
  //## begin DC_COpenGL::SetCurrentPalette%996573935.body preserve=yes
    #ifndef DEBUG
	    if (!_poTexDesc) return;
    #else
        assert(! _poTexDesc);
    #endif

    OGL_ClutDesc *poOgl = (OGL_ClutDesc *)_poTexDesc;	
    assert (!poOgl->bIsFree());			
	poOgl->BindTexture();
  //## end DC_COpenGL::SetCurrentPalette%996573935.body
}

void DC_COpenGL::BeginSprites ()
{
  //## begin DC_COpenGL::BeginSprites%996675434.body preserve=yes
	glBegin(GL_LINES);
  //## end DC_COpenGL::BeginSprites%996675434.body
}

void DC_COpenGL::EndSprites ()
{
  //## begin DC_COpenGL::EndSprites%996675435.body preserve=yes
	glEnd();
  //## end DC_COpenGL::EndSprites%996675435.body
}

void DC_COpenGL::SetColorArrayAlpha (float *_pfTable, int _iNbColor, int _iStride)
{
  //## begin DC_COpenGL::SetColorArrayAlpha%1000366634.body preserve=yes
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_FLOAT, _iStride, _pfTable);
    //glDisableClientState(GL_COLOR_ARRAY);
  //## end DC_COpenGL::SetColorArrayAlpha%1000366634.body
}

void DC_COpenGL::BeginQuad ()
{
  //## begin DC_COpenGL::BeginQuad%1003849248.body preserve=yes
	glBegin(GL_QUADS);
  //## end DC_COpenGL::BeginQuad%1003849248.body
}

void DC_COpenGL::EndQuad ()
{
  //## begin DC_COpenGL::EndQuad%1003849249.body preserve=yes
	glEnd();
  //## end DC_COpenGL::EndQuad%1003849249.body
}

void DC_COpenGL::EnableClientState (E3D_EClientState _eState)
{
  //## begin DC_COpenGL::EnableClientState%1003849250.body preserve=yes
	switch(_eState)
	{
		case E3D_EClientState_VERTEXARRAY:
			glEnableClientState(GL_VERTEX_ARRAY);
			break;
		case E3D_EClientState_COLORARRAY:
			glEnableClientState(GL_COLOR_ARRAY);
			break;
		case E3D_EClientState_NORMALARRAY:
			glEnableClientState(GL_NORMAL_ARRAY);
			break;
		case E3D_EClientState_TEXCOORDARRAY:
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			break;
	}
  //## end DC_COpenGL::EnableClientState%1003849250.body
}

void DC_COpenGL::DisableClientState (E3D_EClientState _eState)
{
  //## begin DC_COpenGL::DisableClientState%1003849251.body preserve=yes
	switch(_eState)
	{
		case E3D_EClientState_VERTEXARRAY:
			glDisableClientState(GL_VERTEX_ARRAY);
			break;
		case E3D_EClientState_COLORARRAY:
			glDisableClientState(GL_COLOR_ARRAY);
			break;
		case E3D_EClientState_NORMALARRAY:
			glDisableClientState(GL_NORMAL_ARRAY);
			break;
		case E3D_EClientState_TEXCOORDARRAY:
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			break;
	}
  //## end DC_COpenGL::DisableClientState%1003849251.body
}

void DC_COpenGL::SetVertexArray (int _iSize, float *_pfTable, int _iNbVertex, int _iStride)
{
  //## begin DC_COpenGL::SetVertexArray%1003849252.body preserve=yes
	glVertexPointer(_iSize, GL_FLOAT, _iStride, _pfTable);
  //## end DC_COpenGL::SetVertexArray%1003849252.body
}

void DC_COpenGL::SetColorArray (int _iSize, float *_pfTable, int _iNbColor, int _iStride)
{
  //## begin DC_COpenGL::SetColorArray%1003849253.body preserve=yes
	glColorPointer(_iSize, GL_FLOAT, _iStride, _pfTable);
  //## end DC_COpenGL::SetColorArray%1003849253.body
}


void DC_COpenGL::SetTexCoordArray (int _iSize, float *_pfTable, int _iNbTexCoord, int _iStride)
{
  //## begin DC_COpenGL::SetTexCoordArray%1003849255.body preserve=yes
	glTexCoordPointer(_iSize, GL_FLOAT, _iStride, _pfTable);
  //## end DC_COpenGL::SetTexCoordArray%1003849255.body
}

void DC_COpenGL::SetNormalArray (int _iSize, float *_pfTable, int _iNbNormal, int _iStride)
{
  //## begin DC_COpenGL::SetNormalArray%1003999242.body preserve=yes
	glNormalPointer(GL_FLOAT, _iStride, _pfTable);
  //## end DC_COpenGL::SetNormalArray%1003999242.body
}

void DC_COpenGL::DrawArrays (E3D_EFaceType _eFaceType, int _iFirst, int _iCount)
{
  //## begin DC_COpenGL::DrawArrays%1003849256.body preserve=yes
    switch (_eFaceType)
    {
    case E3D_EFaceType_TRISTRIP: // TRIANGLE STRIPS
        glDrawArrays(GL_TRIANGLE_STRIP,_iFirst,_iCount);
        break;
    case E3D_EFaceType_TRIFAN: // TRIANGLE FANS
        glDrawArrays(GL_TRIANGLE_FAN,_iFirst,_iCount);
        break;
    case E3D_EFaceType_TRIANGLE: // TRIANGLES
        glDrawArrays(GL_TRIANGLES,_iFirst,_iCount);
        break;
    case E3D_EFaceType_QUAD: // QUADS
        glDrawArrays(GL_QUADS,_iFirst,_iCount);
        break;
    };
  //## end DC_COpenGL::DrawArrays%1003849256.body
}

void DC_COpenGL::SetBlendEquation (E3D_EBlendEquation _eBlendEquation)
{
  //## begin DC_COpenGL::SetBlendEquation%1006444255.body preserve=yes
	
	switch(_eBlendEquation)
	{
		case E3D_EBlendEquation_ADD:
			glBlendEquation ( GL_FUNC_ADD);
			break;
		case E3D_EBlendEquation_SUBSTRACT:
			glBlendEquation ( GL_FUNC_SUBTRACT);
			break;
		case E3D_EBlendEquation_REVERSE_SUBSTRACT:
			glBlendEquation (GL_FUNC_REVERSE_SUBTRACT);
			break;
		case E3D_EBlendEquation_MIN:
			glBlendEquation ( GL_MIN);
			break;
		case E3D_EBlendEquation_MAX:
			glBlendEquation ( GL_MAX);
			break;
		default:;
	}

  //## end DC_COpenGL::SetBlendEquation%1006444255.body
}

void DC_COpenGL::DestroyAllTextures ()
{
  //## begin DC_COpenGL::DestroyAllTextures%1007468261.body preserve=yes
	OGL_TextureDesc *poTexDesc;
	for (int i=0; i< DC_COpenGL_MAXTEXTURES; i++)
	{
			poTexDesc = &m_poTexDesc[i];
			poTexDesc->DestroyTexture();
	}
	//## end DC_COpenGL::DestroyAllTextures%1007468261.body
}

// Additional Declarations
  //## begin DC_COpenGL%38EB1E6B0157.declarations preserve=yes

void DC_COpenGL::SetColorArray (int _iSize, int *_piTable, int _iNbColor, int _iStride)
{
	glColorPointer(_iSize, GL_UNSIGNED_BYTE, _iStride, _piTable);
}


void DC_COpenGL::DestroyClut (E3D_TextureDesc* _poClutDesc)
{
 
	OGL_ClutDesc *poClutDesc;
	for (int i=0; i< DC_COpenGL_MAXTEXTURES; i++)
	{
			poClutDesc = &m_poClutDesc[i];
			if (poClutDesc == _poClutDesc)
			{
				poClutDesc->DestroyTexture();
				return;
			}
	}
 
}

void DC_COpenGL::DestroyAllCluts ()
{
	OGL_ClutDesc *poClutDesc;
	for (int i=0; i< DC_COpenGL_MAXCLUTS; i++)
	{
		poClutDesc = &m_poClutDesc[i];
		poClutDesc->DestroyTexture();
	}

}


void DC_COpenGL::WriteZBuffer (int _bool)
{
	glDepthMask(_bool);
}

void DC_COpenGL::ColorMask(int _R,int _G,int _B,int _A)
{
	glColorMask(_R,_G,_B,_A);
}


void DC_COpenGL::EnableZBias (void)
{
	glEnable (GL_POLYGON_OFFSET_FILL);
}

void DC_COpenGL::DisableZBias (void)
{
	glDisable (GL_POLYGON_OFFSET_FILL);
}

void DC_COpenGL::SetZBiasLevel (float level)
{
	glPolygonOffset (level,level);
}

void DC_COpenGL::DepthFunc	(E3D_EDepthFunc _eDepthFunc)
{
	GLenum gDepthFunc;

	switch(_eDepthFunc)
	{
	case E3D_EDepthFunc_NEVER:
		gDepthFunc = GL_NEVER;
		break;
	case E3D_EDepthFunc_LESS:
		gDepthFunc = GL_LESS;
		break;
	case E3D_EDepthFunc_EQUAL:
		gDepthFunc = GL_EQUAL;
		break;
	case E3D_EDepthFunc_LEQUAL:
		gDepthFunc = GL_LEQUAL;
		break;
	case E3D_EDepthFunc_GREATER:
		gDepthFunc = GL_GREATER;
		break;
	case E3D_EDepthFunc_NOTEQUAL:
		gDepthFunc = GL_NOTEQUAL;
		break;
	case E3D_EDepthFunc_GEQUAL:
		gDepthFunc = GL_GEQUAL;
		break;
	case E3D_EDepthFunc_ALWAYS:
		gDepthFunc = GL_ALWAYS;
		break;
	default:;
	}

	glDepthFunc(gDepthFunc);
}

void DC_COpenGL::DestroyTexturesOfGroup(int _iGroup)
{
	int i;

	OGL_TextureDesc *poTexDesc;
	for (i=0; i< DC_COpenGL_MAXTEXTURES; i++)
	{
		poTexDesc = &m_poTexDesc[i];
		if(poTexDesc->m_iGroupId==_iGroup)
			poTexDesc->DestroyTexture();
	}

	OGL_ClutDesc *poClutDesc;
	for (i=0; i< DC_COpenGL_MAXCLUTS; i++)
	{
		poClutDesc = &m_poClutDesc[i];
		if (poClutDesc->m_iGroupId == _iGroup)
		{					
			poClutDesc->DestroyTexture();
		}
	}
}

  //## end DC_COpenGL%38EB1E6B0157.declarations

// <DMC: Blendmode>
void DC_COpenGL::BlendMode(E3D_BlendMode _eBlendMode)
{
	m_eBlendMode = _eBlendMode;

	switch (m_eBlendMode)
	{
		case E3D_BM_NONE:
		{
			glBlendFunc(GL_ZERO,GL_ONE);
			glBlendEquation(GL_FUNC_ADD);			
		}
		break;
		
		case E3D_BM_COPY:				//	Pix = Src
		{
			glBlendFunc(GL_ONE,GL_ZERO);
			glBlendEquation(GL_FUNC_ADD);			
		}
		break;

		case E3D_BM_COPY_DATM:			//	Pix = Src * DstAlpha
		{
			glBlendFunc(GL_DST_ALPHA, GL_ZERO);
			glBlendEquation(GL_FUNC_ADD);			
		}
		break;

		case E3D_BM_MULT:				//	Pix = Src*Dst
		{
			glBlendFunc(GL_DST_COLOR,GL_ZERO);
			glBlendEquation(GL_FUNC_ADD);			
		}
		break;

		case E3D_BM_MULT2X:				//	Pix = 2*Src*Dst
		{
			glBlendFunc(GL_DST_COLOR,GL_SRC_COLOR);
			glBlendEquation(GL_FUNC_ADD);			
		}
		break;

		case E3D_BM_SUBSTRACTIVE:		//	Pix = Dst - Src
		{
			glBlendFunc(GL_ONE,GL_ONE);
			glBlendEquation (GL_FUNC_REVERSE_SUBTRACT);
		}
		break;

		case E3D_BM_SUBSTRACTIVE_DATM:	//	Pix = Dst - Src * DstAlpha
		{
			glBlendFunc(GL_DST_ALPHA, GL_ONE);
			glBlendEquation (GL_FUNC_REVERSE_SUBTRACT);
		}
		break;

		case E3D_BM_ADD:				//	Pix = Src + Dst				
		{
			glBlendFunc(GL_ONE,GL_ONE);
			glBlendEquation(GL_FUNC_ADD);
		}
		break;

		case E3D_BM_ADD_DATM:				//	Pix = Src + Dst * DstAlpha
		{
			glBlendFunc(GL_DST_ALPHA, GL_ONE);
			glBlendEquation(GL_FUNC_ADD);
		}
		break;

		case E3D_BM_ALPHA:				//	Pix = Src*SrcAlpha + Dst*(1-SrcAlpha)
		{
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			glBlendEquation(GL_FUNC_ADD);			
		}
		break;

		case E3D_BM_ALPHAADD:			//	Pix = Src*SrcAlpha + Dst
		{
			glBlendFunc(GL_SRC_ALPHA,GL_ONE);
			glBlendEquation(GL_FUNC_ADD);			
		}
		break;

		case E3D_BM_INVALPHA:			//	Pix = Src*(1-SrcAlpha) + Dst*SrcAlpha			
		{
			glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
			glBlendEquation(GL_FUNC_ADD);
		}
		break;
		
		case E3D_BM_DALPHA:				//	Pix = Src*DstAlpha + Dst*(1-DstAlpha)
		{
			glBlendFunc(GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA);
			glBlendEquation(GL_FUNC_ADD);			
		}
		break;

		case E3D_BM_DALPHAADD:			//  Pix = Src*(1-DstAlpha) + Dst
		{
			glBlendFunc(GL_DST_ALPHA,GL_ONE);
			glBlendEquation(GL_ADD);			
		}
		break;

		case E3D_BM_DINVALPHA:			//	Pix = Src*DstAlpha + Dst*(1-DstAlpha)
		{
			glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);
			glBlendEquation(GL_FUNC_ADD);			
		}
		break;

		case E3D_BM_ALPHASUBSTRACTIVE:	//  Pix = Dst - Src * SrcAlpha
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glBlendEquation (GL_FUNC_REVERSE_SUBTRACT);			
		}
		break;

		case E3D_BM_CALPHA:				//  Pix = Src*Alpha + dst(1-Alpha)
		/*
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_ZERO,GL_ONE);
		*/
		break;
	}
}

E3D_BlendMode DC_COpenGL::eGetBlendMode()
{
	return( m_eBlendMode );
}

void DC_COpenGL::ConstantAlpha(float _fCAlpha)
{
	m_fCAlpha = _fCAlpha;
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
// LIGHTING
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
void DC_COpenGL::EnableLighting(int _iForceUpdate)
{
	glEnable(GL_LIGHTING);
	bLightingEnabled = true;
}
//-----------------------------------------------------------------------------------------
void DC_COpenGL::DisableLighting()
{
	glDisable(GL_LIGHTING);
	bLightingEnabled = false;
/*	
	for (int i=0;i<MAX_LIGHTS;i++)
		ActivateLight(i,false);
*/	
}
//-----------------------------------------------------------------------------------------
void DC_COpenGL::ActivateLight(int _iLightIdx,int _iOnOff)
{
	oLights[_iLightIdx].bEnabled = (bool)_iOnOff;

	if (oLights[_iLightIdx].bEnabled)
		glEnable (GL_LIGHT0 + _iLightIdx);
	else
		glDisable(GL_LIGHT0 + _iLightIdx);
}
//-----------------------------------------------------------------------------------------
void DC_COpenGL::SetLightPosition(int _iLightIdx, MTH_CPoint3& _roPosition)
{
	oLights[_iLightIdx].oPos = _roPosition;
	oLights[_iLightIdx].oPos.w = 1.0f;	// positional light

	glLightfv (GL_LIGHT0 + _iLightIdx, GL_POSITION, ((float*)&oLights[_iLightIdx].oPos));
}
//-----------------------------------------------------------------------------------------
void DC_COpenGL::SetLightDirection(int _iLightIdx, MTH_CPoint3& _roDirection)
{
	oLights[_iLightIdx].oDir   = _roDirection;
	oLights[_iLightIdx].oDir.w = 0.0f;	// directional light

	glLightfv (GL_LIGHT0 + _iLightIdx, GL_POSITION, ((float*)&oLights[_iLightIdx].oDir));	// ???
}
//-----------------------------------------------------------------------------------------j
void DC_COpenGL::SetLightType(int _iLightIdx, int _iLightType)
{
	oLights[_iLightIdx].iType = _iLightType;
}
//-----------------------------------------------------------------------------------------
void DC_COpenGL::SetLightColor(int _iLightIdx, E3D_Color &_roColor)
{
	float fBlackColor[4] = {0.0f,0.0f,0.0f,1.0f};
	float fWhiteColor[4] = {1.0f,1.0f,1.0f,1.0f};

	switch (oLights[_iLightIdx].iType)
	{
		case 0:	// Ambient
		// *((E3D_Color*)&oLights[_iLightIdx].fAmb) = _roColor;
		glLightfv (GL_LIGHT0+_iLightIdx, GL_AMBIENT, (float*)&_roColor);
		glLightfv (GL_LIGHT0+_iLightIdx, GL_DIFFUSE, fBlackColor);		
		glLightfv (GL_LIGHT0+_iLightIdx, GL_SPECULAR, (float*)&_roColor);
		break; 

		case 1:	// Point light
		// *((E3D_Color*)&oLights[_iLightIdx].fDiff) = _roColor;		
		glLightfv (GL_LIGHT0+_iLightIdx, GL_AMBIENT, fBlackColor);
		glLightfv (GL_LIGHT0+_iLightIdx, GL_DIFFUSE, (float*)&_roColor);
		glLightfv (GL_LIGHT0+_iLightIdx, GL_SPECULAR, (float*)&_roColor);
		break;

		case 2:	// Directional
		// *((E3D_Color*)&oLights[_iLightIdx].fDiff) = _roColor;		
		glLightfv (GL_LIGHT0+_iLightIdx, GL_AMBIENT, fBlackColor);
		glLightfv (GL_LIGHT0+_iLightIdx, GL_DIFFUSE, (float*)&_roColor);
		glLightfv (GL_LIGHT0+_iLightIdx, GL_SPECULAR, (float*)&_roColor);
		break;
	}
}
//-----------------------------------------------------------------------------------------
void DC_COpenGL::SetLightRange(int _iLightIdx, float _fRange)
{
	float fAtt = 1.0f / _fRange;

	glLightf (GL_LIGHT0+_iLightIdx, GL_CONSTANT_ATTENUATION,	0.0f);
	glLightf (GL_LIGHT0+_iLightIdx, GL_LINEAR_ATTENUATION,		fAtt);
	glLightf (GL_LIGHT0+_iLightIdx, GL_QUADRATIC_ATTENUATION,	fAtt*fAtt);
}
//-----------------------------------------------------------------------------------------
// Specific
//-----------------------------------------------------------------------------------------
void DC_COpenGL::SetLightAmbientColor(int _iLightIdx, E3D_Color &_roColor)
{
	/*
	*((E3D_Color*)&oLights[_iLightIdx].fAmb) = _roColor;
	glLightfv (GL_LIGHT0+_iLightIdx, GL_AMBIENT, oLights[_iLightIdx].fAmb);
	*/
	glLightfv (GL_LIGHT0+_iLightIdx, GL_AMBIENT, (float*)&_roColor);
}
//-----------------------------------------------------------------------------------------
void DC_COpenGL::SetLightAmbientColor(int _iLightIdx, float *_pfColor)
{
	/*
	oLights[_iLightIdx].fAmb[0] = _pfColor[0];
	oLights[_iLightIdx].fAmb[1] = _pfColor[1];
	oLights[_iLightIdx].fAmb[2] = _pfColor[2];
	oLights[_iLightIdx].fAmb[3] = _pfColor[3];
	
	glLightfv (GL_LIGHT0+_iLightIdx, GL_AMBIENT, oLights[_iLightIdx].fAmb);
	*/
	glLightfv (GL_LIGHT0+_iLightIdx, GL_AMBIENT, _pfColor);
}
//-----------------------------------------------------------------------------------------
void DC_COpenGL::SetLightDiffuseColor(int _iLightIdx, E3D_Color &_roColor)
{
	/*
	*((E3D_Color*)&oLights[_iLightIdx].fAmb) = _roColor;
	glLightfv (GL_LIGHT0+_iLightIdx, GL_DIFFUSE, oLights[_iLightIdx].fDiff);
	*/
	glLightfv (GL_LIGHT0+_iLightIdx, GL_DIFFUSE, (float*)&_roColor);
}
//-----------------------------------------------------------------------------------------
void DC_COpenGL::SetLightDiffuseColor(int _iLightIdx, float *_pfColor)
{
	/*
	oLights[_iLightIdx].fDiff[0] = _pfColor[0];
	oLights[_iLightIdx].fDiff[1] = _pfColor[1];
	oLights[_iLightIdx].fDiff[2] = _pfColor[2];
	oLights[_iLightIdx].fDiff[3] = _pfColor[3];	
	glLightfv (GL_LIGHT0+_iLightIdx, GL_DIFFUSE, oLights[_iLightIdx].fDiff);
	*/
	glLightfv (GL_LIGHT0+_iLightIdx, GL_DIFFUSE, _pfColor);
}
//-----------------------------------------------------------------------------------------
void DC_COpenGL::SetLightSpecularColor(int _iLightIdx, E3D_Color &_roColor)
{
	/*
	*((E3D_Color*)&oLights[_iLightIdx].fAmb) = _roColor;
	glLightfv (GL_LIGHT0+_iLightIdx, GL_SPECULAR, oLights[_iLightIdx].fSpec);
	*/
	glLightfv (GL_LIGHT0+_iLightIdx, GL_SPECULAR, (float*)&_roColor);
}
//-----------------------------------------------------------------------------------------
void DC_COpenGL::SetLightSpecularColor(int _iLightIdx, float *_pfColor)
{
	/*
	oLights[_iLightIdx].fSpec[0] = _pfColor[0];
	oLights[_iLightIdx].fSpec[1] = _pfColor[1];
	oLights[_iLightIdx].fSpec[2] = _pfColor[2];
	oLights[_iLightIdx].fSpec[3] = _pfColor[3];
	glLightfv (GL_LIGHT0+_iLightIdx, GL_SPECULAR, oLights[_iLightIdx].fSpec);
	*/
	glLightfv (GL_LIGHT0+_iLightIdx, GL_SPECULAR, _pfColor);
}
//-----------------------------------------------------------------------------------------
void DC_COpenGL::SetLightSelectionFunc(TLightSelectionFunc _pLSFunc)
{
	pLSFunc = _pLSFunc;
}

void DC_COpenGL::SelectLights(MTH_CPoint3& _roPos)
{
	if (!bLightingEnabled) return;
	
	if (pLSFunc)
		pLSFunc(_roPos);
}
//-----------------------------------------------------------------------------------------
// Display List stuff
//-----------------------------------------------------------------------------------------
int DC_COpenGL::iCreateDisplayList()
{
	return ( glGenLists(1) );
}
//-----------------------------------------------------------------------------------------
void DC_COpenGL::BeginDisplayList(int _iListID)
{
	glNewList(_iListID,GL_COMPILE);
}
//-----------------------------------------------------------------------------------------
void DC_COpenGL::EndDisplayList()
{
	glEndList();
}
//-----------------------------------------------------------------------------------------
void DC_COpenGL::CallDisplayList(int _iListID)
{
	glCallList(_iListID);
}
//-----------------------------------------------------------------------------------------
//## end module%38EB1E6B0157.epilog