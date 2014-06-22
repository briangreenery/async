#ifndef UVTTY_h
#define UVTTY_h

#include "Async/UVStream.h"

class UVTTY;
typedef IntrusivePtr<UVTTY> UVTTYPtr;

class UVTTY : public UVStream
{
public:
  static UVTTYPtr NewStdIn( uv_loop_t* );
  static UVTTYPtr NewStdOut( uv_loop_t* );
  static UVTTYPtr NewStdErr( uv_loop_t* );

private:
  UVTTY( uv_loop_t*, uv_file, int readable );

  uv_tty_t m_tty;
};

#endif
