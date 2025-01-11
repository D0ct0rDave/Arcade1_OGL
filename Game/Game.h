// -----------------------------------------------------------------------------
#ifndef GameH
#define GameH
// -----------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------
#include "Levels/Entities/Entity.h"


// -----------------------------------------------------------------------------
// Function prototypes
// -----------------------------------------------------------------------------
int GAME_iLoop();




extern unsigned int		guiCurSectSeed;
// -----------------------------------------------------------------------------
#define GAMESTATE_NONE				0
#define GAMESTATE_INIT				1
#define GAMESTATE_FINISH			2
#define GAMESTATE_READY				3
#define GAMESTATE_READYMESSAGE		4
#define GAMESTATE_RUN				5
#define GAMESTATE_DEAD				6
#define GAMESTATE_GAMEOVER			7

extern unsigned int		guiGameState;
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------