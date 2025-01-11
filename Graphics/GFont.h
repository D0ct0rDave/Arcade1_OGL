// -----------------------------------------------------------------------------
#ifndef GFontH
#define GFontH


// -----------------------------------------------------------------------------
#include "graphics.h"

void FONT_SetFont(int _iFontID,BMPHandle _BMP,int _iTX,int _iTY,int _iTransparent);
void FONT_WriteInteger(int _iFontID,int _iX,int _iY,int _iValue);
void FONT_WriteString (int _iFontID,int _iX,int _iY,char *_szString);

int  FONT_iStringWidth(int _iFontID,char *_szString);
int  FONT_iCharHeight (int _iFontID);
int  FONT_iCharWidth  (int _iFontID);

// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
