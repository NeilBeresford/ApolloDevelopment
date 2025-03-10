/** ---------------------------------------------------------------------------
    @file		Markers_ActiveWorm.c
    @defgroup 	AmiWorms Apollo V4 Shell
    @brief		One active marker, will be reassigned if needed...
    @date		2025-03-08
    @copyright	Neil Beresford 2025
 -----------------------------------------------------------------------------
    Notes

--------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "../../Includes/FlagStruct.h"
#include "../../Includes/Markers_ActiveWorm.h"
#include "../../Includes/GlobalData.h"
#include "../../Includes/ResourceFiles.h"
#include "../../Includes/LIB_Sprites.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define MARKER_DIRCOUNT ( 450 )

//-----------------------------------------------------------------------------
// Typedefs & Enumerators
//-----------------------------------------------------------------------------

typedef struct
{
    FlagStruct_t Flags;
    int32_t      nMarkerGfx;
    int32_t      nMarkerLife;
    PSPRHANDLE   pSprHandle;

} sAMCtrl, *psAMCtrl;

sAMCtrl sAM;

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------

void Markers_ActiveWorm_Ctrl( void* pSD );

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

/** ---------------------------------------------------------------------------
    @brief 		Initialize the active worm marker
    @ingroup 	AmiWorms
    @param      worm    Pointer to the sprite structure (PSPRITE)
 --------------------------------------------------------------------------- */
void Markers_ActiveWorm_Init( PSPRITE worm )
{
    // sort out the marker, if it is currently running... stop it!
    if ( sAM.Flags.InUse == true )
    {
        LIB_SprManager_Remove( sAM.pSprHandle );
    }

    // set the marker to be in use
    pSprDimention_t pSD  = ResourceHandling_GetSpriteFrameDimention( worm->SprResourceID, worm->SprNum );
    int32_t         xPos = worm->fWorldX;
    int32_t         yPos = worm->fWorldY;

    sAM.Flags.InUse      = true;
    sAM.nMarkerGfx       = ResourceHandling_GetGroupStartResource( eGroups_Misc ) + 4;
    sAM.nMarkerLife      = MARKER_DIRCOUNT;

    yPos -= ( pSD->nHeight / 2 ) + 30;

    sAM.pSprHandle = LIB_SprManager_Add( sAM.nMarkerGfx, xPos, yPos, 0, 0, Markers_ActiveWorm_Ctrl );
    LIB_SprManager_AddAnim( sAM.pSprHandle, 1000, SPR_ANIM_LOOP, LIB_SprManager_GetTotalFrames( sAM.pSprHandle ) - 1, NULL );
    LIB_SprManager_SetFlags( sAM.pSprHandle, SPR_FLAGS_WORLDSPRITE | SPR_FLAGS_VISIBLE | SPR_FLAGS_ACTIVE );
}

//-----------------------------------------------------------------------------
// Internal Functionality
//-----------------------------------------------------------------------------

void Markers_ActiveWorm_Ctrl( void* pSD )
{
    PSPRITE pSpr = (PSPRITE)pSD;

    if ( pSpr->SprFlags.Active == true )
    {
        if ( --sAM.nMarkerLife == 0 )
        {
            LIB_SprManager_Remove( sAM.pSprHandle->SprIndex );
            sAM.Flags.InUse = false;
        }
    }
}

//-----------------------------------------------------------------------------
// End of file: Markers_ActiveWorm.c
//-----------------------------------------------------------------------------
