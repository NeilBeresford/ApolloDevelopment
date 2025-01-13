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
#include "stdlib.h"
#include "math.h"
#include "string.h"

#include "Includes/Hardware.h"
#include "Includes/HWScreen.h"
#include "Includes/ResourceHandling.h"
#include "Includes/FontModule.h"
#include "Includes/LIB_Files.h"
#include "Includes/LIB_Sprites.h"
#include "Includes/LIB_PerlinNoise.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define MAP_WIDTH 	2560
#define NUM_MAPS 	2

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------

void CreateBackScreens( void );
void DrawMap( void );

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

int32_t pMapHeight[ MAP_WIDTH ];

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
	uint32_t 	ulPanelBottom			= 0;
	uint32_t 	ulTestSpriteCompSize	= 0;
	uint8_t* 	fileBuffer 				= NULL;
	uint8_t* 	TestSpriteWorm 			= NULL;
	uint8_t* 	TestSpriteWorm2			= NULL;
	uint8_t* 	TestGround				= NULL;
	uint8_t* 	TestGradient			= NULL;
	uint8_t* 	TestBack				= NULL;
	uint8_t* 	TestFridge				= NULL;
	uint8_t* 	PanelBottom				= NULL;
	uint32_t* 	paletteBuffer 			= NULL;

	uint32_t* 	pFilePtrs[ 10 ];
	uint32_t 	pFileSizes[ 10 ];

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
	if ( LIB_Files_Load("Data/Panels/WormsPanel.png-640-80.RAW",&PanelBottom, &ulPanelBottom)	== false ) { printf("Failed to load Panel\n"); return 1; }
	if ( LIB_Files_Load("Data/Panels/KO.png-41-26.RAW",&pFilePtrs[ 0 ], &pFileSizes[ 0 ])	== false ) { printf("Failed to load KO\n"); return 1; }
	if ( LIB_Files_Load("Data/Panels/WormsHealth.png-268-28.RAW",&pFilePtrs[ 1 ], &pFileSizes[ 1 ])	== false ) { printf("Failed to load Health\n"); return 1; }

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
	LIB_Sprites_RegisterBank( eSpriteBank_6, eSpriteType_Raw,			106, PanelBottom, 	 ulPanelBottom,			1, 640, 80 );
	LIB_Sprites_RegisterBank( 7, eSpriteType_Raw, 107, pFilePtrs[ 0 ], pFileSizes[ 0 ],		1, 41, 26 );
	LIB_Sprites_RegisterBank( 8, eSpriteType_Raw, 108, pFilePtrs[ 1 ], pFileSizes[ 1 ],		1, 268, 28 );

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

	// Draw panel to all three screens
	for( int32_t count = 0; count < 3; count++)
	{
		LIB_Sprites_Draw( eSpriteBank_6, 0, 0, 400 );
		LIB_Sprites_Draw( 7, 0, 320-20, 10 );
		LIB_Sprites_Draw( 8, 0, 320-24-268, 0 );
		LIB_Sprites_Draw( 8, 0, 320-24-268, 19 );
		LIB_Sprites_DrawFlipped( 8, 0, 320+22, 0 );
		LIB_Sprites_DrawFlipped( 8, 0, 320+22, 19 );

		Hardware_FlipScreen();
		LIB_Sprites_Draw( eSpriteBank_6, 0, 0, 400 );
	}

	uint32_t ulSpriteID = 0;
	uint32_t ulSpriteID2 = 0;
	uint32_t ulFrameCount = 0;
	int32_t ulScrollX = 0x100;
	int32_t ulScrollY = 0x100;
	int32_t ulScrollXSpeed = 0x100;
	int32_t ulScrollYSpeed = 0x200;
	bool bMapMode = false;

	while (true)
	{
		Hardware_WaitVBL();
		Hardware_FlipScreen();

		if ( bMapMode == false )
		{
			Hardware_CopyBackScreen( ulScrollX >> 8, ulScrollY >> 8 );
		}
		else
		{
			Hardware_CopyBackScreenMap();
		}

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

		#if 0
		ulScrollX += ulScrollXSpeed;
		if ((ulScrollX >> 8) > 640*3 || (ulScrollX >> 8) < 0)
		{
			if ( (ulScrollX >> 8) < 0 )
			{
				ulScrollX = 0;
			}
			ulScrollXSpeed = -ulScrollXSpeed;
		}

		ulScrollY += ulScrollYSpeed;
		if ((ulScrollY >> 8) > 900-480 || (ulScrollY >> 8) < 0)
		{
			if ( (ulScrollY >> 8) < 0 )
			{
				ulScrollY = 0;
			}
			ulScrollYSpeed = -ulScrollYSpeed;
		}
		#endif

		// Draw test sprites
		LIB_Sprites_Draw( eSpriteBank_0, ulSpriteID, 20, 80 );
		LIB_Sprites_DrawFlipped( eSpriteBank_0, ulSpriteID, 70, 80 );
		LIB_Sprites_DrawFlipped( eSpriteBank_0, ulSpriteID, 130, 80 );
		LIB_Sprites_DrawFlipped( eSpriteBank_0, ulSpriteID, -30, 80 );
		LIB_Sprites_Draw( eSpriteBank_1, ulSpriteID2, 50, 80 );

		// Draw the font
		FontModule_DisplayString();

		// check for exit
		keyReturn = Hardware_ReadKey();
		if (keyReturn == 0x45)
		{
			break;
		}
		if (keyReturn == 0x01 )
		{
			bMapMode = bMapMode ? false : true;

			while( Hardware_ReadKey() == 0x01 )
			{
				Hardware_WaitVBL();
			}
		}

		if (keyReturn == 0x02)
		{
			CreateBackScreens();
		}

		if ( bMapMode == false)
		{
			// keyboard move map...
			// left
			uint32_t sX = ulScrollX;
			uint32_t sY = ulScrollY;

			if (keyReturn == 0x4F && sX > 0 )
			{
				sX -= 0x100;
				if ( sX < 0 )
				{
					sX = 0;
				}
				ulScrollX = sX;
			}
			// right
			if (keyReturn == 0x4E && sX < (640*3)<<8 )
			{
				sX += 0x100;
				if ( sX > (640*3)<<8 )
				{
					sX = (640*3)<<8;
				}
				ulScrollX = sX;
			}
			// top
			if (keyReturn == 0x4C && sY > 0 )
			{
				sY -= 0x100;
				if ( sY < 0 )
				{
					sY = 0;
				}
				ulScrollY = sY;
			}
			// right
			if (keyReturn == 0x4D && sY < ((900-360)<<8) )
			{
				sY += 0x100;
				if ( sY > ((900-360)<<8) )
				{
					sY = ((900-360)<<8);
				}
				ulScrollY = sY;
			}

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
	// Generate the map
	CreateMap();

	// create reference back screen 2
	Hardware_SetScreenmode( 2 );	
	uint32_t screenWidth = Hardware_GetScreenWidth();
	uint32_t screenHeight = Hardware_GetScreenHeight();

	// Gradient ..
	for( uint32_t gX = 0; gX < screenWidth; gX += 8)
	{
		LIB_Sprites_Draw( eSpriteBank_3, 0, gX, 0 );
	}

	// Ground
	uint8_t* pScreen = Hardware_GetScreenPtr();
	uint32_t screenX = 0;

	for ( uint32_t gx = 0; gx < MAP_WIDTH; gx++ )
	{
		uint32_t refY = pMapHeight[ gx ] - 350;

		for( uint32_t drY = refY; drY < screenHeight; drY ++ )
		{
			pScreen[ gx + (drY * screenWidth) ] = 40;
		}
	}

	// Test worms
	for( uint32_t gX = 0; gX < screenWidth; gX += (rand() % 32) + 20)
	{
		LIB_Sprites_Draw( eSpriteBank_0, 0, gX, 150 );
	}

	// reset the screen mode
	Hardware_CopyBack2ToBack1();
	Hardware_SetScreenmode( 0 );


}


/** ---------------------------------------------------------------------------
 	@brief 		Creates the map
	@ingroup 	MainShell
 --------------------------------------------------------------------------- */ 
void CreateMap( void )
{
	uint32_t ulIndex = 0;

	// generate the maps
	ulIndex = 0;
	while( ulIndex < NUM_MAPS )
	{
		float fRef = 0.00075f + ( 0.003f / rand()  );

		LIB_PerlinNoise_Init( 1234 );
		LIB_PerlinNoise_GenerateMap( pMapHeight, MAP_WIDTH, 0, fRef );
		ulIndex++;
	}
}

//-----------------------------------------------------------------------------
// End of File: main.c
//-----------------------------------------------------------------------------
