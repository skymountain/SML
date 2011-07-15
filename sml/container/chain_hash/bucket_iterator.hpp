#ifndef _SML_CONTAINER_CHAIN_HASH_BUCKET_ITERATOR_HPP
#define _SML_CONTAINER_CHAIN_HASH_BUCKET_ITERATOR_HPP

#include <iterator>

namespace sml { namespace container { namespace chain_hash_detail {

template<class Types>
class table;

template<class Types>
class bucket_iterator_base {
private:
  typedef Types types;
  typedef typename types::pointer     pointer;
  typedef typename types::reference   reference;
  typedef typename types::element_ptr element_ptr;
  typedef typename types::bucket_ptr  bucket_ptr;

public:
  bucket_iterator_base() :
    current_() {
  }

  bucket_iterator_base(bucket_iterator_base const& r) :
    current_(r.current_) {
  }

  explicit bucket_iterator_base(bucket_ptr const bucket) :
    current_(bucket->head()) {
  }

  void assign(bucket_iterator_base const& r) {
    this->current_ = r.current_;
  }

  void increment() {
    this->current_ = this->current_->next();
  }

  reference dereference() const {
    return this->current_->value();
  }

  pointer base() const {
    return this->current_->value_address();
  }

  bool equal(bucket_iterator_base const& r) const {
    return this->current_ == r.current_;
  }

  element_ptr current() {
    return this->current_;
  }

private:
  element_ptr current_;
};

template<class Types>
class bucket_iterator :
  public std::iterator<
    std::forward_iterator_tag,
    typename Types::value_type,
    typename Types::difference_type,
    typename Types::pointer,
    typename Types::reference
  > {

  template<class _Types>
  friend class sml::container::chain_hash_detail::table;

private:
  typedef Types types;
  typedef typename types::bucket_iterator_base  base_iterator;
  typedef typename types::const_bucket_iterator const_bucket_iterator;
  typedef typename types::bucket_ptr            bucket_ptr;

public:
  typedef typename types::pointer   pointer;
  typedef typename types::reference reference;

  bucket_iterator() :
    base_() {
  }

  bucket_iterator(bucket_iterator const& r) :
    base_(r.base_) {
  }

private:
  bucket_iterator(bucket_ptr const bucket) :
    base_(bucket) {
  }

public:
  bucket_iterator& operator=(bucket_iterator const& r) {
    this->base_.assign(r.base_);
    return *this;
  }

  bucket_iterator& operator++() {
    this->base_.increment();
    return *this;
  }

  bucket_iterator const operator++(int) {
    bucket_iterator tmp(*this);
    this->base_.increment();
    return tmp;
  }

  reference operator*() const {
    this->base_.dereference();
  }

  pointer operator->() const {
    this->base_.base();
  }

  bool operator==(bucket_iterator const& r) const {
    return this->base_.equal(r.base_);
  }

  bool operator==(const_bucket_iterator const& r) const {
    return this->base_.equal(r.base_);
  }

  bool operator!=(bucket_iterator const& r) const {
    return !(*this == r);
  }

  bool operator!=(const_bucket_iterator const& r) const {
    return !(*this == r);
  }

private:
  base_iterator base_;
};

template<class Types>
class const_bucket_iterator :
  public std::iterator<
    std::forward_iterator_tag,
    typename Types::const_value_type,
    typename Types::difference_type,
    typename Types::const_pointer,
    typename Types::const_reference
  > {

  template<class _Types>
  friend class sml::container::chain_hash_detail::table;

private:
  typedef Types types;
  typedef typename types::bucket_iterator_base base_iterator;
  typedef typename types::bucket_iterator      bucket_iterator;
  typedef typename types::bucket_ptr           bucket_ptr;

public:
  typedef typename types::const_pointer   pointer;
  typedef typename types::const_reference reference;

  const_bucket_iterator() :
    base_() {
  }

  const_bucket_iterator(const_bucket_iterator const& r) :
    base_(r.base_) {
  }

  const_bucket_iterator(bucket_iterator const& r) :
    base_(r.base_) {
  }

private:
  const_bucket_iterator(bucket_ptr const bucket) :
    base_(bucket) {
  }

public:  
  const_bucket_iterator& operator=(const_bucket_iterator const& r) {
    this->base_.assign(r.base_);
    return *this;
  }

  const_bucket_iterator& operator=(bucket_iterator const& r) {
    this->base_.assign(r.base_);
    return *this;
  }

  const_bucket_iterator& operator++() {
    this->base_.increment();
    return *this;
  }

  const_bucket_iterator const operator++(int) {
    const_bucket_iterator tmp(*this);
    this->base_.increment();
    return *this;
  }

  reference operator*() const {
    return this->base_.dereference();
  }

  pointer operator->() const {
    return this->base_.base();
  }

  bool operator==(const_bucket_iterator const& r) const {
    return this->base_.equal(r.base_);
  }

  bool operator==(bucket_iterator const& r) const {
    return this->base_.equal(r.base_);
  }

  bool operator!=(const_bucket_iterator const& r) const {
    return !(*this == r);
  }

  bool operator!=(bucket_iterator const& r) const {
    return !(*this == r);
  }

private:
  base_iterator base_;

};


}}} // namespace sml::container::chain_hash_detail

#endif
