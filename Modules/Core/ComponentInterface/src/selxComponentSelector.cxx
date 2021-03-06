/*=========================================================================
 *
 *  Copyright Leiden University Medical Center, Erasmus University Medical
 *  Center and contributors
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
//#ifndef ComponentSelector_hxx
//#define ComponentSelector_hxx

#include "selxComponentSelector.h"

namespace selx
{
ComponentSelector::ComponentSelector()
{
  std::list< itk::LightObject::Pointer > allobjects
    = itk::ObjectFactoryBase::CreateAllInstance( "ComponentBase" );

  for( std::list< itk::LightObject::Pointer >::iterator i = allobjects.begin();
    i != allobjects.end(); ++i )
  {
    ComponentBase * component
      = dynamic_cast< ComponentBase * >( i->GetPointer() );
    if (component)
    {
      this->m_PossibleComponents.push_back(component);
    }
  }
}


ComponentSelector::~ComponentSelector()
{
}

void ComponentSelector::ComponentName(const std::string name)
{
  for (const auto & component : this->m_PossibleComponents)
  {
    component->Name(name);
  }
}


void
ComponentSelector::AddCriterion( const CriterionType & criterion )
{
  this->m_PossibleComponents.remove_if([ & ]( ComponentBasePointer component ){
      return !component->MeetsCriterion( criterion );
    } );
}


void
ComponentSelector::AddAcceptingInterfaceCriteria( const InterfaceCriteriaType & interfaceCriteria )
{
  this->m_PossibleComponents.remove_if([ & ]( ComponentBasePointer component ){
      return 0 == component->CountAcceptingInterfaces( interfaceCriteria );
    } );
}


void
ComponentSelector::AddProvidingInterfaceCriteria( const InterfaceCriteriaType & interfaceCriteria )
{
  this->m_PossibleComponents.remove_if([ & ]( ComponentBasePointer component ){
      return 0 == component->CountProvidingInterfaces( interfaceCriteria );
    } );
}


// CompatibleInterfaces
unsigned int
ComponentSelector::RequireAcceptingInterfaceFrom( ComponentBasePointer other, const InterfaceCriteriaType & interfaceCriteria )
{
  this->m_PossibleComponents.remove_if([ & ]( ComponentBasePointer component ){
      auto status = component->CanAcceptConnectionFrom( other, interfaceCriteria );
      return status == InterfaceStatus::noaccepter || status == InterfaceStatus::noprovider;
    } );
  return 0;
}


unsigned int
ComponentSelector::RequireProvidingInterfaceTo( ComponentBasePointer other, const InterfaceCriteriaType & interfaceCriteria )
{
  this->m_PossibleComponents.remove_if([ & ]( ComponentBasePointer component ){
      auto status = other->CanAcceptConnectionFrom( component, interfaceCriteria );
      return status == InterfaceStatus::noaccepter || status == InterfaceStatus::noprovider;
    } );
  return 0;
}


ComponentSelector::ComponentBasePointer
ComponentSelector::GetComponent()
{
  //TODO check if Modified
  //this->UpdatePossibleComponents();

  if( this->m_PossibleComponents.size() == 1 )
  {
    return *( this->m_PossibleComponents.begin() );
  }
  else
  {
    return ITK_NULLPTR;
  }
}


unsigned int
ComponentSelector::NumberOfComponents()
{
  return this->m_PossibleComponents.size();
}

void 
ComponentSelector::PrintComponents(void)
{
  /*
  for (auto & component : this->m_PossibleComponents)
  {
    auto const & properties = component->TemplateProperties();
    for (auto const & keyvalue : properties)
    {
      std::cout << "{ """ << keyvalue.first << """: """ << keyvalue.second << """}" << std::endl;
    }
    std::cout << std::endl;
  }
  */
}
} // end namespace selx

//#endif
