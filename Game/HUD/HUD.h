// -----------------------------------------------------------------------------
#ifndef HUDH
#define HUDH
// -----------------------------------------------------------------------------

#define NORMALFONT_SIZE	16

void HUD_Init();
void HUD_Draw();
void HUD_Finish();

void HUD_DrawInteger(int _iX,int _iY,int _iSprID,int _iValue);
void HUD_DrawString (int _iX,int _iY,int _iSprID,char *_szStr);

// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------