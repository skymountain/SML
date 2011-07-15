#ifndef _SML_CONTAINER_CHAIN_HASH_TABLE_HPP
#define _SML_CONTAINER_CHAIN_HASH_TABLE_HPP

#include <stdexcept>
#include <utility>

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

public:
  table(
    size_type      const  n,
    float          const  max_load_factor,
    hasher         const& hasher,
    key_equal      const& key_eq,
    allocator_type const& allocator,
    double         const  increment_rate
  ) :
    array_(),
    size_(),
    bucket_size_(),
    max_load_factor_(max_load_factor),
    hasher_(hasher),
    key_eq_(key_eq),
    allocator_(allocator),
    INCREMENT_RATE(increment_rate) {
    this->reconstruct(n);
  }

  ~table() {
    this->clear();

    bucket_allocator_type allocator(this->allocator_);
    allocator.destroy(this->bucket_size_, this->array_);
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
    return
      static_cast<float>(this->size()) /
      static_cast<float>(this->bucket_count());
  }

  float max_load_factor() const {
    return this->max_load_factor_;
  }

  float max_load_factor(const float mlf) {
    // XXX
    return this->max_load_factor_ = mlf;
  }

  hasher         const& hash_function() const { return this->hasher_; }
  key_equal      const& key_eq()        const { return this->key_eq_; }
  allocator_type const& get_allocator() const { return this->allocator_; }

  std::pair<iterator, bool> insert(value_type const& v) {
    iterator pos = this->find(v.first);
    return pos == this->end() ?
      std::make_pair(this->insert_impl(v), true) :
      std::make_pair(pos, false);
  }

  size_type erase(key_type const& key) {
    iterator pos(this->find(key));

    if (pos == this->end()) {
      return 0;
    }

    this->erase_impl(pos);
    return 1;
  }

  iterator erase(const_iterator const pos) {
    iterator next = increment(pos);
    this->erase(pos);
    return next;
  }

  void clear() {
    element_allocator_type allocator(this->get_allocator());

    for (size_type idx = 0; idx < this->bucket_count(); ++idx) {
      for (local_iterator iter = this->begin(idx); iter != this->end(idx);) {
        local_iterator tmp = increment(iter);
        allocator.destroy(1, iter.base_.current());
        iter = tmp;
      }
      this->get(idx)->head(element_ptr());
    }
  }

  mapped_type& at(key_type const& key) {
    return at_impl(this->find(key), this->end());
  }

  mapped_type const& at(key_type const& key) const {
    return at_impl(this->find(key), this->end());
  }

  iterator find(key_type const& key) {
    const_iterator pos = static_cast<table const&>(*this).find(key);
    return iterator(
      pos.base_.next_bucket(),
      pos.base_.end_bucket(),
      pos.base_.current()
    );
  }

  const_iterator find(key_type const& key) const {
    bucket_ptr const bucket = this->get(this->bucket(key));
    return this->find_impl(bucket, key);
  }

  size_type count(key_type const& key) const {
    return this->find(key) == this->end() ? 0 : 1;
  }

  size_type bucket_count() const {
    return this->bucket_size_;
  }

  size_type max_bucket_count() const {
    return this->get_allocator().max_size();
  }

  size_type bucket(key_type const& key) const {
    size_type const hash = this->hash_function()(key);
    return hash % this->bucket_count();
  }

  size_type bucket_size(size_type const idx) const {
    return this->get(idx)->size();
  }

  void rehash(size_type const n) {
    if (this->bucket_count() >= n) return;
    this->reconstruct(n);
  }

  void reserve(size_type const n) {
    this->rehash(std::ceil(static_cast<float>(n) / this->max_load_factor()));
  }

  std::pair<const_iterator, const_iterator> equal_range(
    key_type const& key
  ) const {
    const_iterator pos = this->find(key);
    const_iterator end = pos == this->end() ?
      this->end() : increment(pos);

    return std::make_pair(pos, end);
  }

  std::pair<iterator, iterator> equal_range(key_type const& key) {
    return static_cast<table const&>(*this).equal_range(key);
  }

  mapped_type& access(key_type const& key) {
    bucket_ptr const bucket = this->get(this->bucket(key));
    const_iterator pos = static_cast<table const&>(*this).find(bucket, key);

    if (pos == this->end()) {
      return this->insert_impl(bucket, std::make_pair(key, mapped_type()))
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

private:
  bucket_ptr get(size_type idx) const {
    return this->array_ + static_cast<difference_type>(idx);
  }

  const_bucket_ptr end_bucket() const {
    return this->array_ + static_cast<difference_type>(this->bucket_count());
  }

  void reconstruct(size_type const bucket_size) {
    bucket_allocator_type allocator(this->allocator_);
    allocator.construct(bucket_size, bucket_type());

    bucket_ptr const new_array = allocator.get();
    for (iterator iter = this->begin(); iter != this->end(); ++iter) {
      element_ptr const element = iter.base_.current();
      size_type   const idx     = this->bucket(element->value().first);
      new_array[idx].push_front(element);
    }

    allocator.destroy(this->bucket_size_, this->array_);
    allocator.release();

    this->array_       = new_array;
    this->bucket_size_ = bucket_size;
  }

  const_iterator find_impl(
    bucket_ptr const  bucket,
    key_type   const& key
  ) const  {
    const_local_iterator const end;

    for (const_local_iterator iter(bucket); iter != end; ++iter) {
      if (this->key_eq()(iter->first, key)) {
        return const_iterator(
          bucket,
          this->end_bucket(),
          iter.base_.current()
        );
      }
    }

    return this->end();
  }

  iterator insert_impl(value_type const& v) {
    bucket_ptr const bucket = this->get(this->bucket(v.first));
    return this->insert_impl(bucket, v);
  }

  iterator insert_impl(bucket_ptr const bucket, value_type const& v) {
    element_allocator_type allocator(this->get_allocator());
    allocator.construct(1, element_type(v));
    element_ptr const elem = allocator.get();

    bucket->push_front(elem);
    ++this->size_;
    allocator.release();

    if (this->max_load_factor() < this->load_factor()) {
      double const new_size =
        static_cast<double>(this->size()) * INCREMENT_RATE;
      this->reconstruct(static_cast<size_type>(new_size));
    }

    return iterator(bucket, this->end_bucket(), elem);
  }

  void erase_impl(const_iterator pos) {
    element_ptr const target = pos.base_.current();
    bucket_ptr  const bucket =
      pos.base_.next_bucket() - static_cast<difference_type>(1);

    (bucket)->erase(target);

    --this->size_;
    element_allocator_type(this->get_allocator()).destroy(1, target);
  }

  template<class Iterator>
  static Iterator increment(Iterator x) {
    return ++x;
  }

  static mapped_type& at_impl(const_iterator pos, const_iterator end) {
    if (pos == end) {
      throw std::out_of_range("out of range at chain_hash_table#at");
    }
    return pos.base_.current().value().second;
  }

  double const    INCREMENT_RATE;

  bucket_ptr      array_;
  size_type       size_;
  size_type       bucket_size_;
  float           max_load_factor_;
  hasher          hasher_;
  key_equal       key_eq_;
  allocator_type  allocator_;
};

}}} // namespace sml::container::chain_hash_detail


#endif
