#include "Error.h"

Error::Error( const char* message ) : m_message( message )
{
}

const char* Error::what() const throw()
{
  return m_message;
}
