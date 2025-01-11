#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Demo.h"

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

static unsigned int guiDemoState;
static unsigned int guiDemoTime		= 0;
static unsigned int guiCurSectSeed	= 0;
static int			giLastSection	= -1;

// -----------------------------------------------------------------------------
// DEMO Structures
// -----------------------------------------------------------------------------
typedef struct{

	int				iSect;
	int				iX;		// Starting position
	int				iY;

	int				iPlEnergy;
	int				iPlWeap;
	int				iPlScore;
	int				iPlCredit;

	int				iNumEntries;

	int				iSectionSeed;

	unsigned int*	uiFrame;
	unsigned int*	uiPressed;

}TDemoData;


// -----------------------------------------------------------------------------
// Recording DEMO Buffers and structs
// -----------------------------------------------------------------------------
#define RECORD_DEMO 
#ifdef RECORD_DEMO
#define MAX_DEMO_FRAMES	 60*SCREENFPS

unsigned int guiDEMOFrames [MAX_DEMO_FRAMES];
unsigned int guiDEMOPressed[MAX_DEMO_FRAMES];

TDemoData oDDRecord;

static int giDEMOSection = 1;
static int giLastInput	 = 0xffffffff;
#endif

// -----------------------------------------------------------------------------
// Recorded demos
// -----------------------------------------------------------------------------
#include "data/demo/Demo.c"
extern TDemoData oDD;

// -----------------------------------------------------------------------------
// Current running DEMO variables
// -----------------------------------------------------------------------------
static int guiBuffIdx;

// -----------------------------------------------------------------------------
void DEMO_Init()
{

}
// -----------------------------------------------------------------------------
void DEMO_Finish()
{
	// Put any code here for handling things that need to be done before exiting,
	// like saving records into a database, or saving preferences etc.

	/// HUD_Finish();
	
	// Remove the graphics from memory, since they are no longer needed
}

// -----------------------------------------------------------------------------
int DEMO_iLoop()
{
	switch (guiDemoState)
	{
		// ----------------------------------------------------------------------
		case DEMOSTATE_NONE:
		{
			guiDemoState = DEMOSTATE_INIT;
		}
		break;

		// ----------------------------------------------------------------------
		case DEMOSTATE_INIT:
		{
			Level0_Init();
			HUD_Init();

			// Setup current demo values
			MAP_SetCurrentSection(oDD.iSect);

			
			// Reset entities
			ENT_Reset();

			// Initialize persistent data
			LevelInitPersistent();


			// Initializa player values	
			gpoPlayerEnt->oPos.iX	= oDD.iX;
			gpoPlayerEnt->oPos.iY	= oDD.iY;
			gpoPlayerEnt->iEnergy   = oDD.iPlEnergy;
			giPlScore				= oDD.iPlScore;
			giPlCredit				= oDD.iPlCredit;
			giPlWeap				= oDD.iPlWeap;
			

			// Set current section random seed
			srand(oDD.iSectionSeed);
			
			
			
			// Start demo input
			guiBuffIdx   = 0;
						
									
			 
			guiDemoState = DEMOSTATE_RUN;
			guiDemoTime  = 0;
			
			oInput.uiPressed = oInput.uiJustPressed = oInput.uiPressed = oInput.uiReleased = 0;
		}
		break;
	
		// ----------------------------------------------------------------------
		case DEMOSTATE_RUN:
		{

			// ------------------------------------------------------------------
			// BEGIN IMPORTANT DEMO STUFF: SIMULATE EXTERNAL INPUT
			// ------------------------------------------------------------------
			if (giCurSecFrame == oDD.uiFrame[guiBuffIdx])
			{
				oInput.uiPressed = oDD.uiPressed[guiBuffIdx];
				guiBuffIdx++;
			}
						
			// ------------------------------------------------------------------
			// END IMPORTANT DEMO STUFF: SIMULATE EXTERNAL INPUT
			// ------------------------------------------------------------------
			INPUT_Update();


			LevelUpdate();

			// The game handles separatelly the player collisions
			ENT_PlayerHandleCollisions();

			// Render	
			GClear();

				LevelRender();
				HUD_Draw();
				HUD_DrawInteger(80,100,2,oInput.uiPressed);

 			GBlit();

			if ((gpoPlayerEnt->iEnergy == 0) || (guiBuffIdx == oDD.iNumEntries))
			{
				ENT_AddExplosion(gpoPlayerEnt->oPos.iX,gpoPlayerEnt->oPos.iY,ENT_BIG_EXPLOSION);
			
				guiDemoTime  = 0;
				guiDemoState = DEMOSTATE_DEAD;
				ENT_DeleteEntity(gpoPlayerEnt);
			}
		}
		break;
		
		// ----------------------------------------------------------------------
		case DEMOSTATE_DEAD:
		{	
			guiDemoTime++;			
			
			if (guiDemoTime < 2*SCREENFPS)
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
				guiDemoTime  = 0;
				guiDemoState = DEMOSTATE_FINISH;
			}
		}
		break;

		// ----------------------------------------------------------------------
		case DEMOSTATE_FINISH:
		{
			DEMO_Finish();
			
			guiDemoTime  = 0;
			guiDemoState = DEMOSTATE_NONE;
			
			return(DEMOSTATE_DEMOOVER);
		}
		break;
	}

	return(0);
}
// -----------------------------------------------------------------------------
void RecordDemo()
{
	int i;
	
	FILE *fd = fopen("data/demo/demo.c","wt");
	if (fd)
	{	
		// --------------
		// Save frames
		fprintf(fd,"unsigned int uiDEMOFrames[%d] = {\n",oDDRecord.iNumEntries);

		for (i=0;i<oDDRecord.iNumEntries;i++)
		{
			if (i%16==0)
			{
			 	fprintf(fd,"\n");
				fprintf(fd,"\t");
			}

			fprintf(fd,"%4d, ",guiDEMOFrames[i]);
		}
		fprintf(fd,"\n");
		fprintf(fd,"};\n");
		
		
		fprintf(fd,"\n");


		// --------------
		// Save pressed buttons
		fprintf(fd,"unsigned int uiDEMOPressed[%d] = {\n",oDDRecord.iNumEntries);									
		for (i=0;i<oDDRecord.iNumEntries;i++)
		{
			if (i%16==0)
			{
				fprintf(fd,"\n");
				fprintf(fd,"\t");
			}
			
			fprintf(fd,"%4d, ",guiDEMOPressed[i]);									
		}
		fprintf(fd,"\n");
		fprintf(fd,"};\n");

		fprintf(fd,"\n");

		// --------------
		// Save other data
		

	/*
	int				iSect;
	int				iX;		// Starting position
	int				iY;

	int				iPlEnergy;
	int				iPlWeap;
	int				iPlScore;
	int				iPlCredit;

	int				iNumEntries;

	int				iSectionSeed;

	unsigned int*	uiFrame;
	unsigned int*	uiPressed;
	*/

		fprintf(fd,"TDemoData oDD = {\n");
		fprintf(fd,"\t %d,\n",oDDRecord.iSect);
		fprintf(fd,"\t %d,\n",oDDRecord.iX);
		fprintf(fd,"\t %d,\n",oDDRecord.iY);
		fprintf(fd,"\n");

		fprintf(fd,"\t %d,\n",oDDRecord.iPlEnergy);
		fprintf(fd,"\t %d,\n",oDDRecord.iPlWeap);
		fprintf(fd,"\t %d,\n",oDDRecord.iPlScore);
		fprintf(fd,"\t %d,\n",oDDRecord.iPlCredit);
		fprintf(fd,"\n");

		fprintf(fd,"\t %d,\n",oDDRecord.iNumEntries);
		fprintf(fd,"\n");

		fprintf(fd,"\t %d,\n",oDDRecord.iSectionSeed);		
		fprintf(fd,"\n");
		
		fprintf(fd,"\t uiDEMOFrames ,\n");
		fprintf(fd,"\t uiDEMOPressed,\n");
		fprintf(fd,"\n");
		fprintf(fd,"};\n");
		
		fclose(fd);
	}

}
// -----------------------------------------------------------------------------
// DEMO Loop to record a demo
// -----------------------------------------------------------------------------
int DEMO_iLoopRecord()
{
	switch (guiDemoState)
	{
		// ----------------------------------------------------------------------
		case DEMOSTATE_NONE:
		{
			guiDemoState = DEMOSTATE_INIT;
		}
		break;

		// ----------------------------------------------------------------------
		case DEMOSTATE_INIT:
		{
			Level0_Init();
			HUD_Init();

			// Setup current demo values
			MAP_SetCurrentSection(giDEMOSection);
			
			// Reset entities
			ENT_Reset();

			// Initialize persistent data
			LevelInitPersistent();

			// Init player variables
			gpoPlayerEnt->oPos.iX	= 32;
			gpoPlayerEnt->oPos.iY	= SCREENHHEIGHT;
			gpoPlayerEnt->iEnergy   = 100;
			giPlScore				= 0;
			giPlCredit				= 0;
			giPlWeap				= 0;

			// Set current section random seed
			oDDRecord.iSectionSeed	= rand();
			oDDRecord.iSect			= 1;
			oDDRecord.iX			= gpoPlayerEnt->oPos.iX;
			oDDRecord.iY			= gpoPlayerEnt->oPos.iY;
			oDDRecord.iPlEnergy		= gpoPlayerEnt->iEnergy;
			oDDRecord.iPlScore      = giPlScore;
			oDDRecord.iPlCredit		= giPlCredit;			
			oDDRecord.iPlWeap		= giPlWeap;

			
			// Set current section random seed			
			srand(oDDRecord.iSectionSeed);

			
			// Start demo input
			giLastInput				= 0xffffffff;
			oDDRecord.iNumEntries	= 0;


			// Start demo input
			guiDemoState = DEMOSTATE_RUN;
			guiDemoTime  = 0;

			oInput.uiPressed = oInput.uiJustPressed = oInput.uiPressed = oInput.uiReleased = 0;
		}
		break;
	
		// ----------------------------------------------------------------------
		case DEMOSTATE_RUN:
		{
			// ------------------------------------------------------------------
			// BEGIN IMPORTANT DEMO STUFF: CAPTURE EXTERNAL INPUT
			// ------------------------------------------------------------------
			if (oInput.uiPressed != giLastInput)
			{								
				if (guiBuffIdx < MAX_DEMO_FRAMES)
				{
					guiDEMOFrames [oDDRecord.iNumEntries]	= giCurSecFrame;
					guiDEMOPressed[oDDRecord.iNumEntries]	= oInput.uiPressed;

					giLastInput								= oInput.uiPressed;
					oDDRecord.iNumEntries ++;
				}
			}
			// ------------------------------------------------------------------
			// END IMPORTANT DEMO STUFF: CAPTURE EXTERNAL INPUT
			// ------------------------------------------------------------------

			INPUT_Update();


			LevelUpdate();

			// The game handles separatelly the player collisions
			ENT_PlayerHandleCollisions();

			// Render	
			GClear();

				LevelRender();
				HUD_Draw();

 			GBlit();

			if (
				(gpoPlayerEnt->iEnergy == 0) || 
				(oDDRecord.iNumEntries == MAX_DEMO_FRAMES) ||
			    ((oInput.uiPressed     & CMD_CBUTT) && (oInput.uiJustPressed & CMD_DBUTT)) ||
				((oInput.uiJustPressed & CMD_CBUTT) && (oInput.uiPressed     & CMD_DBUTT))
				)
			{
				guiDemoTime  = 0;
				guiDemoState = DEMOSTATE_FINISH;
				
				RecordDemo();
			}
		}
		break;
		
		// ----------------------------------------------------------------------
		case DEMOSTATE_DEAD:
		{	
			guiDemoTime++;			
			
			if (guiDemoTime < 2*SCREENFPS)
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
				guiDemoTime  = 0;
				guiDemoState = DEMOSTATE_FINISH;
			}
		}
		break;

		// ----------------------------------------------------------------------
		case DEMOSTATE_FINISH:
		{	
			DEMO_Finish();
			
			guiDemoTime  = 0;
			guiDemoState = DEMOSTATE_NONE;
			
			return(DEMOSTATE_DEMOOVER);
		}
		break;
	}

	return(0);
}
//-----------------------------------------------------------------------------	

