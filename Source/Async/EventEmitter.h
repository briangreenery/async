#ifndef Async_EventEmitter_h
#define Async_EventEmitter_h

typedef void (*EventCallback)( void* data );

class EventListener
{
public:
  EventListener( void* data, EventCallback cb );
  ~EventListener();

  void Notify();
  void Disconnect();

private:
  friend class EventEmitter;

  EventListener* m_prev;
  EventListener* m_next;

  void* m_data;
  EventCallback m_cb;
};

class EventEmitter
{
public:
  EventEmitter();
  ~EventEmitter();

  void Emit();
  void AddListener( EventListener& );

private:
  EventEmitter( const EventEmitter& );
  EventEmitter& operator=( const EventEmitter& );

  EventListener m_sentinel;
};

#endif
