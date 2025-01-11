// -----------------------------------------------------------------------------
#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__
// -----------------------------------------------------------------------------
typedef enum
{
	Left,
 	Centre,
 	Right

}GAlignment;
// -----------------------------------------------------------------------------
#ifndef Boolean
#define Boolean int
#endif
// -----------------------------------------------------------------------------
#define SCREENFPS				60
#define SCREENDELTAT			(1000.0f/(float)SCREENFPS)
#define SCREENWIDTH				640
#define SCREENHEIGHT			448
#define SCREENHWIDTH			(SCREENWIDTH/2)
#define SCREENHHEIGHT			(SCREENHEIGHT/2)
#define BLENDCOPY				0
#define BLENDALPHA				1
#define BLENDADD				2
#define BLENDALPHAADD			3
#define BMPWIDTH				-1
#define BMPHEIGHT				-1
// -----------------------------------------------------------------------------
extern int			GFRAMENUM;
// ----------------------------------------------------------------------------
typedef void *BMPHandle;
// -----------------------------------------------------------------------------
int  GInit(void *pParam);
void GFinish();
void GClear();
void GBlit();

void GTextXY(char *str,int x,int y,GAlignment align,Boolean black);
void GPutSprite(BMPHandle _SrcHnd,int _iSrcX,int _iSrcY,int _iTX,int _iTY,int _iDstX,int _iDstY,int _iBlend);

void GDrawMesh(BMPHandle _SrcHnd,unsigned short* _pusIdx,float* _pfPoints,float* _pfUVs,unsigned int* _puiColors,unsigned int _uiPoints,int _iBlend);

void GSetResourceTable(char** _pszResTab);
BMPHandle GLoadBitmap(int _iTX,int _iTY,int _iResourceID);
void GFreeBitmap(BMPHandle _SrcHnd);

unsigned int GGetPixel(BMPHandle _SrcHnd,int _iX,int _iY);
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
