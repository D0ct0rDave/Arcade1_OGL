// -----------------------------------------------------------------------------
#include "L0_Sphere.h"
#include "L0_CommonBlast.h"
#include "Levels/Entities/Entity.h"

#include <stdlib.h>
// -----------------------------------------------------------------------------
#define		L0ENT_SPHERE			ENT_USER_04
#define		SPRITE_ID				89
#define		SPRITE_ID_BLAST			100
// -----------------------------------------------------------------------------
#define ENEMY2_STEP0	2
#define ENEMY2_STEP1	3
#define ENEMY2_STEP2	1
// -----------------------------------------------------------------------------
void Level0Ent_SphereThink(TEntity *_poEnt)
{
	// Check if the entity goes outside the screen
	if (
			(_poEnt->oPos.iX + gpoMap->poTiles[SPRITE_ID].usTX < 0) || 
			(_poEnt->oPos.iY + gpoMap->poTiles[SPRITE_ID].usTY < 0)
		)
	{
		ENT_DeleteEntity(_poEnt);
		return;
	}

	_poEnt->oPos.iX -= ENEMY2_STEP0;

	if ((GFRAMENUM % 40) == 0)
	{
		if ((rand() % 3) == 0)
			Level0Ent_AddCommonBlast(_poEnt->oPos.iX,_poEnt->oPos.iY,SPRITE_ID_BLAST,0);
	}
	
	ENT_EntityScreen2World(_poEnt);
}
// -----------------------------------------------------------------------------
void Level0Ent_AddSphere(int _iScrX,int _iScrY)
{
	int		iEntID = ENT_iAddEntity (L0ENT_SPHERE,_iScrX,_iScrY,SPRITE_ID,Level0Ent_SphereThink);
	TEntity*poEnt  = ENT_poGetEntity(iEntID);
	int		iRand  = rand() % 100;

	poEnt->iEnergy  = 10;
	poEnt->iSpawnPU = (iRand < 10)?(iRand<5)?2:1:0;
}
// -----------------------------------------------------------------------------
#undef SPRITE_ID
#undef SPRITE_ID_BLAST