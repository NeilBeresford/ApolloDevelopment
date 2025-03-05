/** ---------------------------------------------------------------------------
    @file		LIB_ApolloKeyboard.c
    @defgroup 	AmiWorms Apollo V4 source
    @brief		Apollo V4 development - Shell
    @date		2025-01-31
 -----------------------------------------------------------------------------
    Notes

--------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "../Includes/LIB_ApolloInput.h"

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

/** ---------------------------------------------------------------------------
    @brief 		Handles the Keyboard input
    @ingroup 	AmiWorms
    @param 		KeyboardState		Pointer to the Keyboard state
 --------------------------------------------------------------------------- */
void ApolloKeyboard( ApolloKeyBoardState* KeyboardState )
{
    uint8_t Keyboard_Raw;

    Keyboard_Raw = Hardware_ReadKey(); // Read the keyboard

    if ( Keyboard_Raw != NOKEY )
    {
        if ( Keyboard_Raw & 0x80 )
        {
            KeyboardState->Keys[ Keyboard_Raw & 0x7F ]          = 0;
            KeyboardState->KeysDown[ Keyboard_Raw & 0x7F ]      = 0;
            KeyboardState->KeysProcessed[ Keyboard_Raw & 0x7F ] = 0;
            KeyboardState->Previous_Key                         = Keyboard_Raw & 0x7F;
        }
        else
        {
            KeyboardState->Keys[ Keyboard_Raw ]     = 1;
            KeyboardState->KeysDown[ Keyboard_Raw ] = 1;
            KeyboardState->Current_Key              = Keyboard_Raw;
        }
    }
    else
    {
        KeyboardState->Current_Key = NOKEY;
    }
}

/** ---------------------------------------------------------------------------
    @brief 		Clears the Keyboard state
    @ingroup 	AmiWorms
    @param 		KeyboardState		Pointer to the Keyboard state
 --------------------------------------------------------------------------- */
void ApolloKeyboardClear( ApolloKeyBoardState* KeyboardState )
{
    for ( int32_t nI = 0; nI < 128; nI++ )
    {
        KeyboardState->Keys[ nI ]          = 0;
        KeyboardState->KeysDown[ nI ]      = 0;
        KeyboardState->KeysProcessed[ nI ] = 0;
    }
}


//-----------------------------------------------------------------------------
// End of file LIB_ApolloKeyboard.c
//-----------------------------------------------------------------------------
