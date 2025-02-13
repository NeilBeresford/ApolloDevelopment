/** ---------------------------------------------------------------------------
	@file		ResourceFiles.h
	@defgroup 	AmiWorms Apollo V4 Shell
	@brief		Resource management for the Apollo V4 Shell
    @date		2024-10-31
	@version	0.1
	@copyright	Neil Beresford 2024	
 -----------------------------------------------------------------------------
	Notes

--------------------------------------------------------------------------- */

#ifndef _RESOURCEFILES_H_
#define _RESOURCEFILES_H_

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// typedefs and enums
//-----------------------------------------------------------------------------

typedef enum
{
    eRAW = 0,
    eSPR,

} eFileType;

typedef struct
{
    uint8_t*    pszResourceName;
    eFileType   eFileType;
    uint32_t    ulNumber;
    uint32_t    ulWidth;
    uint32_t    ulHeight;
} sFileDetails, *psFileDetails;

typedef struct
{
    uint8_t*        pszDirectory;
    psFileDetails   psFileDetails;
    int32_t         reMapValue;
    uint32_t        ulStartResourceID;

} sFileGroup, *psFileGroup;

typedef enum eGroups
{
    eGroups_Icons = 0,      // 00 Icons 
    eGroups_Misc,           // 01 Misc
    eGroups_Weapons,        // 02 Weapons
    eGroups_Worms,          // 03 Worms
    eGroups_Panels,         // 04 Panels
    eGroups_Terrain01,      // 05 Terrain01
    eGroups_Terrain02,      // 06 Terrain02 
    eGroups_Terrain03,      // 07 Terrain03
    eGroups_Terrain04,      // 08 Terrain04
    eGroups_Terrain05,      // 09 Terrain05
    eGroups_Terrain06,      // 10 Terrain06
    eGroups_Terrain07,      // 11 Terrain07
    eGroups_Terrain08,      // 12 Terrain08
    eGroups_Terrain09,      // 13 Terrain09
    eGroups_Terrain10,      // 14 Terrain10
    eGroups_Terrain11,      // 15 Terrain11
    eGroups_Terrain12,      // 16 Terrain12
    eGroups_Terrain13,      // 17 Terrain13 
    eGroups_Terrain14,      // 18 Terrain14
    eGroups_Terrain15,      // 19 Terrain15
    eGroups_Terrain16,      // 20 Terrain16
    eGroups_Terrain17,      // 21 Terrain17
    eGroups_Terrain18,      // 22 Terrain18
    eGroups_Terrain19,      // 23 Terrain19
    eGroups_Terrain20,      // 24 Terrain20
    eGroups_Terrain21,      // 25 Terrain21
    eGroups_Terrain22,      // 26 Terrain22
    eGroups_Terrain23,      // 27 Terrain23
    eGroups_Terrain24,      // 28 Terrain24
    eGroups_Terrain25,      // 29 Terrain25
    eGroups_Terrain26,      // 30 Terrain26
    eGroups_Terrain27,      // 31 Terrain27
    eGroups_Terrain28,      // 32 Terrain28
    eGroups_Terrain29,      // 33 Terrain29
    eGroups_Terrain30,      // 34 Terrain30
    eGroups_Water,          // 35 Water
    eGroups_Font,           // 36 Fonts
    eGroups_Total           // 37 Total number of groups
   
};

typedef enum
{
    eFont_WhiteBig = 0,    // 0
    eFont_WhiteSmall,      // 1
    eFont_BlueBig,         // 2
    eFont_BlueSmall,       // 3
    eFont_CyanBig,         // 4
    eFont_CyanSmall,       // 5
    eFont_GreenBig,        // 6
    eFont_GreenSmall,      // 7
    eFont_PurpleBig,       // 8
    eFont_PurpleSmall,     // 9
    eFont_RedBig,          // 10
    eFont_RedSmall,        // 11
    eFont_YellowBig,       // 12
    eFont_YellowSmall,     // 13
    eFont_TotalFonts       // 14    
};



//-----------------------------------------------------------------------------
// External Variables
//-----------------------------------------------------------------------------

extern sFileGroup theFileGroups[];

//-----------------------------------------------------------------------------

#endif // _RESOURCEFILES_H_

//-----------------------------------------------------------------------------
// End of File: ResourceFiles.h
//-----------------------------------------------------------------------------