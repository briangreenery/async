#ifndef Async_BufferBase_h
#define Async_BufferBase_h

#include <stddef.h>
#include <stdlib.h>

// Buffer base holds the reference count for Buffers. It's split into its own
// header file to resolve a circular #include dependency between Buffer and
// Data.

class BufferBase
{
protected:
  friend void IntrusivePtrAddRef( BufferBase* );
  friend void IntrusivePtrRelease( BufferBase* );

  size_t m_refs;
};

inline void IntrusivePtrAddRef( BufferBase* buffer )
{
  buffer->m_refs++;
}

inline void IntrusivePtrRelease( BufferBase* buffer )
{
  if ( --buffer->m_refs == 0 )
    free( buffer );
}

#endif
