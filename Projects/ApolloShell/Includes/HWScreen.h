/** ---------------------------------------------------------------------------
	@file		HWScreen.h
	@defgroup 	MainShell Apollo V4 Shell
	@brief		Hardware Support for the Apollo system
    @date		2024-10-31
	@version	0.1
	@copyright	Neil Beresford 2024	
 -----------------------------------------------------------------------------
	Notes

--------------------------------------------------------------------------- */

#ifndef _HWSCREEN_H_
#define _HWSCREEN_H_

//-----------------------------------------------------------------------------

#include "stdint.h"
#include "stdbool.h"
#include "Hardware.h"

//-----------------------------------------------------------------------------
// External Data
//-----------------------------------------------------------------------------

extern uint8_t* ScreenBuffer;

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

extern _REG uint8_t* HWSCREEN_GetScreenBuffer( void );
extern _REG void HWSCREEN_ClearScreen( void );
extern _REG void HWSCREEN_SetImagePalette( _A0(uint32_t* palette) );
extern _REG void HWSCREEN_DisplayImage( _A0(uint8_t* screen) );

//-----------------------------------------------------------------------------

#endif // _HWSCREEN_H_

//-----------------------------------------------------------------------------
// End of File: HWScreen.h
//-----------------------------------------------------------------------------