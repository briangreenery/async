#ifndef Async_UVHandle_h
#define Async_UVHandle_h

#include <uv.h>

class UVHandle
{
public:
  explicit UVHandle( uv_handle_t* );
  virtual ~UVHandle();

private:
  friend void IntrusivePtrAddRef( UVHandle* );
  friend void IntrusivePtrRelease( UVHandle* );

  UVHandle( const UVHandle& );
  UVHandle& operator=( const UVHandle& );

  void Close();

  uv_handle_t* m_handle;
  size_t m_refs;
};

inline void IntrusivePtrAddRef( UVHandle* handle )
{
  handle->m_refs++;
}

inline void IntrusivePtrRelease( UVHandle* handle )
{
  if ( --handle->m_refs )
    handle->Close();
}

#endif
