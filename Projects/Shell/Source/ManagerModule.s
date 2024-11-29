;APS026BDA63026BDA63026BDA63026BDA63026BDA63026BDA63026BDA63026BDA63026BDA63026BDA63
;--T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T
;-----------------------------------------------------------------------------
;	Project		Apollo V4 development - Shell
;	File		ManagerModule.s
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
;   This module handles the game processing and mode transitions 
;-----------------------------------------------------------------------------

MMCTRL_NUMBERMODES     = 0
MMCTRL_ACTIVEMODEINDEX = 1
MMCTRL_RUNNING         = 2
MMCTRL_TRANSITION      = 3
	
MMMODE_ID              = 0
MMMODE_TMP1            = 1
MMMODE_TMP2            = 2
MMMODE_TMP3            = 3
MMMODE_FNINIT          = 4
MMMODE_FNPROCESS       = 8
MMMODE_FNEXIT          = 12
MMMODE_SIZE            = 16

MM_TOTALMODES          = 10

;-----------------------------------------------------------------------------

;----------------------------------------------------------
; ManagerModule_Init
; Initialise the manager module
;----------------------------------------------------------
ManagerModule_Init:

    movem.l d0-d1/a0,-(SP)

    clr.l  d0
    
    ; clear the control struct
    move.l d0,MMCtrl

    ; clear the mode store
    lea    MMModeStore,a0
    move.l #MM_TOTALMODES*MMMODE_SIZE,d1

.clearModeStore:

    move.b d0,(a0)+ 
    dbf    d1,.clearModeStore

    movem.l (SP)+,d0-d1/a0
    rts


;----------------------------------------------------------
; ManagerModule_Init
; Process a game loop within the manager module
;----------------------------------------------------------
ManagerModule_Process:

    cpm.l  #0,MMCTRL_NUMBERMODES(MMCtrl)
    beq.s  .notRunning
    cpm.l  #0,MMCTRL_RUNNING(MMCtrl)
    beq.s  .notRunning

    movem.l d0-d1/a0-a2,-(SP)

    ; get the active mode
    lea    MMCtrl,a2
    move.b MMCTRL_ACTIVEMODEINDEX(a2),d0
    moveq  #MMMODE_SIZE,d1
    mulu   d0,d1
    lea    MMModeStore,a1
    add.l  d1,a1



.end:

    movem.l (SP)+,d0-d1/a0-a2

.notRunning:

    rts

;----------------------------------------------------------
; ManagerModule_Init
; Terminates the manager module
;----------------------------------------------------------
ManagerModule_Exit:

    rts


;----------------------------------------------------------
; ManagerModule_SetMode
; Terminates the manager module
;----------------------------------------------------------
ManagerModule_SetMode:
    movem.l d0-d1/a0-a2,-(SP)

    lea    MMCtrl,a2
    cmp.b  #0,MMCTRL_TRANSITION(a2)
    bne.s  .end
    cmp.b  #0,MMCTRL_RUNNING(a2)
    beq.s .setMode

.setMode:

; D0 = mode index

    lea    MMCtrl,a2
    move.b d0,MMCTRL_ACTIVEMODEINDEX(a2)
    movem.l (SP)+,d0-d1/a0-a2
    rts


;----------------------------------------------------------
; ManagerModule_RegisiterMode
; If possible, registers mode with the manager module
; REGS:
;	IN  - a0 - points to MMODE struct
;  OUT - d0 - index for registered mode, or -1 
;----------------------------------------------------------
ManagerModule_RegisterMode:

    movem.l d1/a0-a2,-(SP)

    moveq  #-1,d0

    lea    MMCtrl,a2
    lea    MMModeStore,a1

	; check space to register mode
	
    cmp.b  #MM_TOTALMODES,MMCTRL_NUMBERMODES(a2)
    beq    .end

	; check the data to make sure things have been set

    clr.l  d1
	
    cmp.l  #0,MMMODE_FNINIT(a0)
    beq.s  .end
    cmp.l  #0,MMMODE_FNPROCESS(a0)
    beq.s  .end
    cmp.l  #0,MMMODE_FNEXIT(a0)
    beq.s  .end

	; calculate the offset to the mode store

    move.b MMCTRL_NUMBERMODES(a2),d1
    mulu   #MMMODE_SIZE,d1
    add.l  d1,a1

	; initialize

    clr.b  d1
    move.b d1,MMMODE_TMP1(a0)
    move.b d1,MMMODE_TMP2(a0)
    move.b d1,MMMODE_TMP3(a0)

	; store the mode data

    moveq  #(MMMODE_SIZE/4)-1,d1

.store:	

    move.l (a0)+,(a1)+
    dbf    d1,.store

	; finalize the registration
	; reporting success

    addq.b #1,MMCTRL_NUMBERMODES(a2)

    clr.l  d0
    move.b MMCTRL_NUMBERMODES(a2),d0

.end:

    movem.l (SP)+,d1/a0-a2
    rts


;-----------------------------------------------------------------------------
; Data
;-----------------------------------------------------------------------------

    SECTION mmDats,DATA_F

; Manager Module Control Struct

MMCtrl:

    dc.b   0                                    ; 00 Number of registered modes
    dc.b   0                                    ; 01 Current active mode
    dc.b   0                                    ; 02 Module running or paused
    dc.b   0                                    ; 03 Module in transistion state
    dc.l   0                                    ; 04 pointer to the current mode struct


    EVEN


; Store of possible registered modes
MMModeStore:

    ds.b   MM_TOTALMODES*MMMODE_SIZE            ; Store for the registered modes



;-----------------------------------------------------------------------------
; End of file: ManagerModule.s
;-----------------------------------------------------------------------------
