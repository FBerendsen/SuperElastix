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

#include "selxBlueprint.h"

#include "gtest/gtest.h"

// < CM: Following methods not tested:
//       - GetComponentNames
//       - GetOutputNames
//       - ComponentExists

using namespace selx;

class BlueprintTest : public ::testing::Test {
public:

  typedef Blueprint::Pointer                BlueprintPointerType;
  typedef Blueprint::ParameterMapType       ParameterMapType;
  typedef Blueprint::ParameterValueType     ParameterValueType;

  virtual void SetUp() {
    parameterMap["NameOfClass"] = ParameterValueType(1, "TestClassName");

    // < CM: As you create a blue print in every test, you can also do it here:
    //       this->blueprint_ = Blueprint::New();
  }

  ParameterMapType parameterMap;
};

// < CM: I would expect bool AddComponent(ComponentNameType name) to be tested here as well.
TEST_F( BlueprintTest, AddComponent )
{
  BlueprintPointerType blueprint;
  EXPECT_NO_THROW( blueprint = Blueprint::New() );

  bool success0;
  EXPECT_NO_THROW( success0 = blueprint->AddComponent( "MyComponentName" ) );

  bool success1;
  EXPECT_NO_THROW( success1 = blueprint->AddComponent("MyComponentName", parameterMap));
  
  // < CM: You should also check the effect of the test:
  //       - What should the return value be.
  //       - Check that something has really been added and that it corresponds to what you expect.
  //       - Check all control flows of the code (so check failures as well).
}

TEST_F( BlueprintTest, GetComponent ) 
{
  BlueprintPointerType blueprint = Blueprint::New();
  blueprint->AddComponent("MyComponentName", parameterMap);

  ParameterMapType parameterMapTest;
  EXPECT_NO_THROW( parameterMapTest = blueprint->GetComponent( "MyComponentName" ) );
  EXPECT_EQ( parameterMap["NameOfClass"], parameterMapTest["NameOfClass"] );
}

TEST_F( BlueprintTest, SetComponent ) 
{
  BlueprintPointerType blueprint = Blueprint::New();
  blueprint->AddComponent( "MyComponentName", parameterMap);

  ParameterMapType parameterMapTest;
  EXPECT_NO_THROW(blueprint->SetComponent("MyComponentName", parameterMap));
  EXPECT_NO_THROW(parameterMapTest = blueprint->GetComponent("MyComponentName"));
  EXPECT_EQ( parameterMap["NameOfClass"], parameterMapTest["NameOfClass"] );

  // < CM: As you add and set the component using the same parameter map, the test will also succeed
  //       with an empty implementation of SetComponent.
}

// TODO: The final line segfaults because GetComponent does not check that the index actually
// actually exist. See explanation in selxBlueprint.h
// < CM: This may signal inconsistencies or lack of preconditions. Think well about which cases you
//       really want to tackle in your code and which you assume to be preconditions.
// TEST_F( BlueprintTest, DeleteComponent ) 
// {
//   BlueprintPointerType blueprint = Blueprint::New();
//   blueprint->AddComponent( parameterMap );
//
//   ParameterMapType parameterMapTest;
//   EXPECT_NO_THROW( parameterMapTest = blueprint->GetComponent( index ) );
//   EXPECT_EQ( parameterMap["NameOfClass"], parameterMapTest["NameOfClass"] );
//
//   EXPECT_NO_THROW( blueprint->DeleteComponent( index ) );
//   EXPECT_ANY_THROW( parameterMapTest = blueprint->GetComponent( index ) );
// }

TEST_F( BlueprintTest, AddConnection )
{
  BlueprintPointerType blueprint = Blueprint::New();

  blueprint->AddComponent( "Component0" );
  blueprint->AddComponent( "Component1" );
  blueprint->AddComponent( "Component2" );

  // Connection should not exist
  EXPECT_FALSE(blueprint->ConnectionExists("Component0", "Component1"));

  // Connection should be added
  EXPECT_TRUE(blueprint->AddConnection("Component0", "Component1"));

  // Connection should exist
  EXPECT_TRUE(blueprint->ConnectionExists("Component0", "Component1"));

  // Another connection between same components should not be added
  // (user should use SetComponent() instead)
  EXPECT_FALSE(blueprint->AddConnection("Component0", "Component1"));

  // Connection should be empty
  ParameterMapType parameterMapTest0;
  EXPECT_NO_THROW(parameterMapTest0 = blueprint->GetConnection("Component0", "Component1"));
  EXPECT_EQ( 0u, parameterMapTest0.size() );  // < CM: Or use EXPECT_TRUE(parameterMapTest0.empty())

  // Connection with properties should be added. Testing if properties was 
  // properly set requires a call to GetConnection() which is the responsibility
  // of the next test. // < CM: I think I would test it here anyway and just do a bit more extensive
  //                            testing of GetConnection in the next test.
  ParameterMapType parameterMapTest1;
  EXPECT_TRUE(blueprint->AddConnection("Component1", "Component2", parameterMap));
}

TEST_F( BlueprintTest, GetConnection )
{
  BlueprintPointerType blueprint = Blueprint::New();

  blueprint->AddComponent("Component0");
  blueprint->AddComponent("Component1");

  ParameterMapType parameterMapTest0;
  EXPECT_TRUE(blueprint->AddConnection("Component0", "Component1", parameterMap));
  EXPECT_NO_THROW(parameterMapTest0 = blueprint->GetConnection("Component0", "Component1"));
  EXPECT_EQ( parameterMap["NameOfClass"], parameterMapTest0["NameOfClass"] );
}

TEST_F( BlueprintTest, SetConnection )
{
  BlueprintPointerType blueprint = Blueprint::New();

  blueprint->AddComponent("Component0");
  blueprint->AddComponent("Component1");
  blueprint->AddConnection("Component0", "Component1", parameterMap);

  ParameterMapType parameterMapTest0;
  parameterMapTest0 = blueprint->GetConnection("Component0", "Component1");
  EXPECT_EQ( parameterMap["NameOfClass"], parameterMapTest0["NameOfClass"] );

  ParameterMapType parameterMapTest1;
  parameterMapTest1["NameOfClass"] = ParameterValueType(1, "OtherName");
  EXPECT_TRUE(blueprint->SetConnection("Component0", "Component1", parameterMapTest1));

  ParameterMapType parameterMapTest2;
  EXPECT_NO_THROW(parameterMapTest2 = blueprint->GetConnection("Component0", "Component1"));
  EXPECT_EQ( parameterMapTest1["NameOfClass"], parameterMapTest2["NameOfClass"] );
}

TEST_F( BlueprintTest, DeleteConnection )
{
  BlueprintPointerType blueprint = Blueprint::New();

  blueprint->AddComponent("Component0");
  blueprint->AddComponent("Component1");
  blueprint->AddConnection("Component0", "Component1");

  // Connection should exist
  EXPECT_TRUE(blueprint->ConnectionExists("Component0", "Component1"));

  // Connection be deleted
  EXPECT_TRUE(blueprint->DeleteConnection("Component0", "Component1"));  // < CM: Also check case in which method returns false.

  // Connection should not exist 
  EXPECT_FALSE(blueprint->ConnectionExists("Component0", "Component1"));
}

TEST_F( BlueprintTest, WriteBlueprint ) 
{
  BlueprintPointerType blueprint = Blueprint::New();
  
  // create some made up configuration to show graphviz output
  ParameterMapType component0Parameters;
  component0Parameters["NameOfClass"] = { "MyMetric" };
  component0Parameters["Dimensionality"] = { "3" };
  component0Parameters["Kernel"] = { "5", "5", "5" };
  blueprint->AddComponent("Metric", component0Parameters);

  ParameterMapType component1Parameters;
  component1Parameters["NameOfClass"] = { "MyFiniteDifferenceCalculator" };
  component1Parameters["Delta"] = { "0.01" };
  blueprint->AddComponent("MetricGradient", component1Parameters);

  ParameterMapType component2Parameters;
  component2Parameters["NameOfClass"] = { "MyOptimizer" };
  blueprint->AddComponent("Optimizer", component2Parameters);

  ParameterMapType component3Parameters;
  component3Parameters["NameOfClass"] = { "MyTransform" };
  blueprint->AddComponent("Transform", component3Parameters);
  
  blueprint->AddConnection("Metric", "MetricGradient");
  blueprint->AddConnection("MetricGradient", "Optimizer");

  ParameterMapType connection0Parameters;
  // Example use case: The connection between the metric and optimizer should 
  // only be by "MetricValue", not by "MetricDerivative" as well. Since we want
  // to redirect the "MetricDerivative" through the MetricGradient component,
  // we need to specify "NameOfInterface" otherwise there is an ambiguity in 
  // which "MetricDerivative" to connect to the optimizer.
  
  connection0Parameters["NameOfInterface"] = { "MetricValue" };
  blueprint->AddConnection("Metric", "Optimizer", connection0Parameters);
  
  blueprint->AddConnection("MetricGradient", "Optimizer");
  blueprint->AddConnection("Optimizer", "Transform");
  blueprint->AddConnection("Transform", "Metric");

  EXPECT_NO_THROW( blueprint->WriteBlueprint( "blueprint.dot" ) );

  // < CM: Test effect of write method. Open dot file and check its contents.
}
