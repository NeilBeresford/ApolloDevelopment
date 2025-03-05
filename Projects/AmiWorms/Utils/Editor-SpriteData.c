/** ---------------------------------------------------------------------------
    @file		Editor_SpriteData.c
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
#include "../Includes/GlobalData.h"
#include "../Includes/LIB_Sprites.h"
#include "../Includes/LIB_SpriteFont.h"
#include "../Includes/LIB_SprManager.h"
#include "../Includes/ResourceFiles.h"
#include "../Includes/ResourceHandling.h"
#include "../Includes/defines.h"
#include "Editor-SpriteData.h"

//-----------------------------------------------------------------------------
// Typedefs and Enums
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

/** ---------------------------------------------------------------------------
    @brief 		Initialize the sprite data editor
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void Editor_SpriteData_Init( void )
{
    // setup screen...
    Hardware_ClearScreen();
}

/** ---------------------------------------------------------------------------
    @brief 		Close the sprite data editor
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void Editor_SpriteData_Close( void )
{
}

/** ---------------------------------------------------------------------------
    @brief 		Draw the sprite data editor
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void Editor_SpriteData_Draw( void )
{
    Hardware_ClearScreen();

    uint8_t edTitle[] = "Sprite Data Editor";

    LIB_SpriteFont_Draw( eFont_CyanBig, 320 - ( LIB_SpriteFont_GetStringLength( eFont_CyanBig, edTitle ) ) / 2, 10, edTitle );
}

/** ---------------------------------------------------------------------------
    @brief 		Update the sprite data editor
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void Editor_SpriteData_Update( void )
{
}

//-----------------------------------------------------------------------------
// End of file Editor_SpriteData.c
//-----------------------------------------------------------------------------
