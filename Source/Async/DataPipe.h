#ifndef Async_DataPipe_h
#define Async_DataPipe_h

#include "Async/Data.h"
#include "Async/EventEmitter.h"
#include "Async/IntrusivePtr.h"
#include <list>

class DataPipe;
typedef IntrusivePtr<DataPipe> DataPipePtr;

class DataPipe
{
public:
  static DataPipePtr New();

  Data Read();
  void Write( const Data& );
  void WriteEnd();

  void OnWritable( EventListener& );
  void OnReadable( EventListener& );

  bool IsFull() const;
  bool IsEnd() const;
  bool IsEmpty() const;

private:
  friend void IntrusivePtrAddRef( DataPipe* );
  friend void IntrusivePtrRelease( DataPipe* );

  DataPipe();

  DataPipe( const DataPipe& );
  DataPipe& operator=( DataPipe& );

  size_t m_refs;
  bool m_ended;
  std::list<Data> m_queue;
  EventEmitter m_readable;
  EventEmitter m_writable;
};

inline void IntrusivePtrAddRef( DataPipe* pipe )
{
  pipe->m_refs++;
}

inline void IntrusivePtrRelease( DataPipe* pipe )
{
  if ( --pipe->m_refs == 0 )
    delete pipe;
}

#endif
