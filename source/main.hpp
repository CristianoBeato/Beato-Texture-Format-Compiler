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

class crImageSource
{
public:
    crImageSource( void );
    ~crImageSource( void );

    /// @brief 
    /// @param in_file 
    /// @return 
    bool    Open( const std::string &in_file );
    
    /// @brief 
    /// @param in_file 
    /// @return 
    bool    Save( const std::string &in_file );
    
    /// @brief 
    /// @param in_file 
    /// @return 
    bool    Export( const std::string &in_file );

    /// @brief 
    /// @param in_file 
    /// @return 
    bool    Import( const std::string &in_file );

    /// @brief
    /// @param
    /// @return
    bool    ConvertFormat( const format_t in_format );

    /// @brief
    /// @param
    /// @return
    bool    GenerateMipMaps( void );

private:
    /// 
    std::shared_ptr<gli::texture>   m_texture;
};

class crMainApp
{
public:
    crMainApp( int argc, char *argv[] );
    ~crMainApp( void );

    void    Run( void );

private:
    uint8_t                     m_state;
    std::vector<std::string>    m_cmdline;
    SDL::Mutex                  m_renderLock;
    SDL::Thread                 m_renderThread;
    gl::Context*                m_renderContext;

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