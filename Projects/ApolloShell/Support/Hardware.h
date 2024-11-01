/** ---------------------------------------------------------------------------
	@file		Hardware.h
	@defgroup 	MainShell Apollo V4 Shell
	@brief		Hardware Support for the Apollo system
    @date		2024-10-31
	@version	0.1
	@copyright	Neil Beresford 2024	
 -----------------------------------------------------------------------------
	Notes

--------------------------------------------------------------------------- */

#ifndef _HARDWARE_H_
#define _HARDWARE_H_

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

extern void Hardware_Init( void );
extern void Hardware_Close( void );
extern void Hardware_WaitVBL( void );
extern void Hardware_FlipScreen( void );
extern void Hardware_ClearScreen( void );
extern void Hardware_TestScreen( void );
extern int 	Hardware_ReadKey( void );
extern int 	Hardware_GetScreenPtr( void );

//-----------------------------------------------------------------------------

#endif // _HARDWARE_H_

//-----------------------------------------------------------------------------
// End of File: Hardware.h
//-----------------------------------------------------------------------------