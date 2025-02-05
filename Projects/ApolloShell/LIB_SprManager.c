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
#define YES					( 1 )
#define NO					( 0 )

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

typedef union
{
	struct
	{
		uint16_t    Initialized : 1;
		uint16_t    Assigned : 1;
		uint16_t    Locked : 1;
		uint16_t    Reserved : 13;

	};

	uint16_t    Flags;

} SPRHANDLEFLAGS, * PSPRHANDLEFLAGS;

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
	uint8_t			SprID;
	uint8_t			SprGroup;
	SPRFLAG			SprFlags;
	uint32_t		SprResourceID;
	uint16_t		ScreenX;
	uint16_t		ScreenY;
	float			fWorldX;
	float			fWorldY;
	uint16_t		SprWidth;
	uint16_t		SprHeight;
	uint8_t			SprZ;	

	// Animation Information
	PSPRANIM		pAnimData;
	uint16_t		CurAnimIndex;

	// Collision Information
	RECT			CollisionRect;

	// Movement Information
	float			fMoveSpeed;
	float			fMoveAngle;
	float			fMoveX;
	float			fMoveY;

	// Control Callbacks
	fnSprControl	fnControl;

} SPRITE,*PSPRITE;

typedef struct
{
	SPRHANDLEFLAGS	Flags;
	uint16_t		SprIndex;
	
} SPRHANDLE, *PSPRHANDLE;

typedef struct
{
	FlagStruct_t	sFlags;
	uint16_t		SprCount;
	SPRITE			Sprites[ TOTAL_SPRITES ];
	SPRHANDLE		SprHandles[ TOTAL_SPRITES ];
	
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
	@ingroup	MainShell
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

		// Clear all sprites and handles
		for (uint16_t i = 0; i < TOTAL_SPRITES; i++)
		{
			sSprMgr.Sprites[i]						= { 0 };
			sSprMgr.SprHandles[i]					= { 0 };
			sSprMgr.SprHandles[i].Flags.Initialized = YES;			
		}
	}
}

/** ---------------------------------------------------------------------------
	@brief		Add Sprite to the manager
	@param		nResourceID - Resource ID of the sprite (from LIB_Sprites)
	@param		nX			- X position of the sprite
	@param		nY			- Y position of the sprite
	@param		nGroup		- Group ID of the sprite
	@param		nZ			- Z order of the sprite
	@param		fnControl	- Control function for the sprite (or NULL)
	@return		PSPRHANDLE	- Pointer to a controlled sprite details for
							  recall and control.
---------------------------------------------------------------------------- */
PSPRHANDLE LIB_SprManager_Add(uint32_t nResourceID, uint16_t nX, uint16_t nY, uint16_t nGroup, uint16_t nZ, fnSprControl fnControl)
{
	uint16_t rSprIndex = SPRADDFAILERROR;

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

// function to store the animation frames for a sprite

/** ---------------------------------------------------------------------------
	@brief		Add an animation to a sprite
	@ingroup	MainShell
	@param		pSprHandle 		
	@param		AnimID
	@param		AnimType
	@param		AnimFrames
	@param		pFrameData
	@return		bool
---------------------------------------------------------------------------- */
bool LIB_SprManager_AddAnim( PSPRHANDLE pSprHandle, uint16_t nAnimID, uint16_t nAnimType, uint16_t nAnimFrames, uint16_t* pFrameData)
{
	bool bResult = false;

	if (sSprMgr.sFlags.Initialized == ON)
	{
		if ( pSprHandle && pSprHandle->nSprIndex < TOTAL_SPRITES)
		{
			uint16_t	nSpriteID = pSprHandle->nSprIndex;
			PSPRITE		pSprite = &sSprMgr.Sprites[nSpriteID];

			if (sSprMgr.Sprites[nSpriteID].SprFlags.Active == ON)
			{
				if (sSprMgr.Sprites[nSpriteID].pAnimData == NULL)
				{
					// Allocate memory for the animation data
					sSprMgr.Sprites[nSpriteID].pAnimData = (PSPRANIM)malloc(sizeof(SPRANIM));

					if (sSprMgr.Sprites[nSpriteID].pAnimData != NULL)
					{
						// Set the animation data
						sSprMgr.Sprites[nSpriteID].pAnimData->AnimID = nAnimID;
						sSprMgr.Sprites[nSpriteID].pAnimData->AnimType = nAnimType;
						sSprMgr.Sprites[nSpriteID].pAnimData->AnimFrames = nAnimFrames;
						sSprMgr.Sprites[nSpriteID].pAnimData->AnimCurFrame = 0;

						// Allocate memory for the frame data
						sSprMgr.Sprites[nSpriteID].pAnimData->pFrames = (PSPRFRAME)malloc(sizeof(SPRFRAME) * nAnimFrames);

						if (sSprMgr.Sprites[nSpriteID].pAnimData->pFrames != NULL)
						{
							// Set the frame data
							for (uint16_t i = 0; i < nAnimFrames; i++)
							{
								sSprMgr.Sprites[nSpriteID].pAnimData->pFrames[i].FrameID = pFrameData[i];
							}

							// Set the result
							bResult = true;
						}
					}
				}
			}
		}
	}

	return bResult;
}

/** ---------------------------------------------------------------------------
	@brief		Remove a sprite from the manager
	@ingroup	MainShell
	@param		uint16_t nSpriteID
	@return		None
 --------------------------------------------------------------------------- */
void LIB_SprManager_Update(void)
{
	if (sSprMgr.sFlags.Initialized == ON)
	{
		for (uint16_t i = 0, cnt = 0; i < TOTAL_SPRITES && cnt != sSprMgr.SprCount; i++)
		{
			if (sSprMgr.Sprites[i].SprFlags.Active == ON)
			{
				if (sSprMgr.Sprites[i].fnControl != NULL)
				{
					sSprMgr.Sprites[i].fnControl(&sSprMgr.Sprites[i]);
				}
				cnt++;
			}
		}
	}
}

/** ---------------------------------------------------------------------------
	@brief		Draw all sprites
	@ingroup	MainShell
	@param		None
	@return		None
 --------------------------------------------------------------------------- */
void LIB_SprManager_Draw(void)
{
	if (sSprMgr.sFlags.Initialized == ON)
	{
		for (uint16_t i = 0, cnt = 0; i < TOTAL_SPRITES && cnt != sSprMgr.SprCount ; i++ )
		{
			if (sSprMgr.Sprites[i].SprFlags.Active == ON)
			{
				if (sSprMgr.Sprites[i].SprFlags.Visible == ON)
				{
					LIB_Sprites_Draw(sSprMgr.Sprites[i].SprResourceID, sSprMgr.Sprites[i].ScreenX, sSprMgr.Sprites[i].ScreenY);
				}
				cnt++
			}
		}
	}
}



//-----------------------------------------------------------------------------
// Internal Functionality
//-----------------------------------------------------------------------------

/** ---------------------------------------------------------------------------
	@brief		Find a free sprite handle
	@ingroup	MainShell
	@param		None
	@return		PSPRHANDLE of 
 --------------------------------------------------------------------------- */
PSPRHANDLE LIB_SprManager_GetHandle(uint16_t nSpriteID)
{
	PSPRHANDLE pResult = NULL;

	if (sSprMgr.sFlags.Initialized == ON)
	{
		if (nSpriteID < TOTAL_SPRITES)
		{
			if ((sSprMgr.Sprites[nSpriteID].SprFlags.Active == ON) && (sSprMgr.SprHandles[nSpriteID].Flags.Assigned == NO) )
			{
				sSprMgr.SprHandles[nSpriteID].Flags.Assigned	= YES;
				sSprMgr.SprHandles[nSpriteID].Flags.Locked		= NO;
				sSprMgr.SprHandles[nSpriteID].nSprIndex			= nSpriteID;
				pResult = &sSprMgr.SprHandles[nSpriteID];
			}
		}
	}

	return pResult;
}


/** ---------------------------------------------------------------------------
	@brief		Find a free sprite handle
	@ingroup	MainShell
	@param		None
	@return		PSPRHANDLE - Returns a pointer to a free sprite handle or
				NULL if none are available
 --------------------------------------------------------------------------- */
PSPRHANDLE LIB_SprManager_FindFreeHandle(void)
{
	PSPRHANDLE pResult = NULL;

	if (sSprMgr.sFlags.Initialized == ON)
	{
		PSPRHANDLE pHandle = sSprMgr.SprHandles;

		for (uint16_t i = 0; i < TOTAL_SPRITES; i++, pHandle++)
		{
			if (pHandle->Flags.Assigned == NO)
			{
				pHandle->Flags.Assigned = YES;
				pResult					= pHandle;
				
				break;
			}
		}
	}

	return pResult;
}

// compress the sprite manager to remove any gaps in the sprite array

/** ---------------------------------------------------------------------------
	@brief		Compress the sprite manager
	@ingroup	MainShell
	@param		None
	@return		None
 --------------------------------------------------------------------------- */
void LIB_SprManager_Compress(void)
{
	if (sSprMgr.sFlags.Initialized == ON)
	{
		uint16_t nCount = 0;

		for (uint16_t i = 0; i < TOTAL_SPRITES; i++)
		{
			if (sSprMgr.Sprites[i].SprFlags.Active == ON)
			{
				if (i != nCount)
				{
					sSprMgr.Sprites[nCount] = sSprMgr.Sprites[i];
					sSprMgr.Sprites[i].SprFlags.Active = OFF;

					// Update the sprite handle		
					LIB_SprManager_GetHandle(sSprMgr.Sprites[i].SprID)->SprIndex = nCount;
				}
				nCount++;
			}
		}
	}
}

/** ---------------------------------------------------------------------------
	@brief		Compress the sprite handles
	@ingroup	MainShell
	@param		None
	@return		None
 --------------------------------------------------------------------------- */
void LIB_SprManager_CompressHandles(void)
{
	if (sSprMgr.sFlags.Initialized == ON)
	{
		uint16_t nCount = 0;

		for (uint16_t i = 0; i < TOTAL_SPRITES; i++)
		{
			if (sSprMgr.SprHandles[i].Flags.Assigned == YES)
			{
				if (i != nCount)
				{
					sSprMgr.SprHandles[nCount] = sSprMgr.SprHandles[i];
					sSprMgr.SprHandles[i].Flags.Assigned = NO;
				}
				nCount++;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// End of file: LIB_SprManager.c
//-----------------------------------------------------------------------------

