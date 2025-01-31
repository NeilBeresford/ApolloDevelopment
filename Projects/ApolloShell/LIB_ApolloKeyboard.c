/** ---------------------------------------------------------------------------
	@file		LIB_ApolloKeyboard.c
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
    @brief 		Handles the Keyboard input
    @ingroup 	ApolloSosurce
    @param 		KeyboardState		Pointer to the Keyboard state
 --------------------------------------------------------------------------- */
void ApolloKeyboard(ApolloKeyBoardState *KeyboardState)
{
	uint8_t* const 	Keyboard_Pointer = (uint8_t*)0xBFEC01; 
	uint8_t			Keyboard_Raw, Keyboard_Now;

	Keyboard_Raw = *Keyboard_Pointer;														// retrieve RAW value from register
	Keyboard_Raw = ~Keyboard_Raw;															// not.b
	KeyboardState->Current_Key = (uint8_t) ((Keyboard_Raw>>1) | (Keyboard_Raw<<7));			// ror.b #1

	if ((KeyboardState->Current_Key < 127) && (KeyboardState->Current_Key!=KeyboardState->Previous_Key))
	{
		KeyboardState->Previous_Key = KeyboardState->Current_Key;							// Use only "Low" KB values (0-127)


	} else {
		KeyboardState->Current_Key = 127;
	}
}

//-----------------------------------------------------------------------------
// End of file LIB_ApolloKeyboard.c
//-----------------------------------------------------------------------------
