#include "DataPipe.h"

DataPipePtr DataPipe::New()
{
  return IntrusivePtr<DataPipe>( new DataPipe() );
}

DataPipe::DataPipe()
{
}

void DataPipe::Write( const Data& data )
{
  bool wasEmpty = IsEmpty();

  if ( m_queue.empty() || !m_queue.front().Coalesce( data ) )
    m_queue.push_front( data );

  if ( wasEmpty )
    m_readable.Emit();
}

Data DataPipe::Read()
{
  if ( m_queue.empty() )
    return Data();

  bool wasFull = IsFull();

  Data result = m_queue.back();
  m_queue.pop_back();

  if ( wasFull )
    m_writeable.Emit();

  return result;
}

bool DataPipe::IsFull() const
{
  return m_queue.size() >= 8;
}

bool DataPipe::IsEmpty() const
{
  return m_queue.empty();
}

void DataPipe::OnWriteable( EventListener& listener )
{
  m_writeable.AddListener( listener );
}

void DataPipe::OnReadable( EventListener& listener )
{
  m_readable.AddListener( listener );
}
