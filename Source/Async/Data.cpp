#include "Data.h"
#include <string.h>

bool operator==( const Data& a, const Data& b )
{
  return ( a.Length() == b.Length() ) &&
         ( memcmp( a.Start(), b.Start(), a.Length() ) == 0 );
}
