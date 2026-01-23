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
#include "imageLoader.hpp"

#include <gli/texture1d.hpp>
#include <gli/texture1d_array.hpp>
#include <gli/texture2d.hpp>
#include <gli/texture2d_array.hpp>
#include <gli/texture3d.hpp>
#include <gli/texture_cube.hpp>
#include <gli/texture_cube_array.hpp>
#include <gli/convert.hpp>
#include <gli/generate_mipmaps.hpp>

#include <SDL3_image/SDL_image.h>

static inline gli::format SDLFormatTOGLi( const SDL_PixelFormat in_format )
{
    gli::format format = gli::FORMAT_UNDEFINED;

    switch ( in_format )
    {
        case SDL_PIXELFORMAT_RGBA4444:
            format = gli::FORMAT_RGBA4_UNORM_PACK16;
            break;
            
        case SDL_PIXELFORMAT_BGRA4444:
            format = gli::FORMAT_BGRA4_UNORM_PACK16;
            break;
            
        case SDL_PIXELFORMAT_XRGB1555:
        case SDL_PIXELFORMAT_ARGB1555:
            format = gli::FORMAT_A1RGB5_UNORM_PACK16;
        
        case SDL_PIXELFORMAT_RGBA5551:
        case SDL_PIXELFORMAT_BGRA5551:
            format = gli::FORMAT_BGR5A1_UNORM_PACK16;
            break;

        case SDL_PIXELFORMAT_RGB565:
            format = gli::FORMAT_R5G6B5_UNORM_PACK16;
            break;

        case SDL_PIXELFORMAT_BGR565:
            format = gli::FORMAT_B5G6R5_UNORM_PACK16;
            break;

        case SDL_PIXELFORMAT_RGB24:
            format = gli::FORMAT_RGB8_UNORM_PACK8;
            break;
        
        case SDL_PIXELFORMAT_BGR24:
            format = gli::FORMAT_BGR8_UNORM_PACK8;
            break;

        case SDL_PIXELFORMAT_RGBX8888:
        case SDL_PIXELFORMAT_RGBA8888:
            format = gli::FORMAT_RGBA8_UNORM_PACK8;
            break;

        case SDL_PIXELFORMAT_BGRX8888:
        case SDL_PIXELFORMAT_BGRA8888:
            format = gli::FORMAT_BGRA8_UNORM_PACK8;
            break;
                    
        case SDL_PIXELFORMAT_RGB48:
        case SDL_PIXELFORMAT_BGR48:
            format = gli::FORMAT_RGB16_UNORM_PACK16;
            break;

        case SDL_PIXELFORMAT_RGB48_FLOAT:
        case SDL_PIXELFORMAT_BGR48_FLOAT:
            format = gli::FORMAT_RGB16_SFLOAT_PACK16;
            break;

        case SDL_PIXELFORMAT_RGBA64:
        case SDL_PIXELFORMAT_BGRA64:
            format = gli::FORMAT_RGBA16_UNORM_PACK16;
            break;

        case SDL_PIXELFORMAT_RGBA64_FLOAT:
        case SDL_PIXELFORMAT_BGRA64_FLOAT:
            format = gli::FORMAT_RGBA16_SFLOAT_PACK16;
            break;

        case SDL_PIXELFORMAT_ARGB64: // wizzle
        case SDL_PIXELFORMAT_ABGR64: // wizzle
            format = gli::FORMAT_RGBA16_UNORM_PACK16;
            break;

        case SDL_PIXELFORMAT_ARGB64_FLOAT: // wizzle
        case SDL_PIXELFORMAT_ABGR64_FLOAT: // wizzle
            format = gli::FORMAT_RGBA16_SFLOAT_PACK16;
            break;

        case SDL_PIXELFORMAT_RGB96_FLOAT:
        case SDL_PIXELFORMAT_BGR96_FLOAT:
            format = gli::FORMAT_RGB32_SFLOAT_PACK32;
            break;

        case SDL_PIXELFORMAT_RGBA128_FLOAT:
        case SDL_PIXELFORMAT_BGRA128_FLOAT:
            format = gli::FORMAT_RGB32_SFLOAT_PACK32;    
            break;

        case SDL_PIXELFORMAT_ARGB128_FLOAT:
        case SDL_PIXELFORMAT_ABGR128_FLOAT:
            format = gli::FORMAT_RGBA32_SFLOAT_PACK32;
            break;

    default: 
        format = gli::FORMAT_UNDEFINED;
        break;
    }

    return format;
}

crImageLoader::crImageLoader( void )
{
}

crImageLoader::~crImageLoader( void )
{
}

uint16_t crImageLoader::LoadImage1D( const std::string &in_path, const bool in_array )
{
    gli::format             format;
	gli::extent1d           extent;
    SDL::Surface            surface;

    /// load source image
    if ( ( surface = IMG_Load( in_path.c_str() ) ) == nullptr)
        return ERR_INVALID_FILE;
    
    // get image format
    format = SDLFormatTOGLi( surface.Format() );
    
    /// check if current color format is suported
    if ( format == gli::FORMAT_UNDEFINED )
        return ERR_INVALID_FORMAT;

    extent.x = surface.Width();

    if ( in_array )
    {
        size_t layers = surface.Height();
        m_texture = std::make_shared<gli::texture1d_array>( format, extent, layers, 1 );    
        if ( !m_texture->empty() )
            return ERR_CANT_CREATE_IMAGE;

        for (size_t layer = 0; layer < layers; ++layer) 
        {
            // We get the pointer to the beginning of the 'layer' layer in GLI (mip level 0)
            void* dst_ptr = m_texture->data(layer, 0, 0); 
    
            // We get the pointer to the corresponding line on the SDL surface
            // The SDL pitch ensures we get the correct line even with alignments        
            uint8_t* src_ptr = static_cast<uint8_t*>( surface.Pixels() ) + ( layer * surface.Pitch() );

            // O tamanho a copiar é a largura da textura * tamanho do bloco do formato
            size_t row_size = gli::block_size(format) * extent.x;

            std::memcpy( dst_ptr, src_ptr, row_size );
        }
    }
    else
    {    
        /// create base image
        m_texture = std::make_shared<gli::texture1d>( format, extent, 1 );    
        if ( !m_texture->empty() )
            return ERR_CANT_CREATE_IMAGE;

        /// direct copy the row
        std::memcpy( m_texture->data(), surface.Pixels(), m_texture->size() );
    }

    return ERR_SUCCES;
}

uint16_t crImageLoader::LoadImage2D( const std::string &in_path)
{
    gli::format             format;
	gli::extent2d           extent;
    SDL::Surface            surface;

    /// load source image
    if ( ( surface = IMG_Load( in_path.c_str() ) ) == nullptr)
        return ERR_INVALID_FILE;
        
    // get image format
    format = SDLFormatTOGLi( surface.Format() );
    
    ///
    if( format == gli::FORMAT_UNDEFINED )
        return ERR_INVALID_FORMAT;

    extent.x = surface.Width();
    extent.y = surface.Height();

    /// create base image
    m_texture = std::make_shared<gli::texture2d>( format, extent, 1 );    
    if ( !m_texture->empty() )
        return ERR_CANT_CREATE_IMAGE;
            
    // If the SDL pitch is exactly equal to the texture width, we can use memcpy directly
    if ( surface.Pitch() * surface.Height() == m_texture->size() ) 
    {
        std::memcpy( m_texture->data(), surface.Pixels(), m_texture->size() );
    } 
    else 
    {
        // Otherwise, we copy line by line to respect the alignment.
        uint8_t* dst = static_cast<uint8_t*>( m_texture->data() );
        uint8_t* src = static_cast<uint8_t*>( surface.Pixels() );
        size_t row_size = gli::block_size( format ) * extent.x; 
    
        for (int y = 0; y < surface.Height(); ++y ) 
        {
            std::memcpy(dst + (y * row_size), src + (y * surface.Pitch() ), row_size );
        }
    }

    // release texture
    SDL_DestroySurface( surface );

    return ERR_SUCCES;
}

uint16_t crImageLoader::LoadImage3D( const std::vector<std::string> &in_paths, const bool in_array )
{
    uint32_t                    width = 0;
    uint32_t                    height = 0;
    gli::format                 format = gli::FORMAT_UNDEFINED;
	std::vector<SDL::Surface>   surfaces;

    surfaces.resize( in_paths.size() );

    /// load source image array 
    for (size_t i = 0; i < in_paths.size(); i++)
    {
        /// load image by image
        if ( ( surfaces[i] = IMG_Load( in_paths[i].c_str() ) ) == nullptr)
            return ERR_INVALID_FILE;
        
        /// get format 
        if ( format == gli::FORMAT_UNDEFINED )
        {
            // get image format
            format = SDLFormatTOGLi( surfaces[i].Format() );
    
            /// check if current color format is suported
            if ( format == gli::FORMAT_UNDEFINED )
                return ERR_INVALID_FORMAT;
        }
        else
        {
            /// all image need to be in the same texel format
            if ( SDLFormatTOGLi( surfaces[i].Format() ) != format )
                return ERR_IMAGE_FORMAT_MISMATCH;
        }

        if ( width != 0 || height != 0 )
        {
            /// image are diferent from the base array
            if ( width != surfaces[i].Width() || height != surfaces[i].Height() )
                return ERR_IMAGE_SIZE_MISMATCH;
        }
    }
    
    if ( in_array )
    {
        size_t  layers = surfaces.size();
        gli::extent2d extent;
        extent.x = width;
        extent.y = height;
        m_texture = std::make_shared<gli::texture2d_array>( format, extent, layers, 1 );

        for (size_t layer = 0; layers < layers; layers++)
        {
            auto surface = surfaces[layer];

            // If the SDL pitch is exactly equal to the texture width, we can use memcpy directly on the slice.
            if ( surface.Pitch() * surface.Height() == m_texture->size() ) 
            {
                std::memcpy( m_texture->data( layer, 0, 0 ), surface.Pixels(), m_texture->size() );
            } 
            else 
            {
                // Otherwise, we copy line by line to respect the SDL alignment.
                uint8_t* dst = static_cast<uint8_t*>( m_texture->data( layer, 0, 0 ) );
                uint8_t* src = static_cast<uint8_t*>( surface.Pixels() );
                size_t row_size = gli::block_size( format ) * extent.x; 
            
                for (int y = 0; y < surface.Height(); ++y ) 
                {
                    std::memcpy(dst + (y * row_size), src + (y * surface.Pitch() ), row_size );
                }
            }
        }        
    }
    else
    {
        gli::extent3d extent;
        extent.x = width;
        extent.y = height;
        extent.z = static_cast<int>( surfaces.size() );
        m_texture = std::make_shared<gli::texture3d>( format, extent, 1 );

        for ( size_t i = 0; i < surfaces.size(); i++)
        {
            auto surface = surfaces[i];

            // We need to calculate the offset for slice 'i'.
            // GLI organizes 3D textures by: Slice (Z) -> Line (Y) -> Pixel (X)
    
            // slice size in bytes: ( width * height * bytes_per_pixel )
            size_t bytes_per_pixel = gli::block_size(m_texture->format());
            size_t slice_size = m_texture->extent().x * m_texture->extent().y * bytes_per_pixel;
    
            // Destination pointer for slice 'i' at mipmap level 0
            uint8_t* dst_slice = static_cast<uint8_t*>(m_texture->data(0, 0, 0)) + (i * slice_size);

            // Now we copy the data from the surface to the slice.
            // If the SDL pitch is exactly equal to the texture width, we can use memcpy directly on the slice.
            if ( surface.Pitch() == m_texture->extent().x * bytes_per_pixel) 
            {
                std::memcpy(dst_slice, surface.Pixels(), slice_size);
            } 
            else 
            {
                // Otherwise, we copy line by line to respect the SDL alignment.
                size_t row_size = m_texture->extent().x * bytes_per_pixel;
                for (int y = 0; y < surface.Height(); ++y) 
                {
                    uint8_t* src_row = static_cast<uint8_t*>( surface.Pixels() ) + (y * surface.Pitch() );
                    uint8_t* dst_row = dst_slice + (y * row_size);
                    std::memcpy(dst_row, src_row, row_size);
                }
            }
        }
    }

    return ERR_SUCCES;
}

uint16_t crImageLoader::SaveImage(const std::string &in_path)
{
    return ERR_SUCCES;
}
