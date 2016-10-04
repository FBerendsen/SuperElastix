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

#ifndef selxItkCompositeTransformComponent_h
#define selxItkCompositeTransformComponent_h

#include "selxComponentBase.h"
#include "selxInterfaces.h"
#include "itkCompositeTransform.h"
#include <string.h>
#include "selxMacro.h"
namespace selx
{
template< class InternalComputationValueType, int Dimensionality >
class ItkCompositeTransformComponent :
  public SuperElastixComponent<
  Accepting< >,
  Providing< MultiStageTransformInterface< InternalComputationValueType, Dimensionality >, itkTransformInterface<InternalComputationValueType, Dimensionality> >
  >
{
public:

  selxNewMacro(ItkCompositeTransformComponent, ComponentBase);

  //itkStaticConstMacro(Dimensionality, unsigned int, Dimensionality);

  ItkCompositeTransformComponent();
  virtual ~ItkCompositeTransformComponent();

  /**  Type of the optimizer. */
  using TransformType = typename MultiStageTransformInterface< InternalComputationValueType, Dimensionality >::TransformBaseType;

  typedef typename itk::CompositeTransform< InternalComputationValueType, Dimensionality > CompositeTransformType;

  virtual typename TransformType::Pointer GetTransformFixedInitialTransform(int stageIndex) override;
  virtual typename TransformType::Pointer GetTransformMovingInitialTransform(int stageIndex) override;
  virtual void SetResultTransform(typename TransformType::Pointer resultTransform, int stageIndex) override;
  
  virtual typename TransformType::Pointer GetItkTransform() override;

  virtual bool MeetsCriterion( const ComponentBase::CriterionType & criterion ) override;

  //static const char * GetName() { return "ItkCompositeTransform"; } ;
  static const char * GetDescription() { return "ItkCompositeTransform Component"; }

private:

  typename CompositeTransformType::Pointer m_CompositeTransform;

protected:

  // return the class name and the template arguments to uniquely identify this component.
  static inline const std::map< std::string, std::string > TemplateProperties()
  {
    return { { keys::NameOfClass, "ItkCompositeTransformComponent" }, { keys::InternalComputationValueType, PodString< InternalComputationValueType >::Get() }, { keys::Dimensionality, std::to_string( Dimensionality ) } };
  }
};
} //end namespace selx
#ifndef ITK_MANUAL_INSTANTIATION
#include "selxItkCompositeTransformComponent.hxx"
#endif
#endif // #define selxItkCompositeTransformComponent_h
