/** ---------------------------------------------------------------------------
	@file		LIB_Files.c
	@defgroup 	MainShell Apollo V4 Shell
	@brief		Apollo V4 development - Shell
	@date		2020-06-01
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
#include "stdio.h"
#include "Includes/ResourceHandling.h"
#include "Includes/LIB_Files.h"

//-----------------------------------------------------------------------------
// Code
//-----------------------------------------------------------------------------

/** ----------------------------------------------------------------------------
    @brief 		Load a file into memory
    @ingroup 	MainShell
    @param      pszFileName     - Pointer to the file name
    @param      pFileBuffer     - Pointer to the file buffer to be saved 
    @param      pFileSize       - Pointer to the file size to be saved
    @return 	bool            - true if successful
 -----------------------------------------------------------------------------*/
bool LIB_Files_Load( char* pszFileName, uint8_t** pFileBuffer, uint32_t* pFileSize )
{
    bool        bReturn     = false;
    uint32_t    nFileSize   = 0;

    if ( pszFileName != NULL && pFileBuffer != NULL )
    {
        FILE* fp = fopen( pszFileName, "r" );

        printf( "Loading file: %-40s", pszFileName );

        if ( fp != NULL )
        {
            // get the file size
            fseek( fp, 0, SEEK_END );
            nFileSize = ftell( fp );
            fseek( fp, 0, SEEK_SET );
            if ( pFileSize != NULL )
            {
                *pFileSize = nFileSize;
            }
            printf( "[ %6d bytes ]", nFileSize );

            // allocate memory for the file
            *pFileBuffer = (uint8_t*)malloc( nFileSize );
            if ( *pFileBuffer != NULL )
            {
                // read the file into the buffer
                fread( *pFileBuffer, 1, nFileSize, fp );

                // close the file
                fclose( fp );
                printf( "\n" );

                // return success
                bReturn = true;
            }
        }

        // Report error if needed.
        if ( bReturn == false )
        {
            printf( "Error - Failed to load\n" );
        }
    }



    return bReturn;
}


//-----------------------------------------------------------------------------
// End of File: LIB_Files.c
//-----------------------------------------------------------------------------