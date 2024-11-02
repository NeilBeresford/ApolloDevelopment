/** ---------------------------------------------------------------------------
	@file		main.c
	@defgroup 	MainShell Apollo V4 Shell
	@brief		Apollo V4 development - Shell
	@date		2020-06-01
	@version	0.1
	@copyright	Neil Beresford 2024
 -----------------------------------------------------------------------------
	Notes

--------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "stdint.h"
#include "stdbool.h"
#include "Support/Hardware.h"
#include "Source/Includes/ResourceHandling.h"

//-----------------------------------------------------------------------------
// Code
//-----------------------------------------------------------------------------

/** ---------------------------------------------------------------------------
	@brief 		Main entry point for the Apollo V4 Shell
	@ingroup 	MainShell
	@return 	int - return code, 0 success
 --------------------------------------------------------------------------- */
uint32_t main(int argc, char *argv[])
{
	uint32_t keyReturn = 0;

	// Initialize the system and hardware
	// ResourceHandling_Init();
	Hardware_Init();

	while (true)
	{

		Hardware_WaitVBL();
		Hardware_FlipScreen();
		Hardware_TestScreen();

		keyReturn = Hardware_ReadKey();
		if (keyReturn == 0x45)
		{
			break;
		}
	}

	Hardware_Close();

	// return succes
	return 0;
}

//-----------------------------------------------------------------------------
// End of File: main.c
//-----------------------------------------------------------------------------
