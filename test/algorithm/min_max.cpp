#include <vector>
#include <utility>
#include <gtest/gtest.h>
#include "sml/algorithm/min_max.hpp"

namespace {

using std::vector;
using std::pair;

TEST(MinMax, InEmptyArray) {
  int seq[0] = {};
  pair<int*, int*> res = sml::min_max(seq, seq);

  ASSERT_EQ(seq, res.first);
  ASSERT_EQ(seq, res.second);
}

TEST(MinMax, EmptyRangeInArray) {
  int seq[3] = {3, 1, 2};
  pair<int*, int*> res = sml::min_max(seq+1, seq+1);

  ASSERT_EQ(seq+1, res.first);
  ASSERT_EQ(seq+1, res.second);
}

TEST(MinMax, InEmptyVector) {
  typedef vector<char>::iterator iterator;
  vector<char> seq;
  pair<iterator, iterator> res = sml::min_max(seq.begin(), seq.end());

  ASSERT_EQ(seq.begin(), res.first);
  ASSERT_EQ(seq.begin(), res.second);
}

TEST(MinMax, OneInArray) {
  int seq[1] = {3};
  pair<int*, int*> res = sml::min_max(seq, seq+1);

  ASSERT_EQ(seq, res.first);
  ASSERT_EQ(seq, res.second);
  ASSERT_EQ(3,   *(res.first));
  ASSERT_EQ(3,   *(res.second));
}

TEST(MinMax, OneInVector) {
  typedef vector<char>::iterator iterator;
  vector<char> seq;
  seq.push_back('A');
  pair<iterator, iterator> res = sml::min_max(seq.begin(), seq.end());

  ASSERT_EQ(seq.begin(), res.first);
  ASSERT_EQ(seq.begin(), res.second);
  ASSERT_EQ('A', *(res.first));
  ASSERT_EQ('A', *(res.second));
}

TEST(MinMax, TwoInArray) {
  int seq[2] = {3, 1};
  pair<int*, int*> res = sml::min_max(seq, seq+2);

  ASSERT_EQ(seq+1, res.first);
  ASSERT_EQ(seq,   res.second);
  ASSERT_EQ(1,     *(res.first));
  ASSERT_EQ(3,     *(res.second));
}

TEST(MinMax, TwoInVector) {
  typedef vector<char>::iterator iterator;
  vector<char> seq;
  seq.push_back('A');
  seq.push_back('Z');
  pair<iterator, iterator> res = sml::min_max(seq.begin(), seq.end());

  ASSERT_EQ(seq.begin(),   res.first);
  ASSERT_EQ(seq.begin()+1, res.second);
  ASSERT_EQ('A', *(res.first));
  ASSERT_EQ('Z', *(res.second));
}

TEST(MinMax, InArray) {
  int seq[5] = {102, 50, 88, 71, 21};
  pair<int*, int*> res = sml::min_max(seq, seq+5);

  ASSERT_EQ(seq+4, res.first);
  ASSERT_EQ(seq,   res.second);
  ASSERT_EQ(21,    *(res.first));
  ASSERT_EQ(102,   *(res.second));
}

TEST(MinMax, RangeInArray) {
  int seq[5] = {102, 50, 88, 71, 21};
  pair<int*, int*> res = sml::min_max(seq+1, seq+4);

  ASSERT_EQ(seq+1, res.first);
  ASSERT_EQ(seq+2, res.second);
  ASSERT_EQ(50,    *(res.first));
  ASSERT_EQ(88,    *(res.second));
}

TEST(MinMax, RangeInVector) {
  typedef vector<char>::iterator iterator;
  vector<char> seq;
  seq.push_back('F'), seq.push_back('T'), seq.push_back('I'),
    seq.push_back('P'), seq.push_back('H'), seq.push_back('W');
  pair<iterator, iterator> res = sml::min_max(seq.begin()+1, seq.end()-1);

  ASSERT_EQ(seq.begin()+4, res.first);
  ASSERT_EQ(seq.begin()+1, res.second);
  ASSERT_EQ('H', *(res.first));
  ASSERT_EQ('T', *(res.second));
}

bool greater(const int& a, const int& b) {
  return b < a;
}
TEST(MinMax, InReversedArray) {
  int seq[5] = {102, 50, 88, 71, 21};
  pair<int*, int*> res = sml::min_max(seq, seq+5, greater);

  ASSERT_EQ(seq,   res.first);
  ASSERT_EQ(seq+4, res.second);
  ASSERT_EQ(102,   *(res.first));
  ASSERT_EQ(21,    *(res.second));
}

TEST(MinMax, hasDuplicateMin) {
  int seq[5] = {102, 21, 88, 71, 21};
  pair<int*, int*> res = sml::min_max(seq, seq+5);

  ASSERT_EQ(seq+1, res.first);
  ASSERT_EQ(seq,   res.second);
  ASSERT_EQ(21,    *(res.first));
  ASSERT_EQ(102,   *(res.second));  
}

TEST(MinMax, hasDuplicateMax) {
  int seq[5] = {102, 50, 88, 102, 21};
  pair<int*, int*> res = sml::min_max(seq, seq+5);

  ASSERT_EQ(seq+4, res.first);
  ASSERT_EQ(seq,   res.second);
  ASSERT_EQ(21,    *(res.first));
  ASSERT_EQ(102,   *(res.second));  
}

TEST(MinMax, hasDuplicateMinMax) {
  int seq[5] = {102, 21, 88, 102, 21};
  pair<int*, int*> res = sml::min_max(seq, seq+5);

  ASSERT_EQ(seq+1, res.first);
  ASSERT_EQ(seq,   res.second);
  ASSERT_EQ(21,    *(res.first));
  ASSERT_EQ(102,   *(res.second));  
}

} // namespace

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
