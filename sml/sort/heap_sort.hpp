#ifndef _SML_SORT_HEAP_HPP
#define _SML_SORT_HEAP_HPP

#include <utility>
#include "sml/op/lesser.hpp"

namespace sml { namespace sorting {

template<class Iterator, class Lesser>
Iterator
heap_sort(const Iterator begin, const Iterator end, Lesser lesser) {
  typedef
    typename std::iterator_traits<Iterator>::difference_type
    difference_type;

  const difference_type n = end - begin;

  for (difference_type i = 1; i < n; ++i) {
    for (difference_type j = i; j >= 1; j = (j-1)/2) {
      const Iterator iter  = begin + j;
      const Iterator piter = begin + (j-1)/2;
      if (lesser(*iter, *piter)) break;
      std::swap(*iter, *piter);
    }
  }

  for (difference_type i = n-1; i >= 1; --i) {
    std::swap(*(begin+i), *begin);
    difference_type j = 0;
    while (2*j + 1 < i) {
      difference_type max_idx = j;
      const difference_type l_idx = 2*j + 1;
      const difference_type r_idx = 2*j + 2;

      if (lesser(*(begin+j), *(begin+l_idx))) {
        max_idx = l_idx;
      }
      if (r_idx < i && lesser(*(begin+max_idx), *(begin+r_idx))) {
        max_idx = r_idx;
      }

      if (max_idx == j) {
        break;
      }

      std::swap(*(begin+max_idx), *(begin+j));
      j = max_idx;
    }
  }

  return begin;
}

template<class Iterator>
Iterator heap_sort(const Iterator begin, const Iterator end) {
  return sml::sorting::heap_sort(begin, end, sml::op::lesser());
}

}} // namespace sml::sorting

#endif
