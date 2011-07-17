#include <algorithm>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <tr1/functional>
#include <cmath>
#include <gtest/gtest.h>
#include "sml/iterator/next.hpp"
#include "sml/container/chain_hash_map.hpp"

#include <iostream>
using namespace std;

namespace {

using std::advance;
using std::allocator;
using std::ceil;
using std::make_pair;
using std::out_of_range;
using std::pair;
using std::random_shuffle;
using std::string;
using std::vector;
using std::tr1::hash;

using sml::iterator::next;

typedef sml::container::chain_hash_map< string, int, hash<string> > map_type;
typedef map_type::key_type       key_type;
typedef map_type::mapped_type    mapped_type;
typedef map_type::value_type     value_type;
typedef map_type::key_equal      key_equal;
typedef map_type::hasher         hasher;
typedef map_type::allocator_type allocator_type;
typedef map_type::size_type      size_type;
typedef map_type::iterator       iterator;
typedef map_type::const_iterator const_iterator;
typedef map_type::local_iterator local_iterator;
typedef map_type::const_local_iterator const_local_iterator;

typedef vector< pair<string, int> > value_list_type;

class ChainHashMapFixiture : public testing::Test {
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

value_list_type::const_iterator find_in_values(
  value_list_type const& vs,
  key_type        const&  key
) {
  value_list_type::const_iterator pos = vs.begin();
  for (; pos != vs.end() && pos->first != key; ++pos);
  return pos;
}

void erase_value(value_list_type& vs, key_type const& key) {
  value_list_type::const_iterator cpos = find_in_values(vs, key);
  value_list_type::iterator pos = vs.begin() + (cpos - vs.begin());
  vs.erase(pos);
}

void erase_multiple_value(value_list_type& vs, iterator first, iterator last) {
  for (; first != last; ++first) {
    erase_value(vs, first->first);
  }
}

void key_equal_test(map_type const& cm) {
  key_equal keq = cm.key_eq();
  ASSERT_TRUE ( keq("abc", "abc") );
  ASSERT_FALSE( keq("abc", "efg") );
  ASSERT_FALSE( keq("efg", "abc") );
}

void hash_test(map_type const& cm) {
  hasher hash = cm.hash_function();
  ASSERT_EQ( hash("abc"), hash("abc") );
  ASSERT_EQ( hash("efg"), hash("efg") );
  ASSERT_NE( hash("abc"), hash("efg") );
}

void allocator_test(map_type const& cm) {
  ASSERT_EQ(map_type().max_size(),         cm.max_size());
  ASSERT_EQ(map_type().max_bucket_count(), cm.max_bucket_count());
  ASSERT_EQ(map_type().get_allocator(),    cm.get_allocator());
}

template<class Iterator>
void test_in_bucket(
  Iterator           first,
  Iterator  const    last,
  map_type  const&   cm,
  size_type const    idx,
  key_type  const&   key,
  mapped_type const& mapped
) {
  int count = 0;
  int sum   = 0;

  for (; first != last; ++first) {
    ++sum;
    // cout << "SUM: " << sum << endl;
    if (first->first == key) {
      ASSERT_EQ(mapped, first->second);
      ++count;
    }
  }

  ASSERT_EQ(1, count) << "key: "    << key << ", "
                      << "mapped: " << mapped;

  ASSERT_EQ(sum, cm.bucket_size(idx));
}

template<class Iterator>
void pointed_iterator_test(
  Iterator           pos,
  key_type const&    key,
  mapped_type const& mapped
) {
  ASSERT_EQ(key,                    pos->first);
  ASSERT_EQ(mapped,                 pos->second);
}

void bucket_test(map_type& m, key_type const& key, mapped_type const& mapped) {
  map_type const& cm = static_cast<map_type const&>(m);
  
  size_type idx = cm.bucket(key);
  ASSERT_LE(0,   idx);
  ASSERT_LT(idx, cm.bucket_count());
  ASSERT_LT(0,   cm.bucket_size(idx));

  test_in_bucket(m.begin(idx),   m.end(idx),   cm, idx, key, mapped);
  test_in_bucket(cm.begin(idx),  cm.end(idx),  cm, idx, key, mapped);
  test_in_bucket(cm.cbegin(idx), cm.cend(idx), cm, idx, key, mapped);
}

template<class Iterator>
void end_iterator_test(Iterator end, map_type& m) {
  map_type const& cm = static_cast<map_type const&>(m);

  ASSERT_EQ(m.end(),   end);
  ASSERT_EQ(cm.end(),  end);
  ASSERT_EQ(cm.cend(), end);
}

template<class Iterator>
void iterator_test(Iterator b, map_type& m, value_list_type const& vs) {
  map_type const& cm = static_cast<map_type const&>(m);
  int count = 0;

  for (; b != m.end(); ++b) {
    const_iterator cb = b;
    ASSERT_EQ(b,  cb);
    ASSERT_EQ(cb, b);

    ASSERT_NE(m.end(),      b);
    ASSERT_NE(cm.end(),     b);
    ASSERT_NE(cm.cend(),    b);

    value_list_type::const_iterator pos = find_in_values(vs, b->first);

    ASSERT_NE(vs.end(),    pos);
    ASSERT_EQ(pos->first,  b->first);
    ASSERT_EQ(pos->second, b->second);

    ++count;
  }

  ASSERT_EQ(count, m.size());

  const_iterator cb = b;
  ASSERT_EQ(b,  cb);
  ASSERT_EQ(cb, b);

  end_iterator_test(b, m);
}

void fail_count_test(map_type& m) {
  map_type const& cm = static_cast<map_type const&>(m);

  ASSERT_EQ(0, cm.count(""));
}

void fail_find_test(map_type& m) {
  map_type const& cm = static_cast<map_type const&>(m);

  end_iterator_test(m.find(""),  m);
  end_iterator_test(cm.find(""), m);
}

void fail_equal_range_test(map_type& m) {
  map_type const& cm = static_cast<map_type const&>(m);

  pair<iterator, iterator> range = m.equal_range("");
  end_iterator_test(range.first,  m);
  end_iterator_test(range.second, m);

  pair<const_iterator, const_iterator> crange = cm.equal_range("");
  end_iterator_test(crange.first,  m);
  end_iterator_test(crange.second, m);
}

void fail_at_test(map_type& m) {
  map_type const& cm = static_cast<map_type const&>(m);

  ASSERT_THROW(m.at(""),  out_of_range);
  ASSERT_THROW(cm.at(""), out_of_range);
}

void status_test(map_type& m, value_list_type const& vs) {
  map_type const& cm = static_cast<map_type const&>(m);

  ASSERT_EQ(!vs.size(), cm.empty());
  ASSERT_EQ(vs.size(),  cm.size());

  ASSERT_LT(0,    cm.bucket_count());

  if (cm.size()) ASSERT_LT(0.0f, cm.load_factor());
  else           ASSERT_LE(0.0f, cm.load_factor());
    
  ASSERT_LE(cm.load_factor(), cm.max_load_factor());
  ASSERT_FLOAT_EQ(
    static_cast<float>(cm.size()) / static_cast<float>(cm.bucket_count()),
    cm.load_factor()
  );

  for (
    value_list_type::const_iterator iter = vs.begin();
    iter != vs.end();
    ++iter
  ) {
    key_type    key    = iter->first;
    mapped_type mapped = iter->second;

    ASSERT_EQ(1, cm.count(key)) << key;

    ASSERT_EQ(mapped, m[key]);
    ASSERT_EQ(mapped, m.at(key));
    ASSERT_EQ(mapped, cm.at(key));

    bucket_test(m, key, mapped);

    iterator pos = m.find(key);
    const_iterator cpos = pos;

    pointed_iterator_test(pos,  key, mapped);
    pointed_iterator_test(cpos, key, mapped);

    iterator       next  = pos; ++next;
    const_iterator cnext = next;

    pair<iterator, iterator> eq_range = m.equal_range(key);

    // cout << "EqualRange: ";
    // cout << (next == cm.end()) << ", ";
    // cout << (eq_range.second == cm.end()) << endl;

    ASSERT_EQ(pos,  eq_range.first);
    ASSERT_EQ(next, eq_range.second);

    pair<const_iterator, const_iterator> ceq_range = cm.equal_range(key);
    ASSERT_EQ(cpos,  ceq_range.first);
    ASSERT_EQ(cnext, ceq_range.second);
  }

  iterator_test(m.begin(),   m, vs);
  iterator_test(cm.begin(),  m, vs);
  iterator_test(cm.cbegin(), m, vs);

  fail_count_test(m);
  fail_find_test(m);
  fail_equal_range_test(m);
  fail_at_test(m);

  key_equal_test(m);
  hash_test(m);
  allocator_test(m);
}

void insert_res_test (
  pair<iterator, bool> const& res,
  map_type&                   m,
  key_type             const& key,
  mapped_type          const& mapped
) {
  map_type const& cm = static_cast<map_type const&>(m);

  pointed_iterator_test(res.first, key, mapped);

  ASSERT_EQ(res.first, m.find(key));
  ASSERT_EQ(res.first, cm.find(key));
}

void success_insert_test(
  pair<iterator, bool> const& res,
  map_type&                   m,
  key_type             const& key,
  mapped_type          const& mapped
) {
  insert_res_test(res, m, key, mapped);
  ASSERT_TRUE(res.second);
}

void fail_insert_test(
  pair<iterator, bool> const& res,
  map_type&                   m,
  key_type             const& key,
  mapped_type          const& mapped
) {
  insert_res_test(res, m, key, mapped);
  ASSERT_FALSE(res.second);
}

TEST(ChainHashMap, Empty) {
  map_type m;

  ASSERT_EQ(m.begin(), m.end());
  status_test(m, value_list_type());
}

// --------------------------
// ---- COPY CONSTRUCTOR ----
// --------------------------

TEST_F(ChainHashMapFixiture, CopyConstructor) {
  {
    map_type m1;
    map_type m2(m1);
    status_test(m2, value_list_type());
  }

  {
    map_type m(this->map1);
    status_test(m, this->values1);
  }

  {
    map_type m(this->map2);
    status_test(m, this->values2);
  }
}

// ----------------
// ---- ASSIGN ----
// ----------------

TEST_F(ChainHashMapFixiture, Assign) {
  map_type m;

  m = this->map1;

  ASSERT_EQ(m.max_load_factor(), m.load_factor());
  status_test(m, this->values1);

  m = this->map2;

  ASSERT_EQ(m.max_load_factor(), m.load_factor());
  status_test(m, this->values2);

  m = map_type();

  ASSERT_EQ(0, m.load_factor());
  status_test(m, value_list_type());
}

// ----------------
// ---- INSERT ----
// ----------------

TEST(ChainHashMap, Insert) {
  map_type m;
  value_list_type vs;

  pair<iterator, bool> res1 = m.insert(make_pair("abc", 123));
  success_insert_test(res1, m, "abc", 123);

  vs.push_back(make_pair("abc", 123));
  status_test(m, vs);

  pair<iterator, bool> res2 = m.insert(make_pair("aaa", 100));
  success_insert_test(res2, m, "aaa", 100);

  vs.insert(vs.begin(), make_pair("aaa", 100));
  status_test(m, vs);

  pair<iterator, bool> res3 = m.insert(make_pair("zzz", 999));
  success_insert_test(res3, m, "zzz", 999);

  vs.push_back(make_pair("zzz", 999));
  status_test(m, vs);

  pair<iterator, bool> res4 = m.insert(make_pair("fjs", 111));
  success_insert_test(res4, m, "fjs", 111);

  vs.insert(vs.begin()+2, make_pair("fjs", 111));
  status_test(m, vs);
}

TEST_F(ChainHashMapFixiture, InsertElements) {
  status_test(this->map1, this->values1);
  status_test(this->map2, this->values2);
}

TEST_F(ChainHashMapFixiture, InsertDupElement) {
  pair<iterator, bool> res = this->map1.insert(make_pair("efg", 321));

  fail_insert_test(res, this->map1, "efg", 123);
  status_test(this->map1, this->values1);
}

TEST_F(ChainHashMapFixiture, InsertLowestDupElement) {
  pair<iterator, bool> res = this->map1.insert(make_pair("abc", 654));

  fail_insert_test(res, this->map1, "abc", 456);
  status_test(this->map1, this->values1);
}

TEST_F(ChainHashMapFixiture, InsertLargestDupElement) {
  pair<iterator, bool> res = this->map1.insert(make_pair("vsd", 128));

  fail_insert_test(res, this->map1, "vsd", 821);
  status_test(this->map1, this->values1);
}

TEST_F(ChainHashMapFixiture, InsertUnorderedElementSequence) {
  value_list_type unordered = this->values1;
  random_shuffle(unordered.begin(), unordered.end());

  map_type m;
  m.insert(make_pair("aaa", 100));
  m.insert(make_pair("zzz", 999));
  m.insert(unordered.begin(), unordered.end());

  value_list_type vs = this->values1;
  vs.insert(vs.begin(), make_pair("aaa", 100));
  vs.push_back(make_pair("zzz", 999));

  status_test(m, vs);
}

// ---------------
// ---- ACCESS ----
// ---------------

TEST(ChainHashMap, Access) {
  map_type m;
  value_list_type vs;

  {
    mapped_type& res = m["abc"];
    ASSERT_EQ(mapped_type(), res);

    res = 123;
    ASSERT_EQ(123, res);
    ASSERT_EQ(123, m["abc"]);

    vs.push_back(make_pair("abc", 123));
    status_test(m, vs);
  }

  {
    mapped_type& res = m["aaa"];
    ASSERT_EQ(mapped_type(), res);

    res = 100;
    ASSERT_EQ(100, res);
    ASSERT_EQ(100, m["aaa"]);

    vs.insert(vs.begin(), make_pair("aaa", 100));
    status_test(m, vs);
  }

  {
    mapped_type& res = m["zzz"];
    ASSERT_EQ(mapped_type(), res);

    res = 999;
    ASSERT_EQ(999, res);
    ASSERT_EQ(999, m["zzz"]);

    vs.push_back(make_pair("zzz", 999));
    status_test(m, vs);
  }

  {
    mapped_type& res = m["fjs"];
    ASSERT_EQ(mapped_type(), res);

    res = 111;
    ASSERT_EQ(111, res);
    ASSERT_EQ(111, m["fjs"]);

    vs.insert(vs.begin()+2, make_pair("fjs", 111));
    status_test(m, vs);
  }
}

// ---------------
// ---- ERASE ----
// ---------------

TEST(ChainHashMap, EraseFromEmpty) {
  map_type m;

  size_type res1 = m.erase("abc");
  ASSERT_EQ(0, res1);
  status_test(m, value_list_type());

  iterator res2 = m.erase(m.begin(), m.begin());
  ASSERT_EQ(m.end(), res2);
  status_test(m, value_list_type());
}

TEST_F(ChainHashMapFixiture, EraseByKey) {
  for (
    value_list_type::const_iterator viter = this->values2.begin();
    viter != this->values2.end();
    ++viter
  ) {
    value_list_type vs = this->values2;
    map_type m = this->map2;

    for (iterator iter = m.find(viter->first); iter != m.end(); ) {
      iterator next = iter; ++next;

      erase_value(vs, iter->first);
      size_type erased = m.erase(iter->first);

      ASSERT_EQ(1, erased);
      status_test(m, vs);

      iter = next;
    }
  }
}

TEST_F(ChainHashMapFixiture, EraseByIterator) {
  for (
    value_list_type::const_iterator viter = this->values2.begin();
    viter != this->values2.end();
    ++viter
  ) {
    value_list_type vs = this->values2;
    map_type m = this->map2;

    for (iterator iter = m.find(viter->first); iter != m.end(); ) {
      iterator next = iter; ++next;

      erase_value(vs, iter->first);
      iter = m.erase(iter);

      ASSERT_EQ(next, iter);
      status_test(m, vs);
    }
  }
}

TEST_F(ChainHashMapFixiture, EraseBySequence) {
  map_type m = this->map1;
  value_list_type vs = this->values1;

  {
    iterator res = m.erase(m.begin(), m.begin());

    ASSERT_EQ(m.begin(), res);
    status_test(m, vs);
  }

  {
    erase_value(vs, m.begin()->first);
    value_type value = *(++m.begin());
    iterator res = m.erase(m.begin(), ++m.begin());

    ASSERT_EQ(m.begin(), res);
    ASSERT_EQ(value.first,  res->first);
    ASSERT_EQ(value.second, res->second);
    status_test(m, vs);
  }

  {
    iterator first = next(m.begin());
    iterator last  = first; advance(last, 3);

    erase_multiple_value(vs, first, last);
    value_type value = *last;
    iterator res = m.erase(first, last);

    ASSERT_EQ(next(m.begin()), res);
    ASSERT_EQ(value.first,  res->first);
    ASSERT_EQ(value.second, res->second);
    status_test(m, vs);
  }

  {
    iterator res = m.erase(m.begin(), m.end());

    ASSERT_EQ(m.end(), res);
    status_test(m, value_list_type());
  }
}

// ---------------
// ---- CLEAR ----
// ---------------

TEST_F(ChainHashMapFixiture, Clear) {
  map_type m;
  m.clear();
  status_test(m, value_list_type());

  this->map1.clear();
  status_test(this->map1, value_list_type());

  this->map2.clear();
  status_test(this->map2, value_list_type());
}

// ---------------
// ---- SWAP -----
// ---------------

TEST_F(ChainHashMapFixiture, Swap) {
  {
    map_type m;
    m.swap(m);

    status_test(m, value_list_type());
  }

  {
    map_type m1, m2;
    m1.swap(m2);

    status_test(m1, value_list_type());
    status_test(m2, value_list_type());
  }

  {
    map_type m;
    m.swap(this->map1);

    status_test(m, this->values1);
    status_test(this->map1, value_list_type());

    m.swap(this->map1);

    status_test(m, value_list_type());
    status_test(this->map1, this->values1);
  }

  {
    map_type m;
    m.swap(this->map2);

    status_test(m, this->values2);
    status_test(this->map2, value_list_type());

    this->map2.swap(m);

    status_test(m, value_list_type());
    status_test(this->map2, this->values2);
  }

  {
    this->map1.swap(this->map2);

    status_test(this->map1, this->values2);
    status_test(this->map2, this->values1);

    this->map2.swap(this->map1);

    status_test(this->map1, this->values1);
    status_test(this->map2, this->values2);
  }
}

TEST_F(ChainHashMapFixiture, StaticSwap) {
  namespace container = sml::container;
  {
    map_type m;
    container::swap(m, m);

    status_test(m, value_list_type());
  }

  {
    map_type m1, m2;
    container::swap(m1, m2);

    status_test(m1, value_list_type());
    status_test(m2, value_list_type());
  }

  {
    map_type m;
    container::swap(m, this->map1);

    status_test(m, this->values1);
    status_test(this->map1, value_list_type());

    container::swap(m, this->map1);

    status_test(m, value_list_type());
    status_test(this->map1, this->values1);
  }

  {
    map_type m;
    container::swap(m, this->map2);

    status_test(m, this->values2);
    status_test(this->map2, value_list_type());

    container::swap(this->map2, m);

    status_test(m, value_list_type());
    status_test(this->map2, this->values2);
  }

  {
    container::swap(this->map1, this->map2);

    status_test(this->map1, this->values2);
    status_test(this->map2, this->values1);

    container::swap(this->map2, this->map1);

    status_test(this->map1, this->values1);
    status_test(this->map2, this->values2);
  }
}

// -----------------
// ---- REHASH -----
// -----------------

TEST_F(ChainHashMapFixiture, Rehash) {
  {
    map_type m = this->map1;
    m.rehash(ceil(m.size() / m.max_load_factor()));

    ASSERT_EQ(ceil(m.size() / m.max_load_factor()), m.bucket_count());
    status_test(m, this->values1);
  }

  {
    map_type m = this->map1;
    size_type bucket_count = m.bucket_count();
    m.rehash(ceil(m.size() / m.max_load_factor() - 1));

    ASSERT_EQ(bucket_count, m.bucket_count());
    status_test(m, this->values1);
  }

  {
    map_type m = this->map1;
    m.rehash(0);

    status_test(m, this->values1);
  }
}

// ------------------
// ---- RESERVE -----
// ------------------

TEST_F(ChainHashMapFixiture, Reserve) {
  float max_load_factor = this->map1.max_load_factor();

  {
    map_type m = this->map1;
    m.reserve(m.size()+1);

    ASSERT_EQ(max_load_factor, m.max_load_factor());
    status_test(m, this->values1);
  }

  {
    map_type m = this->map1;
    m.reserve(m.size()-1);

    ASSERT_EQ(max_load_factor, m.max_load_factor());
    status_test(m, this->values1);
  }

  {
    map_type m = this->map1;
    m.reserve(0);

    ASSERT_EQ(max_load_factor, m.max_load_factor());
    status_test(m, this->values1);
  }
}

// --------------------------
// ---- Max Load Factor -----
// --------------------------

TEST_F(ChainHashMapFixiture, MaxLoadFactor) {
  {
    map_type m = this->map1;
    float const bucket_count = m.bucket_count();
    float const next_mlf = m.max_load_factor() + 1.0f;
    m.max_load_factor(next_mlf);

    ASSERT_EQ(bucket_count, m.bucket_count());
    ASSERT_EQ(next_mlf,     m.max_load_factor());
    status_test(m, this->values1);
  }

  {
    map_type m = this->map1;
    float const bucket_count = m.bucket_count();
    float const next_mlf = m.max_load_factor() / 2.0f;
    m.max_load_factor(next_mlf);

    ASSERT_LT(bucket_count, m.bucket_count());
    ASSERT_EQ(next_mlf,     m.max_load_factor());
    status_test(m, this->values1);
  }
}

} // namespace

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
