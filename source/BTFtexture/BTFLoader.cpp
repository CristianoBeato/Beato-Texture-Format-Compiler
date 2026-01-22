
#include "precompiled.hpp"
#include "BTFLoader.hpp"

static inline bool GLI_IsRGB( const gli::format in_format )
{
    switch ( in_format )
    {
    // non compressed
    case gli::FORMAT_R8_SRGB_PACK8:
    case gli::FORMAT_RG8_SRGB_PACK8:
    case gli::FORMAT_RGB8_SRGB_PACK8:
    case gli::FORMAT_RGBA8_SRGB_PACK8:
    case gli::FORMAT_RGBA8_SRGB_PACK32:
    // compressed
    case gli::FORMAT_RGB_DXT1_SRGB_BLOCK8:
    case gli::FORMAT_RGBA_DXT1_SRGB_BLOCK8:
    case gli::FORMAT_RGBA_DXT5_SRGB_BLOCK16:
    case gli::FORMAT_RGBA_ETC2_SRGB_BLOCK16:
    case gli::FORMAT_RGBA_BP_SRGB_BLOCK16:
        return true;
    };

    return false;
}

static inline  gli::target GliTextureType( const uint32_t in_flags )
{
    /// check if are a image array
    if ( ( in_flags & ARRAY ) == ARRAY )
    {
        if ( ( in_flags & IMAGE1D ) == IMAGE1D )
            return gli::TARGET_1D_ARRAY;
        else if ( ( in_flags & IMAGE2D ) == IMAGE2D )
            return gli::TARGET_2D_ARRAY;
        else if ( ( ( in_flags & CUBEMAP ) == CUBEMAP ) )
            return gli::TARGET_CUBE_ARRAY;
    }
    else
    {
        if ( ( in_flags & IMAGE1D ) == IMAGE1D )
            return gli::TARGET_1D;
        else if ( ( in_flags & IMAGE2D ) == IMAGE2D )
            return gli::TARGET_2D;
        else if( ( in_flags & IMAGE3D ) == IMAGE3D )
            return gli::TARGET_3D;
        else if ( ( ( in_flags & CUBEMAP ) == CUBEMAP ) )
            return gli::TARGET_CUBE;
    }

    /// todo: thow a error 
    return gli::TARGET_1D;
};

uint32_t TextureTypeFlags( const gli::target in_target )
{
    uint32_t flags = 0;

    switch (in_target)
    {    
    case gli::TARGET_1D:
        flags = IMAGE1D;
        break;

    case gli::TARGET_1D_ARRAY:
        flags = IMAGE1D | ARRAY;
        break;

    case gli::TARGET_2D:
        flags = IMAGE2D;
        break;

    case gli::TARGET_2D_ARRAY:
        flags = IMAGE2D | ARRAY;
        break;

    case gli::TARGET_3D:
        flags = IMAGE3D;
        break;

    case gli::TARGET_CUBE:
        flags = CUBEMAP;
        break;

    case gli::TARGET_CUBE_ARRAY:
        flags = CUBEMAP | ARRAY;
        break;

    default:
        break;
    }

    return flags;
}

static inline format_t GliFormatToBTF( const gli::format in_gliFormat )
{
    format_t f;
    switch ( in_gliFormat )
    {

        /// 8 bit red 
        case gli::FORMAT_R8_UNORM_PACK8:
        case gli::FORMAT_R8_SRGB_PACK8:
            f = FORMAT_RED8U;
            break;
            
        /// 16 bit red
        case gli::FORMAT_R16_UNORM_PACK16:
            f = FORMAT_RED16U;
            break;

        /// 8 bit red green 
        case gli::FORMAT_RG8_SNORM_PACK8:
        case gli::FORMAT_RG8_SRGB_PACK8:
            f = FORMAT_RG8U;
            break;

        case gli::FORMAT_RG16_SNORM_PACK16:
            f = FORMAT_RG16U;
            break;
            
        // TODO > convert to RGB + x paded
        case gli::FORMAT_RGB8_UNORM_PACK8:  
        case gli::FORMAT_RGB8_SRGB_PACK8:
            f = FORMAT_RGBX8U;
            break;

        case gli::FORMAT_RGBA8_UNORM_PACK8:
        case gli::FORMAT_RGBA8_SRGB_PACK8:
            f = FORMAT_RGBA8U;
            break;

        case gli::FORMAT_RGB16_SNORM_PACK16:
            f = FORMAT_RGBX16U;
            break;
        case gli::FORMAT_RGBA16_SNORM_PACK16:
            f = FORMAT_RGBA16U;
            break;
        case gli::FORMAT_RGBA16_SFLOAT_PACK16:
            f = FORMAT_RGBA16F;
            break;
        case gli::FORMAT_RGBA32_UINT_PACK32:
            f = FORMAT_RGBA32U;
            break;
        case gli::FORMAT_RGBA32_SFLOAT_PACK32:
            f = FORMAT_RGBA32F;
            break;
        case gli::FORMAT_RGB_DXT1_UNORM_BLOCK8:
        case gli::FORMAT_RGB_DXT1_SRGB_BLOCK8:
            f = FORMAT_BC1;
            break;
        case gli::FORMAT_RGBA_DXT5_UNORM_BLOCK16:
        case gli::FORMAT_RGBA_DXT5_SRGB_BLOCK16:
            f = FORMAT_BC3;
            break;
        case gli::FORMAT_RG_ATI2N_UNORM_BLOCK16:
            f = FORMAT_BC5;
            break;
        case gli::FORMAT_RGBA_BP_UNORM_BLOCK16:
        case gli::FORMAT_RGBA_BP_SRGB_BLOCK16:
            f = FORMAT_BC7;
            break;
        case gli::FORMAT_RGB_BP_SFLOAT_BLOCK16:
            f = FORMAT_BC6H;
            break;
        case gli::FORMAT_RGBA_ETC2_UNORM_BLOCK8:
        case gli::FORMAT_RGBA_ETC2_SRGB_BLOCK8:
            f = FORMAT_ETC2;
            break;
        case gli::FORMAT_RG_EAC_UNORM_BLOCK16:
            f = FORMAT_EAC;
            break;    
    default:
        break;
    }
} 

static inline gli::format BTFFormatToGLI( const format_t in_brfFormat, const bool in_SRGB )
{
    gli::format f;
    switch ( in_brfFormat )
    {
        case FORMAT_RED8U:
            f = ( in_SRGB ) ? gli::FORMAT_R8_SRGB_PACK8 : gli::FORMAT_R8_UNORM_PACK8;
            break;

        case FORMAT_RED16U:
            f = gli::FORMAT_R16_UNORM_PACK16;
            break;

        case FORMAT_RG8U:
            f = ( in_SRGB ) ? gli::FORMAT_RG8_SRGB_PACK8 : gli::FORMAT_RG8_UNORM_PACK8;
            break;

        case FORMAT_RG16U:
            f = gli::FORMAT_RG16_UNORM_PACK16;
            break;

        case FORMAT_RGBX8U:
            f = ( in_SRGB ) ? gli::FORMAT_RGB8_SRGB_PACK8 : gli::FORMAT_RGB8_UNORM_PACK8;
            break;

        case FORMAT_RGBA8U:
            f = ( in_SRGB ) ? gli::FORMAT_RGBA8_SRGB_PACK8 : gli::FORMAT_RGBA8_UNORM_PACK8;
            break;

        case FORMAT_RGBX16U:
            f = gli::FORMAT_RGB16_UNORM_PACK16;
            break;

        case FORMAT_RGBA16U:
            f = gli::FORMAT_RGBA16_UNORM_PACK16;
            break;

        case FORMAT_RGBA16F:
            f = gli::FORMAT_RGBA16_SFLOAT_PACK16;
            break;

        case FORMAT_RGBA32U:
            f = gli::FORMAT_RGBA32_UINT_PACK32;
            break;

        case FORMAT_RGBA32F:
            f = gli::FORMAT_RGBA32_SFLOAT_PACK32;
            break;

        case FORMAT_BC1:
            f = ( in_SRGB ) ? gli::FORMAT_RGB_DXT1_SRGB_BLOCK8 : gli::FORMAT_RGB_DXT1_UNORM_BLOCK8; 
            break;

        case FORMAT_BC3:
            f = ( in_SRGB ) ? gli::FORMAT_RGBA_DXT5_SRGB_BLOCK16 : gli::FORMAT_RGBA_DXT5_UNORM_BLOCK16;
            break;

        case FORMAT_BC5:
            f = gli::FORMAT_RG_ATI2N_UNORM_BLOCK16;
            break;

        case FORMAT_BC7:
             f = ( in_SRGB ) ? gli::FORMAT_RGBA_BP_SRGB_BLOCK16 : gli::FORMAT_RGBA_BP_UNORM_BLOCK16;
            break;

        case FORMAT_BC6H:
            f = gli::FORMAT_RGB_BP_SFLOAT_BLOCK16;
            break;

        case FORMAT_ETC2:
            f = ( in_SRGB ) ? gli::FORMAT_RGBA_ETC2_SRGB_BLOCK8 : gli::FORMAT_RGBA_ETC2_UNORM_BLOCK8;
            break;

        case FORMAT_EAC:
            f = gli::FORMAT_RG_EAC_UNORM_BLOCK16;
            break;
    
    default:
        break;
    }
}

BTFTexture::BTFTexture( void ) : m_pixelBuffer( nullptr )
{
}

BTFTexture::~BTFTexture( void )
{
}

gli::texture BTFTexture::GetTexture(void) const
{
    return gli::texture();
}

void BTFTexture::SetTexture( const gli::texture &in_source )
{
}

BTFWriter::BTFWriter( void ) : BTFTexture()
{
}

BTFWriter::~BTFWriter( void )
{
}

bool BTFWriter::Save( const std::string &in_path )
{
    if ( !m_writeStream.FromFile( in_path.c_str(), "wb" ) )
        return false;

    // define header constants 
    m_header.magic = BTF_MAGIC;
    m_header.version = BTF_VERSION;
    m_header.headerSize = BTF_HEADER_SIZE;

    /// write file header
    m_writeStream.Write( &m_header, sizeof( m_header ) );

    /// write the imag
    m_writeStream.Write( m_subimages.data(), m_subimages.size() * BTF_SUBIMAGE_SIZE );

    /// write pixel buffer
    m_writeStream.Write( m_pixelBuffer, m_header.pixelBufferSize );

    /// write footer 
    m_writeStream.Write( &m_footer, sizeof( m_footer ) );
    
    return true;
}

BTFLoader::BTFLoader( void ) : BTFTexture()
{
}

BTFLoader::~BTFLoader( void )
{
}

bool BTFLoader::Load(const std::string &in_path)
{
    /// open the file
    if ( !m_readStream.FromFile( in_path.c_str(), "rb" ) )
        return false;

    // get the file header 
    if( m_readStream.Read( &m_header, sizeof( BTF_Header_t ) != sizeof( BTF_Header_t ) ) )
        return false;

    // not a valid BTF
    if ( m_header.magic != BTF_MAGIC )
        return false;

    if ( m_header.headerSize != BTF_HEADER_SIZE )
        return false;

    // unsuported version
    if ( m_header.version < BTF_VERSION || m_header.version > BTF_VERSION )
        return false;
    
    // get the number of subimages  
    m_subimages.resize( m_header.layerCount * m_header.mipCount );
    
    /// read subimage array
    m_readStream.Read( m_subimages.data(), m_subimages.size() * BTF_SUBIMAGE_SIZE );

    /// read pixel data
    m_pixelBuffer = std::malloc( m_header.pixelBufferSize );
    m_readStream.Read( m_pixelBuffer, m_header.pixelBufferSize );

    /// reader footer
    m_readStream.Read( &m_footer, sizeof( BTF_Footer_t ) );

    // TODO: CRC64
    
    return true;
}
