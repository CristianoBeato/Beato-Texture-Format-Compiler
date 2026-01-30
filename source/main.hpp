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

#ifndef __MAIN_HPP__
#define __MAIN_HPP__

class crMainApp
{
public:
    crMainApp( void );
    ~crMainApp( void );

    void    Run( std::stringstream &in_cmdargs );

private:
    uint8_t         m_state;
    SDL::Mutex      m_renderLock;
    SDL::Thread     m_renderThread;
    gl::Context*    m_renderContext;

    /// @brief Initialize GUI 
    void    Init( void );

    /// @brief Release GUI
    void    Release( void );

    /// @brief retrieve input events
    void    Event( void );
    
    /// @brief perform the rendering
    void    Render( void );
    
    /// @brief
    void    PrintHelpText( void );

    /// @brief 
    void    ListFormatText( void );

    static int RendererEntryPoint( void* in_entry );

    /// disable copy contructor
private:
    crMainApp( const crMainApp& ) = delete;
    crMainApp operator =( const crMainApp& ) = delete;


};

#endif //!__MAIN_HPP__