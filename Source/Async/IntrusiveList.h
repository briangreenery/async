#ifndef Async_IntrusiveList_h
#define Async_IntrusiveList_h

struct IntrusiveListHook
{
  IntrusiveListHook();
  ~IntrusiveListHook();

  IntrusiveListHook* m_prev;
  IntrusiveListHook* m_next;
};

inline IntrusiveListHook::IntrusiveListHook() : m_prev( 0 ), m_next( 0 )
{
}

inline IntrusiveListHook::~IntrusiveListHook()
{
  if ( m_prev )
  {
    m_next->m_prev = m_prev;
    m_prev->m_next = m_next;
  }
}

class IntrusiveList
{
public:
  void PushBack( IntrusiveListHook* );

private:
  IntrusiveListHook m_sentinel;
};

#endif
