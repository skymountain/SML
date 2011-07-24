#ifndef _SML_CONTAINER_CHAIN_HASH_MAP_HPP
#define _SML_CONTAINER_CHAIN_HASH_MAP_HPP

#include <algorithm>
#include <iterator>
#include <cmath>
#include <cstddef>
#include "sml/container/chain_hash/map_types.hpp"
#include "sml/container/chain_hash/element.hpp"
#include "sml/container/chain_hash/bucket.hpp"
#include "sml/container/chain_hash/bucket_iterator.hpp"
#include "sml/container/chain_hash/table_iterator.hpp"
#include "sml/container/chain_hash/table.hpp"

namespace sml { namespace container {

template<
  class Key,
  class T,
  class Hash,
  class Pred  = std::equal_to<Key>,
  class Alloc = std::allocator< std::pair<Key const, T> >
>
class chain_hash_map {

private:
  typedef
    sml::container::chain_hash_detail::map_types<
      Key, T, Hash, Pred, Alloc
    >
    types;
    
  typedef
    sml::container::chain_hash_detail::template table<types>
    table_type;

public:
  typedef typename table_type::key_type             key_type;
  typedef typename table_type::mapped_type          mapped_type;
  typedef typename table_type::value_type           value_type;
  typedef typename table_type::pointer              pointer;
  typedef typename table_type::const_pointer        const_pointer;
  typedef typename table_type::reference            reference;
  typedef typename table_type::const_reference      const_reference;
  typedef typename table_type::difference_type      difference_type;
  typedef typename table_type::size_type            size_type;
  typedef typename table_type::hasher               hasher;
  typedef typename table_type::key_equal            key_equal;
  typedef typename table_type::allocator_type       allocator_type;
  typedef typename table_type::iterator             iterator;
  typedef typename table_type::const_iterator       const_iterator;
  typedef typename table_type::local_iterator       local_iterator;
  typedef typename table_type::const_local_iterator const_local_iterator;

private:
  static size_type const BUCKETS_COUNT   = 17;
  static float     const MAX_LOAD_FACTOR = 1.0f;
  static double    const INCREMENT_RATE  = 1.5;

public:
  explicit chain_hash_map(
    size_type      const  n         = BUCKETS_COUNT,
    hasher         const& hasher    = hasher(),
    key_equal      const& key_eq    = key_equal(),
    allocator_type const& allocator = allocator_type()
  ) :
    tbl_(
      INCREMENT_RATE,
      BUCKETS_COUNT,
      n,
      MAX_LOAD_FACTOR,
      hasher,
      key_eq,
      allocator
    ) {
  }

  explicit chain_hash_map(allocator_type const& allocator) :
    tbl_(
      INCREMENT_RATE,
      BUCKETS_COUNT,
      BUCKETS_COUNT,
      MAX_LOAD_FACTOR,
      hasher(),
      key_equal(),
      allocator
    ) {
  }

  template<class InputIterator>
  chain_hash_map(
    InputIterator         first,
    InputIterator  const  last,
    size_type      const  n         = BUCKETS_COUNT,
    hasher         const& hasher    = hasher(),
    key_equal      const& key_eq    = key_equal(),
    allocator_type const& allocator = allocator_type()
  ) :
    tbl_(
      INCREMENT_RATE,
      BUCKETS_COUNT,
      n,
      MAX_LOAD_FACTOR,
      hasher,
      key_eq,
      allocator
    ) {
    for (; first != last; ++first) {
      this->tbl_.insert(*first);
    }
  }

  chain_hash_map(chain_hash_map const& r) :
    tbl_(r.tbl_) {
  }

  chain_hash_map(chain_hash_map const& r, allocator_type const& allocator) :
    tbl_(r.tb_, allocator) {
  }

  iterator       begin()        { return this->tbl_.begin(); }
  const_iterator begin()  const { return this->tbl_.begin(); }
  iterator       end()          { return this->tbl_.end(); }
  const_iterator end()    const { return this->tbl_.end(); }

  const_iterator cbegin() const { return this->begin(); }
  const_iterator cend()   const { return this->end(); }

  local_iterator       begin(size_type n)        { return this->tbl_.begin(n); }
  const_local_iterator begin(size_type n)  const { return this->tbl_.begin(n); }

  local_iterator       end(size_type n)          { return this->tbl_.end(n); }
  const_local_iterator end(size_type n)    const { return this->tbl_.end(n); }

  const_local_iterator cbegin(size_type n) const { return this->begin(n); }
  const_local_iterator cend(size_type n)   const { return this->end(n); }

  bool      empty()    const { return this->tbl_.empty(); }
  size_type size()     const { return this->tbl_.size(); }
  size_type max_size() const { return this->tbl_.max_size(); }

  float load_factor()     const  { return this->tbl_.load_factor(); }
  float max_load_factor() const  { return this->tbl_.max_load_factor(); }
  void  max_load_factor(float z) { this->tbl_.max_load_factor(z); }

  hasher hash_function()         const { return this->tbl_.hash_function(); }
  key_equal key_eq()             const { return this->tbl_.key_eq(); }
  allocator_type get_allocator() const { return this->tbl_.get_allocator(); }

  std::pair<iterator, bool> insert(value_type const& v) {
    return this->tbl_.insert(v);
  }

  std::pair<iterator, bool> insert(const_iterator, value_type const& v) {
    return this->insert(v);
  }

  template<class InputIterator>
  void insert(InputIterator first, InputIterator last) {
    for (; first != last; ++first) {
      this->insert(*first);
    }
  }

  size_type erase(key_type const& key)     { return this->tbl_.erase(key); }
  iterator erase(const_iterator const pos) { return this->tbl_.erase(pos); }
  iterator erase(const_iterator first, const_iterator last) {
    return this->tbl_.erase(first, last);
  }

  void clear() { return this->tbl_.clear(); }

  mapped_type& at (key_type const& key) {
    return this->tbl_.at(key);
  }
  mapped_type const& at (key_type const& key) const {
    return this->tbl_.at(key);
  }

  iterator find(key_type const& key) {
    return this->tbl_.find(key);
  }
  const_iterator find(key_type const& key) const {
    return this->tbl_.find(key);
  }

  size_type count(key_type const& key) const {
    return this->tbl_.count(key);
  }
  size_type bucket_count() const {
    return this->tbl_.bucket_count();
  }
  size_type max_bucket_count() const {
    return this->tbl_.max_bucket_count();
  }
  size_type bucket(key_type const& key) const {
    return this->tbl_.bucket(key);
  }
  size_type bucket_size(size_type const n) const {
    return this->tbl_.bucket_size(n);
  }

  void rehash(size_type n)  { return this->tbl_.rehash(n); }
  void reserve(size_type n) { return this->tbl_.reserve(n); }

  std::pair<iterator, iterator> equal_range(key_type const& key) {
    return this->tbl_.equal_range(key);
  }
  std::pair<const_iterator, const_iterator> equal_range(
    key_type const& key
  ) const {
    return this->tbl_.equal_range(key);
  }

  void swap(chain_hash_map& x) {
    this->tbl_.swap(x.tbl_);
  }

  chain_hash_map& operator=(chain_hash_map const& r) {
    this->tbl_.assign(r.tbl_);
    return *this;
  }

  mapped_type& operator[](key_type const& key) {
    return this->tbl_.access(key);
  };

  bool operator==(chain_hash_map const& r) const {
    return this->tbl_.equal(r.tbl_);
  }
  bool operator!=(chain_hash_map const& r) const {
    return !(*this == r);
  }

  bool operator<(chain_hash_map const& r) const {
    return std::lexicographical_compare(
      this->tbl_.begin(), this->tbl_.end(),
      r.tbl_.begin(),     r.tbl_.end()
    );
  }
  bool operator>(chain_hash_map const& r) const {
    return r < *this;
  }
  bool operator<=(chain_hash_map const& r) const {
    return !(r < *this);
  }
  bool operator>=(chain_hash_map const& r) const {
    return !(*this < r);
  }

private:

  table_type tbl_;
};

template<class Key, class T, class Hash, class Pred, class Alloc>
void swap(
  chain_hash_map<Key, T, Hash, Pred, Alloc>& x,
  chain_hash_map<Key, T, Hash, Pred, Alloc>& y
) {
  x.swap(y);
}

}} // namespace sml::container

#endif
