#include "Async/EventEmitter.h"
#include <gtest/gtest.h>

static void SetTrue( void* data )
{
  *static_cast<bool*>( data ) = true;
}

TEST( EventEmitterTest, EventListenerNotify )
{
  bool wasNotified = false;
  EventListener listener( &wasNotified, SetTrue );
  listener.Notify();
  ASSERT_TRUE( wasNotified );
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
