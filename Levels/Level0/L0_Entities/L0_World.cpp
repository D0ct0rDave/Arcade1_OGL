// -----------------------------------------------------------------------------
#include <stdlib.h>

#include "L0_World.h"
#include "Graphics/GCam.h"

#include "L0_Entities.h"
#include "L0_Estrellita.h"
#include "L0_Bacterias.h"
#include "L0_ZigZag.h"
#include "L0_Cross.h"
#include "L0_Sphere.h"
#include "L0_Sphere2.h"
#include "L0_Wave.h"
#include "L0_Bouncer.h"
#include "L0_Turret.h"
#include "L0_MapEnt/L0_SmallWall.h"

// -----------------------------------------------------------------------------
void Level0Ent_WorldInit()
{
	// Add world entity
	ENT_iAddEntity(ENT_WORLD ,0,0,-1,Level0Ent_WorldThink);
}
// -----------------------------------------------------------------------------
// Util para spawnear cada cierto tiempo enemigos, powerups, y controlar eventos
// que ocurren en el mundo cada cierto tiempo

void Level0Ent_WorldThink(TEntity *_poEnt)
{
	static int iFirstTime = 1;
	int iAux;
	int i;
	int iSec = MAP_iGetSection();
	int iSecTime = giCurSecFrame / SCREENFPS;

	if ((giCurSecFrame % SCREENFPS) != 0) return;


	// <DEBUG CODE>

	/*
	if (iSec == 0)
	{
		MAP_SetCurrentSection(1);
		return;
	}
	*/

	// <DEBUG CODE>	
	/*
	if (! iFirstTime) return;
	iFirstTime = 0;
	Level0Ent <AddWall(SCREENHHEIGHT-100,0,0,8);	
	Level0Ent_AddWall(SCREENHHEIGHT+100,1,0,8);
	return;
	*/


	switch (giCurSec)
	{
		// ---------------------------------------------------------------
		// Seccion 0
		// ---------------------------------------------------------------
		case 0:
		{
			switch (iSecTime)
			{
				// Wave
				case 5:
				case 20:
				{
					// Add a serie
					for (i=0;i<5;i++)
						Level0Ent_AddWave(SCREENWIDTH + (i*70),150);
				}
				break;
				
				// Wave		
				case 9:
				case 23:
				{
					for (i=0;i<5;i++)
						Level0Ent_AddWave(SCREENWIDTH + (i*70),300);
				}
				break;

				// Cross
				case 14:
				case 16:
				{
					Level0Ent_AddCross(SCREENWIDTH,SCREENHHEIGHT);
					Level0Ent_AddCross(SCREENWIDTH,SCREENHHEIGHT);
				}
				break;

					
				// ZigZag
				case 31:
				{
					Level0Ent_AddZigZag(SCREENWIDTH,SCREENHHEIGHT);
					Level0Ent_AddZigZag(SCREENWIDTH,SCREENHHEIGHT);
				}
				break;
				
				case 34:
				case 38:
				{
					Level0Ent_AddCross(SCREENWIDTH,SCREENHHEIGHT);
				}

				case 36:
				case 40:
				{
					Level0Ent_AddZigZag(SCREENWIDTH,SCREENHHEIGHT);
				}
				break;
			
			}
		}
		break;


		// ---------------------------------------------------------------
		// Seccion 1
		// ---------------------------------------------------------------
		case 1:
		{
			switch (iSecTime)
			{
				case 4:
				case 14:
				{
					Level0Ent_AddTurret(0);
					Level0Ent_AddTurret(1);	
				}
				break;

				case 10:
				case 18:
				{
					// Add a serie
					for (i=0;i<5;i++)
						Level0Ent_AddBacterias(SCREENWIDTH-100,SCREENHEIGHT-50);
				}
				break;
				
				case 24:
				{
					Level0Ent_AddWall(SCREENHHEIGHT-125,0,60,16);
					Level0Ent_AddWall(SCREENHHEIGHT+125,1,60,16);
				}
				break;


				case 31:
				{
					// Add a serie
					for (i=0;i<15;i++)					
						Level0Ent_AddSphere2(SCREENWIDTH - 30,0 - i*70);
				}
				break;

				case 35:
				{

					// Add a serie
					for (i=0;i<15;i++)					
						Level0Ent_AddSphere2(SCREENWIDTH - 30,SCREENHEIGHT + i*70);
				}
				break;

				case 46:
				{
					Level0Ent_AddTurret(0);
				}
				break;
				
				case 47:
				{					
					Level0Ent_AddTurret(1);	
				}
				break;


				case 58:
				case 63:
				{
					Level0Ent_AddTurret(0);
					Level0Ent_AddTurret(1);
				}
				break;
			}
		}
		break;
	
		
		// ---------------------------------------------------------------
		// Seccion 2
		// ---------------------------------------------------------------
		case 2:
		{
			switch (iSecTime)
			{
				case 1:
				{
					Level0Ent_AddWall(SCREENHHEIGHT-128-90,2,60,8);
					Level0Ent_AddWall(SCREENHHEIGHT+30,3,60,8);	
				}
				break;

				case 10:
				{
					Level0Ent_AddTurret(0);
					Level0Ent_AddTurret(1);	
				}
				break;


				case 17:
				{
					// Add a serie
					for (i=0;i<15;i++)					
						Level0Ent_AddSphere2(SCREENWIDTH - 30,0 - i*70);
				}
				break;

				case 21:
				{
					// Add a serie
					for (i=0;i<15;i++)					
						Level0Ent_AddSphere2(SCREENWIDTH-10,SCREENHEIGHT + i*70);
				}
				break;

				case 23:
				{
					Level0Ent_AddTurret(0);
				}
				break;
			}
		}
		break;

		// ---------------------------------------------------------------
		// Seccion 3
		// ---------------------------------------------------------------
		case 3:
		{
			switch (iSecTime)
			{
				case 0:
				{
					// Add a serie
					for (i=0;i<5;i++)
						Level0Ent_AddBacterias(SCREENWIDTH-100,50);
				}
				break;
			}
		}
		break;
	}
}
// -----------------------------------------------------------------------------