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

#ifndef __BTF_STRUCTURES_HPP__
#define __BTF_STRUCTURES_HPP__

inline constexpr uint32_t BTF_MAGIC = 0x00465442u; // "BTF\0"
inline constexpr uint32_t BTF_VERSION = 10; // 1.0 

enum flags_t
{
};

struct BTF_Header_t
{
    uint32_t magic = 0;         // 'BTF\0'
    uint16_t version = 0;       // versionamento é obrigatório
    uint16_t headerSize = 0;    // 
    uint32_t imageFlags = 0;    //

    uint16_t pixelFormat = 0;   //

    uint16_t layerCount = 0;    //
    uint16_t mipCount = 0;      //

    uint32_t baseWidth = 0;     //
    uint32_t baseHeight = 0;    //
    uint32_t baseDepth = 0;     //
    
    uint32_t subImageTableOffset = 0;
    uint32_t pixelDataOffset = 0;

    uint32_t dataAlignment = 0; // ex: 16 ou 256
};

struct BTF_SubImage_t 
{
    uint32_t    offset = 0;
    uint32_t    size = 0;
};

struct BTF_Footer_t
{
    uint64_t contentHash;   // CRC64 ou xxHash
    uint32_t nameOffset;    // string opcional no fim do arquivo
};

#endif //!__BTF_STRUCTURES_HPP__