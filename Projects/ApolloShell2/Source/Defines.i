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

SCREENWIDTH 		= 320							; Screen width
SCREENHEIGHT 		= 256							; Screen height
SCREENSIZE 		    = SCREENWIDTH*SCREENHEIGHT		; Size of screen in pixels
TOTALSCREENSSIZE    = SCREENSIZE * 3 + 64           ; Total bytes needed for the screens
SCREENCOLORDEPTH 	= 8								; 256 colours
SCREENMODE 		    = $0301							; Set to 320x256 mode
SCREENCLEARCOLOUR	= $98989898						; Set to a light green

;-----------------------------------------------------------------------------
; End of file: Defines.i
;-----------------------------------------------------------------------------

