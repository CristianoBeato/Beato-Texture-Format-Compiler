
#include "precompiled.hpp"
#include "BTFLoader.hpp"

BTFTexture::BTFTexture( void ) : m_pixelBuffer( nullptr )
{
}

BTFTexture::~BTFTexture( void )
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
