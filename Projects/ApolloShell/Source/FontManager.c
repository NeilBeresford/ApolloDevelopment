/** ---------------------------------------------------------------------------
	@file	    FontManager.c
	@defgroup 	MainShell Apollo V4 Shell
	@brief		Font Control, drawing directly to the screen
    @date		2024-11-02
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
#include "stdlib.h"
#include "Includes/FlagStruct.h"
#include "../Support/Hardware.h"
#include "Includes/FontManager.h"
#include "Includes/font8x8_basic.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define TOTAL_TEXT_DISPLAYED    ( 100 )      //!< Total number of text strings displayed

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


/** ---------------------------------------------------------------------------
    @brief      Font data structure
    @ingroup    MainShell
---------------------------------------------------------------------------- */
typedef struct
{
    uint8_t *pFontData;     //!< Pointer to the font data
    uint8_t  nWidth;        //!< Width of the font
    uint8_t  nHeight;       //!< Height of the font   
    uint8_t  nCharWidth;    //!< Width of the character   
    uint8_t  nCharHeight;   //!< Height of the character 
    uint8_t  nCharSpacing;  //!< Spacing between characters
    uint8_t  nCharOffset;   //!< Offset of the character 
    uint8_t  nCharCount;    //!< Number of characters

} sFontData;                //!< Font data structure

/** ---------------------------------------------------------------------------
    @brief      Text structure
    @ingroup    MainShell
---------------------------------------------------------------------------- */
typedef struct
{
    uint32_t ulFontID;          //!< Font ID
    uint32_t ulXpos;            //!< X position
    uint32_t ulYpos;            //!< Y position 
    uint32_t ulColour;          //!< Colour
    uint8_t  *pText;            //!< Text to display

} sFMText, *pFMText;            //!< Text structure

/** ---------------------------------------------------------------------------
    @brief      Font manager control structure
    @ingroup    MainShell
---------------------------------------------------------------------------- */
typedef struct
{
    FlagStruct_t    Flags;                          //!< Flags

    uint8_t*        pScreenPtr;                     //!< Pointer to the screen

    sFMText         Text[ TOTAL_TEXT_DISPLAYED ];   //!< Text buffer
    uint32_t        ulTextCount;                    //!< Text count


} sFMCtrl, *pFMCtrl;                                //!< Font manager control structure

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

static sFMCtrl sCtrl;                               //!< Font manager control structure

/** ---------------------------------------------------------------------------
    @brief      Font data
    @ingroup    MainShell
---------------------------------------------------------------------------- */
sFontData gFontData[ eFontID_Total ] = 
{
    { font8x8_basic, 8, 8, 8, 8, 0, 128 }   //!< Basic 8x8 font
};


//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------


/** ---------------------------------------------------------------------------
    @brief      Initialize the Font manager
    @ingroup    MainShell
    @return     bool - true if successful
---------------------------------------------------------------------------- */
bool FontManager_Init(void)
{
    bool bSuccess = false;

    if ( sCtrl.Flags.Initialized == 0 )
    {
        sCtrl.Flags.Flags = 0;
        sCtrl.ulTextCount = 0;

        sCtrl.pScreenPtr = Hardware_GetScreenPtr();

        if ( sCtrl.pScreenPtr != NULL )
        {
            // CLear the text buffer
            for ( uint32_t i = 0; i < TOTAL_TEXT_DISPLAYED; i++ )
            {
                sCtrl.Text[ i ].ulFontID    = eFontID_NotSet;
                sCtrl.Text[ i ].ulXpos      = 0;
                sCtrl.Text[ i ].ulYpos      = 0;
                sCtrl.Text[ i ].ulColour    = 0;
                sCtrl.Text[ i ].pText       = NULL;
            }

            // all finished, set the flag and return success
            sCtrl.Flags.Initialized = 1;
            bSuccess                = true;
        }
    }
    return bSuccess;
}

/** ---------------------------------------------------------------------------
    @brief      Initialize the Font manager
    @ingroup    MainShell
    @return     bool - true if successful
---------------------------------------------------------------------------- */
bool FontManager_Close(void)
{
    bool bSuccess = false;


    return bSuccess;
}

/** ---------------------------------------------------------------------------
    @brief      TEST functionality - draw text to the display 
    @ingroup    MainShell
    @param      ulFontID    - Font ID
    @param      ulXpos      - X position
    @param      ulYpos      - Y position
    @param      ulColour    - Colour
    @param      pText       - Text to display
    @return     bool        - true if successful
--------------------------------------------------------------------------- */
bool FontManager_TestPrint( uint32_t x, uint32_t y, eFontID eFont, char *pText )
{
    bool bSuccess = false;

    if ( eFont < eFontID_Total )
    {
        sFontData *pFontData = &gFontData[ eFont ];

        for ( uint32_t i = 0; i < pFontData->nCharCount; i++ )
        {
            for ( uint32_t j = 0; j < pFontData->nCharHeight; j++ )
            {
                for ( uint32_t k = 0; k < pFontData->nCharWidth; k++ )
                {
                    if ( pFontData->pFontData[ i * pFontData->nCharWidth + j ] & ( 1 << k ) )
                    {
                        // Draw the pixel
                        sCtrl.pScreenPtr[ ( y + j ) * 320 + ( x + k ) ] = 0x01;
                    }
                }
            }
        }
    }

    return bSuccess;
}

//-----------------------------------------------------------------------------
// End of File: FontManager.c
//-----------------------------------------------------------------------------