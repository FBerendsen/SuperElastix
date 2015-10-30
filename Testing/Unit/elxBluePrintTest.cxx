#include "elxBlueprint.h"
#include "elxComponentDescriptor.h"
#include "gtest/gtest.h"

namespace elx {

TEST( Blueprint, Instantiation )
{
  Blueprint::Pointer blueprint = Blueprint::New();
  blueprint->PrintSelf();
  ASSERT_TRUE( true );
}

} // namespace elx
