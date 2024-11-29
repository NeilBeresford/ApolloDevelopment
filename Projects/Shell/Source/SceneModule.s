;APS026BCA3E026BCA3E026BCA3E026BCA3E026BCA3E026BCA3E026BCA3E026BCA3E026BCA3E026BCA3E
;--T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T
;-----------------------------------------------------------------------------
;	Project		Apollo V4 development - Shell
;	File		SceneModule.s
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

    SECTION scene,CODE_C

;-----------------------------------------------------------------------------
; Defines - Internal to SceneModule
;-----------------------------------------------------------------------------

;-----------------------------------------------------------------------------


;----------------------------------------------------------
; SceneModule_Init
; Initializes the scene module controller
;----------------------------------------------------------
SceneModule_Init:

    movem.l d0-d1/a0,-(SP)

    movem.l (SP)+,d0-d1/a0
    rts

;----------------------------------------------------------
; SceneModule_Process
; Processes the scene module controller
;----------------------------------------------------------
SceneModule_Process:

    movem.l d0-d1/a0,-(SP)

    movem.l (SP)+,d0-d1/a0
    rts

;----------------------------------------------------------
; SceneModule_Exit
; Tidy exit for the scene module controller
;----------------------------------------------------------
SceneModule_Exit:

    movem.l d0-d1/a0,-(SP)

    movem.l (SP)+,d0-d1/a0
    rts

;-----------------------------------------------------------------------------
; Data
;-----------------------------------------------------------------------------







;-----------------------------------------------------------------------------
; End of file: SceneModule.s
;-----------------------------------------------------------------------------
