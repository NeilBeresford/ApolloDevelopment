/** ---------------------------------------------------------------------------
    @file		LIB_ApolloInput.h
    @defgroup 	AmiWorms Apollo V4 source
    @brief		Apollo V4 development - Shell
    @date		2025-01-31
 -----------------------------------------------------------------------------
    Notes

--------------------------------------------------------------------------- */

#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "stdint.h"
#include "stdbool.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define APOLLO_POINTER_SET_X           0xDFF1D0
#define APOLLO_POINTER_SET_Y           0xDFF1D2
#define APOLLO_POINTER_GET_X           0xDFE1D0
#define APOLLO_POINTER_GET_Y           0xDFE1D2

#define APOLLO_MOUSE_GET_X             0xDFF00B
#define APOLLO_MOUSE_GET_Y             0xDFF00A
#define APOLLO_MOUSE_BUTTON1           0xBFE001
#define APOLLO_MOUSE_BUTTON2           0xDFF016
#define APOLLO_MOUSE_BUTTON3           0xDFF016 // TODO

#define APOLLO_POINTER_COL1            0xDFF3A2
#define APOLLO_POINTER_COL2            0xDFF3A4
#define APOLLO_POINTER_COL3            0xDFF3A6

#define APOLLO_POINTER_RED             0x0A00
#define APOLLO_POINTER_GREEN           0x00A0
#define APOLLO_POINTER_BLUE            0x000A

#define APOLLOMOUSE_DOUBLECLICKCOUNTER 15
#define APOLLOMOUSE_LEFTCLICK          0x0001
#define APOLLOMOUSE_RIGHTCLICK         0x0002
#define APOLLOMOUSE_MIDDLECLICK        0x0004
#define APOLLOMOUSE_LEFTDOUBLECLICK    0x0008
#define APOLLOMOUSE_RIGHTDOUBLECLICK   0x0010
#define APOLLOMOUSE_MIDDLEDOUBLECLICK  0x0020
#define APOLLOMOUSE_LEFTDOWN           0x0040
#define APOLLOMOUSE_RIGHTDOWN          0x0080
#define APOLLOMOUSE_MIDDLEDOWN         0x0100

#define NOKEY                          127

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------

typedef struct
{
    uint8_t Current_Key;
    uint8_t Previous_Key;

    uint8_t Keys[ 0x80 ];
    uint8_t KeysDown[ 0x80 ];
    uint8_t KeysProcessed[ 0x80 ];

} ApolloKeyBoardState;

typedef struct
{
    int8_t Joypad_X_Delta;
    int8_t Joypad_Y_Delta;
    bool   Joypad_Start;
    bool   Joypad_Back;
    bool   Joypad_TR;
    bool   Joypad_TL;
    bool   Joypad_BR;
    bool   Joypad_BL;
    bool   Joypad_Y;
    bool   Joypad_X;
    bool   Joypad_B;
    bool   Joypad_A;
    bool   Joypad_Connect;
    bool   Joypad_AActioned;
    bool   Joypad_BActioned;
    bool   Joypad_XActioned;
    bool   Joypad_YActioned;

} ApolloJoypadState;

typedef struct
{
    int16_t  MouseX_Pointer;
    int16_t  MouseY_Pointer;
    int16_t  MouseX_Pointer_Max;
    int16_t  MouseY_Pointer_Max;

    int16_t  MouseX_Value;
    int16_t  MouseY_Value;
    int16_t  MouseX_IntValue;
    int16_t  MouseY_IntValue;
    int16_t  MouseX_Value_Old;
    int16_t  MouseY_Value_Old;
    int16_t  MouseX_Value_Delta;
    int16_t  MouseY_Value_Delta;

    bool     Button_Left; // Actual state for Buttons
    bool     Button_Right;
    bool     Button_Middle;

    bool     Button_Left_Old; // Previous state for Buttons
    bool     Button_Right_Old;
    bool     Button_Middle_Old;

    uint16_t Button_State; // Interpreted Button Action (see table below)
    uint16_t Button_Left_Count;
    uint16_t Button_Right_Count;
    uint16_t Button_Middle_Count;
} ApolloMouseState;

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

void ApolloMouse( ApolloMouseState* MouseState );
void ApolloKeyboard( ApolloKeyBoardState* KeyboardState );
void ApolloJoypad( ApolloJoypadState* JoypadState );

//-----------------------------------------------------------------------------
// End of file LIB_ApolloInput.h
//-----------------------------------------------------------------------------
