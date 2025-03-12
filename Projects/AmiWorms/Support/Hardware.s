;** ---------------------------------------------------------------------------
;	@file		Hardware.s
;	@defgroup 	AmiWorms Apollo V4 Shell
;	@brief		Apollo V4 development - Shell
;	@date		2020-06-01
;	@version	0.1
;	@copyright	Neil Beresford 2024	
;-----------------------------------------------------------------------------
;	Notes
;
;--------------------------------------------------------------------------- */

                 include    "Defines.i"
                 include    "Macros.i"
    ; Include necessary system includes
                 INCLUDE    "exec/types.i"
                 INCLUDE    "exec/execbase.i"
                 INCLUDE    "exec/interrupts.i"
                 INCLUDE    "hardware/custom.i"
                 INCLUDE    "hardware/cia.i"
                 INCLUDE    "hardware/intbits.i"

                 CNOP       0,4

;-----------------------------------------------------------------------------
; External Function Defs
;-----------------------------------------------------------------------------

                 XDEF       _Hardware_Init
                 XDEF       _Hardware_Close
                 XDEF       _Hardware_WaitVBL
                 XDEF       _Hardware_FlipScreen
                 XDEF       _Hardware_GetScreenPtr
                 XDEF       _Hardware_ReadKey
                 XDEF       _Hardware_ClearScreen
                 XDEF       _Hardware_ClearAllScreen
                 XDEF       _Hardware_TestScreen
                 XDEF       _Hardware_SwapLong
                 XDEF       _Hardware_GetScreenWidth
                 XDEF       _Hardware_GetScreenHeight
                 XDEF       _Hardware_SetScreenmode
                 XDEF       _Hardware_CopyBackToScreen
                 XDEF       _Hardware_CopyBackScreenMap
                 XDEF       _Hardware_CopyBack2ToBack1
                 XDEF       _Hardware_SetBackscreenBuffers
                 XDEF       _Hardware_GetScreenmode
                 XDEF       _Hardware_GetDebug
                 XDEF       _Hardware_RandomNumber
                 XDEF       _Hardware_SetRandSeed
                 XDEF       _Hardware_StoreLastKey
                 XDEF       _Hardware_CheckKeyUp
                 XDEF       _Hardware_SetMapX
                 XDEF       _Hardware_SetMapY
                 XDEF       _Hardware_GetMapX
                 XDEF       _Hardware_GetMapY
                 XDEF       _Hardware_JoystickButtonPressed
                 XDEF       _Hardware_GetSpriteDims
                 XDEF       _Hardware_DrawBlock
                 XDEF       _Hardware_DrawBackScreenBlock
                 XDEF       _Hardware_ReturnKeyState
                 XDEF       _Hardware_SetMousePosition
                 XDEF       _Hardware_StartAudio
                 XDEF       _Hardware_StopAudio
                 XDEF       _Hardware_TurnOnInts
                 XDEF       _Hardware_TurnOffInts            
                 XDEF       screenPtr
                 XDEF       backScreen1
                 XDEF       backScreen2
                 XDEF       SpriteDims

;-----------------------------------------------------------------------------
; Defines
;-----------------------------------------------------------------------------

VAMPIREHWVERSION   EQU $dff3fc
OPENLIBRARY        EQU -$228
CLOSELIBRARY       EQU -414
OPENSCREENTAGLIST  EQU -$264
BESTCMODEIDTAGLIST EQU -60
VPOSR              EQU $dff004
VPOSRCHIPIDMASK    EQU $0f		
DIMS_TOTAL_SIZE    EQU 20000*12
AUDIO_VOL          EQU $0040

;-----------------------------------------------------------------------------
; Functions
;-----------------------------------------------------------------------------
                 SECTION    TEXT, CODE_C

_ApolloTakeOver:

                 movem.l    a6,-(sp)				* Save all registers to Stack

                 movea.l    $4.w,a6					* Load Exec base address in a6

                 lea        INTName(pc),a1 				* a1 = intuition.library name
                 moveq      #0,d0						* d0 = intuition library version (0 = whatever)							
                 jsr        _LVOOpenLibrary(a6)        	* open intuition library

                 movea.l    d0,a6
                 jsr        _LVOCloseWorkBench(a6)     	* close workbench

                 movea.l    $4.w,a6					* Load Exec base address in a6

                 clr.l      a1            			* Store 0 in a1 (current task)
                 jsr        _LVOFindTask(a6)			* Find current task ID (return in d0)
                 move.l     d0,a1					* Store Task ID in a1
                 moveq      #20,d0		 			* Store Task Prio in d0
                 jsr        _LVOSetTaskPri(a6)

                 movem.l    (sp)+,a6				* Save all registers to Stack

                 rts

INTName          dc.b       'intuition.library',0

;** ---------------------------------------------------------------------------
;	@brief 		Initialize the hardware, closing the workbench
;	@ingroup 	AmiWorms
;	@return 	none
; --------------------------------------------------------------------------- */
_Hardware_Init

                 movem.l    d1-a6,-(sp)

                 move.b     VPOSR,D0                                                      ; OCS or ECS?
                 and.b      VPOSRCHIPIDMASK,D0                                            ;
                 move.b     D0,ECS                                                        ;

                 move.b     VAMPIREHWVERSION,D0                                           ; Vampire version
                 beq.w      .failedEnd                                                    ; no vampire?

                 move.w     #$FFFF,SCREENMASK                                             ; V4 oe V2
                 move.b     #4,V4
                 cmp.b      #1,D0
                 beq.s      .v2
                 cmp.b      #2,D0
                 beq.s      .v2
                 cmp.b      #6,D0
                 bne.s      .enddetection

.v2:

                 move.w     #$00FF,SCREENMASK
                 clr.b      V4

.enddetection:


                 move.l     $4.w,A6                                                       ; execbase
                 lea        intuitionname,A1
                 moveq      #39,D0                                                        ; Min OS 3
                 jsr        OPENLIBRARY(A6)                                               ; open library
                 tst.l      D0
                 beq.w      .failedEnd
                 move.l     D0,intuitionbase

                 lea        cybergfxname,A1
                 moveq      #39,D0                                                        ; Min OS 3
                 jsr        OPENLIBRARY(A6)                                               ; open library
                 tst.l      D0
                 beq.w      .failedEnd
                 move.l     D0,cybergfxbase

                 move.l     cybergfxbase,A6
                 lea        modetags,A0
                 jsr        BESTCMODEIDTAGLIST(A6)
                 tst.l      D0
                 beq.w      .failedEnd
                 move.l     D0,screenmodeid

                 move.l     intuitionbase,A6
                 suba.l     A0,A0
                 lea        screentags,A1
                 jsr        OPENSCREENTAGLIST(A6)                                         ; Openscreentaglist
                 tst.l      D0
                 beq.w      .failedEnd
                 move.l     D0,screen

                 move.w     $DFF002,DMACONSTORE                                           ; SAVE DMACON
                 move.w     #$7FFF,$DFF096                                                ; ALL DMA OFF

                 move.w     $DFF01C,INTENASTORE                                           ; SAVE INTENA
                 move.w     #$7FFF,$DFF09A                                                ; ALL INTENA OFF
                
                 move.l     #screens,D0                                                   ; we use several Double Buffer
                 add.l      #31,D0                                                        ; this demo does not need this
                 and.l      #$FFFFFFE0,D0                                                 ; with this trick we display over 100 FPS without flicker
                 move.l     D0,screenPtr
                 add.l      #SCREENSIZE,D0
                 move.l     D0,screenPtr2
                 add.l      #SCREENSIZE,D0
                 move.l     D0,screenPtr3



                 clr.w      $DFF1E6                                                       ; clear modulo
                 move.l     screenPtr,$DFF1EC                                             ; Set GFXPTR
                 move.w     #SCREENMODE,D0
                 and.w      SCREENMASK,D0	
                 move.w     D0,$DFF1F4                                                    ; Set GFX to assigned mode


                 clr.l      $DFF1D0                                                       ; Clear MousePtr
                 moveC      VBR,A0
                 move.l     $70(A0),OldAudioHandler                                       ; Set Level 4 (audio) Vector.

******************************************
* Play Title Music
*
* On V4 we can use ARNE 32bit Audio DMA to play the Music YAY SUPERCOOLARNECHIP
* V4 will play stereo music on chan 0
*
* On V2 we only have Paula and need to copy chunks to chipmem to allow her to play it
* V2 will use channel 0 and 1 to play music
*
                 tst.b      V4
                 beq.s      .V2
.V4
                 move.w     #$820F,$DFF096		* Audio an
                 move.w     #$000F,$DFF096
                 bra.b      .endmusic    

                 bra.b      .endmusic
.V2
	
	; Transfer initial audio data into buffers
                 move.l     #music+64,AudioStart                                          ; StartPointer
                 move.l     AudioStart,AudioWorkPtr                                       ; StartPointer
                 move.l     #music_e,AudioEnd                                             ; End
                 move.w     #$820F,$DFF096		* Audio an
                 move.w     #$000F,$DFF096
                 bsr.w      InitV2Audio

.endmusic
                 movem.l    (sp)+,D1-A6                                                   ; Successful, returns ZERO in d0 
                 moveq      #0,D0
                 rts


.failedEnd:
	
                 movem.l    (sp)+,D1-A6                                                   ; Failed, returns -1 in d0
                 moveq      #-1,D0
                 rts

.getvbr:         dc.w       $4E7A,$1801
                 rte



;-----------------------------------------------------------------------------
;   @brief 	    Turn on interrupts
;   @ingroup 	AmiWorms
;-----------------------------------------------------------------------------
_Hardware_TurnOnInts

                 move.w     $DFF002,INTDMACONSTORE                                        ; SAVE DMACON
                 move.w     $DFF01C,INTINTENASTORE                                        ; SAVE INTENA

                 move.w     #$7FFF,$DFF096				* Goto back to DOS                          ;
                 move.w     DMACONSTORE,D0				*
                 or.w       #$8000,D0					*
                 move.w     D0,$DFF096					*
                 move.w     #$7FFF,$DFF09A				* ALL INTENA OFF
                 move.w     INTENASTORE,D0				*
                 or.w       #$8000,D0					*
                 move.w     D0,$DFF09A					*
                 rts

;-----------------------------------------------------------------------------
;   @brief 	    Turn off interrupts
;   @ingroup 	AmiWorms
;-----------------------------------------------------------------------------
_Hardware_TurnOffInts

                 move.w     INTDMACONSTORE,d0
                 or.w       #$8000,d0
                 move.w     d0,$DFF096                                                    ; ALL DMA OFF
                 move.w     INTINTENASTORE,d0
                 or.w       #$8000,d0
                 move.w     #$7FFF,$DFF09A                                                ; ALL INTENA OFF
                 rts        

;-----------------------------------------------------------------------------
;   @brief 	Start Music
;   @ingroup 	AmiWorms
;   @param 		d0 - music channel
;   @param 		d1 - music start
;   @param 		d2 - music end
;   @param      d3 - music volume
;   @param      d4 - music ctrl
;   @param      d5 - music period
;-----------------------------------------------------------------------------
; Check slot free ...
; DFF202 - DFF203 - Audio DMA Control (slot0 - DFF202+1 #1,slot12 - DFF202 #1)
;-----------------------------------------------------------------------------
;   @return 	none
;-----------------------------------------------------------------------------

_Hardware_StartAudio

                 movem.l    d0-d6/a0-a1,-(sp)

                 move.l     d0,d6

                 add.l      #64,d1
                 sub.l      #64,d2
                 asr.l      #3,d2
                 lea        $dff400,a0
                 asl.l      #4,d0
                 add        d0,a0

                 move.l     d1,$0(a0)
                 move.l     d2,$4(a0)
                 move.w     d3,$8(a0)
                 move.w     d4,$A(a0)
                 move.w     d5,$C(a0)                                                     ;#120,$C(a0)

                 move.l     #AUDIO_ON,a1
                 move.l     d6,d0
                 asl.l      #1,d6
                 cmpi.l     #4,d0
                 bge.s      .other
                 move.w     (a1,d6.w),$DFF096
                 bra.s      .end
.other
                 move.w     (a1,d6.w),$DFF296
.end
                 movem.l    (sp)+,d0-d6/a0-a1
                 rts


;** ---------------------------------------------------------------------------
;	@brief 		Stop Audio
;	@ingroup 	AmiWorms
;   @paraan     D0 - DMA channel
;	@return 	none
; --------------------------------------------------------------------------- */
_Hardware_StopAudio

                 movem.l    d0-d6/a0-a1,-(sp)

                 move.l     #AUDIO_ON,a1
                 move.l     d0,d6
                 asl.l      #1,d6
                 cmpi.l     #4,d0
                 bge.s      .other
                 move.w     (a1,d6.w),d0
                 bclr       #15,d0
                 move.w     d0,$DFF096
                 bra.s      .end
.other
                 move.w     (a1,d6.w),d0
                 bclr       #15,d0
                 move.w     d0,$DFF296
.end
                 movem.l    (sp)+,d0-d6/a0-a1
                 rts

;** ---------------------------------------------------------------------------
;	@brief 		Checks if a joystick button has been pressed
;	@ingroup 	AmiWorms
;	@return 	d0 - 0 if button pressed, 1 if not
; --------------------------------------------------------------------------- */
_Hardware_JoystickButtonPressed

                 clr.l      d0
                 move.w     $DFE001,D0
                 and.w      #$0F00,D0
                 rts

;** ---------------------------------------------------------------------------
;	@brief 		Sets the back screen buffers
;	@ingroup 	AmiWorms
;	@return 	none
; --------------------------------------------------------------------------- */
_Hardware_SetBackscreenBuffers

                 movem.l    d0,-(sp)

                 move.l     #backScreens,D0
                 add.l      #31,D0						
                 and.l      #$FFFFFFE0,D0				
                 move.l     D0,backScreen1
                 add.l      #BACKSCREENWIDTH*BACKSCREENHEIGHT,D0
                 add.l      #31,D0						
                 and.l      #$FFFFFFE0,D0				
                 move.l     D0,backScreen2

                 movem.l    (sp)+,d0
                 rts

;** ---------------------------------------------------------------------------
;	@brief 		Initialize the hardware, closing the workbench
;	@ingroup 	AmiWorms
;	@return 	none
; --------------------------------------------------------------------------- */
_Hardware_Close

                 movem.l    d0-a6,-(sp)

                 move.w     #$7FFF,$DFF096				* Goto back to DOS                          ;
                 move.w     DMACONSTORE,D0				*
                 or.w       #$8000,D0					*
                 move.w     D0,$DFF096					*
										*

                 move.w     #$7FFF,$DFF09A				* ALL INTENA OFF
                 move.w     INTENASTORE,D0				*
                 or.w       #$8000,D0					*
                 move.w     D0,$DFF09A					*
                 moveC      VBR,A0
                 move.l     OldAudioHandler,$70(A0)		* Set Level 4 (audio) Vector.

                 move.l     intuitionbase,A6
                 move.l     screen,A0
                 jsr        -$42(A6)                                                      ; Close Screen


                 move.l     $4.w,A6
                 move.l     cybergfxbase,A1
                 jsr        CLOSELIBRARY(A6)

                 move.l     intuitionbase,A1
                 jsr        CLOSELIBRARY(A6)

                 movem.l    (SP)+,D0-A6	

                 rts

;** ---------------------------------------------------------------------------
;	@brief 		Waits until the start of the vertical blank
;	@ingroup 	AmiWorms
;	@return 	none
; --------------------------------------------------------------------------- */
_Hardware_WaitVBL

.wait:
                 btst       #5,$dff01f
                 beq        .wait
                 move.w     #$20,$dff09c
                 rts

;** ---------------------------------------------------------------------------
;	@brief 		Flips the screen, setting the new buffer as active screen
;	@ingroup 	AmiWorms
;	@return 	none
; --------------------------------------------------------------------------- */
_Hardware_FlipScreen

                 move.l     a0,-(sp)
                 move.l     screenPtr,a0
                 move.l     a0,$DFF1EC
                 move.l     screenPtr2,screenPtr
                 move.l     screenPtr3,screenPtr2
                 move.l     a0,screenPtr3
                 move.l     (sp)+,a0
                 rts

;** ---------------------------------------------------------------------------
;	@brief 		Returns the screen pointer
;	@ingroup 	AmiWorms
;	@return 	d0 - pointer to the screen buffer
; --------------------------------------------------------------------------- */
_Hardware_GetScreenPtr

                 cmp.b      #0,screenmode
                 beq.s      .normal
.back
                 cmp.b      #2,screenmode
                 beq.s      .back2
                 move.l     backScreen1,d0
                 rts
.back2
                 move.l     backScreen2,d0
                 rts
.normal			
                 move.l     screenPtr,d0
                 rts

;** ---------------------------------------------------------------------------
;	@brief 		Sets the screen mode
;	@ingroup 	AmiWorms
;	@param 		d0 - screen mode
;	@return 	none
; --------------------------------------------------------------------------- */
_Hardware_SetScreenmode
                 move.b     d0,screenmode
                 rts	

;** ---------------------------------------------------------------------------
;	@brief 		Returns the screen mode
;	@ingroup 	AmiWorms
;	@return 	d0 - screen mode
; --------------------------------------------------------------------------- */
_Hardware_GetScreenmode
                 clr.l      d0
                 move.b     screenmode,d0
                 rts


;** ---------------------------------------------------------------------------
;	@brief 		Returns the screen width
;	@ingroup 	AmiWorms
;	@return 	d0 - screen width
; --------------------------------------------------------------------------- */
_Hardware_GetScreenWidth
                 cmp.l      #0,screenmode
                 beq.s      .normal
.back
                 move.l     #BACKSCREENWIDTH,d0
                 rts
.normal
                 move.l     #SCREENWIDTH,d0
                 rts

;** ---------------------------------------------------------------------------
;	@brief 		Returns the screen height
;	@ingroup 	AmiWorms
;	@return 	d0 - screen height
; --------------------------------------------------------------------------- */
_Hardware_GetScreenHeight
                 cmp.l      #0,screenmode
                 beq.s      .normal
.back
                 move.l     #BACKSCREENHEIGHT,d0
                 rts
.normal
                 move.l     #SCREENHEIGHT,d0
                 rts

;** ---------------------------------------------------------------------------
;	@brief 		Clears the game area of screen
;	@ingroup 	AmiWorms
;	@return 	none
; --------------------------------------------------------------------------- */
_Hardware_ClearScreen

                 movem.l    d1-d2/a0,-(sp)

                 move.l     screenPtr,a0
                 adda.l     #42*SCREENWIDTH,a0
                 moveq      #0,d1
                 move.l     #(360*SCREENWIDTH)/4,d2
.clear	
                 move.l     d1,(a0)+
                 SUBQ.L     #1,d2,
                 BNE.S      .clear

                 movem.l    (sp)+,d1-d2/a0
                 rts

;** ---------------------------------------------------------------------------
;	@brief 		Clears the full screen
;	@ingroup 	AmiWorms
;	@return 	none
; --------------------------------------------------------------------------- */
_Hardware_ClearAllScreen

                 movem.l    d1-d2/a0,-(sp)

                 move.l     screenPtr,a0
                 moveq      #0,d1
                 move.l     #(480*SCREENWIDTH)/4,d2
.clear	
                 move.l     d1,(a0)+
                 SUBQ.L     #1,d2,
                 BNE.S      .clear

                 movem.l    (sp)+,d1-d2/a0
                 rts



;** ---------------------------------------------------------------------------
;	@brief 		Copies the back screen 2 to the back screen 1
;	@ingroup 	AmiWorms
;	@return 	none
; --------------------------------------------------------------------------- */
_Hardware_CopyBack2ToBack1

                 movem.l    d0/a0-a1,-(sp)

                 move.l     backScreen2,a0
                 move.l     backScreen1,a1
                 move.l     #(BACKSCREENWIDTH*BACKSCREENHEIGHT)/4,d0
.copy
                 move.l     (a0)+,(a1)+
                 subq.l     #1,d0
                 bne.s      .copy

                 movem.l    (sp)+,d0/a0-a1
                 rts

;** ---------------------------------------------------------------------------
;	@brief 		Generates a random number
;	@ingroup 	AmiWorms
;	@return 	d0 - random number
; --------------------------------------------------------------------------- */
_Hardware_RandomNumber

	; generate a random number
                 movem.l    d1-d2,-(sp)

                 moveq      #$AF-$100,d1
                 moveq      #18,d2
                 move.l     RanSeed,d0
.ran0
                 add.l      d0,d0
                 bcc.s      .ran1
                 eor.l      d1,d0
.ran1
                 dbf        d2,.ran0
                 and.l      #$7FFFFFFF,d0
                 move.l     d0,RanSeed			
                 movem.l    (sp)+,d1-d2
                 rts

;** ---------------------------------------------------------------------------
;	@brief 		Sets the random seed
;	@ingroup 	AmiWorms
;	@param 		d0 - random seed
;	@return 	none
; --------------------------------------------------------------------------- */
_Hardware_SetRandSeed
                 move.l     d0,RanSeed
                 rts


;/**---------------------------------------------------------------------------
; @brief 		Sets the map X
; @ingroup 	    AmiWorms
; @param 		d0 - map X
;----------------------------------------------------------------------------*/
_Hardware_SetMapX

                 move.l     d0,mapX
                 rts

;/**---------------------------------------------------------------------------
; @brief 		Gets the map X
; @ingroup 	    AmiWorms
; @return 		d0 - map X
;----------------------------------------------------------------------------*/
_Hardware_GetMapX

                 move.l     mapX,d0
                 rts
;/**---------------------------------------------------------------------------
; @brief 		Sets the map Y
; @ingroup 	    AmiWorms
; @param 		d0 - map Y
;----------------------------------------------------------------------------*/
_Hardware_SetMapY

                 move.l     d0,mapY
                 rts
	
;/**---------------------------------------------------------------------------
; @brief 		Gets the map Y
; @ingroup 	    AmiWorms
; @return 		d0 - map Y
;----------------------------------------------------------------------------*/
_Hardware_GetMapY

                 move.l     mapY,d0
                 rts

;** ---------------------------------------------------------------------------
;	@brief 		Copies the back screen to the front screen
;	@ingroup 	AmiWorms
;	@param 		d0 - SCrollX
;	@param 		d1 - SCrollY
;	@return 	none
; --------------------------------------------------------------------------- */
_Hardware_CopyBackToScreen

                 movem.l    d0-d6/a0-a1,-(sp)

                 move.l     mapX,d0
                 move.l     mapY,d1	

                 move.l     screenPtr,a0
                 add.l      #42*SCREENWIDTH,a0
                 move.l     backScreen2,a1
                 move.l     #BACKSCREENWIDTH,d2
                 move.l     #BACKSCREENHEIGHT,d3
                 move.l     #SCREENWIDTH,d4
                 move.l     #SCREENHEIGHT-120,d5
                 move.l     d0,d6

	; get the start of the back screen to copy
                 mulu       d2,d1
                 add.l      d0,d1
                 add.l      d1,a1
                 lsr.l      #2,d4
                 subq.l     #1,d4
                 subq.l     #1,d5

	; copy the back screen to the main screen
.copy
                 move.l     d4,d3
.copy2
                 move.l     (a1)+,(a0)+
                 dbra       d3,.copy2
                 add.l      #BACKSCREENWIDTH-SCREENWIDTH,a1
                 dbra       d5,.copy

.end	
                 movem.l    (sp)+,d0-d6/a0-a1
                 rts

;** ---------------------------------------------------------------------------
;	@brief 		Copies the back screen map view to the front screen
;	@ingroup 	AmiWorms
;	@return 	none
; --------------------------------------------------------------------------- */
_Hardware_CopyBackScreenMap

                 movem.l    d0-d7/a0-a2,-(sp)

                 moveq      #0,d0
                 moveq      #0,d1

	; clear the top and bottom of the screen
                 move.l     screenPtr,a2
                 add.l      #42*SCREENWIDTH,a2
                 move.l     #(28*(SCREENWIDTH/4))-1,d2
.loop1
                 move.l     d0,(a2)+
                 dbra       d2,.loop1
                 move.l     screenPtr,a2
                 add.l      #370*SCREENWIDTH,a2
                 move.l     #(32*(SCREENWIDTH/4))-1,d2
.loop2
                 move.l     d0,(a2)+
                 dbra       d2,.loop2

	; copy the back screen to the main screen, drawing a maop view 
                 move.l     screenPtr,a0
                 add.l      #70*SCREENWIDTH,a0
                 move.l     backScreen2,a1
                 move.l     #BACKSCREENWIDTH,d2
                 move.l     #BACKSCREENHEIGHT,d3
                 move.l     #SCREENWIDTH,d4
                 move.l     #320,d5
                 move.l     d0,d6
                 move.l     d1,d7

	; get the start of the back screen to copy
                 mulu       d2,d7
                 add.l      d6,d7
                 add.l      d7,a1

	; calculate the modulos
                 move.l     d2,d0
                 sub.l      d4,d0
                 subq.l     #1,d4
                 subq.l     #1,d5
	; copy the back screen to the main screen
.copy
                 move.l     d4,d3
.copy2
                 move.b     (a1)+,(a0)+

                 addq.l     #2,a1
                 DBEQ       d3,.copy2
                 add.l      #2*BACKSCREENWIDTH,a1
                 DBEQ       d5,.copy

	; end of the copy
                 movem.l    (sp)+,d0-d7/a0-a2
                 rts

;** ---------------------------------------------------------------------------
;	@brief 		Test screen
;	@ingroup 	AmiWorms
;	@return 	none
; --------------------------------------------------------------------------- */
_Hardware_TestScreen

                 movem.l    d1-d2/a0,-(sp)

                 move.l     screenPtr,a0
                 move.l     #$00010203,d1
                 move.l     #SCREENSIZE/4,d2
.clear	
                 move.l     d1,(a0)+
                 add.l      #$04040404,d1
                 dbra       d2,.clear

                 movem.l    (sp)+,d1-d2/a0
                 rts

;** ---------------------------------------------------------------------------
;	@brief 		Get the debug value
;	@ingroup 	AmiWorms
; 	@param 		d0 - debug number required
;	@return 	d0 - debug value
; --------------------------------------------------------------------------- */
_Hardware_GetDebug

                 movem.l    a0,-(SP)

                 asl.l      #2,d0
                 lea        debug1,a0
                 add.l      d0,a0
                 move.l     (a0),d0

                 movem.l    (SP)+,a0
                 rts

                 CNOP       0,4

;** ---------------------------------------------------------------------------
;	@brief 		Reads for a key press
;	@ingroup 	AmiWorms
;	@return 	d0 - key press
; --------------------------------------------------------------------------- */
_Hardware_ReadKey

                 movem.l    d1-a6,-(SP)
                 move.b     $BFEC01,D0
                 bset       #6,$BFEE01
                 ror.b      #1,D0
                 not.b      D0
                 moveq      #50,D1
.wait
                 tst.b      $BFE001
                 dbra       D1,.wait

                 bclr       #6,$BFEE01
                 movem.l    (SP)+,D1-A6	
                 rts


;** ---------------------------------------------------------------------------
;	@brief 		Stores the last key pressed
;	@ingroup 	AmiWorms
;	@param 		d0 - raw key code
;	@return 	none
; --------------------------------------------------------------------------- */
_Hardware_StoreLastKey
                 rts

                 move.b     D0,lastKey
                 rts

;** ---------------------------------------------------------------------------
;	@brief 		Checks if the key has been released
;	@ingroup 	AmiWorms
;	@return 	none
; --------------------------------------------------------------------------- */
_Hardware_CheckKeyUp
                 rts

                 movem.l    D0,-(SP)
                 cmp.b      #0,lastKey
                 beq.s      .exit
                 bclr       #6,$BFEE01
                 moveq      #50,D1
.wait
                 tst.b      $BFE001
                 dbra       D1,.wait
                 move.b     $BFEC01,D0
                 bset       #6,$BFEE01
                 ror.b      #1,D0
                 not.b      D0
                 moveq      #50,D1
.wait2
                 tst.b      $BFE001
                 dbra       D1,.wait2
                 cmp.b      lastKey,D0
                 beq.s      .exit
                 bclr       #6,$BFEE01
                 move.b     #0,lastKey
.exit
                 movem.l    (SP)+,D0
                 rts

                 CNOP       0,4
;** ---------------------------------------------------------------------------
;	@brief 		swaps byte order
;	@ingroup 	AmiWorms
;	@param 		d0 - long to swap
;	@return 	d0 - swapped result
; --------------------------------------------------------------------------- */
_Hardware_SwapLong

                 perm       #@3210,d0,d0									* swap LONG
                 rts


;** ---------------------------------------------------------------------------
;	@brief 		Get the sprite dimensions
;	@ingroup 	AmiWorms
;	@return 	d0 - sprite dimensions
; --------------------------------------------------------------------------- */
_Hardware_GetSpriteDims

                 move.l     #SpriteDims,d0
                 rts

;** ---------------------------------------------------------------------------
;  @brief 	Draws a block of pixels to the screen
;  @ingroup  AmiWorms
;  @param  d0 - x position
;  @param  d1 - y position
;  @param  d2 - width
;  @param  d3 - height
;  @param  d4 - colour
;  @return none
; --------------------------------------------------------------------------- */
_Hardware_DrawBlock

                 movem.l    d0-d7/a0-a6,-(sp)

                 move.l     screenPtr,a1
                 add.l      d0,a1
                 mulu       #SCREENWIDTH,d1
                 add.l      d1,a1
                 move.l     d2,d1
.yloop:
                 move.l     d2,d1
                 move.l     a1,a0
.xloop:
                 move.b     d4,(a0)+
                 subq.l     #1,d1
                 bne.s      .xloop
                 add.l      #SCREENWIDTH,a1
                 dbra       d3,.yloop

                 movem.l    (sp)+,d0-d7/a0-a6
                 rts

;** ---------------------------------------------------------------------------
;  @brief 	Draws a block of pixels to the back screen
;  @ingroup  AmiWorms
;  @param  d0 - x position
;  @param  d1 - y position
;  @param  d2 - width
;  @param  d3 - height
;  @param  d4 - colour
;  @return none
; --------------------------------------------------------------------------- */
_Hardware_DrawBackScreenBlock

                 movem.l    d0-d7/a0-a6,-(sp)

                 move.l     backScreen2,a1
                 add.l      d0,a1
                 mulu       #BACKSCREENWIDTH,d1
                 add.l      d1,a1
                 asr.l      #2,d2
.yloop:
                 move.l     d2,d1
                 move.l     a1,a0
.xloop:
                 move.l     d4,(a0)+
                 subq.l     #1,d1
                 bne.s      .xloop
                 add.l      #BACKSCREENWIDTH,a1
                 dbra       d3,.yloop

                 movem.l    (sp)+,d0-d7/a0-a6
                 rts

;** ---------------------------------------------------------------------------
;	@brief 		Returns the state of the last key pressed
;	@ingroup 	AmiWorms
;	@return 	d0 - key state
; --------------------------------------------------------------------------- */
_Hardware_ReturnKeyState

                 move.l     #keys,d0
                 rts


;-----------------------------------------------------------------------------
; Interrupts
;-----------------------------------------------------------------------------

IntLvlTwoPorts:
                 movem.l    d0-d1/a0-a2,-(a7)

                 lea        $dff00,a0
                 moveq      #INTF_PORTS,d0

	;check if is it level 2 interrupt
                 move.w     intreqr(a0),d1
                 and.w      d0,d1
                 beq.b      .end

	;check if SP cause interrupt, hopefully CIAICRF_SP = 8
                 lea        $bfe001,a1 
                 move.b     ciaicr(a1),d1
                 and.b      d0,d1
                 beq.b      .end

                 move.b     ciasdr(a1),d1                                                 ;get keycode
                 or.b       #CIACRAF_SPMODE,ciacra(a1)                                    ;start SP handshaking

                 lea        keys(pc),a2
                 not.b      d1
                 lsr.b      #1,d1
                 scc        (a2,d1.w)

	;handshake
                 moveq      #3-1,d1
.wait1           move.b     vhposr(a0),d0
.wait2           cmp.b      vhposr(a0),d0
                 beq.b      .wait2
                 dbf        d1,.wait1

	;set input mode
                 and.b      #$7F,ciacra(a1)

.end             move.w     #INTF_PORTS,intreq(a0)
                 tst.w      intreqr(a0)
                 movem.l    (a7)+,d0-d1/a0-a2
                 rte

;/*----------------------------------------------------------------------------
;	@brief		Set the mouse position
;	@ingroup	AmiWorms
;	@param		d0 - x position
;	@param		d1 - y position
;----------------------------------------------------------------------------*/
_Hardware_SetMousePosition

                 movem.l    d0-d1/a0,-(sp)
                 swap       d0
                 or.l       d0,d1      
                 move.l     #$70,d0
                 trap       #15
                 move.l     #$76,d0
                 trap       #15
                 movem.l    (sp)+,d0-d1/a0
                 rts

;-----------------------------------------------------------------------------

;/*----------------------------------------------------------------------------
;	@brief		Apollo V2 music
;	@ingroup	AmiWorms
;----------------------------------------------------------------------------*/
InitV2Audio:
                 move.l     AudioStart,A0
                 lea        leftBuffer1,a1
                 lea        rightBuffer1,a2



                 move.l     A0,AudioWorkPtr                                               ; save Ptr

	; Setup Audio Channel IRQ
                 moveC      VBR,A0
                 move.l     #AudioHandler,$70(A0)                                         ; Set Level 4 (audio) Vector.

	; Set Sampling Rate and Period
	; PAL Clock Constant = 3546895
	; Period = Clock Constant / Hz (22050hz -> 160.857)
                 tst.b      ECS
                 beq        .ocs
                 move.w     #80,$DFF0A6
                 move.w     #80,$DFF0B6

                 move.w     #221-1,d0                                                     ; Number of samples to copy.
.copy
                 move.w     (A0)+,D1                                                      ; Load 16bit sample (L)
                 move.w     (A0)+,D2                                                      ; Load 16bit sample (R)
                 move.b     (A0),D1
                 move.b     2(A0),D2
                 addq.l     #4,A0
                 move.w     D1,(A1)+
                 move.w     D2,(A2)+    
                 dbra       D0,.copy
                 bra        .bitrate
.ocs	
                 move.w     #160,$DFF0A6
                 move.w     #160,$DFF0B6

                 move.w     #221-1,d0                                                     ; Number of samples to copy.
.copy2
                 move.w     (A0)+,D1                                                      ; Load 16bit sample (L)
                 move.w     (A0)+,D2                                                      ; Load 16bit sample (R)
                 addq.l     #4,A0
                 move.b     (A0),D1
                 move.b     2(A0),D2
                 addq.l     #8,A0

                 move.w     D1,(A1)+
                 move.w     D2,(A2)+    
                 dbra       D0,.copy2
.bitrate

	; Set Audio Data Buffer Lengths
                 move.w     #(442/2),$DFF0A4                                              ; Set Audio Length for Channel 0.
                 move.w     #(442/2),$DFF0B4                                              ; Set Audio Length for Channel 1.

	; Set Audio Buffer Locations
                 move.l     #leftBuffer1,$DFF0A0
                 move.l     #rightBuffer1,$DFF0B0

	; Enable Audio IRQ 
	; -> As both channels are in lock-step, we only need a single IRQ.
                 move.w     #$c080,$dff09a

	; Set Volume and Start DMA
                 move.w     #AUDIO_VOL,$DFF0A8                                            ; Set Volume for Channel 0.
                 move.w     #AUDIO_VOL,$DFF0B8                                            ; Set Volume for Channel 1.
                 move.w     #$8203,$DFF096                                                ; Enable Audio Channel DMA 0+1
                 clr.b      Audioticktock

                 rts


;/*----------------------------------------------------------------------------
;	@brief		Music player interrupt, Apollo V2
;	@ingroup	AmiWorms
;----------------------------------------------------------------------------*/
AudioHandler:

                 movem.l    d0-d2/a0-a2,-(sp)

                 move.w     #$0080,$DFF09C                                                ; Clear INTREQ for Audio 0.

                 tst.b      Audioticktock
                 beq.s      .pong
.ping
                 lea        leftBuffer1,a1
                 lea        rightBuffer1,a2
                 bra.s      .process
.pong
                 lea        leftBuffer2,a1
                 lea        rightBuffer2,a2

    ; We process both left and right channels from
    ; a single IRQ, as both are running at the same rate in lock-step
.process
                 not.b      Audioticktock
                 move.l     A1,$DFF0A0                                                    ; Set New Buffer Address.
                 move.l     A2,$DFF0B0                                                    ; Set New Buffer Address.

                 tst.b      ECS
                 beq        .ocs
.ecs
                 move.l     AudioWorkPtr,a0
                 move.w     #442/2-1,D0                                                   ; Number of samples to copy.
.copyE
                 move.w     (A0)+,D1                                                      ; Load 16bit sample (L)
                 move.w     (A0)+,D2                                                      ; Load 16bit sample (R)
                 move.b     (A0),D1
                 move.w     D1,(A1)+
                 move.b     2(A0),D2
                 addq.l     #4,A0
                 move.w     D2,(A2)+
                 dbra       D0,.copyE
                 bra        .bitrate

.ocs
                 move.l     AudioWorkPtr,a0
                 move.w     #442/2-1,D0                                                   ; Number of samples to copy.
.copyO
                 move.w     (A0)+,D1                                                      ; Load 16bit sample (L)
                 move.w     (A0)+,D2                                                      ; Load 16bit sample (R)
                 addq.l     #4,A0
                 move.b     (A0),D1
                 move.w     D1,(A1)+
                 move.b     2(A0),D2
                 addq.l     #8,A0
                 move.w     D2,(A2)+
                 dbra       D0,.copyO
.bitrate

                 move.l     AudioEnd,a1
                 cmp.l      a1,a0
                 blt.s      .noloop
                 move.l     AudioStart,A0
.noloop
                 move.l     A0,AudioWorkPtr
.done

                 movem.l    (sp)+,D0-D2/A0-A2
                 rte



                 SECTION    DATA,DATA_C

;-----------------------------------------------------------------------------

keys             ds.b       $80
vbrBaseAddress   dc.l       0

                 even
V4               dc.l       0                                                             ; V4 ID
ECS              dc.w       0                                                             ; Graphics chip ID
SCREENMASK       dc.w       0
DMACONSTORE      dc.w       0
INTENASTORE      dc.w       0
INTDMACONSTORE   dc.w       0
INTINTENASTORE   dc.w       0


RanSeed          dc.l       $12345678
lastKey          dc.b       0
                 even

* Tags for ask OS for a screen
modetags         dc.l       $80050001,SCREENWIDTH
                 dc.l       $80050002,SCREENHEIGHT
                 dc.l       $80050000,SCREENCOLORDEPTH
                 dc.l       0,0
screentags       dc.l       $80000032
screenmodeid     dc.l       0                                                             ;ID=?
                 dc.l       0,0


intuitionbase    dc.l       0
intuitionname    dc.b       "intuition.library",0

                 even

cybergfxbase     dc.l       0
cybergfxname     dc.b       "cybergraphics.library",0


                 even

screenmode       dc.l       0
screen           dc.l       0
screenPtr        dc.l       0
screenPtr2       dc.l       0
screenPtr3       dc.l       0
backScreen1      dc.l       0
backScreen2      dc.l       0
mapX             dc.l       0
mapY             dc.l       0

debug1           dc.l       0
debug2           dc.l       0
debug3           dc.l       0
debug4           dc.l       0

OldAudioHandler  dc.l       0				
AudioStart       dc.l       0
AudioWorkPtr     dc.l       0
AudioEnd         dc.l       0
Audioticktock    dc.b       0

music            dc.l       0
music_e          dc.l       0

AUDIO_ON         dc.w       $8001                                                         ; channel 0
                 dc.w       $8002                                                         ; channel 1
                 dc.w       $8004                                                         ; channel 2
                 dc.w       $8008                                                         ; channel 3
                 dc.w       $8001                                                         ; channel 4
                 dc.w       $8002                                                         ; channel 5
                 dc.w       $8004                                                         ; channel 6
                 dc.w       $8008                                                         ; channel 7
                 dc.w       $8010                                                         ; channel 8
                 dc.w       $8020                                                         ; channel 9
                 dc.w       $8040                                                         ; channel 10
                 dc.w       $8080                                                         ; channel 11
                 dc.w       $8100                                                         ; channel 12
                 dc.w       $8200                                                         ; channel 13
                 dc.w       $8400                                                         ; channel 14
                 dc.w       $8800                                                         ; channel 15


                 even

                 SECTION    screens,BSS_C

screens          ds.b       (SCREENWIDTH*SCREENHEIGHT)*3+64                               ; Removed TOTALSCREENSSIZE as it was causing an erorr with vasam when saving this file
leftBuffer1      ds.w       221                                                           ; We use 2 buffers for each left/right 
leftBuffer2      ds.w       221                                                           ; so that we can ping-pong between them when reloading
rightBuffer1     ds.w       221                                                           ; during the IRQ.
rightBuffer2     ds.w       221


                 SECTION    backscreens,BSS_F

backScreens      ds.b       BACKSCREENWIDTH*BACKSCREENHEIGHT                              ; back screen 1
bs2              ds.b       BACKSCREENWIDTH*BACKSCREENHEIGHT                              ; back screen 2
bs3              ds.b       BACKSCREENWIDTH*BACKSCREENHEIGHT                              ; back screen 3

                 even
                 section    AppBSS,BSS_C
                 SECTION    SpriteDims,BSS_F

SpriteDims       ds.b       DIMS_TOTAL_SIZE

                 even	

;------------------------------------------------------------------------------
; End of File: Hardware.s
;------------------------------------------------------------------------------
