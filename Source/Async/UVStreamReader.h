#ifndef Async_UVStreamReader_h
#define Async_UVStreamReader_h

#include "Async/Buffer.h"
#include "Async/DataPipe.h"
#include <uv.h>

class UVStreamReader
{
public:
  explicit UVStreamReader( uv_stream_t* );

  void Pipe( const DataPipePtr& );
  void OnEnd( EventListener& );
  void OnError( EventListener& );

private:
  UVStreamReader( const UVStreamReader& );
  UVStreamReader& operator=( const UVStreamReader& );

  void Read();
  void Pause();

  uv_buf_t OnAlloc();
  void OnReadComplete( ssize_t numRead );
  void OnPipeWritable();

  static uv_buf_t OnAlloc( uv_handle_t*, size_t );
  static void OnReadComplete( uv_stream_t*, ssize_t, uv_buf_t );
  static void OnPipeWritable( void* );

  uv_stream_t* m_stream;

  BufferPtr m_buffer;
  DataPipePtr m_pipe;

  EventEmitter m_end;
  EventEmitter m_error;
  EventListener m_pipeWritable;
};

#endif
