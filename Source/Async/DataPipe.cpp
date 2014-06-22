#include "DataPipe.h"
#include "Async/Error.h"

DataPipePtr DataPipe::New()
{
  return IntrusivePtr<DataPipe>( new DataPipe() );
}

DataPipe::DataPipe() : m_ended( false )
{
}

Data DataPipe::Read()
{
  if ( m_queue.empty() )
    return Data();

  bool wasFull = IsFull();

  Data result = m_queue.back();
  m_queue.pop_back();

  if ( wasFull )
    m_writable.Emit();

  return result;
}

void DataPipe::Write( const Data& data )
{
  if ( m_ended )
    throw Error( "Can't write data to an ended pipe." );

  bool wasEmpty = IsEmpty();

  if ( m_queue.empty() || !m_queue.front().Coalesce( data ) )
    m_queue.push_front( data );

  if ( wasEmpty )
    m_readable.Emit();
}

void DataPipe::WriteEnd()
{
  if ( m_ended )
    throw Error( "The pipe has already been ended." );

  m_ended = true;

  if ( m_queue.empty() )
    m_readable.Emit();
}

bool DataPipe::IsFull() const
{
  return m_queue.size() >= 8;
}

bool DataPipe::IsEnd() const
{
  return m_ended && m_queue.empty();
}

bool DataPipe::IsEmpty() const
{
  return m_queue.empty();
}

void DataPipe::OnWritable( EventListener& listener )
{
  m_writable.AddListener( listener );
}

void DataPipe::OnReadable( EventListener& listener )
{
  m_readable.AddListener( listener );
}
