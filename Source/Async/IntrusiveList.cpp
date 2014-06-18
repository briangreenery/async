#include "IntrusiveList.h"

IntrusiveListElement::IntrusiveListElement() : m_prev( 0 ), m_next( 0 )
{
}

IntrusiveListElement::~IntrusiveListElement()
{
  Disconnect();
}

bool IntrusiveListElement::IsConnected() const
{
  return m_prev && m_next;
}

void IntrusiveListElement::Disconnect()
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

IntrusiveListElement* IntrusiveListBase::Pop()
{
  if ( IsEmpty() )
    return 0;

  IntrusiveListElement* element = m_sentinel.m_next;
  element->Disconnect();
  return element;
}

void IntrusiveListBase::Append( IntrusiveListElement& element )
{
  IntrusiveListElement* after = m_sentinel.m_prev;

  element.m_prev = after;
  element.m_next = after->m_next;

  element.m_prev->m_next = &element;
  element.m_next->m_prev = &element;
}

void IntrusiveListBase::Prepend( IntrusiveListBase& other )
{
  if ( other.IsEmpty() )
    return;

  IntrusiveListElement* otherHead = other.m_sentinel.m_next;
  IntrusiveListElement* otherTail = other.m_sentinel.m_prev;

  IntrusiveListElement* myHead = m_sentinel.m_next;

  // Connect our sentinel with the head of the other list.

  m_sentinel.m_next = otherHead;
  otherHead->m_prev = &m_sentinel;

  // Connect the tail of the other list with our head.

  otherTail->m_next = myHead;
  myHead->m_prev = otherTail;

  // Reset the other sentinel so it's a valid empty list.

  other.m_sentinel.m_prev = &other.m_sentinel;
  other.m_sentinel.m_next = &other.m_sentinel;
}
