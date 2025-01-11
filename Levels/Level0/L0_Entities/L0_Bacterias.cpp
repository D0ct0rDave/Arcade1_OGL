// -----------------------------------------------------------------------------
#include "L0_Bacterias.h"
#include "L0_EstrellitaBlast.h"
#include "Levels/Entities/Entity.h"
#include "L0_CommonBlast.h"

#include <stdlib.h>
// -----------------------------------------------------------------------------

#define		L0ENT_BACTERIA			ENT_USER_02
#define		SPRITE_ID_BACTERIA		84
#define		SPRITE_ID_BACTERIABLAST	100

int giSpawnedBacts = 0;
// -----------------------------------------------------------------------------
#define ENEMY1_STEP0	3
#define ENEMY1_STEP1	2
#define ENEMY1_STEP2	1

void Level0Ent_BacteriasThink(TEntity *_poEnt)
{
	// Check if the entity goes outside the screen
	if (
			(_poEnt->oPos.iX > SCREENWIDTH) || 
			(_poEnt->oPos.iY > SCREENHEIGHT) ||
			(_poEnt->oPos.iX < 0) || 
			(_poEnt->oPos.iY < 0)
		)
	{
		ENT_DeleteEntity(_poEnt);
		return;
	}


	_poEnt->iTime--;
	
	if (_poEnt->iTime == 0)	// 15 sec
	{
		_poEnt->iState = 5;

		// steps in this direction
   		_poEnt->oTargetPos.iX = rand() % 10;
	}

	switch (_poEnt->iState)
	{
		case 0:
		{
			if (_poEnt->oPos.iY < SCREENHHEIGHT)
				// Bacterias en la parte alta de la pantalla
				_poEnt->poSP->iY += ENEMY1_STEP0;
			else
				// Bacterias en la parte baja de la pantalla
				_poEnt->poSP->iY -= ENEMY1_STEP0;
			
			// _poEnt->oPos.iX += 0;
		}
		break;
					
		case 1:
		{
			if (_poEnt->poSP->iY < SCREENHHEIGHT)
				// Bacterias en la parte alta de la pantalla
				_poEnt->poSP->iY += ENEMY1_STEP1;
			else
				// Bacterias en la parte baja de la pantalla
				_poEnt->poSP->iY -= ENEMY1_STEP1;
			
			 _poEnt->poSP->iX += ENEMY1_STEP2;
		}
		break;
					
		case 2:
		{
			if (_poEnt->poSP->iY < SCREENHHEIGHT)
				// Bacterias en la parte alta de la pantalla
				_poEnt->poSP->iY += ENEMY1_STEP1;
			else
				// Bacterias en la parte baja de la pantalla
				_poEnt->poSP->iY -= ENEMY1_STEP1;
			
			 _poEnt->poSP->iX -= ENEMY1_STEP2;
		}
		break;
					
		case 3:
		{
			if (_poEnt->poSP->iY < SCREENHHEIGHT)
				// Bacterias en la parte alta de la pantalla
				_poEnt->poSP->iY += ENEMY1_STEP2;
			else
				// Bacterias en la parte baja de la pantalla
				_poEnt->poSP->iY -= ENEMY1_STEP2;
			
			 _poEnt->poSP->iX += ENEMY1_STEP1;
		}
		break;
					
		case 4:
		{
			if (_poEnt->poSP->iY < SCREENHHEIGHT)
				// Bacterias en la parte alta de la pantalla
				_poEnt->poSP->iY += ENEMY1_STEP2;
			else
				// Bacterias en la parte baja de la pantalla
				_poEnt->poSP->iY -= ENEMY1_STEP2;
			
			 _poEnt->poSP->iX -= ENEMY1_STEP1;
		}
		break;

		case 5:
		{
			/*
			if (_poEnt->poSP->iX > 50)
				_poEnt->poSP->iY -= ENEMY1_STEP;
			else
				_poEnt->iState = 2;
			*/
		}
		break;
	}
	
	ENT_EntityWorld2Screen(_poEnt);

	if ((GFRAMENUM % 40) == 0)
	{
		if ((rand() % 8) == 0)
			Level0Ent_AddCommonBlast(_poEnt->oPos.iX,_poEnt->oPos.iY,SPRITE_ID_BACTERIABLAST,0);
	}
}
// -----------------------------------------------------------------------------
void Level0Ent_AddBacterias(int _iScrX,int _iScrY)
{
	int		iEntID = ENT_iAddEntity(L0ENT_BACTERIA,_iScrX,_iScrY,SPRITE_ID_BACTERIA,Level0Ent_BacteriasThink);
	TEntity*poEnt  = ENT_poGetEntity(iEntID);
	int		iRand  = rand() % 100;

	poEnt->iEnergy = 5;
	poEnt->iTime   = 0;
	poEnt->iTime   = 50 + (rand() % 75);
	poEnt->iState   = (giSpawnedBacts % 5);
	poEnt->iSpawnPU = (iRand < 10)?(iRand<5)?2:1:0;
	giSpawnedBacts ++;
}
// -----------------------------------------------------------------------------