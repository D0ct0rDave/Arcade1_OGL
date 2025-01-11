#include "levels/entities/entity.h"
#include "L0_CommonBlast.h"
#include "levels/entities/player.h"
#include <math.h>
// -----------------------------------------------------------------------------
// Global definitions
// -----------------------------------------------------------------------------

// Hay que llegar a un acuerdo para decidir que flags seran reutilizados para enemigos en los diferentes mapas, cuales

#define		L0ENT_COMMONBLAST		ENT_USER_09
#define		SPRITE_ID_COMMONBLAST	100

/*******************************************************
Generic blasting functions
--------------------------------------------------------
*******************************************************/
void Level0Ent_CommonBlastThink(TEntity *_poEnt)
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
void Level0Ent_AddCommonBlast(int _iScrX,int _iScrY,int _iSprID,int _iSubType)
{
	#define BLAST_SPEED 4

	int			iEntID = ENT_iAddEntity (L0ENT_COMMONBLAST,_iScrX,_iScrY,_iSprID,Level0Ent_CommonBlastThink);
	TEntity*	poEnt  = ENT_poGetEntity(iEntID);

	int dX             = (gpoPlayerEnt->oPos.iX - _iScrX);
	int dY             = (gpoPlayerEnt->oPos.iY - _iScrY);

	int iSqrt          = sqrtf(dX*dX + dY*dY);
	
	poEnt->oTargetPos.iX = (BLAST_SPEED * dX) / iSqrt;
	poEnt->oTargetPos.iY = (BLAST_SPEED * dY) / iSqrt;
}
// -----------------------------------------------------------------------------