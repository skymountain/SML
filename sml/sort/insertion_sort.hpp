#ifndef _SML_SORT_INSERTION_SORT_HPP
#define _SML_SORT_INSERTION_SORT_HPP

#include <algorithm>
#include <iterator>
#include "sml/lesser.hpp"
#include "sml/iterator/next.hpp"
#include "sml/iterator/prior.hpp"

namespace sml { namespace sorting {

template<class Iterator, class Lesser>
Iterator
insertion_sort(const Iterator begin, const Iterator end, Lesser lesser) {
  using iterator::next;
  using iterator::prior;

  typedef typename std::iterator_traits<Iterator>::value_type value_type;

  if (begin == end) return begin;

  for (Iterator it = next(begin); it != end; ++it) {
    value_type v = *it;

    if (lesser(v, *begin)) {
      std::copy_backward(begin, it, next(it));
      *begin = v;
    }
    else {
      Iterator  jt;
      for (jt = prior(it); lesser(v, *jt); --jt) {
        *next(jt) = *jt;
      }
      *next(jt) = v;
    }
  }

  return begin;
}

template<class Iterator>
Iterator insertion_sort(const Iterator begin, const Iterator end) {
  return sml::sorting::insertion_sort(begin, end, lesser());
}

}} // namespace sml::sorting

#endif
