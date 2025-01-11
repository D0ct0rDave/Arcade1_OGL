#include <stdlib.h>
#include <string.h>

#include "Menu.h"
#include "graphics/graphics.h"
#include "graphics/gfont.h"
#include "Input/Input.h"


#include "Game/game.h"

// -----------------------------------------------------------------------------
unsigned int guiMenuState		= MENUSTATE_NONE;
unsigned int guiMenuTime		= 0 ;
unsigned int guiMenuReturnState;

// -----------------------------------------------------------------------------
// Function prototypes
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// String table
// -----------------------------------------------------------------------------
#define STRID_START				"COMENZAR"
#define STRID_SCORESTITLE		"SCORES"
#define STRID_SCORESH1			"NAME"
#define STRID_SCORESH2			"MAP"
#define STRID_SCORESH3			"SECTION"
#define STRID_SCORESH4			"SCORE"

// -----------------------------------------------------------------------------
// Scores
// -----------------------------------------------------------------------------
typedef struct{
	unsigned char ucName[4];
	unsigned int uiScore;
	unsigned int uiSection;
	unsigned int uiMap;
}TScoreEntry;

TScoreEntry oScoreList[10] = 
{
	{{'D','M','C',0},100000,1,1},

	{{'C','H','B',0}, 50000,1,1},
	{{'L','S','M',0}, 10000,1,1},
	{{'A','D','I',0},  5000,1,1},
	{{'J','K','W',0},  1000,1,1},
	{{'P','S','C',0},   500,1,1},
	{{'P','P','T',0},   100,1,1},
	{{'T','N','T',0},    50,1,1},
	{{'A','D','N',0},    10,1,1},
	{{'D','M','A',0},	    5,1,1},
};

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
#define MAX_MENU_ELEMS	4

char *szMenuBmp[MAX_MENU_ELEMS] = 
{
	"data/MENU/menufont",
	"data/MENU/introfont",
	"data/MENU/title",
	"data/MENU/subtitle",
};

BMPHandle MenuBMP[4];

int iX1Ofs;
int iY1Ofs;
int iX2Ofs;
int iY2Ofs;

// -----------------------------------------------------------------------------
void MENU_Init()
{
	int i;

	// Set resource filenames
	GSetResourceTable(szMenuBmp);

	// Load bitmaps
	for (i=0;i<MAX_MENU_ELEMS;i++)
		MenuBMP[i] = GLoadBitmap(0,0,i);

	FONT_SetFont(0,MenuBMP[0],16,16,1);
	FONT_SetFont(1,MenuBMP[1],16,16,1);
}
// -----------------------------------------------------------------------------
void MENU_Finish()
{	
}

// -----------------------------------------------------------------------------
// RENDER TITLE PAGE
// -----------------------------------------------------------------------------
void MENU_Render_Title_Page()
{
	static int iX = 450;
	static int iY = 220;
	
	if (oInput.uiJustPressed & CMD_UP) iY -= 2;
	if (oInput.uiJustPressed & CMD_DOWN) iY += 2;
	if (oInput.uiJustPressed & CMD_LEFT) iX -= 2;
	if (oInput.uiJustPressed & CMD_RIGHT) iX += 2;


	// Menu render
	GClear();

		// Render Title
		GPutSprite(MenuBMP[2],0,0,BMPWIDTH,BMPHEIGHT,0,50,BLENDCOPY);

		// Render subtitle
		if (guiMenuTime % 5 == 0)
		{
			iX1Ofs = (rand() % 10) - 5;
			iY1Ofs = (rand() % 10) - 5;
			iX2Ofs = (rand() % 10) - 5;
			iY2Ofs = (rand() % 10) - 5;
		}

		GPutSprite(MenuBMP[3],0,0,BMPWIDTH,BMPHEIGHT,450 + iX1Ofs,216 + iY1Ofs,BLENDALPHA);
		GPutSprite(MenuBMP[3],0,0,BMPWIDTH,BMPHEIGHT,450 + iX2Ofs,216 + iY2Ofs,BLENDALPHA);


		// Render start line
		if (guiMenuTime/SCREENFPS & 0x01)
		{
			// Setup offsets
			int iXOfs,iYOfs;
			iXOfs    = (SCREENWIDTH - FONT_iStringWidth(0,STRID_START)) >> 1;
			iYOfs    = 325; // (SCREENHEIGHT - FONT_iCharHeight()) >> 1;

				
			FONT_WriteString(0,iXOfs,iYOfs,STRID_START);
		}

	GBlit();
}

// -----------------------------------------------------------------------------
// RENDER SCORES PAGE
// -----------------------------------------------------------------------------
void MENU_Render_Scores_Page()
{
	int iNumScores,i;
	

	// Menu render
	GClear();
		
		
		// Render title
		#define XOFS	50
		#define YOFS	20
		#define XOFS2	50
		#define YOFS2	50
		#define XOFS3	50
		#define YOFS3	80
		#define XSEP	150
		#define YSEP	28

		if (guiMenuTime % 5 == 0)
		{
			iX1Ofs = (rand() % 10) - 5;
			iY1Ofs = (rand() % 10) - 5;
			iX2Ofs = (rand() % 10) - 5;
			iY2Ofs = (rand() % 10) - 5;
		}

		// Page title
		FONT_WriteString(0,XOFS+iX1Ofs,YOFS+iY1Ofs,STRID_SCORESTITLE);
		FONT_WriteString(0,XOFS+iX2Ofs,YOFS+iY2Ofs,STRID_SCORESTITLE);
				

		// List Header
		FONT_WriteString(0,XOFS+0*XSEP,YOFS2,STRID_SCORESH1);
		FONT_WriteString(0,XOFS+1*XSEP,YOFS2,STRID_SCORESH2);
		FONT_WriteString(0,XOFS+2*XSEP,YOFS2,STRID_SCORESH3);
		FONT_WriteString(0,XOFS+3*XSEP,YOFS2,STRID_SCORESH4);
		
		iNumScores = (guiMenuTime / (SCREENFPS>>1)) + 1;
		if (iNumScores > 10) iNumScores = 10;

		for (i=0;i<iNumScores;i++)
		{
			FONT_WriteString (1,XOFS+0*XSEP,YOFS3+(i+1)*YSEP,(char*)oScoreList[i].ucName);
			FONT_WriteInteger(1,XOFS+1*XSEP,YOFS3+(i+1)*YSEP,oScoreList[i].uiMap);
			FONT_WriteInteger(1,XOFS+2*XSEP,YOFS3+(i+1)*YSEP,oScoreList[i].uiSection);
			FONT_WriteInteger(1,XOFS+3*XSEP+5*16,YOFS3+(i+1)*YSEP,oScoreList[i].uiScore);
		}


	GBlit();
}
// -----------------------------------------------------------------------------
int MENU_iLoop()
{
	switch (guiMenuState)
	{
		// ----------------------------------------------------------------------
		case MENUSTATE_NONE:
		{
			guiMenuState = MENUSTATE_INIT;
		}
		break;

		// ----------------------------------------------------------------------
		case MENUSTATE_INIT:
		{
			MENU_Init();

			guiMenuState = MENUSTATE_TITLE;
			guiMenuTime  = 0;
		}
		break;
	
		// ----------------------------------------------------------------------		
		case MENUSTATE_TITLE:
		case MENUSTATE_TITLE2:
		{
			INPUT_Update();
			guiMenuTime++;

			MENU_Render_Title_Page();


			// Show Scores Page
			if ((guiMenuTime == 10*SCREENFPS) && (guiMenuState == MENUSTATE_TITLE))
			{
				guiMenuTime  = 0;
				guiMenuState = MENUSTATE_SCORES;
			}
			
			// Start demo
	   else if (
				((oInput.uiPressed     & CMD_BBUTT) && (oInput.uiJustPressed & CMD_DBUTT)) ||
				((oInput.uiJustPressed & CMD_BBUTT) && (oInput.uiPressed     & CMD_DBUTT)) ||
				((guiMenuTime == 10*SCREENFPS) && (guiMenuState == MENUSTATE_TITLE2))
				)
			{
				guiMenuTime			= 0;
				guiMenuState		= MENUSTATE_FINISH;
				guiMenuReturnState	= MENUSTATE_PLAYINGDEMO;
			}
			
			// Start game
	   else if (oInput.uiJustPressed & CMD_ABUTT)
			{
				
				guiMenuTime			= 0;
				guiMenuState		= MENUSTATE_FINISH;
				guiMenuReturnState	= MENUSTATE_PLAYINGGAME;
			}

			// Record demo	   		
	   else if ((oInput.uiPressed     & CMD_CBUTT) && (oInput.uiJustPressed & CMD_DBUTT)||
				(oInput.uiJustPressed & CMD_CBUTT) && (oInput.uiPressed     & CMD_DBUTT))
			{
				guiMenuTime			= 0;
				guiMenuState		= MENUSTATE_FINISH;
				guiMenuReturnState	= MENUSTATE_RECORDDEMO;
			}
		}
		break;
		
		// ----------------------------------------------------------------------
		case MENUSTATE_SCORES:
		{
			INPUT_Update();
			guiMenuTime++;

			MENU_Render_Scores_Page();
			if ((guiMenuTime == 10*SCREENFPS) || (oInput.uiJustPressed & (CMD_ABUTT | CMD_BBUTT | CMD_CBUTT | CMD_DBUTT)) )
			{
				guiMenuTime  = 0;
				guiMenuState = MENUSTATE_TITLE2;
			}
		}
		break;
		
		// ----------------------------------------------------------------------
		case MENUSTATE_PLAYINGGAME:
		break;

		// ----------------------------------------------------------------------
		case MENUSTATE_PLAYINGDEMO:
		break;

		// ----------------------------------------------------------------------
		case MENUSTATE_FINISH:
		{			
			MENU_Finish();
			
			guiMenuState = MENUSTATE_NONE;
			guiMenuTime  = 0;

			return(guiMenuReturnState);
		}
		break;
	}
	
	return(0);
}
// -----------------------------------------------------------------------------