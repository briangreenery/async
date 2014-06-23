#include "UVTCPSocket.h"
#include "Async/Error.h"
#include <assert.h>

UVTCPSocketPtr UVTCPSocket::New( uv_loop_t* loop )
{
  return UVTCPSocketPtr( new UVTCPSocket( loop ) );
}

UVTCPSocket::UVTCPSocket( uv_loop_t* loop )
  : UVStream( reinterpret_cast<uv_stream_t*>( &m_tcp ) )
{
  m_tcp.data = this;

  if ( uv_tcp_init( loop, &m_tcp ) )
    throw Error( "uv_tcp_init" );
}

void UVTCPSocket::OnConnection( EventListener& listener )
{
  m_connection.AddListener( listener );
}

void UVTCPSocket::OnError( EventListener& listener )
{
  m_error.AddListener( listener );
}

void UVTCPSocket::Listen( int port )
{
  Bind( uv_ip4_addr( "127.0.0.1", port ) );
  Listen();
}

UVTCPSocketPtr UVTCPSocket::Accept()
{
  UVTCPSocketPtr client = UVTCPSocket::New( m_tcp.loop );

  if ( uv_accept( Stream(), client->Stream() ) )
    throw Error( "uv_accept" );

  return client;
}

void UVTCPSocket::Bind( const sockaddr_in& addr )
{
  if ( uv_tcp_bind( &m_tcp, addr ) )
    throw Error( "uv_tcp_bind" );
}

void UVTCPSocket::Bind6( const sockaddr_in6& addr6 )
{
  if ( uv_tcp_bind6( &m_tcp, addr6 ) )
    throw Error( "uv_tcp_bind6" );
}

void UVTCPSocket::Listen()
{
  if ( uv_listen( Stream(), 500, OnConnection ) )
    throw Error( "uv_listen" );
}

void UVTCPSocket::OnConnection( int status )
{
  if ( status )
  {
    m_error.Emit();
  }
  else
  {
    m_connection.Emit();
  }
}

void UVTCPSocket::OnConnection( uv_stream_t* tcp, int status )
{
  try
  {
    static_cast<UVTCPSocket*>( tcp->data )->OnConnection( status );
  }
  catch ( ... )
  {
    assert( false );
    abort();
  }
}
