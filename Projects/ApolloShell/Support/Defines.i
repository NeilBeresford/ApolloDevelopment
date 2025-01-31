;--T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T---T
;-----------------------------------------------------------------------------
;	Project		Apollo V4 development - Shell
;	File		Defines.i
;	Coder		Neil Beresford
;	Build envrn	ASMPro V1.20b
;-----------------------------------------------------------------------------
;	Notes
;
;-----------------------------------------------------------------------------

;-----------------------------------------------------------------------------
; Defines
;-----------------------------------------------------------------------------

SCREENWIDTH 		= 640							; Screen width
SCREENHEIGHT 		= 480							; Screen height
SCREENWIDTH 		= 640							; Screen width
SCREENHEIGHT 		= 480							; Screen height
SCREENSIZE 		    = SCREENWIDTH*SCREENHEIGHT		; Size of screen in pixels
TOTALSCREENSSIZE    = SCREENSIZE * 3 + 64           ; Total bytes needed for the screens
SCREENCOLORDEPTH 	= 8								; 256 colours
SCREENMODE 		    = $0501							; Set to 640x480 mode
SCREENMODE 		    = $0501							; Set to 640x480 mode
SCREENCLEARCOLOUR	= $98989898						; Set to a light green

BACKSCREENWIDTH     = SCREENWIDTH*3                 ; Back screen width (640*4)
BACKSCREENHEIGHT    = 900                           ; Back screen height

;-----------------------------------------------------------------------------
; End of file: Defines.i
;-----------------------------------------------------------------------------

