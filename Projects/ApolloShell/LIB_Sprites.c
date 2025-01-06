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
#define SCREENWIDTH      640
#define SCREENHEIGHT     480

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

/** -----------------------------------------------------------------------------
    @brief      Draw a sprite flipped
    @ingroup    MainShell
    @param      eBank           - Sprite bank to draw from
    @param      sprNum          - Sprite number
    @param      x               - X position
    @param      y               - Y position
    @return     bool            - true if successful
   ---------------------------------------------------------------------------- */
bool LIB_Sprites_DrawFlipped( eSpriteBank_t eBank, uint32_t sprNum, uint32_t x, uint32_t y )
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
                        if ( dx+x < screenWidth && dy+y < screenHeight && dy+y >= 0 && dx+x >= 0 )  
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

#define DEBUG 0
#if DEBUG == 1
            printf( "pSprite = %08X\n", pSprite );
            printf( "NumSprs = %08X\n", SprCtrl.SpriteBanks[ eBank ].ulNumSprites  );
            printf( "Data dump\n" );
            uint8_t* pSpriteStart = pSprite;
            for( uint32_t i = 0; i < 0x100; i++ )
            {
                if ( i != 0 && i % 16 == 0 )
                {
                    printf( "\n" );
                }
                printf( "%02X ", *pSprite++ );
            }
            printf( "\nData END\n" );
            pSprite = pSpriteStart;
#endif
            // pSprite now points to the start of the sprite data
            // draw the sprite
            uint32_t xStart = x + SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth -1;
            uint32_t ulIndex = 0;

            while(  *pSprite != 0xFF )
            {
                uint8_t uCmd = *pSprite++;
                if ( uCmd == 0xC9 )
                {
                    y++;
                    x = xStart;
#if DEBUG == 1
        printf( "NEWLINE: x = %d,y = %d\n", x, y );
#endif
                    continue;
                }
                x -= uCmd;
                ulIndex = *pSprite++;

                while( ulIndex != 0 )
                {
#if DEBUG == 0
                    if ( x < screenWidth && y < screenHeight && y >= 0 && x >= 0 )  
                    {
                        pScreen[ ( y * screenWidth ) + x ] = *pSprite;
                    }
                    x--;
                    pSprite++;
                    ulIndex--;
#else
                    pSprite++;
                    x--;
#endif                    
                }
#if DEBUG == 1
        printf( "DATADRAWN: x = %d,y = %d\n", x, y );
#endif
            }
        }

        bRet = true;
    }

    // return the result
    return bRet;
}


/** ----------------------------------------------------------------------------
    @brief 		Draw a raw sprite part
    @ingroup 	MainShell
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
bool LIB_Sprites_DrawRawPart( eSpriteBank_t eBank, uint32_t sprNum, uint32_t x, uint32_t y, uint32_t xOff, uint32_t yOff, uint32_t xSize, uint32_t ySize )
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
            for( uint32_t dy = 0; dy < ySize; dy++ )
            {
                for( uint32_t dx = 0; dx < xSize; dx++ )
                {
                    if ( *pSpriteData != 0 )
                    {
                        if ( dx+x < screenWidth && dy+y < screenHeight && dy+y >= 0 && dx+x >= 0 )  
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
        uint8_t* pSpriteData = SprCtrl.SpriteBanks[ eBank ].pSpriteData;
        uint8_t* pScreen = Hardware_GetScreenPtr();
        uint32_t screenWidth = Hardware_GetScreenWidth();
        uint32_t screenHeight = Hardware_GetScreenHeight();

        // draw the sprite
        if ( SprCtrl.SpriteBanks[ eBank ].ulSpriteType == eSpriteType_Raw )
        {
            // draw the raw sprite

            pSpriteData += sprNum * ( SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth * SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight );

            // draw the data one pixel at a time
            for( uint32_t dy = 0; dy < SprCtrl.SpriteBanks[ eBank ].ulSpriteHeight; dy++ )
            {
                uint8_t* pScrn = pScreen + ( (dy+y) * screenWidth ) + x;

                for( uint32_t dx = 0; dx < SprCtrl.SpriteBanks[ eBank ].ulSpriteWidth; dx++ )
                {
                    if ( *pSpriteData != 0 )
                    {
                        if ( dx+x < screenWidth && dy+y < screenHeight && dy+y >= 0 && dx+x >= 0 )  
                        {
                            *pScrn = *pSpriteData;
                        }
                    }
                    pScrn++;
                    pSpriteData++;
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

#define DEBUG 0
#if DEBUG == 1
            printf( "pSprite = %08X\n", pSprite );
            printf( "NumSprs = %08X\n", SprCtrl.SpriteBanks[ eBank ].ulNumSprites  );
            printf( "Data dump\n" );
            uint8_t* pSpriteStart = pSprite;
            for( uint32_t i = 0; i < 0x100; i++ )
            {
                if ( i != 0 && i % 16 == 0 )
                {
                    printf( "\n" );
                }
                printf( "%02X ", *pSprite++ );
            }
            printf( "\nData END\n" );
            pSprite = pSpriteStart;
#endif
            // pSprite now points to the start of the sprite data
            // draw the sprite
            uint32_t xStart = x;
            uint32_t ulIndex = 0;

            while(  *pSprite != 0xFF )
            {
                uint8_t uCmd = *pSprite++;
                if ( uCmd == 0xC9 )
                {
                    y++;
                    x = xStart;
#if DEBUG == 1
        printf( "NEWLINE: x = %d,y = %d\n", x, y );
#endif
                    continue;
                }
                x += uCmd;
                ulIndex = *pSprite++;

                while( ulIndex != 0 )
                {
#if DEBUG == 0
                    if ( x < screenWidth && y < screenHeight && y >= 0 && x >= 0 )  
                    {
                        pScreen[ ( y * screenWidth ) + x ] = *pSprite;
                    }
                    x++;
                    pSprite++;
                    ulIndex--;
#else
                    pSprite++;
                    x++;
#endif                    
                }
#if DEBUG == 1
        printf( "DATADRAWN: x = %d,y = %d\n", x, y );
#endif
            }
        }

        bRet = true;
    }

    // return the result
    return bRet;
}

/** ----------------------------------------------------------------------------
    @brief 		Remap the sprite colours
    @ingroup 	MainShell
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
                for( uint32_t i = 0; i < SprCtrl.SpriteBanks[ eSpriteBank ].ulSpriteSize; i++ )
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

//-----------------------------------------------------------------------------
// End of file: LIB_Sprites.c
//-----------------------------------------------------------------------------
