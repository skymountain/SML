#include <gtest/gtest.h>
#include "sml/op/lesser.hpp"

namespace {

class Lesser : public testing::Test {
protected:

  void SetUp() {
    for (int i = 0; i < Lesser::SIZE; ++i) {
      this->arr_[i] = i;
    }
  }

  static const int SIZE = 3;
  int arr_[SIZE];
  sml::op::lesser lesser_;
};

TEST_F(Lesser, Less) {
  for (int i = 0; i < Lesser::SIZE - 1; ++i) {
    ASSERT_TRUE(this->lesser_(this->arr_[i], this->arr_[i+1]));
  }
}

TEST_F(Lesser, Greater) {
  for (int i = 0; i < Lesser::SIZE - 1; ++i) {
    ASSERT_FALSE(this->lesser_(this->arr_[i+1], this->arr_[i]));
  }
}

TEST_F(Lesser, Equal) {
  for (int i = 0; i < Lesser::SIZE; ++i) {
    ASSERT_FALSE(this->lesser_(this->arr_[i], this->arr_[i]));
  }
}

} // namespace

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
