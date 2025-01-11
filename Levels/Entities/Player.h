// -----------------------------------------------------------------------------
#ifndef PlayerH
#define PlayerH
// -----------------------------------------------------------------------------

#include "Entity.h"

void ENT_PlayerInit();
void ENT_PlayerThink(TEntity *_poEnt);
void ENT_PlayerHandleCollisions();


// -----------------------------------------------------------------------------
// Player variables
// -----------------------------------------------------------------------------
typedef struct{

	int iFWOShot;		// Frames without shooting
	int	iAmmo;			// Weapon ammonition
	int iHave;			// Has the player this weapon ?

}TWeaponFeatures;
#define MAX_WEAPONS	4

extern TWeaponFeatures	goWeapFeat[MAX_WEAPONS];
extern int				giPlWeap;
extern int				giPlScore;
extern int				giPlCredit;
extern TEntity*			gpoPlayerEnt;

// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
