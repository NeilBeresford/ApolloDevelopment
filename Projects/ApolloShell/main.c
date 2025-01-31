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
#include "time.h"
#include "Includes/Hardware.h"
#include "Includes/HWScreen.h"
#include "Includes/ResourceFiles.h"
#include "Includes/ResourceHandling.h"
#include "Includes/FontModule.h"
#include "Includes/LIB_ApolloInput.h"
#include "Includes/LIB_Files.h"
#include "Includes/LIB_Sprites.h"
#include "Includes/LIB_PerlinNoise.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define MAP_WIDTH 		( 1920 )
#define MAP_HEIGHT 		( 900 )
#define NUM_MAPS 		( 2 )

#define MOUSEMOVAREA 	( 100.0f )
#define VISABLE_HEIGHT 	( 360 )
#define VISABLE_WIDTH 	( 640 )
#define MAPSCROLLSPEED 	( 12.0f )


//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------

void CreateBackScreens( void );
void DrawMap( void );

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

ApolloKeyBoardState sKeyboardState;
ApolloJoypadState   sJoypadState;
ApolloMouseState	sMouseState;

int32_t pMapHeight[ MAP_WIDTH ];
uint32_t ulFrames = 0;

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
	uint32_t* 	paletteBuffer 			= NULL;
	uint8_t Banner[] = 
		"\n" \
		"Apollo v4 Demo - by Neil Beresford\n"  \
		"-----------------------------------\n\n" \
		"    _              _    __        __                       \n"  \
		"   / \\   _ __ ___ (_)   \\ \\      / /__  _ __ _ __ ___  ___ \n"  \
		"  / _ \\ | '_ ` _ \\| |____\\ \\ /\\ / / _ \\| '__| '_ ` _ \\/ __|\n"  \
		" / ___ \\| | | | | | |_____\\ V  V / (_) | |  | | | | | \\__ \\ \n"  \
		"/_/   \\_\\_| |_| |_|_|      \\_/\\_/ \\___/|_|  |_| |_| |_|___/\n\n\n"  \
		"writen using ApolloCrossDev and Visual Studio Code\n\n";

	printf(Banner);
	printf("Press 'ESC' to exit\n");

	// Initialize the system and hardware
	LIB_Sprites_Init();
	ResourceHandling_Init();
	// NB removed ResourceHandling_InitStatus( theFileGroups );

	// load in the files...
	printf("Loading misc files...\n");

	if ( LIB_Files_Load("Data/Palettes/paletteSnow.bin",&paletteBuffer, NULL)	== false ) { printf("Failed to load palette\n"); return 1; }

	// load all the sprite groups
	printf("Loading sprite files and remapping...\n");
	ResourceHandling_LoadGroups( theFileGroups );

	printf("Files loaded\n");	
	printf("Create the back screens\n");
	Hardware_SetBackscreenBuffers();
	
	#if 0
	LIB_Sprites_SetClipArea( 20, 40, 640, 480 );
	LIB_Sprites_Draw( 			ResourceHandling_GetGroupStartResource( 4 ) + 4, 0, -100,-20 );
	#endif

	CreateBackScreens();

	printf( "Initializing screen mode and other hardware\n" );

	Hardware_Init();

	HWSCREEN_SetImagePalette( paletteBuffer );

	// Draw panel to all three screens
	Hardware_SetScreenmode( 0 );
	LIB_Sprites_SetClipArea( 0, 0, 640, 480 );
	for( int32_t count = 0; count < 3; count++)
	{
		LIB_Sprites_Draw( 			ResourceHandling_GetGroupStartResource( 4 ) + 4, 0, 0,400 );
		LIB_Sprites_Draw( 			ResourceHandling_GetGroupStartResource( 4 ), 0,     320-20,		10 );
		LIB_Sprites_Draw( 			ResourceHandling_GetGroupStartResource( 4 ) + 2, 0, 320-24-268, 0 );
		LIB_Sprites_Draw( 			ResourceHandling_GetGroupStartResource( 4 ) + 2, 0, 320-24-268, 19 );
		LIB_Sprites_DrawFlipped( 	ResourceHandling_GetGroupStartResource( 4 ) + 2, 0, 320+22, 	0 );
		LIB_Sprites_DrawFlipped( 	ResourceHandling_GetGroupStartResource( 4 ) + 2, 0, 320+22, 	19 );
		Hardware_WaitVBL();
		Hardware_FlipScreen();
	}


	bool bMapMode = false;
	int32_t nScrollX = 400;
	int32_t nScrollY = 400;
	uint32_t nTimeOut = 800;
	uint32_t nMouseGfxOffset = 6;
	uint32_t nMarkerGfx = 0;

	// Water
	uint32_t ulWaterSprIndex = ResourceHandling_GetGroupStartResource( eGroups_Water ) + 1;
	uint32_t ulSprHeight = LIB_Sprites_GetHeight( ulWaterSprIndex );
	uint32_t ulWaterSprNum = 0;

	sMouseState.MouseX_Pointer_Max = 640;
	sMouseState.MouseY_Pointer_Max = 360;
	sMouseState.MouseX_Value_Old = 320;
	sMouseState.MouseY_Value_Old = 180;
	sMouseState.MouseX_Value = 320;
	sMouseState.MouseY_Value = 180;
	LIB_Sprites_SetClipArea( 0, 42, 640, 360 );

	while ( true ) // --nTimeOut > 0
	{
		ulFrames++;
		Hardware_WaitVBL();
		Hardware_FlipScreen();

		if ( bMapMode == false )
		{
			// copy area opf map to screen
			Hardware_SetMapX( nScrollX );	
			Hardware_SetMapY( nScrollY );
			Hardware_CopyBackToScreen();

			#if 1
			for( int32_t i = 0; i < 26; i++ )
			{
				LIB_Sprites_Draw( ResourceHandling_GetGroupStartResource( eGroups_Font ) + 1, i, 20+(i*7), 50 );
				LIB_Sprites_Draw( ResourceHandling_GetGroupStartResource( eGroups_Font ), i, 20+(i*16), 60 );
			}
			#endif
		}
		else
		{
			Hardware_CopyBackScreenMap();

			uint8_t* pS = Hardware_GetScreenPtr();

			uint32_t ulMarkStartX = nScrollX / 3;
			uint32_t ulMarkStartY = nScrollY;
			uint32_t ulMapYSize = (300.0f * (360.0f / 900.0f)) - 1;

			ulMarkStartY = (float)ulMarkStartY * (300.0f / 900.0f);  			 

			for( uint32_t gX = 0; gX < 213 && (ulMarkStartX + gX < 640); gX++ )
			{
				pS[  (ulMarkStartY + 70) * 640 + ulMarkStartX + gX ] = 0x0f;
				pS[  (ulMarkStartY + 70 + ulMapYSize) * 640 + ulMarkStartX + gX ] = 0x0f;
			}
			for( uint32_t gY = 0; gY < ulMapYSize; gY++ )
			{
				pS[  (ulMarkStartY + gY + 70) * 640 + ulMarkStartX ] = 0x0f;
				pS[  (ulMarkStartY + gY + 70) * 640 + ulMarkStartX + 213 ] = 0x0f;
			}
		}

		if ( bMapMode == false )
		{
			// water...
			uint32_t ulMapWidth = 1920;
			uint32_t ulMapHeight = 900;
			int32_t ulYPos = ulMapHeight;
			uint32_t ulBak = ulWaterSprNum;

			for( uint32_t num = 0; num < 3; num++ )
			{
				for ( int32_t gX = 0; gX < ulMapWidth; gX += 256 )
				{
					LIB_Sprites_Draw( ulWaterSprIndex, ulWaterSprNum, gX - nScrollX, ulYPos - nScrollY );
				}

				ulWaterSprNum += 4;
				if ( ulWaterSprNum > 11 ) ulWaterSprNum -= 11;	
				ulYPos += (ulSprHeight / 3) - 1;
			}

			ulWaterSprNum = ulBak;
			if ( !(ulFrames & 3) )
			{
				ulWaterSprNum++;
				if ( ulWaterSprNum > 11 ) ulWaterSprNum = 0;
			}
		}

		// check for exit
#if 1		
		ApolloJoypad( &sJoypadState );
		ApolloKeyboard( &sKeyboardState );	
		ApolloMouse( &sMouseState );

		// simple joystick map position control
		if ( sJoypadState.Joypad_X_Delta != 0 )
		{
			nScrollX += (int32_t)sJoypadState.Joypad_X_Delta * 4;
			if ( nScrollX < 0 ) nScrollX = 0;
			if ( nScrollX > 1920-640 ) nScrollX = 1920-640;
		}
		if ( sJoypadState.Joypad_Y_Delta != 0 )
		{
			nScrollY += (int32_t)sJoypadState.Joypad_Y_Delta * 4;
			if ( nScrollY < 0 ) nScrollY = 0;
			if ( nScrollY > 900-360 ) nScrollY = 900-360;
		}

		if (sKeyboardState.Current_Key == 0x45)
		{
			break;
		}
		if (sKeyboardState.Current_Key == 0x01 )
		{
			bMapMode = bMapMode ? false : true;
		}
		if (sKeyboardState.Current_Key == 0x02)
		{
			CreateBackScreens();
			LIB_Sprites_SetClipArea( 0, 42, 640, 360 );
		}

        // check for joystick button A - change map
		if ( sJoypadState.Joypad_A == true && sJoypadState.Joypad_AActioned == false )
		{
			sJoypadState.Joypad_AActioned = true;
			CreateBackScreens();
			LIB_Sprites_SetClipArea( 0, 42, 640, 360 );
		}
		else if ( sJoypadState.Joypad_A == false && sJoypadState.Joypad_AActioned == true )
		{
			sJoypadState.Joypad_AActioned = false;
		}

        // check for joystick button B - Quit
		if ( sJoypadState.Joypad_B == true ) break;

		// check for joystick button X - change mode
		if ( sJoypadState.Joypad_X == true && sJoypadState.Joypad_XActioned == false )
		{
			sJoypadState.Joypad_XActioned = true;
			bMapMode = bMapMode ? false : true;
		}
		else if ( sJoypadState.Joypad_X == false && sJoypadState.Joypad_XActioned == true )
		{
			sJoypadState.Joypad_XActioned = false;
		}

		// check for mouse button 1 - change map
		if ( bMapMode == true &&sMouseState.Button_State & APOLLOMOUSE_RIGHTCLICK )
		{
			CreateBackScreens();
			LIB_Sprites_SetClipArea( 0, 42, 640, 360 );
		}
		if ( bMapMode == true && sMouseState.Button_State & APOLLOMOUSE_LEFTDOWN )
		{
			int32_t ulMouseX = sMouseState.MouseX_Pointer;
			int32_t ulMouseY = sMouseState.MouseY_Pointer;
			
			nScrollX = (int32_t)((float)ulMouseX * 1920 / 640) - 320;
			nScrollY = (int32_t)((float)ulMouseY * 900 / 360) - 160;
			if ( nScrollX < 0 ) nScrollX = 0;
			if ( nScrollX > 1920-640 ) nScrollX = 1920-640;
			if ( nScrollY < 0 ) nScrollY = 0;
			if ( nScrollY > 900-360 ) nScrollY = 900-360;
		}

		if ( bMapMode == false && sMouseState.Button_State & APOLLOMOUSE_LEFTDOWN )
		{
			uint32_t ulMouseX = sMouseState.MouseX_Pointer;
			uint32_t ulMouseY = sMouseState.MouseY_Pointer;
			uint8_t ulMouseMove = 0;
			// simple joystick map position control
			if ( ulMouseX < MOUSEMOVAREA )
			{
				nScrollX -= (int32_t)(MAPSCROLLSPEED * (float)(MOUSEMOVAREA - (float)ulMouseX) / (float)MOUSEMOVAREA);
				if ( nScrollX < 0 ) nScrollX = 0;
				ulMouseMove |= 1;	
			}
			if ( ulMouseX > VISABLE_WIDTH-MOUSEMOVAREA )
			{
				nScrollX += (int32_t)(MAPSCROLLSPEED * ((float)ulMouseX - (VISABLE_WIDTH-MOUSEMOVAREA)) / MOUSEMOVAREA);
				if ( nScrollX > MAP_WIDTH-VISABLE_WIDTH ) nScrollX = MAP_WIDTH-VISABLE_WIDTH;
				ulMouseMove |= 2;	
			}
			if ( ulMouseY < MOUSEMOVAREA )
			{
				nScrollY -= (int32_t)(MAPSCROLLSPEED * (MOUSEMOVAREA - (float)ulMouseY) / MOUSEMOVAREA);
				if ( nScrollY < 0 ) nScrollY = 0;
				ulMouseMove |= 4;	

			}
			if ( ulMouseY > VISABLE_HEIGHT-MOUSEMOVAREA )
			{
				nScrollY += (int32_t)(MAPSCROLLSPEED * ((float)ulMouseY - (VISABLE_HEIGHT-MOUSEMOVAREA)) / MOUSEMOVAREA);
				if ( nScrollY > MAP_HEIGHT-VISABLE_HEIGHT ) nScrollY = MAP_HEIGHT-VISABLE_HEIGHT;
				ulMouseMove |= 8;	
			}

			ulMouseY += 42;

			switch( ulMouseMove)
			{
				case 1: nMouseGfxOffset = 48; break;
				case 2: nMouseGfxOffset = 16; break;
				case 4: nMouseGfxOffset = 32; break;
				case 8: nMouseGfxOffset = 0; break;
				case 5: nMouseGfxOffset = 40; break;	
				case 9: nMouseGfxOffset = 56; break;
				case 6: nMouseGfxOffset = 24; break;
				case 10: nMouseGfxOffset = 8; break;
				default: nMouseGfxOffset = 38; break;	
			}

			if ( ulMouseMove )
			{
 				LIB_Sprites_Draw( ResourceHandling_GetGroupStartResource( eGroups_Misc ) + 16, nMouseGfxOffset, (ulMouseX >= 640 ? 640 : ulMouseX) - 15, (ulMouseY > 400 ? 400 : ulMouseY)  - 15 );
			}
			else
			{
				LIB_Sprites_Draw( ResourceHandling_GetGroupStartResource( eGroups_Misc ) + 41, nMarkerGfx, (ulMouseX >= 640 ? 640 : ulMouseX) - 30, (ulMouseY > 400 ? 400 : ulMouseY) - 30 );
			}
		    if ( !(ulFrames & 3))
			{
				nMarkerGfx++;
				if ( nMarkerGfx > 9 ) nMarkerGfx = 0;
			}
		}
		else
		{
			uint32_t ulMouseX = sMouseState.MouseX_Pointer;
			uint32_t ulMouseY = sMouseState.MouseY_Pointer + 42;
			LIB_Sprites_Draw( ResourceHandling_GetGroupStartResource( eGroups_Misc ) + 42, nMarkerGfx, (ulMouseX >= 640 ? 640 : ulMouseX) - 30, (ulMouseY > 400 ? 400 : ulMouseY) - 30 );
		    if ( !(ulFrames & 3))
			{
				nMarkerGfx++;
				if ( nMarkerGfx > 9 ) nMarkerGfx = 0;
			}
		}


#endif
	}

	Hardware_Close();

	printf("\n%d frames displayed\n", ulFrames);
	printf("Time played %d seconds\n", ulFrames / 50 );
	printf("Exiting - have a nice day!\n\n");
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
	uint32_t ulGradientSprIndex = ResourceHandling_GetGroupStartResource( 27 ) + 6;

	LIB_Sprites_SetClipArea( 0, 0, screenWidth, screenHeight);

	// Gradient ..
	for( uint32_t gX = 0; gX < screenWidth; gX += 8)
	{
		LIB_Sprites_Draw( ulGradientSprIndex, 0, gX, 0 );
	}

	// Ground
	uint8_t* pScreen = Hardware_GetScreenPtr();
	uint32_t screenX = 0;
	uint32_t ulSoilIndex = ResourceHandling_GetGroupStartResource( 27 ) + 21;
	uint8_t* pSoil = NULL;

	ResourceHandling_Get( ulSoilIndex, eResourceGet_Data, &pSoil );

	for ( uint32_t gx = 0; gx < MAP_WIDTH; gx++ )
	{
		uint32_t refY = pMapHeight[ gx ] - 350;
		for( uint32_t drY = refY; drY < screenHeight; drY ++ )
		{
			pScreen[ (drY * screenWidth) + gx  ] = pSoil[((drY & 0xff) * 256) + (gx & 0xff)];
		}
	}

	// Test worms
	for( uint32_t gX = 0; gX < screenWidth; gX += (rand() & 31) + 20)
	{
		uint32_t gY = pMapHeight[ gX + 30 ] - 350 - 40;
		if ( gY < 900-40 )
		{
			LIB_Sprites_Draw( ResourceHandling_GetGroupStartResource( 3 ), 0, gX, gY );
		}
	}

	// reset the screen mode
	Hardware_CopyBack2ToBack1();
	Hardware_SetScreenmode( 0 );

	LIB_Sprites_SetClipArea( 0, 0, 640, 480 );

}


/** ---------------------------------------------------------------------------
 	@brief 		Creates the map
	@ingroup 	MainShell
 --------------------------------------------------------------------------- */ 
void CreateMap( void )
{
	uint32_t ulIndex = 0;

	// generate maps
	ulIndex = 0;
	//while( ulIndex < NUM_MAPS )
	{
		float fRef = 0.00075f + ( 0.003f / rand()  );

		LIB_PerlinNoise_Init( 1234 );
		LIB_PerlinNoise_GenerateMap( pMapHeight, MAP_WIDTH, 0, fRef );
		//ulIndex++;
	}
}

//-----------------------------------------------------------------------------
// End of File: main.c
//-----------------------------------------------------------------------------
