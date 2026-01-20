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

#ifndef __BTF_LOADER_HPP__
#define __BTF_LOADER_HPP__

class BTFTexture
{
public:
    BTFTexture( void );
    ~BTFTexture( void );

protected:
    std::string                     m_name;
    BTF_Header_t                    m_header;
    BTF_Footer_t                    m_footer;
    std::vector<BTF_SubImage_t>     m_subimages;
    void*                           m_pixelBuffer;
};

class BTFWriter : public BTFTexture
{
public:
    BTFWriter( void );
    ~BTFWriter( void );

    bool    Save( const std::string &in_path );

private:
    SDL::IO::Stream     m_writeStream;
};

class BTFLoader : public BTFTexture
{
public:
    BTFLoader( void );
    ~BTFLoader( void );

    bool    Load( const std::string &in_path );

private:
    SDL::IO::Stream     m_readStream;
};

#endif //!__BTF_LOADER_HPP__