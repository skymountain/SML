#ifndef _SML_CONTAINER_CHAIN_HASH_BUCKET_HPP
#define _SML_CONTAINER_CHAIN_HASH_BUCKET_HPP

namespace sml { namespace container { namespace chain_hash_detail {

template<class Types>
class bucket {
private:
  typedef Types types;
  typedef typename types::element_ptr       element_ptr;
  typedef typename types::const_element_ptr const_element_ptr;
  typedef typename types::size_type         size_type;

  typedef typename types::element_allocator_type element_allocator_type;

public:
  bucket() :
    head_() {
  }

  element_ptr       head() { return this->head_; }
  const_element_ptr head() const { return this->head_; }

  void push_front(element_ptr const elem) {
    elem->next(this->head());
    elem->prev(element_ptr());

    if (this->head_) this->head_->prev(elem);
    this->head_ = elem;
  }

  void erase(element_ptr const elem) {
    element_ptr const prev = elem->prev();
    element_ptr const next = elem->next();

    if (prev) prev->next(next);
    else this->head_ = next;

    if (next) next->prev(prev);
  }

  void clear(element_allocator_type& allocator) {
    element_ptr iter = this->head_;
    while (iter) {
      element_ptr next = iter->next();
      allocator.destroy(1, iter);
      iter = next;
    }

    this->head_ = element_ptr();
  }

  size_type size() const {
    size_type size = 0;
    for (
      const_element_ptr iter = this->head();
      iter != const_element_ptr();
      iter = iter->next(), ++size
    );
    return size;
  }

private:
  element_ptr head_;
};

}}} // namespace sml::container::chain_hash_detail

#endif
