// -----------------------------------------------------------------------------
#ifndef GMapH
#define GMapH

// -----------------------------------------------------------------------------
#ifndef BOOL
#define BOOL char
#endif

// -----------------------------------------------------------------------------
// Include files
// -----------------------------------------------------------------------------
#include "graphics.h"

// -----------------------------------------------------------------------------
#define START_AT_END_H		0x01
#define START_AT_END_V		0x02

#define MOVEMENTMODE_LEFT	0
#define MOVEMENTMODE_UP		1
#define MOVEMENTMODE_DOWN	2
#define MOVEMENTMODE_RIGHT	3

typedef struct{

	unsigned char*	pucBkData;		// Background data
	unsigned char*	pucOvData;		// Overlay data
	
	unsigned short	usTX;			// Width of the map in tiles 
	unsigned short	usTY;			// Height in tiles of the map
								
	char			cHSpd;			// Horizontal speed	(in pixels/frame)
	char			cVSpd;			// Vertical speed	(in pixels/frame)
	unsigned char	ucHFSkip;		// Frames to skip in horizontal update
	unsigned char	ucVFSkip;		// Frames to skip in vertical update

	unsigned char	ucFlag;			// Flags
	unsigned char	ucMovMode;		// Movement mode (defines player orientation)

}TMapSection;


// -----------------------------------------------------------------------------
typedef struct TSprite{
	
	unsigned char	ucBMP;			// Bitmap Idx	
	unsigned short	usX;			// Starting X coord in Resource
	unsigned short	usY;			// Starting Y coord in Resource
	unsigned short	usTX;			// Sprite Width
	unsigned short	usTY;			// Sprite Height
	unsigned char	ucFrTab[16];	// (frames are horizontal)
	unsigned char	ucFR;			// Number of animation frames
	unsigned short  usATime;		// Animation time (in frames)
	unsigned char	bLoop;
	unsigned char   ucBlend;		// Blend mode to use with this sprite
	char			cCol;			// -1 current sprite frame, framenum other

}TSprite;

// -----------------------------------------------------------------------------
typedef struct TMegaSprite{
	unsigned char	*pucData;
	unsigned short	usTX;			// Width in tiles
	unsigned short	usTY;			// Height in tiles
	TSprite*		poTiles;		// Sprite table

}TMegaSprite;
// -----------------------------------------------------------------------------
// Sprite position structure
// -----------------------------------------------------------------------------
#define SPRITEFLAG_MEGASPRITE		0x01
#define SPRITEFLAG_CHECK_COLLISION	0x02

typedef struct{
	int				iX;			// X coordinate inside the section
	int				iY;			// Y Coordinate inside this section
	unsigned char	ucID;		// Sprite/Megasprite
	unsigned char	ucFlag;		// Sprite flags
	unsigned char	ucEnabled;	// Enabled
}TSpritePos;
// -----------------------------------------------------------------------------
typedef struct TMap{

	unsigned int	uiNumSections;	// Number of sections this map
	TMapSection*	poMSecs;		// Section table

	unsigned short	usNumMSprs;		// Mega Sprite table
	TMegaSprite*	poMSprites;

	TSprite*		poTiles;		// Tile list
	BMPHandle*		poBMPs;			// Bitmap table
	unsigned char*	pucCTiles;		// Collidable tiles

}TMap;
// -----------------------------------------------------------------------------
// Function prototypes
// -----------------------------------------------------------------------------
void MAP_Init(TMap *_poMap);
void MAP_Finish();
void MAP_SetCurrentSection(int _iMapSection);
int  MAP_iGetSection();
void MAP_Update();
void MAP_Render();
int  MAP_iAddSprite(unsigned int _ucSec,int _iX,int _iY,unsigned char _ucID,unsigned char _ucFlag);
void MAP_DeleteSprite(int _iID);
TSpritePos *MAP_poGetSprite(int _iID);
// -----------------------------------------------------------------------------
BOOL MAP_bCOLSpriteMap   (TSpritePos *_poSpr);
BOOL MAP_bCOLSpriteSprite(TSpritePos *_poSrc,TSpritePos *_poDst);

// -----------------------------------------------------------------------------
void MAP_Screen2World(int _iScrX,int _iScrY,int *_iWldX,int *_iWldY);
void MAP_World2Screen(int _iWldX,int _iWldY,int *_iScrX,int *_iScrY);


// -----------------------------------------------------------------------------
// The map itself
// -----------------------------------------------------------------------------
extern TMap*	gpoMap;
extern int		giCurSec;
extern int		giMapFrame;
extern int		giCurSecFrame;
extern int		giStopMapScroll;

// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
