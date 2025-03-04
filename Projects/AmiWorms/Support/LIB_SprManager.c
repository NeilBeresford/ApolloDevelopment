/** ---------------------------------------------------------------------------
    @file		LIB_SprManager.c
    @defgroup 	AmiWorms Apollo V4 Shell
    @brief		Apollo V4 development - Shell
    @copyright	Neil Beresford 2024
 -----------------------------------------------------------------------------
    Notes

--------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "../Includes/FlagStruct.h"
#include "../Includes/GlobalData.h"
#include "../Includes/defines.h"
#include "../Includes/LIB_Sprites.h"
#include "../Includes/LIB_SprManager.h"
#include "../Modules/Module-Scene.h"

extern bool bMapMode; // Naughty and lazy Neil!  :-)

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define TOTAL_SPRITES ( 256 )
#define ON            ( 1 )
#define OFF           ( 0 )
#define YES           ( 1 )
#define NO            ( 0 )

//-----------------------------------------------------------------------------
// Typedefs and enums
//-----------------------------------------------------------------------------

typedef struct
{
    FlagStruct_t sFlags;
    uint16_t     SprCount;
    SPRITE       Sprites[ TOTAL_SPRITES ];
    SPRHANDLE    SprHandles[ TOTAL_SPRITES ];

} SPRITEMANAGER, *PSPRITEMANAGER;

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

SPRITEMANAGER sSprMgr = { .sFlags.Flags = 0 };

//-----------------------------------------------------------------------------
// Forward prototypesON
//-----------------------------------------------------------------------------

PSPRHANDLE LIB_SprManager_FindFreeHandle( void );

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

/** ---------------------------------------------------------------------------
    @brief		Initialise the sprite manager
    @ingroup	AmiWorms
    @param		None
    @return		None
--------------------------------------------------------------------------- */
void LIB_SprManager_Init( void )
{
    if ( sSprMgr.sFlags.Initialized == OFF )
    {
        PSPRITE    pSpr    = sSprMgr.Sprites;
        PSPRHANDLE pHandle = sSprMgr.SprHandles;

        // Clear the sprite manager
        sSprMgr.SprCount           = 0;
        sSprMgr.sFlags.Initialized = YES;

        // Clear all sprites and handles
        for ( uint16_t i = 0; i < TOTAL_SPRITES; i++, pSpr++, pHandle++ )
        {
            pSpr->SprFlags.Flags       = 0;
            pHandle->SprIndex          = 0;
            pHandle->Flags.Flags       = 0;
            pHandle->Flags.Initialized = YES;
        }
    }
}

/** ---------------------------------------------------------------------------
    @brief		Add Sprite to the manager
    @param		nResourceID - Resource ID of the sprite (from LIB_Sprites)
    @param		nX			- X position of the sprite
    @param		nY			- Y position of the sprite
    @param		nGroup		- Group ID of the sprite
    @param		nZ			- Z order of the sprite
    @param		fnControl	- Control function for the sprite (or NULL)
    @return		PSPRHANDLE	- Pointer to a controlled sprite details for
                              recall and control.
---------------------------------------------------------------------------- */
PSPRHANDLE LIB_SprManager_Add( uint32_t nResourceID, uint16_t nX, uint16_t nY, uint16_t nGroup, uint16_t nZ, fnSprControl fnControl )
{
    PSPRHANDLE rHandle = NULL;

    if ( sSprMgr.sFlags.Initialized == ON )
    {
        if ( sSprMgr.SprCount < TOTAL_SPRITES )
        {
            // Find a free sprite
            for ( uint16_t i = 0; i < TOTAL_SPRITES; i++ )
            {
                PSPRITE pSprite = &sSprMgr.Sprites[ i ];

                if ( pSprite->SprFlags.Active == OFF )
                {

                    // Set the sprite data
                    pSprite->SprID         = i;
                    pSprite->SprGroup      = nGroup;
                    pSprite->SprResourceID = nResourceID;
                    pSprite->ScreenX       = nX;
                    pSprite->ScreenY       = nY;
                    pSprite->fWorldX       = nX;
                    pSprite->fWorldY       = nY;
                    pSprite->SprZ          = nZ;
                    pSprite->SprWidth      = LIB_Sprites_GetWidth( nResourceID );
                    pSprite->SprHeight     = LIB_Sprites_GetHeight( nResourceID );
                    pSprite->fnControl     = fnControl;

                    // Set the sprite flags
                    pSprite->SprFlags.Active   = ON;
                    pSprite->SprFlags.OnScreen = ON;
                    pSprite->SprFlags.Visible  = ON;

                    // Increment the sprite count
                    sSprMgr.SprCount++;

                    // Set the result
                    rHandle                 = LIB_SprManager_FindFreeHandle();
                    rHandle->SprIndex       = i;
                    rHandle->Flags.Assigned = ON;
                    // Break out of the loop
                    break;
                }
            }
        }
    }

    return rHandle;
}

// function to store the animation frames for a sprite

/** ---------------------------------------------------------------------------
    @brief		Add an animation to a sprite
    @ingroup	AmiWorms
    @param		pSprHandle
    @param		AnimID
    @param		AnimType
    @param		AnimFrames
    @param		pFrameData
    @return		bool
---------------------------------------------------------------------------- */
bool LIB_SprManager_AddAnim( PSPRHANDLE pSprHandle, uint16_t nAnimID, uint16_t nAnimType, uint16_t nAnimFrames, uint16_t* pFrameData )
{
    bool bResult = false;

    if ( sSprMgr.sFlags.Initialized == ON )
    {
        if ( pSprHandle && pSprHandle->SprIndex < TOTAL_SPRITES )
        {
            PSPRITE pSprite = &sSprMgr.Sprites[ pSprHandle->SprIndex ];

            if ( pSprite->SprFlags.Active == ON && pSprite->SprFlags.Animated == OFF )
            {
                // Allocate memory for the animation data
                // Set the animation data
                pSprite->AnimData.AnimID          = nAnimID;
                pSprite->AnimData.AnimType        = nAnimType;
                pSprite->AnimData.AnimFrames      = nAnimFrames;
                pSprite->AnimData.AnimCurFrame    = 0;
                pSprite->AnimData.AnimDelayCnt    = 4;
                pSprite->AnimData.AnimCurDelayCnt = 0;
                pSprite->SprFlags.Animated        = ON;
                pSprite->AnimData.pFrames         = pFrameData;

                if ( pSprite->AnimData.pFrames != NULL )
                {
                    // Allocate memory for the frame data
                    pSprite->AnimData.pFrames = pFrameData;

                    // Set the result
                    bResult = true;
                }
            }
        }
    }

    return bResult;
}

/**-----------------------------------------------------------------------------
    @brief		Change the Sprite Anim, please note - this needs the SPRITE structure
    @ingroup	AmiWorms
    @param		pSprite        - Pointer to the sprite structure
    @param		nAnimID        - Animation ID
    @param		nAnimType      - Animation Type
    @param		nAnimFrames    - Animation Frames
    @param		pFrameData     - Pointer to the frame data
    @return		None
  -----------------------------------------------------------------------------*/
void LIB_SprManager_ChangeSpriteAnim( PSPRITE pSprite, uint16_t nAnimID, uint16_t nAnimType, uint16_t nAnimFrames, uint16_t* pFrameData )
{
    if ( pSprite->SprFlags.Active == ON )
    {
        // Allocate memory for the animation data
        // Set the animation data
        pSprite->AnimData.AnimID          = nAnimID;
        pSprite->AnimData.AnimType        = nAnimType;
        pSprite->AnimData.AnimFrames      = nAnimFrames;
        pSprite->AnimData.AnimCurFrame    = 0;
        pSprite->AnimData.AnimDelayCnt    = 4;
        pSprite->AnimData.AnimCurDelayCnt = 0;
        pSprite->SprFlags.Animated        = ON;
        pSprite->AnimData.pFrames         = pFrameData;

        if ( pSprite->AnimData.pFrames != NULL )
        {
            // Allocate memory for the frame data
            pSprite->AnimData.pFrames = pFrameData;
        }
    }
}

/** ---------------------------------------------------------------------------
    @brief		Update controller
    @ingroup	AmiWorms
    @param		uint16_t nSpriteID
    @return		None
 --------------------------------------------------------------------------- */
void LIB_SprManager_Update( void )
{
    if ( sSprMgr.sFlags.Initialized == ON )
    {

        PSPRITE pSprite = sSprMgr.Sprites;

        for ( uint16_t i = 0, cnt = 0; i < TOTAL_SPRITES && cnt != sSprMgr.SprCount; i++ )
        {
            if ( pSprite->SprFlags.Active == ON )
            {
                // movement
                pSprite->fWorldX += pSprite->fMoveX;
                pSprite->fWorldY += pSprite->fMoveY;

                // Deletion
                if ( pSprite->SprFlags.DeleteMe == YES )
                {
                    pSprite->SprFlags.Active = OFF;
                }
                else if ( pSprite->SprFlags.Animated == ON )
                {
                    // anim - needs improvement in code
                    if ( ++pSprite->AnimData.AnimCurDelayCnt >= pSprite->AnimData.AnimDelayCnt )
                    {
                        pSprite->AnimData.AnimCurDelayCnt = 0;

                        // Update the animation
                        switch ( pSprite->AnimData.AnimType )
                        {
                            case SPR_ANIM_LOOP:
                            {
                                pSprite->AnimData.AnimCurFrame++;
                                if ( pSprite->AnimData.AnimCurFrame >= pSprite->AnimData.AnimFrames )
                                {
                                    pSprite->AnimData.AnimCurFrame = 0;
                                }

                                // Set the frame
                                pSprite->SprNum = pSprite->AnimData.AnimCurFrame;
                                break;
                            }
                            // Update the animation
                            case SPR_ANIM_ONCE:
                            {
                                pSprite->AnimData.AnimCurFrame++;
                                if ( pSprite->AnimData.AnimCurFrame >= pSprite->AnimData.AnimFrames )
                                {
                                    pSprite->AnimData.AnimType     = SPR_ANIM_NONE;
                                    pSprite->AnimData.AnimCurFrame = 0;
                                    pSprite->SprResourceID         = pSprite->SprResetResourceID;
                                }

                                // Set the frame
                                pSprite->SprNum = pSprite->AnimData.AnimCurFrame;
                                break;
                            }
                            case SPR_ANIM_NONE:
                            default:
                            {
                                break;
                            }
                        }
                    }
                    if ( pSprite->fnControl != NULL )
                    {
                        pSprite->fnControl( &sSprMgr.Sprites[ i ] );
                    }
                }

                cnt++;
            }
            pSprite++;
        }
    }
}

/** ---------------------------------------------------------------------------
    @brief		Remove all sprites from the manager
    @ingroup	AmiWorms
    @param		uint16_t nSpriteID
    @return		None
 --------------------------------------------------------------------------- */
void LIB_SprManager_RemoveAll( void )
{
    if ( sSprMgr.sFlags.Initialized == ON )
    {
        for ( uint16_t i = 0; i < TOTAL_SPRITES; i++ )
        {
            sSprMgr.Sprites[ i ].SprFlags.Flags        = 0;
            sSprMgr.Sprites[ i ].fnControl             = NULL;
            sSprMgr.Sprites[ 1 ].SprNum                = 0;
            sSprMgr.Sprites[ i ].SprResetResourceID    = 0;
            sSprMgr.Sprites[ i ].SprResourceID         = 0;
            sSprMgr.Sprites[ i ].ScreenX               = 0;
            sSprMgr.Sprites[ i ].ScreenY               = 0;
            sSprMgr.Sprites[ i ].fWorldX               = 0;
            sSprMgr.Sprites[ i ].fWorldY               = 0;
            sSprMgr.Sprites[ i ].SprWidth              = 0;
            sSprMgr.Sprites[ i ].SprHeight             = 0;
            sSprMgr.Sprites[ i ].SprZ                  = 0;
            sSprMgr.Sprites[ i ].AnimData.AnimID       = 0;
            sSprMgr.Sprites[ i ].AnimData.AnimType     = SPR_ANIM_NONE;
            sSprMgr.Sprites[ i ].AnimData.AnimFrames   = 0;
            sSprMgr.Sprites[ i ].AnimData.AnimCurFrame = 0;
            sSprMgr.Sprites[ i ].AnimData.AnimDelayCnt = 0;
            sSprMgr.Sprites[ i ].AnimData.pFrames      = NULL;
            sSprMgr.Sprites[ i ].CurAnimIndex          = 0;
            sSprMgr.Sprites[ i ].fMoveSpeed            = 0;
            sSprMgr.Sprites[ i ].fMoveAngle            = 0;
            sSprMgr.Sprites[ i ].fMoveX                = 0;
            sSprMgr.Sprites[ i ].fMoveY                = 0;
            sSprMgr.SprHandles[ i ].SprIndex           = 0;
            sSprMgr.SprHandles[ i ].Flags.Flags        = 0;
            sSprMgr.SprHandles[ i ].Flags.Initialized  = YES;
        }
        sSprMgr.SprCount = 0;
    }
}

/** ---------------------------------------------------------------------------
    @brief		Remove a sprite from the manager
    @ingroup	AmiWorms
    @param		uint16_t nSpriteID
    @return		None
 --------------------------------------------------------------------------- */
void LIB_SprManager_Remove( uint16_t nSpriteID )
{
    if ( sSprMgr.sFlags.Initialized == ON )
    {
        if ( nSpriteID < TOTAL_SPRITES )
        {
            sSprMgr.Sprites[ nSpriteID ].SprFlags.Flags        = 0;
            sSprMgr.Sprites[ nSpriteID ].fnControl             = NULL;
            sSprMgr.Sprites[ nSpriteID ].SprNum                = 0;
            sSprMgr.Sprites[ nSpriteID ].SprResetResourceID    = 0;
            sSprMgr.Sprites[ nSpriteID ].SprResourceID         = 0;
            sSprMgr.Sprites[ nSpriteID ].ScreenX               = 0;
            sSprMgr.Sprites[ nSpriteID ].ScreenY               = 0;
            sSprMgr.Sprites[ nSpriteID ].fWorldX               = 0;
            sSprMgr.Sprites[ nSpriteID ].fWorldY               = 0;
            sSprMgr.Sprites[ nSpriteID ].SprWidth              = 0;
            sSprMgr.Sprites[ nSpriteID ].SprHeight             = 0;
            sSprMgr.Sprites[ nSpriteID ].SprZ                  = 0;
            sSprMgr.Sprites[ nSpriteID ].AnimData.AnimID       = 0;
            sSprMgr.Sprites[ nSpriteID ].AnimData.AnimType     = SPR_ANIM_NONE;
            sSprMgr.Sprites[ nSpriteID ].AnimData.AnimFrames   = 0;
            sSprMgr.Sprites[ nSpriteID ].AnimData.AnimCurFrame = 0;
            sSprMgr.Sprites[ nSpriteID ].AnimData.AnimDelayCnt = 0;
            sSprMgr.Sprites[ nSpriteID ].AnimData.pFrames      = NULL;
            sSprMgr.Sprites[ nSpriteID ].CurAnimIndex          = 0;
            sSprMgr.Sprites[ nSpriteID ].fMoveSpeed            = 0;
            sSprMgr.Sprites[ nSpriteID ].fMoveAngle            = 0;
            sSprMgr.Sprites[ nSpriteID ].fMoveX                = 0;
            sSprMgr.Sprites[ nSpriteID ].fMoveY                = 0;
            sSprMgr.SprHandles[ nSpriteID ].Flags.Flags        = 0;
            sSprMgr.SprHandles[ nSpriteID ].SprIndex           = 0;
            sSprMgr.SprHandles[ nSpriteID ].Flags.Initialized  = YES;
            sSprMgr.SprCount--;
        }
    }
}

/** ---------------------------------------------------------------------------
    @brief		Returns the PSPRITE for the PSPRHANDLE
                PLEASE be careful with this function, it is wise to hack the
                sprite data
    @ingroup	AmiWorms
    @param		pH          - Sprite handle to get the sprite for
    @return		PSPRITE     - linked to haandle, or NULL
 --------------------------------------------------------------------------- */
PSPRITE LIB_SprManager_GetSprite( PSPRHANDLE pH )
{
    PSPRITE pResult = NULL;

    if ( sSprMgr.sFlags.Initialized == ON )
    {
        if ( pH && pH->SprIndex < TOTAL_SPRITES )
        {
            pResult = &sSprMgr.Sprites[ pH->SprIndex ];
        }
    }

    return pResult;
}

/** ---------------------------------------------------------------------------
    @brief		Draw all sprites
    @ingroup	AmiWorms
    @param		None
    @return		None
 --------------------------------------------------------------------------- */
void LIB_SprManager_Draw( int32_t nXScroll, int32_t nYScroll )
{
    if ( sSprMgr.sFlags.Initialized == ON )
    {
        for ( uint16_t i = 0, cnt = 0; i < TOTAL_SPRITES && cnt != sSprMgr.SprCount; i++ )
        {
            PSPRITE pSprite = &sSprMgr.Sprites[ i ];

            if ( pSprite->SprFlags.Active == ON )
            {

                if ( pSprite->SprFlags.Visible == ON )
                {
                    pSprDimention_t pSprDim = NULL;
                    int32_t         nX      = pSprite->ScreenX;
                    int32_t         nY      = pSprite->ScreenY;
                    int32_t         nHalfW  = 0;
                    int32_t         nHalfH  = 0;

                    pSprDim                 = ResourceHandling_GetSpriteFrameDimention( pSprite->SprResourceID, pSprite->SprNum );
                    nHalfW                  = ( LIB_Sprites_GetWidth( pSprite->SprResourceID ) / 2 );
                    nHalfH                  = ( LIB_Sprites_GetHeight( pSprite->SprResourceID ) / 2 );

                    if ( sGlobalData.bMapMode == false )
                    {
                        if ( pSprite->SprFlags.WorldSprite == YES )
                        {
                            nX = pSprite->fWorldX - nXScroll;
                            nY = pSprite->fWorldY - nYScroll;
                        }
                        if ( pSprite->SprFlags.Flipped == ON )
                            LIB_Sprites_DrawFlipped( pSprite->SprResourceID, pSprite->SprNum, nX - nHalfW, nY - nHalfH );
                        else
                            LIB_Sprites_Draw( pSprite->SprResourceID, pSprite->SprNum, nX - nHalfW, nY - nHalfH );
                    }
                    else
                    {
                        nHalfW = ( LIB_Sprites_GetWidth( pSprite->SprResourceID ) / 2 ) / 3;
                        nHalfH = ( LIB_Sprites_GetHeight( pSprite->SprResourceID ) / 2 ) / 3;
                        nX /= 3;
                        nX -= nHalfW;
                        nX += 10;
                        nY /= 3;
                        nY -= nHalfH;
                        nY += 66;
                        if ( pSprite->SprFlags.Flipped == ON )
                            LIB_Sprites_DrawMapFlipped( pSprite->SprResourceID, pSprite->SprNum, nX - nHalfW, nY - nHalfH );
                        else
                            LIB_Sprites_DrawMap( pSprite->SprResourceID, pSprite->SprNum, nX - nHalfW, nY - nHalfH );
                    }
                }
                cnt++;
            }
        }
    }
}

/** ---------------------------------------------------------------------------
    @brief		Sets a new sprite position
    @ingroup	AmiWorms
    @param		pSprHandle 	- Pointer to the sprite handle
    @param		nX			- X position
    @param		nY			- Y position
    @return		None
 --------------------------------------------------------------------------- */
void LIB_SprManager_SetPosition( PSPRHANDLE pSprHandle, int32_t nX, int32_t nY )
{
    if ( sSprMgr.sFlags.Initialized == ON )
    {
        if ( pSprHandle && pSprHandle->SprIndex < TOTAL_SPRITES )
        {
            sSprMgr.Sprites[ pSprHandle->SprIndex ].ScreenX = nX;
            sSprMgr.Sprites[ pSprHandle->SprIndex ].ScreenY = nY;
            sSprMgr.Sprites[ pSprHandle->SprIndex ].fWorldX = nX;
            sSprMgr.Sprites[ pSprHandle->SprIndex ].fWorldY = nY;
        }
    }
}

//-----------------------------------------------------------------------------
// Internal Functionality
//-----------------------------------------------------------------------------

/** ---------------------------------------------------------------------------
    @brief		Find a free sprite handle
    @ingroup	AmiWorms
    @param		None
    @return		PSPRHANDLE of
 --------------------------------------------------------------------------- */
PSPRHANDLE LIB_SprManager_GetHandle( uint16_t nSpriteID )
{
    PSPRHANDLE pResult = NULL;

    if ( sSprMgr.sFlags.Initialized == ON )
    {
        if ( nSpriteID < TOTAL_SPRITES )
        {
            if ( ( sSprMgr.Sprites[ nSpriteID ].SprFlags.Active == ON ) && ( sSprMgr.SprHandles[ nSpriteID ].Flags.Assigned == NO ) )
            {
                sSprMgr.SprHandles[ nSpriteID ].Flags.Assigned = YES;
                sSprMgr.SprHandles[ nSpriteID ].Flags.Locked   = NO;
                sSprMgr.SprHandles[ nSpriteID ].SprIndex       = nSpriteID;
                pResult                                        = &sSprMgr.SprHandles[ nSpriteID ];
            }
        }
    }

    return pResult;
}

/** ---------------------------------------------------------------------------
    @brief		Find a free sprite handle
    @ingroup	AmiWorms
    @param		None
    @return		PSPRHANDLE - Returns a pointer to a free sprite handle or
                NULL if none are available
 --------------------------------------------------------------------------- */
PSPRHANDLE LIB_SprManager_FindFreeHandle( void )
{
    PSPRHANDLE pResult = NULL;

    if ( sSprMgr.sFlags.Initialized == ON )
    {
        PSPRHANDLE pHandle = sSprMgr.SprHandles;

        for ( uint16_t i = 0; i < TOTAL_SPRITES; i++, pHandle++ )
        {
            if ( pHandle->Flags.Assigned == NO )
            {
                pHandle->Flags.Assigned = YES;
                pResult                 = pHandle;

                break;
            }
        }
    }

    return pResult;
}

// compress the sprite manager to remove any gaps in the sprite array
/** ---------------------------------------------------------------------------
    @brief		Compress the sprite handles
    @ingroup	AmiWorms
    @param		None
    @return		None
 --------------------------------------------------------------------------- */
void LIB_SprManager_CompressHandles( void )
{
    if ( sSprMgr.sFlags.Initialized == ON )
    {
        uint16_t nCount = 0;

        for ( uint16_t i = 0; i < TOTAL_SPRITES; i++ )
        {
            if ( sSprMgr.SprHandles[ i ].Flags.Assigned == YES )
            {
                if ( i != nCount )
                {
                    sSprMgr.SprHandles[ nCount ]           = sSprMgr.SprHandles[ i ];
                    sSprMgr.SprHandles[ i ].Flags.Assigned = NO;
                }
                nCount++;
            }
        }
    }
}

/** ---------------------------------------------------------------------------
    @brief		Returns the total number of frames in a sprite
    @ingroup	AmiWorms
    @param		pSprHandle 	- Pointer to the sprite handle
    @return		uint32_t 	- Number of frames
 --------------------------------------------------------------------------- */
uint32_t LIB_SprManager_GetTotalFrames( PSPRHANDLE pSprHandle )
{
    uint32_t nFrames = 0;

    if ( sSprMgr.sFlags.Initialized == ON )
    {
        if ( pSprHandle && pSprHandle->SprIndex < TOTAL_SPRITES )
        {
            nFrames = LIB_Sprites_GetFrames( sSprMgr.Sprites[ pSprHandle->SprIndex ].SprResourceID );
        }
    }

    return nFrames;
}

/** ---------------------------------------------------------------------------
    @brief		Flags the sprite as flipped
    @ingroup	AmiWorms
    @param		pSprHandle 	- Pointer to the sprite handle
    @param		Flipped 	- Flipped flag
    @return		void
 --------------------------------------------------------------------------- */
void LIB_SprManager_FlipSprite( PSPRHANDLE pSprHandle, bool Flipped )
{
    if ( sSprMgr.sFlags.Initialized == ON )
    {
        if ( pSprHandle && pSprHandle->SprIndex < TOTAL_SPRITES )
        {
            sSprMgr.Sprites[ pSprHandle->SprIndex ].SprFlags.Flipped = Flipped;
        }
    }
}

/** ---------------------------------------------------------------------------
    @brief		Sets the sprite flags
    @ingroup	AmiWorms
    @param		pSprHandle 	- Pointer to the sprite handle
    @param		ulFlags 	- Flags to set
    @return		void
 --------------------------------------------------------------------------- */
void LIB_SprManager_SetFlags( PSPRHANDLE pSprHandle, uint32_t ulFlags )
{
    if ( sSprMgr.sFlags.Initialized == ON )
    {
        if ( pSprHandle && pSprHandle->SprIndex < TOTAL_SPRITES )
        {
            sSprMgr.Sprites[ pSprHandle->SprIndex ].SprFlags.Flags |= ulFlags;
        }
    }
}

/** ---------------------------------------------------------------------------
    @brief		Clears the sprite flags
    @ingroup	AmiWorms
    @param		pSprHandle 	- Pointer to the sprite handle
    @param		ulFlags 	- Flags to clear
    @return		void
 --------------------------------------------------------------------------- */
void LIB_SprManager_ClearFlags( PSPRHANDLE pSprHandle, uint32_t ulFlags )
{
    if ( sSprMgr.sFlags.Initialized == ON )
    {
        if ( pSprHandle && pSprHandle->SprIndex < TOTAL_SPRITES )
        {
            sSprMgr.Sprites[ pSprHandle->SprIndex ].SprFlags.Flags &= ~ulFlags;
        }
    }
}

/** ---------------------------------------------------------------------------
    @brief		Sets a sprite variable
    @ingroup	AmiWorms
    @param		pSprHandle 	- Pointer to the sprite handle
    @param		Var 		- Variable to set
    @param		VarValue 	- Value to set
    @return		void
 --------------------------------------------------------------------------- */
void LIB_SprManager_SetVariable( PSPRHANDLE pSprHandle, uint32_t Var, uint32_t VarValue )
{
    if ( sSprMgr.sFlags.Initialized == ON )
    {
        if ( pSprHandle && pSprHandle->SprIndex < TOTAL_SPRITES )
        {
            switch ( Var )
            {
                case eSPRVAR_DEFAULTRESOURCE:
                {
                    sSprMgr.Sprites[ pSprHandle->SprIndex ].SprResetResourceID = VarValue;
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------
// End of file: LIB_SprManager.c
//-----------------------------------------------------------------------------
