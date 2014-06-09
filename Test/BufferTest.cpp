#include "Async/Buffer.h"
#include "Async/Data.h"
#include <gtest/gtest.h>

TEST( BufferTest, ZeroSizedBuffer )
{
  BufferPtr buffer = Buffer::New( 0 );
  ASSERT_EQ( 0, buffer->Write( Data( "Hello, world!" ) ) );
}

TEST( BufferTest, BufferWrite )
{
  Data data( "Hello, world!" );
  BufferPtr buffer = Buffer::New( data.Length() + 1 );

  ASSERT_EQ( data.Length(), buffer->Write( data ) );

  ASSERT_EQ( data.Length(), buffer->UsedLength() );
  ASSERT_EQ( 1, buffer->FreeLength() );
  ASSERT_EQ( data, buffer->Used() );

  ASSERT_EQ( 1, buffer->Write( data ) );
  ASSERT_EQ( 0, buffer->FreeLength() );
}
