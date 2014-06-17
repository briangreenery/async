#include "Async/Error.h"
#include <gtest/gtest.h>

TEST( ErrorTest, IsAStdException )
{
  const char* message = "On noes!";

  try
  {
    throw Error( message );
  }
  catch ( const std::exception& e )
  {
    ASSERT_EQ( message, e.what() );
  }
  catch ( ... )
  {
    FAIL();
  }
}
