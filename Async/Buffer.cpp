#include "Buffer.h"
#include <algorithm>
#include <string.h>

IntrusivePtr<Buffer> Buffer::New( size_t length )
{
  void* space = malloc( sizeof( Buffer ) + length );
  if ( space == 0 )
    throw std::bad_alloc();

  Buffer* buffer = new ( space )
    Buffer( static_cast<uint8_t*>( space ) + sizeof( Buffer ), length );

  return IntrusivePtr<Buffer>( buffer );
}

Buffer::Buffer( uint8_t* start, size_t length )
  : m_refs( 0 ), m_start( start ), m_mark( start ), m_end( start + length )
{
}

size_t Buffer::Write( const uint8_t* data, size_t length )
{
  size_t amount = std::min<size_t>( length, m_end - m_mark );
  memcpy( m_start, data, amount );
  return amount;
}
