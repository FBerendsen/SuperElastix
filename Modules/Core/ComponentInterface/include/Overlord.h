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

#ifndef Overlord_h
#define Overlord_h

#include "itkLightProcessObject.h"
#include "itkObjectFactory.h"
#include "itkVectorContainer.h"

#include "selxAnyFileReader.h"
#include "selxAnyFileWriter.h"

#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>

#include "selxMacro.h"
#include "selxBlueprint.h"
#include "ComponentFactory.h"
#include "Interfaces.h"
namespace selx
{
  // < CM: This class can really benefit from some comments.

  class Overlord : public itk::LightProcessObject  // < CM: Do you really want to inherit from a ITK base class here.
                                                   //       It makes you stick with the ITK factory kind of object creation
                                                   //       which is in my opinion old fashioned and limiting you too much,
                                                   //       especially when you want to use non-default constructors.
  {
  public:

    selxNewMacro(Overlord, itk::LightProcessObject);

    // < CM: Minimize typedefs. It really obfuscates the interface of the class. If you really need
    //       typedefs for the interface put them here. Otherwise put them in the cxx file.
    typedef ComponentBase::CriteriaType CriteriaType;
    typedef ComponentBase::CriterionType CriterionType;
    typedef ComponentBase::ParameterValueType ParameterValueType;

    typedef ComponentBase       ComponentType;
    typedef ComponentSelector::Pointer ComponentSelectorPointer;
    typedef Blueprint BlueprintType;  // < CM: What is the advantage of using BlueprintType over Blueprint directly?
    typedef BlueprintType::ComponentNameType ComponentNameType;
    typedef std::map< ComponentNameType, ComponentSelectorPointer> ComponentSelectorContainerType;
    typedef ComponentSelectorContainerType::iterator ComponentSelectorIteratorType;

    typedef AnyFileReader AnyFileReaderType;  // < CM: What is the advantage of using AnyFileReaderType over AnyFileReader directly?
    typedef AnyFileWriter AnyFileWriterType;

    typedef std::map <
      std::string, SourceInterface* > SourceInterfaceMapType;

    typedef std::map <
      std::string, SinkInterface* > SinkInterfaceMapType;

   SourceInterfaceMapType GetSourceInterfaces();
   SinkInterfaceMapType GetSinkInterfaces();

   SinkInterface::DataObjectPointer GetInitializedOutput(const Overlord::ComponentNameType&);

    typedef itk::VectorContainer <
      unsigned int, ComponentType::Pointer > ComponentsContainerType;

    //TODO make const correct
    //itkSetConstObjectMacro(Blueprint, Blueprint);
    itkSetObjectMacro(Blueprint, Blueprint);
    bool Configure();
    bool Execute();

    AnyFileReaderType::Pointer GetInputFileReader(const ComponentNameType&);
    AnyFileWriterType::Pointer GetOutputFileWriter(const ComponentNameType&);

  protected:
    
    Overlord();
    virtual ~Overlord() {};
  public: // temporarily from private to public during refactoring SuperElastixFilter.
    void ApplyNodeConfiguration();
    void ApplyConnectionConfiguration();
    bool UpdateSelectors();
    bool ConnectComponents();
    bool FindAfterRegistration();
    bool FindRunRegistration();
  private:
    bool ConnectSources();
    bool ConnectSinks();
    bool RunRegistrations();
    bool AfterRegistrations();

    //TODO make const correct
    //Blueprint::ConstPointer m_Blueprint;
    BlueprintType::Pointer m_Blueprint;
    ComponentSelectorContainerType m_ComponentSelectorContainer;
    bool m_isConfigured;
    bool m_allUniqueComponents;
    
    ComponentsContainerType::Pointer m_RunRegistrationComponents;
    ComponentsContainerType::Pointer m_AfterRegistrationComponents;
  };

} // end namespace selx


#endif // Overlord_h

