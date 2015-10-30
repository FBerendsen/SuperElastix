#ifndef __Blueprint_h
#define __Blueprint_h

#include "itkObjectFactory.h"
#include "itkDataObject.h"

#include "elxMacro.h"
#include "elxComponentDescriptor.h"

#include "boost/graph/graph_traits.hpp"
#include "boost/graph/directed_graph.hpp"

namespace elx {

class Blueprint : public itk::DataObject
{
public:

  elxNewMacro( Blueprint, itk::DataObject );
  
  void PrintSelf( void );

private:

  int m_Test = 0;

};

}

#endif // #define __Blueprint_h