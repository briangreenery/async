#include "UVHandle.h"

UVHandle::UVHandle( uv_handle_t* handle ) : m_handle( handle ), m_refs( 0 )
{
}

UVHandle::~UVHandle()
{
}

static void OnCloseComplete( uv_handle_t* handle )
{
  delete static_cast<UVHandle*>( handle->data );
}

void UVHandle::Close()
{
  uv_close( m_handle, OnCloseComplete );
}
