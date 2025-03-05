/** ---------------------------------------------------------------------------
    @file		LIB_ApolloInput.h
    @defgroup 	AmiWorms Apollo V4 source
    @brief		Apollo V4 development - Shell
    @date		2025-01-31
 -----------------------------------------------------------------------------
    Notes

--------------------------------------------------------------------------- */

#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "stdint.h"
#include "stdbool.h"
#include "../Includes/defines.h"

#define NO_SCENE_SET ( -1 )

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------

typedef struct
{
    bool GamePaused;
    bool GameEnded;
    bool HelpActivated;

    // map related ...
    int32_t pMapHeight[ BACKSCREENWIDTH ];
    int32_t nScrollX;
    int32_t nScrollY;
    int32_t nMouseX;
    int32_t nMouseY;
    bool    bMapMode;

    // system related ...
    uint32_t ulFrames;
    uint32_t nNewScene;

} sGLOBALDATA, *psGLOBALDATA;

//-----------------------------------------------------------------------------
// External Data
//-----------------------------------------------------------------------------

extern sGLOBALDATA sGlobalData;

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

void GlobalData_Init( void );

//-----------------------------------------------------------------------------
// End of file LIB_ApolloInput.h
//-----------------------------------------------------------------------------
