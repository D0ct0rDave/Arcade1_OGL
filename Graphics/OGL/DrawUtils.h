#ifndef DrawUtilsH
#define DrawUtilsH

#include "antares.h"

// ------------------------------------------------------------------------
// USE_NORMALIZATION: Deseamos obtener coordenadas normalizadas en el rango (-1,1)
// con independencia de las dimensiones del modo y resolución seleccionada
// de la pantalla. Para ello hemos de definir una resolución estándar como 
// marco de trabajo. Esta es 640x480.
// ------------------------------------------------------------------------
#define STD_SCR_WIDTH  640
#define STD_SCR_HEIGHT 480

void RenderQUAD(CDMCTex *_poTex,		  
				float _fIX,float _fIY,float _fFX,float _fFY,
				float _fIU,float _fIV,float _fFU,float _fFV,
				unsigned int _uiColor,float _fA);

void SetViewport (int _iX, int _iY, int _iWidth, int _iHeight);

#endif