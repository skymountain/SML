#ifndef _SML_CONTAINER_CHAIN_HASH_TABLE_ITERATOR_HPP
#define _SML_CONTAINER_CHAIN_HASH_TABLE_ITERATOR_HPP

#include <iterator>

namespace sml { namespace container { namespace chain_hash_detail {

template<class Types>
class table;

template<class Types>
class table_iterator;

template<class Types>
class const_table_iterator;

template<class Types>
class table_iterator_base {
private:
  typedef Types types;

  typedef typename types::element_ptr      element_ptr;
  typedef typename types::bucket_ptr       bucket_ptr;
  typedef typename types::const_bucket_ptr const_bucket_ptr;
  typedef typename types::pointer          pointer;
  typedef typename types::reference        reference;

public:
  table_iterator_base() :
    next_bucket_(),
    end_bucket_(),
    current_() {
  }

  table_iterator_base(table_iterator_base const& r) :
    next_bucket_(r.next_bucket_),
    end_bucket_(r.end_bucket_),
    current_(r.current_) {
  }

  table_iterator_base(
    bucket_ptr       const first,
    const_bucket_ptr const last
  ) :
    next_bucket_(first),
    end_bucket_(last),
    current_() {
    this->point_next();
  }

  table_iterator_base(
    bucket_ptr       const first,
    const_bucket_ptr const last,
    element_ptr      const current
  ) :
    next_bucket_(first),
    end_bucket_(last),
    current_(current) {
  }

  void assign(table_iterator_base const& r) {
    this->next_bucket_ = r.next_bucket_;
    this->end_bucket_  = r.end_bucket_;
    this->current_     = r.current_;
  }

  void increment() {
    if (this->current_) {
      this->current_ = this->current_->next();
    }

    if (!this->current_) {
      this->point_next();
    }
  }

  reference dereference() const {
    return this->current_->value();
  }

  pointer base() const {
    return this->current_->value_address();
  }

  bool equal(table_iterator_base const& r) const {
    return this->current_ == r.current_;
  }

  bucket_ptr       next_bucket()    { return this->next_bucket_; }
  const_bucket_ptr end_bucket()     { return this->end_bucket_; }
  element_ptr      current()        { return this->current_; }

private:
  void point_next() {
    this->current_ = element_ptr();
    for (
      ; !this->current_ && (this->next_bucket_ != this->end_bucket_);
      ++this->next_bucket_
    ) {
      current_ = this->next_bucket_->head();
    }
  }

  bucket_ptr       next_bucket_;
  const_bucket_ptr end_bucket_;
  element_ptr      current_;
};

template<class Types>
class table_iterator :
  public std::iterator<
    std::forward_iterator_tag,
    typename Types::value_type,
    typename Types::difference_type,
    typename Types::pointer,
    typename Types::reference
  > {

  template<class _Types>
  friend class sml::container::chain_hash_detail::table;

  template<class _T>
  friend class sml::container::chain_hash_detail::const_table_iterator;

private:
  typedef Types types;
  typedef table_iterator_base<types>       base_iterator;
  typedef typename types::bucket_ptr       bucket_ptr;
  typedef typename types::const_bucket_ptr const_bucket_ptr;
  typedef typename types::element_ptr      element_ptr;

public:
  typedef typename types::pointer   pointer;
  typedef typename types::reference reference;

  table_iterator() :
    base_() {
  }

  table_iterator(table_iterator const& r) :
    base_(r.base_) {
  }

private:
  table_iterator(
    bucket_ptr       const first,
    const_bucket_ptr const last
  ) :
    base_(first, last) {
  }

  table_iterator(
    bucket_ptr       const first,
    const_bucket_ptr const last,
    element_ptr      const current
  ) :
    base_(first, last, current) {
  }

public:
  table_iterator& operator=(table_iterator const& r) {
    this->base_.assign(r.base_);
    return *this;
  }

  table_iterator& operator++() {
    this->base_.increment();
    return *this;
  }

  table_iterator const operator++(int) {
    table_iterator tmp(*this);
    this->base_.increment();
    return tmp;
  }

  reference operator*() const {
    return this->base_.dereference();
  }

  pointer operator->() const {
    return this->base_.base();
  }

  bool operator==(table_iterator const& r) const {
    return this->base_.equal(r.base_);
  }

  bool operator==(const_table_iterator<types> const& r) const {
    return this->base_.equal(r.base_);
  }

  bool operator!=(table_iterator const& r) const {
    return !(*this == r);
  }

  bool operator!=(const_table_iterator<types> const& r) const {
    return !(*this == r);
  }

private:
  base_iterator base_;

};

template<class Types>
class const_table_iterator :
  public std::iterator<
    std::forward_iterator_tag,
    typename Types::const_value_type,
    typename Types::difference_type,
    typename Types::const_pointer,
    typename Types::const_reference
  > {

  template<class _Types>
  friend class sml::container::chain_hash_detail::table;

  template<class _T>
  friend class sml::container::chain_hash_detail::table_iterator;

private:
  typedef Types types;
  typedef typename types::table_iterator_base base_iterator;
  typedef typename types::bucket_ptr          bucket_ptr;
  typedef typename types::const_bucket_ptr    const_bucket_ptr;
  typedef typename types::element_ptr         element_ptr;

public:
  typedef typename types::const_pointer   pointer;
  typedef typename types::const_reference reference;

  const_table_iterator() :
    base_() {
  }

  const_table_iterator(const_table_iterator const& r) :
    base_(r.base_) {
  }

  const_table_iterator(table_iterator<types> const& r) :
    base_(r.base_) {
  }

private:
  const_table_iterator(
    bucket_ptr       const first,
    const_bucket_ptr const last
  ) :
    base_(first, last) {
  }

  const_table_iterator(
    bucket_ptr       const first,
    const_bucket_ptr const last,
    element_ptr      const current
  ) :
    base_(first, last, current) {
  }

public:
  const_table_iterator& operator=(const_table_iterator const& r) {
    this->base_.assign(r.base_);
    return *this;
  }

  const_table_iterator& operator=(table_iterator<types> const& r) {
    this->base_.assign(r.base_);
    return *this;
  }

  const_table_iterator& operator++() {
    this->base_.increment();
    return *this;
  }

  const_table_iterator const operator++(int) {
    const_table_iterator tmp(*this);
    this->base_.increment();
    return tmp;
  }

  reference operator*() const {
    return this->base_.dereference();
  }

  pointer operator->() const {
    return this->base_.base();
  }

  bool operator==(const_table_iterator const& r) const {
    return this->base_.equal(r.base_);
  }

  bool operator==(table_iterator<types> const& r) const {
    return this->base_.equal(r.base_);
  }

  bool operator!=(const_table_iterator const& r) const {
    return !(*this == r);
  }

  bool operator!=(table_iterator<types> const& r) const {
    return !(*this == r);
  }

private:
  base_iterator base_;

};

}}} // namespace sml::container::chain_hash_detail

#endif
