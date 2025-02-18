/** ---------------------------------------------------------------------------
    @file		Hardware.h
    @defgroup 	AmiWorms Apollo V4 Shell
    @brief		Hardware Support for the Apollo system
    @date		2024-10-31
    @version	0.1
    @copyright	Neil Beresford 2024
 -----------------------------------------------------------------------------
    Notes

--------------------------------------------------------------------------- */

#ifndef _HARDWARE_H_
#define _HARDWARE_H_

//-----------------------------------------------------------------------------

#define _REG

#define REGP( reg, p ) p __asm( #reg )

#define _A0( param )   REGP( a0, param )
#define _A1( param )   REGP( a1, param )
#define _A2( param )   REGP( a2, param )
#define _A3( param )   REGP( a3, param )
#define _A4( param )   REGP( a4, param )
#define _A5( param )   REGP( a5, param )
#define _A6( param )   REGP( a6, param )
#define _A7( param )   REGP( a7, param )

#define _D0( param )   REGP( d0, param )
#define _D1( param )   REGP( d1, param )
#define _D2( param )   REGP( d2, param )
#define _D3( param )   REGP( d3, param )
#define _D4( param )   REGP( d4, param )
#define _D5( param )   REGP( d5, param )
#define _D6( param )   REGP( d6, param )
#define _D7( param )   REGP( d7, param )

#define _FP0( param )  REGP( fp0, param )
#define _FP1( param )  REGP( fp1, param )
#define _FP2( param )  REGP( fp2, param )
#define _FP3( param )  REGP( fp3, param )
#define _FP4( param )  REGP( fp4, param )
#define _FP5( param )  REGP( fp5, param )
#define _FP6( param )  REGP( fp6, param )
#define _FP7( param )  REGP( fp7, param )

#if REGPARM

#if defined( __GNUC__ )
#undef REGP
#define REGP( reg, p ) p __asm( #reg )
#elif defined( __VBCC__ )
#undef REGP
#define REGP( reg, p ) __reg( #reg ) p
#elif defined( __SASC )
#undef _REG
#define _REG __asm
#undef REGP
#define REGP( reg, p ) register __##reg p
#endif

#endif

//-----------------------------------------------------------------------------
// External Data
//-----------------------------------------------------------------------------

#define _REG

#define REGP( reg, p ) p __asm( #reg )

#define _A0( param )   REGP( a0, param )
#define _A1( param )   REGP( a1, param )
#define _A2( param )   REGP( a2, param )
#define _A3( param )   REGP( a3, param )
#define _A4( param )   REGP( a4, param )
#define _A5( param )   REGP( a5, param )
#define _A6( param )   REGP( a6, param )
#define _A7( param )   REGP( a7, param )

#define _D0( param )   REGP( d0, param )
#define _D1( param )   REGP( d1, param )
#define _D2( param )   REGP( d2, param )
#define _D3( param )   REGP( d3, param )
#define _D4( param )   REGP( d4, param )
#define _D5( param )   REGP( d5, param )
#define _D6( param )   REGP( d6, param )
#define _D7( param )   REGP( d7, param )

#define _FP0( param )  REGP( fp0, param )
#define _FP1( param )  REGP( fp1, param )
#define _FP2( param )  REGP( fp2, param )
#define _FP3( param )  REGP( fp3, param )
#define _FP4( param )  REGP( fp4, param )
#define _FP5( param )  REGP( fp5, param )
#define _FP6( param )  REGP( fp6, param )
#define _FP7( param )  REGP( fp7, param )

#if REGPARM

#if defined( __GNUC__ )
#undef REGP
#define REGP( reg, p ) p __asm( #reg )
#elif defined( __VBCC__ )
#undef REGP
#define REGP( reg, p ) __reg( #reg ) p
#elif defined( __SASC )
#undef _REG
#define _REG __asm
#undef REGP
#define REGP( reg, p ) register __##reg p
#endif

#endif

//-----------------------------------------------------------------------------
// External Data
//-----------------------------------------------------------------------------

#define _REG

#define REGP( reg, p ) p __asm( #reg )

#define _A0( param )   REGP( a0, param )
#define _A1( param )   REGP( a1, param )
#define _A2( param )   REGP( a2, param )
#define _A3( param )   REGP( a3, param )
#define _A4( param )   REGP( a4, param )
#define _A5( param )   REGP( a5, param )
#define _A6( param )   REGP( a6, param )
#define _A7( param )   REGP( a7, param )

#define _D0( param )   REGP( d0, param )
#define _D1( param )   REGP( d1, param )
#define _D2( param )   REGP( d2, param )
#define _D3( param )   REGP( d3, param )
#define _D4( param )   REGP( d4, param )
#define _D5( param )   REGP( d5, param )
#define _D6( param )   REGP( d6, param )
#define _D7( param )   REGP( d7, param )

#define _FP0( param )  REGP( fp0, param )
#define _FP1( param )  REGP( fp1, param )
#define _FP2( param )  REGP( fp2, param )
#define _FP3( param )  REGP( fp3, param )
#define _FP4( param )  REGP( fp4, param )
#define _FP5( param )  REGP( fp5, param )
#define _FP6( param )  REGP( fp6, param )
#define _FP7( param )  REGP( fp7, param )

#if REGPARM

#if defined( __GNUC__ )
#undef REGP
#define REGP( reg, p ) p __asm( #reg )
#elif defined( __VBCC__ )
#undef REGP
#define REGP( reg, p ) __reg( #reg ) p
#elif defined( __SASC )
#undef _REG
#define _REG __asm
#undef REGP
#define REGP( reg, p ) register __##reg p
#endif

#endif

extern uint8_t* SpriteDims;

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void     Hardware_Init( void );
void     Hardware_Close( void );
void     Hardware_WaitVBL( void );
void     Hardware_FlipScreen( void );
void     Hardware_ClearScreen( void );
void     Hardware_TestScreen( void );

uint32_t Hardware_ReadKey( void );
uint8_t* Hardware_GetScreenPtr( void );

int      Hardware_RandomNumber( void );
void     Hardware_SetScreenmode( _D0( uint32_t ScreenMode ) );
void     Hardware_StoreLastKey( _D0( uint32_t lastKey ) );
void     Hardware_CheckKeyUp( void );
void     Hardware_CopyBackToScreen( void );
void     Hardware_CopyBackScreenMap( void );
void     Hardware_CopyBack2ToBack1( void );
void     Hardware_SetBackscreenBuffers( void );
uint32_t Hardware_SwapLong( _D0( uint32_t SwapLong ) );
uint32_t Hardware_GetScreenWidth( void );
uint32_t Hardware_GetScreenHeight( void );
uint32_t Hardware_GetScreenmode( void );
uint32_t Hardware_GetDebug( _D0( uint32_t Debug ) );
void     Hardware_SetMapX( _D0( uint32_t mapX ) );
void     Hardware_SetMapY( _D0( uint32_t mapX ) );
uint32_t Hardware_GetMapX( void );
uint32_t Hardware_GetMapY( void );
uint32_t Hardware_JoystickButtonPressed( void );
uint32_t Haardware_ReturnKeyState( void );
uint8_t* Hardware_GetSpriteDims( void );
void     Hardware_DrawBlock( _D0( uint32_t x ), _D1( uint32_t y ), _D2( uint32_t width ), _D3( uint32_t height ), _D4( uint32_t colour ) );
void     Hardware_DrawBackScreenBlock( _D0( uint32_t x ), _D1( uint32_t y ), _D2( uint32_t width ), _D3( uint32_t height ),
                                       _D4( uint32_t colour ) );

//-----------------------------------------------------------------------------

#endif // _HARDWARE_H_

//-----------------------------------------------------------------------------
// End of File: Hardware.h222
//-----------------------------------------------------------------------------