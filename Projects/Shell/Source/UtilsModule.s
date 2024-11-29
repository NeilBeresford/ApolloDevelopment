;APS026BD5CE026BD5CE026BD5CE026BD5CE026BD5CE026BD5CE026BD5CE026BD5CE026BD5CE026BD5CE
;--T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T
;-----------------------------------------------------------------------------
;	Project		Apollo V4 development - Shell
;	File		UtilsModule.c
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
;		UtilsModule_ClearMem - Clears memory at A0, length D0
;-----------------------------------------------------------------------------

;-----------------------------------------------------------------------------
; Defines
;-----------------------------------------------------------------------------

MASKFIRSTTWOBITS = %00000000000000000000000000000011 ; Mask for the first two bits

;-----------------------------------------------------------------------------
; Code
;-----------------------------------------------------------------------------

    SECTION utils,CODE_F	

;-----------------------------------------------------------------------------
; Memory handling
;-----------------------------------------------------------------------------

;----------------------------------------------------------
; UtilsModule_ClearMem
; Clears the memory at A0, length D0 doing this first by
; 32bits then 8bits
; Regs -
;	D0 - total memory
;   A0 - pointer to the memory to be cleared
; Trashed -
;   None
;----------------------------------------------------------
UtilsModule_ClearMem:

    movem.l d0-d2/a0,-(SP)

    clr.l  d2                  ; Clear the ints first...
    move.l d0,d1

    asr.l  #2,d0
    beq.s  .continue

.longs:							; copy the longs across

    move.l d2,(a0)+
    dbpl   d0,.longs	

.continue:

    and.l  #MASKFIRSTTWOBITS,d1 ; now the old bytes at the end
    beq.s  .end

.bytes:	

    move.b d2,(A0)+
    dbpl   d1,.bytes	

.end:

    movem.l (SP)+,d0-d2/a0
    rts

;-----------------------------------------------------------------------------
; Data
;-----------------------------------------------------------------------------

;-----------------------------------------------------------------------------
; End of file: UtilModule.s
;-----------------------------------------------------------------------------

