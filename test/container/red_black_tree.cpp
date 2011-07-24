#include <algorithm>
#include <functional>
#include <iterator>
#include <list>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <cstddef>
#include <gtest/gtest.h>
#include "sml/container/red_black_tree.hpp"

namespace {

using std::pair;
using std::make_pair;
using std::string;
using std::advance;
using std::list;
using std::random_shuffle;
using std::vector;

using testing::StaticAssertTypeEq;

typedef sml::container::red_black_tree<string, int> map_type;
typedef map_type::iterator               iterator;
typedef map_type::const_iterator         const_iterator;
typedef map_type::reverse_iterator       reverse_iterator;
typedef map_type::const_reverse_iterator const_reverse_iterator;
typedef map_type::value_type             value_type;
typedef map_type::key_compare            key_compare;
typedef map_type::value_compare          value_compare;
typedef map_type::size_type              size_type;

typedef pair<iterator, bool> insert_result_type;
typedef vector< pair<string, int> > value_list_type;

class RedBlackTreeFixture : public testing::Test {
protected:
  void SetUp() {
    {
      this->map1.insert(make_pair("efg", 123));
      this->map1.insert(make_pair("tfs", 498));
      this->map1.insert(make_pair("abc", 456));
      this->map1.insert(make_pair("hij", 789));
      this->map1.insert(make_pair("vsd", 821));
      this->map1.insert(make_pair("klm", 100));

      this->values1.push_back(make_pair("abc", 456));
      this->values1.push_back(make_pair("efg", 123));
      this->values1.push_back(make_pair("hij", 789));
      this->values1.push_back(make_pair("klm", 100));
      this->values1.push_back(make_pair("tfs", 498));
      this->values1.push_back(make_pair("vsd", 821));
    }

    {
      this->map2.insert(make_pair("baa", 101));
      this->map2.insert(make_pair("bab", 102));
      this->map2.insert(make_pair("bac", 103));
      this->map2.insert(make_pair("bad", 104));
      this->map2.insert(make_pair("bae", 105));
      this->map2.insert(make_pair("baf", 106));
      this->map2.insert(make_pair("bag", 107));
      this->map2.insert(make_pair("bah", 108));
      this->map2.insert(make_pair("bai", 109));
      this->map2.insert(make_pair("baj", 110));
      this->map2.insert(make_pair("bak", 111));
      this->map2.insert(make_pair("bal", 112));
      this->map2.insert(make_pair("bam", 113));
      this->map2.insert(make_pair("ban", 114));
      this->map2.insert(make_pair("bao", 115));
      this->map2.insert(make_pair("caa", 201));
      this->map2.insert(make_pair("cab", 202));
      this->map2.insert(make_pair("cac", 203));
      this->map2.insert(make_pair("cad", 204));
      this->map2.insert(make_pair("cae", 205));
      this->map2.insert(make_pair("caf", 206));
      this->map2.insert(make_pair("cag", 207));
      this->map2.insert(make_pair("cah", 208));
      this->map2.insert(make_pair("cai", 209));
      this->map2.insert(make_pair("caj", 210));
      this->map2.insert(make_pair("cak", 211));
      this->map2.insert(make_pair("cal", 212));
      this->map2.insert(make_pair("cam", 213));
      this->map2.insert(make_pair("can", 214));
      this->map2.insert(make_pair("cao", 215));
      this->map2.insert(make_pair("aaa",   1));
      this->map2.insert(make_pair("aab",   2));
      this->map2.insert(make_pair("aac",   3));
      this->map2.insert(make_pair("aad",   4));
      this->map2.insert(make_pair("aae",   5));
      this->map2.insert(make_pair("aaf",   6));
      this->map2.insert(make_pair("aag",   7));
      this->map2.insert(make_pair("aah",   8));
      this->map2.insert(make_pair("aai",   9));
      this->map2.insert(make_pair("aaj",  10));
      this->map2.insert(make_pair("aak",  11));
      this->map2.insert(make_pair("aal",  12));
      this->map2.insert(make_pair("aam",  13));
      this->map2.insert(make_pair("aan",  14));
      this->map2.insert(make_pair("aao",  15));

      this->values2.push_back(make_pair("aaa",   1));
      this->values2.push_back(make_pair("aab",   2));
      this->values2.push_back(make_pair("aac",   3));
      this->values2.push_back(make_pair("aad",   4));
      this->values2.push_back(make_pair("aae",   5));
      this->values2.push_back(make_pair("aaf",   6));
      this->values2.push_back(make_pair("aag",   7));
      this->values2.push_back(make_pair("aah",   8));
      this->values2.push_back(make_pair("aai",   9));
      this->values2.push_back(make_pair("aaj",  10));
      this->values2.push_back(make_pair("aak",  11));
      this->values2.push_back(make_pair("aal",  12));
      this->values2.push_back(make_pair("aam",  13));
      this->values2.push_back(make_pair("aan",  14));
      this->values2.push_back(make_pair("aao",  15));
      this->values2.push_back(make_pair("baa", 101));
      this->values2.push_back(make_pair("bab", 102));
      this->values2.push_back(make_pair("bac", 103));
      this->values2.push_back(make_pair("bad", 104));
      this->values2.push_back(make_pair("bae", 105));
      this->values2.push_back(make_pair("baf", 106));
      this->values2.push_back(make_pair("bag", 107));
      this->values2.push_back(make_pair("bah", 108));
      this->values2.push_back(make_pair("bai", 109));
      this->values2.push_back(make_pair("baj", 110));
      this->values2.push_back(make_pair("bak", 111));
      this->values2.push_back(make_pair("bal", 112));
      this->values2.push_back(make_pair("bam", 113));
      this->values2.push_back(make_pair("ban", 114));
      this->values2.push_back(make_pair("bao", 115));
      this->values2.push_back(make_pair("caa", 201));
      this->values2.push_back(make_pair("cab", 202));
      this->values2.push_back(make_pair("cac", 203));
      this->values2.push_back(make_pair("cad", 204));
      this->values2.push_back(make_pair("cae", 205));
      this->values2.push_back(make_pair("caf", 206));
      this->values2.push_back(make_pair("cag", 207));
      this->values2.push_back(make_pair("cah", 208));
      this->values2.push_back(make_pair("cai", 209));
      this->values2.push_back(make_pair("caj", 210));
      this->values2.push_back(make_pair("cak", 211));
      this->values2.push_back(make_pair("cal", 212));
      this->values2.push_back(make_pair("cam", 213));
      this->values2.push_back(make_pair("can", 214));
      this->values2.push_back(make_pair("cao", 215));
    }
  }

  map_type map1;
  value_list_type values1;

  map_type map2;
  value_list_type values2;
};

class RedBlackTreeCompare : public testing::Test {
protected:
  void SetUp() {
    this->map.insert(make_pair("efg", 123));
    this->map.insert(make_pair("tfs", 498));
    this->map.insert(make_pair("abc", 456));

    this->equal.insert(make_pair("tfs", 498));
    this->equal.insert(make_pair("abc", 456));
    this->equal.insert(make_pair("efg", 123));

    this->lesser.insert(make_pair("sfs", 498));
    this->lesser.insert(make_pair("abc", 456));
    this->lesser.insert(make_pair("efg", 123));

    this->greater.insert(make_pair("tfs", 498));
    this->greater.insert(make_pair("abc", 456));
    this->greater.insert(make_pair("efg", 223));
  }

  map_type map;
  map_type equal;
  map_type lesser;
  map_type greater;
};

void key_compare_test(const map_type& m) {
  key_compare kcmp = m.key_comp();
  ASSERT_TRUE ( kcmp("abc", "efg") );
  ASSERT_FALSE( kcmp("efg", "abc") );
  ASSERT_FALSE( kcmp("abc", "abc") );
}

void value_compare_test(const map_type& m) {
  value_compare vcmp = m.value_comp();
  ASSERT_TRUE ( vcmp(make_pair("abc", 123), make_pair("efg",  23)) );
  ASSERT_FALSE( vcmp(make_pair("efg", 123), make_pair("abc", 456)) );
  ASSERT_FALSE( vcmp(make_pair("abc", 123), make_pair("abc", 456)) );
}

void allocator_test(const map_type& m) {
  ASSERT_EQ(map_type().max_size(),      m.max_size());
  ASSERT_EQ(map_type().get_allocator(), m.get_allocator());
}

void const_iterator_test(
  map_type& m,
  const value_list_type& vs,
  const_iterator cb
) {
  const map_type& cm = static_cast<const map_type&>(m);

  for (
       value_list_type::const_iterator it = vs.begin();
       it != vs.end();
       ++it, ++cb
  ) {
    ASSERT_NE(cm.end(),   cb);
    ASSERT_NE(cm.cend(),  cb);
    ASSERT_NE(m.cend(),   cb);

    ASSERT_EQ(it->first,  cb->first);
    ASSERT_EQ(it->second, cb->second);
  }

  ASSERT_EQ(cm.end(),  cb);
  ASSERT_EQ(cm.cend(), cb);
  ASSERT_EQ(m.cend(),  cb);
}

void const_reverse_iterator_test(
  map_type& m,
  const value_list_type& vs,
  const_reverse_iterator crb
) {
  const map_type& cm = static_cast<const map_type&>(m);

  for (
       value_list_type::const_reverse_iterator it = vs.rbegin();
       it != vs.rend();
       ++it, ++crb
  ) {
    ASSERT_NE(cm.rend(),  crb);
    ASSERT_NE(cm.crend(), crb);
    ASSERT_NE(m.crend(),  crb);

    ASSERT_EQ(it->first,  crb->first);
    ASSERT_EQ(it->second, crb->second);
  }

  ASSERT_EQ(cm.rend(),  crb);
  ASSERT_EQ(cm.crend(), crb);
  ASSERT_EQ(m.crend(),  crb);
}

void status_test(map_type& m, const value_list_type& vs) {
  const map_type& cm = static_cast<const map_type&>(m);

  ASSERT_EQ(!vs.size(), cm.empty());
  ASSERT_EQ(vs.size(),  cm.size());

  for (value_list_type::const_iterator it = vs.begin(); it != vs.end(); ++it) {
    string key = it->first;
    int value = it->second;

    iterator pos  = m.begin(); advance(pos, it - vs.begin());
    iterator next = pos; ++next;
    const_iterator cpos  = cm.begin(); advance(cpos, it - vs.begin());
    const_iterator cnext = cpos; ++cnext;

    ASSERT_EQ(1,     cm.count(key));
    ASSERT_EQ(value, m[key]);

    ASSERT_EQ(pos,   m.find(key));
    ASSERT_EQ(cpos,  cm.find(key));
    ASSERT_EQ(pos,   m.lower_bound(key));
    ASSERT_EQ(cpos,  cm.lower_bound(key));
    ASSERT_EQ(next,  m.upper_bound(key));
    ASSERT_EQ(cnext, cm.upper_bound(key));

    pair<iterator, iterator> eq_range = m.equal_range(key);
    ASSERT_EQ(pos,   eq_range.first);
    ASSERT_EQ(next,  eq_range.second);

    pair<const_iterator, const_iterator> ceq_range = cm.equal_range(key);
    ASSERT_EQ(cpos,  ceq_range.first);
    ASSERT_EQ(cnext, ceq_range.second);
  }

  {
    iterator l1 = m.lower_bound("");
    ASSERT_EQ(m.begin(), l1);

    iterator l2 = m.lower_bound("zzzz");
    ASSERT_EQ(m.end(), l2);

    const_iterator cl1 = cm.lower_bound("");
    ASSERT_EQ(cm.begin(), cl1);

    const_iterator cl2 = cm.lower_bound("zzzz");
    ASSERT_EQ(cm.end(), cl2);
  }

  {
    iterator u1 = m.upper_bound("");
    ASSERT_EQ(m.begin(), u1);

    iterator u2 = m.upper_bound("zzzz");
    ASSERT_EQ(m.end(), u2);

    const_iterator cu1 = cm.upper_bound("");
    ASSERT_EQ(cm.begin(), cu1);

    const_iterator cu2 = cm.upper_bound("zzzz");
    ASSERT_EQ(cm.end(), cu2);
  }

  {
    pair<iterator, iterator> e1 = m.equal_range("");
    ASSERT_EQ(m.begin(), e1.first);
    ASSERT_EQ(m.begin(), e1.second);

    pair<iterator, iterator> e2 = m.equal_range("zzzz");
    ASSERT_EQ(m.end(), e2.first);
    ASSERT_EQ(m.end(), e2.second);

    pair<const_iterator, const_iterator> ce1 = cm.equal_range("");
    ASSERT_EQ(cm.begin(), ce1.first);
    ASSERT_EQ(cm.begin(), ce1.second);

    pair<const_iterator, const_iterator> ce2 = cm.equal_range("zzzz");
    ASSERT_EQ(cm.end(), ce2.first);
    ASSERT_EQ(cm.end(), ce2.second);
  }

  iterator b = m.begin();
  for (
    value_list_type::const_iterator it = vs.begin();
    it != vs.end();
    ++it, ++b
  ) {
    ASSERT_NE(m.end(),    b);
    ASSERT_EQ(it->first,  b->first);
    ASSERT_EQ(it->second, b->second);
  }
  ASSERT_EQ(m.end(), b);

  const_iterator_test(m, vs, cm.begin());
  const_iterator_test(m, vs, cm.cbegin());
  const_iterator_test(m, vs, m.cbegin());

  reverse_iterator rb = m.rbegin();
  for (
    value_list_type::const_reverse_iterator it = vs.rbegin();
    it != vs.rend();
    ++it, ++rb
  ) {
    ASSERT_NE(m.rend(),   rb);
    ASSERT_EQ(it->first,  rb->first);
    ASSERT_EQ(it->second, rb->second);
  }
  ASSERT_EQ(m.rend(), rb);

  const_reverse_iterator_test(m, vs, cm.rbegin());
  const_reverse_iterator_test(m, vs, cm.crbegin());
  const_reverse_iterator_test(m, vs, m.crbegin());

  key_compare_test(m);
  value_compare_test(m);
  allocator_test(m);
}

TEST(RedBlackTree, Constructor) {
  map_type m;

  status_test(m, value_list_type());
}

TEST_F(RedBlackTreeFixture, CopyConstructor) {
  map_type m(this->map1);

  value_list_type vs = this->values1;

  status_test(m, vs);

  m.insert(make_pair("a", 100));
  vs.insert(vs.begin(), make_pair("a", 100));

  status_test(m, vs);
  status_test(this->map1, this->values1);
}

TEST_F(RedBlackTreeFixture, RangeConstructor) {
  {
    map_type m(this->map1.begin(), this->map1.end());

    value_list_type vs = this->values1;

    status_test(m, vs);

    m.insert(make_pair("a", 100));
    vs.insert(vs.begin(), make_pair("a", 100));

    status_test(m, vs);
    status_test(this->map1, this->values1);
  }

  {
    map_type m(this->map1.begin(), --this->map1.end());

    value_list_type vs = this->values1;
    vs.erase(--vs.end());

    status_test(m, vs);

    m.insert(make_pair("a", 100));
    vs.insert(vs.begin(), make_pair("a", 100));

    status_test(m, vs);
    status_test(this->map1, this->values1);
  }
}

TEST(RedBlackTree, InsertToEmpty) {
  map_type m;

  insert_result_type res = m.insert(make_pair("abc", 123));

  ASSERT_EQ(m.begin(), res.first);
  ASSERT_TRUE(res.second);
  ASSERT_EQ("abc", res.first->first);
  ASSERT_EQ(123,   res.first->second);

  value_list_type vs;
  vs.push_back(make_pair("abc", 123));

  status_test(m, vs);
}


TEST(RedBlackTree, InsertLowestElement) {
  map_type m;

  m.insert(make_pair("efg", 123));
  insert_result_type res = m.insert(make_pair("abc", 456));

  ASSERT_EQ(m.begin(), res.first);
  ASSERT_TRUE(res.second);
  ASSERT_EQ("abc", res.first->first);
  ASSERT_EQ(456,   res.first->second);

  value_list_type vs;
  vs.push_back(make_pair("abc", 456));
  vs.push_back(make_pair("efg", 123));

  status_test(m, vs);
}

TEST(RedBlackTree, InsertLargestElement) {
  map_type m;

  m.insert(make_pair("abc", 456));
  insert_result_type res = m.insert(make_pair("efg", 123));

  ASSERT_EQ(++m.begin(), res.first);
  ASSERT_TRUE(res.second);
  ASSERT_EQ("efg", res.first->first);
  ASSERT_EQ(123,   res.first->second);

  value_list_type vs;
  vs.push_back(make_pair("abc", 456));
  vs.push_back(make_pair("efg", 123));

  status_test(m, vs);
}

TEST_F(RedBlackTreeFixture, InsertElements) {
  status_test(this->map1, this->values1);
  status_test(this->map2, this->values2);
}

TEST_F(RedBlackTreeFixture, InsertDupElements) {
  this->map1.insert(make_pair("efg", 987));
  this->values1[1] = make_pair("efg", 987);
  status_test(this->map1, this->values1);

  this->map1.insert(make_pair("klm", 853));
  this->values1[3] = make_pair("klm", 853);
  status_test(this->map1, this->values1);
}

TEST_F(RedBlackTreeFixture, InsertLowestDupElements) {
  this->map1.insert(make_pair("abc", 987));
  this->values1[0] = make_pair("abc", 987);
  status_test(this->map1, this->values1);
}

TEST_F(RedBlackTreeFixture, InsertLargestDupElements) {
  this->map1.insert(make_pair("vsd", 382));
  this->values1[5] = make_pair("vsd", 382);
  status_test(this->map1, this->values1);
}

TEST_F(RedBlackTreeFixture, InsertOrderedElementSequenceToEmpty) {
  map_type m;
  m.insert(this->map1.begin(), this->map1.end());

  status_test(m, this->values1);
}

TEST_F(RedBlackTreeFixture, InsertOrderedElementSequence) {
  map_type m;
  m.insert(make_pair("a", 1));
  m.insert(make_pair("z", 999));
  m.insert(this->map1.begin(), this->map1.end());

  value_list_type vs(this->values1);
  vs.insert(vs.begin(), make_pair("a", 1));
  vs.push_back (make_pair("z", 999));

  status_test(m, vs);
}

TEST_F(RedBlackTreeFixture, InsertUnorderedElementSequenceToEmpty) {
  value_list_type unordered(this->values1);
  random_shuffle(unordered.begin(), unordered.end());

  map_type m;
  m.insert(unordered.begin(), unordered.end());

  status_test(m, this->values1);
}

TEST_F(RedBlackTreeFixture, InsertUnorderedElementSequence) {
  value_list_type unordered(this->values1);
  random_shuffle(unordered.begin(), unordered.end());

  map_type m;
  m.insert(make_pair("a", 1));
  m.insert(make_pair("z", 999));
  m.insert(unordered.begin(), unordered.end());

  value_list_type vs(this->values1);
  vs.insert(vs.begin(), make_pair("a", 1));
  vs.push_back (make_pair("z", 999));

  status_test(m, vs);
}

TEST(RedBlackTree, InsertByPositionToEmpty) {
  map_type m;
  m.insert(m.begin(), make_pair("abc", 123));

  value_list_type vs;
  vs.push_back(make_pair("abc", 123));

  status_test(m, vs);
}

TEST_F(RedBlackTreeFixture, InsertCloseLowerElementByPosition) {
  {
    SCOPED_TRACE("LowestElement");

    map_type m = this->map1;
    m.insert(m.find("abc"), make_pair("a", 1));

    value_list_type vs = this->values1;
    vs.insert(vs.begin(), make_pair("a", 1));

    status_test(m, vs);
  }

  {
    SCOPED_TRACE("MediumElement1");

    map_type m = this->map1;
    m.insert(m.find("efg"), make_pair("e", 1));

    value_list_type vs = this->values1;
    vs.insert(vs.begin()+1, make_pair("e", 1));

    status_test(m, vs);
  }

  {
    SCOPED_TRACE("MediumElement2");

    map_type m = this->map1;
    m.insert(m.find("vsd"), make_pair("u", 2));

    value_list_type vs = this->values1;
    vs.insert(vs.begin()+5, make_pair("u", 2));

    status_test(m, vs);
  }

  {
    SCOPED_TRACE("LargestElement");

    map_type m = this->map1;
    m.insert(m.end(), make_pair("z", 999));

    value_list_type vs = this->values1;
    vs.push_back(make_pair("z", 999));

    status_test(m, vs);
  }
}

TEST_F(RedBlackTreeFixture, InsertByPosition) {
  {
    SCOPED_TRACE("LowestElement");

    map_type m = this->map1;
    m.insert(m.find("abc"), make_pair("a", 1));

    value_list_type vs = this->values1;
    vs.insert(vs.begin(), make_pair("a", 1));

    status_test(m, vs);
  }

  {
    SCOPED_TRACE("MediumElement1");

    map_type m = this->map1;
    m.insert(++m.find("efg"), make_pair("e", 1));

    value_list_type vs = this->values1;
    vs.insert(vs.begin()+1, make_pair("e", 1));

    status_test(m, vs);
  }

  {
    SCOPED_TRACE("MediumElement1");

    map_type m = this->map1;
    m.insert(m.find("vsd"), make_pair("u", 2));

    value_list_type vs = this->values1;
    vs.insert(vs.begin()+5, make_pair("u", 2));

    status_test(m, vs);
  }

  {
    SCOPED_TRACE("LargestElement");

    map_type m = this->map1;
    m.insert(m.begin(), make_pair("z", 999));

    value_list_type vs = this->values1;
    vs.push_back(make_pair("z", 999));

    status_test(m, vs);
  }
}

TEST(RedBlackTree, AcessToEmpty) {
  map_type m;

  int res = m["abc"];
  ASSERT_EQ(int(), res);

  m["abc"] = 123;
  ASSERT_EQ(123, m["abc"]);

  value_list_type vs;
  vs.push_back(make_pair("abc", 123));

  status_test(m, vs);
}

TEST(RedBlackTree, AccessByLowestElement) {
  map_type m;
  m["efg"] = 123;

  int res = m["abc"];
  ASSERT_EQ(int(), res);

  m["abc"] = 456;
  ASSERT_EQ(456, m["abc"]);

  value_list_type vs;
  vs.push_back(make_pair("abc", 456));
  vs.push_back(make_pair("efg", 123));

  status_test(m, vs);
}

TEST(RedBlackTree, AccessByLargestElement) {
  map_type m;
  m["abc"] = 456;

  int res = m["efg"];
  ASSERT_EQ(int(), res);

  m["efg"] = 123;
  ASSERT_EQ(123, m["efg"]);

  value_list_type vs;
  vs.push_back(make_pair("abc", 456));
  vs.push_back(make_pair("efg", 123));

  status_test(m, vs);
}

TEST(RedBlackTree, AccessByElements) {
  map_type m;
  m["efg"] = 123;
  m["abc"] = 456;
  m["hij"] = 789;
  m["klm"] = 100;

  value_list_type vs;
  vs.push_back(make_pair("abc", 456));
  vs.push_back(make_pair("efg", 123));
  vs.push_back(make_pair("hij", 789));
  vs.push_back(make_pair("klm", 100));

  status_test(m, vs);
}

TEST_F(RedBlackTreeFixture, AccessByDupElement) {
  this->map1["efg"] = 987;
  this->values1[1] = make_pair("efg", 987);
  status_test(this->map1, this->values1);

  this->map1["klm"] = 853;
  this->values1[3] = make_pair("klm", 853);
  status_test(this->map1, this->values1);
}

TEST_F(RedBlackTreeFixture, AccessByLowestDupElement) {
  this->map1["abc"] = 987;
  this->values1[0] = make_pair("abc", 987);
  status_test(this->map1, this->values1);
}

TEST_F(RedBlackTreeFixture, AccessByLargestDupElement) {
  this->map1["vsd"] = 382;
  this->values1[5] = make_pair("vsd", 382);
  status_test(this->map1, this->values1);
}

TEST(RedBlackTree, EraseFromEmpty) {
  {
    SCOPED_TRACE("ByPosition");

    map_type m;
    m.erase(m.begin());

    value_list_type vs;
    status_test(m, value_list_type());
  }

  {
    SCOPED_TRACE("ByKey");

    map_type m;
    size_type res = m.erase("abc");
    ASSERT_EQ(0, res);

    value_list_type vs;
    status_test(m, vs);
  }

  {
    SCOPED_TRACE("ByRange");

    map_type m;
    m.erase(m.begin(), m.end());

    value_list_type vs;
    status_test(m, vs);
  }
}

TEST_F(RedBlackTreeFixture, Erase1) {
  for (
    value_list_type::iterator it = this->values2.begin();
    it != this->values2.end();
    ++it
  ) {
    SCOPED_TRACE("By " + it->first);

    map_type m = this->map2;
    m.erase(m.find(it->first));

    value_list_type vs = this->values2;
    vs.erase(vs.begin() + (it - this->values2.begin()));

    status_test(m, vs);
  }
}

TEST_F(RedBlackTreeFixture, Erase2) {
  for (
       value_list_type::iterator it = this->values2.begin();
       it != this->values2.end();
       ++it
  ) {
    SCOPED_TRACE("By " + it->first);

    map_type m = this->map2;
    size_t res = m.erase(it->first);
    ASSERT_EQ(1, res);

    value_list_type vs = this->values2;
    vs.erase(vs.begin() + (it - this->values2.begin()));

    status_test(m, vs);
  }
}

TEST_F(RedBlackTreeFixture, Erase3) {
  for (size_type i = 0; i <= this->values2.size(); ++i) {
    for (size_type j = i; j <= this->values2.size(); ++j) {
      map_type m = this->map2;
      iterator b = m.begin(), e = m.begin();
      advance(b, i); advance(e, j);

      m.erase(b, e);

      value_list_type vs = this->values2;
      vs.erase(vs.begin()+i, vs.begin()+j);

      status_test(m, vs);
    }
  }
}

TEST_F(RedBlackTreeFixture, Assign) {
  map_type m;
  m = this->map1;

  value_list_type vs = this->values1;

  status_test(m, vs);

  m.insert(make_pair("a", 100));
  vs.insert(vs.begin(), make_pair("a", 100));

  status_test(m, vs);
  status_test(this->map1, this->values1);
}

TEST_F(RedBlackTreeFixture, Swap) {
  map_type m1 = this->map1;
  value_list_type vs1 = this->values1;

  map_type m2 = this->map2;
  value_list_type vs2 = this->values2;

  {
    m1.swap(m2);

    status_test(m1, vs2);
    status_test(m2, vs1);
  }

  {
    m1.insert(make_pair("a", 100));
    vs2.insert(vs2.begin(), make_pair("a", 100));

    m2.erase("efg");
    vs1.erase(++vs1.begin());

    status_test(m1, vs2);
    status_test(m2, vs1);
  }

  {
    swap(m1, m2);

    status_test(m1, vs1);
    status_test(m2, vs2);
  }
}

TEST(RedBlackTree, ClearInEmpty) {
  map_type m;
  m.clear();

  status_test(m, value_list_type());
}

TEST_F(RedBlackTreeFixture, Clear) {
  this->map1.clear();

  status_test(this->map1, value_list_type());
}

TEST_F(RedBlackTreeCompare, Equal) {
  ASSERT_TRUE(map_type()     == map_type());
  ASSERT_TRUE(this->map      == this->map);

  ASSERT_FALSE(map_type()    == this->map);
  ASSERT_FALSE(this->map     == map_type());

  ASSERT_TRUE(this->map      == this->equal);
  ASSERT_TRUE(this->equal    == this->map);

  ASSERT_FALSE(this->map     == this->lesser);
  ASSERT_FALSE(this->lesser  == this->map);

  ASSERT_FALSE(this->map     == this->greater);
  ASSERT_FALSE(this->greater == this->map);
}

TEST_F(RedBlackTreeCompare, NotEqual) {
  ASSERT_FALSE(map_type()   != map_type());
  ASSERT_FALSE(this->map    != this->map);

  ASSERT_TRUE(map_type()    != this->map);
  ASSERT_TRUE(this->map     != map_type());

  ASSERT_FALSE(this->map    != this->equal);
  ASSERT_FALSE(this->equal  != this->map);

  ASSERT_TRUE(this->map     != this->lesser);
  ASSERT_TRUE(this->lesser  != this->map);

  ASSERT_TRUE(this->map     != this->greater);
  ASSERT_TRUE(this->greater != this->map);
}

TEST_F(RedBlackTreeCompare, Lesser) {
  ASSERT_FALSE(map_type()    < map_type());
  ASSERT_FALSE(this->map     < this->map);

  ASSERT_TRUE(map_type()     < this->map);
  ASSERT_FALSE(this->map     < map_type());

  ASSERT_FALSE(this->map     < this->equal);
  ASSERT_FALSE(this->equal   < this->map);

  ASSERT_FALSE(this->map     < this->lesser);
  ASSERT_TRUE(this->lesser   < this->map);

  ASSERT_TRUE(this->map      < this->greater);
  ASSERT_FALSE(this->greater < this->map);
}

TEST_F(RedBlackTreeCompare, LesserOrEqual) {
  ASSERT_TRUE(map_type()     <= map_type());
  ASSERT_TRUE(this->map      <= this->map);

  ASSERT_TRUE(map_type()     <= this->map);
  ASSERT_FALSE(this->map     <= map_type());

  ASSERT_TRUE(this->map      <= this->equal);
  ASSERT_TRUE(this->equal    <= this->map);

  ASSERT_FALSE(this->map     <= this->lesser);
  ASSERT_TRUE(this->lesser   <= this->map);

  ASSERT_TRUE(this->map      <= this->greater);
  ASSERT_FALSE(this->greater <= this->map);
}

TEST_F(RedBlackTreeCompare, Greater) {
  ASSERT_FALSE(map_type()   > map_type());
  ASSERT_FALSE(this->map    > this->map);

  ASSERT_FALSE(map_type()   > this->map);
  ASSERT_TRUE(this->map     > map_type());

  ASSERT_FALSE(this->map    > this->equal);
  ASSERT_FALSE(this->equal  > this->map);

  ASSERT_TRUE(this->map     > this->lesser);
  ASSERT_FALSE(this->lesser > this->map);

  ASSERT_FALSE(this->map    > this->greater);
  ASSERT_TRUE(this->greater > this->map);
}

TEST_F(RedBlackTreeCompare, GreaterOrEqual) {
  ASSERT_TRUE(map_type()    >= map_type());
  ASSERT_TRUE(this->map     >= this->map);

  ASSERT_FALSE(map_type()   >= this->map);
  ASSERT_TRUE(this->map     >= map_type());

  ASSERT_TRUE(this->map     >= this->equal);
  ASSERT_TRUE(this->equal   >= this->map);

  ASSERT_TRUE(this->map     >= this->lesser);
  ASSERT_FALSE(this->lesser >= this->map);

  ASSERT_FALSE(this->map    >= this->greater);
  ASSERT_TRUE(this->greater >= this->map);
}

TEST(RedBlackTree, Types) {
  typedef std::allocator< pair<const std::string, int> > allocator_type;

  StaticAssertTypeEq<std::string,               map_type::key_type>();
  StaticAssertTypeEq<int,                       map_type::mapped_type>();
  StaticAssertTypeEq<std::less<std::string>,    map_type::key_compare>();
  StaticAssertTypeEq<map_type::value_compare,   map_type::value_compare>();
  StaticAssertTypeEq<allocator_type,            map_type::allocator_type>();
  StaticAssertTypeEq<allocator_type::reference, map_type::reference>();
  StaticAssertTypeEq<allocator_type::pointer,   map_type::pointer>();
  StaticAssertTypeEq<map_type::iterator,        map_type::iterator>();
  StaticAssertTypeEq<map_type::const_iterator,  map_type::const_iterator>();
  StaticAssertTypeEq<map_type::size_type,       map_type::size_type>();
  StaticAssertTypeEq<map_type::difference_type, map_type::difference_type>();

  StaticAssertTypeEq<
    std::pair<const std::string, int>, map_type::value_type
  >();
  StaticAssertTypeEq<
    allocator_type::const_pointer, map_type::const_pointer
  >();
  StaticAssertTypeEq<
    allocator_type::const_reference, map_type::const_reference
  >();
  StaticAssertTypeEq<
    std::reverse_iterator<map_type::iterator>, map_type::reverse_iterator
  >();

  StaticAssertTypeEq<
    std::reverse_iterator<map_type::const_iterator>,
    map_type::const_reverse_iterator
  >();
}

TEST(RedBlackTree, TypesInIterator) {
  StaticAssertTypeEq<map_type::value_type,  iterator::value_type>();
  StaticAssertTypeEq<map_type::value_type*, iterator::pointer>();
  StaticAssertTypeEq<map_type::value_type&, iterator::reference>();
  StaticAssertTypeEq<std::ptrdiff_t,        iterator::difference_type>();
  StaticAssertTypeEq<
    std::bidirectional_iterator_tag, iterator::iterator_category
  >();

  StaticAssertTypeEq<const map_type::value_type,  const_iterator::value_type>();
  StaticAssertTypeEq<const map_type::value_type*, const_iterator::pointer>();
  StaticAssertTypeEq<const map_type::value_type&, const_iterator::reference>();
  StaticAssertTypeEq<
    std::ptrdiff_t, const_iterator::difference_type
  >();
  StaticAssertTypeEq<
    std::bidirectional_iterator_tag, iterator::iterator_category
  >();
}

} // namespace

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
