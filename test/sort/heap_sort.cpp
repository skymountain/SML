#include <vector>
#include <utility>
#include <gtest/gtest.h>
#include "sml/sort/heap_sort.hpp"

namespace {

using std::vector;
using std::pair;
using std::make_pair;

TEST(HeapSort, InEmptyArray) {
  int seq[0] = {};
  int* res   = sml::sorting::heap_sort(seq, seq);

  ASSERT_EQ(seq, res);
}

TEST(HeapSort, EmptyRangeInArray) {
  int seq[3] = {3, 1, 2};
  int* res   = sml::sorting::heap_sort(seq+1, seq+1);

  ASSERT_EQ(seq+1, res);
  ASSERT_EQ(3, seq[0]);
  ASSERT_EQ(1, seq[1]);
  ASSERT_EQ(2, seq[2]);
}

TEST(HeapSort, InEmptyVector) {
  vector<char> seq;
  vector<char>::iterator res =
    sml::sorting::heap_sort(seq.begin(), seq.end());

  ASSERT_EQ(0, seq.size());
  ASSERT_EQ(seq.begin(), res);
}

TEST(HeapSort, OneInArray) {
  int seq[1] = {3};
  int* res   = sml::sorting::heap_sort(seq, seq+1);

  ASSERT_EQ(seq, res);
  ASSERT_EQ(3, seq[0]);
}

TEST(HeapSort, OneInVector) {
  vector<char> seq;
  seq.push_back('A');
  vector<char>::iterator res =
    sml::sorting::heap_sort(seq.begin(), seq.end());

  ASSERT_EQ(1, seq.size());
  ASSERT_EQ(seq.begin(), res);
  ASSERT_EQ('A', seq[0]);
}

TEST(HeapSort, InArray) {
  int seq[5] = {102, 50, 88, 71, 21};
  int* res   = sml::sorting::heap_sort(seq, seq+5);

  ASSERT_EQ(seq, res);
  ASSERT_EQ(21,  seq[0]);
  ASSERT_EQ(50,  seq[1]);
  ASSERT_EQ(71,  seq[2]);
  ASSERT_EQ(88,  seq[3]);
  ASSERT_EQ(102, seq[4]);
}

TEST(HeapSort, RangeInArray) {
  int seq[5] = {102, 50, 88, 71, 21};
  int* res   = sml::sorting::heap_sort(seq+1, seq+4);

  ASSERT_EQ(seq+1, res);
  ASSERT_EQ(102, seq[0]);
  ASSERT_EQ(50,  seq[1]);
  ASSERT_EQ(71,  seq[2]);
  ASSERT_EQ(88,  seq[3]);
  ASSERT_EQ(21,  seq[4]);
}

TEST(HeapSort, RangeInVector) {
  vector<char> seq;
  seq.push_back('F'), seq.push_back('T'), seq.push_back('I'),
    seq.push_back('P'), seq.push_back('H'), seq.push_back('W');
  vector<char>::iterator res =
    sml::sorting::heap_sort(seq.begin()+2, seq.end()-1);

  ASSERT_EQ(6, seq.size());
  ASSERT_EQ(seq.begin()+2, res);
  ASSERT_EQ('F', seq[0]);
  ASSERT_EQ('T', seq[1]);
  ASSERT_EQ('H', seq[2]);
  ASSERT_EQ('I', seq[3]);
  ASSERT_EQ('P', seq[4]);
  ASSERT_EQ('W', seq[5]);
}

bool greater(const int& a, const int& b) {
  return b < a;
}
TEST(HeapSort, InReversedArray) {
  int seq[5] = {102, 50, 88, 71, 21};
  int* res   = sml::sorting::heap_sort(seq, seq+5, greater);

  ASSERT_EQ(seq, res);
  ASSERT_EQ(102, seq[0]);
  ASSERT_EQ(88,  seq[1]);
  ASSERT_EQ(71,  seq[2]);
  ASSERT_EQ(50,  seq[3]);
  ASSERT_EQ(21,  seq[4]);
}

TEST(HeapSort, InArrayOf2ndPowerMinus1) {
  int seq[7] = {102, 50, 88, 71, 21, 8, 42};
  int* res   = sml::sorting::heap_sort(seq, seq+7);

  ASSERT_EQ(seq, res);
  ASSERT_EQ(8,   seq[0]);
  ASSERT_EQ(21,  seq[1]);
  ASSERT_EQ(42,  seq[2]);
  ASSERT_EQ(50,  seq[3]);
  ASSERT_EQ(71,  seq[4]);
  ASSERT_EQ(88,  seq[5]);
  ASSERT_EQ(102, seq[6]);
}

TEST(HeapSort, InArrayOf2ndPower) {
  int seq[8] = {102, 50, 88, 71, 21, 8, 42, 91};
  int* res   = sml::sorting::heap_sort(seq, seq+8);

  ASSERT_EQ(seq, res);
  ASSERT_EQ(8,   seq[0]);
  ASSERT_EQ(21,  seq[1]);
  ASSERT_EQ(42,  seq[2]);
  ASSERT_EQ(50,  seq[3]);
  ASSERT_EQ(71,  seq[4]);
  ASSERT_EQ(88,  seq[5]);
  ASSERT_EQ(91,  seq[6]);
  ASSERT_EQ(102, seq[7]);
}

TEST(HeapSort, InArrayOf2ndPowerMinus2) {
  int seq[6] = {102, 50, 88, 71, 21, 8};
  int* res   = sml::sorting::heap_sort(seq, seq+6);

  ASSERT_EQ(seq, res);
  ASSERT_EQ(8,   seq[0]);
  ASSERT_EQ(21,  seq[1]);
  ASSERT_EQ(50,  seq[2]);
  ASSERT_EQ(71,  seq[3]);
  ASSERT_EQ(88,  seq[4]);
  ASSERT_EQ(102, seq[5]);
}

} // namespace

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
