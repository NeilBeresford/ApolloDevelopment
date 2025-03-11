/** ---------------------------------------------------------------------------
    @file		Scene_Game.c
    @defgroup 	AmiWorms Apollo V4 source
    @brief		Apollo V4 development - Shell
    @date		2025-01-31
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
#include "../Includes/defines.h"
#include "../Includes/GlobalData.h"
#include "../Includes/Hardware.h"
#include "../Includes/HWScreen.h"
#include "../Includes/FontModule.h"
#include "../Includes/LIB_ApolloInput.h"
#include "../Includes/LIB_Files.h"
#include "../Includes/LIB_Sprites.h"
#include "../Includes/LIB_PerlinNoise.h"
#include "../Includes/LIB_SpriteFont.h"
#include "../Includes/LIB_SprManager.h"
#include "../Modules/Module-Scene.h"
#include "../Includes/GAME_Player.h"
#include "../Includes/ResourceFiles.h"
#include "../Includes/ResourceHandling.h"

extern void      Main_Start_Track( int32_t nTrack );
extern uint32_t* palettes[ 30 ];

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define BACKSCREENWIDTH           ( 1920 )
#define BACKSCREENHEIGHT          ( 960 )
#define NUM_MAPS                  ( 2 )

#define MOUSEMOVAREA              ( 50.0f )
#define VISABLE_HEIGHT            ( 360 )
#define VISABLE_WIDTH             ( 640 )
#define MAPSCROLLSPEED            ( 12.0f )

#define CHECK_KEY( key )          ( sKeyboardState.Keys[ key ] && ( sKeyboardState.KeysProcessed[ key ] == 0 ) )
#define CHECK_KEYDOWN( key )      ( sKeyboardState.Keys[ key ] && ( sKeyboardState.KeysDown[ key ] == 1 ) )
#define CHECK_KEY_RELEASED( key ) ( sKeyboardState.KeysProcessed[ key ] && ( sKeyboardState.KeysDown[ key ] == 0 ) )

//-----------------------------------------------------------------------------
// Typedefs and Enums
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------

void SceneGame_CreateBackScreens( void );
void SceneGame_DrawMap( void );
void SceneGame_DrawMapScreen( void );
void SceneGame_DrawGameScreen( void );
bool SceneGame_ControlGame( void );
void SceneGame_DrawMapWater( void );
void SceneGame_DrawMapModeWater( void );

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

int32_t             nNameScroll = SCREENWIDTH;
ApolloKeyBoardState sKeyboardState;
ApolloJoypadState   sJoypadState;
ApolloMouseState    sMouseState;

// uint32_t            ulFrames = 0;
// bool                bMapMode = false;
//  int32_t             nScrollX        = 400;
//  int32_t             nScrollY        = 400;
uint32_t nTimeOut        = 800;
uint32_t nMouseGfxOffset = 6;
int32_t  nMarkerGfx      = 0;
uint32_t ulWaterSprIndex = 0;
uint32_t ulSprHeight     = 0;
uint32_t ulWaterSprNum   = 0;
uint32_t ulWaterType     = 1;
int32_t  nMarkerDir      = 1;
int32_t  ulBak           = 0;
uint32_t nMapType        = 0;
uint32_t nMapGroup       = 5;
uint32_t nMapGradient    = 0;
uint32_t nMapSoil        = 1;
uint32_t nMapBack        = 2;
uint32_t nMapGrass       = 3;

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

/** ---------------------------------------------------------------------------
    @brief 		Initialize the sprite data editor
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void SceneGame_Init( void )
{
    SceneGame_CreateBackScreens();

    // Draw panel to all three screens
    Hardware_SetScreenmode( 0 );
    LIB_Sprites_SetClipArea( 0, 0, 640, 480 );
    for ( int32_t count = 0; count < 3; count++ )
    {
        Hardware_ClearAllScreen();
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
    sMouseState.MouseX_Pointer     = 310;
    sMouseState.MouseY_Pointer     = 230;
    sMouseState.MouseX_Value_Old   = 310;
    sMouseState.MouseY_Value_Old   = 230;
    sGlobalData.nScrollX           = 400;
    sGlobalData.nScrollY           = 300;
    sGlobalData.bMapMode           = false;

    GAME_Player_StartGame();

    sKeyboardState.Previous_Key = NOKEY;
    sKeyboardState.Current_Key  = NOKEY;
    ApolloKeyboardClear( &sKeyboardState );
    ApolloMouse_SetXY( &sMouseState, 310, 230 );

    // Start the music
    Hardware_StopAudio( 0 );
    Main_Start_Track( 2 );
}

/** ---------------------------------------------------------------------------
    @brief 		Close the sprite data editor
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void SceneGame_Close( void )
{
}

/** ---------------------------------------------------------------------------
    @brief 		Draw the sprite data editor
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void SceneGame_Draw( void )
{
    if ( sGlobalData.bMapMode == false )
    {
        SceneGame_DrawGameScreen();
    }
    else
    {
        SceneGame_DrawMapScreen();
    }
}

/** ---------------------------------------------------------------------------
    @brief 		Update the sprite data editor
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void SceneGame_Update( void )
{
    // check for exit
    if ( SceneGame_ControlGame() == true )
    {
        sGlobalData.nNewScene = 1;
    }
}

//-----------------------------------------------------------------------------
// Internal Functionality
//-----------------------------------------------------------------------------

/** ---------------------------------------------------------------------------
    @brief 		Draw the water
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void SceneGame_DrawMapWater( void )
{
    int32_t ulYPos = ( BACKSCREENHEIGHT - 30 - ( ulSprHeight * 0.25f ) );

    ulBak          = ulWaterSprNum;

    LIB_Sprites_SetClipArea( 0, 42, 640, 360 );

    for ( int32_t gX = 0; gX < BACKSCREENWIDTH; gX += 256 )
    {
        LIB_Sprites_Draw( ulWaterSprIndex, ulWaterSprNum, gX - sGlobalData.nScrollX, ulYPos - sGlobalData.nScrollY );
    }

    ulWaterSprNum = ulBak;
    if ( !( sGlobalData.ulFrames & 3 ) )
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
void SceneGame_DrawGameScreen( void )
{
    uint8_t strBuffer[ 256 ];
    // copy area opf map to screen
    Hardware_SetMapX( sGlobalData.nScrollX );
    Hardware_SetMapY( sGlobalData.nScrollY );
    Hardware_CopyBackToScreen();
    LIB_SprManager_Draw( sGlobalData.nScrollX, sGlobalData.nScrollY );
    SceneGame_DrawMapWater();
    Hardware_DrawBlock( 328, 452, 298, 12, 19 );
    Hardware_DrawBlock( 328, 452, 298, 12, 19 );

#if 1

    sprintf( strBuffer, "Mouse postion   x - %04d y - %03d", (uint32_t)sMouseState.MouseX_Pointer, (uint32_t)sMouseState.MouseY_Pointer );
    LIB_SpriteFont_Draw( eFont_CyanSmall, 5, 50, strBuffer );
    sprintf( strBuffer, "Scroll position x - %04d y - %03d", sGlobalData.nScrollX, sGlobalData.nScrollY );
    LIB_SpriteFont_Draw( eFont_GreenSmall, 5, 59, strBuffer );
    sprintf( strBuffer, "%02d %s", nMapType, ResourceHandling_GetGroupName( eGroups_Terrain01 + nMapType ) );
    LIB_SpriteFont_Draw( eFont_RedSmall, 5, 68, strBuffer );
    sprintf( strBuffer, "%02d <-- Keystate", sKeyboardState.Current_Key );
    LIB_SpriteFont_Draw( eFont_WhiteSmall, 5, 77, strBuffer );

#endif
}

/** ---------------------------------------------------------------------------
    @brief 		Draw the map onto the screen with the current scroll position
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void SceneGame_DrawMapScreen( void )
{
    uint8_t strBuffer[ 64 ];
    uint8_t strName[ 16 ];
    int32_t strWidth = 0;

    // water for the map ...
    Hardware_CopyBackScreenMap();
    LIB_Sprites_SetClipArea( 0, 42, 640, 348 );
    LIB_SprManager_Draw( sGlobalData.nScrollX, sGlobalData.nScrollY );
    SceneGame_DrawMapModeWater();

    uint8_t* pS           = Hardware_GetScreenPtr();

    uint32_t ulMarkStartX = sGlobalData.nScrollX / 3;
    uint32_t ulMarkStartY = sGlobalData.nScrollY;
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
bool SceneGame_ControlGame( void )
{
    bool bDoQuit = false;

    ApolloJoypad( &sJoypadState );
    ApolloKeyboard( &sKeyboardState );
    ApolloMouse( &sMouseState );

    // simple joystick map position control
    if ( sJoypadState.Joypad_X_Delta != 0 )
    {
        sGlobalData.nScrollX += (int32_t)sJoypadState.Joypad_X_Delta * 4;
        if ( sGlobalData.nScrollX < 0 )
            sGlobalData.nScrollX = 0;
        if ( sGlobalData.nScrollX > 1920 - 640 )
            sGlobalData.nScrollX = 1920 - 640;
    }
    if ( sJoypadState.Joypad_Y_Delta != 0 )
    {
        sGlobalData.nScrollY += (int32_t)sJoypadState.Joypad_Y_Delta * 4;
        if ( sGlobalData.nScrollY < 0 )
            sGlobalData.nScrollY = 0;
        if ( sGlobalData.nScrollY > 960 - 360 )
            sGlobalData.nScrollY = 960 - 360;
    }

    //----------------------------------------------------
    // sKeyboardState.Current_Key = Hardware_ReadKey();
    ApolloKeyboard( &sKeyboardState );

    if ( CHECK_KEY( KEYCODE_F1 ) )
    {
        LIB_SprManager_RemoveAll();
        GAME_Player_StartGame();
    }

    // Action on key release - for the ESC key
    if ( CHECK_KEY( KEYCODE_ESC ) )
    {
        sKeyboardState.KeysProcessed[ KEYCODE_ESC ] = 1;
    }
    if ( sKeyboardState.Previous_Key == KEYCODE_ESC && sKeyboardState.Current_Key == NOKEY )
    {
        sKeyboardState.KeysProcessed[ KEYCODE_ESC ] = 0;
        bDoQuit                                     = true;
    }
    if ( CHECK_KEY( KEYCODE_1 ) )
    {
        sKeyboardState.KeysProcessed[ KEYCODE_1 ] = 1;
        sGlobalData.bMapMode                      = sGlobalData.bMapMode ? false : true;
    }
    if ( CHECK_KEY( KEYCODE_2 ) )
    {
        sKeyboardState.KeysProcessed[ KEYCODE_2 ] = 1;
        SceneGame_CreateBackScreens();
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
        SceneGame_CreateBackScreens();
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
        sGlobalData.nScrollY -= 4;
        if ( sGlobalData.nScrollY < 0 )
            sGlobalData.nScrollY = 0;
    }
    if ( CHECK_KEYDOWN( KEYCODE_DOWN ) )
    {
        sGlobalData.nScrollY += 4;
        if ( sGlobalData.nScrollY > 960 - 360 )
            sGlobalData.nScrollY = 960 - 360;
    }
    if ( CHECK_KEYDOWN( KEYCODE_LEFT ) )
    {
        sGlobalData.nScrollX -= 4;
        if ( sGlobalData.nScrollX < 0 )
            sGlobalData.nScrollX = 0;
    }
    if ( CHECK_KEYDOWN( KEYCODE_RIGHT ) )
    {
        sGlobalData.nScrollX += 4;
        if ( sGlobalData.nScrollX > 1920 - 640 )
            sGlobalData.nScrollX = 1920 - 640;
    }
    //----------------------------------------------------

    // check for joystick button A - change map
    if ( sJoypadState.Joypad_A == true && sJoypadState.Joypad_AActioned == false )
    {
        sJoypadState.Joypad_AActioned = true;
        SceneGame_CreateBackScreens();
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
        sGlobalData.bMapMode          = sGlobalData.bMapMode ? false : true;
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
        SceneGame_CreateBackScreens();
        Hardware_WaitVBL();
        HWSCREEN_SetImagePalette( palettes[ nMapType ] );

        LIB_Sprites_SetClipArea( 0, 42, 640, 360 );
    }
    if ( sGlobalData.bMapMode == true && sMouseState.Button_State & APOLLOMOUSE_LEFTDOWN )
    {
        int32_t ulMouseX     = sMouseState.MouseX_Pointer;
        int32_t ulMouseY     = sMouseState.MouseY_Pointer;

        sGlobalData.nScrollX = (int32_t)( (float)ulMouseX * 1920 / 640 ) - 320;
        sGlobalData.nScrollY = (int32_t)( (float)ulMouseY * 960 / 360 ) - 160;
        if ( sGlobalData.nScrollX < 0 )
            sGlobalData.nScrollX = 0;
        if ( sGlobalData.nScrollX > 1920 - 640 )
            sGlobalData.nScrollX = 1920 - 640;
        if ( sGlobalData.nScrollY < 0 )
            sGlobalData.nScrollY = 0;
        if ( sGlobalData.nScrollY > 960 - 360 )
            sGlobalData.nScrollY = 960 - 360;
    }

    if ( sGlobalData.bMapMode == false && !( sMouseState.Button_State & APOLLOMOUSE_LEFTDOWN ) )
    {
        int32_t ulMouseX, ulMouseY;
        uint8_t ulMouseMove = 0;
        sGlobalData.nMouseX = sMouseState.MouseX_Pointer;
        sGlobalData.nMouseY = sMouseState.MouseY_Pointer;
        ulMouseX            = sGlobalData.nMouseX;
        ulMouseY            = sGlobalData.nMouseY;

        // simple joystick map position control
        if ( ulMouseX < MOUSEMOVAREA )
        {
            sGlobalData.nScrollX -= (int32_t)( MAPSCROLLSPEED * (float)( MOUSEMOVAREA - (float)ulMouseX ) / (float)MOUSEMOVAREA );
            if ( sGlobalData.nScrollX < 0 )
                sGlobalData.nScrollX = 0;
            ulMouseMove |= 1;
        }
        if ( ulMouseX > VISABLE_WIDTH - MOUSEMOVAREA )
        {
            sGlobalData.nScrollX += (int32_t)( MAPSCROLLSPEED * ( (float)ulMouseX - ( VISABLE_WIDTH - MOUSEMOVAREA ) ) / MOUSEMOVAREA );
            if ( sGlobalData.nScrollX > BACKSCREENWIDTH - VISABLE_WIDTH )
                sGlobalData.nScrollX = BACKSCREENWIDTH - VISABLE_WIDTH;
            ulMouseMove |= 2;
        }
        if ( ulMouseY < MOUSEMOVAREA )
        {
            sGlobalData.nScrollY -= (int32_t)( MAPSCROLLSPEED * ( MOUSEMOVAREA - (float)ulMouseY ) / MOUSEMOVAREA );
            if ( sGlobalData.nScrollY < 0 )
                sGlobalData.nScrollY = 0;
            ulMouseMove |= 4;
        }
        if ( ulMouseY > VISABLE_HEIGHT - MOUSEMOVAREA )
        {
            sGlobalData.nScrollY += (int32_t)( MAPSCROLLSPEED * ( (float)ulMouseY - ( VISABLE_HEIGHT - MOUSEMOVAREA ) ) / MOUSEMOVAREA );
            if ( sGlobalData.nScrollY > BACKSCREENHEIGHT - VISABLE_HEIGHT )
                sGlobalData.nScrollY = BACKSCREENHEIGHT - VISABLE_HEIGHT;
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
            LIB_Sprites_Draw( ResourceHandling_GetGroupStartResource( eGroups_Misc ) + 16, nMouseGfxOffset, ( ulMouseX >= 640 ? 640 : ulMouseX ) - 15, ( ulMouseY > 400 ? 400 : ulMouseY ) - 15 );
        }
        else
        {
            LIB_Sprites_Draw( ResourceHandling_GetGroupStartResource( eGroups_Misc ) + 41, nMarkerGfx, ( ulMouseX >= 640 ? 640 : ulMouseX ) - 30, ( ulMouseY > 400 ? 400 : ulMouseY ) - 30 );
        }
        if ( !( sGlobalData.ulFrames & 3 ) )
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
        LIB_Sprites_Draw( ResourceHandling_GetGroupStartResource( eGroups_Misc ) + 42, nMarkerGfx, ( ulMouseX >= 640 ? 640 : ulMouseX ) - 30, ( ulMouseY > 400 ? 400 : ulMouseY ) - 30 );
        if ( !( sGlobalData.ulFrames & 3 ) )
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
void SceneGame_CreateBackScreens( void )
{
    // Generate the map
    SceneGame_CreateMap();

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
void SceneGame_DrawMapModeWater( void )
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
    if ( !( sGlobalData.ulFrames & 3 ) )
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
void SceneGame_CreateMap( void )
{
    uint32_t ulIndex = 0;

    // generate maps
    ulIndex    = 0;
    float fRef = 0.00075f + ( 0.003f / rand() );

    LIB_PerlinNoise_Init( 123456 );
    LIB_PerlinNoise_GenerateMap( sGlobalData.pMapHeight, BACKSCREENWIDTH, 0, fRef );
}

//-----------------------------------------------------------------------------
// End of file Scene_Game.c
//-----------------------------------------------------------------------------
