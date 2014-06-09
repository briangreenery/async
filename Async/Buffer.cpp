#include "Buffer.h"
#include "Data.h"
#include <algorithm>
#include <string.h>

BufferPtr Buffer::New( size_t length )
{
  void* space = malloc( sizeof( Buffer ) + length );

  if ( space == 0 )
    throw std::bad_alloc();

  Buffer* buffer = static_cast<Buffer*>( space );

  buffer->m_refs = 0;
  buffer->m_start = static_cast<uint8_t*>( space ) + sizeof( Buffer );
  buffer->m_mark = buffer->m_start;
  buffer->m_end = buffer->m_start + length;

  return BufferPtr( buffer );
}

size_t Buffer::Write( const uint8_t* data, size_t length )
{
  size_t amount = std::min<size_t>( length, m_end - m_mark );
  memcpy( m_mark, data, amount );
  m_mark += amount;
  return amount;
}

size_t Buffer::Write( const Data& data )
{
  return Write( data.Start(), data.Length() );
}
