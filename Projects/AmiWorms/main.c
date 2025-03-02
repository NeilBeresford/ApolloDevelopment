/** ---------------------------------------------------------------------------
    @file		main.c
    @defgroup 	AmiWormsAmiWorms
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
#include "stdlib.h"
#include "math.h"
#include "string.h"
#include "time.h"
#include "Includes/defines.h"
#include "Includes/GlobalData.h"
#include "Includes/Hardware.h"
#include "Includes/HWScreen.h"
#include "Includes/FontModule.h"
#include "Includes/LIB_ApolloInput.h"
#include "Includes/LIB_Files.h"
#include "Includes/LIB_Sprites.h"
#include "Includes/LIB_PerlinNoise.h"
#include "Includes/LIB_SpriteFont.h"
#include "Includes/LIB_SprManager.h"
#include "Includes/GAME_Player.h"
#include "Includes/ResourceFiles.h"
#include "Includes/ResourceHandling.h"
#include "Modules/Module-Scene.h"
#include "Scenes/Scene_Intro.h"
#include "Scenes/Scene_Game.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

uint32_t* palettes[ 30 ];
uint8_t*  paletteFiles[ 30 ] = {
    "Data/Palettes/paletteArt.bin",       "Data/Palettes/palette-Beach.bin",   "Data/Palettes/paletteCheese.bin", "Data/Palettes/paletteConstruction.bin", "Data/Palettes/palette-Desert.bin",  "Data/Palettes/paletteDesert.bin",
    "Data/Palettes/paletteDungeon.bin",   "Data/Palettes/paletteEaster.bin",   "Data/Palettes/palette-Farm.bin",  "Data/Palettes/paletteFarm.bin",         "Data/Palettes/palette-Forest.bin",  "Data/Palettes/paletteForest.bin",
    "Data/Palettes/paletteFruit.bin",     "Data/Palettes/paletteGulf.bin",     "Data/Palettes/palette-Hell.bin",  "Data/Palettes/paletteHell.bin",         "Data/Palettes/paletteHospital.bin", "Data/Palettes/paletteJungle.bin",
    "Data/Palettes/paletteManhattan.bin", "Data/Palettes/paletteMedieval.bin", "Data/Palettes/paletteMusic.bin",  "Data/Palettes/palettePirate.bin",       "Data/Palettes/paletteSnow.bin",     "Data/Palettes/paletteSpace.bin",
    "Data/Palettes/paletteSports.bin",    "Data/Palettes/paletteTentacle.bin", "Data/Palettes/paletteTime.bin",   "Data/Palettes/paletteTools.bin",        "Data/Palettes/paletteTribal.bin",   "Data/Palettes/paletteUrban.bin",
};

PSPRHANDLE handles[ 256 ];

//-----------------------------------------------------------------------------
// Code
//-----------------------------------------------------------------------------

/** ---------------------------------------------------------------------------
    @brief 		Main entry point for theAmiWorms
    @ingroup 	AmiWorms
    @return 	int - return code, 0 success
 --------------------------------------------------------------------------- */
uint32_t main( int argc, char* argv[] )
{

    uint32_t keyReturn = 0;
    uint8_t  Banner[]  = "\n"
                         "An Apollo v4 Production - by Neil Beresford\n"
                         "-------------------------------------------\n\n"
                         "    _              _    __        __                       \n"
                         "   / \\   _ __ ___ (_)   \\ \\      / /__  _ __ _ __ ___  ___ \n"
                         "  / _ \\ | '_ ` _ \\| |____\\ \\ /\\ / / _ \\| '__| '_ ` _ \\/ __|\n"
                         " / ___ \\| | | | | | |_____\\ V  V / (_) | |  | | | | | \\__ \\ \n"
                         "/_/   \\_\\_| |_| |_|_|      \\_/\\_/ \\___/|_|  |_| |_| |_|___/\n\n\n"
                         "Written using ApolloCrossDev and Visual Studio Code\n\n";

    printf( Banner );
    printf( "Press 'ESC' to exit\n" );

    // Initialize the system and hardware
    GlobalData_Init();
    LIB_Sprites_Init();
    LIB_SprManager_Init();

    ResourceHandling_Init();
    for ( uint32_t nFile = 0; nFile < 30; nFile++ )
    {
        if ( LIB_Files_Load( paletteFiles[ nFile ], &palettes[ nFile ], NULL ) == false )
        {
            printf( "Failed to load palette %s\n", paletteFiles[ nFile ] );
        }
    }

    // load all the sprite groups
    ResourceHandling_LoadGroups( theFileGroups );
    LIB_SpriteFont_CalcFontWidthOffsets();
    uint32_t nTotalSprs = ResourceHandling_GetTotalNumSprites();
    ResourceHandling_InitStatus( theFileGroups );
    ResourceHandling_GetTotalNumSprites();
    ResourceHandling_ScanAndSetSpriteDimentions();
    Hardware_SetBackscreenBuffers();

    Hardware_Init();
    HWSCREEN_SetImagePalette( palettes[ 0 ] );

    // Initialize Player Control
    GAME_Player_Init();

    // setup the scenes...
    ModuleScene_Init();
    ModuleScene_RegisterScene( 0, SceneGame_Init, SceneGame_Close, SceneGame_Draw, SceneGame_Update );
    ModuleScene_RegisterScene( 1, SceneIntro_Init, SceneIntro_Close, SceneIntro_Draw, SceneIntro_Update );
    ModuleScene_SetActiveScene( 1 );

    // main loop -
    while ( true )
    {
        sGlobalData.ulFrames++;
        Hardware_WaitVBL();
        Hardware_FlipScreen();

        // process the game logic
        ModuleScene_Draw();
        ModuleScene_Update();
        if ( sGlobalData.GameEnded == true )
            break;

        LIB_SprManager_Update();
    }

    // terminate the program
    Hardware_Close();

    return 0;
}

//-----------------------------------------------------------------------------
// End of File: main.c
//-----------------------------------------------------------------------------
