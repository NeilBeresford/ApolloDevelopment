/** ---------------------------------------------------------------------------
    @file		GAME_Player.h
    @defgroup 	AmiWorms Apollo V4 Shell
    @brief		Perlin Noise Calucations, using vectors
    @date		2025-10-01
    @version	0.1
    @copyright	Neil Beresford 2025
 -----------------------------------------------------------------------------
    Notes

--------------------------------------------------------------------------- */

#ifndef _GAME_PLAYER_H_
#define _GAME_PLAYER_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"
#include "../Includes/defines.h"
#include "../Includes/Hardware.h"
#include "../Includes/LIB_Sprites.h"
#include "../Includes/LIB_SprManager.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define TOTAl_IN_TEAM    8
#define TOTAL_TEAMS      4
#define SMALL_STRING_LEN 32
#define NAME_STRING_LEN  12

//-----------------------------------------------------------------------------
// Typedefs & Enumerators
//-----------------------------------------------------------------------------

typedef enum
{
    eGameWormAnim_Idle = 0,
    eGameWormAnim_Walk,
    eGameWormAnim_Jump,
    eGameWormAnim_Fall,
    eGameWormAnim_Hurt,
    eGameWormAnim_Die,
    eGameWormAnim_Total
} eGAMEWORMANIM;

typedef enum
{
    eGaameType_TwoplayerMatch = 0,
    eGaameType_FourplayerMatch,
    eGaameType_SingleplayerMatch,
    eGaameType_Total

} eGAMETYPE;

typedef struct
{
    PSPRHANDLE    pSprHandle;
    uint8_t       strName[ NAME_STRING_LEN ];
    uint16_t      nHealth;
    uint16_t      nWormID;
    uint16_t      nDamageDone;
    uint16_t      nKills;
    bool          bActive;
    bool          bAlive;
    eGAMEWORMANIM eCurAnim;

} sGameWorm, *psGameWorm;

typedef struct
{
    bool      bActive;
    bool      bCPU;
    uint16_t  nCPULevel;
    uint32_t  nPlayerID;
    uint16_t  nActive;
    sGameWorm pSprWorm[ TOTAl_IN_TEAM ];
    uint16_t  nTotaalHealth;

} sGamePlayer, *psGamePlayer;

typedef struct
{
    bool        bActive;
    bool        bPaused;
    uint32_t    nGameSeed;
    uint16_t    nLevelType;
    uint8_t     strLevelName[ SMALL_STRING_LEN ];
    eGAMETYPE   eGameType;
    uint16_t    nTotalTeams;
    sGamePlayer pPlayer[ TOTAL_TEAMS ];

} sGAME, *psGAME;

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

void GAME_Players_Init( void );

//-----------------------------------------------------------------------------

#endif // _GAME_PLAYER_H_

//-----------------------------------------------------------------------------
// End of file: GAME_player.h
//-----------------------------------------------------------------------------
