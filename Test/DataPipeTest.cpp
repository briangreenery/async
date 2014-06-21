#include "Async/DataPipe.h"
#include <gtest/gtest.h>

TEST( DataPipeTest, NewPipeIsEmpty )
{
  DataPipePtr pipe = DataPipe::New();

  ASSERT_TRUE( pipe->IsEmpty() );
  ASSERT_FALSE( pipe->IsFull() );
}

TEST( DataPipeTest, CanPipeData )
{
  DataPipePtr pipe = DataPipe::New();

  pipe->Write( Data( "hello, " ) );
  pipe->Write( Data( "world!" ) );

  ASSERT_FALSE( pipe->IsEmpty() );

  ASSERT_TRUE( pipe->Read() == Data( "hello, " ) );
  ASSERT_TRUE( pipe->Read() == Data( "world!" ) );

  ASSERT_TRUE( pipe->IsEmpty() );
  ASSERT_TRUE( pipe->Read() == Data() );
}

TEST( DataPipeTest, PipeUsesCoalesce )
{
  DataPipePtr pipe = DataPipe::New();
  Data data( "Hello, world!" );

  pipe->Write( data.Slice( data.Start(), 7 ) );
  pipe->Write( data.Slice( data.Start() + 7, 6 ) );

  ASSERT_TRUE( data == pipe->Read() );
  ASSERT_TRUE( pipe->IsEmpty() );
}

TEST( DataPipeTest, PipeFillsUp )
{
  DataPipePtr pipe = DataPipe::New();

  for ( size_t i = 0; i < 7; i++ )
    pipe->Write( Data( "hello" ) );

  ASSERT_FALSE( pipe->IsFull() );
  pipe->Write( Data( "hello" ) );
  ASSERT_TRUE( pipe->IsFull() );
  pipe->Read();
  ASSERT_FALSE( pipe->IsFull() );
}

static void SetTrue( void* data )
{
  *static_cast<bool*>( data ) = true;
}

TEST( DataPipeTest, EmitsReadableWhenNotEmptyAnymore )
{
  DataPipePtr pipe = DataPipe::New();

  bool wasNotified = false;
  EventListener listener( &wasNotified, SetTrue );
  pipe->OnReadable( listener );

  pipe->Write( Data( "hello" ) );
  ASSERT_TRUE( wasNotified );

  wasNotified = false;
  pipe->Write( Data( "world" ) );
  ASSERT_FALSE( wasNotified );
}

TEST( DataPipeTest, EmitsWriteableWhenNotFullAnymore )
{
  DataPipePtr pipe = DataPipe::New();

  bool wasNotified = false;
  EventListener listener( &wasNotified, SetTrue );
  pipe->OnWriteable( listener );

  for ( size_t i = 0; i < 8; i++ )
    pipe->Write( Data( "hello" ) );

  ASSERT_TRUE( pipe->IsFull() );
  ASSERT_FALSE( wasNotified );

  pipe->Read();
  ASSERT_TRUE( wasNotified );
}
