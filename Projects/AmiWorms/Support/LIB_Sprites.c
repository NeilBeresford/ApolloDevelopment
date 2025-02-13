/** ---------------------------------------------------------------------------
	@file		LIB_Sprites.c
	@defgroup 	AmiWorms Apollo V4 Shell
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
#include "../Includes/FlagStruct.h"
#include "../Includes/HWScreen.h"
#include "../Includes/ResourceFiles.h"
#include "../Includes/ResourceHandling.h"
#include "../Includes/LIB_Sprites.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define MAX_SPRITE_BANKS 2000
#define SCREENWIDTH      640
#define SCREENHEIGHT     480

//-----------------------------------------------------------------------------
// Typedefs and enums
//-----------------------------------------------------------------------------

/**-----------------------------------------------------------------------------
    @brief      Sprite Control structure
    @ingroup 	AmiWorms
 ---------------------------------------------------------------------------- */
typedef struct 
{
    FlagStruct_t   Flags;                               //!< Flags
    uint8_t        nOverWriteColour;                    //!< Overwrite colour
    SpriteBank_t   SpriteBanks[ MAX_SPRITE_BANKS ];     //!< Sprite banks 
    int32_t        ulClipLeft;                          //!< Clip left
    int32_t        ulClipTop;                           //!< Clip top
    int32_t        ulClipRight;                         //!< Clip right
    int32_t        ulClipBottom;                        //!< Clip bottom

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
    @ingroup 	AmiWorms
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
    LIB_Sprites_SetClipArea( 0, 0, SCREENWIDTH, SCREENHEIGHT );
    SprCtrl.Flags.Initialized = true;
    SprCtrl.nOverWriteColour = 0;
}

/** ----------------------------------------------------------------------------
    @brief 		Close the sprite library
    @ingroup 	AmiWorms
    @return 	bool            - true if successful
 -----------------------------------------------------------------------------*/
void LIB_Sprites_Close( void )
{
    //printf("LIB_Sprites_Close\n");
}

/** ----------------------------------------------------------------------------
    @brief 		Returns the width of the sprites in the bank
    @ingroup 	AmiWorms
    @param      eBank           - Sprite bank to draw from
    @return     uint32_t        - Sprite width
 -----------------------------------------------------------------------------*/
uint32_t LIB_Sprites_GetWidth( eSpriteBank_t eBank )
{
    uint32_t ulRet = 0;

    if ( eBank < MAX_SPRITE_BANKS && SprCtrl.Flags.Initialized == true )
    {
        ulRet = SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth;
    }

    return ulRet;
}

/** ----------------------------------------------------------------------------
    @brief 		Returns the height of the sprites in the bank
    @ingroup 	AmiWorms
    @param      eBank           - Sprite bank to draw from
    @return     uint32_t        - Sprite height
 -----------------------------------------------------------------------------*/
uint32_t LIB_Sprites_GetHeight( eSpriteBank_t eBank )
{
    uint32_t ulRet = 0;

    if ( eBank < MAX_SPRITE_BANKS && SprCtrl.Flags.Initialized == true )
    {
        ulRet = SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight;
    }

    return ulRet;
}

/** ----------------------------------------------------------------------------
    @brief 		Overrides sprite data with solid colour. 0 = no overwrite 
    @ingroup 	AmiWorms
    @param      nColour         - Colour to overwrite with
 -----------------------------------------------------------------------------*/
void LIB_Sprites_SetOverwriteColour( uint8_t nColour )
{
    SprCtrl.nOverWriteColour = nColour;
}

/** ----------------------------------------------------------------------------
    @brief 		Register a sprite bank
    @ingroup 	AmiWorms
    @param      eBank           - Sprite bank to register
    @param      eType           - Sprite type
    @param      ulResourceID    - Resource ID
    @param      pSpriteData     - Pointer to the sprite data
    @param      ulSpriteSize    - Size of the sprite data
    @param      ulNumSprites    - Total sprites
    @param      sprW            - Sprite width
    @param      sprH            - Sprite height
    @return 	bool            - true if successful
 -----------------------------------------------------------------------------*/
bool LIB_Sprites_RegisterBank( eSpriteBank_t eBank, eSpriteType_t eType, uint32_t ulResourceID, uint8_t* pSpriteData, uint32_t ulSpriteSize, uint32_t ulNumSprites, uint16_t sprW, uint16_t sprH ) 
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
        SprCtrl.SpriteBanks[ eBank ].ulNumSprites   = ulNumSprites;
        SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth  = sprW;
        SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight = sprH;
        bRet = true;
    }

    // return the result
    return bRet;
}

/** ----------------------------------------------------------------------------
    @brief      Sets the clip area
    @ingroup    AmiWorms
    @param      x               - X position
    @param      y               - Y position
    @param      w               - Width
    @param      h               - Height
 ---------------------------------------------------------------------------- */
void LIB_Sprites_SetClipArea( uint32_t x, uint32_t y, uint32_t w, uint32_t h )
{
    // set the clip area
    SprCtrl.ulClipLeft   = x;
    SprCtrl.ulClipTop    = y;
    SprCtrl.ulClipRight  = x + w;
    SprCtrl.ulClipBottom = y + h;
}

/** ----------------------------------------------------------------------------
    @brief      Calculates the Sprite Dimentions
    @ingroup    AmiWorms
    @param      eSpriteBank     - Sprite bank to remap
    @param      SprNum          - Sprite number
    @param      pSD             - Pointer to the sprite dimentions
    @return     bool            - true if successful
   ---------------------------------------------------------------------------- */
bool LIB_Sprites_GetSpriteDimentions( eSpriteBank_t eBank, uint16_t SprNum, pSprDimention_t pSD )
{
    bool bSuccess = false;

    if ( eBank < MAX_SPRITE_BANKS && (pSD != NULL) && SprCtrl.Flags.Initialized == true )
    {
        int32_t nW = -1, nH = -1, nOL = 0xFFFF, nOR = -1, nOT = 0xFFFF, nOB = -1;        
        pSpriteBank_t pSB = &SprCtrl.SpriteBanks[ eBank ];
        uint8_t* pSpriteData    = pSB->pSpriteData;
        uint32_t nSpriteWidth   = pSB->ulSpriteWidth;
        uint32_t nSpriteHeight  = pSB->ulSpriteHeight;

        if ( pSB->ulSpriteType == eRAW )
        {
            // draw the raw sprite
            pSpriteData += SprNum * ( nSpriteWidth * nSpriteHeight );

            // draw the sprite
            for( int32_t dy = 0; dy < nSpriteHeight; dy++ )
            {
                for( int32_t dx = 0; dx < nSpriteWidth; dx++ )
                {
                    if ( *pSpriteData != 0 )
                    {
                        if ( dx < nOL ) nOL = dx;
                        if ( dx > nOR ) nOR = dx;
                        if ( dy < nOT ) nOT = dy;
                        if ( dy > nOB ) nOB = dy;
                    }
                    pSpriteData++;
                }
            }

            nW = nOR - nOL;
            nH = nOB - nOT;

            pSD->nWidth         = nW;
            pSD->nHeight        = nH;
            pSD->nXOffsetLeft   = nOL;
            pSD->nXOffsetRight  = nOR;
            pSD->nYOffsetTop    = nOT;
            pSD->nYOffsetBottom = nOB;

            bSuccess = true;
        }
        else
        {
            // Calculate the compressed sprite dimentions
            //get to start of index for sprites..
            uint8_t* pSprite = pSpriteData;
            uint32_t ulSpriteIndex = 0;
            uint8_t uCmd = 0;

            pSprite += 12;
            while( *pSprite != ':' )
            {
                pSprite++;
            }
            pSprite++;
            ulSpriteIndex = pSprite - pSpriteData;
            uint32_t ulOffset = *(((uint32_t*)pSprite) + SprNum);
            ulOffset = Hardware_SwapLong( ulOffset );
            pSprite += (pSB->ulNumSprites * 4) + ulOffset;

            // pSprite now points to the start of the sprite data
            // parse the sprite, store the dimentions
            int32_t xStart = 0;
            uint32_t ulIndex = 0;
            int32_t y = 0;

            while(  *pSprite != 0xFF )
            {
                uint8_t uCmd = *pSprite++;
                if ( uCmd == 0xC9 )
                {
                    xStart = 0;
                    y++;
                    continue;
                }
                xStart += uCmd;
                ulIndex = *pSprite++;

                while( ulIndex != 0 )
                {
                    if ( *pSprite != 0 )
                    {
                        if ( xStart < nOL ) nOL = xStart;
                        if ( xStart > nOR ) nOR = xStart;
                        if ( y < nOT ) nOT = y;
                        if ( y > nOB ) nOB = y;
                    }
                    xStart++;
                    pSprite++;
                    ulIndex--;
                }
            }
            nW = nOR - nOL;
            nH = nOB - nOT;

            pSD->nWidth         = nW;
            pSD->nHeight        = nH;
            pSD->nXOffsetLeft   = nOL;
            pSD->nXOffsetRight  = nOR;
            pSD->nYOffsetTop    = nOT;
            pSD->nYOffsetBottom = nOB;

            //printf("SprBank %d, spr %d, nW %d nH %d nOL %d nOR %d nOT %d nOB %d\n", eBank, SprNum, nW, nH, nOL, nOR, nOT, nOB);

            bSuccess = true;
        }
    }
    return bSuccess;
}


/** -----------------------------------------------------------------------------
    @brief      Draw a sprite flipped
    @ingroup    AmiWorms
    @param      eBank           - Sprite bank to draw from
    @param      sprNum          - Sprite number
    @param      x               - X position
    @param      y               - Y position
    @return     bool            - true if successful
   ---------------------------------------------------------------------------- */
bool LIB_Sprites_DrawFlipped( eSpriteBank_t eBank, uint32_t sprNum, int32_t x, int32_t y )
{
    bool bRet = false;

    // long list of protective checks
    if ( (eBank < MAX_SPRITE_BANKS) && SprCtrl.Flags.Initialized == true && SprCtrl.SpriteBanks[ eBank ].pSpriteData != NULL )
    {
        uint8_t* pSpriteData = SprCtrl.SpriteBanks[ eBank ].pSpriteData;
        uint8_t* pScreen = Hardware_GetScreenPtr();
        uint32_t screenWidth = Hardware_GetScreenWidth();
        uint32_t screenHeight = Hardware_GetScreenHeight();

        // draw the sprite
        if ( SprCtrl.SpriteBanks[ eBank ].ulSpriteType == eSpriteType_Raw )
        {
            // draw the raw sprite

            pSpriteData += sprNum * ( SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth * SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight );

            // draw the sprite
            for( int32_t dy = 0; dy < SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight; dy++ )
            {
                for( int32_t dx = SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth -1; dx >= 0; dx-- )
                {
                    if ( *pSpriteData != 0 )
                    {
                        if ( dx+x >= SprCtrl.ulClipLeft && dy+y < SprCtrl.ulClipBottom && dy+y >= SprCtrl.ulClipTop && dx+x < SprCtrl.ulClipRight )  
                        {
                            pScreen[ ( (dy+y) * screenWidth ) + dx + x ] = *pSpriteData;
                        }
                    }
                    *pSpriteData++;
                }
            }
        }
        else
        {
            // draw the compressed sprite
            //get to start of index for sprites..
            uint8_t* pSprite = pSpriteData;
            uint32_t ulSpriteIndex = 0;
            uint8_t uCmd = 0;

            pSprite += 12;   
            while( *pSprite != ':' )
            {
                pSprite++;
            }
            pSprite++;
            ulSpriteIndex = pSprite - pSpriteData;
            uint32_t ulOffset = *(((uint32_t*)pSprite) + sprNum);
            ulOffset = Hardware_SwapLong( ulOffset );
            pSprite += (SprCtrl.SpriteBanks[ eBank ].ulNumSprites * 4) + ulOffset;

            // pSprite now points to the start of the sprite data
            // draw the sprite
            int32_t xStart = x + SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth -1;
            uint32_t ulIndex = 0;


            // Single colour or normal sprite?
            if ( SprCtrl.nOverWriteColour == 0 )
            {
                while(  *pSprite != 0xFF )
                {
                    uint8_t uCmd = *pSprite++;
                    if ( uCmd == 0xC9 )
                    {
                        y++;
                        x = xStart;
                        continue;
                    }
                    x -= uCmd;
                    ulIndex = *pSprite++;

                    while( ulIndex != 0 )
                    {
                        if ( x < SprCtrl.ulClipRight && y < SprCtrl.ulClipBottom && y >= SprCtrl.ulClipTop && x >= SprCtrl.ulClipLeft )  
                        {
                            pScreen[ ( y * screenWidth ) + x ] = *pSprite;
                        }
                        x--;
                        pSprite++;
                        ulIndex--;
                    }
                }
            }
            else
            {
                while(  *pSprite != 0xFF )
                {
                    uint8_t uCmd = *pSprite++;
                    if ( uCmd == 0xC9 )
                    {
                        y++;
                        x = xStart;
                        continue;
                    }
                    x -= uCmd;
                    ulIndex = *pSprite++;

                    while( ulIndex != 0 )
                    {
                        if ( x < SprCtrl.ulClipRight && y < SprCtrl.ulClipBottom && y >= SprCtrl.ulClipTop && x >= SprCtrl.ulClipLeft )  
                        {
                            pScreen[ ( y * screenWidth ) + x ] = SprCtrl.nOverWriteColour;
                        }
                        x--;
                        pSprite++;
                        ulIndex--;

                    }
                }
            }
        }

        bRet = true;
    }

    // return the result
    return bRet;
}

/** -----------------------------------------------------------------------------
    @brief      Draw a sprite flipped, scaled for the map
    @ingroup    AmiWorms
    @param      eBank           - Sprite bank to draw from
    @param      sprNum          - Sprite number
    @param      x               - X position
    @param      y               - Y position
    @return     bool            - true if successful
   ---------------------------------------------------------------------------- */
bool LIB_Sprites_DrawMapFlipped( eSpriteBank_t eBank, uint32_t sprNum, int32_t x, int32_t y )
{
    bool bRet = false;

    // long list of protective checks
    if ( (eBank < MAX_SPRITE_BANKS) && SprCtrl.Flags.Initialized == true && SprCtrl.SpriteBanks[ eBank ].pSpriteData != NULL )
    {
        uint8_t* pSpriteData = SprCtrl.SpriteBanks[ eBank ].pSpriteData;
        uint8_t* pScreen = Hardware_GetScreenPtr();
        uint32_t screenWidth = Hardware_GetScreenWidth();
        uint32_t screenHeight = Hardware_GetScreenHeight();

        // draw the sprite
        if ( SprCtrl.SpriteBanks[ eBank ].ulSpriteType == eSpriteType_Raw )
        {
            // draw the raw sprite

            pSpriteData += sprNum * ( SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth * SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight );

            // draw the sprite
            for( int32_t dy = 0; dy < SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight; dy++ )
            {
                for( int32_t dx = SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth -1; dx >= 0; dx-- )
                {
                    if ( *pSpriteData != 0 )
                    {
                        if ( dx+x >= SprCtrl.ulClipLeft && dy+y < SprCtrl.ulClipBottom && dy+y >= SprCtrl.ulClipTop && dx+x < SprCtrl.ulClipRight )  
                        {
                            pScreen[ ( (dy+y) * screenWidth ) + dx + x ] = *pSpriteData;
                        }
                    }
                    *pSpriteData++;
                }
            }
        }
        else
        {
            // draw the compressed sprite
            //get to start of index for sprites..
            uint8_t* pSprite = pSpriteData;
            uint32_t ulSpriteIndex = 0;
            uint8_t uCmd = 0;

            pSprite += 12;   
            while( *pSprite != ':' )
            {
                pSprite++;
            }
            pSprite++;
            ulSpriteIndex = pSprite - pSpriteData;
            uint32_t ulOffset = *(((uint32_t*)pSprite) + sprNum);
            ulOffset = Hardware_SwapLong( ulOffset );
            pSprite += (SprCtrl.SpriteBanks[ eBank ].ulNumSprites * 4) + ulOffset;

            // pSprite now points to the start of the sprite data
            // draw the sprite
            int32_t xStart = x + SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth -1;
            uint32_t ulIndex = 0;


            // Single colour or normal sprite?
            if ( SprCtrl.nOverWriteColour == 0 )
            {
                while(  *pSprite != 0xFF )
                {
                    uint8_t uCmd = *pSprite++;
                    if ( uCmd == 0xC9 )
                    {
                        y++;
                        x = xStart;
                        continue;
                    }
                    x -= uCmd;
                    ulIndex = *pSprite++;

                    while( ulIndex != 0 )
                    {
                        if ( x < SprCtrl.ulClipRight && y < SprCtrl.ulClipBottom && y >= SprCtrl.ulClipTop && x >= SprCtrl.ulClipLeft )  
                        {
                            pScreen[ ( y * screenWidth ) + x ] = *pSprite;
                        }
                        x--;
                        pSprite++;
                        ulIndex--;
                    }
                }
            }
            else
            {
                while(  *pSprite != 0xFF )
                {
                    uint8_t uCmd = *pSprite++;
                    if ( uCmd == 0xC9 )
                    {
                        y++;
                        x = xStart;
                        continue;
                    }
                    x -= uCmd;
                    ulIndex = *pSprite++;

                    while( ulIndex != 0 )
                    {
                        if ( x < SprCtrl.ulClipRight && y < SprCtrl.ulClipBottom && y >= SprCtrl.ulClipTop && x >= SprCtrl.ulClipLeft )  
                        {
                            pScreen[ ( y * screenWidth ) + x ] = SprCtrl.nOverWriteColour;
                        }
                        x--;
                        pSprite++;
                        ulIndex--;

                    }
                }
            }
        }

        bRet = true;
    }

    // return the result
    return bRet;
}




/** ----------------------------------------------------------------------------
    @brief 		Draw a raw sprite part
    @ingroup 	AmiWorms
    @param      eBank           - Sprite bank to draw from
    @param      sprNum          - Sprite number
    @param      x               - X position
    @param      y               - Y position
    @param      xOff            - X offset
    @param      yOff            - Y offset
    @param      xSize           - X size
    @param      ySize           - Y size
    @return 	bool            - true if successful
 -----------------------------------------------------------------------------*/
bool LIB_Sprites_DrawRawPart( eSpriteBank_t eBank, uint32_t sprNum, int32_t x, int32_t y, uint32_t xOff, uint32_t yOff, uint32_t xSize, uint32_t ySize )
{
    bool bRet = false;

    // long list of protective checks
    if ( (eBank < MAX_SPRITE_BANKS) && SprCtrl.Flags.Initialized == true && SprCtrl.SpriteBanks[ eBank ].pSpriteData != NULL )
    {
        uint8_t* pSpriteData = SprCtrl.SpriteBanks[ eBank ].pSpriteData;
        uint8_t* pScreen = Hardware_GetScreenPtr();
        uint32_t screenWidth = Hardware_GetScreenWidth();
        uint32_t screenHeight = Hardware_GetScreenHeight();

        // draw the sprite
        if ( SprCtrl.SpriteBanks[ eBank ].ulSpriteType == eSpriteType_Raw )
        {
            // draw the raw sprite

            pSpriteData += sprNum * ( SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth * SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight );

            pSpriteData += ( yOff * SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth ) + xOff;

            // draw the sprite
            for( int32_t dy = 0; dy < ySize; dy++ )
            {
                for( int32_t dx = 0; dx < xSize; dx++ )
                {
                    if ( *pSpriteData != 0 )
                    {
                        if ( dx+x >= SprCtrl.ulClipLeft && dy+y < SprCtrl.ulClipBottom && dy+y >= SprCtrl.ulClipTop && dx+x < SprCtrl.ulClipRight )  
                        {
                            pScreen[ ( (dy+y) * screenWidth ) + dx + x ] = *pSpriteData;
                        }
                    }
                    pSpriteData++;
                }
                pSpriteData += SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth - xSize;
            }
        }
    }
    return bRet;
}






/** ----------------------------------------------------------------------------
    @brief 		Draw a sprite
    @ingroup 	AmiWorms
    @param      eBank           - Sprite bank to draw from
    @param      sprNum          - Sprite number
    @param      x               - X position
    @param      y               - Y position
    @return 	bool            - true if successful
 -----------------------------------------------------------------------------*/
bool LIB_Sprites_Draw( eSpriteBank_t eBank, uint32_t sprNum, int32_t x, int32_t y )
{
    bool bRet = false;

    // long list of protective checks
    if ( (eBank < MAX_SPRITE_BANKS) && SprCtrl.Flags.Initialized == true && SprCtrl.SpriteBanks[ eBank ].pSpriteData != NULL )
    {
        uint8_t* pSpriteData = SprCtrl.SpriteBanks[ eBank ].pSpriteData;
        uint8_t* pScreen = Hardware_GetScreenPtr();
        uint32_t screenWidth = Hardware_GetScreenWidth();
        uint32_t screenHeight = Hardware_GetScreenHeight();

        // draw the sprite
        if ( SprCtrl.SpriteBanks[ eBank ].ulSpriteType == eSpriteType_Raw )
        {
            // coarse clip check
            if ( x + SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth < SprCtrl.ulClipLeft || x > SprCtrl.ulClipRight || y + SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight < SprCtrl.ulClipTop || y > SprCtrl.ulClipBottom )
            {
                return bRet;
            }

            // draw the raw sprite
            int32_t xOff = 0, yOff = 0;
            int32_t xEnd = x + SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth, yEnd = y + SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight;

            // reduce the drawing area with clipping
            pSpriteData += sprNum * ( SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth * SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight );
            if ( y < SprCtrl.ulClipTop )
            {
                yOff = SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight - (SprCtrl.ulClipTop - y);
                pSpriteData += (SprCtrl.ulClipTop - y) * SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth;
                y = SprCtrl.ulClipTop;
                yEnd =  yOff + y;
            }
            if ( x < SprCtrl.ulClipLeft )
            {
                xOff = SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth - (SprCtrl.ulClipLeft - x);
                pSpriteData += (SprCtrl.ulClipLeft - x);
                xEnd = xOff;
                x = SprCtrl.ulClipLeft;
            }
            if ( x + SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth >= SprCtrl.ulClipRight )
            {
                xEnd = SprCtrl.ulClipRight;
            }
            if ( y + SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight >= SprCtrl.ulClipBottom )
            {
                yEnd = SprCtrl.ulClipBottom;
                
            }
    #if 0
            printf( "Adjusted spriteData = %08X\n", pSpriteData );
            printf( "x = %d, y = %d, xEnd = %d, yEnd = %d\n", x, y, xEnd, yEnd );
            printf( "Spr W %d, Spr H %d\n", SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth, SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight );
            exit( 0 );
    #endif
            // draw the data one pixel at a time
            uint8_t*  pSprBack = pSpriteData;

            for( int32_t dy = y; dy < yEnd; dy++ )
            {
                uint8_t* pScrn = pScreen + ( dy * screenWidth ) + x;
                for( int32_t dx = x; dx < xEnd; dx++ )
                {
                    if ( *pSpriteData != 0 )
                    {
                        *pScrn = *pSpriteData;
                    }
                    pScrn++;
                    pSpriteData++;
                }
                pSpriteData = pSprBack + SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth;
                pSprBack = pSpriteData;
            }
        }
        else
        {
            // draw the compressed sprite
            //get to start of index for sprites..
            uint8_t* pSprite = pSpriteData;
            uint32_t ulSpriteIndex = 0;
            uint8_t uCmd = 0;

            pSprite += 12;   
            while( *pSprite != ':' )
            {
                pSprite++;
            }
            pSprite++;
            ulSpriteIndex = pSprite - pSpriteData;
            uint32_t ulOffset = *(((uint32_t*)pSprite) + sprNum);
            ulOffset = Hardware_SwapLong( ulOffset );
            pSprite += (SprCtrl.SpriteBanks[ eBank ].ulNumSprites * 4) + ulOffset;

            // pSprite now points to the start of the sprite data
            // draw the sprite
            int32_t xStart = x;
            uint32_t ulIndex = 0;

            // Single colour or normal sprite?
            if ( SprCtrl.nOverWriteColour != 0 )
            {
                uint8_t nCol = SprCtrl.nOverWriteColour;
                while(  *pSprite != 0xFF )
                {
                    uint8_t uCmd = *pSprite++;
                    if ( uCmd == 0xC9 )
                    {
                        y++;
                        x = xStart;
                        continue;
                    }
                    x += uCmd;
                    ulIndex = *pSprite++;

                    while( ulIndex != 0 )
                    {
                        if ( x < SprCtrl.ulClipRight && y < SprCtrl.ulClipBottom && y >= SprCtrl.ulClipTop && x >= SprCtrl.ulClipLeft )  
                        {
                            pScreen[ ( y * screenWidth ) + x ] = nCol;
                        }
                        x++;
                        pSprite++;
                        ulIndex--;
                    }
                }
            }
            else
            {
                while(  *pSprite != 0xFF )
                {
                    uint8_t uCmd = *pSprite++;
                    if ( uCmd == 0xC9 )
                    {
                        y++;
                        x = xStart;
                        continue;
                    }
                    x += uCmd;
                    ulIndex = *pSprite++;

                    while( ulIndex != 0 )
                    {
                        if ( x < SprCtrl.ulClipRight && y < SprCtrl.ulClipBottom && y >= SprCtrl.ulClipTop && x >= SprCtrl.ulClipLeft )  
                        {
                            pScreen[ ( y * screenWidth ) + x ] = *pSprite;
                        }
                        x++;
                        pSprite++;
                        ulIndex--;
                    }
                }
            }

        }

        bRet = true;
    }

    // return the result
    return bRet;
}



/** ----------------------------------------------------------------------------
    @brief 		Draw a sprite, scaled for the MAP screen
    @ingroup 	AmiWorms
    @param      eBank           - Sprite bank to draw from
    @param      sprNum          - Sprite number
    @param      x               - X position
    @param      y               - Y position
    @return 	bool            - true if successful
 -----------------------------------------------------------------------------*/
bool LIB_Sprites_DrawMap( eSpriteBank_t eBank, uint32_t sprNum, int32_t x, int32_t y )
{
    bool bRet = false;

    // long list of protective checks
    if ( (eBank < MAX_SPRITE_BANKS) && SprCtrl.Flags.Initialized == true && SprCtrl.SpriteBanks[ eBank ].pSpriteData != NULL )
    {
        uint8_t* pSpriteData = SprCtrl.SpriteBanks[ eBank ].pSpriteData;
        uint8_t* pScreen = Hardware_GetScreenPtr();
        uint32_t screenWidth = Hardware_GetScreenWidth();
        uint32_t screenHeight = Hardware_GetScreenHeight();

        // draw the sprite
        if ( SprCtrl.SpriteBanks[ eBank ].ulSpriteType == eSpriteType_Raw )
        {
            // coarse clip check
            if ( x + (SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth/3) < SprCtrl.ulClipLeft || x > SprCtrl.ulClipRight || y + (SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight/3) < SprCtrl.ulClipTop || y > SprCtrl.ulClipBottom )
            {
                return bRet;
            }

            // draw the raw sprite
            int32_t xOff = 0, yOff = 0;
            int32_t xEnd = x + (SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth/3), yEnd = y + SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight;

            // reduce the drawing area with clipping
            pSpriteData += sprNum * ( SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth * SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight );
            if ( y < SprCtrl.ulClipTop )
            {
                yOff = (SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight/3) - (SprCtrl.ulClipTop - y);
                pSpriteData += (SprCtrl.ulClipTop - y) * SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth;
                y = SprCtrl.ulClipTop;
                yEnd =  yOff + y;
            }
            if ( x < SprCtrl.ulClipLeft )
            {
                xOff = (SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth/3) - (SprCtrl.ulClipLeft - x);
                pSpriteData += (SprCtrl.ulClipLeft - x);
                xEnd = xOff;
                x = SprCtrl.ulClipLeft;
            }
            if ( x + SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth >= SprCtrl.ulClipRight )
            {
                xEnd = SprCtrl.ulClipRight;
            }
            if ( y + SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight >= SprCtrl.ulClipBottom )
            {
                yEnd = SprCtrl.ulClipBottom;
                
            }
    #if 0
            printf( "Adjusted spriteData = %08X\n", pSpriteData );
            printf( "x = %d, y = %d, xEnd = %d, yEnd = %d\n", x, y, xEnd, yEnd );
            printf( "Spr W %d, Spr H %d\n", SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth, SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight );
            exit( 0 );
    #endif
            // draw the data one pixel at a time
            uint8_t*  pSprBack = pSpriteData;

            for( int32_t dy = y; dy < yEnd; dy++ )
            {
                uint8_t* pScrn = pScreen + ( dy * screenWidth ) + x;
                for( int32_t dx = x; dx < xEnd; dx++ )
                {
                    if ( *pSpriteData != 0 )
                    {
                        *pScrn = *pSpriteData;
                    }
                    pScrn++;
                    pSpriteData += 3;
                }
                pSpriteData = pSprBack + ((SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth) * 3);
                pSprBack = pSpriteData;
            }
        }
        else
        {
            // draw the compressed sprite
            //get to start of index for sprites..
            uint8_t* pSprite = pSpriteData;
            uint32_t ulSpriteIndex = 0;
            uint8_t uCmd = 0;

            pSprite += 12;   
            while( *pSprite != ':' )
            {
                pSprite++;
            }
            pSprite++;
            ulSpriteIndex = pSprite - pSpriteData;
            uint32_t ulOffset = *(((uint32_t*)pSprite) + sprNum);
            ulOffset = Hardware_SwapLong( ulOffset );
            pSprite += (SprCtrl.SpriteBanks[ eBank ].ulNumSprites * 4) + ulOffset;

            // pSprite now points to the start of the sprite data
            // draw the sprite
            int32_t xStart = x;
            uint32_t ulIndex = 0;
            uint32_t nDrawSkip;

            // Single colour or normal sprite?
            if ( SprCtrl.nOverWriteColour != 0 )
            {
                uint8_t nCol = SprCtrl.nOverWriteColour;
                while(  *pSprite != 0xFF )
                {
                    uint8_t uCmd = *pSprite++;
                    if ( uCmd == 0xC9 )
                    {
                        y++;
                        x = xStart;
                    #if 1
                        // skip another two lines...
                        uint32_t nCount = 2;

                        while( nCount )
                        {
                            if ( *pSprite == 0xFF ) { break; };
                            if ( *pSprite == 0xC9 ) { nCount--; pSprite++; continue; }

                            pSprite++;              // skip space
                            pSprite += *pSprite++;
                        }
                    #endif
                        continue;
                    }
                    uCmd /= 3;
            
                    x += uCmd;
                    uCmd = *pSprite / 3;
                    nDrawSkip = *pSprite++;

                    while( uCmd != 0 )
                    {
                        if ( x < SprCtrl.ulClipRight && y < SprCtrl.ulClipBottom && y >= SprCtrl.ulClipTop && x >= SprCtrl.ulClipLeft )  
                        {
                            pScreen[ ( y * screenWidth ) + x ] = nCol;
                        }
                        x++;
                        pSprite+=3;
                        uCmd--;
                    }
                    pSprite += nDrawSkip % 3;
                }
            }
            else
            {
                while(  *pSprite != 0xFF )
                {
                    uint8_t uCmd = *pSprite++;
                    if ( uCmd == 0xC9 )
                    {
                        y++;
                        x = xStart;
                        // skip another two lines...
                        uint32_t nCount = 2;
                    #if 1
                        while( nCount )
                        {
                            if ( *pSprite == 0xFF ) { break; };
                            if ( *pSprite == 0xC9 ) { nCount--; pSprite++; continue; }

                            pSprite++;              // skip space
                            pSprite += *pSprite++;
                        }
                    #endif
                        continue;
                    }
                    uCmd /= 3;
            
                    x += uCmd;
                    uCmd = *pSprite / 3;
                    nDrawSkip = *pSprite++;

                    while( uCmd != 0 )
                    {
                        if ( x < SprCtrl.ulClipRight && y < SprCtrl.ulClipBottom && y >= SprCtrl.ulClipTop && x >= SprCtrl.ulClipLeft )  
                        {
                            pScreen[ ( y * screenWidth ) + x ] = *pSprite;
                        }
                        x++;
                        pSprite+=3;
                        uCmd--;
                    }
                    pSprite += nDrawSkip % 3;
                }
            }

        }

        bRet = true;
    }

    // return the result
    return bRet;
}


/** ----------------------------------------------------------------------------
    @brief 		Remap the sprite colours
    @ingroup 	AmiWorms
    @param      eSpriteBank     - Sprite bank to remap
    @param      ShiftBy         - Shift the colours by
    @return 	bool            - true if successful
 -----------------------------------------------------------------------------*/
bool LIB_Sprites_Remap( eSpriteBank_t eSpriteBank, uint32_t ShiftBy )
{
    bool bRet = false;

    // long list of protective checks
    if ( (eSpriteBank < MAX_SPRITE_BANKS) && SprCtrl.Flags.Initialized == true && SprCtrl.SpriteBanks[ eSpriteBank ].pSpriteData != NULL )
    {
        uint8_t* pSpriteData = SprCtrl.SpriteBanks[ eSpriteBank ].pSpriteData;

        
        if ( SprCtrl.SpriteBanks[ eSpriteBank ].ulSpriteType == eSpriteType_Raw )
        {
            for( uint32_t num = 0; num < SprCtrl.SpriteBanks[ eSpriteBank ].ulNumSprites; num++ )
            {
                pSpriteData = SprCtrl.SpriteBanks[ eSpriteBank ].pSpriteData + ( num * ( SprCtrl.SpriteBanks[ eSpriteBank ].ulSpriteWidth * SprCtrl.SpriteBanks[ eSpriteBank ].ulSpriteHeight ) );
                // remap the raw sprite colours
                for( uint32_t i = 0; i < SprCtrl.SpriteBanks[ eSpriteBank ].ulSpriteHeight*SprCtrl.SpriteBanks[ eSpriteBank ].ulSpriteWidth; i++ )
                {
                    if ( *pSpriteData != 0 )
                    {
                        *pSpriteData = *pSpriteData + ShiftBy;
                    }
                    *pSpriteData++;
                }
            }
        }
        else
        {
            // TODO - this only does the first sprite in the compressed sprite bank
            //        need to do all of them
            
            // remap the compressed sprite colours
            //get to start of index for sprites..
            uint8_t* pSprite = pSpriteData;
            uint32_t ulSpriteIndex = 0;
            uint8_t uCmd = 0;

            pSprite += 12;   
            while( *pSprite != ':' )
            {
                pSprite++;
            }
            pSprite++;
            ulSpriteIndex = pSprite - pSpriteData;

            uint32_t ulOffset = *(((uint32_t*)pSprite) + 0);
            ulOffset = Hardware_SwapLong( ulOffset );
            pSprite += (SprCtrl.SpriteBanks[ eSpriteBank ].ulNumSprites * 4) + ulOffset;

            // pSprite now points to the start of the sprite data
            uint32_t xStart = 0;
            uint32_t ulIndex = 0;

            while(  *pSprite != 0xFF )
            {
                uint8_t uCmd = *pSprite++;
                if ( uCmd == 0xC9 )
                {
                    xStart = 0;
                    continue;
                }
                xStart += uCmd;
                ulIndex = *pSprite++;

                while( ulIndex != 0 )
                {
                    if ( *pSprite != 0 )
                    {
                        *pSprite = *pSprite + ShiftBy;
                    }
                    *pSprite++;
                    ulIndex--;
                }
            }
        }

        bRet = true;
    }

    // return the result
    return bRet;
}

/** ----------------------------------------------------------------------------
    @brief 		Get total number of frames in a sprite bank
    @ingroup 	AmiWorms
    @param      eSpriteresource - Sprite bank to remap
    @return     uint32_t        - Number of frames
 -----------------------------------------------------------------------------*/
uint32_t LIB_Sprites_GetFrames( uint32_t nSprResource )
{
    uint32_t nFrames = 0;

    if ( nSprResource < MAX_SPRITE_BANKS && SprCtrl.Flags.Initialized == true )
    {
        nFrames = SprCtrl.SpriteBanks[ nSprResource ].ulNumSprites;
    }
    return nFrames;
}

/** ----------------------------------------------------------------------------
    @brief 		Get the sprite bank data
    @ingroup 	AmiWorms
    @param      eBank           - Sprite bank to draw from
    @return     pSpriteBank_t   - Pointer to the sprite bank data
 -----------------------------------------------------------------------------*/
uint32_t LIB_Sprites_GetTotalNumSprites( eSpriteBank_t eBank )
{
    uint32_t ulRet = 0;

    if ( eBank < MAX_SPRITE_BANKS && SprCtrl.Flags.Initialized == true )
    {
        ulRet = SprCtrl.SpriteBanks[ eBank ].ulNumSprites;
    }

    return ulRet;
}


//-----------------------------------------------------------------------------
// End of file: LIB_Sprites.c
//-----------------------------------------------------------------------------
