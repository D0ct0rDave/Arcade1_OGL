//## begin module%3920FAAA0167.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3920FAAA0167.cm

//## begin module%3920FAAA0167.cp preserve=no
//## end module%3920FAAA0167.cp

//## Module: OGL_TextureDesc%3920FAAA0167; Pseudo Package specification
//## Source file: e:\ProRally\Realization\ENGINE3D\DC\OGL_TextureDesc.h

#ifndef OGL_TextureDesc_h
#define OGL_TextureDesc_h 1

//## begin module%3920FAAA0167.additionalIncludes preserve=no
//## end module%3920FAAA0167.additionalIncludes

//## begin module%3920FAAA0167.includes preserve=yes
//## end module%3920FAAA0167.includes

// E3D_TextureDesc
#include "../E3D_TextureDesc.h"
//## begin module%3920FAAA0167.additionalDeclarations preserve=yes
//## end module%3920FAAA0167.additionalDeclarations


//## begin OGL_TextureDesc%3920FAAA0167.preface preserve=yes
//## end OGL_TextureDesc%3920FAAA0167.preface

//## Class: OGL_TextureDesc%3920FAAA0167
//## Category: Realization::ENGINE3D::DC%38ED8CDF0212
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class OGL_TextureDesc : public E3D_TextureDesc  //## Inherits: <unnamed>%3920FAC20177
{
  //## begin OGL_TextureDesc%3920FAAA0167.initialDeclarations preserve=yes
  //## end OGL_TextureDesc%3920FAAA0167.initialDeclarations

  public:
    //## Constructors (generated)
      OGL_TextureDesc();

    //## Destructor (generated)
      ~OGL_TextureDesc();


    //## Other Operations (specified)
      //## Operation: Invalidate%958472402
      //	Invalidate() informs the texture that the source data in
      //	main memory has been altered.
      virtual void Invalidate ();

      //## Operation: bIsValid%958472403
      //	Tells if the contents of texture memory and system
      //	memory match.
      virtual int bIsValid ();

      //## Operation: iGetWidth%958472404
      virtual int iGetWidth ();

      //## Operation: iGetHeight%958472405
      virtual int iGetHeight ();

      //## Operation: eGetPixelFormat%958472406
      virtual E3D_EPixelFormat eGetPixelFormat ();
      virtual SetPixelFormat(E3D_EPixelFormat _ePixelFormat) {m_ePixelFmt = _ePixelFormat;};
      

      //## Operation: pucGetImage%958472407
      virtual unsigned char* pucGetImage ();

	  virtual void SetImage (unsigned char *_pucImage);


      //## Operation: InvalidateRect%958472409
      //	InvalidateRect() informs the texture that part of the
      //	source data in main memory has been altered. The region
      //	defined by InvalidateRect maybe used to optimize the
      //	transfer to texture memory.
      virtual void InvalidateRect (int _iX, int _iY, int _iWidth, int _iHeight);

      //## Operation: bTexImage2D%958472411
      int bTexImage2D (E3D_EPixelFormat _ePixelFmt, int _iWidth, int _iHeight, unsigned char *_pucImage, int _iNumLODs);

      //## Operation: bIsFree%958472412
      virtual int bIsFree ();

      //## Operation: DestroyTexture%958472413
      virtual void DestroyTexture ();

      //## Operation: BindTexture%958472414
      virtual void BindTexture ();

      //## Operation: SetTWrapMode%975424981
      virtual void SetTWrapMode (E3D_EWrapMode _eWrapMode);

      //## Operation: SetSWrapMode%975424982
      virtual void SetSWrapMode (E3D_EWrapMode _eWrapMode);

      //## Operation: SetMagFilter%975424983
      virtual void SetMagFilter (E3D_EMagFilter _eMagFilter);

      //## Operation: SetMinFilter%975424984
      virtual void SetMinFilter (E3D_EMinFilter _eMinFilter);

      //## Operation: eGetTWrapMode%975602555
      virtual E3D_EWrapMode eGetTWrapMode ();

      //## Operation: eGetSWrapMode%975602556
      virtual E3D_EWrapMode eGetSWrapMode ();

      //## Operation: eGetMagFilter%975602557
      virtual E3D_EMagFilter eGetMagFilter ();

      //## Operation: eGetMinFilter%975602558
      virtual E3D_EMinFilter eGetMinFilter ();

      //## Operation: UploadImageTo%975667577
      //	Transfers the texture memory contents to a given system
      //	memory buffer.
      virtual void UploadImageTo (unsigned char* _poBuffer, unsigned long _ulBufferLen);

      //## Operation: EnableAlphaTesting%994406999
      virtual void EnableAlphaTesting ();

      //## Operation: DisableAlphaTesting%994407000
      virtual void DisableAlphaTesting ();

      //## Operation: bIsEnableAlphaTesting%994407002
      virtual unsigned char bIsEnableAlphaTesting ();

      //## Operation: EnableOrder%995276432
      virtual void EnableOrder ();

      //## Operation: DisableOrder%995276433
      virtual void DisableOrder ();

      //## Operation: bIsEnableOrder%995276434
      virtual unsigned char bIsEnableOrder ();

      //## Operation: fGetAlphaTestingValue%995276439
      virtual float fGetAlphaTestingValue ();

      //## Operation: SetAlphaTestingValue%995276440
      virtual void SetAlphaTestingValue (float _fValue);

      //## Operation: fGetAlphaTestingFunc%995276445
      virtual E3D_EAlphaFunc fGetAlphaTestingFunc ();

      //## Operation: fSetAlphaTestingFunc%995276446
      virtual void fSetAlphaTestingFunc (E3D_EAlphaFunc _eAlphaFunc);

    // Data Members for Class Attributes

      //## Attribute: m_iTextureName%3921240A003E
      //## begin OGL_TextureDesc::m_iTextureName%3921240A003E.attr preserve=no  private: GLuint {U} 
      unsigned int m_iTextureName;
      //## end OGL_TextureDesc::m_iTextureName%3921240A003E.attr

      //## Attribute: m_pucImage%39212695008C
      //## begin OGL_TextureDesc::m_pucImage%39212695008C.attr preserve=no  private: unsigned char* {U} NULL
      unsigned char* m_pucImage;
      //## end OGL_TextureDesc::m_pucImage%39212695008C.attr

      //## Attribute: m_iWidth%3921273A0290
      //## begin OGL_TextureDesc::m_iWidth%3921273A0290.attr preserve=no  private: int {U} 
      int m_iWidth;
      //## end OGL_TextureDesc::m_iWidth%3921273A0290.attr

      //## Attribute: m_iHeight%392127470000
      //## begin OGL_TextureDesc::m_iHeight%392127470000.attr preserve=no  private: int {U} 
      int m_iHeight;
      //## end OGL_TextureDesc::m_iHeight%392127470000.attr

      //## Attribute: m_bIsValid%392127FC0128
      //## begin OGL_TextureDesc::m_bIsValid%392127FC0128.attr preserve=no  private: unsigned char {U} 
      unsigned char m_bIsValid;
      //## end OGL_TextureDesc::m_bIsValid%392127FC0128.attr

      //## Attribute: m_ePixelFmt%39212814034B
      //## begin OGL_TextureDesc::m_ePixelFmt%39212814034B.attr preserve=no  private: E3D_EPixelFormat {U} 
      E3D_EPixelFormat m_ePixelFmt;
      //## end OGL_TextureDesc::m_ePixelFmt%39212814034B.attr

      //## Attribute: m_bIsFree%3921292F0148
      //## begin OGL_TextureDesc::m_bIsFree%3921292F0148.attr preserve=no  private: unsigned char {U} 
      unsigned char m_bIsFree;
      //## end OGL_TextureDesc::m_bIsFree%3921292F0148.attr

      //## Attribute: m_iTexWrapS%3A23CF380148
      //## begin OGL_TextureDesc::m_iTexWrapS%3A23CF380148.attr preserve=no  private: GLint {U} GL_CLAMP
      int m_iTexWrapS;
      //## end OGL_TextureDesc::m_iTexWrapS%3A23CF380148.attr

      //## Attribute: m_iTexWrapT%3A23CF600213
      //## begin OGL_TextureDesc::m_iTexWrapT%3A23CF600213.attr preserve=no  private: GLint {U} GL_CLAMP
      int m_iTexWrapT;
      //## end OGL_TextureDesc::m_iTexWrapT%3A23CF600213.attr

      //## Attribute: m_iMagFilter%3A23CF670213
      //## begin OGL_TextureDesc::m_iMagFilter%3A23CF670213.attr preserve=no  private: GLint {U} GL_LINEAR
      int m_iMagFilter;
      //## end OGL_TextureDesc::m_iMagFilter%3A23CF670213.attr

      //## Attribute: m_iMinFilter%3A23CF6C01E4
      //## begin OGL_TextureDesc::m_iMinFilter%3A23CF6C01E4.attr preserve=no  private: GLint {U} GL_LINEAR
      int m_iMinFilter;
      //## end OGL_TextureDesc::m_iMinFilter%3A23CF6C01E4.attr

      //## Attribute: m_ucEnableAlphaTesting%3B4575AC02AF
      //## begin OGL_TextureDesc::m_ucEnableAlphaTesting%3B4575AC02AF.attr preserve=no  private: unsigned char {U} 
      unsigned char m_ucEnableAlphaTesting;
      //## end OGL_TextureDesc::m_ucEnableAlphaTesting%3B4575AC02AF.attr

      //## Attribute: m_ucEnableOrder%3B52B7BD0177
      //## begin OGL_TextureDesc::m_ucEnableOrder%3B52B7BD0177.attr preserve=no  private: unsigned char {U} 
      unsigned char m_ucEnableOrder;
      //## end OGL_TextureDesc::m_ucEnableOrder%3B52B7BD0177.attr

      //## Attribute: m_fAlphaTestingValue%3B52BC2D02CE
      //## begin OGL_TextureDesc::m_fAlphaTestingValue%3B52BC2D02CE.attr preserve=no  private: float {U} 
      float m_fAlphaTestingValue;
      //## end OGL_TextureDesc::m_fAlphaTestingValue%3B52BC2D02CE.attr

      //## Attribute: m_eAlphaFunc%3B52BFCB02CE
      //## begin OGL_TextureDesc::m_eAlphaFunc%3B52BFCB02CE.attr preserve=no  private: E3D_EAlphaFunc {U} 
      E3D_EAlphaFunc m_eAlphaFunc;
      //## end OGL_TextureDesc::m_eAlphaFunc%3B52BFCB02CE.attr

    // Additional Public Declarations
      //## begin OGL_TextureDesc%3920FAAA0167.public preserve=yes
		int m_iGroupId; // Default 0
		virtual void SetTextureGroup(int _iGroup);
      //## end OGL_TextureDesc%3920FAAA0167.public

//  protected:
    // Additional Protected Declarations
      //## begin OGL_TextureDesc%3920FAAA0167.protected preserve=yes
      //## end OGL_TextureDesc%3920FAAA0167.protected

  private:
    // Additional Private Declarations
      //## begin OGL_TextureDesc%3920FAAA0167.private preserve=yes
      //## end OGL_TextureDesc%3920FAAA0167.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin OGL_TextureDesc%3920FAAA0167.implementation preserve=yes
      //## end OGL_TextureDesc%3920FAAA0167.implementation

};

//## begin OGL_TextureDesc%3920FAAA0167.postscript preserve=yes
//## end OGL_TextureDesc%3920FAAA0167.postscript

// Class OGL_TextureDesc 

//## begin module%3920FAAA0167.epilog preserve=yes
//## end module%3920FAAA0167.epilog


#endif
