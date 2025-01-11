// -----------------------------------------------------------------------------
#include "L0_Bouncer.h"
#include "L0_CommonBlast.h"
#include "Levels/Entities/Entity.h"
#include "Utils/FMath.h"
#include "Graphics/GCam.h"

#include <stdlib.h>
// -----------------------------------------------------------------------------

#define		L0ENT_BOUNCER		ENT_USER_03
#define		SPRITE_ID			90
#define		SPRITE_ID_BLAST		100

int giFirstTime = 1;
int giGravityTable[256];
int giMaxFrames;
// -----------------------------------------------------------------------------
#define ENEMY_STEP		2

int ACC				= 1;
int RETARDOFACTOR	= 10;
int SPDFACT			= 30;

int Level0Ent_iGetMapLimit(int _iUpDown)
{
	int		iXTile = GCamMaxX / 64;
	int		iTX    = gpoMap->poMSecs[giCurSec].usTX;
	int		iScrY;
	int		i;

	if (_iUpDown)
	{
		// Por la parte de arriba
		for (i=0;i<7;i++)
		{
			if (gpoMap->poMSecs[giCurSec].pucOvData[i*iTX + iXTile] == 0)
			{
				iScrY = i*64 - 32;
				break;
			}
		}
	}
	else
	{
		// Por la parte de abajo
		for (i=7;i>=0;i--)
		{
			if (gpoMap->poMSecs[giCurSec].pucOvData[i*iTX + iXTile] == 0)
			{
				iScrY = i*64 + 32;
				break;
			}
		}
	}

	return(iScrY);
}
// -----------------------------------------------------------------------------
void Level0Ent_BouncerThink(TEntity *_poEnt)
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


	switch (_poEnt->iState)		
	{
		// Standing
		case 0:		
		{
			_poEnt->iTime++;

			if (_poEnt->iTime == 60)	// tiempo de reposo
			{
				_poEnt->iTime  = 0;
				_poEnt->iState = 1;				

				// Frames que dura el salto
				_poEnt->oSpd.iY		  = SPDFACT + (rand() % SPDFACT);
				_poEnt->oTargetPos.iY = _poEnt->poSP->iY;
				_poEnt->oIniPos.iY	  = _poEnt->poSP->iY;
			}	
		}
		break;
		
		// Jumping
		case 1:
		{
			int iTime    = _poEnt->iTime;
			int iNextTime= iTime+1;

			// v = v0 + at
			int iSpd		= _poEnt->oSpd.iY - ACC*iTime;
			int iNextSpd	= _poEnt->oSpd.iY - ACC*iNextTime;
			
			int iSpdPortion = _poEnt->oSpd.iY*iTime;
			int iAccPortion = (ACC*iTime*iTime)/2;
	
			int iOfs = (iSpdPortion - iAccPortion) / RETARDOFACTOR;
			
			// Hacia arriba
			if (_poEnt->iSubType == 0)
			{
				// x = x0 + v0*T + 1/2*a*t^2
				_poEnt->poSP->iY = _poEnt->oIniPos.iY - iOfs;
			}

			// Hacia abajo
			else
			{
				_poEnt->poSP->iY = _poEnt->oIniPos.iY + iOfs;
	 		}

			if ((iSpd>=0) && (iNextSpd < 0))
			{
				// Va a haber un cambio de sentido
				if (_poEnt->iSubType == 0)
				{
					// salta hacia arriba
					if (_poEnt->oPos.iY <= (SCREENHHEIGHT + 50))
					{
						// Cambiamos de rumbo
						_poEnt->iSubType		= 1;						
						_poEnt->oTargetPos.iY	= (_poEnt->oTargetPos.iX >> 16);
						_poEnt->oIniPos.iY      = _poEnt->poSP->iY - iOfs;
					}
				}
				else
				{
					// salta hacia arriba
					if (_poEnt->oPos.iY+gpoMap->poTiles[SPRITE_ID].usTY >= (SCREENHHEIGHT - 50))
					{
						// Cambiamos de rumbo
						_poEnt->iSubType		= 0;
						_poEnt->oTargetPos.iY	= _poEnt->oTargetPos.iX & 0x0000ffff;
						_poEnt->oIniPos.iY      = _poEnt->poSP->iY + iOfs;
					}
				}
			}


			if (_poEnt->iSubType == 0)
			{	
				// Solo miramos los limites
				if (_poEnt->poSP->iY > _poEnt->oTargetPos.iY)
				{
					// Hemos vuelto
					_poEnt->iState	 = 0;
					_poEnt->iTime	 = 0;
					_poEnt->poSP->iY = _poEnt->oTargetPos.iY;
				}
			}
			else
			{
				if (_poEnt->poSP->iY < _poEnt->oTargetPos.iY)
				{
					// Hemos vuelto
					_poEnt->iState	= 0;
					_poEnt->iTime   = 0;
					_poEnt->poSP->iY= _poEnt->oTargetPos.iY;
				}
			}

			_poEnt->iTime++;
		}
		break;
	}


	if ((GFRAMENUM % 40) == 0)
	{
		if ((rand() % 9) == 0)
			Level0Ent_AddCommonBlast(_poEnt->oPos.iX,_poEnt->oPos.iY,SPRITE_ID_BLAST,0);
	}

	ENT_EntityWorld2Screen(_poEnt);
}
// -----------------------------------------------------------------------------
void Level0Ent_AddBouncer(int _iUpDown)
{
	int		iEntID;
	TEntity *poEnt;

	int		i;
	int		iSprID;
	int		iScrY;
	int		iRand  = rand() % 100;
			
	iScrY = Level0Ent_iGetMapLimit(_iUpDown);

	iEntID = ENT_iAddEntity (L0ENT_BOUNCER,SCREENWIDTH,iScrY,SPRITE_ID,Level0Ent_BouncerThink);
	poEnt  = ENT_poGetEntity(iEntID);

	poEnt->iEnergy  = 8;
	poEnt->iTime    = 0;	
	poEnt->iState   = 0;
	poEnt->iSubType = _iUpDown;
	poEnt->iSpawnPU = (iRand < 10)?(iRand<5)?2:1:0;
	
	// Store map limits
	poEnt->oTargetPos.iX = Level0Ent_iGetMapLimit(0) | (Level0Ent_iGetMapLimit(1) << 16); 
	
	poEnt->oIniPos.iX = poEnt->poSP->iX;
	poEnt->oIniPos.iY = poEnt->poSP->iY;
}
// -----------------------------------------------------------------------------
