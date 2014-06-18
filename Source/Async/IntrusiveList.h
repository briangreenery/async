#ifndef Async_IntrusiveList_h
#define Async_IntrusiveList_h

// An element in an IntrusiveList. It automatically removes itself in its
// destructor.

class IntrusiveListElement
{
public:
  IntrusiveListElement();
  ~IntrusiveListElement();

  bool IsConnected() const;
  void Disconnect();

private:
  friend class IntrusiveListBase;

  IntrusiveListElement( const IntrusiveListElement& );
  IntrusiveListElement& operator=( const IntrusiveListElement& );

  IntrusiveListElement* m_prev;
  IntrusiveListElement* m_next;
};

// A list that relies on elements to inherit from IntrusiveListElement. This
// allows it to add and remove elements without memory allocation.

class IntrusiveListBase
{
public:
  IntrusiveListBase();
  ~IntrusiveListBase();

  bool IsEmpty() const;

  // Pops an element from the beginning of this list.
  IntrusiveListElement* Pop();

  // Appends an element to the end of this list.
  void Append( IntrusiveListElement& element );

  // Moves the contents of 'list' to this list at the beginning of the list.
  void Prepend( IntrusiveListBase& list );

protected:
  IntrusiveListElement m_sentinel;
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
