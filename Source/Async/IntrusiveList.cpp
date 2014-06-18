#include "IntrusiveList.h"

IntrusiveListHook::IntrusiveListHook() : m_prev( 0 ), m_next( 0 )
{
}

IntrusiveListHook::~IntrusiveListHook()
{
  Disconnect();
}

bool IntrusiveListHook::IsConnected() const
{
  return m_prev && m_next;
}

void IntrusiveListHook::Disconnect()
{
  if ( IsConnected() )
  {
    m_prev->m_next = m_next;
    m_next->m_prev = m_prev;
  }

  m_prev = 0;
  m_next = 0;
}

IntrusiveListBase::IntrusiveListBase()
{
  m_sentinel.m_prev = &m_sentinel;
  m_sentinel.m_next = &m_sentinel;
}

IntrusiveListBase::~IntrusiveListBase()
{
  while ( Pop() )
  {
    // This loop intentionally left blank.
  }
}

bool IntrusiveListBase::IsEmpty() const
{
  return m_sentinel.m_next == &m_sentinel;
}

IntrusiveListHook* IntrusiveListBase::Pop()
{
  if ( IsEmpty() )
    return 0;

  IntrusiveListHook* element = m_sentinel.m_next;
  element->Disconnect();
  return element;
}

void IntrusiveListBase::Append( IntrusiveListHook& element )
{
  IntrusiveListHook* after = m_sentinel.m_prev;

  element.m_prev = after;
  element.m_next = after->m_next;

  element.m_prev->m_next = &element;
  element.m_next->m_prev = &element;
}

void IntrusiveListBase::Prepend( IntrusiveListBase& other )
{
  IntrusiveListHook* otherHead = other.m_sentinel.m_next;
  IntrusiveListHook* otherTail = other.m_sentinel.m_prev;

  IntrusiveListHook* myHead = m_sentinel.m_next;

  otherTail->m_next = myHead;
  myHead->m_prev = otherTail;

  m_sentinel.m_next = otherHead;
  otherHead->m_prev = &m_sentinel;

  other.m_sentinel.m_prev = &other.m_sentinel;
  other.m_sentinel.m_next = &other.m_sentinel;
}
