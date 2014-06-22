#ifndef Async_UVTCPServer_h
#define Async_UVTCPServer_h

#include <uv.h>

class UVTCPServer
{
public:
  UVTCPServer( uv_loop_t* );

  void Listen( const char* ip, int port, EventListener& onConnection );
  UVTCPSocketPtr Accept();

private:
  EventEmitter m_connection;
};

#endif
