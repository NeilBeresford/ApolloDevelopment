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

#define TOTAL_TRACKS         4
#define TOTAL_SPEECH_SAMPLES 58

//-----------------------------------------------------------------------------
// Typedefs and Enums
//-----------------------------------------------------------------------------

typedef struct
{
    uint8_t  strName[ 128 ];
    uint8_t* pMusicData;
    uint32_t nMusicSize;

} sMusicData, *pMusicData;

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------

void Main_LoadMusic( void );
void Main_Start_Track( uint32_t nTrack );

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------

sMusicData MusicData[ TOTAL_TRACKS ] = {
    {"Data/Music/title.aiff",                   NULL, 0},
    //{"Data/Music/ingame-01-generic.aiff",       NULL, 0},
    {"Data/Music/ingame-09-hell.aiff",          NULL, 0},
    {"Data/Music/ingame-10-mech-workshop.aiff", NULL, 0},
    {"Data/Music/ingame-11-rainsurf.aiff",      NULL, 0},
};

sMusicData SpeechData[ TOTAL_SPEECH_SAMPLES ] = {
    {"Data/Speech/AMAZING.aiff",         NULL, 0},
    {"Data/Speech/BORING.aiff",          NULL, 0},
    {"Data/Speech/BRILLIANT.aiff",       NULL, 0},
    {"Data/Speech/BUMMER.aiff",          NULL, 0},
    {"Data/Speech/BUNGEE.aiff",          NULL, 0},
    {"Data/Speech/BYEBYE.aiff",          NULL, 0},
    {"Data/Speech/COLLECT.aiff",         NULL, 0},
    {"Data/Speech/COMEONTHEN.aiff",      NULL, 0},
    {"Data/Speech/COWARD.aiff",          NULL, 0},
    {"Data/Speech/DRAGONPUNCH.aiff",     NULL, 0},
    {"Data/Speech/DROP.aiff",            NULL, 0},
    {"Data/Speech/EXCELLENT.aiff",       NULL, 0},
    {"Data/Speech/FATALITY.aiff",        NULL, 0},
    {"Data/Speech/FIRE.aiff",            NULL, 0},
    {"Data/Speech/FIREBALL.aiff",        NULL, 0},
    {"Data/Speech/FIRSTBLOOD.aiff",      NULL, 0},
    {"Data/Speech/FLAWLESS.aiff",        NULL, 0},
    {"Data/Speech/GOAWAY.aiff",          NULL, 0},
    {"Data/Speech/GRENADE.aiff",         NULL, 0},
    {"Data/Speech/HELLO.aiff",           NULL, 0},
    {"Data/Speech/HMM.aiff",             NULL, 0},
    {"Data/Speech/HURRY.aiff",           NULL, 0},
    {"Data/Speech/ILLGETYOU.aiff",       NULL, 0},
    {"Data/Speech/INCOMING.aiff",        NULL, 0},
    {"Data/Speech/JUMP1.aiff",           NULL, 0},
    {"Data/Speech/JUMP2.aiff",           NULL, 0},
    {"Data/Speech/JUSTYOUWAIT.aiff",     NULL, 0},
    {"Data/Speech/KAMIKAZE.aiff",        NULL, 0},
    {"Data/Speech/LAUGH.aiff",           NULL, 0},
    {"Data/Speech/LEAVEMEALONE.aiff",    NULL, 0},
    {"Data/Speech/MISSED.aiff",          NULL, 0},
    {"Data/Speech/NOOO.aiff",            NULL, 0},
    {"Data/Speech/OHDEAR.aiff",          NULL, 0},
    {"Data/Speech/OINUTTER.aiff",        NULL, 0},
    {"Data/Speech/OOFF1.aiff",           NULL, 0},
    {"Data/Speech/OOFF2.aiff",           NULL, 0},
    {"Data/Speech/OOFF3.aiff",           NULL, 0},
    {"Data/Speech/OOPS.aiff",            NULL, 0},
    {"Data/Speech/ORDERS.aiff",          NULL, 0},
    {"Data/Speech/OUCH.aiff",            NULL, 0},
    {"Data/Speech/OW1.aiff",             NULL, 0},
    {"Data/Speech/OW2.aiff",             NULL, 0},
    {"Data/Speech/OW3.aiff",             NULL, 0},
    {"Data/Speech/PERFECT.aiff",         NULL, 0},
    {"Data/Speech/REVENGE.aiff",         NULL, 0},
    {"Data/Speech/RUNAWAY.aiff",         NULL, 0},
    {"Data/Speech/STUPID.aiff",          NULL, 0},
    {"Data/Speech/TAKECOVER.aiff",       NULL, 0},
    {"Data/Speech/TRAITOR.aiff",         NULL, 0},
    {"Data/Speech/UH-OH.aiff",           NULL, 0},
    {"Data/Speech/VICTORY.aiff",         NULL, 0},
    {"Data/Speech/walk-compress.aiff",   NULL, 0},
    {"Data/Speech/walk-expand.aiff",     NULL, 0},
    {"Data/Speech/WATCHTHIS.aiff",       NULL, 0},
    {"Data/Speech/WHATTHE.aiff",         NULL, 0},
    {"Data/Speech/WOBBLE.aiff",          NULL, 0},
    {"Data/Speech/YESSIR.aiff",          NULL, 0},
    {"Data/Speech/YOULLREGRETTHAT.aiff", NULL, 0},
};

uint32_t* palettes[ 30 ];
uint8_t*  paletteFiles[ 30 ] = {
    "Data/Palettes/paletteArt.bin",       "Data/Palettes/palette-Beach.bin",   "Data/Palettes/paletteCheese.bin", "Data/Palettes/paletteConstruction.bin", "Data/Palettes/palette-Desert.bin",  "Data/Palettes/paletteDesert.bin",
    "Data/Palettes/paletteDungeon.bin",   "Data/Palettes/paletteEaster.bin",   "Data/Palettes/palette-Farm.bin",  "Data/Palettes/paletteFarm.bin",         "Data/Palettes/palette-Forest.bin",  "Data/Palettes/paletteForest.bin",
    "Data/Palettes/paletteFruit.bin",     "Data/Palettes/paletteGulf.bin",     "Data/Palettes/palette-Hell.bin",  "Data/Palettes/paletteHell.bin",         "Data/Palettes/paletteHospital.bin", "Data/Palettes/paletteJungle.bin",
    "Data/Palettes/paletteManhattan.bin", "Data/Palettes/paletteMedieval.bin", "Data/Palettes/paletteMusic.bin",  "Data/Palettes/palettePirate.bin",       "Data/Palettes/paletteSnow.bin",     "Data/Palettes/paletteSpace.bin",
    "Data/Palettes/paletteSports.bin",    "Data/Palettes/paletteTentacle.bin", "Data/Palettes/paletteTime.bin",   "Data/Palettes/paletteTools.bin",        "Data/Palettes/paletteTribal.bin",   "Data/Palettes/paletteUrban.bin",
};

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
    Main_LoadMusic();

    Hardware_Init();
    HWSCREEN_SetImagePalette( palettes[ 0 ] );

    // Initialize Player Control
    GAME_Player_Init();

    // setup the scenes...
    ModuleScene_Init();
    ModuleScene_RegisterScene( 0, SceneGame_Init, SceneGame_Close, SceneGame_Draw, SceneGame_Update );
    ModuleScene_RegisterScene( 1, SceneIntro_Init, SceneIntro_Close, SceneIntro_Draw, SceneIntro_Update );
    ModuleScene_SetActiveScene( 1 );

    Main_Start_Track( 0 );

    // main loop -
    uint32_t nTestCount = 0;

    while ( true )
    {
        sGlobalData.ulFrames++;
        Hardware_WaitVBL();
        Hardware_FlipScreen();

        if ( ++nTestCount > 500 )
        {
            nTestCount    = 0;
            int32_t voice = rand() % TOTAL_SPEECH_SAMPLES;
            // Hardware_StartAudio( 2, (uint32_t)SpeechData[ voice ].pMusicData, SpeechData[ voice ].nMusicSize, 0x8080, 7, 280 );
        }

        // process the game logic
        ModuleScene_Draw();
        ModuleScene_Update();

        // Scene control...
        if ( sGlobalData.GameEnded == true )
            break;
        if ( sGlobalData.nNewScene != NO_SCENE_SET )
        {
            LIB_SprManager_RemoveAll();

            #if 0
            if ( sGlobalData.nNewScene == 1 )
            {
                // terminate the program
                Hardware_Close();
                exit( 0 ); // terminate the program
            }
            #endif
            ModuleScene_SetActiveScene( sGlobalData.nNewScene );
            sGlobalData.nNewScene = NO_SCENE_SET;
        }

        LIB_SprManager_Update();
    }

    // terminate the program
    Hardware_Close();

    return 0;
}

/** ---------------------------------------------------------------------------
    @brief 		Load the music
    @ingroup 	AmiWorms
 --------------------------------------------------------------------------- */
void Main_LoadMusic( void )
{
    uint8_t* pMusicData;
    uint32_t nMusicSize;

    for ( int32_t nIndex = 0; nIndex < TOTAL_TRACKS; nIndex++ )
    {
        if ( LIB_Files_Load( MusicData[ nIndex ].strName, &pMusicData, &nMusicSize ) == true )
        {
            MusicData[ nIndex ].pMusicData = pMusicData;
            MusicData[ nIndex ].nMusicSize = nMusicSize;
        }
    }

    for ( int32_t nIndex = 0; nIndex < TOTAL_SPEECH_SAMPLES; nIndex++ )
    {
        if ( LIB_Files_Load( SpeechData[ nIndex ].strName, &pMusicData, &nMusicSize ) == true )
        {
            SpeechData[ nIndex ].pMusicData = pMusicData;
            SpeechData[ nIndex ].nMusicSize = nMusicSize;
        }
    }
}

/** ---------------------------------------------------------------------------
    @brief 		Start the music
    @ingroup 	AmiWorms
    @param      nTrack      - The track to start
 --------------------------------------------------------------------------- */
void Main_Start_Track( uint32_t nTrack )
{
    if ( nTrack < TOTAL_TRACKS )
    {
        Hardware_StartAudio( 0, (uint32_t)MusicData[ nTrack ].pMusicData, MusicData[ nTrack ].nMusicSize, 0x4040, 5, 120 );
    }
}

//-----------------------------------------------------------------------------
// End of File: main.c
//-----------------------------------------------------------------------------
