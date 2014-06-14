#include "DataQueue.h"

DataQueue::DataQueue()
  : m_wantRead( false )
  , m_wantWrite( false )
{
}

void DataQueue::Write( const Data& data )
{
  if ( m_queue.empty() || !m_queue.front().Coalesce( data ) )
    m_queue.push_front( data );

  if ( m_wantRead )
  {
    m_wantRead = false;
    m_readable.Emit();
  }
}

Data DataQueue::Read()
{
  if ( m_queue.empty() )
  {
    m_wantRead = true;
    return Data();
  }

  bool wasWriteable = Writeable();

  Data result = m_queue.back();
  m_queue.pop_back();

  if ( !wasWriteable )
    m_writeable.Emit();

  return result;
}

void DataQueue::OnWriteable( Callback& cb )
{
  m_writeable.Connect( cb );
}

void DataQueue::OnReadable( Callback& cb )
{
  m_readable.Connect( cb );
}
