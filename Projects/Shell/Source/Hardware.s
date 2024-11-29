;APS026BCD42026BCD42026BCD42026BCD42026BCD42026BCD42026BCD42026BCD42026BCD42026BCD42
;--T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T
;-----------------------------------------------------------------------------
;	Project		Apollo V4 development - Shell
;	File		Hardware.s
;	Coder		Neil Beresford
;	Build envrn	ASMPro V1.18
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
;		Hardware_InitSystem
;		Hardware_RestoreSystem
;		Hardware_ReadKey
;		Hardware_FlipScreen
;	
;-----------------------------------------------------------------------------

;-----------------------------------------------------------------------------
; Conditional builds
;-----------------------------------------------------------------------------

VAMPIRE            EQU 1                              ; Working on VAMPIRE ?
CHECK_COMPILE      EQU 0                              ; Used to check if this file builds

; Palette data struct offsets

PALETTEDATA_MAX      =  0                              ; Current max colour
PALETTEDATA_MASK     =  4                              ; Mask to apply before or and and
PALETTEDATA_ADD      =  8                              ; Addition to calculate the next colour
PALETTEDATA_OR       =  12                             ; OR to current colour for fixed R,G or B
PALETTEDATA_SIZE     =  16                             ; Size of the Palette data struct


;-----------------------------------------------------------------------------
; Defines
;-----------------------------------------------------------------------------

; For testing build

    IF     CHECK_COMPILE = 1

SCREENWIDTH        EQU 320                            ; Screen width
SCREENHEIGHT       EQU 256                            ; Screen height
SCREENSIZE         EQU SCREENWIDTH*SCREENHEIGHT
SCREENCOLORDEPTH   EQU 8                              ; 256 colours
SCREENMODE         EQU $0301                          ; Set to 320x256 mode


    ENDIF

VAMPIREHWVERSION   EQU $dff3fc
OPENLIBRARY        EQU -$228
CLOSELIBRARY       EQU -414
OPENSCREENTAGLIST  EQU -$264
BESTCMODEIDTAGLIST EQU -60
VPOSR              EQU $dff004
VPOSRCHIPIDMASK    EQU $0f
MINOSVERSION       EQU $39

;-----------------------------------------------------------------------------
; Code
;-----------------------------------------------------------------------------

;-----------------------------------------------------------------------------
;	Function		Hardware_InitSystem
;	Brief		Sets up COPPER,display etc and stops interrupts
;	Regs  		d0 is trashed
;	return		d0 status, 0 successful, -1 failed	
;-----------------------------------------------------------------------------
Hardware_InitSystem:
	
    movem.l d1-a6,-(SP)

    move.b VPOSR,D0                                  ; OCS or ECS?
    and.b  VPOSRCHIPIDMASK,D0                        ;
    move.b D0,ECS                                    ;

    IF     VAMPIRE = 1


    move.b VAMPIREHWVERSION,D0                       ; Vampire version
    beq.w  .failedEnd                                ; no vampire?

    move.w #$FFFF,SCREENMASK                         ; V4 oe V2
    move.b #4,V4
    cmp.b  #1,D0
    beq.s  .v2
    cmp.b  #2,D0
    beq.s  .v2
    cmp.b  #6,D0
    bne.s  .enddetection

.v2:

    ENDIF

    move.w #$00FF,SCREENMASK
    clr.b  V4

.enddetection:

    move.l $4.w,A6                                   ; execbase
    lea    intuitionname,A1
    moveq  #MINOSVERSION,D0                         ; Min OS 3
    jsr    OPENLIBRARY(A6)                           ; open library
    tst.l  D0
    beq.w  .failedEnd
    move.l D0,intuitionbase

    lea    cybergfxname,A1
    moveq  #39,D0                                    ; Min OS 3
    jsr    OPENLIBRARY(A6)                           ; open library
    tst.l  D0
    beq.w  .failedEnd
    move.l D0,cybergfxbase

    move.l cybergfxbase,A6
    lea    modetags,A0
    jsr    BESTCMODEIDTAGLIST(A6)
    tst.l  D0
    beq.w  .failedEnd
    move.l D0,screenmodeid

    move.l intuitionbase,A6
    suba.l A0,A0
    lea    screentags,A1
    jsr    OPENSCREENTAGLIST(A6)                     ; Openscreentaglist
    tst.l  D0
    beq.w  .failedEnd
    move.l D0,screen


    move.w $DFF002,DMACONSTORE                       ; SAVE DMACON
    move.w #$7FFF,$DFF096                            ; ALL DMA OFF

    move.w $DFF01C,INTENASTORE                       ; SAVE INTENA
    move.w #$7FFF,$DFF09A                            ; ALL INTENA OFF



    move.l #screens,D0                               ; we use several Double Buffer
    add.l  #31,D0                                    ; this demo does not need this
    and.l  #$FFFFFFE0,D0                             ; with this trick we display over 100 FPS without flicker
    move.l D0,screenPtr
    add.l  #SCREENSIZE,D0
    move.l D0,screenPtr2
    add.l  #SCREENSIZE,D0
    move.l D0,screenPtr3



    clr.w  $DFF1E6                                   ; clear modulo
    move.l screenPtr,$DFF1EC                         ; Set GFXPTR
    move.w #SCREENMODE,D0
    and.w  SCREENMASK,D0	
    move.w D0,$DFF1F4                                ; Set GFX to assigned mode


    clr.l  $DFF1D0                                   ; Clear MousePtr

.end:
	
    movem.l (SP)+,D1-A6                               ; Successful, returns ZERO in d0 
    moveq  #0,D0
    rts


.failedEnd:
	
    movem.l (SP)+,D1-A6                               ; Failed, returns -1 in d0
    moveq  #-1,D0
    rts

;-----------------------------------------------------------------------------
;	Function	Hardware_RestoreSystem
;	Brief		returns system to prior state and restarts interrupts
;	Regs  		d0 is trashed
;	return		d0 status, 0  - set d0 when exiting to show no errors	
;-----------------------------------------------------------------------------
Hardware_RestoreSystem:
	
    movem.l d1-a6,-(SP)

    move.w #$7FFF,$DFF096				* Goto back to DOS
    move.w DMACONSTORE,D0				*
    or.w   #$8000,D0					*
    move.w D0,$DFF096					*
										*

    move.w #$7FFF,$DFF09A				* ALL INTENA OFF
    move.w INTENASTORE,D0				*
    or.w   #$8000,D0					*
    move.w D0,$DFF09A					*


    move.l intuitionbase,A6
    move.l screen,A0
    jsr    -$42(A6)                                  ; Close Screen


    move.l $4.w,A6
    move.l cybergfxbase,A1
    jsr    CLOSELIBRARY(A6)

    move.l intuitionbase,A1
    jsr    CLOSELIBRARY(A6)

    movem.l (SP)+,D1-A6	

    clr.l  D0                                        ; return with no error
    rts

*******************************

;-----------------------------------------------------------------------------
;	Function	Hardware_ReadKey
;	Brief		returns key being pressed, if any
;	Regs  		d0 is trashed
;	return		d0 keyboard code for key press	
;-----------------------------------------------------------------------------
Hardware_ReadKey:

    movem.l d1-a6,-(SP)
    move.b $BFEC01,D0
    bset   #6,$BFEE01
    ror.b  #1,D0
    not.b  D0
    moveq  #50,D1
.wait
    tst.b  $BFE001
    dbra   D1,.wait
    bclr   #6,$BFEE01
    movem.l (SP)+,D1-A6	
    rts
	
;-----------------------------------------------------------------------------
;	Function	Hardware_FlipScreen
;	Brief		flips the reference screen, tripple bufferd
;	Regs  		None
;	return		None	
;-----------------------------------------------------------------------------
Hardware_FlipScreen:

    move.l a0,-(SP)

    move.l screenPtr,A0
    move.l A0,$DFF1EC
    move.l screenPtr2,screenPtr
    move.l screenPtr3,screenPtr2
    move.l A0,screenPtr3

    move.l (SP)+,a0
    rts

;-----------------------------------------------------------------------------
;	Function	Hardware_WaitVBL
;	Brief		Waits for a display vertical blank
;	Regs  		None
;	return		None	
;-----------------------------------------------------------------------------
Hardware_WaitVBL:

    btst   #5,$DFF01F                                ; Wait VBL
    beq.b  Hardware_WaitVBL
    move.w #$0020,$DFF09C                            ; clr VBL
    rts

;-----------------------------------------------------------------------------
;	Function	Hardware_SetGreyScalePalette
;	Brief		Sets up black to white, using all 256 colours
;	Regs  		None
;	return		None	
;-----------------------------------------------------------------------------
Hardware_SetGreyScalePalette:

    movem.l d0-d1/a0,-(SP)

    moveq  #0,d0                                     ; current colour
    move.w #255,d1                                   ; Range count

.nextcol:

    move.l d0,$DFF388                                ; set new colour via palette registor
    add.l  #$01010101,d0
    dbra   d1,.nextcol

    movem.l (SP)+,d0-d1/a0
    rts
	
;-----------------------------------------------------------------------------
;	Function	Hardware_SetStandardPalette
;	Brief		Sets up various ranges of colours, using the full 256 palette
;	Regs  		None
;	return		None	
;-----------------------------------------------------------------------------
Hardware_SetStandardPalette:

    movem.l d0-d2/a0,-(SP)

    lea    PALETTE_DATA,a0
    moveq  #0,d0                                     ; current colour
    move.l #15,d1                                    ; Range count

.nextRange:

    move.l #15,d2                                    ; colour count
    and.l  #$FF000000,d0                             ; reset the colours to zero

.nextcol:

    and.l  PALETTEDATA_MASK(a0),d0                   ; and the value
    or.l   PALETTEDATA_OR(a0),d0                     ; or the fixed component to the value
    move.l d0,$DFF388                                ; set new colour via palette registor
    add.l  PALETTEDATA_ADD(a0),d0                    ; add the next value 

    dbra   d2,.nextcol

    add.l  #PALETTEDATA_SIZE,a0

    dbf    d1,.nextRange

    movem.l (SP)+,d0-d2/a0
    rts

;-----------------------------------------------------------------------------
;	Function	Hardware_SetPalette
;	Brief		Sets up colours from raw palette data
;	Regs  		None
;	return		None	
;-----------------------------------------------------------------------------
Hardware_SetPalette:

    movem.l d0-d1/a0,-(SP)

    lea    GamePalette,a0

    move.l (a0)+,d1                                  ; Range count
    sub.l  #1,d1	
	
.nextcol:

    move.l (a0)+,d0
    move.l d0,$DFF388                                ; set new colour via palette registor
    dbra   d1,.nextcol

    movem.l (SP)+,d0-d1/a0
    rts


;-----------------------------------------------------------------------------

    SECTION DATA,DATA_C

;-----------------------------------------------------------------------------

PALETTE_DATA:

				; First bank of 16 colours - black to white (0-15)

    dc.l   $00FFFFFF                                 ; max colour range
    dc.l   $FFFFFFFF                                 ; mask for changing the colours
    dc.l   $010F0F0F                                 ; each colour addition
    dc.l   $00000000                                 ; OR value

    dc.l   $00FF0000, $FFFF0000, $010F0000, $00000000 ; Bank 2  (16-31)
    dc.l   $0000FF00, $FF00FF00, $01000F00, $00000000 ; Bank 3  (32-47)
    dc.l   $000000FF, $FF0000FF, $0100000F, $00000000 ; Bank 4  (48-63)
    dc.l   $00FF00FF, $FFFF00FF, $010F000F, $00000000 ; Bank 5  (64-79)
    dc.l   $00FFFF00, $FFFFFF00, $010F0F00, $00000000 ; Bank 6  (80-95)
    dc.l   $0000FFFF, $FF00FFFF, $01000F0F, $00000000 ; Bank 7  (96-111)
    dc.l   $00FFFF00, $FFFFFF00, $010F0F00, $000000FF ; Bank 8  (112-127)
    dc.l   $00FF0000, $FFFF0000, $010F0000, $000000FF ; Bank 9  (128-143)
    dc.l   $0000FF00, $FF00FF00, $01000F00, $000000FF ; Bank 10 (144-159)
    dc.l   $00FF00FF, $FFFF00FF, $010F000F, $0000FF00 ; Bank 11 (160-175)
    dc.l   $00FF0000, $FFFF0000, $010F0000, $0000FF00 ; Bank 12 (176-191)
    dc.l   $000000FF, $FF0000FF, $0100000F, $0000FF00 ; Bank 13 (192-207)
    dc.l   $0000FFFF, $FF00FFFF, $01000F0F, $00FF0000 ; Bank 14 (208-223)
    dc.l   $0000FF00, $FF00FF00, $01000F00, $00FF0000 ; Bank 15 (224-239)
    dc.l   $000000FF, $FF0000FF, $0100000F, $00FF0000 ; Bank 16 (240-255)



V4  dc.l   0                                         ; V4 ID
ECS dc.w   0                                         ; Graphics chip ID
SCREENMASK		dc.w	0
DMACONSTORE	dc.w	0
INTENASTORE	dc.w	0

* Tags for ask OS for a screen
modetags		dc.l	$80050001,SCREENWIDTH
    dc.l   $80050002,SCREENHEIGHT
    dc.l   $80050000,SCREENCOLORDEPTH
    dc.l   0,0
screentags		dc.l	$80000032
screenmodeid	dc.l	0				;ID=?
    dc.l   0,0


intuitionbase	dc.l 	0
intuitionname	dc.b 	"intuition.library",0

    even

cybergfxbase	dc.l	0
cybergfxname	dc.b 	"cybergraphics.library",0

    even

screen			dc.l	0

screenPtr		dc.l	0
screenPtr2		dc.l	0
screenPtr3		dc.l	0

    EVEN

GamePalette:

    INCBIN "Graphics/NeilImage.png.RAW.pal"

    SECTION screens,BSS_C

screens		ds.b	SCREENSIZE*3+64


;-----------------------------------------------------------------------------

;-----------------------------------------------------------------------------
; End of file: Hardware.s
;-----------------------------------------------------------------------------

