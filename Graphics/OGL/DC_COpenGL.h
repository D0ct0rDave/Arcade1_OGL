//## begin module%38EB1E6B0157.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%38EB1E6B0157.cm

//## begin module%38EB1E6B0157.cp preserve=no
//## end module%38EB1E6B0157.cp

//## Module: DC_COpenGL%38EB1E6B0157; Pseudo Package specification
//## Source file: D:\ProRally\Realization\ENGINE3D\DC\DC_COpenGL.h

#ifndef DC_COpenGL_h
#define DC_COpenGL_h 1

//## begin module%38EB1E6B0157.additionalIncludes preserve=no
//## end module%38EB1E6B0157.additionalIncludes

//## begin module%38EB1E6B0157.includes preserve=yes
#ifdef WIN32

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glext.h>		// REQUIRES OPENGL1.1 / 1.2
typedef void (APIENTRY * PGLBLENDEQUATION) (GLint i);

#endif
//## end module%38EB1E6B0157.includes

// E3D_CDisplayContext
#include "..\E3D_CDisplayContext.h"
// OGL_ClutDesc
#include "OGL_ClutDesc.h"
// OGL_TextureDesc
#include "OGL_TextureDesc.h"
//## begin module%38EB1E6B0157.additionalDeclarations preserve=yes

#define	DC_COpenGL_MAXTEXTURES 1024*2
//## end module%38EB1E6B0157.additionalDeclarations


//## begin DC_COpenGL%38EB1E6B0157.preface preserve=yes

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
// LIGHTING
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
typedef struct{
	bool			bEnabled;		
	int				iType;			// 0: Ambient, 1: Diffuse
	MTH_CPoint3		oPos;
	MTH_CPoint3		oDir;
	/*
	float			fAmb[4];		// 
	float			fDiff[4];		// 
	float			fSpec[4];		// 
	*/
}TLightProps;

//-----------------------------------------------------------------------------------------

#define MAX_LIGHTS 8


//## end DC_COpenGL%38EB1E6B0157.preface

//## Class: DC_COpenGL%38EB1E6B0157
//	A DisplayContext that encapsulates OpenGL. Uses Vertex
//	Transform Acceleration (if available).
//## Category: Realization::ENGINE3D::DC%38ED8CDF0212
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3920FABE00CB;OGL_TextureDesc { -> }
//## Uses: <unnamed>%3A895A0A01F4;OGL_ClutDesc { -> }

class DC_COpenGL : public E3D_CDisplayContext  //## Inherits: <unnamed>%38EB1E8801A5
{
  //## begin DC_COpenGL%38EB1E6B0157.initialDeclarations preserve=yes
	public:
		void Init(void *pParam);
  //## end DC_COpenGL%38EB1E6B0157.initialDeclarations

  public:
    //## Constructors (generated)
      DC_COpenGL();

    //## Destructor (generated)
      ~DC_COpenGL();


    //## Other Operations (specified)
      //## Operation: LoadMatrix%955007271
      virtual void LoadMatrix (MTH_CMat4x4& _rMat44);

      //## Operation: ClearViewport2d%955007272
      virtual void ClearViewport2d ();

      //## Operation: PushMatrix%955007273
      virtual void PushMatrix ();

      //## Operation: PopMatrix%955007274
      virtual void PopMatrix ();

      //## Operation: LoadIdentity%955007276
      virtual void LoadIdentity ();

      //## Operation: MultMatrix%955007277
      virtual void MultMatrix (MTH_CMat4x4& _rMatrix);
	  
	  
	  // DMC: Si se mete este código en E3D_CDisplayContext, no será necesario replicar en todas las plataformas
	  virtual void LoadMatrix(DC_EMatrixMode _eMode,MTH_CMat4x4& _rMat44);
	  virtual void PushMatrix(DC_EMatrixMode _eMode);
	  virtual void PopMatrix(DC_EMatrixMode _eMode);
	  virtual void MultMatrix(DC_EMatrixMode _eMode,MTH_CMat4x4& _rMatrix);
	  virtual void LoadIdentity(DC_EMatrixMode _eMode);
	  virtual void GetMatrix (DC_EMatrixMode _eMode,MTH_CMat4x4& _rMat);

		
      //## Operation: Rotate%955007278
      virtual void Rotate (float _fAngle, float _fX, float _fY, float _fZ);

      //## Operation: Scale%955007279
      virtual void Scale (float _fX, float _fY, float _fZ);

      //## Operation: Translate%955007280
      virtual void Translate (float _fX, float _fY, float _fZ);

      //## Operation: SetViewport2d%955007282
      virtual void SetViewport2d (E3D_CViewport2D& _rVp2d);

      //## Operation: PushState%955007283
      virtual void PushState ();

      //## Operation: PopState%955007284
      virtual void PopState ();

      //## Operation: GetMatrix%955007303
      virtual void GetMatrix (MTH_CMat4x4& _rMat);

      //## Operation: SwapBuffers%955010534
      virtual void SwapBuffers ();

      //## Operation: BeginStrip%955091997
      virtual void BeginStrip ();

      //## Operation: EndStrip%955091998
      virtual void EndStrip ();

      //## Operation: BeginFan%955091999
      virtual void BeginFan ();

      //## Operation: EndFan%955092000
      virtual void EndFan ();

      //## Operation: Vertex%955092001
      virtual void Vertex (MTH_CPoint3 &_rPoint);

      //## Operation: ClearBuffer%955092002
      virtual void ClearBuffer (int _eBuffer = (E3D_EBuffer_DEPTH));

      //## Operation: SetWindow%955099841
      void SetWindow (/*HWND*/int _hWnd);

      //## Operation: Vertex%955100772
      virtual void Vertex (float _x, float _y, float _z);

      //## Operation: Frustum%955356746
      virtual void Frustum (float _fLeft, float _fRight, float _fBottom, float _fTop, float _fZnear, float _fZfar);

      //## Operation: Flush%955356749
      virtual void Flush ();

      //## Operation: BeginTriangle%955376251
      virtual void BeginTriangle ();

      //## Operation: EndTriangle%955376252
      virtual void EndTriangle ();

      //## Operation: GetViewport2d%956848990
      virtual void GetViewport2d (E3D_CViewport2D& _rVp2d);

      //## Operation: Color%956927699
      //	This function sets the current color for a vertex. In
      //	particular, you can call Color between a call to Begin
      //	XXX and the corresponding call to EndXXX.
      virtual void Color (E3D_Color &_roColor);
      virtual void Color (float _fR,float _fG,float _fB,float _fA);
      virtual void Color (unsigned int _uiColor);

      //## Operation: SetVertexArray%956928906
      virtual void SetVertexArray (float *_pfTable, int _iNbVertex, int _iStride);

      //## Operation: SetColorArray%956928907
      virtual void SetColorArray (float *_pfTable, int _iNbColor, int _iStride);

	  // ALEX R.C.
	  virtual void SetColorArray (int *_piTable, int _iNbColor, int _iStride);

      //## Operation: SetNormalArray%956928908
      virtual void SetNormalArray (float *_pfTable, int _iNbNormal, int _iStride);

      //## Operation: SetTexCoordArray%956928909
      virtual void SetTexCoordArray (float *_pfTable, int _iNbTexCoord, int _iStride);

      //## Operation: DrawIndexedVertexSet%957256274
      virtual void DrawIndexedVertexSet (unsigned short *_pwTableIdx, int _iNbVertexIdx, int _iStride, E3D_EFaceType _eFace, E3D_EBinding _eColorBind = E3D_EBinding_PERVERTEX, E3D_EBinding _eNormBind = E3D_EBinding_PERVERTEX);

      //## Operation: GetCurrentMatrix%957454859
      virtual void GetCurrentMatrix (MTH_CMat4x4& _roMatrix);

      //## Operation: Frustum%958140738
      //	Set up perspective projection parameters.
      //
      //	_fFov: Field of View angle, in degrees, in the y
      //	direction.
      //
      //	_fAspect: Aspect Ratio that determines the field of view
      //	in the x direction. This is the ratio of x (width) to y
      //	(height).
      //
      //	_fZnear: Distance from the viewer to the near clipping
      //	plane (always positive).
      //
      //	_fZfar: Distance from the viewer to the far clipping
      //	plane (always positive).
      //
      //	Depth buffer precision is affected by the values
      //	specified for _fZnear and _fZfar. The greater the ratio
      //	r = (_fZfar / _fZnear) is, the less effective the depth
      //	buffer will be at distinguishing between surfaces that
      //	are near each other. Roughly log2(r) bits of depth
      //	buffer precision are lost. Because r approaches infinity
      //	as _fZnear approaches zero, _fZnear must never be set to
      //	zero.
      virtual void Frustum (float _fFov, float _fZnear, float _fZfar, float _fAspect);

      //## Operation: DestroyTexture%958398199
      virtual void DestroyTexture (E3D_TextureDesc* _poTexDesc);

      //## Operation: SetCurrentTexture%958398200
      virtual void SetCurrentTexture (E3D_TextureDesc* _poTexDesc);

      //## Operation: TexCoord%958472417
      virtual void TexCoord (float _u, float _v);

      //## Operation: Normal%958472418
      virtual void Normal (MTH_CPoint3& _roNorm);

      //## Operation: Enable%958549574
      //	The Enable and Disable functions enable and disable
      //	various capabilities.
      virtual void Enable (E3D_ESetting _eSetting);

      //## Operation: Disable%958549575
      //	The Enable and Disable functions enable and disable
      //	various capabilities.
      virtual void Disable (E3D_ESetting _eSetting);

      //## Operation: bIsEnabled%958549576
      virtual int bIsEnabled (E3D_ESetting _eSetting);

      //## Operation: poCreateCLUT%959155152
      //	Creates a new CLUT (Color Look up Table) and returns the
      //	associated texture descriptor. To enable it use the E3D_
      //	CDisplayContext::SetCurrentTexture() method with the
      //	returned CLUT texture descriptor. The driver knows that
      //	this is a special CLUT texture and sets the
      //	corresponding hardware, internal CLUT.
      //
      //	This function doesn't accept indexed textures.
      virtual E3D_TextureDesc* poCreateCLUT (int _iNbEntries, E3D_EPixelFormat _ePixel, unsigned char *_pucImage);

      //## Operation: BlendScaleFactors%961362827
      virtual void BlendScaleFactors (E3D_EBlendScale _eSrc, E3D_EBlendScale _eDst);

      //## Operation: SetMatrixMode%975067194
      virtual void SetMatrixMode (DC_EMatrixMode _eMode);

      //## Operation: eGetMatrixMode%975067195
      virtual DC_EMatrixMode eGetMatrixMode ();

      //## Operation: BeginLine%975067198
      virtual void BeginLine ();

      //## Operation: BeginLineFan%975067199
      virtual void BeginLineFan ();

      //## Operation: BeginLineStrip%975067200
      virtual void BeginLineStrip ();

      //## Operation: EndLine%975067201
      virtual void EndLine ();

      //## Operation: EndLineFan%975067202
      virtual void EndLineFan ();

      //## Operation: EndLineStrip%975067203
      virtual void EndLineStrip ();

      //## Operation: EndPoint%975067204
      virtual void EndPoint ();

      //## Operation: UpdateLight%975067207
      virtual void UpdateLight (char _cId);

      //## Operation: BeginPoint%975067208
      virtual void BeginPoint ();

      //## Operation: GettLight%975067209
      virtual void GettLight (LIG_PositionalLight* _oPositionalLight, char _cId);

      //## Operation: unBlindLight%975067210
      virtual void unBlindLight (char _cId);

      //## Operation: cBlindLight%975067211
      virtual char cBlindLight (LIG_PositionalLight* _oPositionalLight);

      //## Operation: SetTexGenModeS%975424988
      //	When DC_ETexGenMode_OBJECTLINEAR
      virtual void SetTexGenModeS (DC_ETexGenMode _eTexGenMode, float _a4fParam[4] = NULL);

      //## Operation: SetTexGenModeT%975424989
      //	When DC_ETexGenMode_OBJECTLINEAR
      virtual void SetTexGenModeT (DC_ETexGenMode _eTexGenMode, float _a4fParam[4] = NULL);

      //## Operation: poCreateTexture%975599247
      //	Insert an image 2D into the texture dictionary.
      //
      //	After creating a texture you may be required to keep a
      //	copy of the texture in system memory (for the texture
      //	cache).  The _pucImage parameter is a pointer to the
      //	beginning of the texture data in system memory. The
      //	remainder parameters are a description of the image,
      //	which is useful to the texture management when reload of
      //	texture is necessary.
      //
      //	Call E3D_TextureDesc::Invalidate() to tell the display
      //	context that the texture needs to be entirely reloaded
      //	from system memory into texture memory.
      //
      //	Returns a pointer to the texture descriptor.
      virtual E3D_TextureDesc* poCreateTexture (int _iWidth, int _iHeight, E3D_EPixelFormat _ePixel, unsigned char *_pucImage, E3D_EWrapMode _eTWrapMode = E3D_EWrapMode_CLAMP, E3D_EWrapMode _eSWrapMode = E3D_EWrapMode_CLAMP, E3D_EMagFilter _eMagFilter = E3D_EMagFilter_NEAREST, E3D_EMinFilter _eMinFilter = E3D_EMinFilter_NEAREST,int _iNumLODs = 1,unsigned char *_pucPal=NULL);

      //## Operation: SetFogColor%983288695
      virtual void SetFogColor (E3D_Color &_roColor);

      //## Operation: SetFogDensity%983288696
      virtual void SetFogDensity (float _fNear, float _fFar, float _fValue);

      //## Operation: SetFogMode%983288699
      virtual void SetFogMode (E3D_EFogMode _eFogMode);

      //## Operation: SetBackgroundColor%985613127
      virtual void SetBackgroundColor (E3D_Color &_roColor);

      //## Operation: SetLineWidth%985613128
      virtual void SetLineWidth (float _fWidth);

      //## Operation: SetCullFaceMode%990608643
      virtual void SetCullFaceMode (E3D_ECullFaceMode _eCullFaceMode);

      //## Operation: eGetCullFaceMode%990623005
      virtual E3D_ECullFaceMode eGetCullFaceMode ();

      //## Operation: SetAlphaFunc%994343011
      virtual void SetAlphaFunc (E3D_EAlphaFunc _eAlphaFunc, float _fRefValue);

      //## Operation: eGetAlphaFunc%994343012
      virtual E3D_EAlphaFunc eGetAlphaFunc ();

      //## Operation: SetTWrapMode%994678138
      virtual void SetTWrapMode (E3D_EWrapMode _eWrapMode);

      //## Operation: SetSWrapMode%994678139
      virtual void SetSWrapMode (E3D_EWrapMode _eWrapMode);

      //## Operation: SetMagFilter%994678140
      virtual void SetMagFilter (E3D_EMagFilter _eMagFilter);

      //## Operation: SetMinFilter%994678141
      virtual void SetMinFilter (E3D_EMinFilter _eMinFilter);

      //## Operation: SetScissor%994678144
      virtual void SetScissor (float _x0, float _y0, float _x1, float _y1);

      //## Operation: SetShadingModel%994678146
      virtual void SetShadingModel (E3D_EShadingMode _eShadingMode);

      //## Operation: Vertex2f%994678148
      virtual void Vertex2f (float _x, float _y);

      //## Operation: SetTexEnvf%994678150
      virtual void SetTexEnvf (E3D_ETextureEnvMode _eTextureEnvMode);

      //## Operation: SetOrtho%994752933
      virtual void SetOrtho (float _fLeft, float _fRight, float _fBottom, float _fTop, float _fZNear, float _fZFar);

      //## Operation: SetViewport%994752934
      virtual void SetViewport (int _iX, int _iY, int _iWidth, int _iHeight);

      //## Operation: SetPixelStorei%994752937
      virtual void SetPixelStorei (E3D_EPixelStoreMode _ePixelStoreMode);

      //## Operation: BeginPrimitive%994758211
      virtual void BeginPrimitive (unsigned char _ucPrimitive);

      //## Operation: EndPrimitive%994758212
      virtual void EndPrimitive (unsigned char _ucPrimitive);

      //## Operation: DefineCurrentTexture%995294306
      virtual void DefineCurrentTexture (int _iWidth, int _iHeight, E3D_EPixelFormat _ePixel, unsigned char *_pucImage);

      //## Operation: SetCurrentPalette%996573935
      virtual void SetCurrentPalette (E3D_TextureDesc* _poTexDesc);

      //## Operation: BeginSprites%996675434
      virtual void BeginSprites ();

      //## Operation: EndSprites%996675435
      virtual void EndSprites ();

      //## Operation: SetColorArrayAlpha%1000366634
      virtual void SetColorArrayAlpha (float *_pfTable, int _iNbColor, int _iStride);

      //## Operation: BeginQuad%1003849248
      virtual void BeginQuad ();

      //## Operation: EndQuad%1003849249
      virtual void EndQuad ();

      //## Operation: EnableClientState%1003849250
      virtual void EnableClientState (E3D_EClientState _eState);

      //## Operation: DisableClientState%1003849251
      virtual void DisableClientState (E3D_EClientState _eState);

      //## Operation: SetVertexArray%1003849252
      virtual void SetVertexArray (int _iSize, float *_pfTable, int _iNbVertex, int _iStride);

      //## Operation: SetColorArray%1003849253
      virtual void SetColorArray (int _iSize, float *_pfTable, int _iNbColor, int _iStride);

	  

      //## Operation: SetTexCoordArray%1003849255
      virtual void SetTexCoordArray (int _iSize, float *_pfTable, int _iNbTexCoord, int _iStride);

      //## Operation: SetNormalArray%1003999242
      virtual void SetNormalArray (int _iSize, float *_pfTable, int _iNbNormal, int _iStride);

      //## Operation: DrawArrays%1003849256
      virtual void DrawArrays (E3D_EFaceType _eFaceType, int _iFirst, int _iCount);

      //## Operation: SetBlendEquation%1006444255
      virtual void SetBlendEquation (E3D_EBlendEquation _eBlendEquation);

      //## Operation: DestroyAllTextures%1007468261
      virtual void DestroyAllTextures ();

	

    // Data Members for Class Attributes

      //## Attribute: m_hWnd%38EDABE202A8
      //## begin DC_COpenGL::m_hWnd%38EDABE202A8.attr preserve=no  private: HWND {U} 
      //## end DC_COpenGL::m_hWnd%38EDABE202A8.attr

      //## Attribute: m_poTexDesc%3921243A038A
      //## begin DC_COpenGL::m_poTexDesc%3921243A038A.attr preserve=no  private: OGL_TextureDesc* {U} NULL
      OGL_TextureDesc* m_poTexDesc;
      //## end DC_COpenGL::m_poTexDesc%3921243A038A.attr

      //## Attribute: m_iNbTex%39212CE20128
      //## begin DC_COpenGL::m_iNbTex%39212CE20128.attr preserve=no  private: int {U} 0
      int m_iNbTex;
      //## end DC_COpenGL::m_iNbTex%39212CE20128.attr

      //## Attribute: m_poPositionalLights%3A1E699003C8
      //## begin DC_COpenGL::m_poPositionalLights%3A1E699003C8.attr preserve=no  private: LIG_PositionalLight*[ 4] {U} 
      LIG_PositionalLight* m_poPositionalLights[ 4];
      //## end DC_COpenGL::m_poPositionalLights%3A1E699003C8.attr

      //## Attribute: m_oBackgroundColor%3ABF45000196
      //## begin DC_COpenGL::m_oBackgroundColor%3ABF45000196.attr preserve=no  private: E3D_Color {U} 
      E3D_Color m_oBackgroundColor;
      //## end DC_COpenGL::m_oBackgroundColor%3ABF45000196.attr

      //## Attribute: m_fLineWidth%3ABF45CA031C
      //## begin DC_COpenGL::m_fLineWidth%3ABF45CA031C.attr preserve=no  private: float {U} 
      float m_fLineWidth;
      //## end DC_COpenGL::m_fLineWidth%3ABF45CA031C.attr

      //## Attribute: m_eCullingMode%3B0BB59F0186
      //## begin DC_COpenGL::m_eCullingMode%3B0BB59F0186.attr preserve=no  private: E3D_ECullFaceMode {U} 
      E3D_ECullFaceMode m_eCullingMode;
      //## end DC_COpenGL::m_eCullingMode%3B0BB59F0186.attr

      //## Attribute: m_eAlphaFunc%3B447E0303C8
      //## begin DC_COpenGL::m_eAlphaFunc%3B447E0303C8.attr preserve=no  private: E3D_EAlphaFunc {U} 
      E3D_EAlphaFunc m_eAlphaFunc;
      //## end DC_COpenGL::m_eAlphaFunc%3B447E0303C8.attr

      //## Attribute: m_iTexWrapT%3B49B151030D
      //## begin DC_COpenGL::m_iTexWrapT%3B49B151030D.attr preserve=no  private: GLint {U} 
      int m_iTexWrapT;
      //## end DC_COpenGL::m_iTexWrapT%3B49B151030D.attr

      //## Attribute: m_iTexWrapS%3B49B19A001F
      //## begin DC_COpenGL::m_iTexWrapS%3B49B19A001F.attr preserve=no  private: GLint {U} 
      int m_iTexWrapS;
      //## end DC_COpenGL::m_iTexWrapS%3B49B19A001F.attr

      //## Attribute: m_iMagFilter%3B49B1B70280
      //## begin DC_COpenGL::m_iMagFilter%3B49B1B70280.attr preserve=no  private: GLint {U} 
      int m_iMagFilter;
      //## end DC_COpenGL::m_iMagFilter%3B49B1B70280.attr

      //## Attribute: m_iMinFilter%3B49B200032C
      //## begin DC_COpenGL::m_iMinFilter%3B49B200032C.attr preserve=no  private: GLint {U} 
      int m_iMinFilter;
      //## end DC_COpenGL::m_iMinFilter%3B49B200032C.attr

    // Additional Public Declarations
      //## begin DC_COpenGL%38EB1E6B0157.public preserve=yes
	  // ALEX R.C.
	  virtual void SetColorArray (int _iSize, int *_piTable, int _iNbColor, int _iStride);
	  // ALEX R.C.
      virtual void DestroyClut (E3D_TextureDesc* _poClutDesc);
      // ALEX R.C.
	  virtual void DestroyAllCluts ();
	  
	  // Pac's functions...
	  virtual void WriteZBuffer (int _bool);
	  virtual void ColorMask(int _R,int _G,int _B,int _A);
	  virtual void EnableZBias	(void);
	  virtual void DisableZBias (void);
	  virtual void SetZBiasLevel(float level);
	  virtual void DepthFunc	(E3D_EDepthFunc _depth);

	  // <JSPG>
	  virtual void DestroyTexturesOfGroup(int _iGroup);

      OGL_ClutDesc* m_poClutDesc;


		// <DMC: Blendmode>
	  	virtual void BlendMode(E3D_BlendMode _eBlendMode);
		virtual E3D_BlendMode eGetBlendMode();
		virtual void ConstantAlpha(float _fCAlpha);

		E3D_BlendMode	m_eBlendMode;
		float			m_fCAlpha;

		// Lighting
		virtual void EnableLighting(int _iForceUpdate = 0);
		virtual void DisableLighting();
		virtual void ActivateLight(int _iLightIdx,int _iOnOff);
		virtual void SetLightPosition(int _iLightIdx, MTH_CPoint3& _roPosition);
		virtual void SetLightDirection(int _iLightIdx, MTH_CPoint3& _roDirection);
		virtual void SetLightType(int _iLightIdx, int _iLightType);
		virtual void SetLightColor(int _iLightIdx, E3D_Color &_roColor);
		virtual void SetLightRange(int _iLightIdx, float _fRange);



		virtual void SetLightSelectionFunc(TLightSelectionFunc _pLSFunc);
		virtual void SelectLights(MTH_CPoint3& _roPos);		
		
		TLightSelectionFunc pLSFunc;
		bool				bLightingEnabled;
		TLightProps			oLights[MAX_LIGHTS];

		// specific (?)
		void SetLightAmbientColor	(int _iLightIdx, E3D_Color &_roColor);
		void SetLightAmbientColor	(int _iLightIdx, float *_pfColor);
		void SetLightDiffuseColor	(int _iLightIdx, E3D_Color &_roColor);
		void SetLightDiffuseColor	(int _iLightIdx, float *_pfColor);
		void SetLightSpecularColor	(int _iLightIdx, E3D_Color &_roColor);
		void SetLightSpecularColor	(int _iLightIdx, float *_pfColor);
      //## end DC_COpenGL%38EB1E6B0157.public


		virtual int iCreateDisplayList();
		virtual void BeginDisplayList(int _iListID);
		virtual void EndDisplayList();
		virtual void CallDisplayList(int _iListID);
	
	//  protected:
    // Additional Protected Declarations
      //## begin DC_COpenGL%38EB1E6B0157.protected preserve=yes
      E3D_CViewport2D m_viewport;
      //## end DC_COpenGL%38EB1E6B0157.protected
  private:
    // Additional Private Declarations
      //## begin DC_COpenGL%38EB1E6B0157.private preserve=yes
      //## end DC_COpenGL%38EB1E6B0157.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin DC_COpenGL%38EB1E6B0157.implementation preserve=yes
	  #ifdef WIN32
		HGLRC				m_hGLRC;
		HDC					m_hGLDC;
		HWND				m_hWnd;
		PGLBLENDEQUATION	glBlendEquation;
	  #endif
      //## end DC_COpenGL%38EB1E6B0157.implementation

};

//## begin DC_COpenGL%38EB1E6B0157.postscript preserve=yes
extern DC_COpenGL* gOGLDC;
//## end DC_COpenGL%38EB1E6B0157.postscript

// Class DC_COpenGL 

//## begin module%38EB1E6B0157.epilog preserve=yes
//## end module%38EB1E6B0157.epilog


#endif
