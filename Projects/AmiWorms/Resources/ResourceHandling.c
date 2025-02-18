/** ---------------------------------------------------------------------------
    @file		ResourceHandling.c
    @defgroup 	AmiWorms Apollo V4 Shell
    @brief		Resource management for the Apollo V4 Shell
    @date		2024-11-01
    @version	0.1
    @copyright	Neil Beresford 2024
 -----------------------------------------------------------------------------
    Notes

    Quick summary of functionality -
    - ResourceHandling_Init()                   Initialize the resource handling
    - ResourceHandling_Close()                  Tidy and close the resource handling
    - ResourceHandling_Add()                    Add a resource to the resource handling
    - ResourceHandling_Remove()                 Remove a resource
    - ResourceHandling_Get()                    Get resource information
    - ResourceHandling_InitStatus()             Initialize the status of the resource handling
    - ResourceHandling_LoadGroups()             Load groups of resources
    - ResourceHandling_GetGroupStartResource()  Get the start resource ID of a group

--------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "../Includes/FlagStruct.h"
#include "../Includes/ResourceFiles.h"
#include "../Includes/Hardware.h"
#include "../Includes/LIB_Sprites.h"
#include "../Includes/ResourceHandling.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define TOTAL_RESOURCES   ( 2000 )
#define START_RESOURCE_ID ( 0x1000 )

//-----------------------------------------------------------------------------
// Typedefs and enums
//-----------------------------------------------------------------------------

/** ----------------------------------------------------------------------------
    @brief   	Resource handling structure
    @ingroup 	AmiWorms
----------------------------------------------------------------------------- */
typedef struct
{
    uint32_t ulResourceID;
    uint32_t ulResourceSize;
    uint32_t ulResourceType;
    uint8_t* pszResourceName;
    uint8_t* pResourceData;

} ResourceHeader_t, *pResourceHeader_t;

/** ----------------------------------------------------------------------------
    @brief   	Resource handling control structure
    @ingroup 	AmiWorms
----------------------------------------------------------------------------- */
typedef struct
{
    FlagStruct_t     Flags;
    ResourceHeader_t Resource[ TOTAL_RESOURCES ];
    pSprDimention_t  pSpriteDim;
    psFileGroup      Groups;
    uint32_t         ulResourceCount;
    uint32_t         nTotalGroups;
    uint32_t         ulCurrentResourceID;
    uint8_t          tmpFileName[ 256 ];
    uint32_t         ulTotalFiles;
    uint32_t         ulCurLoadedFile;
    uint32_t         nTotalSprites;
    bool             bStatusNeeded;

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
    @ingroup 	AmiWorms
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
        sRHCtrl.Groups              = NULL;
        sRHCtrl.pSpriteDim          = NULL;
        sRHCtrl.nTotalGroups        = 0;
        sRHCtrl.nTotalSprites       = 0;

        // return success
        bReturn = true;
    }

    return bReturn;
}

/** ----------------------------------------------------------------------------
    @brief 	    Tidy and close the resource handling
    @ingroup 	AmiWorms
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
    @ingroup 	AmiWorms
    @param      ulResourceID   - Resource ID index
    @param      ulResourceSize  - Resource size
    @param      ulResourceType  - Resource type
    @param      pszResourceName - Resource name
    @param      pResourceData   - Resource data
    @return 	bool            - true if successful
 -----------------------------------------------------------------------------*/
bool ResourceHandling_Add( uint32_t ulResourceID, uint32_t ulResourceSize, uint32_t ulResourceType, uint8_t* pszResourceName,
                           uint8_t* pResourceData )
{
    bool bReturn = false;

    if ( ulResourceID < TOTAL_RESOURCES && sRHCtrl.Flags.Initialized == 1 )
    {
        // validate the remaining parameters
        if ( ulResourceSize > 0 && ulResourceType < eResourceType_Total && pszResourceName != NULL && pResourceData != NULL )
        {
            if ( sRHCtrl.Resource[ ulResourceID ].ulResourceType == eResourceType_NotSet )
            {
                // add the resource
                sRHCtrl.Resource[ ulResourceID ].ulResourceID    = sRHCtrl.ulCurrentResourceID;
                sRHCtrl.Resource[ ulResourceID ].ulResourceSize  = ulResourceSize;
                sRHCtrl.Resource[ ulResourceID ].ulResourceType  = ulResourceType;
                sRHCtrl.Resource[ ulResourceID ].pszResourceName = pszResourceName;
                sRHCtrl.Resource[ ulResourceID ].pResourceData   = pResourceData;

                // increment the resource count
                sRHCtrl.ulResourceCount++;

                // return success
                bReturn = true;
            }
            // Add the resource
            bReturn = true;
        }
    }

    return bReturn;
}

/** ----------------------------------------------------------------------------
    @brief 		Remove ressource from the resource handling
    @ingroup 	AmiWorms
    @param      ulResourceID    - Resource ID to be removed
    @return 	bool            - true if successful
 -----------------------------------------------------------------------------*/
bool ResourceHandling_Remove( uint32_t ulResourceID )
{
    bool bReturn = false;

    if ( sRHCtrl.Flags.Initialized == 1 )
    {
        // remove the resource
        free( sRHCtrl.Resource[ ulResourceID ].pResourceData );
        sRHCtrl.Resource[ ulResourceID ].ulResourceID    = 0;
        sRHCtrl.Resource[ ulResourceID ].ulResourceSize  = 0;
        sRHCtrl.Resource[ ulResourceID ].ulResourceType  = eResourceType_NotSet;
        sRHCtrl.Resource[ ulResourceID ].pszResourceName = NULL;
        sRHCtrl.Resource[ ulResourceID ].pResourceData   = NULL;

        // decrement the resource count
        sRHCtrl.ulResourceCount--;

        // return success
        bReturn = true;
    }

    return bReturn;
}

/** ----------------------------------------------------------------------------
    @brief 		Get ressource from the resource handling
    @ingroup 	AmiWorms
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
        uint32_t ulIndex = ulResourceID;
        // get the resource data
        switch ( eType )
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
        bReturn = true;
    }

    return bReturn;
}

/** ----------------------------------------------------------------------------
    @brief 		Load the resource groups
    @ingroup 	AmiWorms
    @param      groups          - Pointer to the resource groups
    @return 	bool            - true if successful
 -----------------------------------------------------------------------------*/
bool ResourceHandling_LoadGroups( psFileGroup groups )
{
    uint32_t ulResourceID = 0;

    if ( groups != NULL )
    {
        psFileGroup psGroup              = groups;
        uint32_t    ulIndex              = 0;
        uint32_t    ulTotalSize          = 0;
        uint32_t    ulNumLoaded          = 0;
        uint32_t    ulTotalFilesRemapped = 0;
        uint32_t    ulGroupCount         = 0;

        if ( sRHCtrl.bStatusNeeded == true )
        {
            printf( "Loading sprite files : 0%%               \r" );
            fflush( stdout );
        }

        sRHCtrl.Groups = groups;

        while ( psGroup->pszDirectory != NULL )
        {
            psFileDetails psFileDetails = psGroup->psFileDetails;
            uint32_t      ulFileIndex   = 0;

            ulGroupCount++;

            psGroup->ulStartResourceID = ulResourceID;

            while ( psFileDetails->pszResourceName != NULL )
            {
                uint8_t* pFileBuffer = NULL;
                uint32_t ulFileSize  = 0;

                strcpy( sRHCtrl.tmpFileName, psGroup->pszDirectory );
                strcat( sRHCtrl.tmpFileName, psFileDetails->pszResourceName );

                if ( sRHCtrl.bStatusNeeded == true && !( ulNumLoaded & 3 ) )
                {
                    float    fPercent  = (float)ulNumLoaded / (float)sRHCtrl.ulTotalFiles;
                    uint32_t ulPercent = (uint32_t)( fPercent * 100 );
                    printf( "Loading sprite files : %d%% (%d of %d)\r", ulPercent, ulNumLoaded, sRHCtrl.ulTotalFiles );
                    fflush( stdout );
                }

                if ( LIB_Files_Load( sRHCtrl.tmpFileName, &pFileBuffer, &ulFileSize ) == true )
                {
                    if ( ResourceHandling_Add( ulResourceID, ulFileSize, psFileDetails->eFileType, psFileDetails->pszResourceName,
                                               pFileBuffer )
                         == false )
                    {
                        // Exit error
                        printf( "Resource failed to add: %s\n", sRHCtrl.tmpFileName );
                        return false;
                    }
                    if ( LIB_Sprites_RegisterBank( ulResourceID, psFileDetails->eFileType, ulResourceID, pFileBuffer, ulFileSize,
                                                   psFileDetails->ulNumber, psFileDetails->ulWidth, psFileDetails->ulHeight )
                         == false )
                    {
                        // Exit error
                        printf( "Resource failed to register: %s\n", sRHCtrl.tmpFileName );
                        return false;
                    }
                    else
                    {
                        if ( psGroup->reMapValue != 0 && psFileDetails->eFileType == eRAW )
                        {
                            ulTotalFilesRemapped++;
                            if ( strncmp( (int8_t*)( psFileDetails->pszResourceName ), "gradient-8-", 11 ) == 0 )
                            {
                                LIB_Sprites_Remap( ulResourceID, 184 );
                            }
                            else
                            {
                                LIB_Sprites_Remap( ulResourceID, psGroup->reMapValue - 1 );
                            }
                        }
                    }
                }
                else
                {
                    printf( "File failed to load: %s\n", sRHCtrl.tmpFileName );
                }
                ulNumLoaded++;
                ulIndex++;
                ulTotalSize += ulFileSize;
                ulResourceID++;
                ulFileIndex++;
                psFileDetails++;
            }

            psGroup++;
        }
        if ( sRHCtrl.bStatusNeeded == true )
        {
            float fPercent = (float)ulNumLoaded / (float)sRHCtrl.ulTotalFiles;
            printf( "Loading sprite files : 100%% (%d of %d)\n", ulNumLoaded, sRHCtrl.ulTotalFiles );
            printf( "Sprite Resource Loaded into fast memory\n" );
            printf( "Total files loaded   : %d\n", ulNumLoaded );
            printf( "Total files remapped : %d\n", ulTotalFilesRemapped );
            printf( "Total resource size  : %dKB \n", ( ulTotalSize >> 10 ) + 1 );
        }
    }

    return true;
}

/** ----------------------------------------------------------------------------
    @brief 		Initialize the status reporting during loading
    @ingroup 	AmiWorms
    @param      groups          - Pointer to the resource groups
 -----------------------------------------------------------------------------*/
void ResourceHandling_InitStatus( psFileGroup groups )
{
    // count the number of resources
    uint32_t ulIndex      = 0;
    uint32_t ulTotalFiles = 0;

    while ( groups[ ulIndex ].pszDirectory != NULL )
    {
        psFileDetails psFileDetails = groups[ ulIndex ].psFileDetails;

        while ( psFileDetails->pszResourceName != NULL )
        {
            ulTotalFiles++;
            psFileDetails++;
        }

        ulIndex++;
    }

    sRHCtrl.nTotalGroups    = ulIndex;
    sRHCtrl.ulTotalFiles    = ulTotalFiles;
    sRHCtrl.ulCurLoadedFile = 0;
    sRHCtrl.bStatusNeeded   = true;
}

/** ----------------------------------------------------------------------------
    @brief 		Get the start resource ID of a group
    @ingroup 	AmiWorms
    @param      nGroupIndex     - Group index
    @return 	uint32_t        - Start resource ID
 -----------------------------------------------------------------------------*/
uint32_t ResourceHandling_GetGroupStartResource( uint32_t nGroupIndex )
{
    return theFileGroups[ nGroupIndex ].ulStartResourceID;
}

/** ----------------------------------------------------------------------------
    @brief 		Get the resource name
    @ingroup 	AmiWorms
    @param      ulResourceID    - Resource ID
    @return 	uint8_t*        - Pointer to the resource name
 -----------------------------------------------------------------------------*/
uint8_t* ResourceHandling_GetGroupName( uint32_t nGroupIndex )
{
    return theFileGroups[ nGroupIndex ].pszDirectory;
}

/** ----------------------------------------------------------------------------
    @brief 		Get the total number of sprites
    @ingroup 	AmiWorms
    @return 	uint32_t        - Total number of sprites
 -----------------------------------------------------------------------------*/
uint32_t ResourceHandling_GetTotalNumSprites( void )
{
    uint32_t nRefIndex = 0;
    uint32_t nTotal    = 0;

    if ( sRHCtrl.Groups != NULL )
    {
        for ( int32_t nGroup = 0; nGroup < sRHCtrl.nTotalGroups; nGroup++ )
        {
            psFileDetails psFD  = sRHCtrl.Groups[ nGroup ].psFileDetails;
            uint32_t      nFile = 0;
            while ( psFD->pszResourceName != NULL )
            {
                nTotal += LIB_Sprites_GetTotalNumSprites( nFile + nRefIndex );

                nFile++;
                psFD++;
            }

            nRefIndex += nFile;
        }

        sRHCtrl.nTotalSprites = nTotal;
    }

    return nTotal;
}

/** ----------------------------------------------------------------------------
    @brief 		Scan and set the sprite dimentions
    @ingroup 	AmiWorms
    @return 	bool            - true if successful
 -----------------------------------------------------------------------------*/
bool ResourceHandling_ScanAndSetSpriteDimentions( void )
{
    bool     bRet        = false;
    uint32_t nRefIndex   = 0;
    uint32_t nCurScanned = 0;

    if ( sRHCtrl.Groups != NULL && sRHCtrl.nTotalSprites != 0 )
    {
        sRHCtrl.pSpriteDim =
            (pSprDimention_t)Hardware_GetSpriteDims(); // (pSprDimention_t)malloc( sRHCtrl.nTotalSprites * sizeof( SprDimention_t ) );
        pSprDimention_t pSpriteDim = sRHCtrl.pSpriteDim;

        for ( int32_t nGroup = 0; nGroup < sRHCtrl.nTotalGroups; nGroup++ )
        {
            psFileDetails psFD  = sRHCtrl.Groups[ nGroup ].psFileDetails;
            uint32_t      nFile = 0;
            while ( psFD->pszResourceName != NULL )
            {
                uint32_t nSprs = LIB_Sprites_GetTotalNumSprites( nFile + nRefIndex );
                for ( int32_t nSpr = 0; nSpr < nSprs; nSpr++ )
                {
                    LIB_Sprites_GetSpriteDimentions( nRefIndex + nFile, nSpr, pSpriteDim );
                    pSpriteDim++;
                }
                nCurScanned += nSprs;

                pSpriteDim++;
                nFile++;
                psFD++;

                if ( sRHCtrl.bStatusNeeded == true )
                {
                    printf( "Setting sprite dimentions : %d of %d sprites       \r", nCurScanned, sRHCtrl.nTotalSprites );
                    fflush( stdout );
                }
            }
            nRefIndex += nFile;
        }

        bRet = true;

        if ( sRHCtrl.bStatusNeeded == true )
        {
            printf( "\n" );
            fflush( stdout );
        }
    }

    return bRet;
}

//-----------------------------------------------------------------------------
// End of File: ResourceHandling.c
//-----------------------------------------------------------------------------
