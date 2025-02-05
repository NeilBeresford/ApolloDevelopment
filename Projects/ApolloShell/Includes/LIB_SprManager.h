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


struct* PSPRHANDLE;

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

void		LIB_SprManager_Init(void);
void		LIB_SprManager_Draw(void);
PSPRHANDLE 	LIB_SprManager_Add(uint32_t nSprIndex, uint16_t nX, uint16_t nY, uint16_t nGroup, uint16_t nZ, fnSprControl fnControl);
void		LIB_SprManager_Update(void);

//-----------------------------------------------------------------------------

#endif // _LIB_SPRMANAGER_H_

//-----------------------------------------------------------------------------
// End of file: LIB_SprManager.h
//-----------------------------------------------------------------------------

