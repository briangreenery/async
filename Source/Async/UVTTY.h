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
  friend void IntrusivePtrAddRef( UVTTY* );
  friend void IntrusivePtrRelease( UVTTY* );

  UVTTY( uv_loop_t*, uv_file, int readable );
  void Close();

  size_t m_refs;
  uv_tty_t m_tty;
};

inline void IntrusivePtrAddRef( UVTTY* tty )
{
  tty->m_refs++;
}

inline void IntrusivePtrRelease( UVTTY* tty )
{
  if ( --tty->m_refs == 0 )
    tty->Close();
}


#endif
