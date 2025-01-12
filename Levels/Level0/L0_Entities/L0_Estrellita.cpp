// -----------------------------------------------------------------------------
#include "L0_Estrellita.h"
#include "L0_CommonBlast.h"
#include "Levels/Entities/Entity.h"

#include <stdlib.h>
// -----------------------------------------------------------------------------

#define		L0ENT_ESTRELLITA			ENT_USER_01
#define		SPRITE_ID_ESTRELLITA		82
#define		SPRITE_ID_ESTRELLITABLAST	101

// -----------------------------------------------------------------------------
void Level0Ent_EstrellitaThink(TEntity *_poEnt)
{
	extern int	giDisableAI;
	if (giDisableAI) return;

	#define ENEMY0_STEP 5

	switch (_poEnt->iState)
	{
		case 0:
		{
			if (_poEnt->oPos.iY < 350)
				_poEnt->oPos.iY += ENEMY0_STEP;
			else
				_poEnt->iState = 1;
		}
		break;

		case 1:
		{
			if (_poEnt->oPos.iX > 50)
				_poEnt->oPos.iX-= ENEMY0_STEP;
			else
				_poEnt->iState = 2;
		}
		break;
		
		case 2:
		{
			if (_poEnt->oPos.iY > 50)
				_poEnt->oPos.iY-= ENEMY0_STEP;
			else
				_poEnt->iState = 3;
		}
		break;
	
		case 3:
		{
			if (_poEnt->oPos.iX<500)
				_poEnt->oPos.iX+= ENEMY0_STEP;
			else
				_poEnt->iState = 4;
		}
		break;

		case 4:
		{			
			if (_poEnt->oPos.iY < SCREENHEIGHT+64)
				_poEnt->oPos.iY+= ENEMY0_STEP;
			else
				// _poEnt->iState = 5;
				ENT_DeleteEntity(_poEnt);
		}
		break;
	}

	if ((GFRAMENUM % 20) == 0)
	{
		if ((rand() % 4) == 0)
			Level0Ent_AddCommonBlast(_poEnt->oPos.iX,_poEnt->oPos.iY,SPRITE_ID_ESTRELLITABLAST,0);
	}

	ENT_EntityScreen2World(_poEnt);
}
// -----------------------------------------------------------------------------
void Level0Ent_AddEstrellita(int _iScrX,int _iScrY)
{
	int iEntID = ENT_iAddEntity(L0ENT_ESTRELLITA,_iScrX,_iScrY,SPRITE_ID_ESTRELLITA,Level0Ent_EstrellitaThink);
	TEntity*poEnt = ENT_poGetEntity(iEntID);
	int		iRand  = rand() % 100;

	poEnt->iEnergy = 3;
	poEnt->iSpawnPU = (iRand < 10)?(iRand<5)?2:1:0;
}
// -----------------------------------------------------------------------------