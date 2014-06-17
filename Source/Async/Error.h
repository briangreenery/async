#ifndef Async_Error_h
#define Async_Error_h

#include <exception>

class Error : public std::exception
{
public:
  Error( const char* message );

  virtual const char* what() const throw();

private:
  const char* m_message;
};

#endif
