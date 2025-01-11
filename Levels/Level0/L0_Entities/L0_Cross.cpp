// -----------------------------------------------------------------------------
#include "L0_Cross.h"
#include "L0_CommonBlast.h"
#include "Levels/Entities/Entity.h"

#include <stdlib.h>
// -----------------------------------------------------------------------------

#define		L0ENT_CROSS				ENT_USER_04
#define		SPRITE_ID_CROSS			86
#define		SPRITE_ID_CROSSBLAST	100

int giSpawnedCrosses = 0;
// -----------------------------------------------------------------------------
#define ENEMY3_STEP0	2
#define ENEMY3_STEP1	3
#define ENEMY3_STEP2	1
// -----------------------------------------------------------------------------
void Level0Ent_CrossThink(TEntity *_poEnt)
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

	// Move enemy
	if (_poEnt->iSubType == 0)
		_poEnt->oPos.iY += ENEMY3_STEP0;
	else
		_poEnt->oPos.iY -= ENEMY3_STEP0;

	_poEnt->oPos.iX -= ENEMY3_STEP1;

	// Add blast
	if ((GFRAMENUM % 40) == 0)
	{
		if ((rand() % 3) == 0)
			Level0Ent_AddCommonBlast(_poEnt->oPos.iX,_poEnt->oPos.iY,SPRITE_ID_CROSSBLAST,0);
	}
	
	// Setup correctly coordinates
	ENT_EntityScreen2World(_poEnt);
}
// -----------------------------------------------------------------------------
void Level0Ent_AddCross(int _iScrX,int _iScrY)
{
	int			iSubType = (giSpawnedCrosses % 2);
	int			iEntID;
	TEntity*	poEnt;
	int		iRand  = rand() % 100;
		
	iEntID = ENT_iAddEntity (L0ENT_CROSS,_iScrX,iSubType*SCREENHEIGHT,SPRITE_ID_CROSS,Level0Ent_CrossThink);
	poEnt  = ENT_poGetEntity(iEntID);

	poEnt->iEnergy  = 5;
	poEnt->iTime    = 0;
	poEnt->iTime    = 50 + (rand() % 75);
	poEnt->iState   = 0;
	poEnt->iSubType = iSubType;
	giSpawnedCrosses ++;
	poEnt->iSpawnPU = (iRand < 10)?(iRand<5)?2:1:0;
}
// -----------------------------------------------------------------------------
