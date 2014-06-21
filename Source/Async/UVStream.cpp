#include "UVStream.h"
#include <assert.h>

UVStream::UVStream( uv_stream_t* stream )
  : m_stream( stream )
  , m_readToWriteable( this, OnReadToWriteable )
  , m_writeFromReadable( this, OnWriteFromReadable )
{
  m_writeReq.data = this;
}

void UVStream::ReadTo( const DataPipePtr& pipe )
{
  m_readTo = pipe;
  StartReading();
}

void UVStream::OnReadDone( EventListener& listener )
{
  m_readDone.AddListener( listener );
}

void UVStream::OnReadError( EventListener& listener )
{
  m_readError.AddListener( listener );
}

uv_err_t UVStream::LastReadError() const
{
  return m_lastReadError;
}

void UVStream::WriteFrom( const DataPipePtr& pipe )
{
  m_writeFrom = pipe;
  Write();
}

void UVStream::OnWriteDone( EventListener& listener )
{
  m_writeDone.AddListener( listener );
}

void UVStream::OnWriteError( EventListener& listener )
{
  m_writeError.AddListener( listener );
}

uv_err_t UVStream::LastWriteError() const
{
  return m_lastWriteError;
}

void UVStream::OnReadToWriteable()
{
  m_readToWriteable.Disconnect();
  StartReading();
}

void UVStream::OnWriteFromReadable()
{
  m_writeFromReadable.Disconnect();
  Write();
}

void UVStream::StartReading()
{
  if ( uv_read_start( m_stream, OnAlloc, OnReadDone ) )
  {
    m_lastReadError = uv_last_error( m_stream->loop );
    m_readError.Emit();
  }
}

void UVStream::StopReading()
{
  if ( uv_read_stop( m_stream ) )
  {
    m_lastReadError = uv_last_error( m_stream->loop );
    m_readError.Emit();
  }
}

uv_buf_t UVStream::OnAlloc()
{
  if ( !m_readBuffer )
    m_readBuffer = Buffer::New( 4096 );

  uv_buf_t buf;
  buf.base = reinterpret_cast<char*>( m_readBuffer->Mark() );
  buf.len = m_readBuffer->FreeLength();
  return buf;
}

void UVStream::OnReadDone( ssize_t numRead )
{
  if ( numRead > 0 )
  {
    m_readTo->Write( m_readBuffer->Advance( numRead ) );

    if ( m_readTo->IsFull() )
    {
      m_readTo->OnReadable( m_readToWriteable );
      StopReading();
    }
  }
  else if ( numRead < 0 )
  {
    m_lastReadError = uv_last_error( m_stream->loop );

    if ( m_lastReadError.code == UV_EOF )
    {
      m_readTo->Write( Data() );
      m_readDone.Emit();
    }
    else
      m_readError.Emit();
  }
}

void UVStream::Write()
{
  if ( m_writeFrom->IsEmpty() )
  {
    m_writeFrom->OnReadable( m_writeFromReadable );
  }
  else
  {
    m_writeData = m_writeFrom->Read();

    if ( m_writeData.IsEmpty() )
    {
      m_writeDone.Emit();
    }
    else
    {
      uv_buf_t buf;
      buf.base = const_cast<char*>(
        reinterpret_cast<const char*>( m_writeData.Start() ) );
      buf.len = m_writeData.Length();

      if ( uv_write( &m_writeReq, m_stream, &buf, 1, OnWriteDone ) )
      {
        m_lastWriteError = uv_last_error( m_stream->loop );
        m_writeError.Emit();
      }
    }
  }
}

void UVStream::OnWriteDone( int status )
{
  m_writeData = Data();

  if ( status )
  {
    m_lastWriteError = uv_last_error( m_stream->loop );
    m_writeError.Emit();
  }
  else
  {
    Write();
  }
}

void UVStream::OnReadToWriteable( void* data )
{
  static_cast<UVStream*>( data )->OnReadToWriteable();
}

void UVStream::OnWriteFromReadable( void* data )
{
  static_cast<UVStream*>( data )->OnWriteFromReadable();
}

uv_buf_t UVStream::OnAlloc( uv_handle_t* handle, size_t )
{
  try
  {
    return static_cast<UVStream*>( handle->data )->OnAlloc();
  }
  catch ( ... )
  {
    assert( false );
  }
}

void UVStream::OnReadDone( uv_stream_t* stream, ssize_t numRead, uv_buf_t )
{
  try
  {
    static_cast<UVStream*>( stream->data )->OnReadDone( numRead );
  }
  catch ( ... )
  {
    assert( false );
  }
}

void UVStream::OnWriteDone( uv_write_t* req, int status )
{
  try
  {
    static_cast<UVStream*>( req->data )->OnWriteDone( status );
  }
  catch ( ... )
  {
    assert( false );
  }
}
