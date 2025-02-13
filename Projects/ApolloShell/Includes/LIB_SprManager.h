/** ---------------------------------------------------------------------------
	@file		LIB_SprManager.h
	@defgroup 	MainShell Apollo V4 Shell
	@brief		Apollo V4 development - Shell
	@copyright	Neil Beresford 2024
 -----------------------------------------------------------------------------
	Notes

--------------------------------------------------------------------------- */

#ifndef _LIB_SPRMANAGER_H_
#define _LIB_SPRMANAGER_H_

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define SPRADDFAILERROR		( 0xFFFF )	// Error code for failed sprite add

//-----------------------------------------------------------------------------
// Typedefs and Enums
//-----------------------------------------------------------------------------

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
	SPRHANDLEFLAGS	Flags;
	uint16_t		SprIndex;
	
} SPRHANDLE, *PSPRHANDLE;

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


typedef struct
{
   uint16_t	X;
	uint16_t    Y;

} POSITION, *PPOSITION;

typedef struct
{
	uint16_t	X;
	uint16_t	Y;
	uint16_t	W;
	uint16_t	H;

} RECT, *PRECT;

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
		uint32_t    Flipped 	: 1;
		uint32_t    Reserved	: 24;
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

// Callbacks
typedef void (*fnSprControl)(void* pSprite);

typedef struct _SPRITE
{

	// General Sprite Information
	uint8_t			SprID;
	uint8_t			SprNum;
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



//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

void		LIB_SprManager_Init(void);
void		LIB_SprManager_Draw(void);
PSPRHANDLE 	LIB_SprManager_Add(uint32_t nSprIndex, uint16_t nX, uint16_t nY, uint16_t nGroup, uint16_t nZ, fnSprControl fnControl);
bool 		LIB_SprManager_AddAnim( PSPRHANDLE pSprHandle, uint16_t nAnimID, uint16_t nAnimType, uint16_t nAnimFrames, uint16_t* pFrameData);
void		LIB_SprManager_Update(void);
uint32_t 	LIB_SprManager_GetTotalFrames( PSPRHANDLE pSprHandle );

//-----------------------------------------------------------------------------

#endif // _LIB_SPRMANAGER_H_

//-----------------------------------------------------------------------------
// End of file: LIB_SprManager.h
//-----------------------------------------------------------------------------

