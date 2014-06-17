#include <exception>
#include <iostream>
#include <uv.h>

namespace UV
{

class StreamPtr
{
public:
  StreamPtr() : m_refs( 0 ) {}

private:
  friend void IntrusivePtrAddRef( StreamPtr* );
  friend void IntrusivePtrRelease( StreamPtr* );

  size_t m_refs;
};

template < class T >
struct StreamPtr
{
  size_t refs;
  T stream;
};

}

uv_buf_t Socket::OnAlloc( uv_handle_t* handle, size_t suggested_size )
{
  try
  {
    return socket->OnAlloc();
  }
  catch ( ... )
  {
    return uv_buf_t();
  }
}

void Socket::OnRead( uv_stream_t* stream, ssize_t nread, uv_buf_t buf )
{
  try
  {
    socket->OnRead( nread );
  }
  catch ( ... )
  {
  }
}

void Socket::OnRead( ssize_t nread )
{
  if ( nread > 0 )
  {
    m_outPipe->Write( m_buffer->Advance( nread ) );

    if ( m_outPipe->IsFull() )
      StopReading();
  }
  else if ( nread < 0 )
  {
    if ( uv_last_error( m_loop ) == UV_EOF )
      m_readDone.Emit();
    else
      m_readError.Emit();
  }
}

void Socket::OnOutPipeWriteable()
{
  StartReading();
}

void Server::OnNewConnection( uv_stream_t* server, int status )
{
  if ( status != 0 )
  {
    server->OnNewConnectionError()
  }
}

int main()
{
  EventLoopPtr loop = EventLoop::New();

  TCPPtr serverSocket = TCP::New( loop, this );
  serverSocket->Listen( 3000, OnConnection );

  Request req;

  socket->Pipe( );

  return 0;
}

// error, EventLoopPtr, TCPPtr
