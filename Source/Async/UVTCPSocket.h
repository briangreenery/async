#ifndef Async_UVTCPSocket_h
#define Async_UVTCPSocket_h

#include "Async/UVStream.h"

class UVTCPSocket;
typedef IntrusivePtr<UVTCPSocket> UVTCPSocketPtr;

class UVTCPSocket : public UVStream
{
public:
  static UVTCPSocketPtr New( uv_loop_t* );

  void OnConnection( EventListener& );
  void OnError( EventListener& );

  void Listen( int port );
  UVTCPSocketPtr Accept();

private:
  explicit UVTCPSocket( uv_loop_t* );

  UVTCPSocket( const UVTCPSocket& );
  UVTCPSocket& operator=( const UVTCPSocket& );

  void Bind( const sockaddr_in& );
  void Bind6( const sockaddr_in6& );
  void Listen();

  void OnConnection( int status );
  static void OnConnection( uv_stream_t*, int status );

  uv_tcp_t m_tcp;

  EventEmitter m_connection;
  EventEmitter m_error;
};

#endif
