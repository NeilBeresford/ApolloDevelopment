;** ---------------------------------------------------------------------------
;	@file		Hardware.s
;	@defgroup 	AmiWorms Apollo V4 Shell
;	@brief		Apollo V4 development - Shell
;	@date		2020-06-01
;	@version	0.1
;	@copyright	Neil Beresford 2024	
;-----------------------------------------------------------------------------
;	Notes
; This module has test code, so it can be build and run, allowing debugging
; of core functionality
; If the define MAIN_BUILD has not been defined, it builds in the
; FontModule_Test which will get called. Plus a temp screen buffer has been
; created, allowing the drawers to draw.
;
;--------------------------------------------------------------------------- */

;-----------------------------------------------------------------------------
; Includes
;-----------------------------------------------------------------------------

    include "Defines.i"
    include "Macros.i"

;-----------------------------------------------------------------------------
; External defines
;-----------------------------------------------------------------------------

	XDEF _FontModule_DisplayString
	XDEF _FontModule_DisplayChar
	XDEF _FontModule_DisplayHex
	XREF screenPtr

;-----------------------------------------------------------------------------
; Functionality
;-----------------------------------------------------------------------------

;----------------------------------------------------------
; FontModule_DisplayString
; Displays the number on the screen
; Regs:
;	d0	- X position
;	d1	- Y position
;	a0	- zero terminated byte string 
;----------------------------------------------------------
_FontModule_DisplayString:

	movem.l	d0-a6,-(SP)

	move.l  #10,d0
	move.l  #80,d1
	lea		TestString,a0

	move.l	d0,d3
	clr.l	d2
	move.b	#$10,fontCol 
.print:
	move.b	(a0)+,d2
	beq	.endPrint

	; check for carrage returnzxxxcv

	cmp.b	#10,d2
	bne.s	.checkMisc
	move.l	d3,d0
	add	#8,d1
	bra.s	.print
	
.checkMisc:

	cmp.b   #$99,d2
	bne.s   .continue
	move.b	(a0)+,fontCol
	bra.s	.print
	
.continue:

	bsr	_FontModule_DisplayChar
	add.w	#8,d0
	bra.s	.print 

.endPrint:

	movem.l	(SP)+,d0-a6
	rts


;----------------------------------------------------------
; DisplayChar
; Displays the number on the screen
; Regs:
;	d0	- X position
;	d1	- Y position
;	d2	- character to print 0-127 
;----------------------------------------------------------
_FontModule_DisplayChar:

	movem.l	d0-a6,-(SP)

	move.l	screenPtr,a0
	lea	fontData,a1

FontModule_PrintChar:

	mulu	#9,d2
	add.l	d2,a1
	mulu	#SCREENWIDTH,d1
	add.l	d0,d1	
	add.l	d1,a0		

	moveq	#8-1,d2
.charY:
	moveq	#0,d1
	move.b	(a1)+,d6
.charX:
	btst	d1,d6
	beq.b	.charCont
	move.b	fontCol,(a0)
.charCont:
	addq	#1,a0				
	addq	#1,d1
	cmp.l	#8,d1
	bne.s	.charX

	add	#SCREENWIDTH-8,a0
	dbf	d2,.charY

	movem.l	(SP)+,d0-a6
	rts

;----------------------------------------------------------
; DisplayHex
; Displays the number on the screen
; Regs:
;	d0	- X position
;	d1	- Y position
;	d2	- d2 - 8 character hex value 
;----------------------------------------------------------
_FontModule_DisplayHex:

	movem.l	d0-d4/a0,-(SP)

	moveq	#7,d3
	add.l	#56,d0
	move.l	d2,d4
	lea	fontHex,a0
.loop:
	move.l	d4,d2
	and.l	#$f,d2
	move.b	(a0,d2),d2
	bsr	_FontModule_DisplayChar
	sub.l	#8,d0
	ror.l	#4,d4
	dbra	d3,.loop


	movem.l (SP)+,d0-d4/a0
	rts



;-----------------------------------------------------------------------------
; Data
;-----------------------------------------------------------------------------

	SECTION fdata,DATA_F

fontCol	dc.b	$99		; Store for the font colour
	dc.b	0,0,0	
fontHex	dc.b	"0123456789ABCDEF"


fontStrBuffer:

	dcb.b	256		; large buffer	

fontData:

	include "font8x8_basic.i"

    EVEN

TestString:

	dc.b	"123456",$99,$11," Neil Beresford",10,"RULES! ?/@!$%^&*()_+-",0

	EVEN

;-----------------------------------------------------------------------------
; Test Data
;-----------------------------------------------------------------------------


;-----------------------------------------------------------------------------
; End of file: fontControl.s
;-----------------------------------------------------------------------------
