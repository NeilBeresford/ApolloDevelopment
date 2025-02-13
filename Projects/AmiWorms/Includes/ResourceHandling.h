
/** ---------------------------------------------------------------------------
	@file		ResourceHandling.h
	@defgroup 	AmiWorms Apollo V4 Shell
	@brief		Resource management for the Apollo V4 Shell
    @date		2024-10-31
	@version	0.1
	@copyright	Neil Beresford 2024	
 -----------------------------------------------------------------------------
	Notes

--------------------------------------------------------------------------- */

#ifndef _RESOURCEHANDLING_H_
#define _RESOURCEHANDLING_H_

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// typedefs and enums
//-----------------------------------------------------------------------------


/** ----------------------------------------------------------------------------
    @brief 	    Enums for the Resource types
    @ingroup 	AmiWorms
----------------------------------------------------------------------------- */
typedef enum
{
    eResourceType_NotSet    = -1,   //<! -1 Resource type not set
    eResourceType_Text      = 0,    //<!  0 Resource is text
    eResourceType_Binary,           //<!  1 Resource is binary
    eResourceType_Image,            //<!  2 Resource is image
    eResourceType_Sound,            //<!  3 Resource is sound
    eResourceType_Font,             //<!  4 Resource is font
    eResourceType_Palette,          //<!  5 Resource is palette
    eResourceType_Sprite,           //<!  6 Resource is sprite
    eResourceType_Total             //<!  7 Total number of resource types

} ResourceType_t;


/**-----------------------------------------------------------------------------
    @brierf 	Enums for the Resource types using the GET method
    @ingroup 	AmiWorms
 ---------------------------------------------------------------------------- */
typedef enum
{
    eResourceGet_Size = 0,  //!< 0 Get the size of the resource
    eResourceGet_Type,      //!< 1 Get the type of the resource
    eResourceGet_Name,      //!< 2 Get the name of the resource
    eResourceGet_Data,      //!< 3 Get the data of the resource   
    eResourceGet_Total      //!< 4 Total number of resource gets

} eResourceGet_t;

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

extern bool ResourceHandling_Init( void );
extern bool ResourceHandling_Close( void );
extern bool ResourceHandling_Add( uint32_t ulResourceID, uint32_t ulResourceSize, uint32_t ulResourceType, uint8_t* pszResourceName, uint8_t* pResourceData );
extern bool ResourceHandling_Remove( uint32_t ulResourceID );
extern bool ResourceHandling_Get( uint32_t ulResourceID, eResourceGet_t eType, uint32_t* pReturnData );
bool ResourceHandling_LoadGroups( sFileGroup groups[] );
uint32_t ResourceHandling_GetGroupStartResource( uint32_t nGroupIndex );
void ResourceHandling_InitStatus( psFileGroup groups );
uint8_t* ResourceHandling_GetGroupName( uint32_t nGroupIndex );
bool ResourceHandling_ScanAndSetSpriteDimentions( void );
//-----------------------------------------------------------------------------

#endif // _RESOURCEHANDLING_H_

//-----------------------------------------------------------------------------
// End of File: ResourceHandling.h
//-----------------------------------------------------------------------------