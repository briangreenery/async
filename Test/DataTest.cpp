#include "Async/Buffer.h"
#include "Async/Data.h"
#include <gtest/gtest.h>

TEST( DataTest, ManagesRefCount )
{
  BufferPtr buffer = Buffer::New( 0 );

  ASSERT_EQ( 1, buffer->Refs() );

  {
    Data data = buffer->Slice( buffer->Start(), 0 );
    ASSERT_EQ( 2, buffer->Refs() );

    Data data2 = data.Slice( data.Start(), 0 );
    ASSERT_EQ( 3, buffer->Refs() );
  }

  ASSERT_EQ( 1, buffer->Refs() );
}

TEST( DataTest, LiteralConstructor )
{
  Data data( "Hello" );

  ASSERT_EQ( 0, memcmp( data.Start(), "Hello", 5 ) );
  ASSERT_EQ( 5, data.Length() );
}

TEST( DataTest, Slice )
{
  Data data( "Hello" );

  ASSERT_EQ( data, data.Slice( data.Start(), data.Length() ) );
  ASSERT_EQ( Data( "lo" ), data.Slice( data.Start() + 3, 2 ) );
}

TEST( DataTest, CoalesceNeighbors )
{
  Data helloworld( "Hello, world!" );

  Data hello = helloworld.Slice( helloworld.Start(), 7 );
  Data world = helloworld.Slice( helloworld.Start() + 7, 6 );

  ASSERT_TRUE( hello.Coalesce( world ) );
  ASSERT_TRUE( helloworld == hello );
}

TEST( DataTest, CoalesceUnrelated )
{
  Data helloworld( "Hello, world!" );

  Data hello( "Hello, " );
  Data world( "world!" );

  ASSERT_FALSE( hello.Coalesce( world ) );
  ASSERT_FALSE( helloworld == hello );
}
