#include "EventEmitter.h"
#include "Async/Error.h"

EventListener::EventListener( void* data, EventCallback cb )
  : m_data( data ), m_cb( cb )
{
}

void EventListener::Notify()
{
  m_cb( m_data );
}

void EventListener::SetCallback( void* data, EventCallback cb )
{
  m_data = data;
  m_cb = cb;
}

void EventEmitter::AddListener( EventListener& listener )
{
  if ( listener.IsConnected() )
    throw Error( "Event listener is already connected to another emitter" );

  m_listeners.Append( listener );
}

void EventEmitter::Emit()
{
  IntrusiveList<EventListener> pending;
  IntrusiveList<EventListener> notified;

  pending.Prepend( m_listeners );

  while ( EventListener* listener = pending.Pop() )
  {
    notified.Append( *listener );
    listener->Notify();
  }

  m_listeners.Prepend( notified );
}
