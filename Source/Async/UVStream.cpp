#include "UVStream.h"

UVStream::UVStream( uv_stream_t* stream )
  : m_readStream( stream )
  , m_writeStream( stream )
{
}

UVReadStream& UVStream::ReadStream()
{
  return m_readStream;
}

UVWriteStream& UVStream::WriteStream()
{
  return m_writeStream;
}
