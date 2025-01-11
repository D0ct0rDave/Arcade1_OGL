// -----------------------------------------------------------------------------
#include "L0_SmallWall.h"
#include "Levels/Entities/Entity.h"
#include "utils/fmath.h"
// -----------------------------------------------------------------------------

#define		L0ENT_SWALL				ENT_USER_06
#define		SPRITE_ID				0

// -----------------------------------------------------------------------------
#define ENEMY3_STEP0	2
#define ENEMY3_STEP1	3
#define ENEMY3_STEP2	1
// -----------------------------------------------------------------------------
void Level0Ent_SmallWallThink(TEntity *_poEnt)
{
	int iOfs;
	
	// Check if the entity goes outside the screen
	/*
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
	*/

	_poEnt->iTime ++;

	// Move enemy
	iOfs = _poEnt->iAux[0] * sSin(     (2*65535*(_poEnt->iTime) / SCREENFPS) / _poEnt->iAux[1]  ) / 32768;

	if (_poEnt->iSubType == 0)	
		_poEnt->poSP->iY = _poEnt->oIniPos.iY + iOfs;
	else
		_poEnt->poSP->iY = _poEnt->oIniPos.iY - iOfs;

	// Setup correctly coordinates
	ENT_EntityWorld2Screen(_poEnt);
}
// -----------------------------------------------------------------------------
void Level0Ent_AddWall(int _iScrY,int _iType,int _iAmp,int _iPeriod)
{
	int			iEntID;
	TEntity*	poEnt;	
	
	iEntID = ENT_iAddEntity (L0ENT_SWALL,SCREENWIDTH,_iScrY,SPRITE_ID + _iType,Level0Ent_SmallWallThink);
	poEnt  = ENT_poGetEntity(iEntID);
	poEnt->poSP->ucFlag |= SPRITEFLAG_MEGASPRITE;

	// Muy chungo pero posible
	poEnt->iEnergy  = 500;

	poEnt->iTime    = 0;	
	poEnt->iState   = 0;
	poEnt->iSubType = _iType % 2;	
	poEnt->oIniPos  = poEnt->oPos;
	
	poEnt->iAux[0]	= _iAmp;
	poEnt->iAux[1]	= _iPeriod;

	// Siempre saca un pw de los buenos
	poEnt->iSpawnPU = 2;
}
// -----------------------------------------------------------------------------
