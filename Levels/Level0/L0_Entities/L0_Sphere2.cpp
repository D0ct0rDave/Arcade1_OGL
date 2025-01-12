// -----------------------------------------------------------------------------
#include "L0_Sphere2.h"
#include "L0_CommonBlast.h"
#include "Levels/Entities/Entity.h"

#include <stdlib.h>
// -----------------------------------------------------------------------------
#define		L0ENT_SPHERE2			ENT_USER_04
#define		SPRITE_ID				88
#define		SPRITE_ID_BLAST			100
// -----------------------------------------------------------------------------
#define ENEMY2_STEP0	2
// -----------------------------------------------------------------------------
// NOTA: Las entidades que se mueven al son del scroll, son consideradas en 
// coordenadas de mundo, es por ello que al actualizarlas, debemos actualizar las 
// coordenadas de pantalla en funcion de las de mundo (ENT_EntityWorld2Screen)
// Las entidades que se mueven por la pantalla son consideradas en coordenadas 
// de pantalla, y debemos actualizar las coordenadas de mundo en funcion de las de 
// pantalla (ENT_EntityScreen2World)

// -----------------------------------------------------------------------------
void Level0Ent_Sphere2Think(TEntity *_poEnt)
{
	extern int	giDisableAI;
	if (giDisableAI) return;

	// Check if the entity goes outside the screen
	if	(
			(_poEnt->oPos.iX + gpoMap->poTiles[SPRITE_ID].usTX < 0)
		)
	{
		ENT_DeleteEntity(_poEnt);
		return;
	}



	if (_poEnt->iSubType == 0)
		_poEnt->poSP->iY += ENEMY2_STEP0;
	else
		_poEnt->poSP->iY -= ENEMY2_STEP0;



	if ((GFRAMENUM % 40) == 0)
	{
		if ((rand() % 6) == 0)
			Level0Ent_AddCommonBlast(_poEnt->oPos.iX,_poEnt->oPos.iY,SPRITE_ID_BLAST,0);
	}

	ENT_EntityWorld2Screen(_poEnt);
}
// -----------------------------------------------------------------------------
void Level0Ent_AddSphere2(int _iScrX,int _iScrY)
{
	int		iEntID = ENT_iAddEntity (L0ENT_SPHERE2,_iScrX,_iScrY,SPRITE_ID,Level0Ent_Sphere2Think);
	TEntity*poEnt  = ENT_poGetEntity(iEntID);
	int		iRand  = rand() % 100;

	poEnt->iEnergy  = 8;
	poEnt->iSpawnPU = (iRand < 10)?(iRand<5)?2:1:0;
	poEnt->iSubType = (_iScrY >= SCREENHHEIGHT);
}
// -----------------------------------------------------------------------------
