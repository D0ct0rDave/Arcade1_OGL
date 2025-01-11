#include "game/game.h"
#include "Input/input.h"

#include "L0_EstrellitaBlast.h"
// -----------------------------------------------------------------------------
// Global definitions
// -----------------------------------------------------------------------------

// Hay que llegar a un acuerdo para decidir que flags seran reutilizados para enemigos en los diferentes mapas, cuales

#define		L0ENT_ESTRELLITABLAST		ENT_USER_08
#define		SPRITE_ID_ESTRELLITABLAST	83

/*******************************************************
Generic blasting functions
--------------------------------------------------------
*******************************************************/
void Level0Ent_EstrellitaBlastThink(TEntity *_poEnt)
{
	TEntity *poCollEnt;
	int iXMovStep = _poEnt->oTargetPos.iX;
	int iYMovStep = _poEnt->oTargetPos.iY;
	
	_poEnt->oPos.iX += iXMovStep;
	_poEnt->oPos.iY += iYMovStep;

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


	// Control collision against map
	poCollEnt = ENT_poTestCollision(_poEnt,ENT_WORLD);

	if (poCollEnt)
	{
		// Add SFX
	
		ENT_DeleteEntity(_poEnt);
		return;
	}

	// Update graphic part
	ENT_EntityScreen2World(_poEnt);
}

// -----------------------------------------------------------------------------
void Level0Ent_AddEstrellitaBlast(int _iScrX,int _iScrY)
{
	int			iEntID = ENT_iAddEntity (L0ENT_ESTRELLITABLAST,_iScrX,_iScrY,SPRITE_ID_ESTRELLITABLAST,Level0Ent_EstrellitaBlastThink);
	TEntity*	poEnt  = ENT_poGetEntity(iEntID);

	poEnt->oTargetPos.iX = (gpoPlayerEnt->oPos.iX - _iScrX) / 60;
	poEnt->oTargetPos.iY = (gpoPlayerEnt->oPos.iY - _iScrY) / 60;
}
// -----------------------------------------------------------------------------