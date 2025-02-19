/** ---------------------------------------------------------------------------
    @file		main.c
    @defgroup 	AmiWormsAmiWorms
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
#include "Includes/defines.h"
#include "Includes/GlobalData.h"
#include "Includes/Hardware.h"
#include "Includes/HWScreen.h"
#include "Includes/ResourceFiles.h"
#include "Includes/ResourceHandling.h"
#include "Includes/FontModule.h"
#include "Includes/LIB_ApolloInput.h"
#include "Includes/LIB_Files.h"
#include "Includes/LIB_Sprites.h"
#include "Includes/LIB_PerlinNoise.h"
#include "Includes/LIB_SpriteFont.h"
#include "Includes/LIB_SprManager.h"
#include "Includes/GAME_Player.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define BACKSCREENWIDTH      ( 1920 )
#define BACKSCREENHEIGHT     ( 960 )
#define NUM_MAPS             ( 2 )

#define MOUSEMOVAREA         ( 50.0f )
#define VISABLE_HEIGHT       ( 360 )
#define VISABLE_WIDTH        ( 640 )
#define MAPSCROLLSPEED       ( 12.0f )

#define CHECK_KEY( key )     ( sKeyboardState.Keys[ key ] && ( sKeyboardState.KeysProcessed[ key ] == 0 ) )
#define CHECK_KEYDOWN( key ) ( sKeyboardState.Keys[ key ] && ( sKeyboardState.KeysDown[ key ] == 1 ) )

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------

void CreateBackScreens( void );
void DrawMap( void );
void Main_DrawMapScreen( void );
void Main_DrawGameScreen( void );
bool Main_ControlGame( void );
void Main_DrawMapWater( void );
void Main_DrawMapModeWater( void );

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

ApolloKeyBoardState sKeyboardState;
ApolloJoypadState   sJoypadState;
ApolloMouseState    sMouseState;

uint32_t            ulFrames        = 0;
bool                bMapMode        = false;
int32_t             nScrollX        = 400;
int32_t             nScrollY        = 400;
uint32_t            nTimeOut        = 800;
uint32_t            nMouseGfxOffset = 6;
int32_t             nMarkerGfx      = 0;
uint32_t            ulWaterSprIndex = 0;
uint32_t            ulSprHeight     = 0;
uint32_t            ulWaterSprNum   = 0;
uint32_t            ulWaterType     = 1;
int32_t             nMarkerDir      = 1;
int32_t             ulBak           = 0;
uint32_t            nMapType        = 0;
uint32_t            nMapGroup       = 5;
uint32_t            nMapGradient    = 0;
uint32_t            nMapSoil        = 1;
uint32_t            nMapBack        = 2;
uint32_t            nMapGrass       = 3;

uint32_t*           palettes[ 30 ];
uint8_t*            paletteFiles[ 30 ] = {
    "Data/Palettes/paletteArt.bin",          "Data/Palettes/palette-Beach.bin",   "Data/Palettes/paletteCheese.bin",
    "Data/Palettes/paletteConstruction.bin", "Data/Palettes/palette-Desert.bin",  "Data/Palettes/paletteDesert.bin",
    "Data/Palettes/paletteDungeon.bin",      "Data/Palettes/paletteEaster.bin",   "Data/Palettes/palette-Farm.bin",
    "Data/Palettes/paletteFarm.bin",         "Data/Palettes/palette-Forest.bin",  "Data/Palettes/paletteForest.bin",
    "Data/Palettes/paletteFruit.bin",        "Data/Palettes/paletteGulf.bin",     "Data/Palettes/palette-Hell.bin",
    "Data/Palettes/paletteHell.bin",         "Data/Palettes/paletteHospital.bin", "Data/Palettes/paletteJungle.bin",
    "Data/Palettes/paletteManhattan.bin",    "Data/Palettes/paletteMedieval.bin", "Data/Palettes/paletteMusic.bin",
    "Data/Palettes/palettePirate.bin",       "Data/Palettes/paletteSnow.bin",     "Data/Palettes/paletteSpace.bin",
    "Data/Palettes/paletteSports.bin",       "Data/Palettes/paletteTentacle.bin", "Data/Palettes/paletteTime.bin",
    "Data/Palettes/paletteTools.bin",        "Data/Palettes/paletteTribal.bin",   "Data/Palettes/paletteUrban.bin",
};

PSPRHANDLE handles[ 256 ];

//-----------------------------------------------------------------------------
// Code
//-----------------------------------------------------------------------------

/** ---------------------------------------------------------------------------
    @brief 		Main entry point for theAmiWorms
    @ingroup 	AmiWorms
    @return 	int - return code, 0 success
 --------------------------------------------------------------------------- */
uint32_t main( int argc, char* argv[] )
{

    uint32_t keyReturn = 0;
    uint8_t  Banner[]  = "\n"
                         "An Apollo v4 Production - by Neil Beresford\n"
                         "-------------------------------------------\n\n"
                         "    _              _    __        __                       \n"
                         "   / \\   _ __ ___ (_)   \\ \\      / /__  _ __ _ __ ___  ___ \n"
                         "  / _ \\ | '_ ` _ \\| |____\\ \\ /\\ / / _ \\| '__| '_ ` _ \\/ __|\n"
                         " / ___ \\| | | | | | |_____\\ V  V / (_) | |  | | | | | \\__ \\ \n"
                         "/_/   \\_\\_| |_| |_|_|      \\_/\\_/ \\___/|_|  |_| |_| |_|___/\n\n\n"
                         "Written using ApolloCrossDev and Visual Studio Code\n\n";

    // printf( Banner );
    // printf( "Press 'ESC' to exit\n" );

    // Initialize the system and hardware
    LIB_Sprites_Init();
    LIB_SprManager_Init();

    ResourceHandling_Init();
    // ResourceHandling_InitStatus( theFileGroups );

    // load in the files...
    // printf( "Loading misc files...\n" );

    for ( uint32_t nFile = 0; nFile < 30; nFile++ )
    {
        if ( LIB_Files_Load( paletteFiles[ nFile ], &palettes[ nFile ], NULL ) == false )
        {
            printf( "Failed to load palette %s\n", paletteFiles[ nFile ] );
        }
    }

    // load all the sprite groups
    // printf( "Sprite files and remapping scene data...\n" );
    ResourceHandling_LoadGroups( theFileGroups );
    LIB_SpriteFont_CalcFontWidthOffsets();

    // printf( "Files loaded\n" );
    uint32_t nTotalSprs = ResourceHandling_GetTotalNumSprites();
    // printf( "Total number of sprites is %d\n", nTotalSprs );
    // printf( "Total memory for sprite dimentions is number of sprites is %d bytes\n", nTotalSprs * sizeof( SprDimention_t ) );
    ResourceHandling_ScanAndSetSpriteDimentions();

    // printf( "Create the back screens\n" );
    Hardware_SetBackscreenBuffers();

    CreateBackScreens();

    // Create player sprites
    GAME_Player_Init();
    GAME_Player_StartGame();

    // printf( "Initializing screen mode and other hardware\n" );

    Hardware_Init();

    HWSCREEN_SetImagePalette( palettes[ nMapType ] );

    // Draw panel to all three screens
    Hardware_SetScreenmode( 0 );
    LIB_Sprites_SetClipArea( 0, 0, 640, 480 );
    for ( int32_t count = 0; count < 3; count++ )
    {
        LIB_Sprites_Draw( ResourceHandling_GetGroupStartResource( eGroups_Panels ) + 4, 0, 0, 400 );
        LIB_Sprites_Draw( ResourceHandling_GetGroupStartResource( eGroups_Panels ), 0, 320 - 20, 10 );
        LIB_Sprites_Draw( ResourceHandling_GetGroupStartResource( eGroups_Panels ) + 2, 0, 320 - 24 - 268, 0 );
        LIB_Sprites_Draw( ResourceHandling_GetGroupStartResource( eGroups_Panels ) + 2, 0, 320 - 24 - 268, 19 );
        LIB_Sprites_DrawFlipped( ResourceHandling_GetGroupStartResource( eGroups_Panels ) + 2, 0, 320 + 22, 0 );
        LIB_Sprites_DrawFlipped( ResourceHandling_GetGroupStartResource( eGroups_Panels ) + 2, 0, 320 + 22, 19 );

        LIB_Sprites_SetOverwriteColour( 19 );
        LIB_SpriteFont_Draw( eFont_WhiteSmall, 37, 5, "THE BOYS" );
        LIB_SpriteFont_Draw( eFont_WhiteSmall, 37, 24, "MYSTERY" );
        LIB_SpriteFont_Draw( eFont_WhiteSmall, 601 - LIB_SpriteFont_GetStringLength( eFont_WhiteSmall, "ROYALTY" ), 5, "ROYALTY" );
        LIB_SpriteFont_Draw( eFont_WhiteSmall, 601 - LIB_SpriteFont_GetStringLength( eFont_WhiteSmall, "OH NO!!" ), 24, "OH NO!!" );
        LIB_Sprites_SetOverwriteColour( 0 );
        LIB_SpriteFont_Draw( eFont_WhiteSmall, 36, 4, "THE BOYS" );
        LIB_SpriteFont_Draw( eFont_WhiteSmall, 36, 23, "MYSTERY" );
        LIB_SpriteFont_Draw( eFont_WhiteSmall, 600 - LIB_SpriteFont_GetStringLength( eFont_WhiteSmall, "ROYALTY" ), 4, "ROYALTY" );
        LIB_SpriteFont_Draw( eFont_WhiteSmall, 600 - LIB_SpriteFont_GetStringLength( eFont_WhiteSmall, "OH NO!!" ), 23, "OH NO!!" );

        Hardware_WaitVBL();
        Hardware_FlipScreen();
    }
    LIB_Sprites_SetClipArea( 0, 42, 640, 360 );

    // setup the water and mouse and clipping area
    ulWaterSprIndex                = ResourceHandling_GetGroupStartResource( eGroups_Water ) + ulWaterType;
    ulSprHeight                    = LIB_Sprites_GetHeight( ulWaterSprIndex );
    sMouseState.MouseX_Pointer_Max = 640;
    sMouseState.MouseY_Pointer_Max = 360;

    uint32_t ulCol                 = 0xFE000000;
    uint32_t ulInc                 = 0x00080808;
    bool     bCycleDir             = false;

    nScrollX                       = 400;
    nScrollY                       = 300;

    // main loop -
    while ( true )
    {
        ulFrames++;
        Hardware_WaitVBL();
        Hardware_FlipScreen();
        if ( bMapMode == false )
        {
            Main_DrawGameScreen();
        }
        else
        {
            Main_DrawMapScreen();
        }

        // check for exit
        if ( Main_ControlGame() == true )
            break;

        // process the game logic
        LIB_SprManager_Update();
    }

    // terminate the program
    Hardware_Close();

    // printf( "Time played %d minutes %d seconds\n", ulFrames / ( 60 * 60 ), ( ulFrames / 60 ) % 60 );
    // printf( "Exiting - have a nice day!\n\n" );

    return 0;
}

/** ---------------------------------------------------------------------------
    @brief 		Draw the water
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void Main_DrawMapWater( void )
{
    int32_t ulYPos = ( BACKSCREENHEIGHT - 30 - ( ulSprHeight * 0.25f ) );

    ulBak          = ulWaterSprNum;

    LIB_Sprites_SetClipArea( 0, 42, 640, 360 );

    for ( int32_t gX = 0; gX < BACKSCREENWIDTH; gX += 256 )
    {
        LIB_Sprites_Draw( ulWaterSprIndex, ulWaterSprNum, gX - nScrollX, ulYPos - nScrollY );
    }

    ulWaterSprNum = ulBak;
    if ( !( ulFrames & 3 ) )
    {
        ulWaterSprNum++;
        if ( ulWaterSprNum > 11 )
            ulWaterSprNum = 0;
    }
}

/** ---------------------------------------------------------------------------
    @brief 		Draw the game screen
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void Main_DrawGameScreen( void )
{
    uint8_t strBuffer[ 256 ];

    // copy area opf map to screen
    Hardware_SetMapX( nScrollX );
    Hardware_SetMapY( nScrollY );

    Hardware_CopyBackToScreen();
    LIB_SprManager_Draw( nScrollX, nScrollY );
    Main_DrawMapWater();

    Hardware_DrawBlock( 328, 452, 298, 12, 19 );

#if 1

    sprintf( strBuffer, "Mouse postion   x - %04d y - %03d", (uint32_t)sMouseState.MouseX_Pointer, (uint32_t)sMouseState.MouseY_Pointer );
    LIB_SpriteFont_Draw( eFont_CyanSmall, 5, 50, strBuffer );
    sprintf( strBuffer, "Scroll position x - %04d y - %03d", nScrollX, nScrollY );
    LIB_SpriteFont_Draw( eFont_GreenSmall, 5, 59, strBuffer );
    sprintf( strBuffer, "%02d %s", nMapType, ResourceHandling_GetGroupName( eGroups_Terrain01 + nMapType ) );
    LIB_SpriteFont_Draw( eFont_RedSmall, 5, 68, strBuffer );
    sprintf( strBuffer, "%02d <-- Keystate", sKeyboardState.Current_Key );
    LIB_SpriteFont_Draw( eFont_WhiteSmall, 5, 77, strBuffer );

#endif
}

int32_t nNameScroll = SCREENWIDTH;

/** ---------------------------------------------------------------------------
    @brief 		Draw the map onto the screen with the current scroll position
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void Main_DrawMapScreen( void )
{
    uint8_t strBuffer[ 64 ];
    uint8_t strName[ 16 ];
    int32_t strWidth = 0;

    // water for the map ...
    Hardware_CopyBackScreenMap();
    LIB_Sprites_SetClipArea( 0, 42, 640, 348 );
    LIB_SprManager_Draw( nScrollX, nScrollY );
    Main_DrawMapModeWater();

    uint8_t* pS           = Hardware_GetScreenPtr();

    uint32_t ulMarkStartX = nScrollX / 3;
    uint32_t ulMarkStartY = nScrollY;
    uint32_t ulMapYSize   = ( 300.0f * ( 360.0f / 900.0f ) ) - 1;

    ulMarkStartY          = (float)ulMarkStartY * ( 300.0f / 900.0f );

    for ( uint32_t gX = 0; gX < 213 && ( ulMarkStartX + gX < 640 ); gX++ )
    {
        pS[ ( ulMarkStartY + 70 ) * 640 + ulMarkStartX + gX ]              = 0x0f;
        pS[ ( ulMarkStartY + 70 + ulMapYSize ) * 640 + ulMarkStartX + gX ] = 0x0f;
    }
    for ( uint32_t gY = 0; gY < ulMapYSize; gY++ )
    {
        pS[ ( ulMarkStartY + gY + 70 ) * 640 + ulMarkStartX ]       = 0x0f;
        pS[ ( ulMarkStartY + gY + 70 ) * 640 + ulMarkStartX + 213 ] = 0x0f;
    }

    LIB_Sprites_SetClipArea( 0, 42, 640, 360 );

    LIB_SpriteFont_Draw( eFont_WhiteBig, 320 - ( 63 ), 43, "THE MAP" );
    strWidth = ( strlen( strBuffer ) + 1 ) * 18;
    strcpy( strName, ResourceHandling_GetGroupName( eGroups_Terrain01 + nMapType ) + 13 );
    strName[ strlen( strName ) - 1 ] = 0;

    LIB_Sprites_SetClipArea( 328, 453, 298, 12 );
    Hardware_DrawBlock( 328, 452, 298, 12, 19 );

    sprintf( strBuffer, "You are viewing map %02d, named %s", nMapType, strName );
    LIB_SpriteFont_Draw( eFont_GreenSmall, nNameScroll, 453, strBuffer );
    strWidth = LIB_SpriteFont_GetStringLength( eFont_WhiteSmall, strBuffer ) + 18;
    nNameScroll -= 1;
    if ( nNameScroll < 328 - strWidth )
        nNameScroll = 635;
    LIB_Sprites_SetClipArea( 0, 42, 640, 360 );
}

/** ---------------------------------------------------------------------------
    @brief 		Control both the map and game
    @ingroup 	AmiWorms
    @return		bool 		true - quit game
 --------------------------------------------------------------------------- */
bool Main_ControlGame( void )
{
    bool bDoQuit = false;

    ApolloJoypad( &sJoypadState );
    ApolloKeyboard( &sKeyboardState );
    ApolloMouse( &sMouseState );

    // simple joystick map position control
    if ( sJoypadState.Joypad_X_Delta != 0 )
    {
        nScrollX += (int32_t)sJoypadState.Joypad_X_Delta * 4;
        if ( nScrollX < 0 )
            nScrollX = 0;
        if ( nScrollX > 1920 - 640 )
            nScrollX = 1920 - 640;
    }
    if ( sJoypadState.Joypad_Y_Delta != 0 )
    {
        nScrollY += (int32_t)sJoypadState.Joypad_Y_Delta * 4;
        if ( nScrollY < 0 )
            nScrollY = 0;
        if ( nScrollY > 960 - 360 )
            nScrollY = 960 - 360;
    }

    //----------------------------------------------------
    // sKeyboardState.Current_Key = Hardware_ReadKey();
    ApolloKeyboard( &sKeyboardState );

    // Action on key release - for the ESC key
    if ( CHECK_KEY( KEYCODE_ESC ) )
    {
        sKeyboardState.KeysProcessed[ KEYCODE_ESC ] = 1;
        bDoQuit                                     = true;
    }
    if ( CHECK_KEY( KEYCODE_1 ) )
    {
        sKeyboardState.KeysProcessed[ KEYCODE_1 ] = 1;
        bMapMode                                  = bMapMode ? false : true;
    }
    if ( CHECK_KEY( KEYCODE_2 ) )
    {
        sKeyboardState.KeysProcessed[ KEYCODE_2 ] = 1;
        CreateBackScreens();
        LIB_Sprites_SetClipArea( 0, 42, 640, 360 );
    }
    if ( CHECK_KEY( KEYCODE_3 ) )
    {
        sKeyboardState.KeysProcessed[ KEYCODE_3 ] = 1;
        // control the type of map
        nMapType++;
        if ( nMapType > 29 )
            nMapType = 0;
        nMapGroup = nMapType + 5;
        for ( int32_t clearCnt = 0; clearCnt < 3; clearCnt++ )
        {
            Hardware_ClearScreen();
            Hardware_WaitVBL();
            Hardware_FlipScreen();
        }
        CreateBackScreens();
        Hardware_WaitVBL();
        HWSCREEN_SetImagePalette( palettes[ nMapType ] );

        LIB_Sprites_SetClipArea( 0, 42, 640, 360 );
    }
    if ( CHECK_KEYDOWN( KEYCODE_HELP ) )
    {
        uint8_t strBuffer[ 64 ];
        LIB_Sprites_SetOverwriteColour( 19 );
        LIB_SpriteFont_Draw( eFont_CyanBig, 460 + 1, 40 + 1, "Mapped Keys:" );
        LIB_SpriteFont_Draw( eFont_BlueSmall, 460 + 1, 65 + 1, "HELP - This information" );
        LIB_SpriteFont_Draw( eFont_BlueSmall, 460 + 1, 74 + 1, "ESC - Quit Game" );
        LIB_SpriteFont_Draw( eFont_BlueSmall, 460 + 1, 83 + 1, "NUM 1 - Toggle game and map mode" );
        LIB_SpriteFont_Draw( eFont_BlueSmall, 460 + 1, 92 + 1, "NUM 2 - Generate new map" );
        LIB_SpriteFont_Draw( eFont_BlueSmall, 460 + 1, 101 + 1, "NUM 3 - New3 level scheme" );
        LIB_Sprites_SetOverwriteColour( 0 );
        LIB_SpriteFont_Draw( eFont_WhiteBig, 460, 40, "Mapped Keys:" );
        LIB_SpriteFont_Draw( eFont_WhiteSmall, 460, 65, "HELP - This information" );
        LIB_SpriteFont_Draw( eFont_WhiteSmall, 460, 74, "ESC - Quit Game" );
        LIB_SpriteFont_Draw( eFont_WhiteSmall, 460, 83, "NUM 1 - Toggle game and map mode" );
        LIB_SpriteFont_Draw( eFont_WhiteSmall, 460, 92, "NUM 2 - Generate new map" );
        LIB_SpriteFont_Draw( eFont_WhiteSmall, 460, 101, "NUM 3 - New3 level scheme" );
    }

    if ( CHECK_KEYDOWN( KEYCODE_UP ) )
    {
        nScrollY -= 4;
        if ( nScrollY < 0 )
            nScrollY = 0;
    }
    if ( CHECK_KEYDOWN( KEYCODE_DOWN ) )
    {
        nScrollY += 4;
        if ( nScrollY > 960 - 360 )
            nScrollY = 960 - 360;
    }
    if ( CHECK_KEYDOWN( KEYCODE_LEFT ) )
    {
        nScrollX -= 4;
        if ( nScrollX < 0 )
            nScrollX = 0;
    }
    if ( CHECK_KEYDOWN( KEYCODE_RIGHT ) )
    {
        nScrollX += 4;
        if ( nScrollX > 1920 - 640 )
            nScrollX = 1920 - 640;
    }
    //----------------------------------------------------

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
    if ( sJoypadState.Joypad_B == true )
        bDoQuit = true;

    // check for joystick button X - change mode
    if ( sJoypadState.Joypad_X == true && sJoypadState.Joypad_XActioned == false )
    {
        sJoypadState.Joypad_XActioned = true;
        bMapMode                      = bMapMode ? false : true;
    }
    else if ( sJoypadState.Joypad_X == false && sJoypadState.Joypad_XActioned == true )
    {
        sJoypadState.Joypad_XActioned = false;
    }

    // check for mouse button 1 - change map
    if ( sMouseState.Button_State & APOLLOMOUSE_RIGHTCLICK )
    {
        // control the type of map
        nMapType++;
        if ( nMapType > 29 )
            nMapType = 0;
        nMapGroup = nMapType + 5;
        for ( int32_t clearCnt = 0; clearCnt < 3; clearCnt++ )
        {
            Hardware_ClearScreen();
            Hardware_WaitVBL();
            Hardware_FlipScreen();
        }
        CreateBackScreens();
        Hardware_WaitVBL();
        HWSCREEN_SetImagePalette( palettes[ nMapType ] );

        LIB_Sprites_SetClipArea( 0, 42, 640, 360 );
    }
    if ( bMapMode == true && sMouseState.Button_State & APOLLOMOUSE_LEFTDOWN )
    {
        int32_t ulMouseX = sMouseState.MouseX_Pointer;
        int32_t ulMouseY = sMouseState.MouseY_Pointer;

        nScrollX         = (int32_t)( (float)ulMouseX * 1920 / 640 ) - 320;
        nScrollY         = (int32_t)( (float)ulMouseY * 960 / 360 ) - 160;
        if ( nScrollX < 0 )
            nScrollX = 0;
        if ( nScrollX > 1920 - 640 )
            nScrollX = 1920 - 640;
        if ( nScrollY < 0 )
            nScrollY = 0;
        if ( nScrollY > 960 - 360 )
            nScrollY = 960 - 360;
    }

    if ( bMapMode == false && !( sMouseState.Button_State & APOLLOMOUSE_LEFTDOWN ) )
    {
        uint32_t ulMouseX    = sMouseState.MouseX_Pointer;
        uint32_t ulMouseY    = sMouseState.MouseY_Pointer;
        uint8_t  ulMouseMove = 0;
        // simple joystick map position control
        if ( ulMouseX < MOUSEMOVAREA )
        {
            nScrollX -= (int32_t)( MAPSCROLLSPEED * (float)( MOUSEMOVAREA - (float)ulMouseX ) / (float)MOUSEMOVAREA );
            if ( nScrollX < 0 )
                nScrollX = 0;
            ulMouseMove |= 1;
        }
        if ( ulMouseX > VISABLE_WIDTH - MOUSEMOVAREA )
        {
            nScrollX += (int32_t)( MAPSCROLLSPEED * ( (float)ulMouseX - ( VISABLE_WIDTH - MOUSEMOVAREA ) ) / MOUSEMOVAREA );
            if ( nScrollX > BACKSCREENWIDTH - VISABLE_WIDTH )
                nScrollX = BACKSCREENWIDTH - VISABLE_WIDTH;
            ulMouseMove |= 2;
        }
        if ( ulMouseY < MOUSEMOVAREA )
        {
            nScrollY -= (int32_t)( MAPSCROLLSPEED * ( MOUSEMOVAREA - (float)ulMouseY ) / MOUSEMOVAREA );
            if ( nScrollY < 0 )
                nScrollY = 0;
            ulMouseMove |= 4;
        }
        if ( ulMouseY > VISABLE_HEIGHT - MOUSEMOVAREA )
        {
            nScrollY += (int32_t)( MAPSCROLLSPEED * ( (float)ulMouseY - ( VISABLE_HEIGHT - MOUSEMOVAREA ) ) / MOUSEMOVAREA );
            if ( nScrollY > BACKSCREENHEIGHT - VISABLE_HEIGHT )
                nScrollY = BACKSCREENHEIGHT - VISABLE_HEIGHT;
            ulMouseMove |= 8;
        }

        ulMouseY += 42;

        switch ( ulMouseMove )
        {
            case 1:
                nMouseGfxOffset = 48;
                break;
            case 2:
                nMouseGfxOffset = 16;
                break;
            case 4:
                nMouseGfxOffset = 32;
                break;
            case 8:
                nMouseGfxOffset = 0;
                break;
            case 5:
                nMouseGfxOffset = 40;
                break;
            case 9:
                nMouseGfxOffset = 56;
                break;
            case 6:
                nMouseGfxOffset = 24;
                break;
            case 10:
                nMouseGfxOffset = 8;
                break;
            default:
                nMouseGfxOffset = 38;
                break;
        }

        if ( ulMouseMove )
        {
            LIB_Sprites_Draw( ResourceHandling_GetGroupStartResource( eGroups_Misc ) + 16, nMouseGfxOffset,
                              ( ulMouseX >= 640 ? 640 : ulMouseX ) - 15, ( ulMouseY > 400 ? 400 : ulMouseY ) - 15 );
        }
        else
        {
            LIB_Sprites_Draw( ResourceHandling_GetGroupStartResource( eGroups_Misc ) + 41, nMarkerGfx,
                              ( ulMouseX >= 640 ? 640 : ulMouseX ) - 30, ( ulMouseY > 400 ? 400 : ulMouseY ) - 30 );
        }
        if ( !( ulFrames & 3 ) )
        {
            nMarkerGfx += nMarkerDir;
            if ( nMarkerGfx == 9 || nMarkerGfx == 0 )
                nMarkerDir = -nMarkerDir;
        }
    }
    else
    {
        uint32_t ulMouseX = sMouseState.MouseX_Pointer;
        uint32_t ulMouseY = sMouseState.MouseY_Pointer + 42;
        LIB_Sprites_Draw( ResourceHandling_GetGroupStartResource( eGroups_Misc ) + 42, nMarkerGfx,
                          ( ulMouseX >= 640 ? 640 : ulMouseX ) - 30, ( ulMouseY > 400 ? 400 : ulMouseY ) - 30 );
        if ( !( ulFrames & 3 ) )
        {
            nMarkerGfx += nMarkerDir;
            if ( nMarkerGfx == 9 || nMarkerGfx == 0 )
                nMarkerDir = -nMarkerDir;
        }
    }

    return ( bDoQuit );
}

/** ---------------------------------------------------------------------------
    @brief 		Create the back screens
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void CreateBackScreens( void )
{
    // Generate the map
    CreateMap();

    // create reference back screen 2
    Hardware_SetScreenmode( 2 );
    uint32_t screenWidth        = Hardware_GetScreenWidth();
    uint32_t screenHeight       = Hardware_GetScreenHeight();
    uint32_t ulGradientSprIndex = ResourceHandling_GetGroupStartResource( nMapGroup ) + nMapGradient;
    uint32_t nBackSprIndex      = ResourceHandling_GetGroupStartResource( nMapGroup ) + nMapBack;
    uint32_t nBackWidth         = LIB_Sprites_GetWidth( nBackSprIndex );
    uint32_t nBackHeight        = LIB_Sprites_GetHeight( nBackSprIndex );

    LIB_Sprites_SetClipArea( 0, 0, screenWidth, screenHeight );

    // Gradient then back ..
    for ( uint32_t gX = 0; gX < screenWidth; gX += 8 )
    {
        LIB_Sprites_Draw( ulGradientSprIndex, 0, gX, 0 );
    }
    for ( uint32_t gX = 0; gX < BACKSCREENWIDTH; gX += nBackWidth )
    {
        LIB_Sprites_Draw( nBackSprIndex, 0, gX, 900 - nBackHeight );
    }

    // Ground
    uint8_t* pScreen       = Hardware_GetScreenPtr();
    uint32_t screenX       = 0;
    uint32_t ulSoilIndex   = ResourceHandling_GetGroupStartResource( nMapGroup ) + nMapSoil;
    uint8_t* pSoil         = NULL;
    uint8_t* pGrass        = NULL;
    uint32_t ulGrassIndex  = ResourceHandling_GetGroupStartResource( nMapGroup ) + nMapGrass;
    uint32_t ulGrassWidth  = LIB_Sprites_GetWidth( ulGrassIndex );
    uint32_t ulGrassHeight = LIB_Sprites_GetHeight( ulGrassIndex ) / 2;

    ResourceHandling_Get( ulSoilIndex, eResourceGet_Data, &pSoil );
    ResourceHandling_Get( ulGrassIndex, eResourceGet_Data, &pGrass );

    for ( uint32_t gx = 0; gx < BACKSCREENWIDTH; gx++ )
    {
        uint32_t refY = sGlobalData.pMapHeight[ gx ] - 350;

        for ( uint32_t grY = refY - ulGrassHeight, nCnt = 0; grY < refY; grY++, nCnt++ )
        {
            uint8_t pixel = pGrass[ ( nCnt * ulGrassWidth ) + ( gx % 64 ) ];

            if ( pixel == 0 )
                continue;

            pScreen[ ( grY * screenWidth ) + gx ] = pixel;
        }

        for ( uint32_t drY = refY; drY < screenHeight; drY++ )
        {
            pScreen[ ( drY * screenWidth ) + gx ] = pSoil[ ( ( drY & 0xff ) * 256 ) + ( gx & 0xff ) ];
        }
    }

#if 0
	// Test worms
	for( uint32_t gX = 0; gX < screenWidth; gX += (rand() & 31) + 20)
	{
		uint32_t gY = sGlobalData.pMapHeight[ gX + 30 ] - 350 - 40;
		if ( gY < 900-40 )
		{
			LIB_Sprites_Draw( ResourceHandling_GetGroupStartResource( 3 ), 0, gX, gY );
		}
	}
#endif

    // colour the remainder with water
    Hardware_DrawBackScreenBlock( 0, 900, BACKSCREENWIDTH, 60, 0xD3D3D3D3 );

    // reset the screen mode
    Hardware_CopyBack2ToBack1();
    Hardware_SetScreenmode( 0 );

    LIB_Sprites_SetClipArea( 0, 0, 640, 480 );
}

/** ---------------------------------------------------------------------------
    @brief 		Draw the map water, in Map Mode
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void Main_DrawMapModeWater( void )
{
    int32_t ulYPos = 374 - ( ( ulSprHeight / 4 ) );
    int32_t nWidth = 256 / 3;

    ulBak          = ulWaterSprNum;

    for ( int32_t gX = 0; gX < 640; gX += nWidth )
    {
        LIB_Sprites_SetClipArea( 0, ulYPos, 640, 12 );
        LIB_Sprites_DrawMap( ulWaterSprIndex, ulWaterSprNum, gX, ulYPos );
    }

    ulWaterSprNum = ulBak;
    if ( !( ulFrames & 3 ) )
    {
        ulWaterSprNum++;
        if ( ulWaterSprNum > 11 )
            ulWaterSprNum = 0;
    }

    LIB_Sprites_SetClipArea( 0, 42, 640, 348 );
}

/** ---------------------------------------------------------------------------
    @brief 		Creates the map
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void CreateMap( void )
{
    uint32_t ulIndex = 0;

    // generate maps
    ulIndex    = 0;
    float fRef = 0.00075f + ( 0.003f / rand() );

    LIB_PerlinNoise_Init( 123456 );
    LIB_PerlinNoise_GenerateMap( sGlobalData.pMapHeight, BACKSCREENWIDTH, 0, fRef );
}

//-----------------------------------------------------------------------------
// End of File: main.c
//-----------------------------------------------------------------------------
