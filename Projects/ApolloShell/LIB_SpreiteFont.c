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

#include "stdint.h"
#include "stdbool.h"
#include "Includes/LIB_Sprites.h"
#include "Includes/ResourceFiles.h"
#include "Includes/LIB_SpriteFont.h"

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------


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
    '"','"', 84,   // extended characters
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


//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

void LIB_SpriteFont_Draw( uint32_t ulSpriteIndex,  uint32_t ulX, uint32_t ulY, uint8_t* pszText )
{
    uint32_t ulXPos = ulX;
    uint32_t ulYPos = ulY;

    ulSpriteIndex += ResourceHandling_GetGroupStartResource( eGroups_Font );

    for( uint32_t nChar = 0; nChar < strlen( pszText ); nChar++ )
    {
        uint8_t cChar = pszText[ nChar ];
        uint32_t ulSpriteConvIndex = 0;

        if ( cChar == 0 )
        {
            break;
        }
        
        while( CharConvert[ ulSpriteConvIndex ] != 0 )
        {
            if ( cChar >= CharConvert[ ulSpriteConvIndex ] && cChar <= CharConvert[ ulSpriteConvIndex + 1 ] )
            {
                cChar -= CharConvert[ ulSpriteConvIndex ];
                cChar += CharConvert[ ulSpriteConvIndex + 2 ];
                break;
            }
            ulSpriteConvIndex += 3;
            if ( CharConvert[ ulSpriteConvIndex ] == 0 )
            {
                cChar = 0xFF;
            }
        }
        if ( cChar != 0xFF )
        {
            LIB_Sprites_Draw( ulSpriteIndex, cChar, ulXPos, ulYPos );
        }
        ulXPos += 7;
    }
}





//-----------------------------------------------------------------------------
// End of file: LIB_SpriteFont.c
//-----------------------------------------------------------------------------
