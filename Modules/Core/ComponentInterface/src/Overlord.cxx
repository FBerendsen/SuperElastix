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

#include "Overlord.h"

// < CM: I would rename the class. Overlord is vague and maybe even a bit too strong given its definition.
//       This is the more or less entry class of the whole package right? So maybe just call it SuperElastix?
//       If something is difficult to name it might indicate that you need to refactor. Often the class then has
//       multiple responsibilities (violates single responsibility principle). I do not have a very clear picture
//       of how to split this up, but you could consider to separate configuring and running the configuratios.
//       Those can then be separate classes which may be easier to name.

namespace selx
{
  Overlord::Overlord() : m_isConfigured(false), m_allUniqueComponents(false)
  {
    this->m_RunRegistrationComponents = ComponentsContainerType::New();
    this->m_AfterRegistrationComponents = ComponentsContainerType::New();   
  }

  bool Overlord::Configure()
  {
    if (!this->m_isConfigured)
    {
      this->m_isConfigured = true;  // < CM: Maybe better to set this at the end of the if-block.
      this->ApplyNodeConfiguration();
      std::cout << "Applying Component Settings" << std::endl;
      this->m_allUniqueComponents = this->UpdateSelectors();  // < CM: Can you keep m_allUniqueComponents local? It is a member now but not used outside this method.
      std::cout << "Based on Component Criteria unique components could " << (this->m_allUniqueComponents ? "" : "not ") << "be selected" << std::endl;

      std::cout << "Applying Connection Settings" << std::endl;
      this->ApplyConnectionConfiguration();
      this->m_allUniqueComponents = this->UpdateSelectors();
      std::cout << "By adding Connection Criteria unique components could " << (this->m_allUniqueComponents ? "" : "not ") << "be selected" << std::endl;

      //TODO: make a while loop until stable:
      // - propagate for each unique component the required interfaces to neighboring components
      // - update selectors

      //if (allUniqueComponents)
      //{
      //  isSuccess = this->ConnectComponents();
      //}

      //std::cout << "Connecting Components: " << (isSuccess? "succeeded" : "failed") << std::endl;

      //return isSuccess;
    }
    return this->m_allUniqueComponents;
  }

  bool Overlord::UpdateSelectors()
  {
    bool allUniqueComponents = true;  // < CM: Can be removed. Depends if you want to print the information for the rest of the components or not.
    const Blueprint::ComponentNamesType componentNames = m_Blueprint->GetComponentNames();
    for (auto const & name : componentNames)
    {
      ComponentSelector::NumberOfComponentsType numberOfComponents = this->m_ComponentSelectorContainer[name]->UpdatePossibleComponents();  // < CM: Use auto?

      // The current idea of the configuration setup is that the number of 
      // possible components at a node can only be reduced by adding criteria.
      // If a node has 0 possible components, the configuration is aborted (with an exception)
      // If all nodes have exactly 1 possible component, no more criteria are needed.
      //
      // Design consideration: should the exception be thrown by this->m_ComponentSelectorContainer[name]->UpdatePossibleComponents()?
      // The (failing) criteria can be printed as well.
      if (numberOfComponents > 1)
      {
        allUniqueComponents = false; // < CM: You can break the for loop here to save a bit of time: return true;
      }
      std::cout << "blueprint node " << name << " has selected " << numberOfComponents << " components" << std::endl;

    }
    return allUniqueComponents;  // < CM: return true;
  }

  void Overlord::ApplyNodeConfiguration()
  {
    // At the overlord we store all components selectors in a mapping based  
    // on the keys we find in the graph. This is a flexible solution, but is
    // fragile as well since correspondence is implicit.
    // We might consider copying the blueprint graph to a component selector
    // graph, such that all graph operations correspond.
    //
    // This could be in line with the idea of maintaining 2 graphs: 
    // 1 descriptive (= const blueprint) and 1 internal holding to realized 
    // components.
    // Manipulating the internal graph (combining component nodes into a 
    // hybrid node, duplicating sub graphs, etc) is possible then. < CM: This makes sense to me. So you have a blueprint and from that blueprint
    //                                                                   you create an instantiated graph using component selectors. You can put this
    //                                                                   procedure in a separate class I think. Next to the blueprint class there will be
    //                                                                   an instantiated blueprint class (still needs a nice name). Then the overlord can handler
    //                                                                   input to and output from this graph and its execution.
    //
    // Additional redesign consideration: the final graph should hold the 
    // realized components at each node and not the ComponentSelectors that, 
    // in turn, hold 1 (or more) component.

    Blueprint::ComponentNamesType componentNames = this->m_Blueprint->GetComponentNames();
    for (auto const & name : componentNames)
    {
      ComponentSelectorPointer currentComponentSelector = ComponentSelector::New();
      Blueprint::ParameterMapType currentProperty = this->m_Blueprint->GetComponent(name);
      currentComponentSelector->SetCriteria(currentProperty);
      // insert new element
      this->m_ComponentSelectorContainer[name] = currentComponentSelector;
    }
    return;
  }

  void Overlord::ApplyConnectionConfiguration()
  {
    Blueprint::ComponentNamesType componentNames = this->m_Blueprint->GetComponentNames();
    for (auto const & name : componentNames)
    {
      for (auto const & outgoingName : this->m_Blueprint->GetOutputNames(name))
      {
        //TODO check direction upstream/downstream input/output source/target
        Blueprint::ParameterMapType connectionProperties = this->m_Blueprint->GetConnection(name, outgoingName);
        if (connectionProperties.count("NameOfInterface") > 0)
        {
          ComponentBase::CriteriaType additionalSourceCriteria;
          additionalSourceCriteria.insert(ComponentBase::CriterionType("HasProvidingInterface", connectionProperties["NameOfInterface"]));  // < CM: Can you use a type safe way of requesting HasProvidingInterface instead of using a string?

          ComponentBase::CriteriaType additionalTargetCriteria;
          additionalTargetCriteria.insert(ComponentBase::CriterionType("HasAcceptingInterface", connectionProperties["NameOfInterface"]));  // < CM: Can you use a type safe way of requesting HasAcceptingInterface instead of using a string?

          this->m_ComponentSelectorContainer[name]->AddCriteria(additionalSourceCriteria);
          this->m_ComponentSelectorContainer[outgoingName]->AddCriteria(additionalTargetCriteria);
        }
      }
    }

    return;
  }

  bool Overlord::ConnectComponents()  // < CM: This method is not called (at least not in the unit test and not from this class).
                                      //       It is called in the SuperElastixFilter, but would it not make more sense to call it from
                                      //       Configure?
  {
    bool isAllSuccess = true;

    Blueprint::ComponentNamesType componentNames = this->m_Blueprint->GetComponentNames();
    for (auto const & name : componentNames)
    {
      for (auto const & outgoingName : this->m_Blueprint->GetOutputNames(name))
      {
        //TODO check direction upstream/downstream input/output source/target
        //TODO GetComponent returns NULL if possible components !=1 we can check for that, but Overlord::UpdateSelectors() does something similar.
        ComponentBase::Pointer sourceComponent = this->m_ComponentSelectorContainer[name]->GetComponent();
        ComponentBase::Pointer targetComponent = this->m_ComponentSelectorContainer[outgoingName]->GetComponent();

        Blueprint::ParameterMapType connectionProperties = this->m_Blueprint->GetConnection(name, outgoingName);
        int numberOfConnections = 0;
        if (connectionProperties.count("NameOfInterface") > 0)
        {
          // connect only via interfaces provided by user configuration
          for (auto const & interfaceName : connectionProperties["NameOfInterface"])
          {
            numberOfConnections += (targetComponent->AcceptConnectionFrom(interfaceName.c_str(), sourceComponent) == ComponentBase::interfaceStatus::success ? 1: 0);
          }
        }
        else
        {
          // connect via all possible interfaces
          numberOfConnections = targetComponent->AcceptConnectionFrom(sourceComponent);
        }

        if (numberOfConnections == 0)
        {
          isAllSuccess = false;
          std::cout << "Warning: a connection was specified, but no compatible interfaces were found.";
        }
      }
    }
    return isAllSuccess;
  }

  Overlord::SourceInterfaceMapType Overlord::GetSourceInterfaces()
  {
    /** Scans all Components to find those with Sourcing capability and store them in SourceComponents list */
    
    const CriteriaType sourceCriteria = { { "HasProvidingInterface", { "SourceInterface" } } };
    SourceInterfaceMapType sourceInterfaceMap;
    // TODO redesign ComponentBase class to accept a single criterion instead of a criteria mapping.
    for (const auto & componentSelector : (this->m_ComponentSelectorContainer))
    {
      ComponentBase::Pointer component = componentSelector.second->GetComponent();
      if (component->MeetsCriteria(sourceCriteria)) // TODO MeetsCriterion
      {
        SourceInterface* provingSourceInterface = dynamic_cast<SourceInterface*> (&(*component)); // < CM: Does ITK not have a get() method for smart pointers?
        if (provingSourceInterface == nullptr) // is actually a double-check for sanity: based on criterion cast should be successful
        {
          itkExceptionMacro("dynamic_cast<SourceInterface*> fails, but based on component criterion it shouldn't")
        }
        sourceInterfaceMap[componentSelector.first]=provingSourceInterface;
        
      }
    }
    return sourceInterfaceMap;
  }

  Overlord::SinkInterfaceMapType Overlord::GetSinkInterfaces()
  {
    /** Scans all Components to find those with Sinking capability and store them in SinkComponents list */
    // BIG TODO SinkInterface2 -> SinkInterface
    // TODO redesign ComponentBase class to accept a single criterion instead of a criteria mapping.
    const CriteriaType sinkCriteria = { { "HasProvidingInterface", { "SinkInterface" } } };
    
    SinkInterfaceMapType sinkInterfaceMap;

    for (auto const & componentSelector : (this->m_ComponentSelectorContainer))
    {
      ComponentBase::Pointer component = componentSelector.second->GetComponent();
      if (component->MeetsCriteria(sinkCriteria))  // TODO MeetsCriterion
      {
        SinkInterface* provingSinkInterface = dynamic_cast<SinkInterface*> (&(*component)); // < CM: Does ITK not have a get() method for smart pointers?
        if (provingSinkInterface == nullptr) // is actually a double-check for sanity: based on criterion cast should be successful
        {
          itkExceptionMacro("dynamic_cast<SinkInterface*> fails, but based on component criterion it shouldn't")
        }
        sinkInterfaceMap[componentSelector.first] = provingSinkInterface;
      }
    }
    return sinkInterfaceMap;
  }

  bool Overlord::FindRunRegistration()
  {
    /** Scans all Components to find those with Sourcing capability and store them in SourceComponents list */
    const CriterionType runRegistrationCriterion = CriterionType("HasProvidingInterface", { "RunRegistrationInterface" });

    // TODO redesign ComponentBase class to accept a single criterion instead of a criteria mapping.
    CriteriaType runRegistrationCriteria;
    runRegistrationCriteria.insert(runRegistrationCriterion);

    for (auto const & componentSelector : (this->m_ComponentSelectorContainer))  // < CM: parentheses around this->m_ComponentSelectorContainer are probably not needed.
    {
      ComponentBase::Pointer component = componentSelector.second->GetComponent();
      if (component->MeetsCriteria(runRegistrationCriteria)) // TODO MeetsCriterion
      {
        this->m_RunRegistrationComponents->push_back(component);
      }
    }

    return true; // < CM: Is it useful to always return true?
  }

  bool Overlord::FindAfterRegistration()
  {
    /** Scans all Components to find those with Sourcing capability and store them in SourceComponents list */ // < CM: Do you really mean 'Sourcing' or 'Sink'?
    const CriterionType afterRegistrationCriterion = CriterionType("HasProvidingInterface", { "AfterRegistrationInterface" });

    // TODO redesign ComponentBase class to accept a single criterion instead of a criteria mapping.
    CriteriaType afterRegistrationCriteria;
    afterRegistrationCriteria.insert(afterRegistrationCriterion);

    for (auto const & componentSelector : (this->m_ComponentSelectorContainer))  // < CM: parentheses around this->m_ComponentSelectorContainer are probably not needed.
    {
      ComponentBase::Pointer component = componentSelector.second->GetComponent();
      if (component->MeetsCriteria(afterRegistrationCriteria)) // TODO MeetsCriterion
      {
        this->m_AfterRegistrationComponents->push_back(component);
      }
    }

    return true; // < CM: Is it useful to always return true?
  }

  

  bool Overlord::RunRegistrations()
  {

    for (auto const & runRegistrationComponent : *(this->m_RunRegistrationComponents)) // auto&& preferred?  < CM: (this->m_RunRegistrationComponents) -> this->m_RunRegistrationComponents
    {
      RunRegistrationInterface* providingRunRegistrationInterface = dynamic_cast<RunRegistrationInterface*> (&(*runRegistrationComponent));  // < CM: Does ITK not have a get() method for smart pointers?
      if (providingRunRegistrationInterface == nullptr) // is actually a double-check for sanity: based on criterion cast should be successful
      {
        itkExceptionMacro("dynamic_cast<RunRegistrationInterface*> fails, but based on component criterion it shouldn't")
      }
      // For testing purposes, all Sources are connected to an ImageWriter
      providingRunRegistrationInterface->RunRegistration();
    }
    return true;  // < CM: Is it useful to always return true?
  }

  bool Overlord::AfterRegistrations()
  {

    for (auto const & afterRegistrationComponent : *(this->m_AfterRegistrationComponents)) // auto&& preferred?
    {
      AfterRegistrationInterface* providingAfterRegistrationInterface = dynamic_cast<AfterRegistrationInterface*> (&(*afterRegistrationComponent));  // < CM: Does ITK not have a get() method for smart pointers?
      if (providingAfterRegistrationInterface == nullptr) // is actually a double-check for sanity: based on criterion cast should be successful
      {
        itkExceptionMacro("dynamic_cast<AfterRegistrationInterface*> fails, but based on component criterion it shouldn't")
      }
      // For testing purposes, all Sources are connected to an ImageWriter
      providingAfterRegistrationInterface->AfterRegistration();
    }
    return true;  // < CM: Is it useful to always return true?
  }
  bool Overlord::Execute()
  {
    
    // TODO make one "update button" for the overlord
    this->FindRunRegistration();
    this->FindAfterRegistration();

    // RunRegistrations is a simple execution model
    // E.g.if the components are true itk Process Object, the don't need an 'Update' call. 
    // The container of RunRegistrationsInterfaces will therefore be empty, since they will not be added if they don't expose this interface.
    // If components need RunIterations() or RunResolution() we can explicitly 'Update' them here and control the flow.
    // TODO: see if signals-and-slots paradigm is appropriate here.

    this->RunRegistrations();
    this->AfterRegistrations();
    //update all writers...
    
    return true;  // < CM: Is it useful to always return true?
  }

  AnyFileReader::Pointer Overlord::GetInputFileReader(const Overlord::ComponentNameType& inputName)
  {
    SourceInterfaceMapType sources = this->GetSourceInterfaces();
    if (sources.count(inputName) != 1)
    {
      itkExceptionMacro(<< "No Source component found by name:" << inputName);
    }

    //return sources[inputName]->GetInputFileReader();
    return nullptr;  // < CM: ??
  }
  
  AnyFileWriter::Pointer Overlord::GetOutputFileWriter(const Overlord::ComponentNameType& outputName)
  {
    SinkInterfaceMapType sinks = this->GetSinkInterfaces();
    if (sinks.count(outputName) != 1)
    {
      itkExceptionMacro(<< "No Sink component found by name:" << outputName);
    }

    return sinks[outputName]->GetOutputFileWriter();
  }

  SinkInterface::DataObjectPointer Overlord::GetInitializedOutput(const Overlord::ComponentNameType& outputName)
  {
    SinkInterfaceMapType sinks = this->GetSinkInterfaces();
    if (sinks.count(outputName) != 1)
    {
      itkExceptionMacro(<< "No Sink component found by name:" << outputName);
    }

    return sinks[outputName]->GetInitializedOutput();
  }

} // end namespace selx

