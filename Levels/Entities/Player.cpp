#include <stdlib.h>

#include "Player.h"
#include "Game/Game.h"
#include "Input/input.h"
#include "Blaster.h"

#define MOV_STEP				3

#define	SPRITE_ID_PLAYER		64
#define PLAYER_SPRITE_WIDTH		64
#define PLAYER_SPRITE_HEIGHT	64

// -----------------------------------------------------------------------------
TWeaponFeatures goWeapFeat[MAX_WEAPONS] = 
{	
	{20,-1, 1},		// Blaster
	{ 5,-1, 1},		// Hyperblaster
	{30,-1, 1},		// Laser
	{60,-1, 1},		// BFG
};

int			giPlWeap   = 0;
int			giPlScore  = 0;
int			giPlCredit = 0;

TEntity*	gpoPlayerEnt = NULL;
int			giDisableAI  = 0;

// -----------------------------------------------------------------------------
// Player entity think
// -----------------------------------------------------------------------------
void ENT_PlayerInit()
{
	int iPlEnt   = ENT_iAddEntity(ENT_PLAYER,0,0,SPRITE_ID_PLAYER,ENT_PlayerThink);
	gpoPlayerEnt = ENT_poGetEntity(iPlEnt);
	
	// Player sprite ID
	gpoPlayerEnt->iState = 2;
}

// -----------------------------------------------------------------------------
// Player entity think
// -----------------------------------------------------------------------------
static int STATE_TIME = 4;	// #define STATE_TIME (SCREENFPS>>2)

void ENT_PlayerThink(TEntity *_poEnt)
{
	// Handle input
	// ------------
	TCoord oSpd;	
	oSpd.iX = oSpd.iY = 0;

	if (oInput.uiPressed & CMD_UP)
	{		
		oSpd.iY -= MOV_STEP;		
	}

	if (oInput.uiPressed & CMD_DOWN)
	{		
		oSpd.iY  += MOV_STEP;
	}
	
	if (oInput.uiPressed & CMD_LEFT)
	{		
		oSpd.iX  -= MOV_STEP;		
	}
	
	if (oInput.uiPressed & CMD_RIGHT)	
	{
		oSpd.iX  += MOV_STEP;		
	}
	
	// Set new player position
	// -----------------------
	_poEnt->oPos.iX += oSpd.iX;
	_poEnt->oPos.iY += oSpd.iY;


	if (_poEnt->oPos.iY < 0)
		_poEnt->oPos.iY = 0;
	else if (_poEnt->oPos.iY + PLAYER_SPRITE_HEIGHT > SCREENHEIGHT)
	{
		// El player sale por abajo
		_poEnt->oPos.iY = SCREENHEIGHT - PLAYER_SPRITE_HEIGHT;
	}
	

	if (_poEnt->oPos.iX < 0)
		_poEnt->oPos.iX = 0;
	else if (_poEnt->oPos.iX + PLAYER_SPRITE_WIDTH > SCREENWIDTH)
	{
		// El player sale por abajo
		_poEnt->oPos.iX = SCREENWIDTH - PLAYER_SPRITE_WIDTH;
	}

		
	
	// Check fire button
	// -----------------
	if (oInput.uiPressed & CMD_ABUTT)
	{
		// Only allow 1 shot every 4 frames
		#define PLAYERFRAMESWITHOUTSHOTTING	32
		if ((GFRAMENUM % goWeapFeat[giPlWeap].iFWOShot) == 0)
		{
  			ENT_AddShot(giPlWeap,_poEnt->oPos.iX + PLAYER_SPRITE_WIDTH,_poEnt->oPos.iY + (PLAYER_SPRITE_HEIGHT>>1));
		}
	}

	// Check next weapon button
	// ------------------------
	if (oInput.uiJustPressed & CMD_CBUTT)
	{
		do{
			giPlWeap++;
			if (giPlWeap==MAX_WEAPONS) giPlWeap = 0;
		}while ((! goWeapFeat[giPlWeap].iHave) || (goWeapFeat[giPlWeap].iAmmo == 0));
	}

	// Check prev weapon button
	// ------------------------
	if (oInput.uiJustPressed & CMD_DBUTT)
	{
		do{
			giPlWeap--;
			if (giPlWeap<0) giPlWeap = MAX_WEAPONS-1; 

		}while ((! goWeapFeat[giPlWeap].iHave) || (goWeapFeat[giPlWeap].iAmmo == 0));
	}



	// Control player sprite
	// ---------------------
	_poEnt->iTime++;
	
	switch(_poEnt->iState)
	{
		case 0:
		{
			if (_poEnt->iTime > STATE_TIME)
			{
				if (oSpd.iY<=0)
				{	
					_poEnt->iState= 1;
					_poEnt->iTime = 0;
				}
			}
		}
		break;

		case 1:
		{
			if (_poEnt->iTime > STATE_TIME)
			{	

				if (oSpd.iY>0)
				{
					_poEnt->iState= 0;
					_poEnt->iTime = 0;
				}
				else
				{
					_poEnt->iState= 2;
					_poEnt->iTime = 0;
				}
			}
		}
		break;

		case 2:
		{
			if (_poEnt->iTime > STATE_TIME)
			{	

				if (oSpd.iY>0)
				{
					_poEnt->iState= 1;
					_poEnt->iTime = 0;
				}
		   else if (oSpd.iY<0)
				{
					_poEnt->iState= 3;
					_poEnt->iTime = 0;
				}
			}
		}
		break;

		case 3:
		{
			if (_poEnt->iTime > STATE_TIME)
			{	

				if (oSpd.iY>=0)
				{
					_poEnt->iState= 2;
					_poEnt->iTime = 0;
				}
				else
				{
					_poEnt->iState= 4;
					_poEnt->iTime = 0;
				}
			}
		}
		break;

		case 4:
		{
			if (_poEnt->iTime > STATE_TIME)
			{	

				if (oSpd.iY>=0)
				{
					_poEnt->iState= 3;
					_poEnt->iTime = 0;
				}
			}
		}
		break;
	}
	
	gpoMap->poTiles[SPRITE_ID_PLAYER].ucFrTab[0] = _poEnt->iState;

	// Update graphic part
	// -------------------
	ENT_EntityScreen2World(_poEnt);
}
// -----------------------------------------------------------------------------
#define			ENEMY_ENTITIES				(ENT_USER_00 | ENT_USER_01 | ENT_USER_02 | ENT_USER_03 | ENT_USER_04 | ENT_USER_05 | ENT_USER_06 | ENT_USER_07)
#define			ENEMYWEAP_ENTITIES			(ENT_USER_08 | ENT_USER_09 | ENT_USER_10 | ENT_USER_11)
#define			ENT_POWERUP 				ENT_USER_25

void ENT_PlayerHandleCollisions()
{
	TEntity *poCollEnt;

	// -------------------------------------------------------------
	// Test collision
	
	// Control collision against map
	poCollEnt = ENT_poTestCollision(gpoPlayerEnt,ENT_WORLD	
												| ENT_POWERUP
												| ENEMY_ENTITIES
												| ENEMYWEAP_ENTITIES
												);

	if (poCollEnt)
	{
		// Nos hemos chocado contra el propio mapa
		if (poCollEnt->iType == ENT_WORLD)
		{
			gpoPlayerEnt->iEnergy = 0;
			return;
		}


		// Nos hemos chocado contra un powerup
		if (poCollEnt->iType == ENT_POWERUP )
		{
			// Add Sfx
			int iCredits = (poCollEnt->iSubType==76)?10:50;

			giPlCredit += iCredits;

			// Delete powerup
			ENT_DeleteEntity(poCollEnt);
			return;
		}

		
		// Nos hemos chocado contra uno de los enemigos		
		if (giDisableAI) return;

		if (poCollEnt->iType & ENEMY_ENTITIES)
		{
			// Add SFX
			
			// Delete blast
			poCollEnt->iEnergy = 0;
			ENT_DeleteEntity(poCollEnt);

			gpoPlayerEnt->iEnergy -= 50;

			if (gpoPlayerEnt->iEnergy <0)
				gpoPlayerEnt->iEnergy = 0;
			return;
		}

		// Nos hemos chocado contra un projectil enemigo
		if (poCollEnt->iType & ENEMYWEAP_ENTITIES)
		{
			// Add SFX
			
			
			// Delete blast
			ENT_DeleteEntity(poCollEnt);

			gpoPlayerEnt->iEnergy -= 15;

			if (gpoPlayerEnt->iEnergy<0)
				gpoPlayerEnt->iEnergy = 0;
			return;			
		}
	}
}