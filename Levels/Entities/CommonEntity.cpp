// -----------------------------------------------------------------------------
#include "CommonEntity.h"
#include "Levels/Entities/Entity.h"
// -----------------------------------------------------------------------------
void ENT_CommonEntityThink(TEntity *_poEnt)
{
	// Check if the entity goes outside the screen
	if (	
		(_poEnt->oPos.iX > SCREENWIDTH) || 
		(_poEnt->oPos.iY > SCREENHEIGHT) ||
		(_poEnt->oPos.iX + gpoMap->poTiles[_poEnt->iSubType].usTX < 0) || 
		(_poEnt->oPos.iY + gpoMap->poTiles[_poEnt->iSubType].usTY < 0)
		)
	{
		ENT_DeleteEntity(_poEnt);
		return;
	}
}
// -----------------------------------------------------------------------------
void ENT_AddCommonEntity(int _iScrX,int _iScrY,int _iType,int _iSprID)
{	
	int		iEntID  = ENT_iAddEntity (_iType,_iScrX,_iScrY,_iSprID,ENT_CommonEntityThink);
	TEntity*poEnt   = ENT_poGetEntity(iEntID);
	
	poEnt->iSubType = _iSprID;
}
// -----------------------------------------------------------------------------
