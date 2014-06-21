#include "UVTTY.h"
#include "Async/Error.h"

UVTTYPtr UVTTY::NewStdIn( uv_loop_t* loop )
{
  return UVTTYPtr( new UVTTY( loop, 0, 1 ) );
}

UVTTYPtr UVTTY::NewStdOut( uv_loop_t* loop )
{
  return UVTTYPtr( new UVTTY( loop, 1, 0 ) );
}

UVTTYPtr UVTTY::NewStdErr( uv_loop_t* loop )
{
  return UVTTYPtr( new UVTTY( loop, 2, 0 ) );
}

UVTTY::UVTTY( uv_loop_t* loop, uv_file fd, int readable )
  : UVStream( reinterpret_cast<uv_stream_t*>( &m_tty ) )
{
  m_tty.data = this;

  if ( uv_tty_init( loop, &m_tty, fd, readable ) )
    throw Error( "uv_tty_init" );
}

static void Destroy( uv_handle_t* handle )
{
  UVTTY* tty = static_cast<UVTTY*>( handle->data );
  delete tty;
}

void UVTTY::Close()
{
  uv_close( reinterpret_cast<uv_handle_t*>( &m_tty ), Destroy );
}
