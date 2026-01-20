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

#ifndef __SDLGL_CONTEXT_HPP__
#define __SDLGL_CONTEXT_HPP__

#include "crglContext.hpp"

class SDLGLContext : public gl::Context
{
public:
    SDLGLContext( void );
    ~SDLGLContext( void );

    virtual bool    Create( const void* in_windowHandle);
    virtual void    Destroy( void );
    virtual bool    MakeCurrent( void );
    virtual bool    Release( void );
    virtual bool    SwapBuffers( void );
    virtual void*   GetFunctionPointer( const char* in_name ) const;
    virtual void    DebugOuput( const char* in_message ) const;

private:    
    SDL::Window     m_window;
    SDL_GLContext   m_context;
};


#endif //!__SDLGL_CONTEXT_HPP__