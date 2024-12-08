/** ---------------------------------------------------------------------------
	@file		LIB_Sprites.c
	@defgroup 	MainShell Apollo V4 Shell
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
#include "Includes/FlagStruct.h"
#include "Includes/HWScreen.h"
#include "Includes/ResourceHandling.h"
#include "Includes/LIB_Sprites.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define MAX_SPRITE_BANKS 10

//-----------------------------------------------------------------------------
// Typedefs and enums
//-----------------------------------------------------------------------------

/**-----------------------------------------------------------------------------
    @brief      Sprite Control structure
    @ingroup 	MainShell
 ---------------------------------------------------------------------------- */
typedef struct 
{
    FlagStruct_t    Flags;                              //!< Flags
    SpriteBank_t    SpriteBanks[ MAX_SPRITE_BANKS ];    //!< Sprite banks 

} SpriteCtrl, *pSpriteCtrl;                             //!< Sprite Control structure

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

SpriteCtrl     SprCtrl = { .Flags = { .Flags = 0 } };   //!< Sprite Control structure

//-----------------------------------------------------------------------------
// Code
//-----------------------------------------------------------------------------

/** ----------------------------------------------------------------------------
    @brief 		Initialize the sprite library
    @ingroup 	MainShell
    @return 	bool            - true if successful
 -----------------------------------------------------------------------------*/
void LIB_Sprites_Init( void )
{
    printf("LIB_Sprites_Init\n");

    // setup the sprite banks
    for ( uint32_t i = 0; i < MAX_SPRITE_BANKS; i++ )
    {
        SprCtrl.SpriteBanks[ i ].ulSpriteID     = i;
        SprCtrl.SpriteBanks[ i ].ulSpriteSize   = 0;
        SprCtrl.SpriteBanks[ i ].ulSpriteType   = eSpriteType_Raw;
        SprCtrl.SpriteBanks[ i ].pSpriteData    = NULL;
    }

    SprCtrl.Flags.Initialized = true;
}

/** ----------------------------------------------------------------------------
    @brief 		Close the sprite library
    @ingroup 	MainShell
    @return 	bool            - true if successful
 -----------------------------------------------------------------------------*/
void LIB_Sprites_Close( void )
{
    //printf("LIB_Sprites_Close\n");
}

/** ----------------------------------------------------------------------------
    @brief 		Register a sprite bank
    @ingroup 	MainShell
    @param      eBank           - Sprite bank to register
    @param      eType           - Sprite type
    @param      ulResourceID    - Resource ID
    @param      pSpriteData     - Pointer to the sprite data
    @param      ulSpriteSize    - Size of the sprite data
    @param      sprW            - Sprite width
    @param      sprH            - Sprite height
    @return 	bool            - true if successful
 -----------------------------------------------------------------------------*/
bool LIB_Sprites_RegisterBank( eSpriteBank_t eBank, eSpriteType_t eType, uint32_t ulResourceID, uint8_t* pSpriteData, uint32_t ulSpriteSize, uint16_t sprW, uint16_t sprH ) 
{
    bool bRet = false;

    // long list of protective checks
    if ( (eBank < MAX_SPRITE_BANKS) && (pSpriteData != NULL) && (ulSpriteSize > 0) && SprCtrl.Flags.Initialized == true && SprCtrl.SpriteBanks[ eBank ].pSpriteData == NULL )
    {
        // store the sprite bank data
        SprCtrl.SpriteBanks[ eBank ].ulSpriteSize   = ulSpriteSize;
        SprCtrl.SpriteBanks[ eBank ].pSpriteData    = pSpriteData;
        SprCtrl.SpriteBanks[ eBank ].ulSpriteID     = ulResourceID;
        SprCtrl.SpriteBanks[ eBank ].ulSpriteType   = eType;
        SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth  = sprW;
        SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight = sprH;
        bRet = true;
    }

    // return the result
    return bRet;
}


/** ----------------------------------------------------------------------------
    @brief 		Draw a sprite
    @ingroup 	MainShell
    @param      eBank           - Sprite bank to draw from
    @param      sprNum          - Sprite number
    @param      x               - X position
    @param      y               - Y position
    @return 	bool            - true if successful
 -----------------------------------------------------------------------------*/
bool LIB_Sprites_Draw( eSpriteBank_t eBank, uint32_t sprNum, uint32_t x, uint32_t y )
{
    bool bRet = false;

    // long list of protective checks
    if ( (eBank < MAX_SPRITE_BANKS) && SprCtrl.Flags.Initialized == true && SprCtrl.SpriteBanks[ eBank ].pSpriteData != NULL )
    {
        // draw the sprite
        if ( SprCtrl.SpriteBanks[ eBank ].ulSpriteType == eSpriteType_Raw )
        {
            // draw the raw sprite
            uint8_t* pSpriteData = SprCtrl.SpriteBanks[ eBank ].pSpriteData;
            uint8_t* pScreen = HWSCREEN_GetScreenBuffer();

            pSpriteData += sprNum * ( SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth * SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight );

            // draw the sprite
            for( uint32_t y = 0; y < SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight; y++ )
            {
                for( uint32_t x = 0; x < SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth; x++ )
                {
                    if ( *pSpriteData != 0 )
                    {
                        pScreen[ ( y * 320 ) + x ] = *pSpriteData;
                    }
                    *pSpriteData++;
                }
            }
        }
 
        bRet = true;
    }

    // return the result
    return bRet;
}

//-----------------------------------------------------------------------------
// End of file: LIB_Sprites.c
//-----------------------------------------------------------------------------
