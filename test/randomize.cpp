#include <algorithm>
#include <vector>
#include <cstdlib>
#include <gtest/gtest.h>
#include "sml/randomize.hpp"

namespace {

using std::vector;
using std::find;
using std::rand;
  
TEST(Randomize, InEmptyArray) {
  int seq[0] = {};
  int* res   = sml::randomize(seq, seq, rand);

  EXPECT_EQ(seq, res);
}

TEST(Randomize, InEmptyVector) {
  vector<char> seq;
  vector<char>::iterator res = sml::randomize(seq.begin(), seq.end(), rand);

  EXPECT_EQ(0, seq.size());
  EXPECT_EQ(seq.begin(), res);
}

TEST(Randomize, EmptyRangeInArray) {
  int seq[3] = {1, 2, 3};
  int* res   = sml::randomize(seq+1, seq+1, rand);

  EXPECT_EQ(seq+1, res);
  EXPECT_EQ(1, seq[0]);
  EXPECT_EQ(2, seq[1]);
  EXPECT_EQ(3, seq[2]);
}

TEST(Randomize, InArray) {
  int seq[3] = {1, 2, 3};
  int* end   = seq+3;
  int* res   = sml::randomize(seq, end, rand);

  EXPECT_EQ(seq, res);
  EXPECT_NE(end, std::find(seq, end, 1));
  EXPECT_NE(end, std::find(seq, end, 2));
  EXPECT_NE(end, std::find(seq, end, 3));

  int seq2[3] = { 1, 2, 3 };
  EXPECT_EQ(seq2+3, std::find(seq2, seq2+3, 4));
}

TEST(Randomize, RangeInArray) {
  int seq[5] = {1, 2, 3, 4, 5};
  int* end   = seq+3;
  int* res   = sml::randomize(seq, end, rand);

  EXPECT_EQ(seq, res);
  EXPECT_NE(end, std::find(seq, end, 1));
  EXPECT_NE(end, std::find(seq, end, 2));
  EXPECT_NE(end, std::find(seq, end, 3));
  EXPECT_EQ(4, seq[3]);
  EXPECT_EQ(5, seq[4]);
}

TEST(Randomize, RangeInVector) {
  vector<char> seq;
  seq.push_back('A'), seq.push_back('B'),
  seq.push_back('C'), seq.push_back('D'), seq.push_back('E');
  vector<char>::iterator begin = seq.begin();
  vector<char>::iterator end   = seq.end();
  vector<char>::iterator res   = sml::randomize(begin+2, end-1, rand);

  EXPECT_EQ(5, seq.size());
  EXPECT_EQ(begin+2, res);
  EXPECT_EQ('A', seq[0]);
  EXPECT_EQ('B', seq[1]);
  EXPECT_NE(end-1, std::find(begin+2, end-1, 'C'));
  EXPECT_NE(end-1, std::find(begin+2, end-1, 'D'));
  EXPECT_EQ('E', seq[4]);
}

} // namespace

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();

}
