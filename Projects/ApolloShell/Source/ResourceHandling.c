/** ---------------------------------------------------------------------------
	@file		ResourceHandling.c
	@defgroup 	MainShell Apollo V4 Shell
	@brief		Resource management for the Apollo V4 Shell
	@date		2024-11-01
	@version	0.1
	@copyright	Neil Beresford 2024	
 -----------------------------------------------------------------------------
	Notes

--------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "Includes/FlagStruct.h"
#include "Includes/ResourceHandling.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define TOTAL_RESOURCES 	( 10 )
#define START_RESOURCE_ID 	( 0x1000 )

//-----------------------------------------------------------------------------
// Typedefs and enums
//-----------------------------------------------------------------------------

/** ----------------------------------------------------------------------------
    @brief 	    Enums for the Resource types
    @ingroup 	MainShell
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

/** ----------------------------------------------------------------------------
    @brief   	Resource handling structure
    @ingroup 	MainShell
----------------------------------------------------------------------------- */
typedef struct
{
    uint32_t 	ulResourceID;
    uint32_t 	ulResourceSize;
    uint32_t 	ulResourceType;
    uint8_t* 	pszResourceName;
    uint8_t* 	pResourceData;

} ResourceHeader_t, *pResourceHeader_t;

/** ----------------------------------------------------------------------------
    @brief   	Resource handling control structure
    @ingroup 	MainShell
----------------------------------------------------------------------------- */
typedef struct 
{
    FlagStruct_t 	    Flags;
    ResourceHeader_t 	Resource[ TOTAL_RESOURCES ];
    uint32_t 		    ulResourceCount;
    uint32_t 		    ulCurrentResourceID;

} RHCtrl_t, *pRHCtrl_t;

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

static RHCtrl_t sRHCtrl = { .Flags.Flags = 0, .ulResourceCount = 0, .ulCurrentResourceID = START_RESOURCE_ID };

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

/** ----------------------------------------------------------------------------
    @brief 		Initialize the resource handling
    @ingroup 	MainShell
    @return 	bool            - true if successful
 -----------------------------------------------------------------------------*/
bool ResourceHandling_Init( void )
{
    bool bReturn = false;

    if ( sRHCtrl.Flags.Initialized == 0 )
    {
        uint32_t ulIndex = 0;

        for ( ulIndex = 0; ulIndex < TOTAL_RESOURCES; ulIndex++ )
        {
            sRHCtrl.Resource[ ulIndex ].ulResourceID    = 0;
            sRHCtrl.Resource[ ulIndex ].ulResourceSize  = 0;
            sRHCtrl.Resource[ ulIndex ].ulResourceType  = eResourceType_NotSet;
            sRHCtrl.Resource[ ulIndex ].pszResourceName = NULL;
            sRHCtrl.Resource[ ulIndex ].pResourceData   = NULL;
        }

        sRHCtrl.Flags.Initialized   = 1;
        sRHCtrl.Flags.Loaded        = 0;
        sRHCtrl.Flags.InUse         = 0;
        sRHCtrl.ulResourceCount     = 0;
        sRHCtrl.ulCurrentResourceID = START_RESOURCE_ID;

        // return success
        bReturn = true;
    }

    return bReturn;
}

/** ----------------------------------------------------------------------------
    @brief 	    Tidy and close the resource handling
    @ingroup 	MainShell
    @return 	bool            - true if successful
 -----------------------------------------------------------------------------*/
bool ResourceHandling_Close( void )
{
    bool bReturn = false;

    if ( sRHCtrl.Flags.Initialized == 1 )
    {
        sRHCtrl.Flags.Flags         = 0;
        sRHCtrl.ulResourceCount     = 0;
        sRHCtrl.ulCurrentResourceID = START_RESOURCE_ID;

        // return success
        bReturn = true;
    }

    return bReturn;
}

/** ----------------------------------------------------------------------------
    @brief 		Add ressource to the resource handling
    @ingroup 	MainShell
    @param      pulResourceID   - Resource ID pointer, for external reference
    @param      ulResourceSize  - Resource size
    @param      ulResourceType  - Resource type
    @param      pszResourceName - Resource name
    @param      pResourceData   - Resource data
    @return 	bool            - true if successful
 -----------------------------------------------------------------------------*/
bool ResourceHandling_Add( uint32_t* pulResourceID, uint32_t ulResourceSize, uint32_t ulResourceType, uint8_t* pszResourceName, uint8_t* pResourceData )
{
    bool bReturn = false;

    if ( pulResourceID != NULL && sRHCtrl.Flags.Initialized == 1 )
    {
        // validate the remaining parameters
        if (ulResourceSize > 0 && ulResourceType < eResourceType_Total && pszResourceName != NULL && pResourceData != NULL)
        {
            // check if we can store the resource
            if ( sRHCtrl.ulResourceCount < TOTAL_RESOURCES )
            {
                uint32_t ulIndex = 0;

                while( ulIndex < sRHCtrl.ulResourceCount )
                {
                    if ( sRHCtrl.Resource[ ulIndex ].ulResourceType == eResourceType_NotSet )
                    {
                        // add the resource
                        sRHCtrl.Resource[ ulIndex ].ulResourceID    = sRHCtrl.ulCurrentResourceID;
                        sRHCtrl.Resource[ ulIndex ].ulResourceSize  = ulResourceSize;
                        sRHCtrl.Resource[ ulIndex ].ulResourceType  = ulResourceType;
                        sRHCtrl.Resource[ ulIndex ].pszResourceName = pszResourceName;
                        sRHCtrl.Resource[ ulIndex ].pResourceData   = pResourceData;

                        // return the resource ID
                        *pulResourceID = ulIndex;

                        // increment the resource count
                        sRHCtrl.ulResourceCount++;
                        sRHCtrl.ulCurrentResourceID++;

                        // return success
                        bReturn = true;
                        break;
                    }
                    ulIndex++;
               }
            }
            // Add the resource
            bReturn = true;
        }
    }

    return bReturn;
}

/** ----------------------------------------------------------------------------
    @brief 		Remove ressource from the resource handling
    @ingroup 	MainShell
    @param      ulResourceID    - Resource ID to be removed
    @return 	bool            - true if successful
 -----------------------------------------------------------------------------*/
bool ResourceHandling_Remove( uint32_t ulResourceID )
{
    bool bReturn = false;

    if ( sRHCtrl.Flags.Initialized == 1 )
    {
        uint32_t ulIndex = 0;

        while ( ulIndex < sRHCtrl.ulResourceCount )
        {
            if ( sRHCtrl.Resource[ ulIndex ].ulResourceID == ulResourceID )
            {
                // remove the resource
                sRHCtrl.Resource[ ulIndex ].ulResourceID    = 0;
                sRHCtrl.Resource[ ulIndex ].ulResourceSize  = 0;
                sRHCtrl.Resource[ ulIndex ].ulResourceType  = eResourceType_NotSet;
                sRHCtrl.Resource[ ulIndex ].pszResourceName = NULL;
                sRHCtrl.Resource[ ulIndex ].pResourceData   = NULL;

                // decrement the resource count
                sRHCtrl.ulResourceCount--;

                // return success
                bReturn = true;
                break;
            }
            ulIndex++;
        }

    }

    return bReturn;
}

/** ----------------------------------------------------------------------------
    @brief 		Get ressource from the resource handling
    @ingroup 	MainShell
    @param      ulResourceID    - Resource ID to be removed
    @param      eType           - Type of data to be returned
    @param      pReturnData     - Pointer to the data to be returned
    @return 	bool            - true if successful
 -----------------------------------------------------------------------------*/
bool ResourceHandling_Get( uint32_t ulResourceID, eResourceGet_t eType, uint32_t* pReturnData )
{
    bool bReturn = false;

    if ( sRHCtrl.Flags.Initialized == 1 )
    {
        uint32_t ulIndex = 0;

        while ( ulIndex < sRHCtrl.ulResourceCount )
        {
            if ( sRHCtrl.Resource[ ulIndex ].ulResourceID == ulResourceID )
            {
                // get the resource data
                switch( eType )
                {
                    case eResourceGet_Size:
                    {
                        *pReturnData = sRHCtrl.Resource[ ulIndex ].ulResourceSize;
                        break;
                    }
                    case eResourceGet_Type:
                    {
                        *pReturnData = sRHCtrl.Resource[ ulIndex ].ulResourceType;
                        break;
                    }
                    case eResourceGet_Name:
                    {
                        *pReturnData = (uint32_t)sRHCtrl.Resource[ ulIndex ].pszResourceName;
                        break;
                    }
                    case eResourceGet_Data:
                    {
                        *pReturnData = (uint32_t)sRHCtrl.Resource[ ulIndex ].pResourceData;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }

                // return success
                bReturn = true;
                break;
            }
            ulIndex++;
        }
    }

    return bReturn;
}

//-----------------------------------------------------------------------------
// End of File: ResourceHandling.c
//-----------------------------------------------------------------------------
