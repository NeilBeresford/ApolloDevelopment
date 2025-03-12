/** ---------------------------------------------------------------------------
    @file		Map_Scroll.c
    @defgroup 	AmiWorms Apollo V4 Shell
    @brief		Map scrolling functionality
    @date		2025-03-12
    @copyright	Neil Beresford 2025
 -----------------------------------------------------------------------------
    Notes

--------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"

#include "../../Includes/defines.h"
#include "../../Includes/GlobalData.h"
#include "../../Includes/FlagStruct.h"
#include "../../Includes/Map_Scroll.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define SCROLLSPEED_1SEC ( 0.05f )

//-----------------------------------------------------------------------------
// Typedefs and Enums
//-----------------------------------------------------------------------------

typedef enum
{
    eMapScrollState_Idle = 0,  //!< 00 - Idle
    eMapScrollState_Scrolling, //!< 01 - Scrolling
    eMapScrollState_ScrollTo,  //!< 02 - Scroll to
    eMapScrollState_Position,  //!< 03 - Position
    eMapScrollState_Tracking,  //!< 04 - Tracking
    eMapScrollState_Demo,      //!< 05 - Demo

    eMapScrollState_Total

} eMapScrollState;

typedef struct
{
    FlagStruct_t    Flags;
    eMapScrollState State;
    eMapScrollState OldState;
    bool            DemoMode;
    uint16_t        nScrollCnt;
    float           fScrollX;
    float           fScrollY;
    float           fTargetX;
    float           fTargetY;
    float           fSpeed;
    float           fXDiv;
    float           fYDiv;
    float           fDist;

} MapScrollCtrl_t, *pMapScrollCtrl_t;

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------

void Map_Scroll_LimitXY( int32_t* nX, int32_t* nY );

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

MapScrollCtrl_t sMSCtrl;

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

/** ---------------------------------------------------------------------------
    @brief 		Init the map scrolling control
    @ingroup 	AmiWorms
    @param		pSprData 	- Pointer to the sprite data
 --------------------------------------------------------------------------- */
void Map_Scroll_Init( void )
{
    sMSCtrl.Flags.Flags       = 0;
    sMSCtrl.Flags.Initialized = YES;

    sMSCtrl.fScrollX          = 0;
    sMSCtrl.fScrollY          = 0;
    sMSCtrl.fTargetX          = 0;
    sMSCtrl.fTargetY          = 0;
    sMSCtrl.fSpeed            = 0.0f;

    // sMSCtrl.State             = eMapScrollState_Demo;
    // sMSCtrl.DemoMode          = true;
    sMSCtrl.State    = eMapScrollState_Idle;
    sMSCtrl.OldState = eMapScrollState_Idle;
}

/** ---------------------------------------------------------------------------
    @brief 		update the map scrolling control
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void Map_Scroll_Update()
{
    switch ( sMSCtrl.State )
    {
        case eMapScrollState_Idle:
        {
            break;
        }
        case eMapScrollState_Scrolling:
        {
            break;
        }
        case eMapScrollState_ScrollTo:
        {
            sMSCtrl.fScrollX += sMSCtrl.fXDiv;
            sMSCtrl.fScrollY += sMSCtrl.fYDiv;

            // int32_t nX = (int32_t)sMSCtrl.fScrollX;
            // int32_t nY = (int32_t)sMSCtrl.fScrollY;
            // Map_Scroll_LimitXY( &nX, &nY );
            // sMSCtrl.fScrollX     = (float)nX;
            // sMSCtrl.fScrollY     = (float)nY;

            sGlobalData.nScrollX = (int32_t)sMSCtrl.fScrollX;
            sGlobalData.nScrollY = (int32_t)sMSCtrl.fScrollY;

            if ( --sMSCtrl.nScrollCnt == 0 )
            {
                sMSCtrl.State = eMapScrollState_Idle;
                sMSCtrl.fXDiv = 0;
                sMSCtrl.fYDiv = 0;
            }

            break;
        }
        case eMapScrollState_Position:
        {
            break;
        }
        case eMapScrollState_Tracking:
        {
            break;
        }
        case eMapScrollState_Demo:
        {
            Map_Scroll_DemoCtrl();
            break;
        }
        default:
        {
            break;
        }
    }
}

/** ---------------------------------------------------------------------------
    @brief 		Map scrolling demo control
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void Map_Scroll_DemoCtrl( void )
{
    Map_Scroll_ScrollTo( 100, 100 );
}

//-----------------------------------------------------------------------------
// External Assigners

/** ---------------------------------------------------------------------------
    @brief 		Scroll to a position
    @ingroup 	AmiWorms
    @param		nX 	- X position
    @param		nY 	- Y position
 --------------------------------------------------------------------------- */
void Map_Scroll_ScrollTo( int32_t nX, int32_t nY )
{
    // limit the coordinates to the map, as x,y in the center of the screen
    Map_Scroll_LimitXY( &nX, &nY );

    sMSCtrl.fScrollX   = sGlobalData.nScrollX;
    sMSCtrl.fScrollY   = sGlobalData.nScrollY;
    sMSCtrl.fTargetX   = nX;
    sMSCtrl.fTargetY   = nY;

    sMSCtrl.fDist      = sqrt( ( sMSCtrl.fTargetX - sMSCtrl.fScrollX ) * ( sMSCtrl.fTargetX - sMSCtrl.fScrollX ) + ( sMSCtrl.fTargetY - sMSCtrl.fScrollY ) * ( sMSCtrl.fTargetY - sMSCtrl.fScrollY ) );
    sMSCtrl.fXDiv      = ( sMSCtrl.fTargetX - sMSCtrl.fScrollX ) * SCROLLSPEED_1SEC;
    sMSCtrl.fYDiv      = ( sMSCtrl.fTargetY - sMSCtrl.fScrollY ) * SCROLLSPEED_1SEC;
    sMSCtrl.nScrollCnt = 20;
    // if ( sMSCtrl.DemoMode != true )
    {
        sMSCtrl.State = eMapScrollState_ScrollTo;
    }
}

//-----------------------------------------------------------------------------
// Internal Functionality
//-----------------------------------------------------------------------------

void Map_Scroll_LimitXY( int32_t* nX, int32_t* nY )
{
    if ( *nX < 0 )
    {
        *nX = 0;
    }
    if ( *nX > 1920 - 640 )
    {
        *nX = 1920 - 640;
    }
    if ( *nY < 0 )
    {
        *nY = 0;
    }
    if ( *nY > 960 - 360 )
    {
        *nY = 960 - 360;
    }
}

//-----------------------------------------------------------------------------
// End of file: Map_Scroll.c
//-----------------------------------------------------------------------------
