#include "Async/Buffer.h"
#include <gtest/gtest.h>

TEST( BufferTest, WriteRaw )
{
  BufferPtr buffer = Buffer::New( 1 );

  ASSERT_EQ( 1, buffer->Write( reinterpret_cast<const uint8_t*>( "hi" ), 2 ) );
  ASSERT_EQ( buffer->Start() + 1, buffer->Mark() );
  ASSERT_EQ( 0, memcmp( buffer->Start(), "h", 1 ) );
  ASSERT_EQ( 1, buffer->UsedLength() );
  ASSERT_EQ( 0, buffer->FreeLength() );
}

TEST( BufferTest, WriteData )
{
  BufferPtr buffer = Buffer::New( 1 );

  ASSERT_EQ( 1, buffer->Write( Data( "hi" ) ) );
  ASSERT_EQ( buffer->Start() + 1, buffer->Mark() );
  ASSERT_EQ( 0, memcmp( buffer->Start(), "h", 1 ) );
  ASSERT_EQ( 1, buffer->UsedLength() );
  ASSERT_EQ( 0, buffer->FreeLength() );
}

TEST( BufferTest, Slice )
{
  BufferPtr buffer = Buffer::New( 32 );
  Data data( "Hello, world!" );

  ASSERT_EQ( data.Length(), buffer->Write( data ) );
  ASSERT_EQ( data, buffer->Slice( buffer->Start(), buffer->UsedLength() ) );
  ASSERT_EQ( Data( "ello" ), buffer->Slice( buffer->Start() + 1, 4 ) );
}
