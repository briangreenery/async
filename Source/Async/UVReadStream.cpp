#include "UVReadStream.h"
#include "Async/UVStream.h"
#include <assert.h>

UVReadStream::UVReadStream( uv_stream_t* stream )
  : m_stream( stream )
  , m_pipeWritable( this, OnPipeWritable )
{
}

void UVReadStream::Pipe( const DataPipePtr& pipe )
{
  m_pipe = pipe;
  Read();
}

void UVReadStream::OnEnd( EventListener& listener )
{
  m_end.AddListener( listener );
}

void UVReadStream::OnError( EventListener& listener )
{
  m_error.AddListener( listener );
}

void UVReadStream::Read()
{
  if ( uv_read_start( m_stream, OnAlloc, OnReadComplete ) )
    m_error.Emit();
}

void UVReadStream::Pause()
{
  if ( uv_read_stop( m_stream ) )
    m_error.Emit();
}

uv_buf_t UVReadStream::OnAlloc()
{
  if ( !m_buffer || m_buffer->FreeLength() == 0 )
    m_buffer = Buffer::New( 4096 );

  uv_buf_t buf;
  buf.base = reinterpret_cast<char*>( m_buffer->Mark() );
  buf.len = m_buffer->FreeLength();
  return buf;
}

void UVReadStream::OnReadComplete( ssize_t numRead )
{
  if ( numRead > 0 )
  {
    m_pipe->Write( m_buffer->Advance( numRead ) );

    if ( m_pipe->IsFull() )
    {
      m_pipe->OnWritable( m_pipeWritable );
      Pause();
    }
  }
  else if ( numRead < 0 )
  {
    uv_err_t lastError = uv_last_error( m_stream->loop );

    if ( lastError.code == UV_EOF )
    {
      m_pipe->WriteEnd();
      m_end.Emit();
    }
    else
    {
      m_error.Emit();
    }
  }
}

void UVReadStream::OnPipeWritable()
{
  m_pipeWritable.Disconnect();
  Read();
}

uv_buf_t UVReadStream::OnAlloc( uv_handle_t* handle, size_t )
{
  try
  {
    UVStream& stream = *static_cast<UVStream*>( handle->data );
    return stream.ReadStream().OnAlloc();
  }
  catch ( ... )
  {
    assert( false );
    abort();
  }
}

void UVReadStream::OnReadComplete( uv_stream_t* handle,
                                   ssize_t numRead,
                                   uv_buf_t )
{
  try
  {
    UVStream& stream = *static_cast<UVStream*>( handle->data );
    stream.ReadStream().OnReadComplete( numRead );
  }
  catch ( ... )
  {
    assert( false );
    abort();
  }
}

void UVReadStream::OnPipeWritable( void* data )
{
  static_cast<UVReadStream*>( data )->OnPipeWritable();
}
