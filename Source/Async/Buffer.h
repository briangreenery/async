#ifndef Async_Buffer_h
#define Async_Buffer_h

#include "Async/BufferBase.h"
#include "Async/Data.h"
#include "Async/IntrusivePtr.h"
#include <stdint.h>

class Buffer;
class Data;
typedef IntrusivePtr<Buffer> BufferPtr;

// A Buffer is a writeable chunk of memory. It can only be accessed through the
// reference counted BufferPtr. This is so that we can pipe data along without
// copying it, and have it be freed automatically when it's no longer
// referenced.

class Buffer : public BufferBase
{
public:
  static BufferPtr New( size_t length );

  Data Slice( const uint8_t* start, size_t length );

  size_t Refs() const;

  uint8_t* Start() const;
  uint8_t* Mark() const;
  uint8_t* End() const;

  size_t UsedLength() const;
  size_t FreeLength() const;

  size_t Write( const uint8_t* data, size_t length );
  size_t Write( const Data& );

private:
  Buffer( const Buffer& );
  Buffer& operator=( const Buffer& );

  uint8_t* m_start;
  uint8_t* m_mark;
  uint8_t* m_end;
};

inline size_t Buffer::Write( const Data& data )
{
  return Write( data.Start(), data.Length() );
}

inline size_t Buffer::Refs() const
{
  return m_refs;
}

inline uint8_t* Buffer::Start() const
{
  return m_start;
}

inline uint8_t* Buffer::Mark() const
{
  return m_mark;
}

inline uint8_t* Buffer::End() const
{
  return m_end;
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
