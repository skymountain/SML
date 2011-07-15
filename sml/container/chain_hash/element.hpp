#ifndef _SML_CONTAINER_CHAIN_HASH_ELEMENT_HPP
#define _SML_CONTAINER_CHAIN_HASH_ELEMENT_HPP

namespace sml { namespace container { namespace chain_hash_detail {

template<class Types>
class element {
private:
  typedef Types types;

public:
  typedef element*       element_ptr;
  typedef element const* const_element_ptr;
  typedef element&       element_reference;
  typedef element const& const_element_reference;

  typedef typename types::value_type      value_type;
  typedef typename types::pointer         value_ptr;
  typedef typename types::const_pointer   const_value_ptr;
  typedef typename types::reference       value_reference;
  typedef typename types::const_reference const_value_reference;

  explicit element(const_value_reference v) :
    value_(v) {
  }

  element(element const& r) :
    value_(r.value_),
    next_(r.next_),
    prev_(r.prev_) {
  }

  value_reference       value()       { return this->value_; }
  const_value_reference value() const { return this->value_; }

  value_ptr       value_address() { return &(this->value_); }
  const_value_ptr value_address() const { return &(this->value_); }

  element_ptr       next()       { return this->next_; }
  const_element_ptr next() const { return this->next_; }

  void next(element_ptr next) { this->next_ = next; }

  element_ptr       prev()       { return this->prev_; }
  const_element_ptr prev() const { return this->prev_; }

  void prev(element_ptr prev) { this->prev_ = prev; }

private:
  element_reference operator=(element const& r) {}

  value_type  value_;
  element_ptr next_;
  element_ptr prev_;
};

}}} // namespace sml::container::chain_hash_detail

#endif
