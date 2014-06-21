#include "Async/IntrusivePtr.h"
#include <gtest/gtest.h>

struct Bearmo
{
  Bearmo() : refs( 0 ), released( false ) {}
  size_t refs;
  bool released;
};

inline void IntrusivePtrAddRef( Bearmo* bearmo )
{
  bearmo->refs++;
}

inline void IntrusivePtrRelease( Bearmo* bearmo )
{
  if ( --bearmo->refs == 0 )
    bearmo->released = true;
}

TEST( IntrusivePtrTest, ConstructorAddsRef )
{
  Bearmo bearmo;
  ASSERT_EQ( 0, bearmo.refs );

  IntrusivePtr<Bearmo> ptr1( &bearmo );
  ASSERT_EQ( 1, bearmo.refs );

  IntrusivePtr<Bearmo> ptr2( ptr1 );
  ASSERT_EQ( 2, bearmo.refs );
}

TEST( IntrusivePtrTest, AssignmentAddsRef )
{
  Bearmo bearmo;
  ASSERT_EQ( 0, bearmo.refs );

  IntrusivePtr<Bearmo> ptr1( &bearmo );
  ASSERT_EQ( 1, bearmo.refs );

  IntrusivePtr<Bearmo> ptr2;
  ptr2 = ptr1;
  ASSERT_EQ( 2, bearmo.refs );
}

TEST( IntrusivePtrTest, SelfAssignment )
{
  Bearmo bearmo;
  ASSERT_EQ( 0, bearmo.refs );

  IntrusivePtr<Bearmo> ptr1( &bearmo );
  ASSERT_EQ( 1, bearmo.refs );

  ptr1 = ptr1;
  ASSERT_EQ( 1, bearmo.refs );
  ASSERT_FALSE( bearmo.released );  
}

TEST( IntrusivePtrTest, DestructorReleases )
{
  Bearmo bearmo;
  ASSERT_EQ( 0, bearmo.refs );
  ASSERT_FALSE( bearmo.released );

  {
    IntrusivePtr<Bearmo> ptr1( &bearmo );
    ASSERT_EQ( 1, bearmo.refs );
  }

  ASSERT_EQ( 0, bearmo.refs );
  ASSERT_TRUE( bearmo.released );
}

TEST( IntrusivePtrTest, GetReturnsPointer )
{
  IntrusivePtr<Bearmo> nullPtr;
  ASSERT_EQ( 0, nullPtr.Get() );

  Bearmo bearmo;
  IntrusivePtr<Bearmo> validPtr( &bearmo );
  ASSERT_EQ( &bearmo, validPtr.Get() );
}

TEST( IntrusivePtrTest, BoolCast )
{
  IntrusivePtr<Bearmo> nullPtr;

  if ( nullPtr )
    FAIL();
  else
    SUCCEED();

  if ( !nullPtr )
    SUCCEED();
  else
    FAIL();

  Bearmo bearmo;
  IntrusivePtr<Bearmo> validPtr( &bearmo );

  if ( validPtr )
    SUCCEED();
  else
    FAIL();

  if ( !validPtr )
    FAIL();
  else
    SUCCEED();
}
