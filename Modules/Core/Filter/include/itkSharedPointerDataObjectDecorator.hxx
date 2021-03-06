/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
/*=========================================================================
 *
 *  Portions of this file are subject to the VTK Toolkit Version 3 copyright.
 *
 *  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
 *
 *  For complete copyright, license and disclaimer of warranty information
 *  please refer to the NOTICE file at the top of the ITK source tree.
 *
 *=========================================================================*/
#ifndef selxSharedPointerDataObjectDecorator_hxx
#define selxSharedPointerDataObjectDecorator_hxx

#include "itkSharedPointerDataObjectDecorator.h"

namespace itk
{
/** Constructor */
  template< typename T >
  SharedPointerDataObjectDecorator< T >
  ::SharedPointerDataObjectDecorator():m_Component()
  {}

/** Destructor */
  template< typename T >
  SharedPointerDataObjectDecorator< T >
  ::~SharedPointerDataObjectDecorator()
  {}

/** Set value */
  template< typename T >
  void
  SharedPointerDataObjectDecorator< T >
  ::Set(std::shared_ptr< T > val)
  {
    if ( m_Component != val )
    {
      m_Component = val;
      this->Modified();
    }
  }

/** PrintSelf method */
  template< typename T >
  void
  SharedPointerDataObjectDecorator< T >
  ::PrintSelf(std::ostream & os, Indent indent) const
  {
    Superclass::PrintSelf(os, indent);

    os << indent << "Component: " << typeid( m_Component ).name() << std::endl;
  }
} // end namespace itk

#endif