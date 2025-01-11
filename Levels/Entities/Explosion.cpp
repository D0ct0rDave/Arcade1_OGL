// -----------------------------------------------------------------------------
#include "Explosion.h"
#include "Levels/Entities/Entity.h"
// -----------------------------------------------------------------------------
#define		ENT_EXPLOSION					ENT_USER_26
#define		SPRITE_BIG_EXPLOSION_ID			78
#define		SPRITE_SMALL_EXPLOSION_ID		79
// -----------------------------------------------------------------------------
void ENT_ExplosionThink(TEntity *_poEnt)
{
	// Check if the entity goes outside the screen
	
	_poEnt->iTime--;

	if (
			(! _poEnt->iTime) ||
			(_poEnt->oPos.iX + gpoMap->poTiles[_poEnt->iSubType].usTX < 0) || 
			(_poEnt->oPos.iY + gpoMap->poTiles[_poEnt->iSubType].usTY < 0)
		)
	{
		ENT_DeleteEntity(_poEnt);
		return;
	}
}
// -----------------------------------------------------------------------------
void ENT_AddExplosion(int _iScrX,int _iScrY,int _iType)
{	
	int		iEntID  = ENT_iAddEntity (ENT_EXPLOSION,_iScrX,_iScrY,_iType?SPRITE_SMALL_EXPLOSION_ID:SPRITE_BIG_EXPLOSION_ID,ENT_ExplosionThink);
	TEntity*poEnt   = ENT_poGetEntity(iEntID);
	
	poEnt->iSubType = _iType?SPRITE_SMALL_EXPLOSION_ID:SPRITE_BIG_EXPLOSION_ID;
	poEnt->iTime    = SCREENFPS;
}
// -----------------------------------------------------------------------------
