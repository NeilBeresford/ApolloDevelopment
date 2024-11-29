;APS00001A5800001A5800001A5800001A5800001A5800001A5800001A5800001A5800001A5800001A58
;--T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T
;-----------------------------------------------------------------------------
;	Project		Apollo V4 development - testGame
;	File		FontModule.s
;	Coder		Neil Beresford
;	Build envrn	ASMPro1.20b
;-----------------------------------------------------------------------------
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
; Notes -
;
; This module has test code, so it can be build and run, allowing debugging
; of core functionality
; If the define MAIN_BUILD has not been defined, it builds in the
; FontModule_Test which will get called. Plus a temp screen buffer has been
; created, allowing the drawers to draw.
;
;-----------------------------------------------------------------------------

    SECTION font,CODE_F

;-----------------------------------------------------------------------------
; Code
;-----------------------------------------------------------------------------

;	IFND MAIN_BUILD

;-----------------------------------------------------------------------------
; Defines
;-----------------------------------------------------------------------------

;SCREENWIDTH	= 320
;SCREENHEIGHT	= 256
;SCREENSIZE		= SCREENWIDTH*SCREENHEIGHT

FMDEFAUTOCOL	    = $99
FMPRINTCF           = $10
FMCOMMANDCOLCHANGE  = $95

;-----------------------------------------------------------------------------
; Functionaly 
;-----------------------------------------------------------------------------

;----------------------------------------------------------
; FontModule_DisplayString
; Displays the number on the screen
; Regs:
;	d0	- X position
;	d1	- Y position
;	a0	- zero terminated byte string 
;----------------------------------------------------------
FontModule_DisplayString:

    movem.l d0-a6,-(SP)
    move.l d0,d3
    clr.l  d2
    move.b #FMDEFAUTOCOL,fontCol 
.print:
    move.b (a0)+,d2
    beq    .endPrint

	; check for carrage return

    cmp.b  #FMPRINTCF,d2
    bne.s  .checkMisc
    move.l d3,d0
    add    #8,d1
    bra.s  .print
	
.checkMisc:

    cmp.b  #FMCOMMANDCOLCHANGE,d2
    bne.s  .continue
    move.b (a0)+,fontCol
    bra.s  .print
	
.continue:

    bsr    FontModule_DisplayChar
    add.w  #8,d0
    bra.s  .print 

.endPrint:

    movem.l (SP)+,d0-a6
    rts


;----------------------------------------------------------
; DisplayChar
; Displays the number on the screen
; Regs:
;	d0	- X position
;	d1	- Y position
;	d2	- character to print 0-127 
;----------------------------------------------------------
FontModule_DisplayChar:

    movem.l d0-a6,-(SP)

    move.l screenPtr,a0
    lea    fontData,a1

FontModule_PrintChar:

    mulu   #9,d2
    add.l  d2,a1
    mulu   #SCREENWIDTH,d1
    add.l  d0,d1	
    add.l  d1,a0		

    moveq  #8-1,d2
.charY:
    moveq  #0,d1
    move.b (a1)+,d6
.charX:
    btst   d1,d6
    beq.b  .charCont
    move.b fontCol,(a0)
.charCont:
    addq   #1,a0				
    addq   #1,d1
    cmp.l  #8,d1
    bne.s  .charX

    add    #SCREENWIDTH-8,a0
    dbf    d2,.charY

    movem.l (SP)+,d0-a6
    rts

;----------------------------------------------------------
; DisplayHex
; Displays the number on the screen
; Regs:
;	d0	- X position
;	d1	- Y position
;	d2	- d2 - 8 character hex value 
;----------------------------------------------------------
FontModule_DisplayHex:

    movem.l d0-d4/a0,-(SP)

    moveq  #7,d3
    add.l  #56,d0
    move.l d2,d4
    lea    fontHex,a0
.loop:
    move.l d4,d2
    and.l  #$f,d2
    move.b (a0,d2),d2
    bsr    FontModule_DisplayChar
    sub.l  #8,d0
    ror.l  #4,d4
    dbra   d3,.loop


    movem.l (SP)+,d0-d4/a0
    rts



;-----------------------------------------------------------------------------
; Data
;-----------------------------------------------------------------------------

    SECTION data,DATA_F

fontCol	dc.b	$99		; Store for the font colour
    dc.b   0,0,0	
fontHex	dc.b	"0123456789ABCDEF"


fontStrBuffer:

    dcb.b  256                                                            ; large buffer	

fontData:

    IFD    MAIN_BUILD

    include "Source/font8x8_basic.i"

    ELSE

    include "font8x8_basic.i"

screenPtr	dc.l	0

TestString:

    dc.b   "123456",$99,$95," Neil Beresford",10,"RULES! ?/@!$%^&*()_+-",0

    EVEN


    ENDIF

    EVEN

;-----------------------------------------------------------------------------
; Test Data
;-----------------------------------------------------------------------------

    IFND   MAIN_BUILD

    SECTION datascreen,BSS_F

screen		ds.b	SCREENSIZE

    ENDIF


;-----------------------------------------------------------------------------
; End of file: fontControl.s
;-----------------------------------------------------------------------------
