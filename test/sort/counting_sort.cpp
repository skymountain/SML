#include <vector>
#include <iterator>
#include <utility>
#include <cstddef>
#include <gtest/gtest.h>
#include "sml/sort/counting_sort.hpp"

namespace {

using std::vector;
using std::pair;
using std::make_pair;
using std::size_t;

struct id_encoder_class {
  typedef size_t result_type;

  size_t operator()(int x) {
    return static_cast<size_t>(x);
  }
};

const size_t id_encoder_fun(const int& x) {
  return static_cast<size_t>(x);
}

TEST(CountingSort, InEmptyArray) {
  int seq[0] = {};
  vector<int> dst(0);
  vector<int>::iterator res =
    sml::sorting::counting_sort(seq, seq, dst.begin(), id_encoder_class());

  ASSERT_EQ(dst.begin(), res);
  ASSERT_EQ(0, dst.size());
}

TEST(CountingSort, EmptyRangeInArray) {
  int seq[3] = {3, 1, 2};
  int* dst = NULL;
  int* res =
    sml::sorting::counting_sort(seq, seq, dst, id_encoder_fun);

  ASSERT_EQ(dst, res);
}

TEST(CountingSort, InEmptyVector) {
  vector<char> seq;
  vector<char> dst(0);
  vector<char>::iterator res =
    sml::sorting::counting_sort(
      seq.begin(), seq.end(), dst.begin(), id_encoder_fun
    );

  ASSERT_EQ(dst.begin(), res);
  ASSERT_EQ(0, dst.size());
}

TEST(CountingSort, OneInArray) {
  int seq[1] = {3};
  int dst[1] = {0};
  int* res =
    sml::sorting::counting_sort(
      seq, seq+1, dst, id_encoder_class()
    );

  ASSERT_EQ(dst, res);
  ASSERT_EQ(3,   *dst);
}

TEST(CountingSort, OneInVector) {
  vector<char> seq;
  seq.push_back('A');
  vector<char> dst(1);
  vector<char>::iterator res =
    sml::sorting::counting_sort(
      seq.begin(), seq.end(), dst.begin(), id_encoder_fun
    );

  ASSERT_EQ(dst.begin(), res);
  ASSERT_EQ(1,   dst.size());
  ASSERT_EQ('A', dst[0]);
}

TEST(CountingSort, InArray) {
  int seq[5] = {102, 50, 88, 71, 21};
  int dst[5] = {1, 2, 3, 4, 5};
  int*res =
    sml::sorting::counting_sort(
      seq, seq+5, dst, id_encoder_class()
    );

  ASSERT_EQ(dst, res);
  ASSERT_EQ(21,  dst[0]);
  ASSERT_EQ(50,  dst[1]);
  ASSERT_EQ(71,  dst[2]);
  ASSERT_EQ(88,  dst[3]);
  ASSERT_EQ(102, dst[4]);
}

TEST(CountingSort, RangeInArray) {
  int seq[5] = {102, 50, 88, 71, 21};
  vector<int> dst(3);
  vector<int>::iterator res =
    sml::sorting::counting_sort(seq+1, seq+4, dst.begin(), id_encoder_fun);

  ASSERT_EQ(dst.begin(), res);
  ASSERT_EQ(3,   dst.size());
  ASSERT_EQ(50,  dst[0]);
  ASSERT_EQ(71,  dst[1]);
  ASSERT_EQ(88,  dst[2]);
}

TEST(CountingSort, RangeInVector) {
  vector<char> seq;
  seq.push_back('F'), seq.push_back('T'), seq.push_back('I'),
  seq.push_back('P'), seq.push_back('H'), seq.push_back('W');
  char dst[3] = {'1', '2', '3'};
  char* res =
    sml::sorting::counting_sort(
      seq.begin()+2, seq.end()-1, dst, id_encoder_class()
    );

  ASSERT_EQ(dst, res);
  ASSERT_EQ('H', dst[0]);
  ASSERT_EQ('I', dst[1]);
  ASSERT_EQ('P', dst[2]);
}

TEST(CountingSort, InArrayOf2ndPowerMinus1) {
  int seq[7] = {102, 50, 88, 71, 21, 8, 42};
  vector<int> dst(7);
  vector<int>::iterator res =
    sml::sorting::counting_sort(
      seq, seq+7, dst.begin(), id_encoder_fun
    );

  ASSERT_EQ(dst.begin(), res);
  ASSERT_EQ(7,   dst.size());
  ASSERT_EQ(8,   dst[0]);
  ASSERT_EQ(21,  dst[1]);
  ASSERT_EQ(42,  dst[2]);
  ASSERT_EQ(50,  dst[3]);
  ASSERT_EQ(71,  dst[4]);
  ASSERT_EQ(88,  dst[5]);
  ASSERT_EQ(102, dst[6]);
}

TEST(CountingSort, InArrayOf2ndPower) {
  int seq[8] = {102, 50, 88, 71, 21, 8, 42, 91};
  int dst[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  int* res = sml::sorting::counting_sort(seq, seq+8, dst, id_encoder_fun);

  ASSERT_EQ(dst, res);
  ASSERT_EQ(8,   dst[0]);
  ASSERT_EQ(21,  dst[1]);
  ASSERT_EQ(42,  dst[2]);
  ASSERT_EQ(50,  dst[3]);
  ASSERT_EQ(71,  dst[4]);
  ASSERT_EQ(88,  dst[5]);
  ASSERT_EQ(91,  dst[6]);
  ASSERT_EQ(102, dst[7]);
}

TEST(CountingSort, InArrayOf2ndPowerMinus2) {
  int seq[6] = {102, 50, 88, 71, 21, 8};
  vector<int> dst(6);
  vector<int>::iterator res =
    sml::sorting::counting_sort(
      seq, seq+6, dst.begin(), id_encoder_fun
    );

  ASSERT_EQ(dst.begin(), res);
  ASSERT_EQ(6,   dst.size());
  ASSERT_EQ(8,   dst[0]);
  ASSERT_EQ(21,  dst[1]);
  ASSERT_EQ(50,  dst[2]);
  ASSERT_EQ(71,  dst[3]);
  ASSERT_EQ(88,  dst[4]);
  ASSERT_EQ(102, dst[5]);
}

TEST(CountingSort, InArrayOfMillion) {
  int seq[1000000];
  for (int i = 0; i < 1000000; ++i) {
    seq[i] = rand() % 1000;
  }
  int dst[1000000];
  int* res =
    sml::sorting::counting_sort(
      seq, seq+1000000, dst, id_encoder_fun
    );

  ASSERT_EQ(dst, res);

  int prev = -1;
  for (int i = 0; i < 1000000; ++i) {
    ASSERT_LE(prev, dst[i]);
    prev = dst[i];
  }
}

size_t pair_encoder(pair<int, int> p) {
  return static_cast<size_t>(p.first);
}
TEST(CountingSort, Stable) {
  pair<int, int> seq[5] = {
    make_pair(26, 5), make_pair(10, 1),
    make_pair(5, 2),  make_pair(10, 3), make_pair(26, 0)
  };
  vector< pair<int, int> > dst(5);
  vector< pair<int, int> >::iterator res =
    sml::sorting::counting_sort(
      seq, seq+5, dst.begin(), pair_encoder
    );

  ASSERT_EQ(dst.begin(), res);
  ASSERT_EQ(make_pair(5,  2), dst[0]);
  ASSERT_EQ(make_pair(10, 1), dst[1]);
  ASSERT_EQ(make_pair(10, 3), dst[2]);
  ASSERT_EQ(make_pair(26, 5), dst[3]);
  ASSERT_EQ(make_pair(26, 0), dst[4]);
}

} // namespace

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
