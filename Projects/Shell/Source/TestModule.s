;APS026BC4DA026BC4DA026BC4DA026BC4DA026BC4DA026BC4DA026BC4DA026BC4DA026BC4DA026BC4DA
;--T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T
;-----------------------------------------------------------------------------
;	Project		Apollo V4 development - Shell
;	File		TestModule.s
;	Coder		Neil Beresford
;	Build envrn	ASMPro V1.20b, Visual Code
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
;	Notes
;	Functions -
;		TestModule_FillScreen - Fills the screen with full colour palette
;-----------------------------------------------------------------------------

;-----------------------------------------------------------------------------
; Code
;-----------------------------------------------------------------------------

    SECTION test,CODE_C

;-----------------------------------------------------------------------------

;----------------------------------------------------------
; TestModule_FillScreen
; writes the palette to the screen, will show full screen
; fill. Basically a test function, incorrerctly fills
; the screen as it wraps around after adding 4 colours
; to the index ( indexes are byte sized )	
;----------------------------------------------------------
TestModule_FillScreen:

    movem.l d0-d1/a0,-(SP)

    move.l #$00010203,d1
    move.l #((SCREENWIDTH*SCREENHEIGHT)/4)-1,d0
    move.l screenPtr,a0

.loop:	

    move.l d1,(a0)+
    addi.l #$04040404,d1
    dbra   d0,.loop

    movem.l (SP)+,d0-d1/a0
    rts

;-----------------------------------------------------------------------------
; End of file: TestModule.s
;-----------------------------------------------------------------------------
