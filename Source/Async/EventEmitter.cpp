#include "EventEmitter.h"
#include "Async/Error.h"

static void IgnoreNotification( void* )
{
}

EventListener::EventListener( void* data, EventCallback cb )
  : m_prev( 0 ), m_next( 0 ), m_data( data ), m_cb( cb )
{
}

EventListener::~EventListener()
{
  Disconnect();
}

void EventListener::Notify()
{
  m_cb( m_data );
}

void EventListener::Disconnect()
{
  if ( m_prev && m_next )
  {
    m_prev->m_next = m_next;
    m_next->m_prev = m_prev;
  }

  m_prev = 0;
  m_next = 0;
}

EventEmitter::EventEmitter()
  : m_sentinel( 0, IgnoreNotification )
{
  m_sentinel.m_prev = &m_sentinel;
  m_sentinel.m_next = &m_sentinel;
}

EventEmitter::~EventEmitter()
{
  while ( m_sentinel.m_next != &m_sentinel )
  {
    EventListener* listener = m_sentinel.m_next;
    listener->Disconnect();
  }
}

void EventEmitter::AddListener( EventListener& listener )
{
  if ( listener.m_prev || listener.m_next )
    throw Error( "Event listener is already connected to another emitter" );

  EventListener* after = m_sentinel.m_prev;

  listener.m_prev = after;
  listener.m_next = after->m_next;

  listener.m_prev->m_next = &listener;
  listener.m_next->m_prev = &listener;
}

void EventEmitter::Emit()
{
  while ( m_sentinel.m_next != &m_sentinel )
  {
    EventListener* listener = m_sentinel.m_next;
    listener->Disconnect();
    listener->Notify();
  }
}
