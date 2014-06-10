#ifndef Async_Buffer_h
#define Async_Buffer_h

#include "Async/IntrusivePtr.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

class Buffer;
class Data;
typedef IntrusivePtr<Buffer> BufferPtr;

class Buffer
{
public:
  static BufferPtr New( size_t length );

  size_t Write( const uint8_t* data, size_t length );
  size_t Write( const Data& );

  Data Used() const;

  size_t UsedLength() const;
  size_t FreeLength() const;

private:
  Buffer( const Buffer& );
  Buffer& operator=( const Buffer& );

  friend void IntrusivePtrAddRef( Buffer* );
  friend void IntrusivePtrRelease( Buffer* );

  size_t m_refs;
  uint8_t* m_start;
  uint8_t* m_mark;
  uint8_t* m_end;
};

inline void IntrusivePtrAddRef( Buffer* buffer )
{
  buffer->m_refs++;
}

inline void IntrusivePtrRelease( Buffer* buffer )
{
  if ( --buffer->m_refs == 0 )
    free( buffer );
}

inline size_t Buffer::UsedLength() const
{
  return m_mark - m_start;
}

inline size_t Buffer::FreeLength() const
{
  return m_end - m_mark;
}

#endif
