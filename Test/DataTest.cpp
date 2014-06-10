#include "Async/Buffer.h"
#include "Async/Data.h"
#include <gtest/gtest.h>

TEST( DataTest, ManagesBufferRefCount )
{
  BufferPtr buffer = Buffer::New( 0 );

  ASSERT_EQ( 1, buffer->Refs() );

  {
    Data data = buffer->Slice( buffer->Start(), 0 );
    ASSERT_EQ( 2, buffer->Refs() );
  }

  ASSERT_EQ( 1, buffer->Refs() );
}

TEST( DataTest, LiteralConstructor )
{
  Data data( "Hello" );

  ASSERT_EQ( 0, memcmp( data.Start(), "Hello", 5 ) );
  ASSERT_EQ( 5, data.Length() );
}
