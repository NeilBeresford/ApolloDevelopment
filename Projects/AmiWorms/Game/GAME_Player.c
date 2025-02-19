/** ---------------------------------------------------------------------------
    @file		GAME_Player.c
    @defgroup 	AmiWorms Apollo V4 source
    @brief		Apollo V4 development - Shell
    @date		2025-01-31
 -----------------------------------------------------------------------------
    Notes

--------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "../Includes/GAME_Player.h"
#include "../Includes/ResourceFiles.h"

//-----------------------------------------------------------------------------
// Vaariables
//-----------------------------------------------------------------------------

sGAME sGame;

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

/** ---------------------------------------------------------------------------
    @brief 		Initialize the game players
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void GAME_Player_Init( void )
{
    // init the game structure
    memset( &sGame, 0, sizeof( sGAME ) );

    sGame.nCurTeam       = eTeam_One;
    sGame.nCurTeamMember = ePlayer_One;
    sGame.nGameSeed      = 654321;
    sGame.eGameType      = eGaameType_SingleplayerMatch;
    sGame.nLevelType     = 0;
    sGame.bActive        = true;
    sGame.nTotalTeams    = 1;

    for ( int32_t nI = 0; nI < TOTAL_TEAMS; nI++ )
    {
        sGame.pPlayer[ nI ].bActive       = true;
        sGame.pPlayer[ nI ].bCPU          = false;
        sGame.pPlayer[ nI ].nCPULevel     = 0;
        sGame.pPlayer[ nI ].nPlayerID     = nI;
        sGame.pPlayer[ nI ].nActive       = 0;
        sGame.pPlayer[ nI ].nTotaalHealth = 0;

        for ( int32_t nJ = 0; nJ < TOTAl_IN_TEAM; nJ++ )
        {
            sGame.pPlayer[ nI ].pSprWorm[ nJ ].bActive     = true;
            sGame.pPlayer[ nI ].pSprWorm[ nJ ].bAlive      = true;
            sGame.pPlayer[ nI ].pSprWorm[ nJ ].nHealth     = 100;
            sGame.pPlayer[ nI ].pSprWorm[ nJ ].nWormID     = nJ;
            sGame.pPlayer[ nI ].pSprWorm[ nJ ].nDamageDone = 0;
            sGame.pPlayer[ nI ].pSprWorm[ nJ ].nKills      = 0;
            sGame.pPlayer[ nI ].pSprWorm[ nJ ].eCurAnim    = eGameWormAnim_Idle;
        }
    }
}

/** ---------------------------------------------------------------------------
    @brief 		Start Game
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void GAME_Player_StartGame( void )
{
    // start the game
    sGame.bActive = true;

    for ( int32_t nI = 0; nI < sGame.nTotalTeams; nI++ )
    {
        sGame.pPlayer[ nI ].nActive = 0;

        for ( int32_t nJ = 0; nJ < 4; nJ++ )
        {
            psGameWorm pWorm   = &sGame.pPlayer[ nI ].pSprWorm[ nJ ];

            pWorm->bAlive      = true;
            pWorm->nHealth     = 100;
            pWorm->nDamageDone = 0;
            pWorm->nKills      = 0;
            pWorm->eCurAnim    = eGameWormAnim_Idle;

            // create sprite
            int32_t nXpos = ( rand() % 1890 ) + 15;
            int32_t nYpos = sGlobalData.pMapHeight[ nXpos ] - 350 + 30;

            while ( nYpos > 910 )
            {
                nXpos = ( rand() % 1890 ) + 15;
                nYpos = sGlobalData.pMapHeight[ nXpos ] - 350 + 30;
            }

            // create sprite
            pWorm->pSprHandle = LIB_SprManager_Add( ResourceHandling_GetGroupStartResource( eGroups_Worms ), nXpos, nYpos, 0, 0, NULL );
            LIB_SprManager_FlipSprite( pWorm->pSprHandle, Hardware_RandomNumber() & 1 );
            LIB_SprManager_AddAnim( pWorm->pSprHandle, 1, SPR_ANIM_LOOP, LIB_SprManager_GetTotalFrames( pWorm->pSprHandle ), NULL );
            LIB_SprManager_SetFlags( pWorm->pSprHandle, SPR_FLAGS_WORLDSPRITE );
        }
    }
}

//-----------------------------------------------------------------------------
// End of file GAME_Player.c
//-----------------------------------------------------------------------------
