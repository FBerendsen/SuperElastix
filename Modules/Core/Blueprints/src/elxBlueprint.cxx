#ifndef __Blueprint_cxx
#define __Blueprint_cxx

#include "elxBlueprint.h"

namespace elx {

void
Blueprint
::PrintSelf( void )
{
  this->m_Test = 1;
  std::cout << "Blueprint" << std::endl;
}

} // namespace elx 

#endif // __Blueprint_cxx