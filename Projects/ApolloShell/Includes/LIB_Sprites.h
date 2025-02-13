/** ---------------------------------------------------------------------------
	@file		LIB_Sprites.h
	@defgroup 	MainShell Apollo V4 Shell
	@brief		Apollo V4 development - Shell
	@date		2020-06-01
	@version	0.1
	@copyright	Neil Beresford 2024
 -----------------------------------------------------------------------------
	Notes

--------------------------------------------------------------------------- */

#ifndef _LIB_SPRITES_H_
#define _LIB_SPRITES_H_

//-----------------------------------------------------------------------------
// typedefs and enums
//-----------------------------------------------------------------------------

/**-----------------------------------------------------------------------------
    @brief      Sprite Bank emums
    @ingroup 	MainShell
 ---------------------------------------------------------------------------- */
typedef enum
{
    eSpriteBank_0 = 0,  //!< 0 Sprite Bank 0
    eSpriteBank_1,      //!< 1 Sprite Bank 1
    eSpriteBank_2,      //!< 2 Sprite Bank 2
    eSpriteBank_3,      //!< 3 Sprite Bank 3
    eSpriteBank_4,      //!< 4 Sprite Bank 4
    eSpriteBank_5,      //!< 5 Sprite Bank 5
    eSpriteBank_6,      //!< 6 Sprite Bank 6
    eSpriteBank_7,      //!< 7 Sprite Bank 7
    eSpriteBank_8,      //!< 8 Sprite Bank 8
    eSpriteBank_9,      //!< 9 Sprite Bank 9
    eSpriteBank_Total   //!< 10 Total number of sprite banks

} eSpriteBank_t;        //!< Sprite Bank emums

/**-----------------------------------------------------------------------------
    @brief      Sprite types
    @ingroup 	MainShell
 ---------------------------------------------------------------------------- */
typedef enum
{
    eSpriteType_Raw = 0,    //!< 0 SpriteType_Raw
    eSpriteType_Compressed, //!< 1 SpriteType_Compressed
    eSpriteType_Total       //!< 2 Total number of sprite types

} eSpriteType_t;            //!< Sprite types


/*-----------------------------------------------------------------------------
    @brief      Sprite structure
    @ingroup 	MainShell
 ---------------------------------------------------------------------------- */
typedef struct
{
    uint32_t        ulSpriteID;     //!< Sprite ID
    uint32_t        ulSpriteSize;   //!< Sprite size   
    uint8_t*        pSpriteData;    //!< Pointer to the sprite data
    eSpriteType_t   ulSpriteType;   //!< Sprite type
    uint32_t        ulNumSprites;   //!< Sprite width
    uint16_t        ulSpriteWidth;  //!< Sprite width
    uint16_t        ulSpriteHeight; //!< Sprite height
    

} SpriteBank_t, *pSpriteBank_t;     //!< Sprite structure

/**-----------------------------------------------------------------------------
    @brief      Sprite Dimentions structure
    @ingroup 	MainShell
 ---------------------------------------------------------------------------- */
typedef struct 
{
    uint16_t nWidth;            //!< Width
    uint16_t nHeight;           //!< Height
    uint16_t nXOffsetLeft;      //!< X Offset Left
    uint16_t nXOffsetRight;     //!< X Offset Right
    uint16_t nYOffsetTop;       //!< Y Offset Top
    uint16_t nYOffsetBottom;    //!< Y Offset Bottom

} SprDimention_t, *pSprDimention_t; //!< Sprite Dimentions structure

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

void LIB_Sprites_Init( void );
void LIB_Sprites_Close( void );
bool LIB_Sprites_RegisterBank( eSpriteBank_t eBank, eSpriteType_t eType, uint32_t ulResourceID, uint8_t* pSpriteData, uint32_t ulSpriteSize, uint32_t ulNumSprs, uint16_t sprW, uint16_t sprH );
bool LIB_Sprites_Draw( eSpriteBank_t eBank, uint32_t sprNum, int32_t x, int32_t y );
bool LIB_Sprites_DrawMap( eSpriteBank_t eBank, uint32_t sprNum, int32_t x, int32_t y );
bool LIB_Sprites_DrawRawPart( eSpriteBank_t eBank, uint32_t sprNum, int32_t x, int32_t y, uint32_t xOff, uint32_t yOff, uint32_t xSize, uint32_t ySize );
bool LIB_Sprites_DrawFlipped( eSpriteBank_t eBank, uint32_t sprNum, int32_t x, int32_t y );
bool LIB_Sprites_Remap( eSpriteBank_t eSpriteBank, uint32_t ShiftBy );
void LIB_Sprites_SetClipArea( uint32_t x, uint32_t y, uint32_t w, uint32_t h );
uint32_t LIB_Sprites_GetWidth( eSpriteBank_t eBank );
uint32_t LIB_Sprites_GetHeight( eSpriteBank_t eBank );
void LIB_Sprites_SetOverwriteColour( uint8_t nColour );
bool LIB_Sprites_GetSpriteDimentions( eSpriteBank_t eBank, uint16_t SprNum, pSprDimention_t pSD );
uint32_t LIB_Sprites_GetFrames( uint32_t nSprResource );
uint32_t LIB_Sprites_GetTotalNumSprites( eSpriteBank_t eBank );

//-----------------------------------------------------------------------------

#endif // _LIB_SPRITES_H_

//-----------------------------------------------------------------------------
// End of file: LIB_Sprites.h
//-----------------------------------------------------------------------------
