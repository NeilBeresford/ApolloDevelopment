/** ---------------------------------------------------------------------------
	@file		LIB_SprManager.c
	@defgroup 	MainShell Apollo V4 Shell
	@brief		Apollo V4 development - Shell
	@copyright	Neil Beresford 2024
 -----------------------------------------------------------------------------
	Notes

--------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "stdint.h"
#include "stdbool.h"
#include "Includes/FlagStruct.h"
#include "Includes/defines.h"
#include "Includes/LIB_Sprites.h"
#include "Includes/LIB_SprManager.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define TOTAL_SPRITES		( 256 )
#define ON					( 1 )
#define OFF					( 0 )

//-----------------------------------------------------------------------------
// Typedefs and enums
//-----------------------------------------------------------------------------

typedef enum
{
	SPR_CMD_NONE = 0,
	SPR_CMD_MOVE,
	SPR_CMD_ROTATE,
	SPR_CMD_ANIM,
	SPR_CMD_FRAME,
	SPR_CMD_END

} eSPRCMD;

typedef enum
{
	SPR_ANIM_NONE = 0,
	SPR_ANIM_LOOP,
	SPR_ANIM_PINGPONG,
	SPR_ANIM_ONCE

} eSPRANIMTYPE;

// Callbacks
typedef void (*fnSprControl)(PSPRITE pSprite);

typedef struct
{
    uint16_t	X;
	uint16_t    Y;

} POSITION, *POSITION;

typedef struct
{
	uint16_t	X;
	uint16_t	Y;
	uint16_t	W;
	uint16_t	H;

} RECT, *RECT;

typedef union
{
	struct
	{
		uint32_t    Active		: 1;
		uint32_t    OnScreen	: 1;
		uint32_t    Paused		: 1;
		uint32_t    DeleteMe	: 1;
		uint32_t    Collidable	: 1;
		uint32_t    Visible		: 1;
		uint32_t    Animated	: 1;
		uint32_t    Reserved	: 25;
	};

	uint32_t	Flags;

} SPRFLAG,*PSPRFLAG;

typedef union
{
	struct
	{
		uint16_t    Active : 1;
		uint16_t    Looping : 1;
		uint16_t    Reserved : 14;

	};

	uint16_t    Flags;

} SPRFRAMEFLAGS, * PSPRFRAMEFLAGS;

typedef struct
{
	SPRFRAMEFLAGS	FrameFlags;

	// Reference Frame Information	
	uint16_t		FrameID;
	uint16_t		FrameCMD;
	uint16_t		FrameData[ 2 ];
	POSITION		Positions[ 2 ];			
	uint16_t		FrameCount;
	uint16_t		FrameDelay;

	// Working Frame Data
	uint16_t		FrameCurCount;
	uint16_t		FrameCurDelay;
	POSITION		FrameCurPos;

} SPRFRAME, *PSPRFRAME;


typedef struct
{
	uint16_t	AnimID;
	uint16_t	AnimType;
	uint16_t	AnimFrames;
	uint16_t	AnimCurFrame;
	SPRFRAME*	pFrames;

} SPRANIM, * PSPRANIM;

typedef struct
{
	// General Sprite Information
	uint8_t		SprID;
	uint8_t     SprGroup;
	SPRFLAG		SprFlags;
	uint32_t	SprResourceID;
	uint16_t	ScreenX;
	uint16_t	ScreenY;
	float		fWorldX;
	float		fWorldY;
	uint16_t	SprWidth;
	uint16_t	SprHeight;
	uint8_t		SprZ;	

	// Animation Information
	PSPRANIM	pAnimData;
	uint16_t	CurAnimIndex;

	// Collision Information
	RECT		CollisionRect;

	// Movement Information
	float		fMoveSpeed;
	float		fMoveAngle;
	float		fMoveX;
	float		fMoveY;

	// Control Callbacks
	fnSprControl	fnControl;

} SPRITE,*PSPRITE;

typedef struct
{
	FlagStruct_t	sFlags;
	uint16_t		SprCount;
	SPRITE			Sprites[ TOTAL_SPRITES ];

} SPRITEMANAGER, *PSPRITEMANAGER;

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

SPRITEMANAGER sSprMgr = { .sFlags.Flags = 0 };

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

/** ---------------------------------------------------------------------------
	@brief		Initialise the sprite manager
	@param		None
	@return		None
--------------------------------------------------------------------------- */
void LIB_SprManager_Init(void)
{
	if ( sSprMgr.sFlags.Initialized == OFF )
	{
		// Clear the sprite manager
		sSprMgr.SprCount			= 0;
		sSprMgr.sFlags.Initialized	= ON;

		// Clear all sprites
		for (uint16_t i = 0; i < TOTAL_SPRITES; i++)
		{
			sSprMgr.Sprites[i] = { 0 };
		}

	}
}

/** ---------------------------------------------------------------------------
	@brief		Add Sprite to the manager
	@param		nResourceID - Resource ID of the sprite (from LIB_Sprites)
				nX			- X position of the sprite
				nY			- Y position of the sprite
				nGroup		- Group ID of the sprite
				nZ			- Z order of the sprite
				fnControl	- Control function for the sprite
	@return		bool		- True if successful
---------------------------------------------------------------------------- */
bool LIB_SprManager_Add(uin32_t nResourceID, uint16_t nX, uint16_t nY, uint16_t nGroup, uint16_t nZ, fnSprControl fnControl)
{
	bool bResult = false;

	if (sSprMgr.sFlags.Initialized == ON)
	{
		if (sSprMgr.SprCount < TOTAL_SPRITES)
		{
			// Find a free sprite
			for (uint16_t i = 0; i < TOTAL_SPRITES; i++)
			{
				if (sSprMgr.Sprites[i].SprFlags.Active == OFF)
				{
					// Set the sprite data
					sSprMgr.Sprites[i].SprID = i;
					sSprMgr.Sprites[i].SprGroup = nGroup;
					sSprMgr.Sprites[i].SprResourceID = nResourceID;
					sSprMgr.Sprites[i].ScreenX = nX;
					sSprMgr.Sprites[i].ScreenY = nY;
					sSprMgr.Sprites[i].SprZ = nZ;
					sSprMgr.Sprites[i].fnControl = fnControl;

					// Set the sprite flags
					sSprMgr.Sprites[i].SprFlags.Active = ON;
					sSprMgr.Sprites[i].SprFlags.OnScreen = ON;
					sSprMgr.Sprites[i].SprFlags.Visible = ON;

					// Increment the sprite count
					sSprMgr.SprCount++;

					// Set the result
					bResult = true;

					// Break out of the loop
					break;
				}
			}
		}
	}

	return bResult;
}

/** ---------------------------------------------------------------------------
	@brief		Remove a sprite from the manager
	@param		uint16_t nSpriteID
	@return		None
 --------------------------------------------------------------------------- */
void LIB_SprManager_Update(void)
{
	if (sSprMgr.sFlags.Initialized == ON)
	{
		for (uint16_t i = 0; i < TOTAL_SPRITES; i++)
		{
			if (sSprMgr.Sprites[i].SprFlags.Active == ON)
			{
				if (sSprMgr.Sprites[i].fnControl != NULL)
				{
					sSprMgr.Sprites[i].fnControl(&sSprMgr.Sprites[i]);
				}
			}
		}
	}
}

/** ---------------------------------------------------------------------------
	@brief		Draw all sprites
	@param		None
	@return		None
 --------------------------------------------------------------------------- */
void LIB_SprManager_Draw(void)
{
	if (sSprMgr.sFlags.Initialized == ON)
	{
		for (uint16_t i = 0; i < TOTAL_SPRITES; i++)
		{
			if (sSprMgr.Sprites[i].SprFlags.Active == ON)
			{
				if (sSprMgr.Sprites[i].SprFlags.Visible == ON)
				{
					LIB_Sprites_Draw(sSprMgr.Sprites[i].SprResourceID, sSprMgr.Sprites[i].ScreenX, sSprMgr.Sprites[i].ScreenY);
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
// End of file: LIB_SprManager.c
//-----------------------------------------------------------------------------

