#include "Async/UVTTY.h"

class Cat
{
public:
  Cat( uv_loop_t* );

  void OnDone();

private:
  static void OnDone( void* );

  UVTTYPtr m_stdin;
  UVTTYPtr m_stdout;
  EventListener m_onDone;
};

Cat::Cat( uv_loop_t* loop )
  : m_stdin( UVTTY::NewStdIn( loop ) )
  , m_stdout( UVTTY::NewStdOut( loop ) )
  , m_onDone( this, OnDone )
{
  DataPipePtr pipe = DataPipe::New();
  m_stdin->ReadTo( pipe );
  m_stdout->WriteFrom( pipe );
}

void Cat::OnDone()
{
  m_stdin = UVTTYPtr();
  m_stdout = UVTTYPtr();
}

void Cat::OnDone( void* data )
{
  static_cast<Cat*>( data )->OnDone();
}

int main()
{
  uv_loop_t* loop = uv_loop_new();
  Cat cat( loop );
  uv_run( loop, UV_RUN_DEFAULT );
  return 0;
}
