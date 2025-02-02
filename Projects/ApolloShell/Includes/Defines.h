/** ---------------------------------------------------------------------------
	@file		Defines.h
	@defgroup 	MainShell Apollo V4 Shell
	@brief		Display of sprite based fonts
	@date		2020-06-01
	@version	0.1
	@copyright	Neil Beresford 2024
 -----------------------------------------------------------------------------
	Notes

    This is the C friendly version of Defines.i

--------------------------------------------------------------------------- */

#ifndef _DEFINES_H_
#define _DEFINES_H_

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define SCREENWIDTH 		= ( 640 )							// Screen width
#define SCREENHEIGHT 		= ( 480	)						    // Screen height
#define SCREENSIZE 		    = ( SCREENWIDTH*SCREENHEIGHT )		// Size of screen in pixels
#define TOTALSCREENSSIZE    = ( SCREENSIZE * 3 + 64 )           // Total bytes needed for the screens
#define SCREENCOLORDEPTH 	= ( 8 )								// 256 colours
#define SCREENMODE 		    = ( $0501 )							// Set to 640x480 mode
#define SCREENCLEARCOLOUR	= ( $98989898 )						// Set to a light green

#define BACKSCREENWIDTH     = ( SCREENWIDTH*3                   // Back screen width (640*4)
#define BACKSCREENHEIGHT    = ( 900 )                           // Back screen height

//-----------------------------------------------------------------------------

#endif // _DEFINES_H_

//-----------------------------------------------------------------------------
// End of file: Defines.h
//-----------------------------------------------------------------------------
