
// -----------------------------------------------------------------------------
#include "HUD.h"
#include "Game/Game.h"
#include "Levels/Entities/Player.h"
#include "graphics/Graphics.h"

// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Variables
// -----------------------------------------------------------------------------
#define MAX_HUD_ELEMS	8

char *szHUDBmp[MAX_HUD_ELEMS] = 
{
	"data/HUD/HUD_FRAME",
	"data/HUD/HUD_ENERGY",
	"data/HUD/HUD_SCOREFONT",
	"data/HUD/HUD_FONT",
	"data/HUD/HUD_BLAST",
	"data/HUD/HUD_HBLAST",
	"data/HUD/HUD_LASER",
	"data/HUD/HUD_BFG",
};


BMPHandle HUDBMP[MAX_HUD_ELEMS];

// -----------------------------------------------------------------------------
void HUD_Init()
{
	int i;

	// Set resource filenames
	GSetResourceTable(szHUDBmp);

	// Load bitmaps
	for (i=0;i<MAX_HUD_ELEMS;i++)
		HUDBMP[i] = GLoadBitmap(0,0,i);
}
// -----------------------------------------------------------------------------
void HUD_Finish()
{	
}
// -----------------------------------------------------------------------------
void HUD_DrawInteger(int _iX,int _iY,int _iSprID,int _iValue)
{
	int iXOfs  = 0;
	int iValue = _iValue;
	int iDigit;

	do{
		iDigit  = iValue % 10;

		GPutSprite(HUDBMP[_iSprID],(iDigit*16),0,16,16,_iX - iXOfs,_iY,BLENDCOPY);
		
		iXOfs  += 16;
		iValue /= 10;
		
	}while (iValue);
}
// -----------------------------------------------------------------------------
void HUD_DrawString (int _iX,int _iY,int _iSprID,char *_szStr)
{
	int iXOfs  = 0;
	int iRow,iCol;
	char a;

	#define FIRSTCHAR	(' ')
	#define LASTCHAR	('_')
	
	while (*_szStr)
	{
		a = *_szStr;

		if ((a>=FIRSTCHAR) && (a<=LASTCHAR))
		{
			a -= FIRSTCHAR;
			iRow = a >> 4;
			iCol = a & 0x0f;

			GPutSprite(HUDBMP[_iSprID],iCol*16,iRow*16,16,16,_iX + iXOfs,_iY,BLENDCOPY);
		}
		
		iXOfs += 16;
		_szStr++;
	}	
}
// -----------------------------------------------------------------------------
void HUD_Draw()
{
	// Draw HUD frame
	GPutSprite(HUDBMP[0],0,0,302,65,(SCREENWIDTH-302)>>1,448-65-8,BLENDALPHA);

	// Draw current weapon
	GPutSprite(HUDBMP[4 + giPlWeap], 0, 0, 64, 64, 288, 376, BLENDCOPY);

	// Draw energy
	GPutSprite(HUDBMP[1],0,0,100*((float)gpoPlayerEnt->iEnergy/(float)100),25,178,394,BLENDALPHA);

	// Draw Credits
	HUD_DrawInteger(443,393,2,giPlCredit);

	// Draw Score
	HUD_DrawInteger(443,406,2,giPlScore);

	// Draw section
	HUD_DrawInteger(32,32,2,giCurSec);
	
	// Draw time
	HUD_DrawInteger(80,32,2,giCurSecFrame/60);

}
// -----------------------------------------------------------------------------

