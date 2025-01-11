// -----------------------------------------------------------------------------
#include "GFont.h"
#include "graphics.h"

#include <string.h>
// -----------------------------------------------------------------------------
#define MAX_FONTS 8

typedef struct{
	
	BMPHandle	BMP;
	int			iTX;
	int			iTY;
	int			iTransparent;
	
}TFont;

TFont	oFontList[MAX_FONTS];
// -----------------------------------------------------------------------------
void FONT_SetFont(int _iFontID,BMPHandle _BMP,int _iTX,int _iTY,int _iTransparent)
{
	if (_iFontID >= MAX_FONTS) return;

	oFontList[_iFontID].BMP = _BMP;
	oFontList[_iFontID].iTX = _iTX;
	oFontList[_iFontID].iTY = _iTY;
	oFontList[_iFontID].iTransparent = _iTransparent;
}
// -----------------------------------------------------------------------------
void FONT_WriteChar(int _iFontID,int _iX,int _iY,char a)
{
	int iRow,iCol;


	#define FIRSTCHAR	(' ')
	#define LASTCHAR	('_')


	if (! ((a>=FIRSTCHAR) && (a<=LASTCHAR)) ) return;

	a -= FIRSTCHAR;

	iRow = a >> 4;
	iCol = a & 0x0f;

	GPutSprite(	oFontList[_iFontID].BMP,
						iCol*oFontList[_iFontID].iTX,
						iRow*oFontList[_iFontID].iTY,
						oFontList[_iFontID].iTX,
						oFontList[_iFontID].iTY,
						_iX,
						_iY,
						oFontList[_iFontID].iTransparent);
}
// -----------------------------------------------------------------------------
void FONT_WriteInteger(int _iFontID,int _iX,int _iY,int _iValue)
{	
	int iXOfs  = 0;
	int iValue = _iValue;
	int iDigit;

	if (_iFontID >= MAX_FONTS) return;

	do{
		iDigit  = iValue % 10;
		
		FONT_WriteChar(_iFontID,_iX - iXOfs,_iY, iDigit + '0');

		iXOfs  += 16;
		iValue /= 10;

	}while (iValue);

}
// -----------------------------------------------------------------------------
void FONT_WriteString (int _iFontID,int _iX,int _iY,char *_szString)
{
	int iXOfs  = 0;
	char a;

	#define FIRSTCHAR	(' ')
	#define LASTCHAR	('_')
	
	while (*_szString)
	{
		a = *_szString;
		FONT_WriteChar(_iFontID,_iX + iXOfs,_iY,a);
	
		iXOfs += 16;
		_szString++;
	}
}
// -----------------------------------------------------------------------------
int FONT_iStringWidth (int _iFontID,char *_szString)
{
	return(strlen(_szString)*oFontList[_iFontID].iTX);
}
// -----------------------------------------------------------------------------
int  FONT_iCharHeight (int _iFontID)
{
	return(oFontList[_iFontID].iTY);
}
// -----------------------------------------------------------------------------
int  FONT_iCharWidth  (int _iFontID)
{
	return(oFontList[_iFontID].iTX);
}
// -----------------------------------------------------------------------------
