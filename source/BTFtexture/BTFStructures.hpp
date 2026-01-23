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

#ifndef __BTF_STRUCTURES_HPP__
#define __BTF_STRUCTURES_HPP__

/// @brief image pixel depth format
enum format_t : uint16_t
{
    FORMAT_RED8U,   // 8  bits unsigned 1 component ( red only )
    FORMAT_RED16U,  // 16 bits unsigned 1 component ( rend only ) 
    FORMAT_RG8U,    // 8  bits unsigned 2 components ( red green ) 
    FORMAT_RG16U,   // 16 bits unsigned 2 components ( red green ) 
    FORMAT_RGBX8U,  // 8  bits unsigned 3 components + 1 padding ( RGB + X )
    FORMAT_RGBA8U,  // 8  bits unsigned 4 components ( RGB + Alpha )
    FORMAT_RGBX16U, // 16 bits unsigned 3 components + 1 padding ( RGB + X )
    FORMAT_RGBA16U, // 16 bits unsigned 4 components ( RGB + Alpha )
    FORMAT_RGBA16F, // 16 bits half float 4 components ( RGB + Alpha )
    FORMAT_RGBA32U, // 32 bits unsigned 4 components ( RGB + Alpha )
    FORMAT_RGBA32F, // 32 bits true float 4 components ( RGB + Alpha )
    FORMAT_BC1,     // 8 bytes 4×4 block compression 3 components ( RGB )
    FORMAT_BC3,     // BC3 stores RGBA data, using BC1 for the RGB part and BC4 for the alpha part,
    FORMAT_BC5,     // 
    FORMAT_BC7,     //
    FORMAT_BC6H,    //
    FORMAT_ETC2,    //
    FORMAT_EAC      //
};

enum flags_t : uint32_t
{
    IMAGE1D = 0x0001,   /// Image 1D
    IMAGE2D = 0x0002,   /// Image 2D
    IMAGE3D = 0x0004,   /// Image 3D
    CUBEMAP = 0x0008,   /// Cubemap texture
    ARRAY   = 0x0010,   /// Array Image
    SRGB    = 0x0020,   /// sRGB color range
};

typedef struct BTF_Header_s
{
    uint32_t magic = 0;         // 'BTF\0'
    uint16_t version = 0;       // versionamento é obrigatório
    uint16_t headerSize = 0;    // 
    uint32_t imageFlags = 0;    //

    // the format suported
    uint16_t pixelFormat = 0;   //
    uint16_t pad0;              //

    uint16_t layerCount = 0;    //
    uint16_t mipCount = 0;      //
    
    uint32_t subImageTableOffset = 0;
    uint32_t pixelDataOffset = 0;
    uint32_t pixelBufferSize = 0;

    uint32_t dataAlignment = 0; // ex: 16 ou 256
} BTF_Header_t; 

typedef struct alignas( 16 ) BTF_SubImage_s 
{
    uint32_t    width = 0;
    uint32_t    height = 0;
    uint32_t    depth = 0; 
    uint32_t    padding0;   
    uint32_t    offset = 0;
    uint32_t    size = 0;
} BTF_SubImage_t;

struct BTF_Footer_t
{
    uint64_t contentHash;   // CRC64 ou xxHash
    uint32_t nameOffset;    // string opcional no fim do arquivo
};

inline constexpr uint32_t   BTF_MAGIC = { 'BTF\0' }; /// 0x00465442u; // "BTF\0"
inline constexpr uint32_t   BTF_VERSION = 10; // 1.0 
inline constexpr size_t     BTF_HEADER_SIZE = sizeof( BTF_Header_t );
inline constexpr size_t     BTF_SUBIMAGE_SIZE = sizeof( BTF_SubImage_t );

#endif //!__BTF_STRUCTURES_HPP__