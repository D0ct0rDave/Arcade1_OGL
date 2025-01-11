// -----------------------------------------------------------------------------
#ifndef EntityH
#define EntityH
// -----------------------------------------------------------------------------

// Includes
// -----------------------------------------------------------------------------
#include "Graphics/GMap.h"


// -----------------------------------------------------------------------------
// Type declarations
// -----------------------------------------------------------------------------

// Coord
typedef struct{
	int		iX;
	int		iY;
}TCoord;



typedef struct{
	
	// Status
	int					iState;
	int 				iType;
	int					iSubType;
	int					iSPID;
	int					iEnergy;
	int					iSpawnPU;	// Spawn a powerup
	int					iTime;
	
	TCoord				oTargetPos;
	TCoord				oIniPos;
	TCoord				oSpd;
	int					iAux[16];	// Array de auxiliares para entidades

	// Graphic information
	TSpritePos*			poSP;
	TCoord				oPos;		// Screen relative position
		
	// AI/Think
	void				*Think;
}TEntity;

#ifndef APIENTRY
#define APIENTRY
#endif

// Entity type
typedef void (APIENTRY *TEntityThinkFunc)(TEntity *_poEnt);



// Entity List
#define 	MAX_ENTITIES	128

typedef struct{
	TEntity oList[MAX_ENTITIES];
	int 	iCurEnt;
}TEntityList;

// -----------------------------------------------------------------------------
// Common entity definitions
// -----------------------------------------------------------------------------
#define 	ENT_NONE			0x00000000

#define 	ENT_WORLD			0x00000001
#define		ENT_PLAYER			0x00000002
#define		ENT_RESERVED0		0x00000004
#define		ENT_RESERVED1		0x00000008

#define		ENT_USER_00			0x00000010
#define		ENT_USER_01			0x00000020
#define		ENT_USER_02			0x00000040
#define		ENT_USER_03			0x00000080
#define		ENT_USER_04			0x00000100
#define		ENT_USER_05			0x00000200
#define		ENT_USER_06			0x00000400
#define		ENT_USER_07			0x00000800
#define		ENT_USER_08			0x00001000
#define		ENT_USER_09			0x00002000
#define		ENT_USER_10			0x00004000
#define		ENT_USER_11			0x00008000
#define		ENT_USER_12			0x00010000
#define		ENT_USER_13			0x00020000
#define		ENT_USER_14			0x00040000
#define		ENT_USER_15			0x00080000
#define		ENT_USER_16			0x00100000
#define		ENT_USER_17			0x00200000
#define		ENT_USER_18			0x00400000
#define		ENT_USER_19			0x00800000
#define		ENT_USER_20			0x01000000
#define		ENT_USER_21			0x02000000
#define		ENT_USER_22			0x04000000
#define		ENT_USER_23			0x08000000
#define		ENT_USER_24			0x10000000
#define		ENT_USER_25			0x20000000
#define		ENT_USER_26			0x40000000
#define		ENT_USER_27			0x80000000
// -----------------------------------------------------------------------------
// Function prototypes
// -----------------------------------------------------------------------------
void ENT_Reset();
void ENT_Update();

int  ENT_iAddEntity(int _iType,int _iX,int _iY,int _iSprID,TEntityThinkFunc _pThink);
void ENT_DeleteEntity(TEntity *_poEnt);
TEntity *ENT_poGetEntity(int _iEntID);

// Utility functions
void ENT_EntityScreen2World(TEntity *_poEnt);
void ENT_EntityWorld2Screen(TEntity *_poEnt);
TEntity *ENT_poTestCollision(TEntity *_poEnt,unsigned int _uiTypesFlag);
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
