#include <functional>
#include <iterator>
#include <list>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <cstddef>
#include <gtest/gtest.h>
#include "sml/red_black_tree.hpp"

namespace {

using std::pair;
using std::make_pair;
using std::string;
using std::advance;
using std::list;
using std::vector;

using testing::StaticAssertTypeEq;

typedef sml::red_black_tree<string, int> map_type;
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
    this->map1.insert(make_pair("efg", 123));
    this->map1.insert(make_pair("abc", 456));
    this->map1.insert(make_pair("hij", 789));
    this->map1.insert(make_pair("klm", 100));

    this->values1.push_back(make_pair("abc", 456));
    this->values1.push_back(make_pair("efg", 123));
    this->values1.push_back(make_pair("hij", 789));
    this->values1.push_back(make_pair("klm", 100));

    this->map2.insert(make_pair("saf", 831));
    this->map2.insert(make_pair("ljz", 193));
    this->map2.insert(make_pair("zdf", 371));
    this->map2.insert(make_pair("bam", 418));
    this->map2.insert(make_pair("fkd", 710));
    this->map2.insert(make_pair("pzj", 318));

    this->values2.push_back(make_pair("bam", 418));
    this->values2.push_back(make_pair("fkd", 710));
    this->values2.push_back(make_pair("ljz", 193));
    this->values2.push_back(make_pair("pzj", 318));
    this->values2.push_back(make_pair("saf", 831));
    this->values2.push_back(make_pair("zdf", 371));

    this->unordered_vs1.push_back(make_pair("hij", 789));
    this->unordered_vs1.push_back(make_pair("efg", 123));
    this->unordered_vs1.push_back(make_pair("klm", 100));
    this->unordered_vs1.push_back(make_pair("abc", 456));
  }

  map_type map1;
  value_list_type values1;

  map_type map2;
  value_list_type values2;

  value_list_type unordered_vs1;
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

    ASSERT_EQ(1,     cm.count(key)) << key;
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

  const_iterator cb = cm.begin();
  for (
    value_list_type::const_iterator it = vs.begin();
    it != vs.end();
    ++it, ++cb
  ) {
    ASSERT_NE(cm.end(),   cb);
    ASSERT_EQ(it->first,  cb->first);
    ASSERT_EQ(it->second, cb->second);
  }
  ASSERT_EQ(cm.end(), cb);

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

  const_reverse_iterator crb = cm.rbegin();
  for (
    value_list_type::const_reverse_iterator it = vs.rbegin();
    it != vs.rend();
    ++it, ++crb
  ) {
    ASSERT_NE(cm.rend(),  crb);
    ASSERT_EQ(it->first,  crb->first);
    ASSERT_EQ(it->second, crb->second);
  }
  ASSERT_EQ(cm.rend(), crb);

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
  map_type m;
  m.insert(this->unordered_vs1.begin(), this->unordered_vs1.end());

  status_test(m, this->values1);
}

TEST_F(RedBlackTreeFixture, InsertUnorderedElementSequence) {
  map_type m;
  m.insert(make_pair("a", 1));
  m.insert(make_pair("z", 999));
  m.insert(this->unordered_vs1.begin(), this->unordered_vs1.end());

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
    SCOPED_TRACE("MediumElement");

    map_type m = this->map1;
    m.insert(m.find("efg"), make_pair("e", 1));

    value_list_type vs = this->values1;
    vs.insert(vs.begin()+1, make_pair("e", 1));

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
    SCOPED_TRACE("MediumElement");

    map_type m = this->map1;
    m.insert(++m.find("efg"), make_pair("e", 1));

    value_list_type vs = this->values1;
    vs.insert(vs.begin()+1, make_pair("e", 1));

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
    value_list_type::iterator it = this->values1.begin();
    it != this->values1.end();
    ++it
  ) {
    SCOPED_TRACE("By " + it->first);

    map_type m = this->map1;
    m.erase(m.find(it->first));

    value_list_type vs = this->values1;
    vs.erase(vs.begin() + (it - this->values1.begin()));

    status_test(m, vs);
  }
}

TEST_F(RedBlackTreeFixture, Erase2) {
  for (
       value_list_type::iterator it = this->values1.begin();
       it != this->values1.end();
       ++it
  ) {
    SCOPED_TRACE("By " + it->first);

    map_type m = this->map1;
    size_t res = m.erase(it->first);
    ASSERT_EQ(1, res);

    value_list_type vs = this->values1;
    vs.erase(vs.begin() + (it - this->values1.begin()));

    status_test(m, vs);
  }
}

TEST_F(RedBlackTreeFixture, Erase3) {
  for (size_type i = 0; i <= this->values1.size(); ++i) {
    for (size_type j = i; j <= this->values1.size(); ++j) {
      map_type m = this->map1;
      iterator b = m.begin(), e = m.begin();
      advance(b, i); advance(e, j);

      m.erase(b, e);

      value_list_type vs = this->values1;
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
  map_type m = this->map1;
  m.clear();

  status_test(m, value_list_type());
}

TEST(RedBlackTreeType, TypesInMap) {
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

TEST(RedBlackTreeType, TypesInIterator) {
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
