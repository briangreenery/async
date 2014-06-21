#include "Data.h"
#include <assert.h>
#include <string.h>

Data::Data() : m_start( 0 ), m_length( 0 )
{
}

Data::Data( const IntrusivePtr<BufferBase>& buffer,
            const uint8_t* start,
            size_t length )
  : m_buffer( buffer ), m_start( start ), m_length( length )
{
}

Data Data::Slice( const uint8_t* start, size_t length ) const
{
  assert( start >= m_start );
  assert( start + length <= m_start + m_length );

  return Data( m_buffer, start, length );
}

bool Data::Coalesce( const Data& other )
{
  if ( m_start + m_length == other.m_start )
  {
    m_length += other.m_length;
    return true;
  }

  return false;
}

bool operator==( const Data& a, const Data& b )
{
  return ( a.Length() == b.Length() ) &&
         ( memcmp( a.Start(), b.Start(), a.Length() ) == 0 );
}
