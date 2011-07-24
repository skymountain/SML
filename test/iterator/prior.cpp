#include <gtest/gtest.h>
#include "sml/iterator/prior.hpp"

namespace {

TEST(Prior, Prior) {
  int arr[] = {1, 2, 3};
  int size = sizeof(arr) / sizeof(int);

  for (int i = 0; i < size; ++i) {
    int* const pos = arr + i;
    ASSERT_EQ(pos - 1, sml::iterator::prior(pos)) << "index: " << i;
  }
}

} // namespace

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
