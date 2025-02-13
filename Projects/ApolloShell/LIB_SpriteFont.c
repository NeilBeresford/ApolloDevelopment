/** ---------------------------------------------------------------------------
	@file		LIB_SpriteFont.c
	@defgroup 	MainShell Apollo V4 Shell
	@brief		Display of sprite based fonts
	@date		2020-06-01
	@version	0.1
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
#include "Includes/LIB_Sprites.h"
#include "Includes/ResourceFiles.h"
#include "Includes/LIB_SpriteFont.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define TOTAL_CHARS 96

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------

typedef struct
{
    uint16_t nWidth;
    uint16_t nOffset;

} SprFontOffsets_t, *pSprFontOffsets_t;

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

uint8_t CharConvert[] = 
{
    'A','Z', 0,      // uppercase letters
    'a','z', 26,     // lowercase letters
    '0','9', 52,     // lowercase letters
    '`','`', 62,     // extended characters
    '!','!', 63,     // extended characters
    '@','@', 64,     // extended characters
    '#','#', 65,     // extended characters
    '$','$', 66,     // extended characters
    '%','%', 67,     // extended characters
    '^','^', 68,     // extended characters
    '&','&', 69,     // extended characters
    '*','*', 70,     // extended characters
    '(','(', 71,     // extended characters
    ')',')', 72,     // extended characters
    '-','-', 73,     // extended characters
    '_','_', 74,     // extended characters
    '=','=', 75,     // extended characters
    '+','+', 76,     // extended characters
    '[','[', 77,     // extended characters
    '{','{', 78,     // extended characters
    ']',']', 79,     // extended characters
    '}','}', 80,     // extended characters
    ';',';', 81,     // extended characters
    ':',':', 82,     // extended characters
    '\'','\'', 83,   // extended characters
    '"','"', 84,     // extended characters
    '\\','\\', 85,   // extended characters
    '|','|', 86,     // extended characters
    ',',',', 87,     // extended characters
    '<','<', 88,     // extended characters
    '.','.', 89,     // extended characters
    '>','>', 90,     // extended characters
    '/','/', 91,     // extended characters
    '?','?', 92,     // extended characters
    '£','£', 93,     // extended characters
    '~','~', 94,     // extended characters
    0                // end of table
};

SprFontOffsets_t SprFntOffs[ eFont_TotalFonts ][ TOTAL_CHARS ];

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

/** ----------------------------------------------------------------------------
    @brief      Draw a sprite font
    @ingroup    MainShell
    @param      ulSpriteIndex   - Sprite index
    @param      ulX             - X position
    @param      ulY             - Y position
    @param      pszText         - Text to draw
    @return     void
 ---------------------------------------------------------------------------- */
void LIB_SpriteFont_Draw( uint32_t ulSpriteIndex,  int32_t ulX, int32_t ulY, uint8_t* pszText )
{
    int32_t ulXPos = ulX;
    int32_t ulYPos = ulY;
    int32_t ulSpriteWidth = 0;
    int32_t nFontIndex = ulSpriteIndex;
    ulSpriteIndex += ResourceHandling_GetGroupStartResource( eGroups_Font );
    ulSpriteWidth = (float)LIB_Sprites_GetWidth( ulSpriteIndex ) * 0.33f;

    for( uint32_t nChar = 0; nChar < strlen( pszText ); nChar++ )
    {
        uint8_t cChar = pszText[ nChar ];
        uint32_t ulSpriteConvIndex = 0;

        if ( cChar == 0 )
        {
            break;
        }
        
        cChar = LIB_SpriteFont_ConverAscii( cChar );

        if ( cChar != 0xFF )
        {
            ulXPos -= SprFntOffs[ nFontIndex ][ cChar ].nOffset;

            LIB_Sprites_Draw( ulSpriteIndex, cChar, ulXPos, ulYPos );

            ulXPos += SprFntOffs[ nFontIndex ][ cChar ].nOffset;
            ulXPos += SprFntOffs[ nFontIndex ][ cChar ].nWidth;
        }
        else
        {
            ulXPos += ulSpriteWidth;
        }
    }
}

/** ----------------------------------------------------------------------------
    @brief      Get the length of a string in sprite font
    @ingroup    MainShell
    @param      ulSpriteIndex   - Sprite index
    @param      pszText         - Text to measure
    @return     uint32_t        - Length of string
 ---------------------------------------------------------------------------- */
uint32_t LIB_SpriteFont_GetStringLength( uint32_t ulSpriteIndex, uint8_t* pszText )
{
    uint32_t rLength = 0;
    uint32_t nSpace = (float)LIB_Sprites_GetWidth( ulSpriteIndex + ResourceHandling_GetGroupStartResource( eGroups_Font ) ) * 0.33f;
    
    if ( pszText != NULL )
    {
        while( *pszText != 0 )
        {
            uint8_t cChar = *pszText;
            cChar = LIB_SpriteFont_ConverAscii( cChar );
            if ( cChar != 0xFF )
            {
                rLength += SprFntOffs[ ulSpriteIndex ][ cChar ].nWidth;
            }
            else
            {
                rLength += nSpace;
            }
            pszText++;
        }
    }

    return rLength;
}

/** ----------------------------------------------------------------------------
    @brief      Convert an ascii character to a sprite font character
    @ingroup    MainShell
    @param      cChar   - Character to convert
    @return     uint8_t - Converted character
 ---------------------------------------------------------------------------- */
uint8_t LIB_SpriteFont_ConverAscii( uint8_t cChar )
{
    uint32_t ulSpriteConvIndex = 0;
    int8_t cCharConv = 0xFF;

    while( CharConvert[ ulSpriteConvIndex ] != 0 )
    {
        if ( cChar >= CharConvert[ ulSpriteConvIndex ] && cChar <= CharConvert[ ulSpriteConvIndex + 1 ] )
        {
            cCharConv = cChar - CharConvert[ ulSpriteConvIndex ];
            cCharConv += CharConvert[ ulSpriteConvIndex + 2 ];
            break;
        }
        ulSpriteConvIndex += 3;
        if ( CharConvert[ ulSpriteConvIndex ] == 0 )
        {
            cCharConv = 0xFF;
        }
    }
    return cCharConv;
}


/** ----------------------------------------------------------------------------
    @brief      Calculate the font width offsets
    @ingroup    MainShell
    @return     void
 ---------------------------------------------------------------------------- */
void LIB_SpriteFont_CalcFontWidthOffsets( void )
{
    SprDimention_t  sDim        = {0};
    uint32_t        nSprBank    = ResourceHandling_GetGroupStartResource( eGroups_Font );

    for ( uint32_t nFontIndex = 0; nFontIndex < eFont_TotalFonts; nFontIndex++ )
    {
        for( uint32_t nChar = 0; nChar < TOTAL_CHARS; nChar++ )
        {
            LIB_Sprites_GetSpriteDimentions( nSprBank, nChar, &sDim );
            SprFntOffs[ nFontIndex ][ nChar ].nWidth = sDim.nWidth + 2;
            SprFntOffs[ nFontIndex ][ nChar ].nOffset = sDim.nXOffsetLeft;
        }
        nSprBank++;
    }
}

//-----------------------------------------------------------------------------
// End of file: LIB_SpriteFont.c
//-----------------------------------------------------------------------------
