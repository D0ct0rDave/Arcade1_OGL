// -----------------------------------------------------------------------------
#include "L0_ZigZag.h"
#include "L0_CommonBlast.h"
#include "Levels/Entities/Entity.h"

#include <stdlib.h>
// -----------------------------------------------------------------------------

#define		L0ENT_ZIGZAG			ENT_USER_03
#define		SPRITE_ID				83
#define		SPRITE_ID_BLAST			100

int giSpawnedZigZags = 0;
// -----------------------------------------------------------------------------
#define ENEMY2_STEP0	2
#define ENEMY2_STEP1	3
#define ENEMY2_STEP2	1
// -----------------------------------------------------------------------------
void Level0Ent_ZigZagThink(TEntity *_poEnt)
{
	extern int	giDisableAI;
	if (giDisableAI) return;

	// Check if the entity goes outside the screen
	if (
			(_poEnt->oPos.iX + gpoMap->poTiles[SPRITE_ID].usTX < 0) || 
			(_poEnt->oPos.iY + gpoMap->poTiles[SPRITE_ID].usTY < 0)
		)
	{
		ENT_DeleteEntity(_poEnt);
		return;
	}

	if (_poEnt->iSubType == 0)
	{
		if (_poEnt->iState == 0)
		{
			_poEnt->oPos.iY -= ENEMY2_STEP0;

			if (_poEnt->oPos.iY == 0)
				_poEnt->iState = 1;
		}
		else
		{
			_poEnt->oPos.iY += ENEMY2_STEP0;

			if (_poEnt->oPos.iY == SCREENHHEIGHT)
				_poEnt->iState = 0;
		}
	}
	else
	{
		if (_poEnt->iState == 0)
		{
			_poEnt->oPos.iY += ENEMY2_STEP0;

			if (_poEnt->oPos.iY == SCREENHEIGHT)
				_poEnt->iState = 1;
		}
		else
		{
			_poEnt->oPos.iY -= ENEMY2_STEP0;

			if (_poEnt->oPos.iY == 0)
				_poEnt->iState = 0;
		}
	}

	_poEnt->oPos.iX -= ENEMY2_STEP1;


	if ((GFRAMENUM % 40) == 0)
	{
		if ((rand() % 3) == 0)
			Level0Ent_AddCommonBlast(_poEnt->oPos.iX,_poEnt->oPos.iY,SPRITE_ID_BLAST,0);
	}
	
	ENT_EntityScreen2World(_poEnt);
}
// -----------------------------------------------------------------------------
void Level0Ent_AddZigZag(int _iScrX,int _iScrY)
{
	int		iEntID = ENT_iAddEntity (L0ENT_ZIGZAG,_iScrX,_iScrY,SPRITE_ID,Level0Ent_ZigZagThink);
	TEntity*poEnt  = ENT_poGetEntity(iEntID);
	int		iRand  = rand() % 100;

	poEnt->iEnergy  = 5;
	poEnt->iState   = 0;
	poEnt->iSubType = giSpawnedZigZags % 2;
	giSpawnedZigZags ++;
	poEnt->iSpawnPU = (iRand < 10)?(iRand<5)?2:1:0;
}
// -----------------------------------------------------------------------------