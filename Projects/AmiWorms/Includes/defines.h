/** ---------------------------------------------------------------------------
    @file		Defines.h
    @defgroup 	AmiWorms Apollo V4 Shell
    @brief		Basic defines, taken from defines.i (assembler)
    @date		2024-10-31
    @version	0.1
    @copyright	Neil Beresford 2024
 -----------------------------------------------------------------------------
    Notes

--------------------------------------------------------------------------- */

#ifndef _DEFINES_H_
#define _DEFINES_H_

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define SCREENWIDTH           640                       // Screen width
#define SCREENHEIGHT          480                       // Screen height
#define SCREENWIDTH           640                       // Screen width
#define SCREENHEIGHT          480                       // Screen height
#define SCREENSIZE            SCREENWIDTH* SCREENHEIGHT // Size of screen in pixels
#define TOTALSCREENSSIZE      SCREENSIZE * 3 + 64       // Total bytes needed for the screens
#define SCREENCOLORDEPTH      8                         // 256 colours
#define SCREENMODE            $0501                     // Set to 640x480 mode
#define SCREENMODE            $0501                     // Set to 640x480 mode
#define SCREENCLEARCOLOUR     $98989898                 // Set to a light green
#define BACKSCREENWIDTH       SCREENWIDTH * 3           // Back screen width (640*4)
#define BACKSCREENHEIGHT      960                       // Back screen height

// Keycodes

#define KEYCODE_ESC           0x45
#define KEYCODE_1             0x01
#define KEYCODE_2             0x02
#define KEYCODE_3             0x03
#define KEYCODE_4             0x04
#define KEYCODE_5             0x05
#define KEYCODE_6             0x06
#define KEYCODE_7             0x07
#define KEYCODE_8             0x08
#define KEYCODE_9             0x09
#define KEYCODE_0             0x0A
#define KEYCODE_Q             0x10
#define KEYCODE_W             0x11
#define KEYCODE_E             0x12
#define KEYCODE_R             0x13
#define KEYCODE_T             0x14
#define KEYCODE_Y             0x15
#define KEYCODE_U             0x16
#define KEYCODE_I             0x17
#define KEYCODE_O             0x18
#define KEYCODE_P             0x19
#define KEYCODE_A             0x1E
#define KEYCODE_S             0x1F
#define KEYCODE_D             0x20
#define KEYCODE_F             0x21
#define KEYCODE_G             0x22
#define KEYCODE_H             0x23
#define KEYCODE_J             0x24
#define KEYCODE_K             0x25
#define KEYCODE_L             0x26
#define KEYCODE_Z             0x2C
#define KEYCODE_X             0x2D
#define KEYCODE_C             0x2E
#define KEYCODE_V             0x2F
#define KEYCODE_B             0x30
#define KEYCODE_N             0x31
#define KEYCODE_M             0x32
#define KEYCODE_SPACE         0x40
#define KEYCODE_RETURN        0x44
#define KEYCODE_BACKSPACE     0x41
#define KEYCODE_UP            0x4c
#define KEYCODE_DOWN          0x4d
#define KEYCODE_LEFT          0x4f
#define KEYCODE_RIGHT         0x4e
#define KEYCODE_LSHIFT        0x60
#define KEYCODE_RSHIFT        0x61
#define KEYCODE_CTRL          0x63
#define KEYCODE_ALT           0x64
#define KEYCODE_TAB           0x42
#define KEYCODE_CAPS          0x62
#define KEYCODE_LAMIGA        0x66
#define KEYCODE_RAMIGA        0x67
#define KEYCODE_DEL           0x46
#define KEYCODE_HELP          0x5F
#define KEYCODE_F1            0x50
#define KEYCODE_F2            0x51
#define KEYCODE_F3            0x52
#define KEYCODE_F4            0x53
#define KEYCODE_F5            0x54
#define KEYCODE_F6            0x55
#define KEYCODE_F7            0x56
#define KEYCODE_F8            0x57
#define KEYCODE_F9            0x58
#define KEYCODE_F10           0x59
#define KEYCODE_NUMBRACKOPEN  0x5A
#define KEYCODE_NUMBRACKCLOSE 0x5B
#define KEYCODE_NUMSLASH      0x5C
#define KEYCODE_NUMSTAR       0x5D
#define KEYCODE_NUMMINUS      0x4A
#define KEYCODE_NUMPLUS       0x5E
#define KEYCODE_NUMENTER      0x43
#define KEYCODE_NUMPERIOD     0x3C
#define KEYCODE_NUM0          0x0f
#define KEYCODE_NUM7          0x3d
#define KEYCODE_NUM8          0x3e
#define KEYCODE_NUM9          0x3f
#define KEYCODE_NUM4          0x2d
#define KEYCODE_NUM5          0x2e
#define KEYCODE_NUM6          0x2f
#define KEYCODE_NUM1          0x1d
#define KEYCODE_NUM2          0x1e
#define KEYCODE_NUM3          0x1f

//-----------------------------------------------------------------------------

#endif // _DEFINES_H_

//-----------------------------------------------------------------------------
// End of File: Defines.h
//-----------------------------------------------------------------------------