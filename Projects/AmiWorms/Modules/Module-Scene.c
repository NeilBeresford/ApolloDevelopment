/** ---------------------------------------------------------------------------
    @file		Module-Scene.c
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
#include "../Includes/defines.h"
#include "../Includes/FlagStruct.h"
#include "../Includes/GlobalData.h"
#include "../Includes/LIB_Sprites.h"
#include "../Includes/LIB_SpriteFont.h"
#include "../Includes/LIB_SprManager.h"
#include "../Includes/ResourceFiles.h"
#include "../Includes/ResourceHandling.h"
#include "../Includes/defines.h"
#include "Module-Scene.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define TOTAL_SCENES ( 20 )
#define NO_SCENE_SET ( -1 )

//-----------------------------------------------------------------------------
// Typedefs and Enums
//-----------------------------------------------------------------------------

typedef struct
{
    uint32_t       ID;
    FlagStruct_t   Flags;
    FN_ModuleScene Init;
    FN_ModuleScene Close;
    FN_ModuleScene Draw;
    FN_ModuleScene Update;

} MODULESCENE, *PMODULESCENE;

typedef struct
{
    FlagStruct_t Flags;
    int32_t      CurActiveScene;
    MODULESCENE  Scenes[ TOTAL_SCENES ];

} SCENECTRL, *PSCENECTRL;

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

SCENECTRL SceneCtrl;

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

/** ---------------------------------------------------------------------------
    @brief 		Initialize the sprite data editor
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void ModuleScene_Init( void )
{
    if ( SceneCtrl.Flags.Initialized == NO )
    {
        // setup screen...
        memset( &SceneCtrl, 0, sizeof( SCENECTRL ) );
        SceneCtrl.CurActiveScene    = NO_SCENE_SET;
        SceneCtrl.Flags.Initialized = YES;
        for ( int32_t nIndex = 0; nIndex < TOTAL_SCENES; nIndex++ )
        {
            SceneCtrl.Scenes[ nIndex ].ID                = nIndex + 1;
            SceneCtrl.Scenes[ nIndex ].Flags.Initialized = YES;
        }
    }
}

/** ---------------------------------------------------------------------------
    @brief 		Close the sprite data editor
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void ModuleScene_Close( void )
{
    if ( SceneCtrl.Flags.Initialized == YES )
    {
        for ( int32_t nIndex = 0; nIndex < TOTAL_SCENES; nIndex++ )
        {
            SceneCtrl.Scenes[ nIndex ].Flags.Initialized = NO;
        }
        SceneCtrl.Flags.Initialized = NO;
    }
}

/** ---------------------------------------------------------------------------
    @brief 		Draw the sprite data editor
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void ModuleScene_Draw( void )
{
    if ( SceneCtrl.Flags.Initialized == YES )
    {
        if ( SceneCtrl.CurActiveScene != NO_SCENE_SET )
        {
            if ( SceneCtrl.Scenes[ SceneCtrl.CurActiveScene ].Draw != NULL )
            {
                SceneCtrl.Scenes[ SceneCtrl.CurActiveScene ].Draw( 0 );
            }
        }
    }
}

/** ---------------------------------------------------------------------------
    @brief 		Update the sprite data editor
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void ModuleScene_Update( void )
{
    if ( SceneCtrl.Flags.Initialized == YES )
    {
        if ( SceneCtrl.CurActiveScene != NO_SCENE_SET )
        {
            if ( SceneCtrl.Scenes[ SceneCtrl.CurActiveScene ].Update != NULL )
            {
                SceneCtrl.Scenes[ SceneCtrl.CurActiveScene ].Update( 0 );
            }
        }
    }
}

/** ---------------------------------------------------------------------------
    @brief 		Switch to another scene
    @ingroup 	AmiWorms
    @param      SceneID     The ID of the scene to set
---------------------------------------------------------------------------- */
void ModuleScene_SetActiveScene( int32_t SceneID )
{
    if ( SceneCtrl.Flags.Initialized == YES )
    {
        if ( SceneID >= 0 && SceneID < TOTAL_SCENES )
        {
            if ( SceneCtrl.CurActiveScene != NO_SCENE_SET )
            {
                if ( SceneCtrl.Scenes[ SceneCtrl.CurActiveScene ].Close != NULL )
                {
                    SceneCtrl.Scenes[ SceneCtrl.CurActiveScene ].Close( 0 );
                }
            }
            if ( SceneCtrl.Scenes[ SceneID ].Init != NULL )
            {
                SceneCtrl.Scenes[ SceneID ].Init( 0 );
            }
            SceneCtrl.CurActiveScene = SceneID;
        }
    }
}

/** ---------------------------------------------------------------------------
    @brief 		Register a scene
    @ingroup 	AmiWorms
    @param      SceneID     The ID of the scene to register
    @param      Init        The function to call when the scene is initialized
    @param      Close       The function to call when the scene is closed
    @param      Draw        The function to call when the scene is drawn
    @param      Update      The function to call when the scene is updated
 --------------------------------------------------------------------------- */
void ModuleScene_RegisterScene( int32_t SceneID, FN_ModuleScene fnInit, FN_ModuleScene fnClose, FN_ModuleScene fnDraw, FN_ModuleScene fnUpdate )
{
    if ( SceneCtrl.Flags.Initialized == YES )
    {
        if ( SceneID >= 0 && SceneID < TOTAL_SCENES )
        {
            SceneCtrl.Scenes[ SceneID ].Init        = fnInit;
            SceneCtrl.Scenes[ SceneID ].Close       = fnClose;
            SceneCtrl.Scenes[ SceneID ].Draw        = fnDraw;
            SceneCtrl.Scenes[ SceneID ].Update      = fnUpdate;
            SceneCtrl.Scenes[ SceneID ].Flags.InUse = YES;
        }
    }
}

//-----------------------------------------------------------------------------
// End of file Module-Scene.c
//-----------------------------------------------------------------------------
