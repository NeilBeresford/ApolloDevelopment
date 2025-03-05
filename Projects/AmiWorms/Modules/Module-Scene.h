/** ---------------------------------------------------------------------------
    @file		Module-Scene.h
    @defgroup 	AmiWorms Apollo V4 source
    @brief		Apollo V4 development - Shell
    @date		2025-01-31
 -----------------------------------------------------------------------------
    Notes

--------------------------------------------------------------------------- */

#ifndef _MODULE_SCENE_H_
#define _MODULE_SCENE_H_

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

typedef void ( *FN_ModuleScene )( uint32_t data );

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

void ModuleScene_Init( void );
void ModuleScene_Close( void );
void ModuleScene_Draw( void );
void ModuleScene_Update( void );
void ModuleScene_RegisterScene( int32_t SceneID, FN_ModuleScene fnInit, FN_ModuleScene fnClose, FN_ModuleScene fnDraw, FN_ModuleScene fnUpdate );
void ModuleScene_SetActiveScene( int32_t SceneID );

//-----------------------------------------------------------------------------

#endif // _MODULE_SCENE_H_

//-----------------------------------------------------------------------------
// End of file Module-Scene.h
//-----------------------------------------------------------------------------
