#include <vector>
#include <utility>
#include <gtest/gtest.h>
#include "sml/algorithm/binary_search_min_max.hpp"

namespace {

using std::vector;
using std::pair;

template<class Iterator>
void find_in_range(const Iterator begin, const Iterator end) {
  for (Iterator it = begin; it != end; ++it) {
    pair<Iterator, Iterator> res =
      sml::algorithm::binary_search_min_max(begin, end, *it);

    ASSERT_LE(res.first,  it);
    ASSERT_GE(res.second, it);
  }
}

template<class SearchIterator, class ValueIterator>
void no_find_in_range(
                      const SearchIterator begin,  const SearchIterator end,
                      const ValueIterator  vbegin, const ValueIterator  vend) {
  for (ValueIterator it = vbegin; it != vend; ++it) {
    pair<SearchIterator, SearchIterator> res =
      sml::algorithm::binary_search_min_max(begin, end, *it);

    ASSERT_EQ(end, res.first);
    ASSERT_EQ(end, res.second);
  }
}

TEST(BinarySearch, InEmptyArray) {
  int seq[0] = {};

  pair<int*, int*> res = sml::algorithm::binary_search_min_max(seq, seq, 0);
  ASSERT_EQ(seq, res.first);
  ASSERT_EQ(seq, res.second);
}

TEST(BinarySearch, EmptyRangeInArray) {
  SCOPED_TRACE("EmptyRangeInArray");

  int seq[3] = {1, 2, 3};

  int no_seq[5] = {1, 2, 3, 0, 4};
  no_find_in_range(seq+1, seq+1, no_seq, no_seq+5);

  ASSERT_EQ(1, seq[0]);
  ASSERT_EQ(2, seq[1]);
  ASSERT_EQ(3, seq[2]);
}

TEST(BinarySearch, InEmptyVector) {
  vector<char> seq;

  pair<vector<char>::iterator, vector<char>::iterator> res =
    sml::algorithm::binary_search_min_max(seq.begin(), seq.end(), 'A');
  ASSERT_EQ(seq.begin(), res.first);
  ASSERT_EQ(seq.begin(), res.second);

  ASSERT_EQ(0, seq.size());
}

TEST(BinarySearch, OneInArray) {
  SCOPED_TRACE("OneInArray");

  int seq[1] = {3};
  find_in_range(seq, seq+1);

  int no_seq[2] = {1, 5};
  no_find_in_range(seq, seq+1, no_seq, no_seq+2);

  ASSERT_EQ(3, seq[0]);
}

TEST(BinarySearch, OneInVector) {
  SCOPED_TRACE("OneInVector");

  vector<char> seq;
  seq.push_back('B');

  find_in_range(seq.begin(), seq.end());

  char no_seq[2] = {'A', 'C'};
  no_find_in_range(seq.begin(), seq.end(), no_seq, no_seq+2);

  ASSERT_EQ(1, seq.size());
  ASSERT_EQ('B', seq[0]);
}

TEST(BinarySearch, InArray) {
  SCOPED_TRACE("InArray");

  int seq[10] = {21, 21, 50, 68, 71, 71, 71, 88, 102, 102};
  find_in_range(seq, seq+10);

  int no_seq[3] = {13, 79, 112};
  no_find_in_range(seq, seq+10, no_seq, no_seq+3);

  pair<int*, int*> res1 =
    sml::algorithm::binary_search_min_max(seq, seq+10, 21);
  ASSERT_EQ(seq,   res1.first);
  ASSERT_EQ(seq+1, res1.second);

  pair<int*, int*> res2 =
    sml::algorithm::binary_search_min_max(seq, seq+10, 71);
  ASSERT_EQ(seq+4, res2.first);
  ASSERT_EQ(seq+6, res2.second);

  pair<int*, int*> res3 =
    sml::algorithm::binary_search_min_max(seq, seq+10, 102);
  ASSERT_EQ(seq+8, res3.first);
  ASSERT_EQ(seq+9, res3.second);

  ASSERT_EQ(21,  seq[0]);
  ASSERT_EQ(21,  seq[1]);
  ASSERT_EQ(50,  seq[2]);
  ASSERT_EQ(68,  seq[3]);
  ASSERT_EQ(71,  seq[4]);
  ASSERT_EQ(71,  seq[5]);
  ASSERT_EQ(71,  seq[6]);
  ASSERT_EQ(88,  seq[7]);
  ASSERT_EQ(102, seq[8]);
  ASSERT_EQ(102, seq[9]);
}

TEST(BinarySearch, RangeInArray) {
  SCOPED_TRACE("RangeInArray");

  int seq[12] = {51, 21, 21, 50, 68, 71, 71, 71, 88, 102, 102, 14};
  find_in_range(seq+1, seq+11);

  int no_seq[3] = {13, 79, 112};
  no_find_in_range(seq+1, seq+11, no_seq, no_seq+3);

  pair<int*, int*> res1 =
    sml::algorithm::binary_search_min_max(seq+1, seq+11, 21);
  ASSERT_EQ(seq+1,   res1.first);
  ASSERT_EQ(seq+2, res1.second);

  pair<int*, int*> res2 =
    sml::algorithm::binary_search_min_max(seq+1, seq+11, 71);
  ASSERT_EQ(seq+5, res2.first);
  ASSERT_EQ(seq+7, res2.second);

  pair<int*, int*> res3 =
    sml::algorithm::binary_search_min_max(seq+1, seq+11, 102);
  ASSERT_EQ(seq+9,  res3.first);
  ASSERT_EQ(seq+10, res3.second);

  ASSERT_EQ(51,  seq[0]);
  ASSERT_EQ(21,  seq[1]);
  ASSERT_EQ(21,  seq[2]);
  ASSERT_EQ(50,  seq[3]);
  ASSERT_EQ(68,  seq[4]);
  ASSERT_EQ(71,  seq[5]);
  ASSERT_EQ(71,  seq[6]);
  ASSERT_EQ(71,  seq[7]);
  ASSERT_EQ(88,  seq[8]);
  ASSERT_EQ(102, seq[9]);
  ASSERT_EQ(102, seq[10]);
  ASSERT_EQ(14,  seq[11]);
}

TEST(BinarySearch, RangeInVector) {
  SCOPED_TRACE("RangeInVector");

  vector<char> seq;
  seq.push_back('G'), seq.push_back('T'), seq.push_back('H'),
  seq.push_back('H'), seq.push_back('P'), seq.push_back('Q'),
  seq.push_back('Q'), seq.push_back('T'), seq.push_back('X'),
  seq.push_back('X'), seq.push_back('X'), seq.push_back('B');

  find_in_range(seq.begin()+2, seq.begin()+10);

  char no_seq[6] = {'A', 'Z', 'J', 'F', 'S', 'C'};
  no_find_in_range(seq.begin()+2, seq.begin()+10, no_seq, no_seq+6);

  pair<vector<char>::iterator, vector<char>::iterator> res1 =
    sml::algorithm::binary_search_min_max(seq.begin()+2, seq.begin()+10, 'H');
  ASSERT_EQ(seq.begin()+2, res1.first);
  ASSERT_EQ(seq.begin()+3, res1.second);

  pair<vector<char>::iterator, vector<char>::iterator> res2 =
    sml::algorithm::binary_search_min_max(seq.begin()+2, seq.begin()+10, 'Q');
  ASSERT_EQ(seq.begin()+5, res2.first);
  ASSERT_EQ(seq.begin()+6, res2.second);

  pair<vector<char>::iterator, vector<char>::iterator> res3 =
    sml::algorithm::binary_search_min_max(seq.begin()+2, seq.begin()+10, 'X');
  ASSERT_EQ(seq.begin()+8,  res3.first);
  ASSERT_EQ(seq.begin()+9, res3.second);

  ASSERT_EQ(12, seq.size());
  ASSERT_EQ('G', seq[0]);
  ASSERT_EQ('T', seq[1]);
  ASSERT_EQ('H', seq[2]);
  ASSERT_EQ('H', seq[3]);
  ASSERT_EQ('P', seq[4]);
  ASSERT_EQ('Q', seq[5]);
  ASSERT_EQ('Q', seq[6]);
  ASSERT_EQ('T', seq[7]);
  ASSERT_EQ('X', seq[8]);
  ASSERT_EQ('X', seq[9]);
  ASSERT_EQ('X', seq[10]);
  ASSERT_EQ('B', seq[11]);
}

TEST(BinarySearch, InArrayOf2ndPower) {
  SCOPED_TRACE("InArrayOf2ndPower");

  int seq[8] = {8, 8, 42, 71, 71, 88, 102, 102};
  find_in_range(seq, seq+8);

  int no_seq[3] = {13, 79, 112};
  no_find_in_range(seq, seq+8, no_seq, no_seq+3);

  pair<int*, int*> res1 =
    sml::algorithm::binary_search_min_max(seq, seq+8, 8);
  ASSERT_EQ(seq,   res1.first);
  ASSERT_EQ(seq+1, res1.second);

  pair<int*, int*> res2 =
    sml::algorithm::binary_search_min_max(seq, seq+8, 71);
  ASSERT_EQ(seq+3, res2.first);
  ASSERT_EQ(seq+4, res2.second);

  pair<int*, int*> res3 =
    sml::algorithm::binary_search_min_max(seq, seq+8, 102);
  ASSERT_EQ(seq+6, res3.first);
  ASSERT_EQ(seq+7, res3.second);

  ASSERT_EQ(8,   seq[0]);
  ASSERT_EQ(8,   seq[1]);
  ASSERT_EQ(42,  seq[2]);
  ASSERT_EQ(71,  seq[3]);
  ASSERT_EQ(71,  seq[4]);
  ASSERT_EQ(88,  seq[5]);
  ASSERT_EQ(102, seq[6]);
  ASSERT_EQ(102, seq[7]);
}

TEST(BinarySearch, InArrayOf2ndPowerMinus1) {
  SCOPED_TRACE("InArrayOf2ndPowerMinus1");

  int seq[7] = {8, 8, 42, 50, 71, 71, 71};
  find_in_range(seq, seq+7);

  int no_seq[3] = {13, 79, 112};
  no_find_in_range(seq, seq+7, no_seq, no_seq+3);

  pair<int*, int*> res1 =
    sml::algorithm::binary_search_min_max(seq, seq+7, 8);
  ASSERT_EQ(seq,   res1.first);
  ASSERT_EQ(seq+1, res1.second);

  pair<int*, int*> res2 =
    sml::algorithm::binary_search_min_max(seq, seq+7, 71);
  ASSERT_EQ(seq+4, res2.first);
  ASSERT_EQ(seq+6, res2.second);

  ASSERT_EQ(8,  seq[0]);
  ASSERT_EQ(8,  seq[1]);
  ASSERT_EQ(42, seq[2]);
  ASSERT_EQ(50, seq[3]);
  ASSERT_EQ(71, seq[4]);
  ASSERT_EQ(71, seq[5]);
  ASSERT_EQ(71, seq[6]);
}

TEST(BinarySearch, InArrayOf2ndPowerMinus2) {
  SCOPED_TRACE("InArrayOf2ndPowerMinus2");

  int seq[6] = {8, 8, 42, 71, 102, 102};
  find_in_range(seq, seq+6);

  int no_seq[3] = {13, 79, 112};
  no_find_in_range(seq, seq+6, no_seq, no_seq+3);

  pair<int*, int*> res1 =
    sml::algorithm::binary_search_min_max(seq, seq+6, 8);
  ASSERT_EQ(seq,   res1.first);
  ASSERT_EQ(seq+1, res1.second);

  pair<int*, int*> res2 =
    sml::algorithm::binary_search_min_max(seq, seq+6, 102);
  ASSERT_EQ(seq+4, res2.first);
  ASSERT_EQ(seq+5, res2.second);

  ASSERT_EQ(8,   seq[0]);
  ASSERT_EQ(8,   seq[1]);
  ASSERT_EQ(42,  seq[2]);
  ASSERT_EQ(71,  seq[3]);
  ASSERT_EQ(102, seq[4]);
  ASSERT_EQ(102, seq[5]);
}

} // namespace

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
