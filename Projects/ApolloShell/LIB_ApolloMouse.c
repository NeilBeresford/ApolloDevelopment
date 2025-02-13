/** ---------------------------------------------------------------------------
	@file		LIB_ApolloMouse.c
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
    @brief 		Handles the Mouse input
    @ingroup 	ApolloSosurce
    @param 		MouseState		Pointer to the Mouse state
 --------------------------------------------------------------------------- */
void ApolloMouse(ApolloMouseState *MouseState)
{
	uint8_t MouseButtonLeft_Value;	
	uint16_t MouseButtonRight_Value;	
	uint16_t MouseButtonMiddle_Value;

	// Initialize Mouse Buttons
	MouseState->Button_Left = false;
	MouseState->Button_Right = false;
	MouseState->Button_Middle = false;
	
	// Read Mouse Buttons 
	MouseButtonLeft_Value = *((volatile uint8_t*)APOLLO_MOUSE_BUTTON1);
	if ((MouseButtonLeft_Value & 0x40) == 0) MouseState->Button_Left = true;
	MouseButtonRight_Value= *((volatile uint16_t*)APOLLO_MOUSE_BUTTON2);
	if ((MouseButtonRight_Value & 0x400) == 0) MouseState->Button_Right = true;
	MouseButtonMiddle_Value= *((volatile uint16_t*)APOLLO_MOUSE_BUTTON3);
	if ((MouseButtonMiddle_Value & 0x400) == 0) MouseState->Button_Middle = true;

	// Read Mouse Movement	
	MouseState->MouseX_Value 		= *((signed char *)APOLLO_MOUSE_GET_X);
	MouseState->MouseY_Value 		= *((signed char *)APOLLO_MOUSE_GET_Y);
	MouseState->MouseX_Value_Delta 	= MouseState->MouseX_Value - MouseState->MouseX_Value_Old;
	MouseState->MouseY_Value_Delta 	= MouseState->MouseY_Value - MouseState->MouseY_Value_Old;
	MouseState->MouseX_Value_Old 	= MouseState->MouseX_Value;
	MouseState->MouseY_Value_Old 	= MouseState->MouseY_Value;

	// Correct Delta for BYTE overflow
	if (MouseState->MouseX_Value_Delta < -128) MouseState->MouseX_Value_Delta += 256;
	if (MouseState->MouseX_Value_Delta >  128) MouseState->MouseX_Value_Delta -= 256;
	if (MouseState->MouseY_Value_Delta < -128) MouseState->MouseY_Value_Delta += 256;
	if (MouseState->MouseY_Value_Delta >  128) MouseState->MouseY_Value_Delta -= 256;	

	// Check for Mouse Screen Boundaries
	if (MouseState->MouseX_Value_Delta != 0 || MouseState->MouseY_Value_Delta != 0)
	{
		if (MouseState->MouseX_Pointer + MouseState->MouseX_Value_Delta < 1)
		{
			MouseState->MouseX_Pointer = 1;
		} else {
			if (MouseState->MouseX_Pointer + MouseState->MouseX_Value_Delta > MouseState->MouseX_Pointer_Max)
			{
				MouseState->MouseX_Pointer = MouseState->MouseX_Pointer_Max - 1;
			} else {
				MouseState->MouseX_Pointer += MouseState->MouseX_Value_Delta;
			}
		}	

		if (MouseState->MouseY_Pointer + MouseState->MouseY_Value_Delta <1)
		{
			MouseState->MouseY_Pointer = 1;
		} else {
			if (MouseState->MouseY_Pointer + MouseState->MouseY_Value_Delta >  MouseState->MouseY_Pointer_Max)
			{
				MouseState->MouseY_Pointer = MouseState->MouseY_Pointer_Max - 1;
			} else {
				MouseState->MouseY_Pointer += MouseState->MouseY_Value_Delta;	
			}
		}
	}
	// Translate Mouse Buttons to Mouse ButtonState
	MouseState->Button_State = 0;

	if( MouseState->Button_Left_Count > 0 ) MouseState->Button_Left_Count--;
	if( MouseState->Button_Right_Count > 0 ) MouseState->Button_Right_Count--;
	if( MouseState->Button_Middle_Count > 0 ) MouseState->Button_Middle_Count--;

	if( MouseState->Button_Left == false && MouseState->Button_Left_Old == true )
	{
		if( MouseState->Button_Left_Count == 0 )
		{
			MouseState->Button_State |= APOLLOMOUSE_LEFTCLICK;
			MouseState->Button_Left_Count = APOLLOMOUSE_DOUBLECLICKCOUNTER;
		}
		else
		{
			MouseState->Button_State |= APOLLOMOUSE_LEFTDOUBLECLICK;
		}
	}

	if( MouseState->Button_Left == true )
	{
		MouseState->Button_State |= APOLLOMOUSE_LEFTDOWN;
	}
	
	if( MouseState->Button_Right == false && MouseState->Button_Right_Old == true )
	{
		if( MouseState->Button_Right_Count == 0 )
		{
			MouseState->Button_State |= APOLLOMOUSE_RIGHTCLICK;
			MouseState->Button_Right_Count = APOLLOMOUSE_DOUBLECLICKCOUNTER;
		}
		else
		{
			MouseState->Button_State |= APOLLOMOUSE_RIGHTDOUBLECLICK;
		}
	}

	if( MouseState->Button_Right == true )
	{
		MouseState->Button_State |= APOLLOMOUSE_RIGHTDOWN;
	}

	if( MouseState->Button_Middle == false && MouseState->Button_Middle_Old == true )
	{
		if( MouseState->Button_Middle_Count == 0 )
		{
			MouseState->Button_State |= APOLLOMOUSE_MIDDLECLICK;
			MouseState->Button_Middle_Count = APOLLOMOUSE_DOUBLECLICKCOUNTER;
		}
		else
		{
			MouseState->Button_State |= APOLLOMOUSE_MIDDLEDOUBLECLICK;
		}
	}

	if( MouseState->Button_Middle == true )
	{
		MouseState->Button_State |= APOLLOMOUSE_MIDDLEDOWN;
	}

	MouseState->Button_Left_Old = MouseState->Button_Left;
	MouseState->Button_Right_Old = MouseState->Button_Right;
	MouseState->Button_Middle_Old = MouseState->Button_Middle;

	return;
}



//-----------------------------------------------------------------------------
// End of file LIB_ApolloMouse.c
//-----------------------------------------------------------------------------
