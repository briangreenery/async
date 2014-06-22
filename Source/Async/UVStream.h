#ifndef Async_UVStream_h
#define Async_UVStream_h

#include "Async/UVHandle.h"
#include "Async/UVStreamReader.h"
#include "Async/UVStreamWriter.h"

class UVStream : public UVHandle
{
public:
  explicit UVStream( uv_stream_t* );

  UVStreamReader& Reader();
  UVStreamWriter& Writer();

private:
  UVStreamReader m_reader;
  UVStreamWriter m_writer;
};

#endif
