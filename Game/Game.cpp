
#include <stdlib.h>
#include <string.h>

#include "Game.h"

#include "graphics/graphics.h"
#include "graphics/gmap.h"
#include "graphics/gcam.h"

// #include "Entities/entities.h"
#include "Levels/Level.h"
#include "Levels/Level0/Level0.h"
#include "Levels/Entities/Explosion.h"
#include "Levels/Entities/Player.h"

#include "Input/Input.h"
#include "Game/HUD/HUD.h"

static unsigned int guiGameState;
static unsigned int guiGameTime	= 0;
static unsigned int guiCurSectSeed = 0;
// -----------------------------------------------------------------------------
// String table
// -----------------------------------------------------------------------------
#define STRID_READY "PREPARADO"

extern int	giDisableAI;
static int	giLastSection = -1;
// -----------------------------------------------------------------------------
void GAME_Init()
{

}
// -----------------------------------------------------------------------------
void GAME_Finish()
{
	// Put any code here for handling things that need to be done before exiting,
	// like saving records into a database, or saving preferences etc.

	/// HUD_Finish();
	
	// Remove the graphics from memory, since they are no longer needed
}

// -----------------------------------------------------------------------------
int GAME_iLoop()
{
	switch (guiGameState)
	{
		// ----------------------------------------------------------------------
		case GAMESTATE_NONE:
		{
			guiGameState = GAMESTATE_INIT;
		}
		break;

		// ----------------------------------------------------------------------
		case GAMESTATE_INIT:
		{
			Level0_Init();			// switch (guiLevel)
			HUD_Init();

			giPlScore	 = 0;

			guiCurSectSeed = rand();
			
			#ifndef _DEBUG
			guiGameState = GAMESTATE_READYMESSAGE;
			#else
			guiGameState = GAMESTATE_READY;
			#endif

			guiGameTime  = 0;
			MAP_SetCurrentSection(0);			
		}
		break;
	
		// ----------------------------------------------------------------------
		case GAMESTATE_READYMESSAGE:
		{
			guiGameTime++;

			if (guiGameTime < 2*SCREENFPS)
			{
				int iLetters,iXOfs,iYOfs;
				char szStr[16] = { 0 };

				// Compute number of letters to copy
				iLetters = guiGameTime / (SCREENFPS/20);
				if (iLetters > strlen(STRID_READY)) iLetters = strlen(STRID_READY);
				
				// Get string
				strncpy(szStr,STRID_READY,iLetters);
				
				// Setup offsets
				iXOfs    = (SCREENWIDTH - strlen(STRID_READY)*NORMALFONT_SIZE)>> 1;
				iYOfs    = (SCREENHEIGHT - NORMALFONT_SIZE) >> 1;


				GClear();
					
					if (guiGameTime < 90)
						HUD_DrawString (iXOfs,iYOfs,3,szStr);

				GBlit();
			}
			else
			{
				guiGameTime  = 0;
				guiGameState = GAMESTATE_READY;
			}
		}
		break;
		
		// ----------------------------------------------------------------------
		case GAMESTATE_READY:
		{
			int iSect = MAP_iGetSection();
			MAP_SetCurrentSection(iSect);

			ENT_Reset();
			
			// Initialize persistent data
			LevelInitPersistent();

			// Init player variables
			gpoPlayerEnt->iEnergy = 100;

			gpoPlayerEnt->oPos.iX = 32;
			gpoPlayerEnt->oPos.iY = SCREENHHEIGHT;

			guiGameTime    = 0;
			guiGameState   = GAMESTATE_RUN;
		}
		break;	
		// ----------------------------------------------------------------------
		case GAMESTATE_RUN:
		{
			INPUT_Update();
			
			if (giCurSec != giLastSection)
			{
				guiCurSectSeed = rand();
				srand(guiCurSectSeed);
				giLastSection  = giCurSec;
			}

			// DEBUG CODE
			if ((oInput.uiPressed     & CMD_CBUTT) && (oInput.uiJustPressed & CMD_DBUTT)||
				(oInput.uiJustPressed & CMD_CBUTT) && (oInput.uiPressed     & CMD_DBUTT))
				giStopMapScroll = ! giStopMapScroll;

			// DEBUG CODE
			if ((oInput.uiPressed     & CMD_ABUTT) && (oInput.uiJustPressed & CMD_DBUTT)||
				(oInput.uiJustPressed & CMD_ABUTT) && (oInput.uiPressed     & CMD_DBUTT))
				giDisableAI	= ! giDisableAI;
			
			// DEBUG CODE
			if ((oInput.uiPressed & CMD_ABUTT) && (oInput.uiPressed & CMD_BBUTT))
			{
				int i;
				giDisableAI = 1;
				
				for (i=0;i<9;i++)
					LevelUpdate();
			}
			
			LevelUpdate();

			// The game handles separatelly the player collisions
			ENT_PlayerHandleCollisions();

			// Render	
			GClear();

				LevelRender();
				HUD_Draw();

 			GBlit();

			if (gpoPlayerEnt->iEnergy == 0)
			{
				ENT_AddExplosion(gpoPlayerEnt->oPos.iX,gpoPlayerEnt->oPos.iY,ENT_BIG_EXPLOSION);
			
				guiGameTime  = 0;
				guiGameState = GAMESTATE_DEAD;
				ENT_DeleteEntity(gpoPlayerEnt);
			}
		}
		break;
		
		// ----------------------------------------------------------------------
		case GAMESTATE_DEAD:
		{	
			guiGameTime++;			
			
			if (guiGameTime < 2*SCREENFPS)
			{
				LevelUpdate();

				// Render	
				GClear();

					LevelRender();
					HUD_Draw();
	
 				GBlit();
			}
			else
			{
				guiGameTime  = 0;
				guiGameState = GAMESTATE_FINISH;	// GAMESTATE_READYMESSAGE;
			}
		}
		break;

		// ----------------------------------------------------------------------
		case GAMESTATE_FINISH:
		{
			GAME_Finish();

			guiGameState = GAMESTATE_NONE;
			guiGameTime  = 0;
			
			return(GAMESTATE_GAMEOVER);
		}
		break;
		
	}

	return(0);
}
// -----------------------------------------------------------------------------