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
#include "SDLGLContext.hpp"

SDLGLContext::SDLGLContext( void ) :
    m_window( nullptr ),
    m_context( nullptr )
{
}

SDLGLContext::~SDLGLContext(void)
{
}

bool SDLGLContext::Create(const void *in_windowHandle)
{
    //
    if ( !m_window.Create( "BTF-Edit", 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE ) )
        throw std::runtime_error( SDL_GetError() );

    // 4.5 core
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 5 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    
    // debug output enable
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG );

    // 32bit color
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

    // depth stencil 24 8
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );

    // sRGB enable
    SDL_GL_SetAttribute( SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1 );

    // 4x MSAA
    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 4 );

    // use double buffer
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    // Create the OpenGL Context
    if( !( m_context = SDL_GL_CreateContext( m_window ) ) )
        throw std::runtime_error( SDL_GetError() );

    gl::Context::Init();
    
    return true;
}

void SDLGLContext::Destroy(void)
{
    if( m_context != nullptr )
    {
        SDL_GL_DestroyContext( m_context );
        m_context = nullptr;
    }

    if( m_window != nullptr )
    {
        SDL_DestroyWindow( m_window );
        m_window = nullptr;
    }
}

bool SDLGLContext::MakeCurrent(void)
{
    return SDL_GL_MakeCurrent( m_window, m_context );
}

bool SDLGLContext::Release(void)
{
    return SDL_GL_MakeCurrent( m_window, nullptr );
}

bool SDLGLContext::SwapBuffers(void)
{
    return SDL_GL_SwapWindow( m_window );
}

void *SDLGLContext::GetFunctionPointer(const char *in_name) const
{
    return reinterpret_cast<void*>( SDL_GL_GetProcAddress( in_name ) );
}

void SDLGLContext::DebugOuput(const char *in_message) const
{
    std::cout << in_message << std::endl;
}
