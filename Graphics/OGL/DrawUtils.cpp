// ------------------------------------------------------------------------
#include "DrawUtils.h"

#ifdef _XBOX
#include <d3d8.h>
#include <xgmath.h>
#endif

#ifndef _XBOX
#define USE_NORMALIZATION
#endif
// ------------------------------------------------------------------------
// USE_NORMALIZATION: Deseamos obtener coordenadas normalizadas en el rango (-1,1)
// con independencia de las dimensiones del modo y resolución seleccionada
// de la pantalla. Para ello hemos de definir una resolución estándar como 
// marco de trabajo.
// ------------------------------------------------------------------------
float fCorrectX(float _fCoord)
{
	#ifdef USE_NORMALIZATION
	return ( 2.0f*((_fCoord) / STD_SCR_WIDTH) - 1.0f);
	#else
	return ( _fCoord - 0.5f);
	#endif
}

float fCorrectY(float _fCoord)
{
	#ifdef USE_NORMALIZATION
	return ( 2.0f*(1.0f - (_fCoord) / STD_SCR_HEIGHT) - 1.0f );
	#else
	return ( (STD_SCR_HEIGHT - (_fCoord - 0.5f)) );
	#endif
}
// ------------------------------------------------------------------------
void RenderQUAD(CDMCTex *_poTex,
				float _fIX,float _fIY,float _fFX,float _fFY,
				float _fIU,float _fIV,float _fFU,float _fFV,
				unsigned int _uiColor,float _fA)
{
	E3D_Color oColor(
					(float)( _uiColor       & 0xff)/255.0f,
					(float)((_uiColor >>  8)& 0xff)/255.0f,
					(float)((_uiColor >> 16)& 0xff)/255.0f,
					_fA);
	
	// Render Line
	if (_poTex)
	{
		_poTex->SetFilter(E3D_EMagFilter_NEAREST,E3D_EMinFilter_NEAREST);
		_poTex->MakeCurrent();
	}
	else
		gDC->Disable(E3D_ESetting_TEXTURE);		

	gDC->PushMatrix(DC_EMatrixMode_MODELVIEW);
	gDC->PushMatrix(DC_EMatrixMode_PROJECTION);
	gDC->LoadIdentity(DC_EMatrixMode_MODELVIEW);
	gDC->LoadIdentity(DC_EMatrixMode_PROJECTION);
	
#if defined _XBOX && ! defined(_X360)
	
	_uiColor &= 0x00ffffff;
	_uiColor |= ((unsigned int)(_fA*255.0f)) << 24;

	// Save old vertex shaders
	DWORD dwOldVS;
	gDC->m_pod3dDevice->GetVertexShader( &dwOldVS );	

	// Avoid DX crashing for non power of 2 textures
	gDC->m_pod3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
	gDC->m_pod3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);

		gDC->m_pod3dDevice->SetVertexShader( D3DFVF_XYZ | D3DFVF_TEXCOORDSIZE2(0) );
	
		gDC->m_pod3dDevice->Begin( D3DPT_QUADLIST );

			gDC->m_pod3dDevice->SetVertexDataColor( D3DVSDE_DIFFUSE, _uiColor );

			gDC->m_pod3dDevice->SetVertexData2f( D3DVSDE_TEXCOORD0,		_fIU		    ,  _fFV);
			gDC->m_pod3dDevice->SetVertexData4f( D3DVSDE_VERTEX,		fCorrectX(_fIX)	,  fCorrectY(_fIY), 0.5f, 1.0f ); // left/bottom

			gDC->m_pod3dDevice->SetVertexData2f( D3DVSDE_TEXCOORD0,		_fFU            ,  _fFV);
			gDC->m_pod3dDevice->SetVertexData4f( D3DVSDE_VERTEX,		fCorrectX(_fFX)	,  fCorrectY(_fIY), 0.5f, 1.0f );	// right/bottom

			gDC->m_pod3dDevice->SetVertexData2f( D3DVSDE_TEXCOORD0,		_fFU		    ,  _fIV);
			gDC->m_pod3dDevice->SetVertexData4f( D3DVSDE_VERTEX,		fCorrectX(_fFX)	,  fCorrectY(_fFY), 0.5f, 1.0f );	// right/top

			gDC->m_pod3dDevice->SetVertexData2f( D3DVSDE_TEXCOORD0,		_fIU		    ,  _fIV);
			gDC->m_pod3dDevice->SetVertexData4f( D3DVSDE_VERTEX,		fCorrectX(_fIX)	,  fCorrectY(_fFY), 0.5f, 1.0f );  // left/top


		gDC->m_pod3dDevice->End();

	// Restore old vertex shader
	gDC->m_pod3dDevice->SetVertexShader( dwOldVS );

#else

		gDC->BeginQuad();
			
			gDC->Color(oColor);
			gDC->TexCoord(_fIU,_fIV);
			gDC->Vertex2f(fCorrectX(_fIX),fCorrectY(_fIY));
			
			gDC->Color(oColor);
			gDC->TexCoord(_fIU,_fFV);
			gDC->Vertex2f(fCorrectX(_fIX),fCorrectY(_fFY));
			
			gDC->Color(oColor);
			gDC->TexCoord(_fFU,_fFV);
			gDC->Vertex2f(fCorrectX(_fFX),fCorrectY(_fFY));

			gDC->Color(oColor);
			gDC->TexCoord(_fFU,_fIV);
			gDC->Vertex2f(fCorrectX(_fFX),fCorrectY(_fIY));

		gDC->EndQuad();	
#endif

	gDC->PopMatrix(DC_EMatrixMode_MODELVIEW);
	gDC->PopMatrix(DC_EMatrixMode_PROJECTION);
	
	if (! _poTex)
		gDC->Enable(E3D_ESetting_TEXTURE);	
}


void SetViewport (int _iX, int _iY, int _iWidth, int _iHeight)
{
	float fVFact = ((float)SCR_HEIGHT / (float)STD_SCR_HEIGHT);
	float fHFact = ((float)SCR_WIDTH  /  (float)STD_SCR_WIDTH);

    gDC->SetViewport (_iX * fHFact, _iY* fVFact, _iWidth * fHFact, _iHeight * fVFact);
}