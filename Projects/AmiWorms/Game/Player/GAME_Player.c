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

#include "../../Includes/GAME_Player.h"
#include "../../Includes/Markers_ActiveWorm.h"
#include "../../Includes/ResourceFiles.h"

//-----------------------------------------------------------------------------
// Typedefs and Enums
//-----------------------------------------------------------------------------

enum
{
    eWormTemp_IdleDelay = 0,
};

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------

void GAME_Player_SprWormCtrl( void* pSprite );
void GAME_Player_ActivateNextWorm( void );

//-----------------------------------------------------------------------------
// Variables
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

        for ( int32_t nJ = 0; nJ < 8; nJ++ )
        {
            psGameWorm pWorm   = &sGame.pPlayer[ nI ].pSprWorm[ nJ ];

            pWorm->bAlive      = true;
            pWorm->bActive     = false;
            pWorm->nHealth     = 100;
            pWorm->nDamageDone = 0;
            pWorm->nKills      = 0;
            pWorm->eCurAnim    = eGameWormAnim_Idle;

            // create sprite
            int32_t nXpos = 0;
            int32_t nYpos = 960;

            while ( nYpos > 910 )
            {
                nXpos = ( rand() % 1890 ) + 15;
                nYpos = sGlobalData.pMapHeight[ nXpos ] - 350 + 30;
            }

            // create sprite
            pWorm->pSprHandle = LIB_SprManager_Add( ResourceHandling_GetGroupStartResource( eGroups_Worms ), nXpos, nYpos, 0, 0, GAME_Player_SprWormCtrl );
            LIB_SprManager_FlipSprite( pWorm->pSprHandle, Hardware_RandomNumber() & 1 );
            LIB_SprManager_SetVariable( pWorm->pSprHandle, eSPRVAR_DEFAULTRESOURCE, ResourceHandling_GetGroupStartResource( eGroups_Worms ) );
            LIB_SprManager_AddAnim( pWorm->pSprHandle, 1, SPR_ANIM_ONCE, LIB_SprManager_GetTotalFrames( pWorm->pSprHandle ) - 1, NULL );
            LIB_SprManager_SetFlags( pWorm->pSprHandle, SPR_FLAGS_WORLDSPRITE );
        }
    }

    GAME_Player_ActivateNextWorm();
}

//-----------------------------------------------------------------------------
// Internal Functionality
//-----------------------------------------------------------------------------

/** ---------------------------------------------------------------------------
    @brief 		Activate the next worm
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void GAME_Player_ActivateNextWorm( void )
{
    // activate the next worm
    psGameWorm pWorm = &sGame.pPlayer[ sGame.nCurTeam ].pSprWorm[ sGame.pPlayer[ sGame.nCurTeam ].nActive ];

    if ( pWorm->bAlive == true )
    {
        // activate the worm
        pWorm->bActive = true;
    }
    else
    {
        // find the next worm
        for ( int32_t nI = 0; nI < TOTAl_IN_TEAM; nI++ )
        {
            pWorm = &sGame.pPlayer[ sGame.nCurTeam ].pSprWorm[ nI ];

            if ( pWorm->bAlive == true )
            {
                // activate the worm
                pWorm->bActive                          = true;
                sGame.pPlayer[ sGame.nCurTeam ].nActive = nI;
                break;
            }
        }
    }

    // activate marker!
    Markers_ActiveWorm_Init( LIB_SprManager_GetSprite( pWorm->pSprHandle ) );
}

/** ---------------------------------------------------------------------------
    @brief 		Control the game players
    @ingroup 	AmiWorms
    @param      pSprite     Pointer to the sprite structure (void*)
 --------------------------------------------------------------------------- */
void GAME_Player_SprWormCtrl( void* pSprite )
{
    PSPRITE pSpr = (PSPRITE)pSprite;

    if ( pSpr->SprFlags.Active == ON )
    {
        // Idle processing ...
        if ( pSpr->AnimData.AnimType == SPR_ANIM_NONE )
        {
            if ( pSpr->TempData[ eWormTemp_IdleDelay ] == 0 )
            {
                pSpr->TempData[ eWormTemp_IdleDelay ] = ( rand() % 300 ) + 100;
                pSpr->SprResourceID                   = ResourceHandling_GetGroupStartResource( eGroups_Worms ) + ( rand() % 400 );
                LIB_SprManager_ChangeSpriteAnim( pSpr, 1, SPR_ANIM_ONCE, LIB_Sprites_GetFrames( pSpr->SprResourceID ), NULL );
            }
            else
            {
                pSpr->TempData[ eWormTemp_IdleDelay ]--;
            }
        }
    }
}

//-----------------------------------------------------------------------------
// End of file GAME_Player.c
//-----------------------------------------------------------------------------
