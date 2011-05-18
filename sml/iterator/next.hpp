#ifndef _SML_ITERATOR_NEXT_HPP
#define _SML_ITERATOR_NEXT_HPP

namespace sml { namespace iterator {

template<class Iterator>
Iterator next(Iterator iter) {
  return ++iter;
}

}} // namespace sml::iterator

#endif
