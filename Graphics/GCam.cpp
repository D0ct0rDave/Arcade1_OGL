// -----------------------------------------------------------------------------
#include "GCam.h"
#include "graphics.h"
// -----------------------------------------------------------------------------
int GCamX;
int GCamY;
int GCamMinX;
int GCamMinY;
int GCamMaxX;
int GCamMaxY;
// -----------------------------------------------------------------------------
void CAM_Set(int _iX,int _iY)
{
	GCamX = _iX;
	GCamY = _iY;

	GCamMinX = _iX - SCREENHWIDTH;
	GCamMaxX = _iX + SCREENHWIDTH;
	GCamMinY = _iY - SCREENHHEIGHT;
	GCamMaxY = _iY + SCREENHHEIGHT;
}
// -----------------------------------------------------------------------------
void CAM_GoToMapSection(int _iMapSect)
{
}
// -----------------------------------------------------------------------------