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

#ifndef Blueprint_h
#define Blueprint_h

#include "boost/graph/graph_traits.hpp"
#include "boost/graph/directed_graph.hpp"
#include "boost/graph/labeled_graph.hpp"  // < CM: If you can I would try to hide that you use boost. It is basically an
                                          //       implementation detail. Architecture wise it would be better to hide it
                                          //       so in theory you can swap out the boost graph for another graph implementation
                                          //       if desired. Encapsulation will be better; you will not drag in the boost incldues everywhere
                                          //       where you include selxBlueprint.h. Your .h file will be a lot cleaner as well. You can remove
                                          //       the boost includes and boost related typedefs to you cxx file. You may have to use the PIMPL idiom
                                          //       to store the m_Graph member. GetConnectionIndex can be rewritten as a non-member function
                                          //       (cxx file local function) or be part of the PIMPL class.

#include "itkObjectFactory.h"
#include "itkDataObject.h"

#include "selxMacro.h"

namespace selx {

class Blueprint : public itk::DataObject  // < CM: Is there a reason to make the SuperElastix classes derive from
                                          //       ITK DataObject? If you do not really need it, I would remove it.
                                          //
                                          // < CM: Shouldn't it be 'BluePrint'?
{
public:

  selxNewMacro( Blueprint, itk::DataObject );

  typedef std::string                                                ParameterKeyType;
  typedef std::vector< std::string >                                 ParameterValueType;
  typedef std::map< ParameterKeyType, ParameterValueType >           ParameterMapType;

  typedef std::string                                                ComponentNameType;
  
  // Component parameter map that sits on a node in the graph 
  // and holds component configuration settings
  struct ComponentPropertyType { 
    ComponentNameType name;
    ParameterMapType parameterMap;  // < CM: parameterMap -> parameters?
  };

  // Component parameter map that sits on an edge in the graph  < CM : Component -> Connection?
  // and holds component connection configuration settings
  struct ConnectionPropertyType { 
    ParameterMapType parameterMap;  // < CM: parameterMap -> parameters?
  };
  
  typedef boost::labeled_graph < boost::adjacency_list <                       
                                                        boost::vecS,
                                                        boost::vecS,
                                                        boost::directedS,
                                                        ComponentPropertyType,
                                                        ConnectionPropertyType 
                                                        >,
                                 ComponentNameType >                  GraphType;
  
  typedef std::vector<ComponentNameType>                              ComponentNamesType;

  typedef boost::graph_traits< GraphType >::vertex_descriptor         ComponentIndexType;
  typedef boost::graph_traits< GraphType >::vertex_iterator           ComponentIteratorType;
  typedef std::pair< ComponentIteratorType, ComponentIteratorType >   ComponentIteratorPairType;

  typedef boost::graph_traits< GraphType >::edge_descriptor           ConnectionIndexType;
  typedef boost::graph_traits< GraphType >::edge_iterator             ConnectionIteratorType;
  typedef std::pair< ConnectionIteratorType, ConnectionIteratorType > ConnectionIteratorPairType;

  typedef boost::graph_traits< GraphType >::in_edge_iterator          InputIteratorType;
  typedef std::pair< InputIteratorType, InputIteratorType >           InputIteratorPairType;

  typedef boost::graph_traits< GraphType >::out_edge_iterator         OutputIteratorType;
  typedef std::pair< OutputIteratorType, OutputIteratorType >         OutputIteratorPairType;

  // Interface for managing components
  bool AddComponent(ComponentNameType name);
  bool AddComponent(ComponentNameType name, ParameterMapType parameterMap);
  ParameterMapType GetComponent(ComponentNameType name) const;
  void SetComponent(ComponentNameType, ParameterMapType parameterMap);  // < CM: Do you really need a Set method? Is Update maybe a better name?

  // TODO: Let user delete component. Before we do this, we need a proper way of
  // checking that a vertex exist. Otherwise a call to GetComponent() on 
  // a deleted vertex will result in segfault. It is not really an issue
  // _before_ release since typically we (the developers) will use blueprint 
  // interface procedurally.
  // void DeleteComponent( ComponentIndexType );  // < CM: Do you expect users to modify the blueprint after creation?
  //                                                       If you assume they cannot, the interface would be simpler you have less trouble
  //                                                       implementing the updates/removals.

  // Returns a vector of the all Component names in the graph.
  // TODO: should this be an iterator over the names?  < CM: I think an iterator is a bit heavy here. Users can just run std::algorithms on
  //                                                         the returned vector if they want to. Only when you can get a real performance
  //                                                         improvement by creating iterators that directly point to the in memory location
  //                                                         of the names in the boost graph you can consider using iterators. Otherwise this
  //                                                         method is quite efficient given the compiler uses RVO.
  ComponentNamesType GetComponentNames(void) const;

  // Interface for managing connections between components in which we 
  // deliberately avoid using connection indexes, but instead force
  // the user to think in terms of components (which is conceptually simpler)
  bool AddConnection(ComponentNameType upstream, ComponentNameType downstream);
  bool AddConnection(ComponentNameType upstream, ComponentNameType downstream, ParameterMapType parameterMap);
  ParameterMapType GetConnection(ComponentNameType upstream, ComponentNameType downstream) const;
  bool SetConnection(ComponentNameType upstream, ComponentNameType downstream, ParameterMapType parameterMap);
  bool DeleteConnection(ComponentNameType upstream, ComponentNameType downstream);

  bool ComponentExists(ComponentNameType componentName) const;
  bool ConnectionExists(ComponentNameType upstream, ComponentNameType downstream) const;

  // Returns a vector of the Component names at the outgoing direction
  // TODO: should this be an iterator over the names?
  ComponentNamesType GetOutputNames(const ComponentNameType name) const;

  void WriteBlueprint(const std::string filename);  // < CM: WriteBlueprint -> Write.

private:
 
  ConnectionIndexType GetConnectionIndex( ComponentNameType upsteam, ComponentNameType downstream ) const;

  GraphType m_Graph;

};

}

#endif // #define Blueprint_h