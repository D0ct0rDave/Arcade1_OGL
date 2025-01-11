// -----------------------------------------------------------------------------
#include "Input.h"

TInput oInput;
unsigned int uiOldKeys;

// -----------------------------------------------------------------------------
void INPUT_Update()
{
	oInput.uiJustPressed = oInput.uiPressed  & (oInput.uiPressed  ^ uiOldKeys);
	oInput.uiReleased    =~oInput.uiPressed;
	oInput.uiJustReleased= (oInput.uiReleased ^ uiOldKeys);
	
	uiOldKeys = oInput.uiPressed;	
}
// -----------------------------------------------------------------------------