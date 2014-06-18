#ifndef Async_IntrusiveList_h
#define Async_IntrusiveList_h

class IntrusiveListHook
{
public:
  IntrusiveListHook();
  ~IntrusiveListHook();

  bool IsConnected() const;
  void Disconnect();

private:
  friend class IntrusiveListBase;

  IntrusiveListHook( const IntrusiveListHook& );
  IntrusiveListHook& operator=( const IntrusiveListHook& );

  IntrusiveListHook* m_prev;
  IntrusiveListHook* m_next;
};

class IntrusiveListBase
{
public:
  IntrusiveListBase();
  ~IntrusiveListBase();

  bool IsEmpty() const;

  // Pops an element from the beginning of this list.
  IntrusiveListHook* Pop();

  // Appends an element to the end of this list.
  void Append( IntrusiveListHook& element );

  // Moves the contents of 'list' to this list at the beginning of the list.
  void Prepend( IntrusiveListBase& list );

protected:
  IntrusiveListHook m_sentinel;
};

template <class T>
class IntrusiveList : public IntrusiveListBase
{
public:
  T* Pop();
};

template <class T>
inline T* IntrusiveList<T>::Pop()
{
  return static_cast<T*>( IntrusiveListBase::Pop() );
}

#endif
