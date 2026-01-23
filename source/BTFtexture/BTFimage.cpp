
#include "precompiled.hpp"
#include "BTFStructures.hpp"

bool MapGliFormatToBTF(gli::format fmt, uint16_t& out)
{
    switch (fmt)
    {
        case gli::FORMAT_RGBA8_UNORM_PACK8: out = FORMAT_RGBA8U; return true;
        case gli::FORMAT_RGBA16_SFLOAT_PACK16: out = FORMAT_RGBA16F; return true;
        case gli::FORMAT_RGBA32_SFLOAT_PACK32: out = FORMAT_RGBA32F; return true;

        case gli::FORMAT_RGB_DXT1_SRGB_BLOCK8: out = FORMAT_BC1; return true;
        case gli::FORMAT_BC3_UNORM_BLOCK:     out = FORMAT_BC3; return true;
        case gli::FORMAT_BC5_UNORM_BLOCK:     out = FORMAT_BC5; return true;
        case gli::FORMAT_BC7_UNORM_BLOCK:     out = FORMAT_BC7; return true;
        case gli::FORMAT_BC6H_SFLOAT_BLOCK:   out = FORMAT_BC6H; return true;

        case gli::FORMAT_ETC2_R8G8B8_UNORM_BLOCK: out = FORMAT_ETC2; return true;
        case gli::FORMAT_EAC_R11_UNORM_BLOCK:     out = FORMAT_EAC; return true;
        default: return false;
    }
}

bool ExportBTF( const std::shared_ptr<gli::texture> & tex, const char* path)
{
    BTF_Header_t header{};
    header.magic      = BTF_MAGIC;
    header.version    = BTF_VERSION;
    header.headerSize = sizeof(BTF_Header_t);
    header.layerCount = static_cast<uint16_t>( tex->layers() + tex->faces() );
    header.mipCount   = static_cast<uint16_t>( tex->levels() );
    header.dataAlignment = 256;

    // Flags
    if ( tex->target() == gli::TARGET_1D) 
        header.imageFlags |= IMAGE1D;
    else if ( tex->target() == gli::TARGET_2D) 
        header.imageFlags |= IMAGE2D;
    else if ( tex->target() == gli::TARGET_3D) 
        header.imageFlags |= IMAGE3D;
    else if ( tex->target() == gli::TARGET_CUBE) 
        header.imageFlags |= CUBEMAP;
    else if ( tex->layers() > 1) 
        header.imageFlags |= ARRAY;

    if (!MapGliFormatToBTF( tex->format(), header.pixelFormat ) )
        return false;

    const uint32_t faceCount = tex->faces();
    const uint32_t subImageCount = header.layerCount * faceCount * header.mipCount;

    std::vector<BTF_SubImage_t> subImages(subImageCount);

    // Layout offsets
    header.subImageTableOffset = header.headerSize;
    header.pixelDataOffset = Align(header.subImageTableOffset + subImageCount * sizeof(BTF_SubImage_t), header.dataAlignment);

    uint32_t pixelOffset = header.pixelDataOffset;
    uint32_t index = 0;

    for (uint32_t layer = 0; layer < header.layerCount; ++layer)
    {
        for (uint32_t face  = 0; face  < faceCount;        ++face)
        {
            for (uint32_t mip   = 0; mip   < header.mipCount;  ++mip)
            {
                auto extent = tex->extent(mip);
                size_t size = tex->size(mip);

                pixelOffset = Align(pixelOffset, header.dataAlignment);

                auto& si = subImages[index++];
                si.width  = extent.x;
                si.height = extent.y;
                si.depth  = extent.z;
                si.offset = pixelOffset;
                si.size   = static_cast<uint32_t>(size);
                pixelOffset += si.size;
            }
        }
    }

    header.pixelBufferSize = pixelOffset - header.pixelDataOffset;

    // Write file
    SDL::IO::Stream file;
    if ( !file.FromFile( path, "wb" ) ) 
        return false;

    file.Write( (char*)&header,  BTF_HEADER_SIZE );
    file.Write( (char*)subImages.data(), subImages.size() * BTF_SUBIMAGE_SIZE );

    // Padding até pixelDataOffset
    file.Seek( header.pixelDataOffset, SDL_IO_SEEK_SET );

    index = 0;
    for (uint32_t layer = 0; layer < header.layerCount; ++layer)
    {
        for (uint32_t face  = 0; face  < faceCount;        ++face)
        {
            for (uint32_t mip   = 0; mip   < header.mipCount;  ++mip)
            {
                const void* data = tex->data(layer, face, mip);
                file.Write( (const char*)data, tex->size(mip));
            }
        }
    }

    return true;
}
