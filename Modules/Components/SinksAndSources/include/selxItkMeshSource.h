/*=========================================================================
 *
 *  Copyright Leiden University Medical Center, Erasmus University Medical
 *  Center and contributors
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef selxItkMeshSource_h
#define selxItkMeshSource_h

#include "selxSuperElastixComponent.h"
#include "selxInterfaces.h"
#include <string.h>
#include "selxMacro.h"
#include "itkMeshFileReader.h"
#include "selxAnyFileReader.h"
#include "selxFileReaderDecorator.h"

namespace selx
{
template< int Dimensionality, class TPixel >
class ItkMeshSourceComponent :
  public SuperElastixComponent<
  Accepting< >,
  Providing< SourceInterface, itkMeshInterface< Dimensionality, TPixel > >
  >
{
public:

  selxNewMacro( ItkMeshSourceComponent, ComponentBase );

  ItkMeshSourceComponent();
  virtual ~ItkMeshSourceComponent();

  typedef itk::Mesh< TPixel, Dimensionality >         ItkMeshType;
  typedef typename itk::MeshFileReader< ItkMeshType > ItkMeshReaderType;
  typedef FileReaderDecorator< ItkMeshReaderType >    DecoratedReaderType;

  virtual typename ItkMeshType::Pointer GetItkMesh() override;

  virtual void SetMiniPipelineInput( itk::DataObject::Pointer ) override;
  virtual AnyFileReader::Pointer GetInputFileReader( void ) override;

  virtual bool MeetsCriterion( const ComponentBase::CriterionType & criterion ) override;

  static const char * GetDescription() { return "ItkMeshSource Component"; }

private:

  typename ItkMeshType::Pointer m_Mesh;

protected:

  // return the class name and the template arguments to uniquely identify this component.
  static inline const std::map< std::string, std::string > TemplateProperties()
  {
    return { { keys::NameOfClass, "ItkMeshSourceComponent" }, { keys::PixelType, PodString< TPixel >::Get() }, { keys::Dimensionality, std::to_string( Dimensionality ) } };
  }
};
} //end namespace selx
#ifndef ITK_MANUAL_INSTANTIATION
#include "selxItkMeshSource.hxx"
#endif
#endif // #define selxItkMeshSource_h
