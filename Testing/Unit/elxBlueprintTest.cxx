#include "elxBlueprint.h"

#include "gtest/gtest.h"

using namespace selx;

class BlueprintTest : public ::testing::Test {
public:

  typedef Blueprint::Pointer                BlueprintPointerType;
  typedef Blueprint::ComponentIndexType     ComponentIndexType;
  typedef Blueprint::ParameterMapType       ParameterMapType;
  typedef Blueprint::ParameterValueType     ParameterValueType;

  virtual void SetUp() {
    parameterMap["ComponentName"] = ParameterValueType(1, "TestName");
  }

  ParameterMapType parameterMap;
};

TEST_F( BlueprintTest, AddComponent )
{
  BlueprintPointerType blueprint;
  EXPECT_NO_THROW( blueprint = Blueprint::New() );

  ComponentIndexType index0;
  EXPECT_NO_THROW( index0 = blueprint->AddComponent() );

  ComponentIndexType index1;
  EXPECT_NO_THROW( index1 = blueprint->AddComponent( parameterMap ) );
}

TEST_F( BlueprintTest, GetComponent ) 
{
  BlueprintPointerType blueprint = Blueprint::New();
  ComponentIndexType index = blueprint->AddComponent( parameterMap );

  ParameterMapType parameterMapTest;
  EXPECT_NO_THROW( parameterMapTest = blueprint->GetComponent( index ) );
  EXPECT_EQ( parameterMap["ComponentName"], parameterMapTest["ComponentName"] );
}

TEST_F( BlueprintTest, SetComponent ) 
{
  BlueprintPointerType blueprint = Blueprint::New();
  ComponentIndexType index = blueprint->AddComponent( parameterMap );

  ParameterMapType parameterMapTest;
  EXPECT_NO_THROW( blueprint->SetComponent( index, parameterMap ) );
  EXPECT_NO_THROW( parameterMapTest = blueprint->GetComponent( index ) ); // <-- Change to GetComponentParameters?
  EXPECT_EQ( parameterMap["ComponentName"], parameterMapTest["ComponentName"] );
}

// TODO: The final line segfaults because GetComponent does not check that the index actually
// actually exist. See explanation in elxBlueprint.h
// TEST_F( BlueprintTest, DeleteComponent ) 
// {
//   BlueprintPointerType blueprint = Blueprint::New();
//   ComponentIndexType index = blueprint->AddComponent( parameterMap );
//
//   ParameterMapType parameterMapTest;
//   EXPECT_NO_THROW( parameterMapTest = blueprint->GetComponent( index ) );
//   EXPECT_EQ( parameterMap["ComponentName"], parameterMapTest["ComponentName"] );
//
//   EXPECT_NO_THROW( blueprint->DeleteComponent( index ) );
//   EXPECT_ANY_THROW( parameterMapTest = blueprint->GetComponent( index ) );
// }

TEST_F( BlueprintTest, AddConnection )
{
  BlueprintPointerType blueprint = Blueprint::New();

  ComponentIndexType index0 = blueprint->AddComponent();
  ComponentIndexType index1 = blueprint->AddComponent();
  ComponentIndexType index2 = blueprint->AddComponent();

  // Connection should not exist
  EXPECT_FALSE( blueprint->ConnectionExists( index0, index1 ) );

  // Connection should be added
  EXPECT_TRUE( blueprint->AddConnection( index0, index1 ) );

  // Connection should exist
  EXPECT_TRUE( blueprint->ConnectionExists( index0, index1 ) );

  // Another connection between same components should not be added
  // (user should use SetComponent() instead)
  EXPECT_FALSE( blueprint->AddConnection( index0, index1 ) );

  // Connection should be empty
  ParameterMapType parameterMapTest0;
  EXPECT_NO_THROW( parameterMapTest0 = blueprint->GetConnection( index0, index1 ) );
  EXPECT_EQ( 0u, parameterMapTest0.size() );

  // Connection with properties should be added. Testing if properties was 
  // properly set requires a call to GetConnection() which is the responsibility
  // of the next test.
  ParameterMapType parameterMapTest1;
  EXPECT_TRUE( blueprint->AddConnection( index1, index2, parameterMap ) );
}

TEST_F( BlueprintTest, GetConnection )
{
  BlueprintPointerType blueprint = Blueprint::New();

  ComponentIndexType index0 = blueprint->AddComponent();
  ComponentIndexType index1 = blueprint->AddComponent();

  ParameterMapType parameterMapTest0;
  EXPECT_TRUE( blueprint->AddConnection( index0, index1, parameterMap ) );
  EXPECT_NO_THROW( parameterMapTest0 = blueprint->GetConnection( index0, index1 ) );
  EXPECT_EQ( parameterMap["ComponentName"], parameterMapTest0["ComponentName"] );
}

TEST_F( BlueprintTest, SetConnection )
{
  BlueprintPointerType blueprint = Blueprint::New();

  ComponentIndexType index0 = blueprint->AddComponent();
  ComponentIndexType index1 = blueprint->AddComponent();
  blueprint->AddConnection( index0, index1, parameterMap );

  ParameterMapType parameterMapTest0;
  parameterMapTest0 = blueprint->GetConnection( index0, index1 );
  EXPECT_EQ( parameterMap["ComponentName"], parameterMapTest0["ComponentName"] );

  ParameterMapType parameterMapTest1;
  parameterMapTest1["ComponentName"] = ParameterValueType(1, "OtherName");
  EXPECT_TRUE( blueprint->SetConnection( index0, index1, parameterMapTest1 ) );

  ParameterMapType parameterMapTest2;
  EXPECT_NO_THROW( parameterMapTest2 = blueprint->GetConnection( index0, index1 ) );
  EXPECT_EQ( parameterMapTest1["ComponentName"], parameterMapTest2["ComponentName"] );
}

TEST_F( BlueprintTest, DeleteConnection )
{
  BlueprintPointerType blueprint = Blueprint::New();

  ComponentIndexType index0 = blueprint->AddComponent();
  ComponentIndexType index1 = blueprint->AddComponent();
  blueprint->AddConnection( index0, index1 );

  // Connection should exist
  EXPECT_TRUE( blueprint->ConnectionExists( index0, index1 ) );

  // Connection be deleted
  EXPECT_TRUE( blueprint->DeleteConnection( index0, index1 ) );

  // Connection should not exist 
  EXPECT_FALSE( blueprint->ConnectionExists( index0, index1 ) );
}

TEST_F( BlueprintTest, WriteBlueprint ) 
{
  BlueprintPointerType blueprint = Blueprint::New();

  ComponentIndexType index0 = blueprint->AddComponent();
  ComponentIndexType index1 = blueprint->AddComponent();
  ComponentIndexType index2 = blueprint->AddComponent();
  ComponentIndexType index3 = blueprint->AddComponent();

  blueprint->AddConnection( index0, index1 );
  blueprint->AddConnection( index0, index2 );
  blueprint->AddConnection( index2, index3 );

  EXPECT_NO_THROW( blueprint->WriteBlueprint( "blueprint.dot" ) );
}

TEST_F( BlueprintTest, ComponentIterator )
{
  typedef Blueprint::ComponentIteratorType ComponentIteratorType;
  typedef Blueprint::ComponentIteratorPairType ComponentIteratorPairType;

  BlueprintPointerType blueprint = Blueprint::New();

  ComponentIndexType index0 = blueprint->AddComponent();
  blueprint->SetComponent( index0, parameterMap ); // <-- Change name to SetComponentParameters?
  ComponentIndexType index1 = blueprint->AddComponent();
  blueprint->SetComponent( index1, parameterMap );
  ComponentIndexType index2 = blueprint->AddComponent();
  blueprint->SetComponent( index2, parameterMap );

  ComponentIteratorPairType componentIterators = blueprint->GetComponentIterator();
  ComponentIteratorType componentIterator = componentIterators.first; // <-- begin()
  ComponentIteratorType componentIteratorEnd = componentIterators.second; // <-- end()

  // Dereferencing the iterator gives the component index
  EXPECT_TRUE( (*componentIterator) == 0u );
  ++componentIterator;
  EXPECT_TRUE( (*componentIterator) == 1u );
  ++componentIterator;
  EXPECT_TRUE( (*componentIterator) == 2u );
  ++componentIterator;

  // TODO: Test iterator on parameter map component names
}

TEST_F( BlueprintTest, ConnectionIterator )
{
  typedef Blueprint::OutputIteratorType OutputIteratorType;
  typedef Blueprint::OutputIteratorPairType OutputIteratorPairType;

  BlueprintPointerType blueprint = Blueprint::New();

  ComponentIndexType index0 = blueprint->AddComponent();
  ComponentIndexType index1 = blueprint->AddComponent();
  ComponentIndexType index2 = blueprint->AddComponent();
  ComponentIndexType index3 = blueprint->AddComponent();

  blueprint->AddConnection( index0, index1 );
  blueprint->AddConnection( index0, index2 );
  blueprint->AddConnection( index2, index3 );

  OutputIteratorPairType outputIterators = blueprint->GetOutputIterator( index0 );
  OutputIteratorType outputIterator = outputIterators.first;
  OutputIteratorType outputIteratorEnd = outputIterators.second;

  // TODO: Should we write an ITK style iterator that hides boost iterators? These
  // are very 90s style and clumsy to use because you need the graph itself to 
  // dereference the iterator because of a source/target indirection
  //
  // index = boost::source( *connectionIterator, blueprint->GetGraph() )
  //
}
