#include <vector>
#include <utility>
#include <gtest/gtest.h>
#include "sml/sort/insertion_sort.hpp"

namespace {

using std::vector;
using std::pair;
using std::make_pair;

TEST(InsertionSort, InEmptyArray) {
  int seq[0] = {};
  int* res   = sml::sorting::insertion_sort(seq, seq);

  ASSERT_EQ(seq, res);
}

TEST(InsertionSort, EmptyRangeInArray) {
  int seq[3] = {3, 1, 2};
  int* res   = sml::sorting::insertion_sort(seq+1, seq+1);

  ASSERT_EQ(seq+1, res);
  ASSERT_EQ(3, seq[0]);
  ASSERT_EQ(1, seq[1]);
  ASSERT_EQ(2, seq[2]);
}

TEST(InsertionSort, InEmptyVector) {
  vector<char> seq;
  vector<char>::iterator res =
    sml::sorting::insertion_sort(seq.begin(), seq.end());

  ASSERT_EQ(0, seq.size());
  ASSERT_EQ(seq.begin(), res);
}

TEST(InsertionSort, OneInArray) {
  int seq[1] = {3};
  int* res   = sml::sorting::insertion_sort(seq, seq+1);

  ASSERT_EQ(seq, res);
  ASSERT_EQ(3, seq[0]);
}

TEST(InsertionSort, OneInVector) {
  vector<char> seq;
  seq.push_back('A');
  vector<char>::iterator res =
    sml::sorting::insertion_sort(seq.begin(), seq.end());

  ASSERT_EQ(1, seq.size());
  ASSERT_EQ(seq.begin(), res);
  ASSERT_EQ('A', seq[0]);
}

TEST(InsertionSort, InArray) {
  int seq[5] = {102, 50, 88, 71, 21};
  int* res   = sml::sorting::insertion_sort(seq, seq+5);

  ASSERT_EQ(seq, res);
  ASSERT_EQ(21,  seq[0]);
  ASSERT_EQ(50,  seq[1]);
  ASSERT_EQ(71,  seq[2]);
  ASSERT_EQ(88,  seq[3]);
  ASSERT_EQ(102, seq[4]);
}

TEST(InsertionSort, RangeInArray) {
  int seq[5] = {102, 50, 88, 71, 21};
  int* res   = sml::sorting::insertion_sort(seq+1, seq+4);

  ASSERT_EQ(seq+1, res);
  ASSERT_EQ(102, seq[0]);
  ASSERT_EQ(50,  seq[1]);
  ASSERT_EQ(71,  seq[2]);
  ASSERT_EQ(88,  seq[3]);
  ASSERT_EQ(21,  seq[4]);
}

TEST(InsertionSort, RangeInVector) {
  vector<char> seq;
  seq.push_back('F'), seq.push_back('T'), seq.push_back('I'),
  seq.push_back('P'), seq.push_back('H'), seq.push_back('W');
  vector<char>::iterator res =
    sml::sorting::insertion_sort(seq.begin()+2, seq.end()-1);

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
TEST(InsertionSort, InReversedArray) {
  int seq[5] = {102, 50, 88, 71, 21};
  int* res   = sml::sorting::insertion_sort(seq, seq+5, greater);

  ASSERT_EQ(seq, res);
  ASSERT_EQ(102, seq[0]);
  ASSERT_EQ(88,  seq[1]);
  ASSERT_EQ(71,  seq[2]);
  ASSERT_EQ(50,  seq[3]);
  ASSERT_EQ(21,  seq[4]);
}

TEST(InsertionSort, InArrayOf2ndPowerMinus1) {
  int seq[7] = {102, 50, 88, 71, 21, 8, 42};
  int* res   = sml::sorting::insertion_sort(seq, seq+7);

  ASSERT_EQ(seq, res);
  ASSERT_EQ(8,   seq[0]);
  ASSERT_EQ(21,  seq[1]);
  ASSERT_EQ(42,  seq[2]);
  ASSERT_EQ(50,  seq[3]);
  ASSERT_EQ(71,  seq[4]);
  ASSERT_EQ(88,  seq[5]);
  ASSERT_EQ(102, seq[6]);
}

TEST(InsertionSort, InArrayOf2ndPower) {
  int seq[8] = {102, 50, 88, 71, 21, 8, 42, 91};
  int* res   = sml::sorting::insertion_sort(seq, seq+8);

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

TEST(InsertionSort, InArrayOf2ndPowerMinus2) {
  int seq[6] = {102, 50, 88, 71, 21, 8};
  int* res   = sml::sorting::insertion_sort(seq, seq+6);

  ASSERT_EQ(seq, res);
  ASSERT_EQ(8,   seq[0]);
  ASSERT_EQ(21,  seq[1]);
  ASSERT_EQ(50,  seq[2]);
  ASSERT_EQ(71,  seq[3]);
  ASSERT_EQ(88,  seq[4]);
  ASSERT_EQ(102, seq[5]);
}

bool pair_lesser(const pair<int, int>& p1, const pair<int, int>& p2) {
  return p1.first < p2.first;
}
TEST(InsertionSort, Stable) {
  pair<int, int> seq[5] = {
    make_pair(26, 5), make_pair(10, 1),
    make_pair(5, 2),  make_pair(10, 3), make_pair(26, 0)
  };
  pair<int, int>* res = sml::sorting::insertion_sort(seq, seq+5, pair_lesser);

  ASSERT_EQ(seq, res);
  ASSERT_EQ(make_pair(5,  2), res[0]);
  ASSERT_EQ(make_pair(10, 1), res[1]);
  ASSERT_EQ(make_pair(10, 3), res[2]);
  ASSERT_EQ(make_pair(26, 5), res[3]);
  ASSERT_EQ(make_pair(26, 0), res[4]);
}

} // namespace

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
