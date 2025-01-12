// -----------------------------------------------------------------------------
#include "L0_Turret.h"
#include "L0_CommonBlast.h"
#include "Levels/Entities/Entity.h"
#include "Graphics/GMap.h"
#include "Graphics/GCam.h"

#include <stdlib.h>
// -----------------------------------------------------------------------------

#define		L0ENT_TURRET			ENT_USER_04
#define		SPRITE_ID_TURRET		85
#define		SPRITE_ID_TURRETBLAST	101

int giSpawnedTurrets = 0;
// -----------------------------------------------------------------------------
#define ENEMY2_STEP0	2
#define ENEMY2_STEP1	3
#define ENEMY2_STEP2	1
// -----------------------------------------------------------------------------
void Level0Ent_TurretThink(TEntity *_poEnt)
{
	extern int	giDisableAI;
	if (giDisableAI) return;

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

	if ((GFRAMENUM % 40) == 0)
	{
		if ((rand() % 3) == 0)
		{
			if (_poEnt->iSubType == 1)
				Level0Ent_AddCommonBlast(_poEnt->oPos.iX,_poEnt->oPos.iY+32,SPRITE_ID_TURRETBLAST,0);
			else
				Level0Ent_AddCommonBlast(_poEnt->oPos.iX,_poEnt->oPos.iY,SPRITE_ID_TURRETBLAST,0);
		}
	}
	
	 ENT_EntityWorld2Screen(_poEnt);
}
// -----------------------------------------------------------------------------
void Level0Ent_AddTurret(int _iUpDown)
{
	int		iEntID;
	TEntity *poEnt;  
	int i;
	int iXTile = GCamMaxX / 64;
	int iTX    = gpoMap->poMSecs[giCurSec].usTX;
	int iSprID;
	int iScrY;
	int	iRand  = rand() % 100;

	if (_iUpDown)
	{
		// Por la parte de arriba
		for (i=0;i<7;i++)
		{
			if (gpoMap->poMSecs[giCurSec].pucOvData[i*iTX + iXTile] == 0)
			{
				iScrY = i*64 - 32;
				break;
			}
		}
	}
	else
	{
		// Por la parte de abajo
		for (i=7;i>=0;i--)
		{
			if (gpoMap->poMSecs[giCurSec].pucOvData[i*iTX + iXTile] == 0)
			{
				iScrY = i*64 + 32;
				break;
			}
		}
	}

	iEntID = ENT_iAddEntity (L0ENT_TURRET,SCREENWIDTH,iScrY,_iUpDown?80:81,Level0Ent_TurretThink);
	poEnt  = ENT_poGetEntity(iEntID);


	poEnt->iEnergy  = 15;
	poEnt->iTime    = 0;
	poEnt->iState   = 0;
	poEnt->iSubType = _iUpDown;
	giSpawnedTurrets ++;

	poEnt->iSpawnPU = (iRand < 10)?(iRand<5)?2:1:0;
}
// -----------------------------------------------------------------------------
