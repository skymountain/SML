#include <vector>
#include <gtest/gtest.h>
#include "sml/algorithm/binary_search.hpp"

namespace {

using std::vector;

template<class Iterator>
void find_in_range(const Iterator begin, const Iterator end) {
  for (Iterator it = begin; it != end; ++it) {
    Iterator res = sml::algorithm::binary_search(begin, end, *it);
    ASSERT_EQ(it, res);
  }
}

template<class SearchIterator, class ValueIterator>
void no_find_in_range(
  const SearchIterator begin,  const SearchIterator end,
  const ValueIterator  vbegin, const ValueIterator  vend) {
  for (ValueIterator it = vbegin; it != vend; ++it) {
    SearchIterator res = sml::algorithm::binary_search(begin, end, *it);
    ASSERT_EQ(end, res);
  }
}

TEST(BinarySearch, InEmptyArray) {
  int seq[0] = {};

  int* res   = sml::algorithm::binary_search(seq, seq, 0);
  ASSERT_EQ(seq, res);
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

  vector<char>::iterator res =
    sml::algorithm::binary_search(seq.begin(), seq.end(), 'A');
  ASSERT_EQ(seq.begin(), res);

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

  int seq[5] = {21, 50, 71, 88, 102};
  find_in_range(seq, seq+5);

  int no_seq[3] = {13, 79, 112};
  no_find_in_range(seq, seq+5, no_seq, no_seq+3);

  ASSERT_EQ(21,  seq[0]);
  ASSERT_EQ(50,  seq[1]);
  ASSERT_EQ(71,  seq[2]);
  ASSERT_EQ(88,  seq[3]);
  ASSERT_EQ(102, seq[4]);
}

TEST(BinarySearch, RangeInArray) {
  SCOPED_TRACE("RangeInArray");

  int seq[5] = {71, 21, 50, 102, 88};
  find_in_range(seq+1, seq+4);

  int no_seq[5] = {13, 79, 112, 71, 88};
  no_find_in_range(seq+1, seq+4, no_seq, no_seq+5);

  ASSERT_EQ(71,  seq[0]);
  ASSERT_EQ(21,  seq[1]);
  ASSERT_EQ(50,  seq[2]);
  ASSERT_EQ(102, seq[3]);
  ASSERT_EQ(88,  seq[4]);
}

TEST(BinarySearch, RangeInVector) {
  SCOPED_TRACE("RangeInVector");

  vector<char> seq;
  seq.push_back('F'), seq.push_back('T'), seq.push_back('H'),
  seq.push_back('I'), seq.push_back('P'), seq.push_back('C');

  find_in_range(seq.begin()+2, seq.begin()+5);

  char no_seq[6] = {'A', 'Z', 'J', 'F', 'T', 'C'};
  no_find_in_range(seq.begin()+2, seq.begin()+5, no_seq, no_seq+6);

  ASSERT_EQ(6, seq.size());
  ASSERT_EQ('F', seq[0]);
  ASSERT_EQ('T', seq[1]);
  ASSERT_EQ('H', seq[2]);
  ASSERT_EQ('I', seq[3]);
  ASSERT_EQ('P', seq[4]);
  ASSERT_EQ('C', seq[5]);
}

TEST(BinarySearch, InArrayOf2ndPower) {
  SCOPED_TRACE("InArrayOf2ndPower");

  int seq[8] = {8, 21, 42, 50, 71, 88, 91, 102};
  find_in_range(seq, seq+8);

  int no_seq[3] = {13, 79, 112};
  no_find_in_range(seq, seq+8, no_seq, no_seq+3);

  ASSERT_EQ(8,   seq[0]);
  ASSERT_EQ(21,  seq[1]);
  ASSERT_EQ(42,  seq[2]);
  ASSERT_EQ(50,  seq[3]);
  ASSERT_EQ(71,  seq[4]);
  ASSERT_EQ(88,  seq[5]);
  ASSERT_EQ(91,  seq[6]);
  ASSERT_EQ(102, seq[7]);
}

TEST(BinarySearch, InArrayOf2ndPowerMinus1) {
  SCOPED_TRACE("InArrayOf2ndPowerMinus1");

  int seq[7] = {8, 21, 42, 50, 71, 88, 102};
  find_in_range(seq, seq+7);

  int no_seq[3] = {13, 79, 112};
  no_find_in_range(seq, seq+7, no_seq, no_seq+3);

  ASSERT_EQ(8,   seq[0]);
  ASSERT_EQ(21,  seq[1]);
  ASSERT_EQ(42,  seq[2]);
  ASSERT_EQ(50,  seq[3]);
  ASSERT_EQ(71,  seq[4]);
  ASSERT_EQ(88,  seq[5]);
  ASSERT_EQ(102, seq[6]);
}

TEST(BinarySearch, InArrayOf2ndPowerMinus2) {
  SCOPED_TRACE("InArrayOf2ndPowerMinus2");

  int seq[6] = {8, 21, 50, 71, 88, 102};
  find_in_range(seq, seq+6);

  int no_seq[3] = {13, 79, 112};
  no_find_in_range(seq, seq+6, no_seq, no_seq+3);

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
