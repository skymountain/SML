#include <gtest/gtest.h>
#include "sml/op/comparator.hpp"

namespace {

class Comparator : public testing::Test {
protected:

  void SetUp() {
    for (int i = 0; i < Comparator::SIZE; ++i) {
      this->arr_[i] = i;
    }
  }

  static const int SIZE = 3;
  int arr_[SIZE];
  sml::op::comparator cmp_;
};

TEST_F(Comparator, Less) {
  for (int i = 0; i < Comparator::SIZE - 1; ++i) {
    ASSERT_EQ(-1, this->cmp_(this->arr_[i], this->arr_[i+1]));
  }
}

TEST_F(Comparator, Greater) {
  for (int i = 0; i < Comparator::SIZE - 1; ++i) {
    ASSERT_EQ(1, this->cmp_(this->arr_[i+1], this->arr_[i]));
  }
}

TEST_F(Comparator, Equal) {
  for (int i = 0; i < Comparator::SIZE; ++i) {
    ASSERT_EQ(0, this->cmp_(this->arr_[i], this->arr_[i]));
  }
}

} // namespace

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
