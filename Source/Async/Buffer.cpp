#include "Buffer.h"
#include <algorithm>
#include <assert.h>
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
  size_t amount = std::min( length, FreeLength() );
  memcpy( m_mark, data, amount );
  m_mark += amount;
  return amount;
}

Data Buffer::Slice( const uint8_t* start, size_t length )
{
  assert( start >= m_start );
  assert( start + length <= m_mark );

  return Data( IntrusivePtr<BufferBase>( this ), start, length );
}

Data Buffer::Advance( size_t amount )
{
  amount = std::min( amount, FreeLength() );
  const uint8_t* oldMark = m_mark;
  m_mark += amount;
  return Data( IntrusivePtr<BufferBase>( this ), oldMark, amount );
}
