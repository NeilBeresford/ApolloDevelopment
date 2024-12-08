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
#include "stdio.h"
#include "Includes/Hardware.h"
#include "Includes/HWScreen.h"
#include "Includes/ResourceHandling.h"
#include "Includes/FontModule.h"
#include "Includes/LIB_Files.h"
#include "Includes/LIB_Sprites.h"

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
	uint32_t 	keyReturn 			= 0;
	uint32_t 	ulTestSpriteSize	= 0;
	uint8_t* 	fileBuffer 			= NULL;
	uint8_t* 	TestSpriteRawm 		= NULL;
	uint32_t* 	paletteBuffer 		= NULL;

	printf("Apollo V4 Shell\n");
	printf("Press 'ESC' to exit\n");
	printf("Loading files...\n");

	// load in the files...
	if ( LIB_Files_Load("Data/NeilImage.png.RAW", 		&fileBuffer, NULL)		== false ) { printf("Failed to load screen\n"); return 1; }
	if ( LIB_Files_Load("Data/NeilImage.png.RAW.pal", 	&paletteBuffer, NULL)	== false ) { printf("Failed to load palette\n"); return 1; }
	if ( LIB_Files_Load("Data/waccused.png.RAW",		&TestSpriteRawm, &ulTestSpriteSize)	== false ) { printf("Failed to load sprite\n"); return 1; }

	// Initialize the system and hardware
	ResourceHandling_Init();

	LIB_Sprites_Init();
	LIB_Sprites_RegisterBank( eSpriteBank_0, eSpriteType_Raw, 100, TestSpriteRawm, ulTestSpriteSize, 60, 60 );

	Hardware_Init();
	HWSCREEN_SetImagePalette( paletteBuffer );

	while (true)
	{
		Hardware_WaitVBL();
		Hardware_FlipScreen();
		HWSCREEN_DisplayImage( fileBuffer);

		if ( LIB_Sprites_Draw( eSpriteBank_0, 1, 0, 0 ) == false  )
		{
			break;
		}

		FontModule_DisplayString();

		keyReturn = Hardware_ReadKey();
		if (keyReturn == 0x45)
		{
			break;
		}
	}

	Hardware_Close();

	free(fileBuffer);
	free(paletteBuffer);

	// return succes
	return 0;
}

//-----------------------------------------------------------------------------
// End of File: main.c
//-----------------------------------------------------------------------------
