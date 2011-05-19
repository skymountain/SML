#ifndef _SML_SORT_HPP
#define _SML_SORT_HPP

#include <iterator>
#include <utility>
#include "sml/op/lesser.hpp"
#include "sml/sort/insertion_sort.hpp"

namespace sml { namespace detail {

template<class Iterator, class Lesser>
void _sort(const Iterator begin, const Iterator end, Lesser lesser) {
  typedef
    typename std::iterator_traits<Iterator>::difference_type
    difference_type;

  const difference_type THRESHOLD = 21;
  difference_type l = 0, r = end - begin - 1;

  while (r - l >= THRESHOLD) {
    const difference_type m  = l + (r-l)/2;
    const Iterator left      = begin + l;
    const Iterator right     = begin + r;
    const Iterator middle    = begin + m;
    const Iterator pre_pivot =
      lesser(*left, *middle) ? (lesser(*middle, *right)? middle :
                                (lesser(*right,  *left)  ? left  : right)) :
                               (lesser(*left, *right)  ? left   :
                                (lesser(*middle, *right) ? right : middle));
    std::swap(*pre_pivot, *right);

    Iterator pivot = left;
    for (Iterator it = left; it != right; ++it) {
      if (lesser(*it, *right)) {
        std::swap(*pivot, *it);
        ++pivot;
      }
    }

    std::swap(*pivot, *right);
    const difference_type left_size  =  pivot - left;
    const difference_type right_size = right - pivot;

    if (right_size < left_size) {
      _sort(pivot+1, right+1, lesser);
      r = l + left_size - 1;
    }
    else {
      _sort(left, pivot, lesser);
      l = r - right_size + 1;
    }
  }
}

} // namespace detail

template<class Iterator, class Lesser>
Iterator sort(const Iterator begin, const Iterator end, Lesser lesser) {
  detail::_sort(begin, end, lesser);
  return sml::sorting::insertion_sort(begin, end, lesser);
}

template<class Iterator>
Iterator sort(const Iterator begin, const Iterator end) {
  return sml::sort(begin, end, sml::op::lesser());
}

} // namespace sml

#endif
