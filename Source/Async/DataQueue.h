#ifndef Async_DataQueue_h
#define Async_DataQueue_h

#include "Async/Data.h"
#include <list>

class DataQueue
{
public:
  DataQueue();

  void Write( const Data& );
  bool Writeable() const;
  void OnWriteable( Callback& );

  Data Read();
  void Readable() const;
  void OnReadable( Callback& );

private:
  std::list<Data> m_queue;

  bool m_wantRead;
  bool m_wantWrite;

  CallbackSlot m_readable;
  CallbackSlot m_writeable;
};

inline bool DataQueue::Writeable() const
{
  return m_queue.size() < 16;
}

inline bool DataQueue::Readable() const
{
  return !m_queue.empty();
}

#endif
