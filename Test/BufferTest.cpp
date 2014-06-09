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
  BufferPtr buffer = Buffer::New( 1024 );

  Data data( "Hello, world!" );
  ASSERT_EQ( data.Length(), buffer->Write( data ) );
}
