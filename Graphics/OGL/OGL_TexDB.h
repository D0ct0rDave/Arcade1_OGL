//---------------------------------------------------------------------------
#ifndef OGL_TexDBH
#define OGL_TexDBH
//---------------------------------------------------------------------------

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdlib.h>

typedef struct
{
    unsigned char* data;
    unsigned int TX;
    unsigned int TY;
    unsigned int channels;
}Texture;
//---------------------------------------------------------------------------
class COGL_TexDB
{
	public:
	    	 COGL_TexDB();
            ~COGL_TexDB();

            void Init(unsigned int _iMaxTEXs);
            int	 iAddTexture(Texture* _poTex);
            GLuint GetTexture(int _iTex);
            Texture* pGetTexture(int _iTex);

	    	void Finish();
    private:
			

//			void InvalidateTexture (unsigned int _uiTex);
//            void SetTextureWrapMode(unsigned int _uiTex,int _iWrapMode);


//            void EraseTextureHandler(unsigned int _uiTex);
//            void EraseTextures();

            unsigned int	MaxTEXs;
			unsigned int	NumTEXs;
            Texture**	    TexData;
            GLuint*			TexHnd;
};
//---------------------------------------------------------------------------
#endif
