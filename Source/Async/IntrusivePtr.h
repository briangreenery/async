#ifndef Async_IntrusivePtr_h
#define Async_IntrusivePtr_h

// A generic reference counted pointer.

template <class T>
class IntrusivePtr
{
public:
  IntrusivePtr();
  explicit IntrusivePtr( T* t );
  IntrusivePtr( const IntrusivePtr<T>& );
  ~IntrusivePtr();

  T* Get() const;

  IntrusivePtr<T>& operator=( const IntrusivePtr<T>& );

  T* operator->() const;
  T& operator*() const;

  typedef T* ( IntrusivePtr::*SafeBoolType )() const;
  operator SafeBoolType() const
  {
    return m_t ? &IntrusivePtr::Get : 0;
  }

private:
  T* m_t;
};

template <class T>
IntrusivePtr<T>::IntrusivePtr()
  : m_t( 0 )
{
}

template <class T>
IntrusivePtr<T>::IntrusivePtr( T* t )
  : m_t( t )
{
  if ( m_t )
    IntrusivePtrAddRef( m_t );
}

template <class T>
IntrusivePtr<T>::IntrusivePtr( const IntrusivePtr<T>& other )
  : m_t( other.m_t )
{
  if ( m_t )
    IntrusivePtrAddRef( m_t );
}

template <class T>
IntrusivePtr<T>& IntrusivePtr<T>::operator=( const IntrusivePtr<T>& other )
{
  if ( other.m_t )
    IntrusivePtrAddRef( other.m_t );

  if ( m_t )
    IntrusivePtrRelease( m_t );

  m_t = other.m_t;
  return *this;
}

template <class T>
IntrusivePtr<T>::~IntrusivePtr()
{
  if ( m_t )
    IntrusivePtrRelease( m_t );
}

template <class T>
T* IntrusivePtr<T>::Get() const
{
  return m_t;
}

template <class T>
T* IntrusivePtr<T>::operator->() const
{
  return m_t;
}

template <class T>
T& IntrusivePtr<T>::operator*() const
{
  return *m_t;
}

#endif
