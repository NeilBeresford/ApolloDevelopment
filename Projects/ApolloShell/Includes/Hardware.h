/** ---------------------------------------------------------------------------
	@file		Hardware.h
	@defgroup 	MainShell Apollo V4 Shell
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

#define REGP(reg, p)	p __asm(#reg)

#define _A0(param) REGP(a0, param)
#define _A1(param) REGP(a1, param)
#define _A2(param) REGP(a2, param)
#define _A3(param) REGP(a3, param)
#define _A4(param) REGP(a4, param)
#define _A5(param) REGP(a5, param)
#define _A6(param) REGP(a6, param)
#define _A7(param) REGP(a7, param)

#define _D0(param) REGP(d0, param)
#define _D1(param) REGP(d1, param)
#define _D2(param) REGP(d2, param)
#define _D3(param) REGP(d3, param)
#define _D4(param) REGP(d4, param)
#define _D5(param) REGP(d5, param)
#define _D6(param) REGP(d6, param)
#define _D7(param) REGP(d7, param)

#define _FP0(param) REGP(fp0, param)
#define _FP1(param) REGP(fp1, param)
#define _FP2(param) REGP(fp2, param)
#define _FP3(param) REGP(fp3, param)
#define _FP4(param) REGP(fp4, param)
#define _FP5(param) REGP(fp5, param)
#define _FP6(param) REGP(fp6, param)
#define _FP7(param) REGP(fp7, param)

#if REGPARM

#if defined(__GNUC__)
#undef  REGP
#define REGP(reg, p)	p __asm(#reg)
#elif defined(__VBCC__)
#undef  REGP
#define REGP(reg, p)	__reg(#reg) p
#elif defined(__SASC)
#undef  _REG
#define _REG	__asm
#undef  REGP
#define REGP(reg, p)	register __##reg p
#endif

#endif

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

extern void Hardware_Init( void );
extern void Hardware_Close( void );
extern void Hardware_WaitVBL( void );
extern void Hardware_FlipScreen( void );
extern void Hardware_ClearScreen( void );
extern void Hardware_TestScreen( void );
extern int 	Hardware_ReadKey( void );
extern int 	Hardware_GetScreenPtr( void );
extern void Hardware_SetScreenmode( _D0(uint32_t ScreenMode) );
extern void Hardware_CopyBackScreen( _D0(uint32_t x), _D1(uint32_t y) );
extern void Hardware_CopyBack2ToBack1( void );	
extern void Hardware_SetBackscreenBuffers( void );
extern _REG uint32_t Hardware_SwapLong( _D0(uint32_t SwapLong) );
extern _REG uint32_t Hardware_GetScreenWidth( void );
extern _REG uint32_t Hardware_GetScreenHeight( void );
extern _REG uint32_t Hardware_GetScreenmode( void );
extern _REG uint32_t Hardware_GetDebug( _D0(uint32_t Debug) );

//-----------------------------------------------------------------------------

#endif // _HARDWARE_H_

//-----------------------------------------------------------------------------
// End of File: Hardware.h
//-----------------------------------------------------------------------------