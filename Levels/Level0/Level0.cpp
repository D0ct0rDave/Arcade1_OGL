// -----------------------------------------------------------------------------
#include <stdlib.h>	// for NULL definition
#include <windows.h>
#include <gl/gl.h>

#include "graphics/graphics.h"
#include "graphics/GMap.h"
#include "graphics/GCam.h"

#include "levels/level.h"
#include <math.h>

// *****************************************************************************
// -----------------------------------------------------------------------------
// Editar las secciones del mapa
// -----------------------------------------------------------------------------
// *****************************************************************************
#include "data/Levels/Level0/MAP0_Sect0.c"
#include "data/Levels/Level0/MAP0_Sect1.c"
#include "data/Levels/Level0/MAP0_Sect2.c"
#include "data/Levels/Level0/MAP0_Sect3.c"
#include "data/Levels/Level0/MAP0_Sect4.c"


TMapSection MAP0_Sect_Lst[] = {

	{
	(unsigned char*)MAP0_Sect0_BK_Data,	// Background data
	(unsigned char*)MAP0_Sect0_OV_Data,	// Overlay data
	MAP0_SECT0_TX,		// TX
	MAP0_SECT0_TY,		// TY
	1,					// cHSpd
	0,					// cVSpd
	0,					// ucHFSkip
	0,					// ucVFSkip
	0,					// cFlags
	MOVEMENTMODE_LEFT	// ucMovMode
	},

	{
	(unsigned char*)MAP0_Sect1_BK_Data,	// Background data
	(unsigned char*)MAP0_Sect1_OV_Data,	// Overlay data
	MAP0_SECT1_TX,		// TX
	MAP0_SECT1_TY,		// TY
	1,					// cHSpd
	0,					// cVSpd
	0,					// ucHFSkip
	0,					// ucVFSkip
	0/*START_AT_END_H*/,// cFlags
	MOVEMENTMODE_LEFT	// ucMovMode
	},
	
	{
	(unsigned char*)MAP0_Sect2_BK_Data,	// Background data
	(unsigned char*)MAP0_Sect2_OV_Data,	// Overlay data
	MAP0_SECT2_TX,		// TX
	MAP0_SECT2_TY,		// TY
	1,					// cHSpd
	0,					// cVSpd
	0,					// ucHFSkip
	0,					// ucVFSkip
	0/*START_AT_END_H*/,// cFlags
	MOVEMENTMODE_LEFT	// ucMovMode
	},

	{
	(unsigned char*)MAP0_Sect3_BK_Data,	// Background data
	(unsigned char*)MAP0_Sect3_OV_Data,	// Overlay data
	MAP0_SECT3_TX,		// TX
	MAP0_SECT3_TY,		// TY
	0,					// cHSpd
	1,					// cVSpd
	0,					// ucHFSkip
	0,					// ucVFSkip
	0/*START_AT_END_H*/,// cFlags
	MOVEMENTMODE_DOWN	// ucMovMode
	},

	{
	(unsigned char*)MAP0_Sect4_BK_Data,	// Background data
	(unsigned char*)MAP0_Sect4_OV_Data,	// Overlay data
	MAP0_SECT4_TX,		// TX
	MAP0_SECT4_TY,		// TY
	0,					// cHSpd
	1,					// cVSpd
	0,					// ucHFSkip
	0,					// ucVFSkip
	0/*START_AT_END_H*/,// cFlags
	MOVEMENTMODE_DOWN	// ucMovMode
	},
};
// -----------------------------------------------------------------------------
// Tablas de sprites
// -----------------------------------------------------------------------------
TSprite MAP0_SprLst[] = {
		
		// Bmp ID, X, Y,Width,Height,AnimTable,Num Frames,AFrames,Loop
		#include "data/levels/level0/MAP0_Tiles.c"
		
		#include "data/Ship.c"

		#include "data/common/common.c"

		// Bmp ID, X, Y,Width,Height,AnimTable,Num Frames,AFrames,Loop
		{1,   0,   0,  64, 64, {0},1, 0, 1,1},		// 80	Cono Up
		{1,   0,  64,  64, 64, {0},1, 0, 1,1},		// 81	Cono Down
		{1,   0, 128,  64, 64, {0,1}, 2,24, 1,1},		// 82	Estrellita		
		{1,   0, 192, 128, 64, {0,1}, 2, 6, 1,1},		// 83	Espermatozoo
		{1, 192,  64,  64, 64, {0},1, 0, 1,1},		// 84	Bacteria
		{1,  64,   0,  64,128, {0},1, 0, 1,1},		// 85	Cromosoma
		{1, 128,   0,  64, 64, {0},1, 0, 1,1},		// 86	Huevo
		{1, 128,  64,  64, 64, {0},1, 0, 1,1},		// 87	Nudo
		{1, 192,   0,  64, 64, {0},1, 0, 1,1},		// 88	Esfera
		{1, 128, 128,  64, 64, {0,1}, 2,24,1,1},		// 89	Esfera-ovoide
		{1,   0, 256,  64, 64, {0,1,2,1},4,60,1,1},	// 90	Muelle
		{0,   0, 0, 0, 0, {0}, 0, 0, 0, 0},			// 91
		{0,   0, 0, 0, 0, {0}, 0, 0, 0, 0},			// 92
		{0,   0, 0, 0, 0, {0}, 0, 0, 0, 0},			// 93
		{0,   0, 0, 0, 0, {0}, 0, 0, 0, 0},			// 94
		{0,   0, 0, 0, 0, {0}, 0, 0, 0, 0},			// 95
		{0,   0, 0, 0, 0, {0}, 0, 0, 0, 0},			// 96
		{0,   0, 0, 0, 0, {0}, 0, 0, 0, 0},			// 97
		{0,   0, 0, 0, 0, {0}, 0, 0, 0, 0},			// 98
		{0,   0, 0, 0, 0, {0}, 0, 0, 0, 0},			// 99

		{1, 192, 256,  16, 16, {0},1, 0, 1, 2},		// 100	circulo blast
		{1, 208, 256,  16, 16, {0,1}, 2, 6, 1, 2},	// 101	cruz blast
		{1, 192, 272,  16, 16, {0},1, 0, 1, 2},		// 102	donut blast
		{1, 208, 272,  16, 16, {0},1, 0, 1, 2},		// 103	naranja blast
		{1, 192, 288,  16, 16, {0,1}, 2, 6, 1, 2},	// 104	piramid blast

		// Last sprite
		{255, 0, 0, 0, 0, {0}, 0, 0, 0},
};


unsigned char MAP0_CollLst[] = {
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// Background sprites
		0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,	// overlay sprites
		0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,	// overlay sprites
		0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,	// overlay sprites
		0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,	// overlay sprites
		0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,	// overlay sprites		
		0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,	// overlay sprites
};
// -----------------------------------------------------------------------------
// Mega sprites
// -----------------------------------------------------------------------------
#include "data/Levels/Level0/MAP0_Sect10.c"
#include "data/Levels/Level0/MAP0_Sect11.c"
#include "data/Levels/Level0/MAP0_Sect12.c"
#include "data/Levels/Level0/MAP0_Sect13.c"


TMegaSprite MAP0_MS_Lst[] = {

	{
		(unsigned char*)MAP0_Sect10_OV_Data,
		MAP0_SECT10_TX,
		MAP0_SECT10_TY,
		MAP0_SprLst,
	},
	{
		(unsigned char*)MAP0_Sect11_OV_Data,
		MAP0_SECT11_TX,
		MAP0_SECT11_TY,
		MAP0_SprLst,
	},
	{
		(unsigned char*)MAP0_Sect12_OV_Data,
		MAP0_SECT12_TX,
		MAP0_SECT12_TY,
		MAP0_SprLst,
	},
	{
		(unsigned char*)MAP0_Sect13_OV_Data,
		MAP0_SECT13_TX,
		MAP0_SECT13_TY,
		MAP0_SprLst,
	},
};
// -----------------------------------------------------------------------------
// Tabla de Bitmaps
// -----------------------------------------------------------------------------
char *szMAP0_ResTab[] = {
	"data/ship",
	"data/Levels/Level0/Enemies",
	"data/Levels/Level0/MAP0_Tiles_BkTiles",
	"data/Levels/Level0/MAP0_Tiles_OvTiles",
	"data/Common/Common",
	"data/Levels/Level0/MAP0_Tiles_BkTiles2",
	"data/Levels/Level0/background",
	NULL,
	NULL,
	NULL,
};


BMPHandle	MAP0_BMPs[16] = {
	(BMPHandle)0,
	(BMPHandle)1,
	(BMPHandle)2,
	(BMPHandle)3,
	(BMPHandle)4,
	(BMPHandle)5,
	(BMPHandle)6,
};
// -----------------------------------------------------------------------------
// Rellenar datos del mapa
// -----------------------------------------------------------------------------
TMap MAP0 = {
	5,					// Map sections
	MAP0_Sect_Lst,

	5,					// Megasprite list
	MAP0_MS_Lst,

	MAP0_SprLst,		// Sprite list
	MAP0_BMPs,			// Bitmap list
	MAP0_CollLst,		// Collidable tiles
};
// *****************************************************************************
// -----------------------------------------------------------------------------
// Function prototypes
// -----------------------------------------------------------------------------
// *****************************************************************************
void GAMEAPIENTRY Level0_Render();
void GAMEAPIENTRY Level0_Update();
void GAMEAPIENTRY Level0_InitPersistent();
// *****************************************************************************
// -----------------------------------------------------------------------------
// Entities
// -----------------------------------------------------------------------------
// *****************************************************************************
#include "L0_Entities/L0_World.h"
#include "Levels/Entities/Player.h"
// -----------------------------------------------------------------------------
#define MAX_DIVS	32
#define MAX_VERT	(MAX_DIVS*MAX_DIVS)
#define MAX_IDXS	((MAX_DIVS-1)*(MAX_DIVS-1)*6)

float*			gpfVXs  = NULL;
float*			gpfUVs  = NULL;
unsigned int*	gpuiVCs = NULL;
unsigned short*	gpusIdx = NULL;

float*			gpfVX2s  = NULL;
float*			gpfUV2s  = NULL;

// -----------------------------------------------------------------------------
void Level0_Init()
{
	int iEnt;
	int i,j,iIdx;
	float fXStep,fYStep,fCurY,fCurX;
	
	// Setup new special background sprite	
	TSprite	oBackSpr = {  5,  0,  0, 64, 64,{0,0,0,1,0,0,0,1}, 8, 60, 1};
	MAP0_SprLst[1] = oBackSpr;

	// Set resource filenames
	GSetResourceTable(szMAP0_ResTab);

	// Setup callbacks
	LevelRender			= Level0_Render;
	LevelUpdate			= Level0_Update;
	LevelInitPersistent	= Level0_InitPersistent;

	// Load bitmaps
	MAP0_BMPs[0] = GLoadBitmap(128,128,0);
	MAP0_BMPs[1] = GLoadBitmap(128,128,1);
	MAP0_BMPs[2] = GLoadBitmap(128,128,2);
	MAP0_BMPs[3] = GLoadBitmap(128,128,3);
	MAP0_BMPs[4] = GLoadBitmap(128,128,4);
	MAP0_BMPs[5] = GLoadBitmap(128,128,5);
	MAP0_BMPs[6] = GLoadBitmap(128,128,6);

	// Initialize map
	MAP_Init(&MAP0);

	// Init background mesh
	gpfVXs  = (float*)malloc(MAX_VERT*3*sizeof(float));
	gpfUVs  = (float*)malloc(MAX_VERT*2*sizeof(float));
	gpfVX2s = (float*)malloc(MAX_VERT*3*sizeof(float));
	gpfUV2s = (float*)malloc(MAX_VERT*2*sizeof(float));
	gpuiVCs = (unsigned int  *)malloc(MAX_VERT*sizeof(unsigned int  ));	

	fXStep = 1.0f / (float)(MAX_DIVS-1);
	fYStep = 1.0f / (float)(MAX_DIVS-1);

	fCurY = 0.0f;
	for (j=0;j<MAX_DIVS;j++)
	{
		fCurX = 0.0f;
		for (i=0;i<MAX_DIVS;i++)
		{
			iIdx = j*MAX_DIVS+i;

			gpfVXs[iIdx*3 + 0] = SCREENWIDTH *fCurX;
			gpfVXs[iIdx*3 + 1] = SCREENHEIGHT*fCurY;
			gpfVXs[iIdx*3 + 2] = 1.0f;

			gpfUVs[iIdx*2 + 0] = fCurX*0.5f;
			gpfUVs[iIdx*2 + 1] = fCurY*0.5f;

			gpuiVCs[iIdx     ] = 0xffffffff;

			fCurX += fXStep;
		}

		fCurY += fYStep;
	}

	memcpy(gpfVX2s,gpfVXs,MAX_VERT*3*sizeof(float));
	memcpy(gpfUV2s,gpfUVs,MAX_VERT*2*sizeof(float));

	// Generate indices
	gpusIdx = (unsigned short*)malloc(MAX_IDXS*sizeof(unsigned short));

	iIdx = 0;
	for (j=0;j<MAX_DIVS-1;j++)
		for (i=0;i<MAX_DIVS-1;i++)
		{
			// t1
			gpusIdx[iIdx++] = (j  )*MAX_DIVS+(i  );
			gpusIdx[iIdx++] = (j  )*MAX_DIVS+(i+1);
			gpusIdx[iIdx++] = (j+1)*MAX_DIVS+(i+1);

			// t2
			gpusIdx[iIdx++] = (j  )*MAX_DIVS+(i  );
			gpusIdx[iIdx++] = (j+1)*MAX_DIVS+(i+1);
			gpusIdx[iIdx++] = (j+1)*MAX_DIVS+(i  );
		}

	
}
// -----------------------------------------------------------------------------
void GAMEAPIENTRY Level0_Update()
{
	// Update background mesh	
	int i,j;

	// Restore initial values
	memcpy(gpfVXs,gpfVX2s,MAX_VERT*3*sizeof(float));
	memcpy(gpfUVs,gpfUV2s,MAX_VERT*2*sizeof(float));

	// Scroll
	unsigned int uiOfs = GCamX % SCREENWIDTH;
	float	     fOfs  = (float)uiOfs /(float)SCREENWIDTH;

	for (i=0;i<MAX_VERT;i++)
		gpfUVs[i*2+0] += fOfs;

	// Perturbations
	#define _PI_	3.1415926535897932384626433832795f
	#define _2PI_	(2*_PI_)

	float fTime = (float)giMapFrame/(float)SCREENFPS;
	for (j=1;j<MAX_DIVS-1;j++)
	{
		float fMaxAmp = 64.0f*(1.0f - gpfUV2s[ j*MAX_DIVS*2+1 ]*4.0f);

		for (i=0;i<MAX_DIVS;i++)
		{
			int iIdx = j*MAX_DIVS+i;

			float fPhase = (float)(GCamX % SCREENWIDTH)/(float)SCREENWIDTH;
			float fFreq  = 0.125f;

			gpfVXs[iIdx*3 + 1] += fMaxAmp * sinf( (fPhase + gpfUV2s[iIdx*2 + 0] + fFreq *fTime)* _2PI_);
		}
	}


	// Update map
	MAP_Update();

	// Update entities
	ENT_Update();
}
// -----------------------------------------------------------------------------
void GAMEAPIENTRY Level0_Render()
{
	// glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	GDrawMesh(MAP0_BMPs[6],gpusIdx,gpfVXs,gpfUVs,gpuiVCs,MAX_IDXS,BLENDCOPY);
	// glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	MAP_Render();
}
// -----------------------------------------------------------------------------
void GAMEAPIENTRY Level0_InitPersistent()
{
	Level0Ent_WorldInit();
	ENT_PlayerInit();
}
// -----------------------------------------------------------------------------