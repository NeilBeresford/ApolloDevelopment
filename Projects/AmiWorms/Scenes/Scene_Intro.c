/** ---------------------------------------------------------------------------
    @file		Scene_Intro.c
    @defgroup 	AmiWorms Apollo V4 Shell
    @brief		Startup screen for Amiworms
    @date		2025-10-01
    @version	0.1
    @copyright	Neil Beresford 2025
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
#include "../Includes/FlagStruct.h"
#include "../Includes/GlobalData.h"
#include "../Includes/Hardware.h"
#include "../Includes/HWScreen.h"
#include "../Includes/FontModule.h"
#include "../Includes/LIB_ApolloInput.h"
#include "../Includes/LIB_Files.h"
#include "../Includes/LIB_Sprites.h"
#include "../Includes/LIB_SprManager.h"
#include "../Modules/Module-Scene.h"
#include "../Includes/ResourceFiles.h"
#include "../Includes/ResourceHandling.h"

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------

#define CHECK_KEY( key )          ( sICtrl.sKeyboardState.Keys[ key ] && ( sICtrl.sKeyboardState.KeysProcessed[ key ] == 0 ) )
#define CHECK_KEYDOWN( key )      ( sICtrl.sKeyboardState.Keys[ key ] && ( sICtrl.sKeyboardState.KeysDown[ key ] == 1 ) )
#define CHECK_KEY_RELEASED( key ) ( sICtrl.sKeyboardState.KeysProcessed[ key ] && ( sICtrl.sKeyboardState.KeysDown[ key ] == 0 ) )

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------

typedef struct
{
    FlagStruct_t        Flags;
    PSPRHANDLE          pSprHTitle;
    PSPRHANDLE          pSprHSpanner;
    ApolloKeyBoardState sKeyboardState;
    ApolloJoypadState   sJoypadState;
    ApolloMouseState    sMouseState;

} sIntroCtrl, *psIntroCtrl;

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

sIntroCtrl sICtrl = { .Flags.Flags = 0, .pSprHTitle = NULL, .pSprHSpanner = NULL };

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

/** ---------------------------------------------------------------------------
    @brief 		Initialize the intro scene
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void SceneIntro_Init( void )
{
    if ( sICtrl.Flags.Initialized == NO )
    {
        // iniitialise the global data and make sure map mode is off
        sGlobalData.bMapMode = false;
        LIB_SprManager_RemoveAll();

        // Add the title sprite ...
        sICtrl.pSprHTitle = LIB_SprManager_Add( ResourceHandling_GetGroupStartResource( eGroups_Menu ), 320, 80, 0, 0, NULL );
        LIB_SprManager_AddAnim( sICtrl.pSprHTitle, 2, SPR_ANIM_LOOP, 1, NULL );
        LIB_SprManager_SetFlags( sICtrl.pSprHTitle, SPR_FLAGS_WORLDSPRITE | SPR_FLAGS_VISIBLE );

        // Add anim for the spanner ...
        sICtrl.pSprHSpanner = LIB_SprManager_Add( ResourceHandling_GetGroupStartResource( eGroups_Menu ) + 1, 320, 300, 0, 0, NULL );
        LIB_SprManager_AddAnim( sICtrl.pSprHSpanner, 1, SPR_ANIM_LOOP, LIB_SprManager_GetTotalFrames( sICtrl.pSprHSpanner ), NULL );
        LIB_SprManager_SetFlags( sICtrl.pSprHSpanner, SPR_FLAGS_WORLDSPRITE | SPR_FLAGS_VISIBLE );

        // clear the controllers and set clip aand screen
        LIB_Sprites_SetClipArea( 0, 0, 640, 480 );
        sICtrl.sKeyboardState.Previous_Key = NOKEY;
        sICtrl.sKeyboardState.Current_Key  = NOKEY;
        ApolloKeyboardClear( &sICtrl.sKeyboardState );
        Hardware_SetScreenmode( 0 );

        // All completed
        sICtrl.Flags.Initialized = YES;
    }
}

/** ---------------------------------------------------------------------------
    @brief 		Close the intro scene
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void SceneIntro_Close( void )
{
    LIB_SprManager_RemoveAll();
    LIB_SprManager_Update();
    sICtrl.Flags.Initialized = NO;
}

/** ---------------------------------------------------------------------------
    @brief 		Draw the intro scene
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void SceneIntro_Draw( void )
{
    Hardware_ClearAllScreen();

    LIB_SpriteFont_Draw( eFont_WhiteBig, 320 - ( LIB_SpriteFont_GetStringLength( eFont_WhiteBig, "AMIWORMS - An Apollo Game" ) / 2 ), 180, "AMIWORMS - An Apollo Game" );
    LIB_SpriteFont_Draw( eFont_WhiteBig, 320 - ( LIB_SpriteFont_GetStringLength( eFont_WhiteBig, "Coded by Neil Beresford" ) / 2 ), 205, "Coded by Neil Beresford" );

    LIB_SpriteFont_Draw( eFont_YellowBig, 320 - ( LIB_SpriteFont_GetStringLength( eFont_YellowBig, "Press 'RETURN' to start game" ) / 2 ), 320, "Press 'RETURN' to start game" );
    LIB_SpriteFont_Draw( eFont_BlueBig, 320 - ( LIB_SpriteFont_GetStringLength( eFont_BlueBig, "Press 'ESCAPE' to quit game" ) / 2 ), 450, "Press 'ESCAPE' to quit game" );

    LIB_SprManager_Draw( 0, 0 );
}

/** ---------------------------------------------------------------------------
    @brief 		Update the intro scene
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void SceneIntro_Update( void )
{
    ApolloKeyboard( &sICtrl.sKeyboardState );
    ApolloJoypad( &sICtrl.sJoypadState );
    ApolloMouse( &sICtrl.sMouseState );

    if ( CHECK_KEY( KEYCODE_ESC ) || sICtrl.sJoypadState.Joypad_X )
    {
        sGlobalData.GameEnded = YES;
    }
    if ( CHECK_KEY( KEYCODE_RETURN ) || sICtrl.sJoypadState.Joypad_A )
    {
        ModuleScene_SetActiveScene( 0 );
    }
}

//-----------------------------------------------------------------------------
// End of file: Scene_Intro.c
//-----------------------------------------------------------------------------
