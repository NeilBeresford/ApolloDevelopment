;** ---------------------------------------------------------------------------
;	@file		HWScreen.s
;	@defgroup 	MainShell Apollo V4 Shell
;	@brief		Apollo V4 development - Shell
;	@date		2020-06-01
;	@version	0.1
;	@copyright	Neil Beresford 2024	
;-----------------------------------------------------------------------------
;	Notes
;
;--------------------------------------------------------------------------- */

    include "Defines.i"
    include "Macros.i"

;-----------------------------------------------------------------------------
; External Function Defs
;-----------------------------------------------------------------------------

	XDEF _HWSCREEN_ClearScreen
	XDEF _HWSCREEN_SetImagePalette
	XDEF _HWSCREEN_DisplayImage
	XREF screenPtr

;-----------------------------------------------------------------------------
; Defines
;-----------------------------------------------------------------------------

;** ---------------------------------------------------------------------------
;	@brief 		Clears the screen
;	@ingroup 	MainShell
;	@return 	none
; --------------------------------------------------------------------------- */
_HWSCREEN_ClearScreen

	movem.l d1-d2/a0,-(sp)

	move.l	screenPtr,a0
	move.l	#0,d1
	move.l	#(SCREENSIZE/4)-1,d2
.clear	
	move.l 	d1,(a0)+
	dbf 	d2,.clear

	movem.l (sp)+,d1-d2/a0
	rts

;** ---------------------------------------------------------------------------
;	@brief 		Sets the palette from the stored image.
;	@ingroup 	MainShell
;	@return 	none
; --------------------------------------------------------------------------- */
_HWSCREEN_SetImagePalette:

	movem.l d0/a0,-(sp)

	lea		ScreenPalette,a0
	move.l  (a0)+,d0
	subq.l  #1,d0

.loop:	

	move.l  (a0)+,$dff388
	dbf 	d0,.loop

	movem.l (sp)+,d0/a0
	rts

;** ---------------------------------------------------------------------------
;	@brief 		Displays the image stored.
;	@ingroup 	MainShell
;	@return 	none
; --------------------------------------------------------------------------- */
_HWSCREEN_DisplayImage:

	movem.l d0/a0-a1,-(sp)

	lea		ScreenBuffer,a0
	move.l  screenPtr,a1
	move.l  #(SCREENSIZE/4)-1,d0

.loop:

	move.l  (a0)+,(a1)+
	dbf 	d0,.loop

	movem.l (sp)+,d0/a0-a1
	rts

;------------------------------------------------------------------------------
; Data
;------------------------------------------------------------------------------

	SECTION hwscreenD,DATA_F

ScreenBuffer:

	INCBIN "/home/neil/Development/ApolloCrossDev/Projects/ApolloShell/Graphics/NeilImage.png.RAW"

ScreenBufferEnd:

ScreenPalette:

	INCBIN "/home/neil/Development/ApolloCrossDev/Projects/ApolloShell/Graphics/NeilImage.png.RAW.pal"

ScreenPaletteEnd:


;------------------------------------------------------------------------------
; End of File: HWScreen.s
;------------------------------------------------------------------------------
