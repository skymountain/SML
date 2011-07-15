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

public:
  bucket() :
    head_() {
  }

  element_ptr       head() { return this->head_; }
  const_element_ptr head() const { return this->head_; }

  void head(element_ptr head) { this->head_ = head; }

  void push_front(element_ptr const elem) {
    elem->next(this->head());
    elem->prev(element_ptr());

    this->head(elem);
  }

  void erase(element_ptr const elem) {
    element_ptr const prev = elem->prev();
    element_ptr const next = elem->next();

    if (prev) prev->next(next);
    else this->head(next);

    if (next) next->prev(prev);
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
