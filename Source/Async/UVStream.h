#ifndef Async_UVStream_h
#define Async_UVStream_h

#include "Async/UVHandle.h"
#include "Async/UVStreamReader.h"
#include "Async/UVStreamWriter.h"

class UVStream : public UVHandle
{
public:
  explicit UVStream( uv_stream_t* );

  uv_stream_t* Stream() const;

  UVStreamReader& Reader();
  UVStreamWriter& Writer();

private:
  uv_stream_t* m_stream;

  UVStreamReader m_reader;
  UVStreamWriter m_writer;
};

#endif
