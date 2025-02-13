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

#include "stdlib.h"
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
// Forward prototypesON
//-----------------------------------------------------------------------------

PSPRHANDLE LIB_SprManager_FindFreeHandle(void);

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
		PSPRITE 	pSpr 	= sSprMgr.Sprites;
		PSPRHANDLE 	pHandle = sSprMgr.SprHandles;

		// Clear the sprite manager
		sSprMgr.SprCount			= 0;
		sSprMgr.sFlags.Initialized	= YES;

		// Clear all sprites and handles
		for (uint16_t i = 0; i < TOTAL_SPRITES; i++,pSpr++,pHandle++)
		{
			pSpr->SprFlags.Flags 		= 0;
			pHandle->SprIndex 			= 0;
			pHandle->Flags.Flags 		= 0;
			pHandle->Flags.Initialized 	= YES;			
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
	PSPRHANDLE rHandle = NULL;

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
					sSprMgr.Sprites[i].SprWidth = LIB_Sprites_GetWidth(nResourceID);
					sSprMgr.Sprites[i].SprHeight = LIB_Sprites_GetHeight(nResourceID);
					sSprMgr.Sprites[i].fnControl = fnControl;

					// Set the sprite flags
					sSprMgr.Sprites[i].SprFlags.Active = ON;
					sSprMgr.Sprites[i].SprFlags.OnScreen = ON;
					sSprMgr.Sprites[i].SprFlags.Visible = ON;

					// Incremprent the sprite count
					sSprMgr.SprCount++;

					// Set the result
					rHandle = LIB_SprManager_FindFreeHandle();
					rHandle->SprIndex = i;
					rHandle->Flags.Assigned = ON;
					// Break out of the loop
					break;
				}
			}
		}
	}

	return rHandle;
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
		if ( pSprHandle && pSprHandle->SprIndex < TOTAL_SPRITES)
		{
			uint16_t	nSpriteID = pSprHandle->SprIndex;
			PSPRITE		pSprite = &sSprMgr.Sprites[nSpriteID];

			if (pSprite->SprFlags.Active == ON)
			{
				// Allocate memory for the animation data
				if (pSprite->pAnimData == NULL)
					pSprite->pAnimData = (PSPRANIM)malloc(sizeof(SPRANIM));

				// Set the animation data
				pSprite->pAnimData->AnimID = nAnimID;
				pSprite->pAnimData->AnimType = nAnimType;
				pSprite->pAnimData->AnimFrames = nAnimFrames;
				pSprite->pAnimData->AnimCurFrame = 0;
				pSprite->SprFlags.Animated = ON;

				if (pSprite->pAnimData->pFrames != NULL)
				{
					// Allocate memory for the frame data
					pSprite->pAnimData->pFrames = (PSPRFRAME)malloc(sizeof(SPRFRAME) * nAnimFrames);
					// Set the frame data
					for (uint16_t i = 0; i < nAnimFrames; i++)
					{
						pSprite->pAnimData->pFrames[i].FrameID = pFrameData[i];
					}

					// Set the result
					bResult = true;
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
		PSPRITE		pSprite = sSprMgr.Sprites;
		for (uint16_t i = 0, cnt = 0; i < TOTAL_SPRITES && cnt != sSprMgr.SprCount; i++)
		{
			if (sSprMgr.Sprites[i].SprFlags.Active == ON)
			{
				
				if ( sSprMgr.Sprites[i].SprFlags.Animated == ON )
				{
					// Update the animation
					if (sSprMgr.Sprites[i].pAnimData != NULL)
					{
						if (sSprMgr.Sprites[i].pAnimData->AnimType == SPR_ANIM_LOOP)
						{
							sSprMgr.Sprites[i].pAnimData->AnimCurFrame++;
							if (sSprMgr.Sprites[i].pAnimData->AnimCurFrame >= sSprMgr.Sprites[i].pAnimData->AnimFrames)
							{
								sSprMgr.Sprites[i].pAnimData->AnimCurFrame = 0;
							}

							// Set the frame
							sSprMgr.Sprites[i].SprNum = sSprMgr.Sprites[i].pAnimData->AnimCurFrame;
						}
					}
				}
				
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
					if ( sSprMgr.Sprites[i].SprFlags.Flipped == ON )
						LIB_Sprites_DrawFlipped(sSprMgr.Sprites[i].SprResourceID, sSprMgr.Sprites[i].SprNum, sSprMgr.Sprites[i].ScreenX - (sSprMgr.Sprites[i].SprWidth/2) , sSprMgr.Sprites[i].ScreenY - (sSprMgr.Sprites[i].SprHeight/2));
					else
						LIB_Sprites_Draw(sSprMgr.Sprites[i].SprResourceID, sSprMgr.Sprites[i].SprNum, sSprMgr.Sprites[i].ScreenX - (sSprMgr.Sprites[i].SprWidth/2) , sSprMgr.Sprites[i].ScreenY - (sSprMgr.Sprites[i].SprHeight/2));
				}
				cnt++;
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
			if ((sSprMgr.Sprites[ nSpriteID ].SprFlags.Active == ON) && (sSprMgr.SprHandles[nSpriteID].Flags.Assigned == NO) )
			{
				sSprMgr.SprHandles[nSpriteID].Flags.Assigned	= YES;
				sSprMgr.SprHandles[nSpriteID].Flags.Locked		= NO;
				sSprMgr.SprHandles[nSpriteID].SprIndex			= nSpriteID;
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

/** ---------------------------------------------------------------------------
	@brief		Returns the total number of frames in a sprite
	@ingroup	MainShell
	@param		pSprHandle 	- Pointer to the sprite handle
	@return		uint32_t 	- Number of frames
 --------------------------------------------------------------------------- */
uint32_t LIB_SprManager_GetTotalFrames( PSPRHANDLE pSprHandle )
{
	uint32_t nFrames = 0;

	if (sSprMgr.sFlags.Initialized == ON)
	{
		if (pSprHandle && pSprHandle->SprIndex < TOTAL_SPRITES)
		{
			nFrames = LIB_Sprites_GetFrames( sSprMgr.Sprites[pSprHandle->SprIndex].SprResourceID );
		}
	}

	return nFrames;
}

/** ---------------------------------------------------------------------------
	@brief		Flags the sprite as flipped
	@ingroup	MainShell
	@param		pSprHandle 	- Pointer to the sprite handle
	@param		Flipped 	- Flipped flag
	@return		void
 --------------------------------------------------------------------------- */
void LIB_SprManager_FlipSprite( PSPRHANDLE pSprHandle, bool Flipped )
{
	if (sSprMgr.sFlags.Initialized == ON)
	{
		if (pSprHandle && pSprHandle->SprIndex < TOTAL_SPRITES)
		{
			sSprMgr.Sprites[ pSprHandle->SprIndex ].SprFlags.Flipped = Flipped;
		}
	}
}

//-----------------------------------------------------------------------------
// End of file: LIB_SprManager.c
//-----------------------------------------------------------------------------

