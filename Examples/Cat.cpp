#include "Async/UVTTY.h"

int main()
{
  uv_loop_t* loop = uv_loop_new();
  
  UVTTYPtr stdIn = UVTTY::NewStdIn( loop );
  UVTTYPtr stdOut = UVTTY::NewStdOut( loop );

  stdIn->Pipe( *stdOut );

  uv_run( loop, UV_RUN_DEFAULT );
  return 0;
}
