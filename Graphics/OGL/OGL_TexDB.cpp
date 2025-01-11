//---------------------------------------------------------------------------
#include "OGL_TexDB.h"

#ifdef _DEBUG
static GLenum eErr = GL_NO_ERROR;
#define CHECKERROR() eErr=glGetError();
#else
#define CHECKERROR()
#endif

//---------------------------------------------------------------------------
COGL_TexDB::COGL_TexDB()
{
	TexData = NULL;
    TexHnd = NULL;    
    MaxTEXs = 0;
    NumTEXs = 0;
}
//---------------------------------------------------------------------------
COGL_TexDB::~COGL_TexDB()
{
	Finish();
}
//---------------------------------------------------------------------------
void COGL_TexDB::Init(unsigned int _iMaxTEXs)
{
	MaxTEXs = _iMaxTEXs;

    // allocate space for buffers
	TexHnd  = (GLuint*)malloc(MaxTEXs*sizeof(GLuint));
	TexData = (Texture**)malloc(MaxTEXs*sizeof(Texture*));

    // Clear buffers
    for (int i=0;i<MaxTEXs;i++)
    {
		TexHnd[i]= -1;
		TexData[i] = NULL;
	}

	NumTEXs = 0;
}
//---------------------------------------------------------------------------
int COGL_TexDB::iAddTexture(Texture* _poTex)
{	
	if (NumTEXs == MaxTEXs) return(-1);	
    TexData[NumTEXs] = _poTex;

    // Generate a new texture handler
    glGenTextures(1,&TexHnd[NumTEXs]);
	CHECKERROR();
	
    // Establecer el contexto de la nueva textura
    glBindTexture(GL_TEXTURE_2D,TexHnd[NumTEXs]);
	CHECKERROR();

    glTexImage2D (GL_TEXTURE_2D,
                  0,
                  GL_RGBA,
                  TexData[NumTEXs]->TX,
                  TexData[NumTEXs]->TY,
                  0,
                  GL_RGBA,
                  GL_UNSIGNED_BYTE,
                  TexData[NumTEXs]->data);
	CHECKERROR();

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT    );CHECKERROR();
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT    );CHECKERROR();
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);CHECKERROR();
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);CHECKERROR();

	NumTEXs++;

	return( NumTEXs-1 );
}
//---------------------------------------------------------------------------
GLuint COGL_TexDB::GetTexture(int _iTex)
{
	if (_iTex >= MaxTEXs) return(-1);	
	return(TexHnd[_iTex]);
}

Texture* COGL_TexDB::pGetTexture(int _iTex)
{
	if (_iTex >= MaxTEXs) return(NULL);
	return(TexData[_iTex]);
}
//---------------------------------------------------------------------------
void COGL_TexDB::Finish()
{
    unsigned int cTex;
    for (cTex=0;cTex<NumTEXs;cTex++)
	{
		glDeleteTextures(1,(GLuint*)&TexHnd[cTex]);
	}

	free(TexHnd);
    free(TexData);

	TexData = NULL;
    TexHnd = NULL;    
    MaxTEXs = 0;
    NumTEXs = 0;
}
//---------------------------------------------------------------------------

