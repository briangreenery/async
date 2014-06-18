#ifndef Async_EventEmitter_h
#define Async_EventEmitter_h

#include "Async/IntrusiveList.h"

typedef void (*EventCallback)( void* data );

class EventListener : public IntrusiveListHook
{
public:
  EventListener( void* data, EventCallback cb );

  void Notify();
  void SetCallback( void* data, EventCallback cb );

private:
  void* m_data;
  EventCallback m_cb;
};

class EventEmitter
{
public:
  void Emit();
  void AddListener( EventListener& );

private:
  IntrusiveList<EventListener> m_listeners;
};

#endif
