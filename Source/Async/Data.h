#ifndef Async_Data_h
#define Async_Data_h

#include "Async/BufferBase.h"
#include "Async/IntrusivePtr.h"
#include <assert.h>

// Data is a reference to a chunk of memory, possibly living in a Buffer.
// Since the data can be living in a reference counted Buffer, to safely copy
// Data you must use the Slice() function or operator=, which will also copy
// over a reference to the Buffer.

class Data
{
public:
  Data( const IntrusivePtr<BufferBase>& buffer,
        const uint8_t* start,
        size_t length );

  template <size_t count>
  explicit Data( const char ( &literal )[count] );

  const uint8_t* Start() const;
  size_t Length() const;

  Data Slice( const uint8_t* start, size_t length ) const;

  bool Coalesce( const Data& other );

private:
  IntrusivePtr<BufferBase> m_buffer;
  const uint8_t* m_start;
  size_t m_length;
};

inline Data::Data( const IntrusivePtr<BufferBase>& buffer,
                   const uint8_t* start,
                   size_t length )
  : m_buffer( buffer ), m_start( start ), m_length( length )
{
}

template <size_t count>
inline Data::Data( const char ( &literal )[count] )
  : m_start( reinterpret_cast<const uint8_t*>( literal ) )
  , m_length( count - 1 )
{
}

inline const uint8_t* Data::Start() const
{
  return m_start;
}

inline size_t Data::Length() const
{
  return m_length;
}

inline Data Data::Slice( const uint8_t* start, size_t length ) const
{
  assert( start >= m_start );
  assert( start + length <= m_start + m_length );

  return Data( m_buffer, start, length );
}

inline bool Data::Coalesce( const Data& other )
{
  if ( m_start + m_length == other.m_start )
  {
    m_length += other.m_length;
    return true;
  }

  return false;
}

bool operator==( const Data&, const Data& );

#endif
