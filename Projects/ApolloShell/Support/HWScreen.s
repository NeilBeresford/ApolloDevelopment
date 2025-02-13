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

	XDEF _HWSCREEN_GetScreenBuffer
	XDEF _HWSCREEN_ClearScreen
	XDEF _HWSCREEN_SetImagePalette
	XDEF _HWSCREEN_DisplayImage
	XDEF _HWSCREEN_SetSingleCol
	XREF screenPtr

;-----------------------------------------------------------------------------
; Defines
;-----------------------------------------------------------------------------

;** ---------------------------------------------------------------------------
; 	@brief 		Returns the screen buffer
;	@ingroup 	MainShell
;	@return 	D0 - ScreenBuffer - The screen buffer
; --------------------------------------------------------------------------- */
_HWSCREEN_GetScreenBuffer

	move.l	screenPtr,d0
	rts

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
;	@param [IN]	A0 - PaletteBuffer - The image palette colours
;	@return 	none
; --------------------------------------------------------------------------- */
_HWSCREEN_SetImagePalette:

	movem.l d0/a0,-(sp)

	cmp.l   #0,a0
	beq     .exit

	move.l   #255,d0
.loop:	
	move.l  (a0)+,$dff388
	dbf 	d0,.loop

.exit:

	movem.l (sp)+,d0/a0
	rts

;** ---------------------------------------------------------------------------
;	@brief 		Sets the single colour. Format 0xIIRRGGBB
;	@ingroup 	MainShell
;	@param [IN]	D0 - Colour with Index/Red/Green/Blue
;	@return 	none
; --------------------------------------------------------------------------- */
_HWSCREEN_SetSingleCol:

	move.l  d0,$dff388

	rts

;** ---------------------------------------------------------------------------
;	@brief 		Displays the image stored.
;	@ingroup 	MainShell
;	@param [IN]	A0 - ScreenBuffer - The image to display
;	@return 	none
; --------------------------------------------------------------------------- */
_HWSCREEN_DisplayImage:

	movem.l d0/a0-a1,-(sp)

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

ScreenBuffer:	dc.l 0

;------------------------------------------------------------------------------
; End of File: HWScreen.s
;------------------------------------------------------------------------------
