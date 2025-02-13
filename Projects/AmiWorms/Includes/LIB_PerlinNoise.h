/** ---------------------------------------------------------------------------
	@file		LIB_PerlinNoise.h
	@defgroup 	AmiWorms Apollo V4 Shell
	@brief		Perlin Noise Calucations, using vectors
	@date		2025-10-01
	@version	0.1
	@copyright	Neil Beresford 2025
 -----------------------------------------------------------------------------
	Notes

--------------------------------------------------------------------------- */

#ifndef _LIB_PERLINNOISE_H_
#define _LIB_PERLINNOISE_H_

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Typedefs & Enumerators
//-----------------------------------------------------------------------------

typedef struct
{
    float x;
    float y;

} sVector2, *psVector2;

//-----------------------------------------------------------------------------
// External Functionality
//-----------------------------------------------------------------------------

void    LIB_PerlinNoise_Init( float fSeed );
float   LIB_PerlinNoise_Noise2D( float x, float y );
void 	LIB_PerlinNoise_GenerateMap( uint32_t* pMapHeight, uint32_t width, uint32_t height, float fRef );
int32_t LIB_PerlinNoise_IntLerp( int32_t t, int32_t a, int32_t b );

//-----------------------------------------------------------------------------

#endif // _LIB_PERLINNOI_H_

//-----------------------------------------------------------------------------
// End of file: LIB_PerlinNoise.h
//-----------------------------------------------------------------------------

