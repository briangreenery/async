#include "UVStream.h"

UVStream::UVStream( uv_stream_t* stream )
  : UVHandle( reinterpret_cast<uv_handle_t*>( stream ) )
  , m_reader( stream )
  , m_writer( stream )
{
}

UVStreamReader& UVStream::Reader()
{
  return m_reader;
}

UVStreamWriter& UVStream::Writer()
{
  return m_writer;
}
