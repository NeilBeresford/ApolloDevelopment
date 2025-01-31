
/** ---------------------------------------------------------------------------
	@file		ResourceHandling.h
	@defgroup 	MainShell Apollo V4 Shell
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

/**-----------------------------------------------------------------------------
    @brierf 	Enums for the Resource types using the GET method
    @ingroup 	MainShell
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

//-----------------------------------------------------------------------------

#endif // _RESOURCEHANDLING_H_

//-----------------------------------------------------------------------------
// End of File: ResourceHandling.h
//-----------------------------------------------------------------------------