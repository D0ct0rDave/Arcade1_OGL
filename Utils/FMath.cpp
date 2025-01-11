// -----------------------------------------------------------------------------
#include "FMath.h"
#include <math.h>

#define _PI_ 3.1415926535897932384626433832795f

short sSin(unsigned short usAngle)
{
	// De momento asi
	return ( (short)(32767.0f * sin(2.0f*_PI_*(float)usAngle/65535.0f) ) );

}

short sCos(unsigned short usAngle)
{
	return ( (short)(32767.0f * cos(2.0f*_PI_*(float)usAngle/65535.0f) ) );
}
// -----------------------------------------------------------------------------