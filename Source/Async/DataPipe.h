#ifndef Async_DataPipe_h
#define Async_DataPipe_h

#include "Async/Data.h"
#include <list>

class DataPipe
{
public:
  DataPipe();

  void Write( const Data& );
  void OnWriteable( EventListener& );

  Data Read();
  void OnReadable( EventListener& );

  bool IsFull() const;
  void IsEmpty() const;

private:
  std::list<Data> m_queue;

  bool m_wantRead;
  bool m_wantWrite;

  EventEmitter m_readable;
  EventEmitter m_writeable;
};

inline bool DataPipe::IsFull() const
{
  return m_queue.size() < 16;
}

inline bool DataPipe::IsEmpty() const
{
  return m_queue.empty();
}

#endif
