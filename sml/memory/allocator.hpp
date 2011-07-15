#ifndef _SML_MEMORY_ALLOCATOR_HPP
#define _SML_MEMORY_ALLOCATOR_HPP

#include <cstddef>
#include "sml/utility/noncopyable.hpp"

namespace sml { namespace memory {

template<class Alloc>
class allocator : Alloc {

  typedef Alloc base_allocator_type;

public:
  typedef typename base_allocator_type::value_type      value_type;
  typedef typename base_allocator_type::pointer         pointer;
  typedef typename base_allocator_type::reference       reference;
  typedef typename base_allocator_type::const_pointer   const_pointer;
  typedef typename base_allocator_type::const_reference const_reference;
  typedef typename base_allocator_type::size_type       size_type ;
  typedef typename base_allocator_type::difference_type difference_type;

  template<class _T>
  struct rebind {
    typedef
      allocator<typename base_allocator_type::template rebind<_T>::other>
      other;
  };

  template<class _Alloc>
  explicit allocator(const _Alloc& allocator) :
    base_allocator_type(allocator),
    ptr_(),
    constructed_end_(),
    size_() {
  }

  allocator() :
    base_allocator_type(),
    ptr_(),
    constructed_end_(),
    size_() {
  }

  ~allocator() {
    if (this->ptr_) {
      for (pointer it = this->ptr_; it != this->constructed_end_; ++it) {
        this->base_allocator_type::destroy(it);
      }
      this->deallocate(this->ptr_, this->size_);
    }
  }

  void construct(const size_type size, const value_type& base) {
    this->ptr_  = this->allocate(size);
    this->size_ = size;

    const pointer end = this->ptr_ + static_cast<std::ptrdiff_t>(size);
    for (
      this->constructed_end_ = this->ptr_;
      this->constructed_end_ != end;
      ++this->constructed_end_
    ) {
      this->base_allocator_type::construct(this->constructed_end_, base);
    }
  }

  pointer get() {
    return this->ptr_;
  }

  pointer release() {
    pointer ptr = this->ptr_;

    this->ptr_ = pointer();
    this->constructed_end_ = pointer();
    this->size_ = size_type();

    return ptr;
  }

  void destroy(const size_type size, const pointer ptr) {
    const pointer end = ptr + static_cast<std::ptrdiff_t>(size);

    for (pointer it = ptr; it != end; ++it) {
      this->base_allocator_type::destroy(it);
    }

    this->deallocate(ptr, size);
  }

private:
  pointer   ptr_;
  pointer   constructed_end_;
  size_type size_;

};

}} // namespace

#endif
