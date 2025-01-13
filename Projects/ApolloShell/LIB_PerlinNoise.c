/** ---------------------------------------------------------------------------
	@file		LIB_PerlinNoise.c
	@defgroup 	MainShell Apollo V4 Shell
	@brief		Perlin Noise Calucations, using vectors
	@date		2025-10-01
	@version	0.1
	@copyright	Neil Beresford 2025
 -----------------------------------------------------------------------------
	Notes

--------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "Includes/Hardware.h"

//#include "Includes/LIB_PerlinNoise.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

#define PERM_TABLE_SIZE ( 256*2 ) // 256 * 2 
#define PERM_MAX_MASK   ( 255 )

//-----------------------------------------------------------------------------
// Typedefs & Enumerators
//-----------------------------------------------------------------------------

enum eVectorCorner
{
    eVectorCorner_TopLeft   = 0,
    eVectorCorner_TopRight,
    eVectorCorner_BottomLeft,
    eVectorCorner_BottomRight,
    eVectorCorner_Total
};

typedef struct
{
    float x;
    float y;

} sVector2, *psVector2;

typedef struct 
{
    bool        bInitialized;
    uint8_t     pPermTable[ PERM_TABLE_SIZE ];
    float       fSeed;

    sVector2    sVectors[ eVectorCorner_Total ];

} sPerlinCtrl, *psPerlinCtrl;

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------

static void Shuffle( uint8_t* pArray, uint32_t ulArraySize );
static uint8_t* GeneratePermutation( void );
static sVector2 GetConstantVector( uint8_t ucValue );
static float Fade( float t );
static float Lerp( float a, float b, float t );
static float Dot( sVector2 sVector, sVector2 sVector2 );
static float randomFloat( void );

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

sPerlinCtrl sPerlin = { .bInitialized = false, .pPermTable = NULL, .fSeed = 0.0f };

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

/** ---------------------------------------------------------------------------
    @brief 		Initialize the Perlin Noise
    @ingroup 	MainShell
    @param 		fSeed - the seed value
    @return 	none
 --------------------------------------------------------------------------- */
void LIB_PerlinNoise_Init( float fSeed )
{
    GeneratePermutation();
    sPerlin.bInitialized = true;
    sPerlin.fSeed = fSeed;
}


/** ---------------------------------------------------------------------------
    @brief 		Generate 2D Perlin Noise from X,Y position
    @ingroup 	MainShell
    @param 		x - the x value
    @param 		y - the y value
    @return 	float - the noise value
 --------------------------------------------------------------------------- */
float LIB_PerlinNoise_Noise2D( float x, float y )
{
    uint32_t X = (uint32_t)floor(x) & PERM_MAX_MASK;
    uint32_t Y = (uint32_t)floor(y) & PERM_MAX_MASK;
    float fXf = x - floor( x );
    float fYf = y - floor( y );

    sPerlin.sVectors[ eVectorCorner_TopRight    ].x = fXf - 1.0;
    sPerlin.sVectors[ eVectorCorner_TopRight    ].y = fYf - 1.0;
    sPerlin.sVectors[ eVectorCorner_TopLeft     ].x = fXf;
    sPerlin.sVectors[ eVectorCorner_TopLeft     ].y = fYf - 1.0;
    sPerlin.sVectors[ eVectorCorner_BottomRight ].x = fXf - 1.0;
    sPerlin.sVectors[ eVectorCorner_BottomRight ].y = fYf;
    sPerlin.sVectors[ eVectorCorner_BottomLeft  ].x = fXf;
    sPerlin.sVectors[ eVectorCorner_BottomLeft  ].y = fYf;

    uint8_t valueTopRight    = sPerlin.pPermTable[ sPerlin.pPermTable[ X + 1] + Y + 1 ];      
    uint8_t valueTopLeft     = sPerlin.pPermTable[ sPerlin.pPermTable[ X] + Y + 1 ];      
    uint8_t valueBottomRight = sPerlin.pPermTable[ sPerlin.pPermTable[ X + 1] + Y ];      
    uint8_t valueBottomLeft  = sPerlin.pPermTable[ sPerlin.pPermTable[ X] + Y ];      

    float dotTopRight    = Dot( sPerlin.sVectors[ eVectorCorner_TopRight    ], GetConstantVector( valueTopRight ) );
    float dotTopLeft     = Dot( sPerlin.sVectors[ eVectorCorner_TopLeft     ], GetConstantVector( valueTopLeft ) );
    float dotBottomRight = Dot( sPerlin.sVectors[ eVectorCorner_BottomRight ], GetConstantVector( valueBottomRight ) );
    float dotBottomLeft  = Dot( sPerlin.sVectors[ eVectorCorner_BottomLeft  ], GetConstantVector( valueBottomLeft ) );
    
    float u = Fade( fXf );
    float v = Fade( fYf );

    return Lerp( u, Lerp( v, dotBottomLeft, dotTopLeft ), Lerp( v, dotBottomRight, dotTopRight ) );
}

/** ---------------------------------------------------------------------------
    @brief 		Generate a Perlin Noise map
    @ingroup 	MainShell
    @param 		pMapHeight - the height of the map
    @param 		width - the width of the map
    @param 		height - the height of the map
    @param 		x - X offset from the reference 0 to width
    @param 		y - Y offset from the reference 0 to height
    @return 	none
 --------------------------------------------------------------------------- */
void LIB_PerlinNoise_GenerateMap( uint32_t* pMapHeight, uint32_t width, uint32_t height, float fRef )
{
    uint32_t ulIndex = 0;

    uint32_t xAdd = rand() % 1200;
    uint32_t yAdd = rand() % 1200;
    uint32_t refY = rand() % 1200;

    for( uint32_t ulX = 0; ulX < width; ulX++ )
    {
        float n = 0.0f;
        float a = 1.0f;
        float f = fRef;

        for (uint32_t ulOctave = 0; ulOctave < 8; ulOctave++)
        {
            n += a * LIB_PerlinNoise_Noise2D( (float)(ulX + xAdd) * f, (float)(refY + yAdd) * f);
            a *= 0.5f;
            f *= 2.0f;
        }
        n = (n + 1) * 0.5f;
        int32_t ucValue = 1550 - (n * 1000.0f);

        pMapHeight[ ulX ] = ucValue;

    }
}

/** ---------------------------------------------------------------------------
    @brief 		Linear Interpolation
    @ingroup 	MainShell
    @param 		t - the value to interpolate
    @param 		a - the first value
    @param 		b - the second value
    @return 	float - the interpolated value
 --------------------------------------------------------------------------- */
int32_t LIB_PerlinNoise_IntLerp( int32_t t, int32_t a, int32_t b )
{
    return a + t * ( b - a );
}


//-----------------------------------------------------------------------------
// Internal Functionality
//-----------------------------------------------------------------------------

/** ---------------------------------------------------------------------------
    @brief 		Shuffle the array
    @ingroup 	MainShell
    @param 		pArray - the array to shuffle
    @param 		ulArraySize - the size of the array
    @return 	none
 --------------------------------------------------------------------------- */
static void Shuffle( uint8_t* pArray, uint32_t ulArraySize )
{
    uint32_t ulIndex = 0;
    uint8_t  ucTemp = 0;
    uint32_t ulRandom = 0;

    for( ulIndex = ulArraySize; ulIndex > 0; ulIndex-- )
    {
        ulRandom = randomFloat() * (ulIndex -1);
        ucTemp = pArray[ulIndex];
        pArray[ulIndex] = pArray[ulRandom];
        pArray[ulRandom] = ucTemp;
    }
}

/** ---------------------------------------------------------------------------
    @brief 		Generate a permutation table
    @ingroup 	MainShell
    @return 	pointer to the permutation table
 --------------------------------------------------------------------------- */
static uint8_t* GeneratePermutation( void )
{
    uint8_t* pPermutation = sPerlin.pPermTable;
    uint32_t ulIndex = 0;

    for( ulIndex = 0; ulIndex < 256; ulIndex++ )
    {
        pPermutation[ulIndex] = ulIndex;
    }

    Shuffle( pPermutation, 256 );

    for( ulIndex = 0; ulIndex < 256; ulIndex++ )
    {
        pPermutation[256 + ulIndex] = pPermutation[ ulIndex ];
    }

    return pPermutation;
}

/** ---------------------------------------------------------------------------
    @brief 		Get Constant Vector2
    @ingroup 	MainShell
    @param 		ucValue - the value from the permutation table
    @return 	sVector2 - the vector  
 --------------------------------------------------------------------------- */
static sVector2 GetConstantVector( uint8_t ucValue )
{
    sVector2 sVector = { 0.0f, 0.0f };

    ucValue = ucValue & 3;

    switch( ucValue )
    {
        case 0:
            sVector.x = 1.0f;
            sVector.y = 1.0f;
            break;
        case 1:
            sVector.x = -1.0f;
            sVector.y = 1.0f;
            break;
        case 2:
            sVector.x = -1.0f;
            sVector.y = -1.0f;
            break;
        case 3:
            sVector.x = 1.0f;
            sVector.y = -1.0f;
            break;
        default:
            break;
    }

    return sVector;
}

/** ---------------------------------------------------------------------------
    @brief 		Fade function
    @ingroup 	MainShell
    @param 		t - the value to fade
    @return 	float faded value
 --------------------------------------------------------------------------- */
static float Fade( float t )
{
    return t * t * t * ( t * ( t * 6 - 15 ) + 10 );
}

/** ---------------------------------------------------------------------------
    @brief 		Calculate the Dot Product
    @ingroup 	MainShell
    @param 		sVector  - the first value
    @param 		sVector2 - the second value
    @return 	float - the Dot Product
--------------------------------------------------------------------------- */
float Dot( sVector2 sVector, sVector2 sVector2 )
{
    return sVector.x * sVector2.x + sVector.y * sVector2.y;
}

/** ---------------------------------------------------------------------------
    @brief 		Linear Interpolation
    @ingroup 	MainShell
    @param 		t - the value to interpolate
    @param 		a - the first value
    @param 		b - the second value
    @return 	float - the interpolated value
 --------------------------------------------------------------------------- */
static float Lerp( float t, float a, float b )
{
    return a + t * ( b - a );
}

/** ---------------------------------------------------------------------------
    @brief 		Random number generator
    @ingroup 	MainShell
    @return 	float - the random value betwween 0 - 1.0f
 --------------------------------------------------------------------------- */
static float randomFloat( void )
{
    return 1.0f / (float)rand();
}


//-----------------------------------------------------------------------------
// End of file: LIB_PerlinNoise.c
//-----------------------------------------------------------------------------

