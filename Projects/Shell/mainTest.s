;APS00000AE700000AE700000AE700000AE700000AE700000AE700000AE700000AE700000AE700000AE7
;--T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T
;-----------------------------------------------------------------------------
;	Project		Apollo V4 development - Shell
;	File		Hardware.s
;	Coder		Neil Beresford
;	Build envrn	ASMPro V1.20b, Visual Code
;------------------------------------------------------------------------------
;  MIT License
;
; Copyright (c) 2024 Neil Beresford
;
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in all
; copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
; SOFTWARE
;
;---------------------------------------------------------------------------
;	Notes
;   Main entry to test area - for the shell.
;-----------------------------------------------------------------------------

;-----------------------------------------------------------------------------

    SECTION code,CODE_F

;-----------------------------------------------------------------------------

;-----------------------------------------------------------------------------
; Defines
;-----------------------------------------------------------------------------

MAIN_BUILD = 1                                                             ; Used to remove the test code for the modules
TEST_BUILD = 1                                                             ; Allows test to be processed.

;-----------------------------------------------------------------------------
; Code
;-----------------------------------------------------------------------------

;----------------------------------------------------------
; MainEntry
; Entry point, jumps to the start of the main application
; skipping the included source.
;----------------------------------------------------------
MainEntry:

    jmp    Start

;-----------------------------------------------------------------------------
; Includes
;-----------------------------------------------------------------------------

    include "Source/Defines.i"                                             ; Defines for Application
    include "Source/Macros.i"                                              ; Include - macros
    include "Source/Hardware.s"                                            ; Hardware related functionality
    include "Source/Screen.s"                                              ; Screen handling functionality
    include "Source/FontModule.s"                                          ; Font display module
    include "Source/UtilsModule.s"                                         ; General Utils module
    include "Source/TestModule.s"                                          ; Test functionality, used in development
	
    EVEN

;----------------------------------------------------------
; Start
; Starting point, initialise and process the main loop
;----------------------------------------------------------
Start:
		

    movem.l d1-a6,-(SP)

	; System Init

    jsr    Hardware_InitSystem
    FAILTO .end
	
    jsr    Hardware_SetPalette                                            ; or use Hardware_SetGreyScalePalette 
    jsr    Hardware_FlipScreen

.loop:

    jsr    Hardware_WaitVBL
    lea    TestPic,a1
    bsr    DrawImage

    IF     TEST_BUILD=1
	; Testing ----------

    move.l #10,d0                                                         ; X pos
    move.l #20,d1                                                         ; Y pos
    lea    TestString,a0                                                  ; String to display
    jsr    FontModule_DisplayString


    move.l #10,d0                                                         ; X pos
    move.l #2,d1                                                          ; Y pos
    move.l #$abcdef90,d2                                                  ; 32 bit value to be displayed as hex
    jsr    FontModule_DisplayHex

	; Testing ends -----
    ENDIF
			
    jsr    Hardware_FlipScreen
    jsr    Hardware_ReadKey
    cmp.b  #$45,d0
    bne    .loop

.end:

	; Restore System

    jsr    Hardware_RestoreSystem

.fail:

    movem.l (SP)+,d1-a6
    moveq  #0,d0
    rts


;----------------------------------------------------------
; KeyCheckRelease
; Checks for keypress, then awaits its release
; Regs d1 key to check	
;----------------------------------------------------------
KeyCheckRelease:
	
    jsr    Hardware_ReadKey
    cmp.b  d1,d0
    bne.s  .end

.loop:

    jsr    Hardware_ReadKey
    cmp.b  d1,d0
    beq.s  .loop

    move.b d1,d0

.end:
    rts

;----------------------------------------------------------
; DrawImage
; copys image data to screen
; Regs:
;	a1 - Points to the image to copy
;----------------------------------------------------------
DrawImage:

    movem.l d0/a0-a1,-(SP)

    move.l screenPtr,A0
    move.l #(SCREENSIZE/4)-1,d0

.background:

    move.l (a1)+,(a0)+
    dbf    d0,.background
			
    movem.l (SP)+,d0/a0-a1
    rts
	
	
;-----------------------------------------------------------------------------
; Data
;-----------------------------------------------------------------------------

    SECTION data,DATA_F

TestString:

    dc.b   "123456",$99,$02," Neil Beresford",10,"RULES! ?/@!$%^&*()_+-",0

    EVEN
	
TestPic:

    INCBIN "Graphics/NeilImage.png.RAW"

TestPic_End:

    EVEN

