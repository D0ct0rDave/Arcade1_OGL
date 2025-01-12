// -----------------------------------------------------------------------------
#include <stdlib.h>
#include <string.h>

#include "GMap.h"
#include "graphics.h"
#include "GCam.h"
// -----------------------------------------------------------------------------
#define MAX_MAP_SECTS		512
#define MAX_SPRITES			256
#define TILEXSIZE 			64
#define TILEYSIZE 			64

#define SCREENXTILES		(SCREENWIDTH / TILEXSIZE)
#define SCREENYTILES		(SCREENHEIGHT/ TILEYSIZE)
#define SCREENXHTILES		(SCREENXTILES/2)
#define SCREENYHTILES		(SCREENYTILES/2)
// -----------------------------------------------------------------------------
char   szChivato2[64];

// -----------------------------------------------------------------------------
TMap*	gpoMap;		// The map itself


// Runtime fields
int		giCurSec;			// Current map section
int		giFrame;
int		giCurSecHFact;
int		giCurSecVFact;
int		giCurSecHSkip;
int		giCurSecVSkip;
int		giFramesToYLim;
int		giFramesToXLim;
int		giCurSecFrame;
int		giMapFrame;


typedef struct{	
	unsigned char   bUpd;		// Update this sprite ?
	unsigned char	ucFrame;	// Current frame (computed after updating Animation Time)
	unsigned short	usSFPAF;	// Screen frames per animation frame
	unsigned short	usSF;		// Current screen frame for this sprite

}TRTSprite;

TRTSprite		goRTST[256];
TSpritePos		goMS[MAX_SPRITES];
TRTSprite		goMSRTST[MAX_SPRITES];
int				iCurSP = 0;
int				giMapSectFrames[MAX_MAP_SECTS];

// -----------------------------------------------------------------------------
// Debugging fields
int		giStopMapScroll = 0;

/*******************************************************
--------------------------------------------------------
Renders the current map section background
--------------------------------------------------------
*******************************************************/


void DrawMap_Background(TMap *poMap, int iCX,int iCY, int _iSect)
{	
	int iIX,iIY,iFX,iFY;
 	int cX,cY;

	if (_iSect>= gpoMap->uiNumSections) return;

	iIX   = ((iCX - SCREENHWIDTH     ) / TILEXSIZE) - 1;
	iIY   = ((iCY - SCREENHHEIGHT    ) / TILEYSIZE) - 1;
	iFX   = ((iCX + SCREENHWIDTH  - 1) / TILEXSIZE) + 1;
	iFY   = ((iCY + SCREENHHEIGHT - 1) / TILEYSIZE) + 1;

	// Check Boundaries
	if (iIX < 0) iIX = 0;
	if (iIX >= gpoMap->poMSecs[_iSect].usTX) iIX = gpoMap->poMSecs[_iSect].usTX-1;
	if (iIY < 0) iIY = 0;
	if (iIY >= gpoMap->poMSecs[_iSect].usTY) iFY = gpoMap->poMSecs[_iSect].usTY-1;
	if (iFX < 0) iFX = 0;
	if (iFX >= gpoMap->poMSecs[_iSect].usTX) iFX = gpoMap->poMSecs[_iSect].usTX-1;
	if (iFY < 0) iFY = 0;
	if (iFY >= gpoMap->poMSecs[_iSect].usTY) iFY = gpoMap->poMSecs[_iSect].usTY-1;

	// Draw tiles
	for (cY=iIY;cY<=iFY;cY++)
		for (cX=iIX;cX<=iFX;cX++)
		{
			int iID				= cY*gpoMap->poMSecs[_iSect].usTX + cX;		// Index to the tile in the data array
			int iSprID			= gpoMap->poMSecs[_iSect].pucBkData[iID];	// Get the sprite we want to draw
			if (iSprID!=0)
			{

				TSprite* poSprite	= &gpoMap->poTiles[iSprID];				// Get the sprite from the table
				int iAnimFrame		= goRTST[iSprID].ucFrame;				// Get the current animation frame for this sprite				
				int iBmpFrame		= poSprite->ucFrTab[ iAnimFrame ];		// Access the real frame on the bitmap

				GPutSprite(	gpoMap->poBMPs[ poSprite->ucBMP ],
									poSprite->usX + iBmpFrame*poSprite->usTX,
									poSprite->usY,
									TILEXSIZE,
									TILEYSIZE,
									cX*TILEXSIZE - iCX + SCREENHWIDTH,
									cY*TILEYSIZE - iCY + SCREENHHEIGHT,
									poSprite->ucBlend);
			}
		}
}
/*******************************************************
--------------------------------------------------------
Renders the current map section overlay
--------------------------------------------------------
*******************************************************/
void DrawMap_Overlay(TMap *poMap, int iCX,int iCY,int _iSect)
{	
	int iIX,iIY,iFX,iFY;
 	int cX,cY;

	if (_iSect>= gpoMap->uiNumSections) return;

	iIX   = ((iCX - SCREENHWIDTH     ) / TILEXSIZE) - 1;
	iIY   = ((iCY - SCREENHHEIGHT    ) / TILEYSIZE) - 1;
	iFX   = ((iCX + SCREENHWIDTH  - 1) / TILEXSIZE) + 1;
	iFY   = ((iCY + SCREENHHEIGHT - 1) / TILEYSIZE) + 1;

	// Check Boundaries
	if (iIX < 0) iIX = 0;
	if (iIX >= gpoMap->poMSecs[_iSect].usTX) iIX = gpoMap->poMSecs[_iSect].usTX-1;
	if (iIY < 0) iIY = 0;
	if (iIY >= gpoMap->poMSecs[_iSect].usTY) iFY = gpoMap->poMSecs[_iSect].usTY-1;
	if (iFX < 0) iFX = 0;
	if (iFX >= gpoMap->poMSecs[_iSect].usTX) iFX = gpoMap->poMSecs[_iSect].usTX-1;
	if (iFY < 0) iFY = 0;
	if (iFY >= gpoMap->poMSecs[_iSect].usTY) iFY = gpoMap->poMSecs[_iSect].usTY-1;

	// Draw tiles
	for (cY=iIY;cY<=iFY;cY++)
		for (cX=iIX;cX<=iFX;cX++)
		{
			int iID				= cY*gpoMap->poMSecs[_iSect].usTX + cX;	// Index to the tile in the data array
			int iSprID			= gpoMap->poMSecs[_iSect].pucOvData[iID];	// Get the sprite we want to draw

			if (iSprID!=0)
			{
				TSprite* poSprite	= &gpoMap->poTiles[iSprID];		// Get the sprite from the table
				int iAnimFrame		= goRTST[iSprID].ucFrame;			// Get the current animation frame for this sprite				
				int iBmpFrame		= poSprite->ucFrTab[ iAnimFrame ];	// Access the real frame on the bitmap

				GPutSprite(	gpoMap->poBMPs[ poSprite->ucBMP ],
									poSprite->usX + iBmpFrame*poSprite->usTX,
									poSprite->usY,
									TILEXSIZE,
									TILEYSIZE,
									cX*TILEXSIZE - iCX + SCREENHWIDTH,
									cY*TILEYSIZE - iCY + SCREENHHEIGHT,
									poSprite->ucBlend);
			}
		}
}
/*******************************************************
--------------------------------------------------------
Compute the map frames at the beginning of each Section
--------------------------------------------------------
*******************************************************/
void MAP_ComputeSectionFrames(TMap* _poMap)
{
	int i,iFrame,iSect,iFramesToXLim,iFramesToYLim,iCurSecHFact,iCurSecVFact,iCurSecHSkip,iCurSecVSkip,iCurSecFrames;

	giMapSectFrames[0] = 0;
	for (i=0;i<_poMap->uiNumSections-1;i++)
	{
		iCurSecHFact = _poMap->poMSecs[i].cHSpd;
		iCurSecVFact = _poMap->poMSecs[i].cVSpd;
		iCurSecHSkip = _poMap->poMSecs[i].ucHFSkip+1;
		iCurSecVSkip = _poMap->poMSecs[i].ucVFSkip+1;
		
		if (iCurSecHFact)
			iFramesToXLim = abs( ((_poMap->poMSecs[i].usTX*TILEXSIZE - SCREENWIDTH ) / iCurSecHFact) * iCurSecHSkip );
		else
			iFramesToXLim = 0;

		if (iCurSecVFact )
			iFramesToYLim = abs( ((_poMap->poMSecs[i].usTY*TILEYSIZE - SCREENHEIGHT) / iCurSecVFact) * iCurSecVSkip );
		else
			iFramesToYLim = 0;

		if (iFramesToXLim > iFramesToYLim) iCurSecFrames = iFramesToXLim; else iCurSecFrames = iFramesToYLim;

		giMapSectFrames[i+1] = giMapSectFrames[i] + iCurSecFrames;
	}
}

/*******************************************************
--------------------------------------------------------
Setup the current map with the given specification
--------------------------------------------------------
*******************************************************/
void MAP_Init(TMap *_poMap)
{
	int i;
	
	gpoMap     = _poMap;
	giFrame    = 0;
	giMapFrame = 0;	
	
	MAP_ComputeSectionFrames(_poMap);
	MAP_SetCurrentSection(0);
	
	// Reset runtime sprite table
	for (i=0;gpoMap->poTiles[i].ucBMP != 255;i++)
	{
		if (gpoMap->poTiles[i].ucFR > 0)
		{
			goRTST[i].usSFPAF= gpoMap->poTiles[i].usATime / gpoMap->poTiles[i].ucFR;
			goRTST[i].usSF   = 0;
			goRTST[i].ucFrame= 0;
			goRTST[i].bUpd   = (gpoMap->poTiles[i].ucFR>1)?1:0;
		}
		else
		{
			goRTST[i].usSFPAF= 0;
			goRTST[i].usSF   = 0;
			goRTST[i].ucFrame= 0;
			goRTST[i].bUpd   = 0;
		}
	}

	memset(goMS,0,sizeof(TSpritePos *)*MAX_SPRITES);
}

/*******************************************************
--------------------------------------------------------
Setup the current map with the given specification
--------------------------------------------------------
*******************************************************/
void MAP_Finish()
{
}
/*******************************************************
--------------------------------------------------------
Setup the current map with the given specification
--------------------------------------------------------
*******************************************************/
void MAP_RenderMegaSprite(TSpritePos *_poSpr,int _iSprInst)
{
	int					j,i,iID;
	TSprite*			poSprite;
	TMegaSprite*		poMSprite;
	
	poMSprite = &gpoMap->poMSprites[_poSpr->ucID];

	for (j=0;j<poMSprite->usTY;j++)
	{
		for (i=0;i<poMSprite->usTX;i++)
		{	
			int iSprID			= poMSprite->pucData[ j * poMSprite->usTX + i];	// Get the sprite we want to draw
			TSprite* poSprite	= &poMSprite->poTiles[iSprID];					// Get the sprite from the table
			int iAnimFrame		= goRTST[iSprID].ucFrame;						// Get the current animation frame for this sprite				
			int iBmpFrame		= poSprite->ucFrTab[ iAnimFrame ];				// Access the real frame on the bitmap

			GPutSprite(	gpoMap->poBMPs[ poSprite->ucBMP ],
								poSprite->usX + iBmpFrame*poSprite->usTX,
								poSprite->usY,
								TILEXSIZE,
								TILEYSIZE,
								(i*TILEXSIZE) + _poSpr->iX - GCamX + SCREENHWIDTH,
								(j*TILEYSIZE) + _poSpr->iY - GCamY + SCREENHHEIGHT,
								poSprite->ucBlend);
		}
	}
}
/*******************************************************
--------------------------------------------------------
Setup the current map with the given specification
--------------------------------------------------------
*******************************************************/
void MAP_RenderMiniSprite(TSpritePos *_poSpr,int _iSprInst)
{

	TSprite	*poSprite	= &gpoMap->poTiles[_poSpr->ucID];				// Get the sprite from the table
	int iAnimFrame		= goMSRTST[_iSprInst].ucFrame;					// Get the current animation frame for this sprite				
	int iBmpFrame		= poSprite->ucFrTab[ iAnimFrame ];				// Access the real frame on the bitmap

	GPutSprite(	gpoMap->poBMPs[ poSprite->ucBMP ],
						poSprite->usX + iBmpFrame*poSprite->usTX,
						poSprite->usY,
						poSprite->usTX,
						poSprite->usTY,
						_poSpr->iX - GCamX + SCREENHWIDTH,
						_poSpr->iY - GCamY + SCREENHHEIGHT,
						poSprite->ucBlend);
}
/*******************************************************
--------------------------------------------------------
Renders a sprite or megasprite
--------------------------------------------------------
*******************************************************/
void MAP_RenderSprite(TSpritePos *_poSpr,int _iSprInst)
{
	if (_poSpr->ucFlag & SPRITEFLAG_MEGASPRITE)
		MAP_RenderMegaSprite(_poSpr,_iSprInst);
	else
		MAP_RenderMiniSprite(_poSpr,_iSprInst);
}
/*******************************************************
--------------------------------------------------------
Setup the current map with the given specification
--------------------------------------------------------
*******************************************************/
void MAP_SetSecProps(int _iSec)
{
	giCurSecHFact = gpoMap->poMSecs[giCurSec].cHSpd;
	giCurSecVFact = gpoMap->poMSecs[giCurSec].cVSpd;
	giCurSecHSkip = gpoMap->poMSecs[giCurSec].ucHFSkip+1;
	giCurSecVSkip = gpoMap->poMSecs[giCurSec].ucVFSkip+1;	
	
	if (giCurSecHFact)
		giFramesToXLim = abs( ((gpoMap->poMSecs[giCurSec].usTX*TILEXSIZE - SCREENWIDTH ) / giCurSecHFact) * giCurSecHSkip );
	else
		giFramesToXLim = 0;

	if (giCurSecVFact )
		giFramesToYLim = abs( ((gpoMap->poMSecs[giCurSec].usTY*TILEYSIZE - SCREENHEIGHT) / giCurSecVFact) * giCurSecVSkip );
	else
		giFramesToYLim = 0;
}
/*******************************************************
--------------------------------------------------------
Setup the current map with the given specification
--------------------------------------------------------
*******************************************************/
void MAP_SetCurrentSection(int _iMapSection)
{
	int i,iSectXPixels,iSectYPixels,iMaxSpd;

	if (_iMapSection != gpoMap->uiNumSections)
	{
		int iNewCamX,iNewCamY;

		giCurSec = _iMapSection;		
		
		if (gpoMap->poMSecs[giCurSec].ucFlag & START_AT_END_H)
			iNewCamX = gpoMap->poMSecs[giCurSec].usTX*TILEXSIZE - SCREENHWIDTH;
		else
			iNewCamX = SCREENHWIDTH;

		if (gpoMap->poMSecs[giCurSec].ucFlag & START_AT_END_V)
			iNewCamY = gpoMap->poMSecs[giCurSec].usTY*TILEYSIZE - SCREENHHEIGHT;
		else
			iNewCamY = SCREENHHEIGHT;

		MAP_SetSecProps(giCurSec);
		CAM_Set(iNewCamX,iNewCamY);

		// Proceed normally
		giMapFrame		= giMapSectFrames[giCurSec];
		giCurSecFrame	= 0;
	}
	else
	{
		giCurSecHFact = 0;
		giCurSecVFact = 0;
	}	
}

/*******************************************************
--------------------------------------------------------
Get current map section
--------------------------------------------------------
*******************************************************/
int MAP_iGetSection()
{
	return(giCurSec);
}
/*******************************************************
--------------------------------------------------------
Called once per frame. Assume constant framerate
--------------------------------------------------------
*******************************************************/
void MAP_Update()
{
	int i;
	int iCamX,iCamY;
	int iLastFlags = 0;
	
	// Update the animation of the map tiles
	for (i=0;gpoMap->poTiles[i].ucBMP != 255;i++)
	{
		if (goRTST[i].bUpd)
		{
			if (goRTST[i].usSF == goRTST[i].usSFPAF)
			{
				goRTST[i].usSF    = 0;
				goRTST[i].ucFrame ++;
				
				if (goRTST[i].ucFrame == gpoMap->poTiles[i].ucFR)
				{
					if (gpoMap->poTiles[i].bLoop)
						goRTST[i].ucFrame = 0;
					else
						goRTST[i].ucFrame--;
				}
			}
			else
				goRTST[i].usSF++; // Next screen frame for this sprite
		}
	}

	// Update the animation of the sprite instances
	for (i=0;i<MAX_SPRITES;i++)
	{
		if (goMSRTST[i].bUpd)
		{
			if (goMSRTST[i].usSF == goMSRTST[i].usSFPAF)
			{
				goMSRTST[i].usSF    = 0;
				goMSRTST[i].ucFrame ++;

				if (goMSRTST[i].ucFrame == gpoMap->poTiles[ goMS[i].ucID].ucFR)
				{
					if (gpoMap->poTiles[ goMS[i].ucID ].bLoop)
						goMSRTST[i].ucFrame = 0;
					else
						goMSRTST[i].ucFrame--;
				}
			}
			else
				goMSRTST[i].usSF++; // Next screen frame for this sprite
		}		
	}

	if (giStopMapScroll) return;
		
	// Update scroll position
	iCamX = GCamX;
	iCamY = GCamY;

	if (giCurSecFrame < giFramesToXLim)
	{
		if (giCurSecFrame % giCurSecHSkip == 0) iCamX += giCurSecHFact;
	}
	else
		iLastFlags |= 0x01;

	if (giCurSecFrame < giFramesToYLim)
	{
		if (giCurSecFrame % giCurSecVSkip == 0) iCamY += giCurSecVFact;
	}
	else
		iLastFlags |= 0x02;

	// Next screen frame	
	if (iLastFlags != 0x03)
	{
		CAM_Set(iCamX,iCamY);
		
		giMapFrame  ++;
		giCurSecFrame++;
	}
	else
		MAP_SetCurrentSection(giCurSec+1);

	giFrame++;
}

/*******************************************************
Draw the given map from the given point of view.
poMap -> The map
iCX,iCY : point of view
--------------------------------------------------------
FIXED Tile size: 32x32 pixels
*******************************************************/
void MAP_Render()
{
	int i;
	
	// Render the map background
	// DrawMap_Background(gpoMap,GCamX,GCamY,giCurSec);	

	// Render sprites
	for (i=0;i<MAX_SPRITES;i++)
		if (goMS[i].ucEnabled)
			MAP_RenderSprite( &goMS[i],i );

	// Render the map overlay
	DrawMap_Overlay(gpoMap,GCamX,GCamY,giCurSec);
}
// -----------------------------------------------------------------------------
int  MAP_iAddSprite(unsigned int _ucSec,int _iX,int _iY,unsigned char _ucID,unsigned char _ucFlag)
{
	int i,j;
	for (i=0;i<MAX_SPRITES;i++)
	{
		if (! goMS[iCurSP].ucEnabled)
		{
			goMS[iCurSP].iX			= _iX;
			goMS[iCurSP].iY			= _iY;
			goMS[iCurSP].ucID		= _ucID;
			goMS[iCurSP].ucFlag		= _ucFlag;
			goMS[iCurSP].ucEnabled	= 1;	

			// Look if this sprite has animation, so initilize this sprite animation instance
			if (gpoMap->poTiles[_ucID].ucFR > 1)
			{
				goMSRTST[iCurSP].usSFPAF= gpoMap->poTiles[_ucID].usATime / gpoMap->poTiles[_ucID].ucFR;
				goMSRTST[iCurSP].usSF   = 0;
				goMSRTST[iCurSP].ucFrame= 0;
				goMSRTST[iCurSP].bUpd   = 1;
			}
			else
			{
				goMSRTST[iCurSP].usSFPAF= 0;
				goMSRTST[iCurSP].usSF   = 0;
				goMSRTST[iCurSP].ucFrame= 0;
				goMSRTST[iCurSP].bUpd   = 0;
			}

			return(iCurSP);
		}

		iCurSP = (iCurSP + 1) % MAX_SPRITES;
	}
	
	return(-1);
}
// -----------------------------------------------------------------------------
void MAP_DeleteSprite(int _iID)
{
	goMS[_iID].ucEnabled = 0;
}

TSpritePos *MAP_poGetSprite(int _iID)
{
	return( &goMS[_iID] );
}
// -----------------------------------------------------------------------------
// Sprite collision utility functions
// -----------------------------------------------------------------------------
typedef struct TCOLRect{
	int iIX;
	int iIY;
	int iFX;
	int iFY;
}TCOLRect;

/*******************************************************
Retrieves the Collision rectangle for the specific sprite
--------------------------------------------------------
*******************************************************/
void MAP_GetCOLRect(TSpritePos *_poSpr,TCOLRect *_poRect)
{
	if (_poSpr->ucFlag & SPRITEFLAG_MEGASPRITE)
	{
		_poRect->iIX = _poSpr->iX;
		_poRect->iIY = _poSpr->iY;
		_poRect->iFX = _poSpr->iX + gpoMap->poMSprites[_poSpr->ucID].usTX * TILEXSIZE;
		_poRect->iFY = _poSpr->iY + gpoMap->poMSprites[_poSpr->ucID].usTY * TILEYSIZE;
	}
	else
	{
		_poRect->iIX = _poSpr->iX;
		_poRect->iIY = _poSpr->iY;
		_poRect->iFX = _poSpr->iX + gpoMap->poTiles[_poSpr->ucID].usTX;
		_poRect->iFY = _poSpr->iY + gpoMap->poTiles[_poSpr->ucID].usTY;
	}
}

/*******************************************************
Tests if the tile below the coords is collidable
--------------------------------------------------------
Se deberia tener en cuenta si la coordenada que se nos 
supera el limite de la seccion, testear con los tiles de
la seccion anterior/posterior.
*******************************************************/
BOOL MAP_bTestCollBMPs(int _iSBMP,int _iSIX,int _iSFX,int _iSIY,int _iSFY,
					   int _iDBMP,int _iDIX,int _iDFX,int _iDIY,int _iDFY)
{
	int iDX;
	int iDY;
	int iSX;
	int iSY;
	unsigned int uiSColor;
	unsigned int uiDColor;
	
	for (iSY=_iSIY,iDY=_iDIY;iSY<_iSFY;iSY++,iDY++)
	{
		for (iSX=_iSIX,iDX=_iDIX;iSX<_iSFX;iSX++,iDX++)
		{
			// Get both source and destination pixels
			uiSColor = GGetPixel(gpoMap->poBMPs[_iSBMP],iSX,iSY);
			uiDColor = GGetPixel(gpoMap->poBMPs[_iDBMP],iDX,iDY);

			// allow certain values of alpha to pass the test
			if ((uiSColor >> 24)*(uiDColor >> 24) >> 15)
				return(1);
		}
	}

	return(0);
}

/*******************************************************
Tests collision between 2 different sprites
--------------------------------------------------------
*******************************************************/
BOOL MAP_bCOLSpriteSprite(TSpritePos *_poSrc,TSpritePos *_poDst)
{
	TCOLRect oSrc,oDst;
	MAP_GetCOLRect(_poSrc,&oSrc);
	MAP_GetCOLRect(_poDst,&oDst);

	int iIX,iIY,iFX,iFY;

	if (oSrc.iIX < oDst.iIX)
	{
		if	(oDst.iIX > oSrc.iFX)
			return(false);	// this axis failed
		else
		{
			iIX = oDst.iIX;
			if (oSrc.iFX < oDst.iFX)
				iFX = oSrc.iFX;
			else
				iFX = oDst.iFX;
		}
	}
	else
	{
		if	(oDst.iFX < oSrc.iIX)
			return(false);	// this axis failed
		else
		{
			iIX = oSrc.iIX;
			if (oSrc.iFX < oDst.iFX)
				iFX = oSrc.iFX;
			else
				iFX = oDst.iFX;
		}
	}

	if (oSrc.iIY < oDst.iIY)
	{
		if	(oDst.iIY > oSrc.iFY)
			return(false);	// this axis failed
		else
		{
			iIY = oDst.iIY;
			if (oSrc.iFY < oDst.iFY)
				iFY = oSrc.iFY;
			else
				iFY = oDst.iFY;
		}
	}
	else
	{
		if	(oDst.iFY < oSrc.iIY)
			return(false);	// this axis failed
		else
		{
			iIY = oSrc.iIY;
			if (oSrc.iFY < oDst.iFY)
				iFY = oSrc.iFY;
			else
				iFY = oDst.iFY;
		}
	}

	if (_poDst->ucFlag & SPRITEFLAG_MEGASPRITE) return(true);


	int iSIX,iSIY,iSFX,iSFY,iDIX,iDIY,iDFX,iDFY;

	TSprite *pSSpr = &gpoMap->poTiles[_poSrc->ucID];
	TSprite *pDSpr = &gpoMap->poTiles[_poDst->ucID];
	int iSBmp = pSSpr->ucBMP;
	int iDBmp = pDSpr->ucBMP;

	iSIX = iIX - oSrc.iIX + pSSpr->usX;
	iSFX = iFX - oSrc.iIX + pSSpr->usX;
	iSIY = iIY - oSrc.iIY + pSSpr->usY;
	iSFY = iFY - oSrc.iIY + pSSpr->usY;

	iDIX = iIX - oDst.iIX + pDSpr->usX;
	iDFX = iFX - oDst.iIX + pDSpr->usX;
	iDIY = iIY - oDst.iIY + pDSpr->usY;
	iDFY = iFY - oDst.iIY + pDSpr->usY;

	int iSFrame = goRTST[_poSrc->ucID].ucFrame;
	int iDFrame = goRTST[_poDst->ucID].ucFrame;
	int iSXOfs  = ((pSSpr->cCol == -1)?pSSpr->ucFrTab[ iSFrame ]:pSSpr->cCol)*pSSpr->usTX;
	int iDXOfs  = ((pDSpr->cCol == -1)?pDSpr->ucFrTab[ iDFrame ]:pDSpr->cCol)*pDSpr->usTX;

	iSIX += iSXOfs;
	iSFX += iSXOfs;
	iDIX += iDXOfs;
	iDFX += iDXOfs;

	return ( MAP_bTestCollBMPs(iSBmp,iSIX,iSFX,iSIY,iSFY,
           					   iDBmp,iDIX,iDFX,iDIY,iDFY) );
}
/*******************************************************
Tests if the tile below the coords is collidable
--------------------------------------------------------
Se deberia tener en cuenta si la coordenada que se nos 
supera el limite de la seccion, testear con los tiles de
la seccion anterior/posterior.
*******************************************************/
BOOL MAP_bCollAtPos(TSpritePos *_poMS,int _iX,int _iY,int _iXEnd,int _iYEnd)
{
	TMapSection *poSec	= &gpoMap->poMSecs[giCurSec];
	int iX				= _iX / TILEXSIZE;
	int iY				= _iY / TILEYSIZE;
	int iTileID			= poSec->pucOvData[iY*poSec->usTX + iX];
	
	if (iTileID == 0)
	{
		return(false);
	}

	TSpritePos oDstSpr;
	
	if (iX < 0) return(false); else if (iX >= gpoMap->poMSecs[giCurSec].usTX) return(false);
	if (iY < 0) return(false); else if (iY >= gpoMap->poMSecs[giCurSec].usTY) return(false);

	oDstSpr.iX			= iX * TILEXSIZE;
	oDstSpr.iY			= iY * TILEYSIZE;
	oDstSpr.ucEnabled	= true;
	oDstSpr.ucFlag		= SPRITEFLAG_CHECK_COLLISION;
	oDstSpr.ucID		= iTileID;

	return ( MAP_bCOLSpriteSprite(_poMS,&oDstSpr) );


	/*
	unsigned char ucQFlags = 0;
	unsigned char ucCFlag = gpoMap->pucCTiles[ iTileID ];
	if (! ucCFlag) return(0);


	// Clamp the coordinates

	_iX %= TILEXSIZE;
	_iY %= TILEYSIZE;

	
	// Get the quadrant the point is belonging to			// Quadrants
	if (_iX > TILEXSIZE>>1) ucQFlags |= 0x01;				// 0 1
	if (_iY > TILEYSIZE>>1) ucQFlags |= 0x02;				// 2 3

	if ( (1 << ucQFlags) & ucCFlag)
	{
   		if (_poMS->ucFlag & SPRITEFLAG_MEGASPRITE)
			return(1);
		else
		{
			int iSIX;
			int iSIY;
			int iSFX;
			int iSFY;
			int iSBMP;
			int iSXOfs;
			int iSYOfs;

			int iDIX;
			int iDIY;
			int iDFX;
			int iDFY;
			int iDBMP;
			int iDXOfs;
			int iDYOfs;

			TSprite* poSSpr;
			TSprite* poDSpr;

			poSSpr = &gpoMap->poTiles[ iTileID     ];
			poDSpr = &gpoMap->poTiles[ _poMS->ucID ];

			iSXOfs = poSSpr->usX + poSSpr->ucFrTab[ goRTST[iTileID    ].ucFrame ] * poSSpr->usTX;
			iSYOfs = poSSpr->usY;

			iDXOfs = poDSpr->usX + poDSpr->ucFrTab[ goRTST[_poMS->ucID].ucFrame ] * poDSpr->usTX;
			iDYOfs = poDSpr->usY;

			// Stablish source rect bounds
			if (_iXEnd)
			{
				iSIX = iSXOfs + 0;
				iSFX = iSXOfs + _iX;
				iDIX = iDXOfs + poDSpr->usTX - _iX;
				iDFX = iDXOfs + poDSpr->usTX - 1;
			}
			else
			{
				iSIX = iSXOfs + _iX;
				iSFX = iSXOfs + TILEXSIZE-1;
				iDIX = iDXOfs + 0;
				iDFX = iDXOfs + TILEXSIZE - _iX;
			}

			if (_iYEnd)
			{
				iSIY = iSYOfs + 0;
				iSFY = iSYOfs + _iY;
				iDIY = iDYOfs + poDSpr->usTY - _iY;
				iDFY = iDYOfs + poDSpr->usTY - 1;
			}
			else
			{
				iSIY = iSYOfs + _iY;
				iSFY = iSYOfs + TILEYSIZE - 1;
				iDIY = iDYOfs + 0;
				iDFY = iDYOfs + TILEYSIZE - _iY;
			}

			iSBMP = poSSpr->ucBMP;
			iDBMP = poDSpr->ucBMP;

			return( MAP_bTestCollBMPs(iSBMP,iSIX,iSFX,iSIY,iSFY,
									  iDBMP,iDIX,iDFX,iDIY,iDFY) );
		}
	}
	else
		return(-1);
	*/
}

/*******************************************************
Checks wether the given sprite collides with the rest of
the map elements
--------------------------------------------------------
FIXED Tile size: 32x32 pixels
*******************************************************/
BOOL MAP_bCOLSpriteMap(TSpritePos *_poMS)
{
	int i;
	int iCols[4];
	
	// Test first against the map
	TCOLRect oSrc;
	MAP_GetCOLRect(_poMS,&oSrc);

	// Test each 4 sprite corners against the map
	if (MAP_bCollAtPos(_poMS,oSrc.iIX,oSrc.iIY,0,0)) return(1);
	if (MAP_bCollAtPos(_poMS,oSrc.iFX,oSrc.iIY,1,0)) return(1);
	if (MAP_bCollAtPos(_poMS,oSrc.iIX,oSrc.iFY,0,1)) return(1);
	if (MAP_bCollAtPos(_poMS,oSrc.iFX,oSrc.iFY,1,1)) return(1);

	return(0);
}

/*******************************************************
Tests if the tile below the coords is collidable
--------------------------------------------------------
Se deberia tener en cuenta si la coordenada que se nos 
supera el limite de la seccion, testear con los tiles de
la seccion anterior/posterior.
*******************************************************/
void MAP_Screen2World(int _iScrX,int _iScrY,int *_iWldX,int *_iWldY)
{
	*_iWldX = GCamX - SCREENHWIDTH  + _iScrX;
	*_iWldY = GCamY - SCREENHHEIGHT + _iScrY;
}

void MAP_World2Screen(int _iWldX,int _iWldY,int *_iScrX,int *_iScrY)
{
	*_iScrX = _iWldX - (GCamX -  SCREENHWIDTH);
	*_iScrY = _iWldY - (GCamY - SCREENHHEIGHT);
}
/*
unsigned char ucColFlag;
ucColFlag = 0;

if (_poMS  ->ucFlag & SPRITEFLAG_MEGASPRITE) ucColFlag |= 0x01;
if (goMS[i]->ucFlag & SPRITEFLAG_MEGASPRITE) ucColFlag |= 0x02;

switch (ucColFlag)
{
	case 0:
	{
		// Sprite-Sprite collision
	}
	break;

	case 1:
	{
		// MegaSprite-Sprite collision
	}
	break;

	case 2:
	{
		// Sprite-MegaSprite collision
	}
	break;

	case 3:
	{
		// MegaSprite-MegaSprite collision
	}
	break;
}
*/
