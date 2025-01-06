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
// Forward declarations
//-----------------------------------------------------------------------------

void CreateBackScreens( void );

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
	uint32_t 	keyReturn 				= 0;
	uint32_t 	ulTestSpriteSize		= 0;
	uint32_t 	ulTestSpriteSize2		= 0;
	uint32_t 	ulTestGround			= 0;
	uint32_t 	ulTestGradient			= 0;
	uint32_t 	ulTestBack				= 0;
	uint32_t 	ulTestFridge			= 0;
	uint32_t 	ulTestSpriteCompSize	= 0;
	uint8_t* 	fileBuffer 				= NULL;
	uint8_t* 	TestSpriteWorm 			= NULL;
	uint8_t* 	TestSpriteWorm2			= NULL;
	uint8_t* 	TestGround				= NULL;
	uint8_t* 	TestGradient			= NULL;
	uint8_t* 	TestBack				= NULL;
	uint8_t* 	TestFridge				= NULL;
	uint32_t* 	paletteBuffer 			= NULL;

	printf("Apollo V4 Shell\n");
	printf("Press 'ESC' to exit\n");

	Hardware_SetScreenmode( 0 );

	printf( "Screen width: %d\n", Hardware_GetScreenWidth() );
	printf( "Screen height: %d\n", Hardware_GetScreenHeight() );

	Hardware_SetScreenmode( 1 );
	
	printf( "Back Screen width: %d\n", Hardware_GetScreenWidth() );
	printf( "Back Screen height: %d\n", Hardware_GetScreenHeight() );

	Hardware_SetScreenmode( 0 );

	printf("Loading files...\n");

	// load in the files...
	if ( LIB_Files_Load("Data/Palettes/paletteSnow.bin",&paletteBuffer, NULL)	== false ) { printf("Failed to load palette\n"); return 1; }
	if ( LIB_Files_Load("Data/Worms/waccuse.png.SPR",	&TestSpriteWorm, &ulTestSpriteSize)	== false ) { printf("Failed to load sprite\n"); return 1; }
	if ( LIB_Files_Load("Data/Worms/wkamlnkd.png.SPR",	&TestSpriteWorm2, &ulTestSpriteSize2)	== false ) { printf("Failed to load sprite\n"); return 1; }
	if ( LIB_Files_Load("Data/Terrain/Snow/text.png.RAW",&TestGround, &ulTestGround)	== false ) { printf("Failed to load ground\n"); return 1; }
	if ( LIB_Files_Load("Data/Terrain/Snow/gradient.png-8-916.RAW",&TestGradient, &ulTestGradient)	== false ) { printf("Failed to load gradient\n"); return 1; }
	if ( LIB_Files_Load("Data/Terrain/Snow/back.png.RAW",&TestBack, &ulTestBack)	== false ) { printf("Failed to load back\n"); return 1; }
	if ( LIB_Files_Load("Data/Terrain/Snow/Fridge.png.SPR",&TestFridge, &ulTestFridge)	== false ) { printf("Failed to load Fridge\n"); return 1; }

	// Initialize the system and hardware
	ResourceHandling_Init();

	LIB_Sprites_Init();

	// ---------------------: Bank,          Type,                      ID,  Data,           Size,                  Num,Wid,Hei
	LIB_Sprites_RegisterBank( eSpriteBank_0, eSpriteType_Compressed, 	100, TestSpriteWorm, ulTestSpriteSize, 		36, 60, 60 );
	LIB_Sprites_RegisterBank( eSpriteBank_1, eSpriteType_Compressed, 	101, TestSpriteWorm2,ulTestSpriteSize2, 	9, 60, 60 );
	LIB_Sprites_RegisterBank( eSpriteBank_2, eSpriteType_Raw, 			102, TestGround, 	 ulTestGround, 			1, 256, 256 );
	LIB_Sprites_RegisterBank( eSpriteBank_3, eSpriteType_Raw, 			103, TestGradient, 	 ulTestGradient, 		1, 8, 900 );
	LIB_Sprites_RegisterBank( eSpriteBank_4, eSpriteType_Raw, 			104, TestBack, 		 ulTestBack, 			1, 640, 157 );
	LIB_Sprites_RegisterBank( eSpriteBank_5, eSpriteType_Compressed,	105, TestFridge, 	 ulTestFridge, 			1, 216, 201 );

	printf("Files loaded\n");	
	printf("Remapping level sprites\n");

	LIB_Sprites_Remap( eSpriteBank_2, 89 );
	LIB_Sprites_Remap( eSpriteBank_3, 184 );
	LIB_Sprites_Remap( eSpriteBank_4, 89 );
	LIB_Sprites_Remap( eSpriteBank_5, 89 );

	printf("Create the back screens\n");
	Hardware_SetBackscreenBuffers();
	CreateBackScreens();		

	Hardware_Init();
	HWSCREEN_SetImagePalette( paletteBuffer );

	uint32_t ulSpriteID = 0;
	uint32_t ulSpriteID2 = 0;
	uint32_t ulFrameCount = 0;
	uint32_t ulScrollX = 0x100;
	uint32_t ulScrollY = 0x100;
	uint32_t ulScrollXSpeed = 0x200;
	uint32_t ulScrollYSpeed = 0x100;

	while (true)
	{
		Hardware_WaitVBL();
		Hardware_FlipScreen();
		Hardware_CopyBackScreen( ulScrollX >> 8, ulScrollY  >> 8);

		ulFrameCount++;
		if (ulFrameCount > 2)
		{
			ulFrameCount = 0;
			ulSpriteID++;
			if (ulSpriteID > 35)
			{
				ulSpriteID = 0;
			}
			ulSpriteID2++;
			if (ulSpriteID2 >= 9)
			{
				ulSpriteID2 = 0;
			}
		}	

		ulScrollX += ulScrollXSpeed;
		if ((ulScrollX >> 8) > 640*3 || (ulScrollX >> 8) == 0)
		{
			ulScrollXSpeed = -ulScrollXSpeed;
		}

		ulScrollY += ulScrollYSpeed;
		if ((ulScrollY >> 8) > 900-480 || (ulScrollY >> 8) == 0)
		{
			ulScrollYSpeed = -ulScrollYSpeed;
		}

		// Draw test sprites
		LIB_Sprites_Draw( eSpriteBank_0, ulSpriteID, 20, 20 );
		LIB_Sprites_DrawFlipped( eSpriteBank_0, ulSpriteID, 70, 20 );
		LIB_Sprites_DrawFlipped( eSpriteBank_0, ulSpriteID, 130, 20 );
		LIB_Sprites_DrawFlipped( eSpriteBank_0, ulSpriteID, -30, 20 );
		LIB_Sprites_Draw( eSpriteBank_1, ulSpriteID2, 50, 20 );

		// Draw the font
		FontModule_DisplayString();

		// check for exit
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

/** ---------------------------------------------------------------------------
	@brief 		Create the back screens
	@ingroup 	MainShell
 --------------------------------------------------------------------------- */
void CreateBackScreens( void )
{
	// create reference back screen 2
	Hardware_SetScreenmode( 2 );	
	uint32_t screenWidth = Hardware_GetScreenWidth();
	uint32_t screenHeight = Hardware_GetScreenHeight();

	// Gradient ..
	for( uint32_t gX = 0; gX < screenWidth; gX += 8)
	{
		LIB_Sprites_Draw( eSpriteBank_3, 0, gX, 0 );
	}
	// Back ground
	for( uint32_t gX = 0; gX < screenWidth; gX += 640)
	{
		LIB_Sprites_Draw( eSpriteBank_4, 0, gX, 300 );
	}

	LIB_Sprites_Draw( eSpriteBank_5, 0, 150,  180 );
	LIB_Sprites_Draw( eSpriteBank_5, 0, 450,  180 );
	LIB_Sprites_Draw( eSpriteBank_5, 0, 1050, 180 );
	LIB_Sprites_Draw( eSpriteBank_5, 0, 1500, 180 );

	// Test worms
	for( uint32_t gX = 0; gX < screenWidth; gX += (rand() % 32) + 20)
	{
		LIB_Sprites_Draw( eSpriteBank_0, 0, gX, 400 );
	}

	// reset the screen mode
	Hardware_CopyBack2ToBack1();
	Hardware_SetScreenmode( 0 );
}


//-----------------------------------------------------------------------------
// End of File: main.c
//-----------------------------------------------------------------------------
