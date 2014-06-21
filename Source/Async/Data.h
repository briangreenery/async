#ifndef Async_Data_h
#define Async_Data_h

#include "Async/BufferBase.h"
#include "Async/IntrusivePtr.h"

// Data is a reference to a chunk of memory, possibly living in a Buffer.
// Since the data can be living in a reference counted Buffer, to safely copy
// Data you must use the Slice() function or operator=, which will also copy
// over a reference to the Buffer.

class Data
{
public:
  Data();

  Data( const IntrusivePtr<BufferBase>& buffer,
        const uint8_t* start,
        size_t length );

  template <size_t count>
  explicit Data( const char ( &literal )[count] );

  const uint8_t* Start() const;
  size_t Length() const;
  bool IsEmpty() const;

  Data Slice( const uint8_t* start, size_t length ) const;

  bool Coalesce( const Data& other );

private:
  IntrusivePtr<BufferBase> m_buffer;
  const uint8_t* m_start;
  size_t m_length;
};

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

inline bool Data::IsEmpty() const
{
  return m_length == 0;
}

bool operator==( const Data&, const Data& );

#endif
