#include "Async/UVTCPSocket.h"
#include "Async/UVTTY.h"
#include <iostream>

static UVTCPSocketPtr listener;
static UVTCPSocketPtr client;
static UVTTYPtr stdOut;

static void OnConnection( void* )
{
  client = listener->Accept();

  DataPipePtr pipe = DataPipe::New();
  client->Reader().Pipe( pipe );
  stdOut->Writer().Pipe( pipe );

  listener.Reset();
}

int main( int argc, const char* argv[] )
{
  if ( argc != 2 )
  {
    std::cout << "Usage: nc <port>" << std::endl;
    return 1;
  }

  uv_loop_t* loop = uv_loop_new();

  listener = UVTCPSocket::New( loop );
  stdOut = UVTTY::NewStdOut( loop );

  EventListener onConnection( 0, OnConnection );
  listener->OnConnection( onConnection );
  listener->Listen( atoi( argv[1] ) );

  uv_run( loop, UV_RUN_DEFAULT );
  return 0;
}
