#include "Async/EventEmitter.h"
#include <gtest/gtest.h>

static void SetTrue( void* data )
{
  *static_cast<bool*>( data ) = true;
}

static void Increment( void* data )
{
  *static_cast<int*>( data ) += 1;
}

TEST( EventEmitterTest, EventListenerNotify )
{
  bool wasNotified = false;
  EventListener listener( &wasNotified, SetTrue );
  listener.Notify();
  ASSERT_TRUE( wasNotified );
}

TEST( EventEmitterTest, SetCallback )
{
  int count1 = 0;
  EventListener listener( &count1, Increment );

  listener.Notify();

  int count2 = 0;
  listener.SetCallback( &count2, Increment );

  listener.Notify();

  ASSERT_EQ( 1, count1 );
  ASSERT_EQ( 1, count2 );
}

TEST( EventEmitterTest, NotifyOneListener )
{
  bool wasNotified = false;
  EventListener listener( &wasNotified, SetTrue );

  EventEmitter emitter;
  emitter.AddListener( listener );

  emitter.Emit();
  ASSERT_TRUE( wasNotified );
}

TEST( EventEmitterTest, NotifyTwoListeners )
{
  bool firstWasNotified = false;
  EventListener firstListener( &firstWasNotified, SetTrue );

  bool secondWasNotified = false;
  EventListener secondListener( &secondWasNotified, SetTrue );

  EventEmitter emitter;
  emitter.AddListener( firstListener );
  emitter.AddListener( secondListener );

  emitter.Emit();

  ASSERT_TRUE( firstWasNotified );
  ASSERT_TRUE( secondWasNotified );
}

TEST( EventEmitterTest, DisconnectManually )
{
  bool firstWasNotified = false;
  EventListener firstListener( &firstWasNotified, SetTrue );

  bool secondWasNotified = false;
  EventListener secondListener( &secondWasNotified, SetTrue );

  EventEmitter emitter;
  emitter.AddListener( firstListener );
  emitter.AddListener( secondListener );

  firstListener.Disconnect();
  emitter.Emit();

  ASSERT_FALSE( firstWasNotified );
  ASSERT_TRUE( secondWasNotified );
}

static void DisconnectListener( void* data )
{
  static_cast<EventListener*>( data )->Disconnect();
}

TEST( EventEmitterTest, DisconnectDuringNotify )
{
  bool wasNotified;
  EventListener normalListener( &wasNotified, SetTrue );
  EventListener disconnectingListener( &normalListener, DisconnectListener );

  EventEmitter emitter;
  emitter.AddListener( disconnectingListener );
  emitter.AddListener( normalListener );

  emitter.Emit();
  ASSERT_FALSE( wasNotified );
}

TEST( EventEmitterTest, ThrowsWhenAddedTwice )
{
  bool wasNotified = false;
  EventListener listener( &wasNotified, SetTrue );

  EventEmitter emitter;
  emitter.AddListener( listener );

  ASSERT_THROW( emitter.AddListener( listener ), std::exception );
}

TEST( EventEmitterTest, RearmsAutomatically )
{
  int count = 0;
  EventListener listener( &count, Increment );

  EventEmitter emitter;
  emitter.AddListener( listener );

  emitter.Emit();
  emitter.Emit();

  ASSERT_EQ( 2, count );
}

struct EmitterListener
{
  EventEmitter* emitter;
  EventListener* listener;
};

static void AddListener( void* data )
{
  EmitterListener* fixture = static_cast<EmitterListener*>( data );
  fixture->emitter->AddListener( *fixture->listener );
}

TEST( EventEmitterTest, ListenersAddedDuringEmitAreNotNotified )
{
  // In this test, 'adder' adds 'listener' during emitter.Emit(). Since it's
  // added after emitter.Emit() is called it should not be notified.

  EventEmitter emitter;

  bool wasNotified;
  EventListener listener( &wasNotified, SetTrue );

  EmitterListener fixture = { &emitter, &listener };
  EventListener adder( &fixture, AddListener );

  emitter.AddListener( adder );
  emitter.Emit();

  ASSERT_FALSE( wasNotified );
}

struct OrderTracker
{
  std::vector<std::string>* order;
  std::string id;
};

static void TrackOrder( void* data )
{
  OrderTracker* fixture = static_cast<OrderTracker*>( data );
  fixture->order->push_back( fixture->id );
}

TEST( EventEmitterTest, ListenersNotifiedInOrder )
{
  // In this test 'listenerA' adds 'listenerC' during emitter.Emit().
  // Afterwards, we Emit() again to make sure that 'listenerA' is notified
  // before 'listenerB', which is notified before 'listenerC'.

  EventEmitter emitter;

  std::vector<std::string> order;

  OrderTracker trackerC = { &order, "C" };
  EventListener listenerC( &trackerC, TrackOrder );

  EmitterListener fixture = { &emitter, &listenerC };
  EventListener listenerA( &fixture, AddListener );

  OrderTracker trackerB = { &order, "B" };
  EventListener listenerB( &trackerB, TrackOrder );

  emitter.AddListener( listenerA );
  emitter.AddListener( listenerB );

  emitter.Emit();
  order.clear();

  OrderTracker trackerA = { &order, "A" };
  listenerA.SetCallback( &trackerA, TrackOrder );

  emitter.Emit();

  std::vector<std::string> expected;
  expected.push_back( "A" );
  expected.push_back( "B" );
  expected.push_back( "C" );

  ASSERT_EQ( expected, order );
}
