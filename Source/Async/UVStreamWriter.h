#ifndef Async_UVStreamWriter_h
#define Async_UVStreamWriter_h

#include "Async/DataPipe.h"
#include "Async/EventEmitter.h"
#include <uv.h>

class UVStreamWriter
{
public:
  explicit UVStreamWriter( uv_stream_t* );

  void Pipe( const DataPipePtr& );
  void OnFinished( EventListener& );
  void OnError( EventListener& );

private:
  UVStreamWriter( const UVStreamWriter& );
  UVStreamWriter& operator=( const UVStreamWriter& );

  void Write();

  void OnWriteComplete( int status );
  void OnPipeReadable();

  static void OnWriteComplete( uv_write_t*, int );
  static void OnPipeReadable( void* );

  uv_stream_t* m_stream;
  uv_write_t m_req;

  Data m_data;
  DataPipePtr m_pipe;

  EventEmitter m_finished;
  EventEmitter m_error;
  EventListener m_pipeReadable;
};

#endif
