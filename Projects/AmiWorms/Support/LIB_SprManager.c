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
#include "../Includes/defines.h"
#include "../Includes/LIB_Sprites.h"
#include "../Includes/LIB_SprManager.h"

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

            if ( pSprite->SprFlags.Active == ON )
            {
                // Allocate memory for the animation data
                // Set the animation data
                pSprite->AnimData.AnimID       = nAnimID;
                pSprite->AnimData.AnimType     = nAnimType;
                pSprite->AnimData.AnimFrames   = nAnimFrames;
                pSprite->AnimData.AnimCurFrame = 0;
                pSprite->SprFlags.Animated     = ON;

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

/** ---------------------------------------------------------------------------
    @brief		Remove a sprite from the manager
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
                if ( pSprite->SprFlags.Animated == ON )
                {
                    // Update the animation
                    if ( pSprite->AnimData.AnimType == SPR_ANIM_LOOP )
                    {
                        pSprite->AnimData.AnimCurFrame++;
                        if ( pSprite->AnimData.AnimCurFrame >= pSprite->AnimData.AnimFrames )
                        {
                            pSprite->AnimData.AnimCurFrame = 0;
                        }

                        // Set the frame
                        pSprite->SprNum = pSprite->AnimData.AnimCurFrame;
                    }
                }

                if ( pSprite->fnControl != NULL )
                {
                    pSprite->fnControl( &sSprMgr.Sprites[ i ] );
                }

                cnt++;
            }
            pSprite++;
        }
    }
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
                    int32_t nX     = pSprite->ScreenX;
                    int32_t nY     = pSprite->ScreenY;
                    int32_t nHalfW = pSprite->SprWidth / 2;
                    int32_t nHalfH = pSprite->SprHeight / 2;

                    if ( bMapMode == false )
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
                        nX /= 3;
                        nX += 20;
                        nY /= 3;
                        nY += 76;
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

//-----------------------------------------------------------------------------
// End of file: LIB_SprManager.c
//-----------------------------------------------------------------------------
