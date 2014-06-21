#ifndef Async_UVStream_h
#define Async_UVStream_h

#include "Async/Buffer.h"
#include "Async/DataPipe.h"
#include <uv.h>

class UVStream
{
public:
  UVStream( uv_stream_t* );

  void ReadTo( const DataPipePtr& );
  void OnReadDone( EventListener& );
  void OnReadError( EventListener& );
  uv_err_t LastReadError() const;

  void WriteFrom( const DataPipePtr& );
  void OnWriteDone( EventListener& );
  void OnWriteError( EventListener& );
  uv_err_t LastWriteError() const;

private:
  UVStream( const UVStream& );
  UVStream& operator=( const UVStream& );

  void StartReading();
  void StopReading();

  void Write();

  void OnReadToWriteable();
  void OnWriteFromReadable();

  uv_buf_t OnAlloc();
  void OnReadDone( ssize_t );
  void OnWriteDone( int status );

  static void OnReadToWriteable( void* );
  static void OnWriteFromReadable( void* );
  static uv_buf_t OnAlloc( uv_handle_t*, size_t );
  static void OnReadDone( uv_stream_t*, ssize_t, uv_buf_t );
  static void OnWriteDone( uv_write_t*, int );

  uv_stream_t* m_stream;

  BufferPtr m_readBuffer;
  DataPipePtr m_readTo;
  EventEmitter m_readDone;
  EventEmitter m_readError;
  EventListener m_readToWriteable;
  uv_err_t m_lastReadError;

  uv_write_t m_writeReq;
  Data m_writeData;
  DataPipePtr m_writeFrom;
  EventEmitter m_writeDone;
  EventEmitter m_writeError;
  EventListener m_writeFromReadable;
  uv_err_t m_lastWriteError;
};

#endif
