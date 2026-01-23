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

#ifndef __IMAGE_LOADER_HPP__
#define __IMAGE_LOADER_HPP__

enum error_t : uint16_t
{
    ERR_SUCCES = 0,                 // no error
    ERR_UNKNOW,                     // some error 
    ERR_INVALID_FILE,               // can't open file 
    ERR_INVALID_FORMAT,             // image texel format not suported
    ERR_IMAGE_FORMAT_MISMATCH,      // some image from array use diferent format
    ERR_IMAGE_SIZE_MISMATCH,        // some image from array have diferent size
    ERR_CANT_CREATE_IMAGE           // failed to create the resquested image 
};

class crImageLoader
{
public:
    crImageLoader( void );
    ~crImageLoader( void );

    /// @brief Load a single image 
    /// @param in_path path to image
    /// @return true on sucess 
    uint16_t    LoadImage1D( const std::string &in_path, const bool in_array );
    uint16_t    LoadImage2D( const std::string &in_path );
    uint16_t    LoadImage3D( const std::vector<std::string> &in_paths, const bool in_array );
    uint16_t    LoadCubemap( const std::vector<std::string> &in_paths, const bool in_array );
    
    /// @brief
    uint16_t    SaveImage( const std::string &in_path );
 
private:
    std::shared_ptr<gli::texture>   m_texture;

};

#endif //!__IMAGE_LOADER_HPP__