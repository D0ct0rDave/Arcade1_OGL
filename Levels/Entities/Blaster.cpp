#include "Player.h"
#include "Input/input.h"

#include "Blaster.h"
#include "Explosion.h"
#include "CommonEntity.h"
// -----------------------------------------------------------------------------
// Global definitions
// -----------------------------------------------------------------------------

// Hay que llegar a un acuerdo para decidir que flags seran reutilizados para enemigos en los diferentes mapas, cuales
// para armas de enemigos, y cuales para otro tipo de items que no son necesariamente enemigos,

#define			ENT_BLAST_TYPE				ENT_USER_20
#define			ENT_HYPERBLAST_TYPE			ENT_USER_21
#define			ENT_LASER_TYPE				ENT_USER_22
#define			ENT_BFG_TYPE				ENT_USER_23
// -----------------------------------------------------------------------------
#define			SPRITE_BLASTER_ID			66
#define			SPRITE_HYPERBLASTER_ID		65
#define			SPRITE_LASER_ID				67
#define			SPRITE_BFG_ID				68
#define			SPRITE_MINIBLAST0_ID		69
#define			SPRITE_MINIBLAST1_ID		70
#define			SPRITE_MINIBLAST2_ID		71
// -----------------------------------------------------------------------------
#define			BLASTER_MOVSTEP				8
#define			HYPERBLASTER_MOVSTEP		10
#define			LASER_MOVSTEP				5
#define			BFG_MOVSTEP					2
#define			MINIBLASTSTEP				5

#define			ENEMY_ENTITIES				(ENT_USER_00 | ENT_USER_01 | ENT_USER_02 | ENT_USER_03 | ENT_USER_04 | ENT_USER_05 | ENT_USER_06 | ENT_USER_07)
#define			ENEMYWEAP_ENTITIES			(ENT_USER_08 | ENT_USER_09 | ENT_USER_10 | ENT_USER_11)
/*******************************************************
Generic blasting functions
--------------------------------------------------------
*******************************************************/
void ENT_GenericBlastThink(TEntity *_poEnt,int _iXMovStep,int _iYMovStep,int _iDeleteAfterTouch)
{
	TEntity *poCollEnt;
	
	_poEnt->oPos.iX    += _iXMovStep;
	_poEnt->oPos.iY    += _iYMovStep;

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
  	poCollEnt = ENT_poTestCollision(_poEnt,ENT_WORLD | ENEMY_ENTITIES);

	if (poCollEnt)
	{		
		if (poCollEnt->iType == ENT_WORLD)
		{
			// Add SFX
			ENT_AddExplosion(_poEnt->oPos.iX,_poEnt->oPos.iY,ENT_SMALL_EXPLOSION);

			// Delete blast
			ENT_DeleteEntity(_poEnt);

			return;
		}
		else
		{
			// Add SFX

			// Delete this ent only if required
			if (_iDeleteAfterTouch)
				ENT_DeleteEntity(_poEnt);

			poCollEnt->iEnergy--;

			if (poCollEnt->iEnergy<=0)
			{
				// Nos hemos cargado una nave
				ENT_AddExplosion(poCollEnt->oPos.iX,poCollEnt->oPos.iY,ENT_BIG_EXPLOSION);
				
				if (poCollEnt->iSpawnPU)
				{
					// Add powerup
					ENT_AddCommonEntity(poCollEnt->oPos.iX,poCollEnt->oPos.iY,ENT_USER_25,75 + poCollEnt->iSpawnPU);
				}
				
				// delete enemy
				ENT_DeleteEntity(poCollEnt);
			}
		}

	}

	// Update graphic part
	ENT_EntityScreen2World(_poEnt);
}

/*******************************************************
Blaster functions
--------------------------------------------------------
*******************************************************/
void ENT_BlasterThink(TEntity *_poEnt)
{		
	ENT_GenericBlastThink(_poEnt,BLASTER_MOVSTEP,0,1);
}
// -----------------------------------------------------------------------------
void ENT_AddBlaster(int _iScrX,int _iScrY)
{
	ENT_iAddEntity(ENT_BLAST_TYPE,_iScrX,_iScrY,SPRITE_BLASTER_ID,ENT_BlasterThink);
}

/*******************************************************
HyperBlaster functions
--------------------------------------------------------
*******************************************************/
void ENT_HyperBlasterThink(TEntity *_poEnt)
{	
	ENT_GenericBlastThink(_poEnt,HYPERBLASTER_MOVSTEP,0,1);
}
// -----------------------------------------------------------------------------
void ENT_AddHyperBlaster(int _iScrX,int _iScrY)
{
	ENT_iAddEntity(ENT_HYPERBLAST_TYPE,_iScrX,_iScrY,SPRITE_HYPERBLASTER_ID,ENT_BlasterThink);
}
/*******************************************************
Laser functions
--------------------------------------------------------
*******************************************************/
void ENT_LaserThink(TEntity *_poEnt)
{	
	ENT_GenericBlastThink(_poEnt,LASER_MOVSTEP,0,0);
}
// -----------------------------------------------------------------------------
void ENT_AddLaser(int _iScrX,int _iScrY)
{
	ENT_iAddEntity(ENT_LASER_TYPE,_iScrX,_iScrY,SPRITE_LASER_ID,ENT_LaserThink);
}
/*******************************************************
Mini BFG blast functions
--------------------------------------------------------
*******************************************************/
// -----------------------------------------------------------------------------
void ENT_MiniBlast0Think(TEntity *_poEnt)
{		
	ENT_GenericBlastThink(_poEnt,MINIBLASTSTEP,0,1);
}
void ENT_MiniBlast1Think(TEntity *_poEnt)
{		
	ENT_GenericBlastThink(_poEnt,-MINIBLASTSTEP,0,1);
}
void ENT_MiniBlast2Think(TEntity *_poEnt)
{		
	ENT_GenericBlastThink(_poEnt,0,MINIBLASTSTEP,1);
}
void ENT_MiniBlast3Think(TEntity *_poEnt)
{		
	ENT_GenericBlastThink(_poEnt,0,-MINIBLASTSTEP,1);
}

void ENT_MiniBlast4Think(TEntity *_poEnt)
{		
	ENT_GenericBlastThink(_poEnt,MINIBLASTSTEP,MINIBLASTSTEP,1);
}
void ENT_MiniBlast5Think(TEntity *_poEnt)
{		
	ENT_GenericBlastThink(_poEnt,-MINIBLASTSTEP,MINIBLASTSTEP,1);
}
void ENT_MiniBlast6Think(TEntity *_poEnt)
{		
	ENT_GenericBlastThink(_poEnt,MINIBLASTSTEP,-MINIBLASTSTEP,1);
}
void ENT_MiniBlast7Think(TEntity *_poEnt)
{		
	ENT_GenericBlastThink(_poEnt,-MINIBLASTSTEP,-MINIBLASTSTEP,1);
}


void ENT_AddMiniBlasts(int _iScrX,int _iScrY)
{
	ENT_iAddEntity(ENT_BLAST_TYPE,_iScrX,_iScrY,SPRITE_MINIBLAST0_ID,ENT_MiniBlast0Think);
	ENT_iAddEntity(ENT_BLAST_TYPE,_iScrX,_iScrY,SPRITE_MINIBLAST0_ID,ENT_MiniBlast1Think);
	ENT_iAddEntity(ENT_BLAST_TYPE,_iScrX,_iScrY,SPRITE_MINIBLAST0_ID,ENT_MiniBlast2Think);
	ENT_iAddEntity(ENT_BLAST_TYPE,_iScrX,_iScrY,SPRITE_MINIBLAST0_ID,ENT_MiniBlast3Think);
	ENT_iAddEntity(ENT_BLAST_TYPE,_iScrX,_iScrY,SPRITE_MINIBLAST2_ID,ENT_MiniBlast4Think);
	ENT_iAddEntity(ENT_BLAST_TYPE,_iScrX,_iScrY,SPRITE_MINIBLAST1_ID,ENT_MiniBlast5Think);
	ENT_iAddEntity(ENT_BLAST_TYPE,_iScrX,_iScrY,SPRITE_MINIBLAST1_ID,ENT_MiniBlast6Think);
	ENT_iAddEntity(ENT_BLAST_TYPE,_iScrX,_iScrY,SPRITE_MINIBLAST2_ID,ENT_MiniBlast7Think);
}
/*******************************************************
BFG functions
--------------------------------------------------------
*******************************************************/
void ENT_BFGThink(TEntity *_poEnt)
{	
	TEntity *poCollEnt;

	_poEnt->oPos.iX += BFG_MOVSTEP;

	if (_poEnt->oPos.iX > SCREENWIDTH)
	{
		ENT_DeleteEntity(_poEnt);
		return;
	}
	
	// Control collision against map
	poCollEnt = ENT_poTestCollision(_poEnt,ENT_WORLD | ENEMY_ENTITIES);

	if (poCollEnt)
	{
		if (poCollEnt->iType == ENT_WORLD)
		{
			// Add SFX

			// Delete entity
			ENT_DeleteEntity(_poEnt);
		}
		else
		{
			// Delete this blast
			// ENT_DeleteEntity(_poEnt);

			// Create secondary blasts
			ENT_AddMiniBlasts(_poEnt->oPos.iX,_poEnt->oPos.iY);

			// Add SFX

			// Nos hemos cargado una nave
			poCollEnt->iEnergy -= 5;

			if (poCollEnt->iEnergy<=0)
			{
				ENT_DeleteEntity(poCollEnt);
			}
		}

		return;
	}
	
	// Update graphic part
	ENT_EntityScreen2World(_poEnt);
}
// -----------------------------------------------------------------------------
void ENT_AddBFG(int _iScrX,int _iScrY)
{
	ENT_iAddEntity(ENT_BFG_TYPE,_iScrX,_iScrY,SPRITE_BFG_ID,ENT_BFGThink);
}

/*******************************************************
Add shot
--------------------------------------------------------
*******************************************************/
void ENT_AddShot(int _iType,int _iScrX,int _iScrY)
{
	switch (_iType)
	{
		case 0:
		ENT_AddBlaster(_iScrX,_iScrY);
		break;

		case 1:
		ENT_AddHyperBlaster(_iScrX,_iScrY);
		break;
		
		case 2:
		ENT_AddLaser(_iScrX,_iScrY);
		break;

		case 3:
		ENT_AddBFG(_iScrX,_iScrY);
		break;
	}
}
// -----------------------------------------------------------------------------