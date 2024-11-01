;--T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T
;-----------------------------------------------------------------------------
;	Project		Apollo V4 development - Shell
;	File		Hardware.s
;	Coder		Neil Beresford
;	Build envrn	ASMPro V1.18
;-----------------------------------------------------------------------------
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

VAMPIRE				EQU 1				; Working on VAMPIRE ?
CHECK_COMPILE		EQU 0				; Used to check if this file builds

;-----------------------------------------------------------------------------
; Defines
;-----------------------------------------------------------------------------

; For testing build

	IF CHECK_COMPILE = 1

SCREENWIDTH 		EQU 320				; Screen width
SCREENHEIGHT 		EQU 256				; Screen height
SCREENSIZE 			EQU SCREENWIDTH*SCREENHEIGHT
SCREENCOLORDEPTH 	EQU 8				; 256 colours
SCREENMODE 			EQU $0301			; Set to 320x256 mode


	ENDIF

VAMPIREHWVERSION	EQU	$dff3fc
OPENLIBRARY 		EQU -$228
CLOSELIBRARY 		EQU -414
OPENSCREENTAGLIST 	EQU -$264
BESTCMODEIDTAGLIST 	EQU -60
VPOSR				EQU $dff004
VPOSRCHIPIDMASK		EQU	$0f		

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
	
	movem.l	d1-a6,-(SP)

	move.b	VPOSR,D0					; OCS or ECS?
	and.b	VPOSRCHIPIDMASK,D0			;
	move.b	D0,ECS						;

	IF VAMPIRE = 1


	move.b	VAMPIREHWVERSION,D0			; Vampire version
	beq.w	.failedEnd					; no vampire?

	move.w	#$FFFF,SCREENMASK			; V4 oe V2
	move.b	#4,V4
	cmp.b	#1,D0
	beq.s	.v2
	cmp.b	#2,D0
	beq.s	.v2
	cmp.b	#6,D0
	bne.s	.enddetection

.v2:

	ENDIF

	move.w	#$00FF,SCREENMASK
	clr.b	V4

.enddetection:

	move.l	$4.w,A6						; execbase
	lea 	intuitionname,A1
	moveq	#39,D0						; Min OS 3
	jsr		OPENLIBRARY(A6)				; open library
	tst.l	D0
	beq.w	.failedEnd
	move.l	D0,intuitionbase

	lea 	cybergfxname,A1
	moveq	#39,D0						; Min OS 3
	jsr		OPENLIBRARY(A6)				; open library
	tst.l	D0
	beq.w	.failedEnd
	move.l	D0,cybergfxbase

	move.l	cybergfxbase,A6
	lea		modetags,A0
	jsr		BESTCMODEIDTAGLIST(A6)
	tst.l	D0
	beq.w	.failedEnd
	move.l	D0,screenmodeid

	move.l	intuitionbase,A6
	suba.l	A0,A0
	lea		screentags,A1
	jsr		OPENSCREENTAGLIST(A6)		; Openscreentaglist
	tst.l	D0
	beq.w	.failedEnd
	move.l	D0,screen


	move.w	$DFF002,DMACONSTORE			; SAVE DMACON
	move.w	#$7FFF,$DFF096				; ALL DMA OFF

	move.w	$DFF01C,INTENASTORE			; SAVE INTENA
	move.w	#$7FFF,$DFF09A				; ALL INTENA OFF



	move.l	#screens,D0					; we use several Double Buffer
	add.l	#31,D0						; this demo does not need this
	and.l	#$FFFFFFE0,D0				; with this trick we display over 100 FPS without flicker
	move.l	D0,screenPtr
	add.l	#SCREENSIZE,D0
	move.l	D0,screenPtr2
	add.l	#SCREENSIZE,D0
	move.l	D0,screenPtr3



	clr.w	$DFF1E6						; clear modulo
	move.l	screenPtr,$DFF1EC			; Set GFXPTR
	move.w	#SCREENMODE,D0
	and.w	SCREENMASK,D0	
	move.w	D0,$DFF1F4					; Set GFX to assigned mode


	clr.l	$DFF1D0						; Clear MousePtr

.end:
	
	movem.l (SP)+,D1-A6					; Successful, returns ZERO in d0 
	moveq	#0,D0
	rts


.failedEnd:
	
	movem.l (SP)+,D1-A6					; Failed, returns -1 in d0
	moveq	#-1,D0
	rts

	EVEN

;-----------------------------------------------------------------------------
;	Function	Hardware_RestoreSystem
;	Brief		returns system to prior state and restarts interrupts
;	Regs  		d0 is trashed
;	return		d0 status, 0  - set d0 when exiting to show no errors	
;-----------------------------------------------------------------------------
Hardware_RestoreSystem:
	
	movem.l	d1-a6,-(SP)

	move.w	#$7FFF,$DFF096				* Goto back to DOS
	move.w	DMACONSTORE,D0				*
	or.w	#$8000,D0					*
	move.w	D0,$DFF096					*
										*

	move.w	#$7FFF,$DFF09A				* ALL INTENA OFF
	move.w	INTENASTORE,D0				*
	or.w	#$8000,D0					*
	move.w	D0,$DFF09A					*


	move.l	intuitionbase,A6
	move.l	screen,A0
	jsr		-$42(A6)		; Close Screen


	move.l	$4.w,A6
	move.l	cybergfxbase,A1
	jsr		CLOSELIBRARY(A6)

	move.l	intuitionbase,A1
	jsr		CLOSELIBRARY(A6)

	movem.l (SP)+,D1-A6	

	clr.l	D0			; return with no error
	rts

*******************************

	EVEN

;-----------------------------------------------------------------------------
;	Function	Hardware_ReadKey
;	Brief		returns key being pressed, if any
;	Regs  		d0 is trashed
;	return		d0 keyboard code for key press	
;-----------------------------------------------------------------------------
Hardware_ReadKey:

	movem.l	d1-a6,-(SP)
	move.b	$BFEC01,D0
	bset	#6,$BFEE01
	ror.b	#1,D0
	not.b	D0
	moveq	#50,D1
.wait
	tst.b	$BFE001
	dbra	D1,.wait
	bclr	#6,$BFEE01
	movem.l (SP)+,D1-A6	
	rts
	

	EVEN
;-----------------------------------------------------------------------------
;	Function	Hardware_FlipScreen
;	Brief		flips the reference screen, tripple bufferd
;	Regs  		None
;	return		None	
;-----------------------------------------------------------------------------
Hardware_FlipScreen:

	move.l	a0,-(SP)

	move.l	screenPtr,A0
	move.l	A0,$DFF1EC
	move.l	screenPtr2,screenPtr
	move.l	screenPtr3,screenPtr2
	move.l	A0,screenPtr3

	move.l	(SP)+,a0
	rts


;-----------------------------------------------------------------------------

	SECTION	DATA,DATA_C

;-----------------------------------------------------------------------------

V4			dc.l	0				; V4 ID
ECS			dc.w	0				; Graphics chip ID
SCREENMASK	dc.w	0
DMACONSTORE	dc.w	0
INTENASTORE	dc.w	0

* Tags for ask OS for a screen
modetags		dc.l	$80050001,SCREENWIDTH
				dc.l	$80050002,SCREENHEIGHT
				dc.l	$80050000,SCREENCOLORDEPTH
				dc.l	0,0
screentags		dc.l	$80000032
screenmodeid	dc.l	0				;ID=?
				dc.l	0,0


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

	SECTION	screens,BSS_C

screens			ds.b	245824 		; Removed TOTALSCREENSSIZE as it was causing an erorr with vasam when savbing this file


;-----------------------------------------------------------------------------


;-----------------------------------------------------------------------------
; End of file: Hardware.s
;-----------------------------------------------------------------------------

