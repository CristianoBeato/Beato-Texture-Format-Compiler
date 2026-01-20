
#include "precompiled.hpp"
#include "BTFLoader.hpp"

BTFTexture::BTFTexture( void )
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

bool BTFWriter::Save(const std::string in_path)
{
    if ( !m_writeStram.FromFile( in_path.c_str(), "w" ) )
        return false;

    /// write file header
    m_writeStram.Write( &m_header, sizeof( m_header ) );

    for ( uint32_t i = 0; i < static_cast<uint32_t>( m_imacheChain.size() ); i++)
    {
        m_writeStram.Write( &m_imacheChain[i], sizeof(BTF_SubImage_t) );
    }

    /// write pixel buffer
    m_writeStram.Write( m_pixelBuffer, m_bufferSize );

    /// 
    m_writeStram.Write( &m_footer, sizeof( m_footer ) );
    
    return true;
}

BTFLoader::BTFLoader( void ) : BTFTexture()
{
}

BTFLoader::~BTFLoader( void )
{
}
