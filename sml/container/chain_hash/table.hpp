#ifndef _SML_CONTAINER_CHAIN_HASH_TABLE_HPP
#define _SML_CONTAINER_CHAIN_HASH_TABLE_HPP

#include <stdexcept>
#include <utility>
#include <cstddef>
#include "sml/iterator/next.hpp"
#include "sml/utility/noncopyable.hpp"

namespace sml { namespace container { namespace chain_hash_detail {

template<class Types>
class table {
public:
  typedef Types types;
  typedef typename types::key_type               key_type;
  typedef typename types::mapped_type            mapped_type;
  typedef typename types::value_type             value_type;
  typedef typename types::pointer                pointer;
  typedef typename types::reference              reference;
  typedef typename types::pointer                const_pointer;
  typedef typename types::reference              const_reference;
  typedef typename types::difference_type        difference_type;
  typedef typename types::size_type              size_type;
  typedef typename types::hasher                 hasher;
  typedef typename types::key_equal              key_equal;
  typedef typename types::allocator_type         allocator_type;

  typedef typename types::bucket_type            bucket_type;
  typedef typename types::bucket_ptr             bucket_ptr;
  typedef typename types::const_bucket_ptr       const_bucket_ptr;
  typedef typename types::bucket_allocator_type  bucket_allocator_type;

  typedef typename types::element_type           element_type;
  typedef typename types::element_ptr            element_ptr;
  typedef typename types::const_element_ptr      const_element_ptr;
  typedef typename types::element_allocator_type element_allocator_type;

  typedef
    sml::container::chain_hash_detail::table_iterator<types>
    iterator;
  typedef
    sml::container::chain_hash_detail::const_table_iterator<types>
    const_iterator;
  typedef
    sml::container::chain_hash_detail::bucket_iterator<types>
    local_iterator;
  typedef
    sml::container::chain_hash_detail::const_bucket_iterator<types>
    const_local_iterator;

  typedef typename sml::container::chain_hash_detail::table<types> table_type;
  typedef table_type* table_ptr;

  class Destroyer : sml::utility::noncopyable {
  public:
    Destroyer(table_type& tbl) : tbl_(&tbl) {
    }

    ~Destroyer() {
      if (this->tbl_) this->tbl_->destroy();
    }

    void release() {
      this->tbl_ = table_ptr();
    }

  private:
    table_ptr tbl_;
  };

public:
  table(
    double         const  increment_rate,
    size_type      const  initial_bucket_count,
    size_type      const  n,
    float          const  max_load_factor,
    hasher         const& hasher,
    key_equal      const& key_eq,
    allocator_type const& allocator
  ) :
    INCREMENT_RATE(increment_rate),
    INITIAL_BUCKET_COUNT(initial_bucket_count),
    array_(),
    size_(),
    bucket_count_(),
    max_load_factor_(max_load_factor),
    hasher_(hasher),
    key_eq_(key_eq),
    allocator_(allocator) {
    this->reconstruct(n);
  }

  table(table const& r) :
    INCREMENT_RATE(r.INCREMENT_RATE),
    INITIAL_BUCKET_COUNT(r.INITIAL_BUCKET_COUNT),
    array_(),
    size_(),
    bucket_count_(),
    max_load_factor_(r.max_load_factor_),
    hasher_(r.hasher_),
    key_eq_(r.key_eq_),
    allocator_(r.allocator_) {
    Destroyer destroyer(*this);
    this->copy_table(r);
    destroyer.release();
  }

  table(table const& r, allocator_type const& allocator) :
    INCREMENT_RATE(r.INCREMENT_RATE),
    INITIAL_BUCKET_COUNT(r.INITIAL_BUCKET_COUNT),
    array_(),
    size_(),
    bucket_count_(),
    max_load_factor_(r.max_load_factor_),
    hasher_(r.hasher_),
    key_eq_(r.key_eq_),
    allocator_(allocator) {
    Destroyer destroyer(*this);
    this->copy_table(r);
    destroyer.release();
  }

  ~table() {
    this->destroy();
  }

  iterator begin() {
    return iterator(this->array_, this->end_bucket());
  }
  const_iterator begin() const {
    return const_iterator(this->array_, this->end_bucket());
  }

  iterator       end()          { return iterator(); }
  const_iterator end()    const { return const_iterator(); }

  local_iterator begin(size_type const n) {
    return local_iterator(this->get(n));
  }
  const_local_iterator begin(size_type const n) const {
    return const_local_iterator(this->get(n));
  }

  local_iterator end(size_type) {
    return local_iterator();
  }
  const_local_iterator end(size_type) const {
    return const_local_iterator();
  }

  bool empty() const {
    return this->size() == 0;
  }

  size_type size() const {
    return this->size_;
  }

  size_type max_size() const {
    return this->allocator_.max_size();
  }

  float load_factor() const {
    return table_type::load_factor(this->size(), this->bucket_count());
  }

  float max_load_factor() const {
    return this->max_load_factor_;
  }

  void max_load_factor(float const mlf) {
    if (mlf < this->load_factor()) {
      this->reconstruct(this->size() / mlf);
    }

    this->max_load_factor_ = mlf;
  }

  hasher         const& hash_function() const { return this->hasher_; }
  key_equal      const& key_eq()        const { return this->key_eq_; }
  allocator_type const& get_allocator() const { return this->allocator_; }

  std::pair<iterator, bool> insert(value_type const& v) {
    iterator pos = this->find(v.first);
    return pos == this->end() ?
      std::make_pair(this->insert_and_rehash(v), true) :
      std::make_pair(pos, false);
  }

  size_type erase(key_type const& key) {
    iterator pos(this->find(key));

    if (pos == this->end()) {
      return 0;
    }

    this->naive_erase(pos);
    return 1;
  }

  iterator erase(const_iterator const pos) {
    iterator next = table_type::to_iterator(sml::iterator::next(pos));
    this->naive_erase(pos);
    return next;
  }

  iterator erase(const_iterator first, const_iterator last) {
    iterator iter = table_type::to_iterator(first);
    while (iter.base_.current() != last.base_.current()) {
      iter = this->erase(iter);
    }
    return iter;
  }

  void clear() {
    element_allocator_type allocator(this->get_allocator());

    for (size_type idx = 0; idx < this->bucket_count(); ++idx) {
      this->get(idx)->clear(allocator);
    }

    this->size_ = 0;
  }

  mapped_type& at(key_type const& key) {
    return table_type::at(this->find(key), this->end());
  }

  mapped_type const& at(key_type const& key) const {
    return table_type::at(this->find(key), this->end());
  }

  iterator find(key_type const& key) {
    const_iterator pos = static_cast<table const&>(*this).find(key);
    return table_type::to_iterator(pos);
  }

  const_iterator find(key_type const& key) const {
    bucket_ptr const bucket = this->get(this->bucket(key));
    return this->find_in_bucket(bucket, key);
  }

  size_type count(key_type const& key) const {
    return this->find(key) == this->end() ? 0 : 1;
  }

  size_type bucket_count() const {
    return this->bucket_count_;
  }

  size_type max_bucket_count() const {
    return this->get_allocator().max_size();
  }

  size_type bucket(key_type const& key) const {
    return table_type::bucket(this->hash_function(), this->bucket_count(), key);
  }

  size_type bucket_size(size_type const idx) const {
    return this->get(idx)->size();
  }

  void rehash(size_type n) {
    if (n <= 0) n = INITIAL_BUCKET_COUNT;

    float const load_factor = table_type::load_factor(this->size(), n);
    if (load_factor > this->max_load_factor()) return;

    this->reconstruct(n);
  }

  void reserve(size_type const n) {
    this->rehash(
      table_type::minimum_bucket_count(n, this->max_load_factor())
    );
  }

  std::pair<const_iterator, const_iterator> equal_range(
    key_type const& key
  ) const {
    const_iterator pos = this->find(key);
    const_iterator next = pos == this->end() ?
      this->end() : sml::iterator::next(pos);

    return std::make_pair(pos, next);
  }

  std::pair<iterator, iterator> equal_range(key_type const& key) {
    std::pair<const_iterator, const_iterator> range =
      static_cast<table const&>(*this).equal_range(key);

    return std::make_pair(
      table_type::to_iterator(range.first),
      table_type::to_iterator(range.second)
    );
  }

  void swap(table& tbl) {
    if (this == &tbl) return;

    if (this->get_allocator() == tbl.get_allocator()) {
      this->naive_swap(tbl);
    }
    else {
      table this_src(tbl,   this->get_allocator());
      this->swap(this_src);

      table tbl_src(*this,  tbl.get_allocator());
      tbl.swap(tbl_src);
    }
  }

  void assign(table const& r) {
    if (this != &r) {
      table tmp(r);
      this->swap(tmp);
    }
  }

  mapped_type& access(key_type const& key) {
    bucket_ptr const bucket = this->get(this->bucket(key));
    const_iterator pos =
      static_cast<table const&>(*this).find_in_bucket(bucket, key);

    if (pos == this->end()) {
      return this->insert_and_rehash(bucket, std::make_pair(key, mapped_type()))
                 ->second;
    }
    else {
      return pos.base_.current()->value().second;
    }
  }

  bool equal(table const& tbl) const {
    if (this->size() != tbl.size()) return false;

    for (const_iterator iter = this->begin(); iter != this->end(); ++iter) {
      const_iterator pos = tbl.find(iter->first);
      if (
        pos == tbl.end() ||
        !(iter->second == pos->second)
      ) {
        return false;
      }
    }

    return true;
  }

  void destroy() {
    this->clear();

    bucket_allocator_type allocator(this->allocator_);
    allocator.destroy(this->bucket_count_, this->array_);
  }

private:
  void copy_table(table const& tbl) {
    this->reserve(tbl.size());

    for (const_iterator iter = tbl.begin(); iter != tbl.end(); ++iter) {
      this->naive_insert(*iter);
    }
  }

  bucket_ptr get(size_type idx) const {
    return this->array_ + static_cast<std::ptrdiff_t>(idx);
  }

  const_bucket_ptr end_bucket() const {
    return this->array_ + static_cast<std::ptrdiff_t>(this->bucket_count());
  }

  void reconstruct(size_type const bucket_count) {
    bucket_allocator_type allocator(this->get_allocator());
    allocator.construct(bucket_count, bucket_type());

    bucket_ptr const new_array = allocator.get();
    for (
      bucket_ptr bucket_iter = this->array_;
      bucket_iter != this->end_bucket();
      ++bucket_iter
    ) {
      for (element_ptr elem_iter = bucket_iter->head(); elem_iter; ) {
        element_ptr const next = elem_iter->next();
        size_type   const idx  = table_type::bucket(
          this->hash_function(),
          bucket_count,
          elem_iter->value().first
        );

        new_array[idx].push_front(elem_iter);
        elem_iter = next;
      }
    }

    allocator.destroy(this->bucket_count_, this->array_);
    allocator.release();

    this->array_        = new_array;
    this->bucket_count_ = bucket_count;
  }

  const_iterator find_in_bucket(
    bucket_ptr const  bucket,
    key_type   const& key
  ) const  {
    const_local_iterator const end;

    for (const_local_iterator iter(bucket); iter != end; ++iter) {
      if (this->key_eq()(iter->first, key)) {

        return const_iterator(
          bucket + static_cast<std::ptrdiff_t>(1),
          this->end_bucket(),
          iter.base_.current()
        );

      }
    }

    return this->end();
  }

  iterator insert_and_rehash(value_type const& v) {
    bucket_ptr const bucket = this->get(this->bucket(v.first));
    return this->insert_and_rehash(bucket, v);
  }

  iterator insert_and_rehash(bucket_ptr bucket, value_type const& v) {
    element_allocator_type allocator(this->get_allocator());
    allocator.construct(1, element_type(v));

    float const future_load_factor =
      table_type::load_factor(this->size()+1, this->bucket_count());

    if (future_load_factor > this->max_load_factor()) {
      double const new_size =
        static_cast<double>(this->size()) * INCREMENT_RATE;
      this->reconstruct(static_cast<size_type>(new_size));

      bucket = this->get(this->bucket(v.first));
    }

    element_ptr const elem = allocator.release();
    return this->insert_into_bucket(bucket, elem);
  }

  iterator naive_insert(value_type const& v) {
    bucket_ptr const bucket = this->get(this->bucket(v.first));
    return this->insert_into_bucket(bucket, v);
  }

  iterator insert_into_bucket(bucket_ptr const bucket, value_type const& v) {
    element_allocator_type allocator(this->get_allocator());
    allocator.construct(1, element_type(v));

    element_ptr const elem = allocator.release();

    return this->insert_into_bucket(bucket, elem);
  }

  iterator insert_into_bucket(bucket_ptr const bucket, element_ptr const elem) {
    bucket->push_front(elem);
    ++this->size_;

    return iterator(bucket, this->end_bucket(), elem);
  }

  void naive_erase(const_iterator pos) {
    element_ptr const target = pos.base_.current();
    bucket_ptr  const bucket =
      pos.base_.next_bucket() - static_cast<std::ptrdiff_t>(1);

    (bucket)->erase(target);

    --this->size_;
    element_allocator_type(this->get_allocator()).destroy(1, target);
  }

  void naive_swap(table& tbl) {
    using std::swap;

    swap(this->array_,           tbl.array_);
    swap(this->size_,            tbl.size_);
    swap(this->bucket_count_,    tbl.bucket_count_);
    swap(this->max_load_factor_, tbl.max_load_factor_);
    swap(this->hasher_,          tbl.hasher_);
    swap(this->key_eq_,          tbl.key_eq_);
  }

  static size_type bucket(
    hasher    const& hasher,
    size_type const  bucket_count,
    key_type  const& key
  ) {
    return hasher(key) % bucket_count;
  }

  static mapped_type& at(const_iterator pos, const_iterator end) {
    if (pos == end) {
      throw std::out_of_range("out of range at chain_hash_table#at");
    }
    return pos.base_.current()->value().second;
  }

  static float load_factor(size_type size, size_type bucket_count) {
    return static_cast<float>(size) / static_cast<float>(bucket_count);
  }

  static size_type minimum_bucket_count(size_type size, float load_factor) {
    return std::ceil(static_cast<float>(size) / load_factor);
  }

  static iterator to_iterator(const_iterator from) {
    return iterator(
      from.base_.next_bucket(),
      from.base_.end_bucket(),
      from.base_.current()
    );
  }

  double    const INCREMENT_RATE;
  size_type const INITIAL_BUCKET_COUNT;

  bucket_ptr      array_;
  size_type       size_;
  size_type       bucket_count_;
  float           max_load_factor_;
  hasher          hasher_;
  key_equal       key_eq_;
  allocator_type  allocator_;
};

}}} // namespace sml::container::chain_hash_detail


#endif
