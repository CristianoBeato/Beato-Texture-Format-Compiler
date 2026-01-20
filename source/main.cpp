/*
===========================================================================================
    This file is part of BTFEdit BTF Texture format framework.

    Copyright (c) 2026 Cristiano B. Santos <cristianobeato_dm@hotmail.com>
    Contributor(s): none yet.

-------------------------------------------------------------------------------------------

 This file is part of the crglLib library and is licensed under the
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

crMainApp::crMainApp( void ) :
    m_state( 0 ),
    m_renderContext( nullptr )
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

crMainApp::~crMainApp( void )
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

void crMainApp::Run(const std::vector<std::string> &in_cmdargs)
{
    while ( m_state )
    {
        Event();
    }
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
    std::vector<std::string> cmdargs;

    for ( int i = 0; i < argc; i++)
    {
        cmdargs.push_back( std::string( argv[i] ) );
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