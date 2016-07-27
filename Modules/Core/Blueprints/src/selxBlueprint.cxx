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

#ifndef Blueprint_cxx  // < CM: You don't need define guards for CXX files. Only for header files to prevent multiple inclusions.
#define Blueprint_cxx

#include "boost/graph/graphviz.hpp"

#include "selxBlueprint.h"  // < CM: Always include your own header file first. This prevents that you header file may depend
                            //       on including other files first.

namespace selx {

  // Declared outside of the class body, so it is a free function  < CM: States the obvious. Better to comment why you make it a
  //                                                                     free function (which is BTW the right choice, but you can
  //                                                                     wonder if ParameterMapType should be a member type of Blueprint).
  std::ostream& operator<<(std::ostream& out, const Blueprint::ParameterMapType& val) {
    for (auto const & mapPair : val)  // < CM: mapPair is not really descriptive. It's a parameter/value type right?
    {
      out << mapPair.first << " : [ ";
      for (auto const & value : mapPair.second)
      {
        out << value << " ";
      }
      out << "]\\n";  // < CM: do you really need the double \ here?
    }
    return out;
  }

  // < CM: I would move all writer related stuff just above the write method.
  // < CM: The writer helper classes look like functors. Can you replace them with lamda's? That would save you a lot of code.

  template <class NameType, class ParameterMapType>
  class vertex_label_writer {  // < CM: Is there a reason to use lower case / underscore style
                               //       here instead of UpperCamelCase which you use for the test of the classes?
                               //       Same holds for edge_label_writer and make_edge_label_writer.

  public:
    vertex_label_writer(NameType _name, ParameterMapType _parameterMap) : name(_name), parameterMap(_parameterMap) {}
      // < CM: I would prevent using prefix underscore. Better postfix you members with an underscore or don't prefix/postfix.
      //       The C++ standard can perfectly handler:
      //         vertex_label_writer(NameType name, ParameterMapType parameterMap) : name(name), parameterMap(parameterMap) {}

    template <class VertexOrEdge>
    void operator()(std::ostream& out, const VertexOrEdge& v) const {
      out << "[label=\"" << name[v] << "\n" << parameterMap[v] << "\"]";
    }
  private:
    NameType name;
    ParameterMapType parameterMap;
  };

  template <class NameType, class ParameterMapType>
  inline vertex_label_writer<NameType, ParameterMapType>
    make_vertex_label_writer(NameType n, ParameterMapType p) {
    return vertex_label_writer<NameType, ParameterMapType>(n, p);
  }

  template <class ParameterMapType>
  class edge_label_writer {  // < CM: See vertex_label_writer comments.

  public:
    edge_label_writer(ParameterMapType _parameterMap) : parameterMap(_parameterMap) {}
    template <class VertexOrEdge>
    void operator()(std::ostream& out, const VertexOrEdge& v) const {
      out << "[label=\"" << parameterMap[v] << "\"]";
    }
  private:
    ParameterMapType parameterMap;
  };

  template <class ParameterMapType>
  inline edge_label_writer<ParameterMapType>
    make_edge_label_writer(ParameterMapType p) {
    return edge_label_writer<ParameterMapType>(p);
  }

bool
Blueprint
::AddComponent(ComponentNameType name)
{
  this->Modified();

  // Returns true is addition was successful 
  return this->m_Graph.insert_vertex(name, { name, { {} } }).second;
}

bool
Blueprint
::AddComponent(ComponentNameType name, ParameterMapType parameterMap)  // < CM: parameterMap -> parameters
{
  this->Modified();

  // Returns true is addition was successful 
  return this->m_Graph.insert_vertex(name, { name, parameterMap }).second;
}

Blueprint::ParameterMapType
Blueprint
::GetComponent(ComponentNameType name) const
{
  if (!this->ComponentExists(name))
  {
    itkExceptionMacro("Blueprint does not contain component " << name);
    // < CM: It may be good to think about preconditions versus exceptions. If you make the existence 
    //       a precondition you can do an assert here. Same holds for other exceptions in this file.
    //       It is just a programming error of the user when he/she requests an not existing component
    //       or not? If so, a precondition makes more sense. Exceptions are more appropriate for errors
    //       that are exceptional (which you cannot really influence, such as resource/file handling).
  }
  this->Modified();
  return this->m_Graph[name].parameterMap;
}

void
Blueprint
::SetComponent(ComponentNameType name, ParameterMapType parameterMap)
{
  this->Modified();
  this->m_Graph[name].parameterMap = parameterMap;
}

// TODO: See explanation in selxBlueprint.h
// void
// Blueprint
// ::DeleteComponent( const ComponentIndexType index )
// {
//   this->Modified();
//
//   clear_vertex( index, this->m_Graph );
//   remove_vertex( index, this->m_Graph );
// }

Blueprint::ComponentNamesType Blueprint::GetComponentNames(void) const{
  ComponentNamesType container;  // < CM: container -> names.
  for (auto it = boost::vertices(this->m_Graph.graph()).first; it != boost::vertices(this->m_Graph.graph()).second; ++it){
    container.push_back(this->m_Graph.graph()[*it].name);
  }
  return container;
}

bool
Blueprint
::AddConnection(ComponentNameType upstream, ComponentNameType downstream)
{
  this->Modified();

  if( this->ConnectionExists( upstream, downstream) ) {  // < CM: Maybe a precondition as well?
    return false;
  }
  
  // Adds directed connection from upstream component to downstream component
  return boost::add_edge_by_label(upstream, downstream, this->m_Graph).second;
}

bool
Blueprint
::AddConnection(ComponentNameType upstream, ComponentNameType downstream, ParameterMapType parameterMap)
{
  this->Modified();

  if( !this->ConnectionExists( upstream, downstream ) ) {
    // TODO check if vertices exist
    boost::add_edge_by_label(upstream, downstream,  { parameterMap }, this->m_Graph);
    return true;
  }

  // If the connection does exist don't do anything because previous settings 
  // will be overwritten. If the user does want to overwrite current settings, 
  // she should use SetConnection() instead where the intent is explicit.  
  return false;
}

// < CM: Possible shorter form (assuming you do not need Modified and benefitting from lazy evaluation):
//       
// bool Blueprint::AddConnection(ComponentNameType upstream, ComponentNameType downstream, ParameterMapType parameterMap) {
//   return !this->ConnectionExists(upstream, downstream) &&
//          boost::add_edge_by_label(upstream, downstream, {parameterMap}, this->m_Graph);
// }

Blueprint::ParameterMapType
Blueprint
::GetConnection( ComponentNameType upstream, ComponentNameType downstream ) const
{
  this->Modified();

  return this->m_Graph[ this->GetConnectionIndex( upstream, downstream ) ].parameterMap;
}

bool
Blueprint
::SetConnection(ComponentNameType upstream, ComponentNameType downstream, ParameterMapType parameterMap)
{
  this->Modified();

  if( !this->ConnectionExists( upstream, downstream ) ) {
    return this->AddConnection( upstream, downstream, parameterMap );
  } else {
    this->m_Graph[ this->GetConnectionIndex( upstream, downstream ) ].parameterMap = parameterMap;
    return true;
  }
}

bool
Blueprint
::DeleteConnection(ComponentNameType upstream, ComponentNameType downstream)
{
  this->Modified();

  if( this->ConnectionExists( upstream, downstream ) ) {
    boost::remove_edge_by_label(upstream, downstream, this->m_Graph);
  }
  
  return !this->ConnectionExists( upstream, downstream );
}

bool
Blueprint
::ComponentExists(ComponentNameType componentName) const
{
  const GraphType::vertex_descriptor descriptor = this->m_Graph.vertex(componentName);
  return descriptor != boost::graph_traits<GraphType>::null_vertex();
}

bool
Blueprint
::ConnectionExists( ComponentNameType upstream, ComponentNameType downstream ) const
{
  if (!this->ComponentExists(upstream))
  {
    itkExceptionMacro("Blueprint does not contain component " << upstream);
    // < CM: Precondition? Or just return false. If the component is not there there also cannot be a connection right?
  }
  if (!this->ComponentExists(downstream))
  {
    itkExceptionMacro("Blueprint does not contain component " << downstream);
    // < CM: Precondition? Or just return false. If the component is not there there also cannot be a connection right?
  }

  return boost::edge_by_label( upstream, downstream, this->m_Graph).second;
}

Blueprint::ComponentNamesType
Blueprint
::GetOutputNames(const ComponentNameType name) const{
  ComponentNamesType container;  // < CM: container -> names
  OutputIteratorPairType outputIteratorPair = boost::out_edges(this->m_Graph.vertex(name), this->m_Graph);
  for (auto it = outputIteratorPair.first; it != outputIteratorPair.second; ++it){
    container.push_back(this->m_Graph.graph()[it->m_target].name);
  }

  return container;
}


Blueprint::ConnectionIndexType
Blueprint
::GetConnectionIndex( ComponentNameType upstream, ComponentNameType downstream ) const
{
  // This function is part of the internal API and should fail hard if we use it incorrectly
  if( !this->ConnectionExists( upstream, downstream ) ) {
    itkExceptionMacro( "Blueprint does not contain connection from component " << upstream << " to " << downstream );
  }
  
  return boost::edge_by_label(upstream, downstream, this->m_Graph).first;
}

void 
Blueprint
::WriteBlueprint( const std::string filename ) 
{
  std::ofstream dotfile( filename.c_str() );
  boost::write_graphviz(dotfile, this->m_Graph, 
    make_vertex_label_writer(boost::get(&ComponentPropertyType::name, this->m_Graph), 
                             boost::get(&ComponentPropertyType::parameterMap, this->m_Graph)), 
    make_edge_label_writer(boost::get(&ConnectionPropertyType::parameterMap, this->m_Graph)));
}

} // namespace selx 

#endif // Blueprint_cxx
