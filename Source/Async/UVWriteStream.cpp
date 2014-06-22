#include "UVWriteStream.h"
#include <assert.h>

UVWriteStream::UVWriteStream( uv_stream_t* stream )
  : m_stream( stream )
  , m_pipeReadable( this, OnPipeReadable )
{
  m_req.data = this;
}

void UVWriteStream::Pipe( const DataPipePtr& pipe )
{
  m_pipe = pipe;
  Write();
}

void UVWriteStream::OnFinished( EventListener& listener )
{
  m_finished.AddListener( listener );
}

void UVWriteStream::OnError( EventListener& listener )
{
  m_error.AddListener( listener );
}

void UVWriteStream::Write()
{
  if ( m_pipe->IsEnd() )
  {
    m_finished.Emit();
  }
  else if ( m_pipe->IsEmpty() )
  {
    m_pipe->OnReadable( m_pipeReadable );
  }
  else
  {
    m_data = m_pipe->Read();

    uv_buf_t buf;
    buf.base =
      const_cast<char*>( reinterpret_cast<const char*>( m_data.Start() ) );
    buf.len = m_data.Length();

    if ( uv_write( &m_req, m_stream, &buf, 1, OnWriteComplete ) )
      m_error.Emit();
  }
}

void UVWriteStream::OnWriteComplete( int status )
{
  m_data = Data();

  if ( status )
    m_error.Emit();
  else
    Write();
}

void UVWriteStream::OnPipeReadable()
{
  m_pipeReadable.Disconnect();
  Write();
}

void UVWriteStream::OnWriteComplete( uv_write_t* req, int status )
{
  try
  {
    static_cast<UVWriteStream*>( req->data )->OnWriteComplete( status );
  }
  catch ( ... )
  {
    assert( false );
    abort();
  }
}

void UVWriteStream::OnPipeReadable( void* data )
{
  static_cast<UVWriteStream*>( data )->OnPipeReadable();
}
