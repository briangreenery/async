#ifndef Async_UVStream_h
#define Async_UVStream_h

#include "Async/UVHandle.h"
#include "Async/UVReadStream.h"
#include "Async/UVWriteStream.h"

class UVStream : public UVHandle
{
public:
  explicit UVStream( uv_stream_t* );

  UVReadStream& ReadStream();
  UVWriteStream& WriteStream();

private:
  UVReadStream m_readStream;
  UVWriteStream m_writeStream;
};

#endif
