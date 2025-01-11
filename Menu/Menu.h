// -----------------------------------------------------------------------------
#ifndef MenuH
#define MenuH

// -----------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Function prototypes
// -----------------------------------------------------------------------------
int  MENU_iLoop();

// -----------------------------------------------------------------------------
#define MENUSTATE_NONE			0
#define MENUSTATE_INIT			1
#define MENUSTATE_FINISH		2
#define MENUSTATE_TITLE			3
#define MENUSTATE_TITLE2		4
#define MENUSTATE_SCORES		5
#define MENUSTATE_PLAYINGGAME	6
#define MENUSTATE_PLAYINGDEMO	7
#define MENUSTATE_RECORDDEMO	8

extern unsigned int		guiMenuState;
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------