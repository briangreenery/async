#include "Async/IntrusiveList.h"
#include <gtest/gtest.h>

TEST( IntrusiveListTest, AppendTest )
{
  IntrusiveListBase list;
  ASSERT_TRUE( list.IsEmpty() );

  IntrusiveListElement element;
  ASSERT_FALSE( element.IsConnected() );

  list.Append( element );

  ASSERT_FALSE( list.IsEmpty() );
  ASSERT_TRUE( element.IsConnected() );
}

TEST( IntrusiveListTest, AppendShouldDisconnect )
{
  IntrusiveListBase list1;
  IntrusiveListBase list2;

  IntrusiveListElement element;
  list1.Append( element );
  list2.Append( element );

  ASSERT_TRUE( list1.IsEmpty() );
  ASSERT_FALSE( list2.IsEmpty() );
}

TEST( IntrusiveListTest, PoppedInOrder )
{
  IntrusiveListBase list;

  IntrusiveListElement first;
  IntrusiveListElement second;

  list.Append( first );
  list.Append( second );

  IntrusiveListElement* firstPopped = list.Pop();
  IntrusiveListElement* secondPopped = list.Pop();
  IntrusiveListElement* thirdPopped = list.Pop();

  ASSERT_TRUE( firstPopped == &first );
  ASSERT_TRUE( secondPopped == &second );
  ASSERT_TRUE( thirdPopped == 0 );

  ASSERT_FALSE( first.IsConnected() );
  ASSERT_FALSE( second.IsConnected() );

  ASSERT_TRUE( list.IsEmpty() );
}

TEST( IntrusiveListTest, ElementDestructorDisconnects )
{
  IntrusiveListBase list;

  {
    IntrusiveListElement element;
    list.Append( element );

    ASSERT_FALSE( list.IsEmpty() );
  }

  ASSERT_TRUE( list.IsEmpty() );
}

TEST( IntrusiveListTest, PrependEmptyToEmpty )
{
  IntrusiveListBase a;
  IntrusiveListBase b;

  a.Prepend( b );

  ASSERT_TRUE( a.IsEmpty() );
  ASSERT_TRUE( b.IsEmpty() );
}

TEST( IntrusiveListTest, PrependSomethingToNothing )
{
  IntrusiveListBase a;
  IntrusiveListBase b;

  IntrusiveListElement elementB;
  b.Append( elementB );

  a.Prepend( b );

  ASSERT_FALSE( a.IsEmpty() );
  ASSERT_TRUE( b.IsEmpty() );

  IntrusiveListElement* popped = a.Pop();
  ASSERT_TRUE( popped == &elementB );
}

TEST( IntrusiveListTest, PrependSomethingToSomething )
{
  IntrusiveListBase a;
  IntrusiveListBase b;

  IntrusiveListElement elementA;
  IntrusiveListElement elementB;

  a.Append( elementA );
  b.Append( elementB );

  a.Prepend( b );

  ASSERT_FALSE( a.IsEmpty() );
  ASSERT_TRUE( b.IsEmpty() );

  IntrusiveListElement* firstPopped = a.Pop();
  IntrusiveListElement* secondPopped = a.Pop();

  ASSERT_TRUE( firstPopped == &elementB );
  ASSERT_TRUE( secondPopped == &elementA );
}
