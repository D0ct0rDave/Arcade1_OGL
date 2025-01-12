// -----------------------------------------------------------------------------
#include "L0_Wave.h"
#include "L0_CommonBlast.h"
#include "Levels/Entities/Entity.h"
#include "Utils/FMath.h"

#include <stdlib.h>
// -----------------------------------------------------------------------------

#define		L0ENT_WAVE			ENT_USER_03
#define		SPRITE_ID			87
#define		SPRITE_ID_BLAST		100

// -----------------------------------------------------------------------------
#define ENEMY_STEP		2
// -----------------------------------------------------------------------------
void Level0Ent_WaveThink(TEntity *_poEnt)
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


	#define MARGIN (SCREENWIDTH/5)
	#define MULT	60

	switch (_poEnt->iState)		
	{
		case 0:		
		{
			_poEnt->oPos.iX -= ENEMY_STEP;

			if (_poEnt->oPos.iX < (SCREENWIDTH-MARGIN))
				_poEnt->iState = 1;
		}
		break;
		
		case 1:
		{
			int iAngle;
			int iSin;

			_poEnt->oPos.iX -= ENEMY_STEP;
			
			iAngle = (65535 * (_poEnt->oPos.iX - MARGIN)) / (SCREENWIDTH - 2*MARGIN);
			iSin   = sSin(iAngle);

			if (_poEnt->iSubType == 0)
				_poEnt->oPos.iY = _poEnt->oTargetPos.iY + MULT*iSin / 32768;
			else
				_poEnt->oPos.iY = _poEnt->oTargetPos.iY - MULT*iSin / 32768;

			if (_poEnt->oPos.iX < MARGIN)
				_poEnt->iState = 2;
		}
		break;

		case 2:
		{
			_poEnt->oPos.iX -= ENEMY_STEP;
		}
		break;
	}


	if ((GFRAMENUM % 40) == 0)
	{
		if ((rand() % 9) == 0)
			Level0Ent_AddCommonBlast(_poEnt->oPos.iX,_poEnt->oPos.iY,SPRITE_ID_BLAST,0);
	}
	
	ENT_EntityScreen2World(_poEnt);
}
// -----------------------------------------------------------------------------
void Level0Ent_AddWave(int _iScrX,int _iScrY)
{
	int		iEntID = ENT_iAddEntity (L0ENT_WAVE,_iScrX,_iScrY,SPRITE_ID,Level0Ent_WaveThink);
	TEntity*poEnt  = ENT_poGetEntity(iEntID);
	int		iRand  = rand() % 100;

	poEnt->iEnergy  = 8;
	poEnt->iTime    = 0;	
	poEnt->iState   = 0;
	poEnt->iSubType = _iScrY > SCREENHHEIGHT?1:0;
	poEnt->iSpawnPU = (iRand < 10)?(iRand<5)?2:1:0;
	
	poEnt->oTargetPos.iX = _iScrX;
	poEnt->oTargetPos.iY = _iScrY;
}
// -----------------------------------------------------------------------------
