// -----------------------------------------------------------------------------
#ifndef InputH
#define InputH
// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif	/* __cplusplus */

// -----------------------------------------------------------------------------
#define CMD_UP		0x00000001
#define CMD_DOWN	0x00000002
#define CMD_LEFT	0x00000004
#define CMD_RIGHT	0x00000008
#define CMD_LTRIG	0x00000010
#define CMD_RTRIG	0x00000020
#define CMD_ABUTT	0x00000040
#define CMD_BBUTT	0x00000080
#define CMD_CBUTT	0x00000100
#define CMD_DBUTT	0x00000200
#define CMD_SELECT	0x00000400
#define CMD_START	0x00000800
// -----------------------------------------------------------------------------
typedef struct {
	unsigned int uiPressed;
	unsigned int uiJustPressed;
	unsigned int uiReleased;
	unsigned int uiJustReleased;

}TInput;

extern TInput oInput;
void INPUT_Update();
// -----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	/* __cplusplus */
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
