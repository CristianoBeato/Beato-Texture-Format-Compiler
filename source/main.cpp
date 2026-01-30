/*
===========================================================================================
    This file is part of BTFEdit BTF Texture format framework.

    Copyright (c) 2026 Cristiano B. Santos <cristianobeato_dm@hotmail.com>
    Contributor(s): none yet.

-------------------------------------------------------------------------------------------

 This file is part of the BTFEdit library and is licensed under the
 MIT License with Attribution Requirement.

 You are free to use, modify, and distribute this file (even commercially),
 as long as you give credit to the original author:

     “Based on BTFEdit by Cristiano Beato – https://github.com/CristianoBeato”

 For full license terms, see the LICENSE file in the root of this repository.
===============================================================================================
*/

#include "precompiled.hpp"
#include "main.hpp"
#include "SDLGLContext.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static const char* k_NOGUI      = "--nogui";
static const char* k_IMPORT     = "--import";
static const char* k_IMPORTR    = "-i";
static const char* k_EXPORT     = "--export";
static const char* k_EXPORTR    = "-e";
static const char* k_OUTPUT     = "--output";
static const char* k_OUTPUTR    = "-o";
static const char* k_LOAD       = "--load";
static const char* k_LOADR      = "-l";
static const char* k_SRGB       = "--sRGB";
static const char* k_LIST       = "--list";
static const char* k_HELP       = "--help";
static const char* k_HELP_TEXT = 
{
    "Beato Texture Format Compiler v0.1\n"
    "--nogui        run on console only ( comand line compile decompile ) \n"
    "--import or -i import image\n"
    "--export or -e export imges from the open source\n"
    "--format or -f convert image to a specific format\n"
    "--output or -o output texture\n"
    "--load   or -l input texture for export\n"
    "--sRGB   or -s export texture in sRGB color"
    "--list         list suported formats and number index\n"
    "--help         print help\n"
};

static struct format_info_t
{
    uint16_t        id;     /// command line format id
    const char*     name;   /// format name 
    const char*     desc;   /// format decription

} 
formats_info[FORMAT_COUNT]
{
    { FORMAT_RED8U,     "R8U",      "8  bits unsigned normalized 1 component ( red only ) ( Suport sRGB )" },
    { FORMAT_RED16U,    "R16U",     "16 bits unsigned normalized 1 component ( red only )" },
    { FORMAT_RG8U,      "RG8U",     "8  bits unsigned normalized 2 components ( red green ) ( Suport sRGB )" },
    { FORMAT_RG16U,     "RG16U",    "16 bits unsigned normalized 2 components ( red green )" },
    { FORMAT_RGBA8U,    "RGBA8U",   "8  bits unsigned 4 components ( RGB + Alpha ) ( Suport sRGB )" },
    { FORMAT_RGBA16U,   "RGBA16U",  "16 bits unsigned 4 components ( RGB + Alpha )" },
    { FORMAT_RGBA16F,   "RGBA16F",  "RGBA16F 16 bits half float 4 components ( RGB + Alpha )" },
    { FORMAT_RGBA32U,   "RGBA32U",  "32 bits unsigned 4 components ( RGB + Alpha )" },
    { FORMAT_RGBA32F,   "RGBA32F",  "32 bits true float 4 components ( RGB + Alpha )" },
    { FORMAT_BC1,       "BC1",      "4 bits per pixel (bpp), 4×4 block, RGB (no Alpha), 6:1 compression ratio ( DXT1 ) ( Suport sRGB )" },
    { FORMAT_BC3,       "BC3",      "8 bits per pixel (bpp), 4×4 block, RGBA, high quality Alpha (interpolated) ( DXT5 ) ( Suport sRGB )" },
    { FORMAT_BC5,       "BC5",      "8 bits per pixel (bpp), 4×4 block, 2 components ( RG ), ideal for Normal Maps ( RGTC2 )" },
    { FORMAT_BC7,       "BC7",      "8 bits per pixel (bpp), 4×4 block, high quality RGBA, reduces artifacts in gradients ( BPTC ) ( Suport sRGB )" },
    { FORMAT_BC6H,      "BC6H",     "8 bits per pixel (bpp), 4×4 block, RGB Half Float ( HDR support )" },
    { FORMAT_ETC2,      "ETC2",     "8 bits per pixel (bpp), 4×4 block, 4 components ( RGBA ) ( Suport sRGB )" },
    { FORMAT_EAC,       "EAC",      "8 bits per pixel (bpp), 4×4 block, 2 components ( RG ), high precision (11 bits per channel)" }
};

crMainApp::crMainApp( void ) :
    m_state( 0 ),
    m_renderContext( nullptr )
{
}

crMainApp::~crMainApp( void )
{
}

void crMainApp::Run( std::stringstream &in_cmdargs )
{
    bool noGUI = false;
    std::string cmd;

    while (  in_cmdargs >> cmd )
    {
        // disable program in GUI mode 
        if ( cmd.compare( k_NOGUI ) == 0 )
        {
            noGUI = true;
            continue;
        }

        // display ( in console ) the help string
        if ( cmd.compare( k_HELP ) == 0 )
        {
            PrintHelpText();
            exit( EXIT_SUCCESS );
        }

        // display suported formats
        if ( cmd.compare( k_LIST ) == 0 )
        {
            ListFormatText();
            exit( EXIT_SUCCESS );
        }
    }

    /// init in GUI mode
    if ( !noGUI )
    {
        Init();
        while ( m_state )
        {
            Event();
        }
        Release();
    }
}

void crMainApp::Init(void)
{
    /// Initialize SDL
    if ( !SDL_Init( SDL_INIT_EVENTS | SDL_INIT_VIDEO ) )
        throw std::runtime_error( SDL_GetError() );

    // create window and render context
    m_renderContext = new SDLGLContext();
    m_renderContext->Create( nullptr );
    m_renderContext->Release();

    // state 2 runnign
    m_state = 1;

    /// create render thread
    if( !m_renderThread.Create( RendererEntryPoint, "RenderThread", this ) )
        throw std::runtime_error( SDL_GetError() );
}

void crMainApp::Release(void)
{
    /// Release OpenGL Render Context and window
    if ( m_renderContext != nullptr )
    {
        m_renderContext->MakeCurrent();
        m_renderContext->Destroy();
        delete m_renderContext;
    }

    /// release SDL
    SDL_Quit();

}

void crMainApp::Event(void)
{
    SDL_Event e{};
    if( !SDL_WaitEvent( &e ) )
        throw std::runtime_error( SDL_GetError() );

    // exit call 
    if ( e.type == SDL_EVENT_QUIT || e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED )
    {
        m_state = 0;
        return;
    }
    

}

void crMainApp::Render(void)
{    
    glClearColor( 0.45f, 0.45f, 0.45f, 1.0f );

    /// render loop
    do
    {
        m_renderLock.Lock();
        glClear( GL_COLOR_BUFFER_BIT );

        //
        m_renderContext->SwapBuffers();
        m_renderLock.Unlock();

    } while ( m_state );
}

void crMainApp::PrintHelpText(void)
{
    std::cout << k_HELP_TEXT << std::endl;
}

void crMainApp::ListFormatText(void)
{
    for (size_t i = 0; i < FORMAT_COUNT; i++)
    {
        std::cout << formats_info[i].id << " " << formats_info[i].name << "\t" << formats_info[i].desc << std::endl;
    }
}

int crMainApp::RendererEntryPoint(void *in_entry)
{
    crMainApp * mainApp = static_cast<crMainApp*>( in_entry );
    mainApp->m_renderContext->MakeCurrent();
    mainApp->Render();
    mainApp->m_renderContext->Release();
    return 0;
}

int main(int argc, char *argv[])
{
    bool noGUI = false;
    std::stringstream cmdargs;

    /// aquire comand line arguments
    for ( int i = 0; i < argc; i++)
    {
        cmdargs <<  argv[i];
    }
    
    try
    {
            crMainApp app = crMainApp();
            app.Run( cmdargs );
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}