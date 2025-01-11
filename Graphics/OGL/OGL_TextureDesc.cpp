	//## begin module%3920FAAA0167.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3920FAAA0167.cm

//## begin module%3920FAAA0167.cp preserve=no
//## end module%3920FAAA0167.cp

//## Module: OGL_TextureDesc%3920FAAA0167; Pseudo Package body
//## Source file: e:\ProRally\Realization\ENGINE3D\DC\OGL_TextureDesc.cpp

//## begin module%3920FAAA0167.additionalIncludes preserve=no
//## end module%3920FAAA0167.additionalIncludes

//## begin module%3920FAAA0167.includes preserve=yes
#include <stdlib.h>
#include <assert.h>
//## end module%3920FAAA0167.includes

// OGL_TextureDesc
#include "OGL_TextureDesc.h"
//## begin module%3920FAAA0167.additionalDeclarations preserve=yes
//## end module%3920FAAA0167.additionalDeclarations

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glext.h>	// REQUIRES OPENGL1.1 / 1.2
#include <math.h>
#include <system.h>
// Class OGL_TextureDesc 
















OGL_TextureDesc::OGL_TextureDesc()
  //## begin OGL_TextureDesc::OGL_TextureDesc%.hasinit preserve=no
      : m_pucImage(NULL), m_iTexWrapS(GL_CLAMP_TO_EDGE), m_iTexWrapT(GL_CLAMP_TO_EDGE), m_iMagFilter(GL_LINEAR), m_iMinFilter(GL_LINEAR)
  //## end OGL_TextureDesc::OGL_TextureDesc%.hasinit
  //## begin OGL_TextureDesc::OGL_TextureDesc%.initialization preserve=yes
  //## end OGL_TextureDesc::OGL_TextureDesc%.initialization
{
  //## begin OGL_TextureDesc::OGL_TextureDesc%.body preserve=yes
	m_bIsValid =0;
	m_pucImage = NULL;
	m_iWidth =0;
	m_iHeight =0;
	m_iTextureName =0;
	m_bIsFree =1;
	m_iGroupId = 0;
  //## end OGL_TextureDesc::OGL_TextureDesc%.body
}


OGL_TextureDesc::~OGL_TextureDesc()
{
  //## begin OGL_TextureDesc::~OGL_TextureDesc%.body preserve=yes
	DestroyTexture();
  //## end OGL_TextureDesc::~OGL_TextureDesc%.body
}



//## Other Operations (implementation)
void OGL_TextureDesc::Invalidate ()
{
  //## begin OGL_TextureDesc::Invalidate%958472402.body preserve=yes
	m_bIsValid = 0;
  //## end OGL_TextureDesc::Invalidate%958472402.body
}

int OGL_TextureDesc::bIsValid ()
{
  //## begin OGL_TextureDesc::bIsValid%958472403.body preserve=yes
	return m_bIsValid;
  //## end OGL_TextureDesc::bIsValid%958472403.body
}

int OGL_TextureDesc::iGetWidth ()
{
  //## begin OGL_TextureDesc::iGetWidth%958472404.body preserve=yes
	return m_iWidth;
  //## end OGL_TextureDesc::iGetWidth%958472404.body
}

int OGL_TextureDesc::iGetHeight ()
{
  //## begin OGL_TextureDesc::iGetHeight%958472405.body preserve=yes
	return m_iHeight;
  //## end OGL_TextureDesc::iGetHeight%958472405.body
}

E3D_EPixelFormat OGL_TextureDesc::eGetPixelFormat ()
{
  //## begin OGL_TextureDesc::eGetPixelFormat%958472406.body preserve=yes
	return m_ePixelFmt;
  //## end OGL_TextureDesc::eGetPixelFormat%958472406.body
}

unsigned char* OGL_TextureDesc::pucGetImage ()
{
  //## begin OGL_TextureDesc::pucGetImage%958472407.body preserve=yes
	return m_pucImage;
  //## end OGL_TextureDesc::pucGetImage%958472407.body
}

void OGL_TextureDesc::SetImage (unsigned char *_pucImage)
{
	m_pucImage = _pucImage;
	Invalidate();
}

void OGL_TextureDesc::InvalidateRect (int _iX, int _iY, int _iWidth, int _iHeight)
{
  //## begin OGL_TextureDesc::InvalidateRect%958472409.body preserve=yes
	m_bIsValid =0;
  //## end OGL_TextureDesc::InvalidateRect%958472409.body
}

int OGL_TextureDesc::bTexImage2D (E3D_EPixelFormat _ePixelFmt, int _iWidth, int _iHeight, unsigned char *_pucImage, int _iNumLODs)
{
  //## begin OGL_TextureDesc::bTexImage2D%958472411.body preserve=yes
	if (_ePixelFmt == E3D_EPixelFormat_PAL8)
		glEnable(GL_EXT_paletted_texture);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_iTextureName);	
	m_ePixelFmt = _ePixelFmt;
	m_iWidth	= _iWidth;
	m_iHeight	= _iHeight;
	m_pucImage	= _pucImage;
	m_bIsFree	= 0;
	m_bIsValid	= 0;
	m_iNumLODs	= _iNumLODs;

	BindTexture();	
	return 1;
  //## end OGL_TextureDesc::bTexImage2D%958472411.body
}

int OGL_TextureDesc::bIsFree ()
{
  //## begin OGL_TextureDesc::bIsFree%958472412.body preserve=yes
	return m_bIsFree;
  //## end OGL_TextureDesc::bIsFree%958472412.body
}

void OGL_TextureDesc::DestroyTexture ()
{
  //## begin OGL_TextureDesc::DestroyTexture%958472413.body preserve=yes
	if (!m_bIsFree)
	{
		glDeleteTextures(1,&m_iTextureName);
		m_bIsValid =0;
		m_pucImage = NULL;
		m_iWidth =0;
		m_iHeight =0;
		m_iTextureName =0;		
		m_bIsFree = 1;
	}
  //## end OGL_TextureDesc::DestroyTexture%958472413.body
}

void OGL_TextureDesc::BindTexture ()
{
  //## begin OGL_TextureDesc::BindTexture%958472414.body preserve=yes
	glBindTexture  (GL_TEXTURE_2D, m_iTextureName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_iTexWrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_iTexWrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_iMagFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_iMinFilter);

	if (m_bIsValid)
		return;

	glPixelStorei  (GL_UNPACK_ALIGNMENT, 1);

	GLint	components;
	GLenum	format;
	
	switch (m_ePixelFmt)
	{
		case E3D_EPixelFormat_RGBA32:
			components = GL_RGBA;
			format = GL_RGBA;
		break;

		case E3D_EPixelFormat_RGB24:
			components = GL_RGB8;
			format = GL_RGB;
		break;

		case E3D_EPixelFormat_RGBA16:
			components = GL_RGBA16;
			format = GL_RGBA;
		break;

		case E3D_EPixelFormat_PAL8:
			components = GL_COLOR_INDEX8_EXT;
			format = GL_COLOR_INDEX;
		break;

		case E3D_EPixelFormat_PAL4:
			components = GL_COLOR_INDEX4_EXT;
			format = GL_COLOR_INDEX;
		break;
	}



	/*
	if( iNumLODs() > 1)
		gluBuild2DMipmaps(GL_TEXTURE_2D, components, m_iWidth, m_iHeight, format,GL_UNSIGNED_BYTE, m_pucImage);
	*/

	// WARNING: OpenGL 1.2
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,iNumLODs()-1 );

	// Setup mipmap LODs
	int imageSize				= lGetSize();
	unsigned char *pucImgData	= m_pucImage;


	for (int l=0;l<iNumLODs();l++)
	{
		glTexImage2D(	GL_TEXTURE_2D,
						l,
						components,
						m_iWidth >> l,
						m_iHeight >> l,
						0,
						format,
						GL_UNSIGNED_BYTE,
						pucImgData
						);

		pucImgData	+= imageSize;
		imageSize	>>= 2;
	}

	m_bIsValid =1;
  //## end OGL_TextureDesc::BindTexture%958472414.body
}

void OGL_TextureDesc::SetTWrapMode (E3D_EWrapMode _eWrapMode)
{
  //## begin OGL_TextureDesc::SetTWrapMode%975424981.body preserve=yes
	switch (_eWrapMode)
	{
	case E3D_EWrapMode_CLAMP_TO_EDGE:
		m_iTexWrapT = GL_CLAMP_TO_EDGE;
		break;
	case E3D_EWrapMode_REPEAT:
		m_iTexWrapT = GL_REPEAT;
		break;
	case E3D_EWrapMode_CLAMP:
	default:
		m_iTexWrapT = GL_CLAMP_TO_EDGE;
		break;
	}
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_iTexWrapT);
  //## end OGL_TextureDesc::SetTWrapMode%975424981.body
}

void OGL_TextureDesc::SetSWrapMode (E3D_EWrapMode _eWrapMode)
{
  //## begin OGL_TextureDesc::SetSWrapMode%975424982.body preserve=yes
	switch (_eWrapMode)
	{
	case E3D_EWrapMode_CLAMP_TO_EDGE:
		m_iTexWrapS = GL_CLAMP_TO_EDGE;
		break;
	case E3D_EWrapMode_REPEAT:
		m_iTexWrapS = GL_REPEAT;
		break;
	case E3D_EWrapMode_CLAMP:
	default:
		m_iTexWrapS = GL_CLAMP_TO_EDGE;
		break;
	}
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_iTexWrapS);
  //## end OGL_TextureDesc::SetSWrapMode%975424982.body
}

void OGL_TextureDesc::SetMagFilter (E3D_EMagFilter _eMagFilter)
{
  //## begin OGL_TextureDesc::SetMagFilter%975424983.body preserve=yes
	switch (_eMagFilter)
	{
	case E3D_EMagFilter_NEAREST:
		m_iMagFilter = GL_NEAREST;
		break;
	case E3D_EMagFilter_LINEAR:
	default:
		m_iMagFilter = GL_LINEAR;
		break;
	}
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_iMagFilter);	
  //## end OGL_TextureDesc::SetMagFilter%975424983.body
}

void OGL_TextureDesc::SetMinFilter (E3D_EMinFilter _eMinFilter)
{
  //## begin OGL_TextureDesc::SetMinFilter%975424984.body preserve=yes
	switch (_eMinFilter)
	{
	case E3D_EMinFilter_NEAREST:
		m_iMinFilter = GL_NEAREST;
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
	case E3D_EMinFilter_LINEAR:
	default:
		m_iMinFilter = GL_LINEAR;
		break;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_iMinFilter);
  //## end OGL_TextureDesc::SetMinFilter%975424984.body
}

E3D_EWrapMode OGL_TextureDesc::eGetTWrapMode ()
{
  //## begin OGL_TextureDesc::eGetTWrapMode%975602555.body preserve=yes
	switch (m_iTexWrapT)
	{
	case GL_CLAMP:
	case GL_CLAMP_TO_EDGE:
		return E3D_EWrapMode_CLAMP;
		break;
		/*
	case E3D_EWrapMode_CLAMP_TO_EDGE:
		m_iTexWrapT = GL_CLAMP;
		break;
		*/
	case GL_REPEAT:
	default:
		return E3D_EWrapMode_REPEAT;
		break;
	}
  //## end OGL_TextureDesc::eGetTWrapMode%975602555.body
}

E3D_EWrapMode OGL_TextureDesc::eGetSWrapMode ()
{
  //## begin OGL_TextureDesc::eGetSWrapMode%975602556.body preserve=yes
	switch (m_iTexWrapS)
	{
	case GL_CLAMP:
	case GL_CLAMP_TO_EDGE:
		return E3D_EWrapMode_CLAMP;		
		break;
		/*
	case E3D_EWrapMode_CLAMP_TO_EDGE:
		m_iTexWrapT = GL_CLAMP;
		break;
		*/
	case GL_REPEAT:
	default:
		return E3D_EWrapMode_REPEAT;
		break;
	}
  //## end OGL_TextureDesc::eGetSWrapMode%975602556.body
}

E3D_EMagFilter OGL_TextureDesc::eGetMagFilter ()
{
  //## begin OGL_TextureDesc::eGetMagFilter%975602557.body preserve=yes
	switch (m_iMagFilter)
	{
	case GL_NEAREST:
		return E3D_EMagFilter_NEAREST;
		break;
	case GL_LINEAR:
	default:
		return E3D_EMagFilter_LINEAR;
		break;
	}	
  //## end OGL_TextureDesc::eGetMagFilter%975602557.body
}

E3D_EMinFilter OGL_TextureDesc::eGetMinFilter ()
{
  //## begin OGL_TextureDesc::eGetMinFilter%975602558.body preserve=yes
	switch (m_iMinFilter)
	{
	case GL_NEAREST:
		return E3D_EMinFilter_NEAREST;
		break;
	case GL_LINEAR: 
		return E3D_EMinFilter_LINEAR;
		break;
	case GL_NEAREST_MIPMAP_LINEAR:
		return E3D_EMinFilter_NEAREST_MIPMAP_LINEAR;
		break;
	case GL_NEAREST_MIPMAP_NEAREST:
		return E3D_EMinFilter_NEAREST_MIPMAP_NEAREST;
		break;
	case GL_LINEAR_MIPMAP_LINEAR:
		return E3D_EMinFilter_LINEAR_MIPMAP_LINEAR;
		break;
	case GL_LINEAR_MIPMAP_NEAREST:
	default:
		return E3D_EMinFilter_LINEAR_MIPMAP_NEAREST;
		break;
	}
  //## end OGL_TextureDesc::eGetMinFilter%975602558.body
}

void OGL_TextureDesc::UploadImageTo (unsigned char* _poBuffer, unsigned long _ulBufferLen)
{
  //## begin OGL_TextureDesc::UploadImageTo%975667577.body preserve=yes
	assert(lGetSize() <= _ulBufferLen);
	BindTexture ();
	GLenum format;
	switch (m_ePixelFmt)
	{
	case E3D_EPixelFormat_RGBA32: 
		format = GL_RGBA;
		break;
	case E3D_EPixelFormat_RGB24: 
		format = GL_RGB;	
		break;
	case E3D_EPixelFormat_RGBA16: 
		format = GL_RGBA;
		break;
	case E3D_EPixelFormat_PAL8: 
		format = GL_COLOR_INDEX;
		break;
	case E3D_EPixelFormat_PAL4: 
		format = GL_COLOR_INDEX;
		break;
	}	
	glGetTexImage(GL_TEXTURE_2D,0,format, GL_UNSIGNED_BYTE, _poBuffer);
  //## end OGL_TextureDesc::UploadImageTo%975667577.body
}

void OGL_TextureDesc::EnableAlphaTesting ()
{
  //## begin OGL_TextureDesc::EnableAlphaTesting%994406999.body preserve=yes
	m_ucEnableAlphaTesting = 1;
  //## end OGL_TextureDesc::EnableAlphaTesting%994406999.body
}

void OGL_TextureDesc::DisableAlphaTesting ()
{
  //## begin OGL_TextureDesc::DisableAlphaTesting%994407000.body preserve=yes
	m_ucEnableAlphaTesting = 0;
  //## end OGL_TextureDesc::DisableAlphaTesting%994407000.body
}

unsigned char OGL_TextureDesc::bIsEnableAlphaTesting ()
{
  //## begin OGL_TextureDesc::bIsEnableAlphaTesting%994407002.body preserve=yes
	return this->m_ucEnableAlphaTesting;
  //## end OGL_TextureDesc::bIsEnableAlphaTesting%994407002.body
}

void OGL_TextureDesc::EnableOrder ()
{
  //## begin OGL_TextureDesc::EnableOrder%995276432.body preserve=yes
	m_ucEnableOrder = 1;
  //## end OGL_TextureDesc::EnableOrder%995276432.body
}

void OGL_TextureDesc::DisableOrder ()
{
  //## begin OGL_TextureDesc::DisableOrder%995276433.body preserve=yes
	m_ucEnableOrder = 0;
  //## end OGL_TextureDesc::DisableOrder%995276433.body
}

unsigned char OGL_TextureDesc::bIsEnableOrder ()
{
  //## begin OGL_TextureDesc::bIsEnableOrder%995276434.body preserve=yes
	return m_ucEnableOrder;
  //## end OGL_TextureDesc::bIsEnableOrder%995276434.body
}

float OGL_TextureDesc::fGetAlphaTestingValue ()
{
  //## begin OGL_TextureDesc::fGetAlphaTestingValue%995276439.body preserve=yes
	return m_fAlphaTestingValue;
  //## end OGL_TextureDesc::fGetAlphaTestingValue%995276439.body
}

void OGL_TextureDesc::SetAlphaTestingValue (float _fValue)
{
  //## begin OGL_TextureDesc::SetAlphaTestingValue%995276440.body preserve=yes
	m_fAlphaTestingValue = _fValue;
  //## end OGL_TextureDesc::SetAlphaTestingValue%995276440.body
}

E3D_EAlphaFunc OGL_TextureDesc::fGetAlphaTestingFunc ()
{
  //## begin OGL_TextureDesc::fGetAlphaTestingFunc%995276445.body preserve=yes
	return m_eAlphaFunc;
  //## end OGL_TextureDesc::fGetAlphaTestingFunc%995276445.body
}

void OGL_TextureDesc::fSetAlphaTestingFunc (E3D_EAlphaFunc _eAlphaFunc)
{
  //## begin OGL_TextureDesc::fSetAlphaTestingFunc%995276446.body preserve=yes
	m_eAlphaFunc = _eAlphaFunc;
  //## end OGL_TextureDesc::fSetAlphaTestingFunc%995276446.body
}

// Additional Declarations
  //## begin OGL_TextureDesc%3920FAAA0167.declarations preserve=yes
void OGL_TextureDesc::SetTextureGroup(int _iGroup)
{
	m_iGroupId = _iGroup;
}
  //## end OGL_TextureDesc%3920FAAA0167.declarations

//## begin module%3920FAAA0167.epilog preserve=yes
//## end module%3920FAAA0167.epilog
