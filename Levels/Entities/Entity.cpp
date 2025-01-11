#include <stdlib.h>	// for NULL definition

#include "Entity.h"
#include "graphics/GCam.h"

// -----------------------------------------------------------------------------
TEntityList 	oEL;					
// -----------------------------------------------------------------------------
int ENT_iAddEntity(int _iType,int _iX,int _iY,int _iSprID,TEntityThinkFunc _pThink)
{
	int iTestEnts = 0;

	do 
	{
		if (oEL.oList[oEL.iCurEnt].iType == ENT_NONE)
		{
			oEL.oList[oEL.iCurEnt].iType			= _iType;
			oEL.oList[oEL.iCurEnt].oPos.iX			= _iX;
			oEL.oList[oEL.iCurEnt].oPos.iY			= _iY;
			oEL.oList[oEL.iCurEnt].iState			= 0;
			oEL.oList[oEL.iCurEnt].Think			= _pThink;
			
			if (_iSprID != -1)
			{
				int iWldX,iWldY;

				// Get world coordinates
				MAP_Screen2World(_iX,_iY,&iWldX,&iWldY);
				
				// Add sprite to map
				oEL.oList[oEL.iCurEnt].iSPID = MAP_iAddSprite(0,iWldX,iWldY,_iSprID,0);

				// Get sprite position structure
				oEL.oList[oEL.iCurEnt].poSP  = MAP_poGetSprite(oEL.oList[oEL.iCurEnt].iSPID);
			}
			else
			{
				oEL.oList[oEL.iCurEnt].iSPID = -1;
				oEL.oList[oEL.iCurEnt].poSP = NULL;
			}

			return(oEL.iCurEnt);
		}

		oEL.iCurEnt = (oEL.iCurEnt+1) % MAX_ENTITIES;
		iTestEnts++;

	}while (iTestEnts<MAX_ENTITIES);

	return(-1);
}
// -----------------------------------------------------------------------------
void ENT_DeleteEntity(TEntity *_poEnt)
{
	_poEnt->iType = ENT_NONE;

	if (_poEnt->iSPID != -1)
		MAP_DeleteSprite(_poEnt->iSPID);
}
// -----------------------------------------------------------------------------
TEntity *ENT_poGetEntity(int _iEntID)
{
	return( & oEL.oList[_iEntID] );
}
// -----------------------------------------------------------------------------
void ENT_Reset()
{
	// Delete all entities
	int iEnt;
	for (iEnt=0;iEnt<MAX_ENTITIES;iEnt++)
	{
		ENT_DeleteEntity( &oEL.oList[iEnt] );
 	}
}
// -----------------------------------------------------------------------------
void ENT_Update()
{
	int 		iEnt;
	TEntity*	poEnt;
 	
	for (iEnt=0,poEnt=oEL.oList;iEnt<MAX_ENTITIES;iEnt++,poEnt++)
	{
		if ((poEnt->iType != ENT_NONE) && (poEnt->Think))
			((TEntityThinkFunc)poEnt->Think)(poEnt);
 	}
}
// -----------------------------------------------------------------------------
// Entity utils
// -----------------------------------------------------------------------------
void ENT_EntityScreen2World(TEntity *_poEnt)
{
	MAP_Screen2World(_poEnt->oPos.iX,_poEnt->oPos.iY,&_poEnt->poSP->iX,&_poEnt->poSP->iY);
}

void ENT_EntityWorld2Screen(TEntity *_poEnt)
{
	MAP_World2Screen(_poEnt->poSP->iX,_poEnt->poSP->iY,&_poEnt->oPos.iX,&_poEnt->oPos.iY);
}
// -----------------------------------------------------------------------------
TEntity *ENT_poTestCollision(TEntity *_poEnt,unsigned int _uiTypesFlag)
{
	int 		iEnt;
	TEntity*	poEnt;

	if (!_poEnt)		return(NULL);
	if (!_poEnt->poSP)	return(NULL);
 
	for (iEnt=0,poEnt=oEL.oList;iEnt<MAX_ENTITIES;iEnt++,poEnt++)
	{
		if ((poEnt->iType != ENT_NONE) && (_poEnt != poEnt) && (_uiTypesFlag & poEnt->iType))
		{
			if (poEnt->poSP)
			{
				// Test against this entity
				if (MAP_bCOLSpriteSprite(_poEnt->poSP,poEnt->poSP))
					return(poEnt);
			}
			else
			{
				// Test against the world
				if (MAP_bCOLSpriteMap(_poEnt->poSP))
					return(poEnt);
			}
		}
 	}

	return(NULL);
}
// -----------------------------------------------------------------------------