;-----------------------------------------------------------------------------
;	Project		Apollo V4 development - Shell
;	File		mainTest.s
;	Coder		Neil Beresford
;	Build envrn	ASMPro V1.20b
;-----------------------------------------------------------------------------
;	Notes
;
;-----------------------------------------------------------------------------

;-----------------------------------------------------------------------------

	SECTION	code,CODE_F

;-----------------------------------------------------------------------------

;-----------------------------------------------------------------------------
; Code
;-----------------------------------------------------------------------------

;----------------------------------------------------------
; Main
; Entry point, jumps to the start of the main application
; skipping the included source.
;----------------------------------------------------------
Main:

	jmp		Start

;-----------------------------------------------------------------------------
; Includes
;-----------------------------------------------------------------------------

	incdir  "/home/neil/Development/ApolloCrossDev/Projects/ApolloShell/" 
	include "Source/Defines.i"		; Defines for Application
	include "Source/Macros.i"		; Include - macros
	include "Source/Hardware.s"		; Hardware related functionality
	include "Source/Screen.s"		; Screen handling functionality
	include "Source/FontModule.s"	; Font display module
	include "Source/UtilsModule.s"	; General Utils module
	include "Source/TestModule.s"	; Test functionality, used in development
	
	EVEN

;----------------------------------------------------------
; Start
; Starting point, initialise and process the main loop
;----------------------------------------------------------
Start:

	movem.l	d1-a6,-(SP)

	; System Init
	jsr		Hardware_InitSystem
	FAILTO	.end
	
	bsr		SetPalette
	jsr		Hardware_FlipScreen

.loop:

	bsr		WaitVBL

	jsr		Screen_Clear

	move.l	#10,d0
	move.l	#20,d1
	lea		TestString,a0
	jsr		FontModule_DisplayString
			
	jsr		Hardware_FlipScreen
	jsr		Hardware_ReadKey
	cmp.b	#$45,d0
	bne		.loop

.end:

	; Restore System

	jsr		Hardware_RestoreSystem

.fail:

	movem.l	(SP)+,d1-a6
	rts

;----------------------------------------------------------
; WaitVBLu
; Waits until the vertical blank to be triggered
;----------------------------------------------------------
WaitVBL:

	btst	#5,$DFF01F		; Wait VBL
	beq.b	WaitVBL
	move.w	#$0020,$DFF09C	; clr VBL
	rts



;----------------------------------------------------------
; SetPalette
; Sets 64 32bit colours $dff388 ChunkColReg
;----------------------------------------------------------
SetPalette:

	movem.l	d0/a0,-(SP)
	
	lea	paletteData,A0
	move.l	(a0)+,d0
	sub.l	#1,d0

.palset:
	
	move.l	(A0)+,$DFF388
	dbra	D0,.palset

	movem.l	(SP)+,d0/a0

	rts


;----------------------------------------------------------
; KeyCheckRelease
; Checks for keypress, then awaits its release
; Regs d1 key to check	
;----------------------------------------------------------
KeyCheckRelease:


	jsr		Hardware_ReadKey
	cmp.b	d1,d0
	bne.s	.end

.loop:

	jsr		Hardware_ReadKey
	cmp.b	d1,d0
	beq.s	.loop

	move.b	d1,d0

.end:
	rts



;----------------------------------------------------------
; DrawImage
; copys image data to screen
; Regs:
;	a1 - Points to the image to copy
;----------------------------------------------------------
DrawImage:

	movem.l	d0/a0-a1,-(SP)

	move.l	screenPtr,A0
	move.l	#(SCREENSIZE/4)-1,d0
.background:
	move.l	(a1)+,(a0)+
	dbf	d0,.background
			
	movem.l	(SP)+,d0/a0-a1
	rts
	
	
;-----------------------------------------------------------------------------
; Data
;-----------------------------------------------------------------------------

	SECTION data,DATA_F

paletteData:

	; details - NUMBER OF ENTRIES
	;         - INDEX,RED,GREEN,BLUE

	incbin "Graphics/palette.pal"
	
PaletteDataE:

TestString:

	dc.b	"APOLLOSHELL VERTSION 0.0.1",10,"TEST MODE",0

;-----------------------------------------------------------------------------
; End of file: mainTest.s
;-----------------------------------------------------------------------------
