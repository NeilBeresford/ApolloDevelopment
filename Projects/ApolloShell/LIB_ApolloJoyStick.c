/** ---------------------------------------------------------------------------
	@file		LIB_ApolloJoystick.c
	@defgroup 	ApolloSosurce Apollo V4 source
	@brief		Apollo V4 development - Shell
	@date		2025-01-31
 -----------------------------------------------------------------------------
	Notes

--------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "Includes/LIB_ApolloInput.h"

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

/** ---------------------------------------------------------------------------
    @brief 		Handles the Joypad input
    @ingroup 	ApolloSosurce
    @param 		JoypadState		Pointer to the Joypad state
 --------------------------------------------------------------------------- */
 void ApolloJoypad(ApolloJoypadState *JoypadState)
{
	uint16_t * const Joypad_Pointer  = (uint16_t*)0xDFF220;
	uint16_t Joypad_Value;
	
	Joypad_Value = *Joypad_Pointer;

	if ((Joypad_Value & 0x8000) == 0x8000) JoypadState->Joypad_X_Delta = 1;
	else if ((Joypad_Value & 0x4000) == 0x4000) JoypadState->Joypad_X_Delta = -1;
	else JoypadState->Joypad_X_Delta = 0;

	if ((Joypad_Value & 0x2000) == 0x2000) JoypadState->Joypad_Y_Delta = 1;
	else if ((Joypad_Value & 0x1000) == 0x1000) JoypadState->Joypad_Y_Delta = -1;
	else JoypadState->Joypad_Y_Delta = 0;
			
	if ((Joypad_Value & 0x0400) == 0x0400) JoypadState->Joypad_Start = true; else JoypadState->Joypad_Start = false;
	if ((Joypad_Value & 0x0200) == 0x0200) JoypadState->Joypad_Back = true; else JoypadState->Joypad_Back = false;
			
	if ((Joypad_Value & 0x0100) == 0x0100) JoypadState->Joypad_TR = true; else JoypadState->Joypad_TR = false;
	if ((Joypad_Value & 0x0080) == 0x0080) JoypadState->Joypad_TL = true; else JoypadState->Joypad_TL = false;
	if ((Joypad_Value & 0x0040) == 0x0040) JoypadState->Joypad_BR = true; else JoypadState->Joypad_BR = false;
	if ((Joypad_Value & 0x0020) == 0x0020) JoypadState->Joypad_BL = true; else JoypadState->Joypad_BL = false;
	if ((Joypad_Value & 0x0010) == 0x0010) JoypadState->Joypad_Y = true; else JoypadState->Joypad_Y = false;
	if ((Joypad_Value & 0x0008) == 0x0008) JoypadState->Joypad_X = true; else JoypadState->Joypad_X = false;
	if ((Joypad_Value & 0x0004) == 0x0004) JoypadState->Joypad_B = true; else JoypadState->Joypad_B = false;
	if ((Joypad_Value & 0x0002) == 0x0002) JoypadState->Joypad_A = true; else JoypadState->Joypad_A = false;
	if ((Joypad_Value & 0x0001) == 0x0001) JoypadState->Joypad_Connect = true; else JoypadState->Joypad_Connect = false;
}




//-----------------------------------------------------------------------------
// End of file LIB_ApolloJoystick.c
//-----------------------------------------------------------------------------
