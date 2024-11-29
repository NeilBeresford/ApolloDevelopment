;APS026BCD05026BCD05026BCD05026BCD05026BCD05026BCD05026BCD05026BCD05026BCD05026BCD05
;--T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T
;-----------------------------------------------------------------------------
;	Project		Apollo V4 development - Shell
;	File		Screen.s
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
;
;-----------------------------------------------------------------------------

;-----------------------------------------------------------------------------
; Code
;-----------------------------------------------------------------------------

    SECTION screen,CODE_C

;-----------------------------------------------------------------------------

;----------------------------------------------------------
; Screen_Clear
; Clears the screen, currently fixed to a light green
;----------------------------------------------------------
Screen_Clear:

    movem.l d0-d1/a0,-(SP)

    move.l #SCREENCLEARCOLOUR,d1
    move.l #((SCREENWIDTH*SCREENHEIGHT)/4)-1,d0
    move.l screenPtr,a0

.loop:	

    move.l d1,(a0)+
    dbra   d0,.loop

    movem.l (SP)+,d0-d1/a0
    rts

;----------------------------------------------------------
; Screen_Gradient
; Vertical gradient fill going light to dark
;----------------------------------------------------------
Screen_Gradient:

    movem.l d0-d2/a0,-(SP)

    move.l screenPtr,a0
    moveq  #0,d2

    move.l #SCREENHEIGHT-1,d1

.lines:
	
    move.l #(SCREENWIDTH/4)-1,d0

.loop:	

    move.l d2,(a0)+
    dbra   d0,.loop

    add.l  #$01010101,d2
    dbra   d1,.lines

    movem.l (SP)+,d0-d2/a0
    rts



;----------------------------------------------------------
; Screen_Test
; displays the full 256 palette in pixels, looping
;----------------------------------------------------------
Screen_Test:

    movem.l d0-d1/a0,-(SP)

    move.l screenPtr,a0
    move.l #SCREENSIZE,d0
    moveq  #0,d1

.loop:

    move.b d1,(a0)+
    add.b  #1,d1
    dbpl  d0,.loop	

    movem.l (SP)+,d0-d1/a0
    rts


;-----------------------------------------------------------------------------
; End of file: Screen.s
;-----------------------------------------------------------------------------

