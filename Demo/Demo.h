// -----------------------------------------------------------------------------
#ifndef DemoH
#define DemoH
// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif	/* __cplusplus */


// -----------------------------------------------------------------------------
// Function prototypes
// -----------------------------------------------------------------------------
int DEMO_iLoop();
int DEMO_iLoopRecord();

// -----------------------------------------------------------------------------
#define DEMOSTATE_NONE				0
#define DEMOSTATE_INIT				1
#define DEMOSTATE_FINISH			2
#define DEMOSTATE_READY				3
#define DEMOSTATE_READYMESSAGE		4
#define DEMOSTATE_RUN				5
#define DEMOSTATE_DEAD				6
#define DEMOSTATE_DEMOOVER			7

extern unsigned int		guiDemoState;
// -----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif	/* __cplusplus */
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------