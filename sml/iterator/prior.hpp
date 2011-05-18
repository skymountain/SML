#ifndef _SML_ITERATOR_PRIOR_HPP
#define _SML_ITERATOR_PRIOR_HPP

namespace sml { namespace iterator  {

template<class Iterator>
Iterator prior(Iterator iter) {
  return --iter;
}

}} // namespace

#endif
