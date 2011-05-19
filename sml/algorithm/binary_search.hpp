#ifndef _SML_ALGORITHM_BINARY_SEARCH_HPP
#define _SML_ALGORITHM_BINARY_SEARCH_HPP

#include <iterator>
#include "sml/ext/functional.hpp"
#include "sml/comparator.hpp"

namespace sml { namespace algorithm {

template<class Iterator, class T, class Comparator>
Iterator
binary_search(
  const Iterator begin,
  const Iterator end,
  const T&       v,
  Comparator     cmp
) {
  typedef typename std::iterator_traits<Iterator> traits_type;
  typedef typename traits_type::difference_type difference_type;
  typedef typename traits_type::value_type      value_type;
  typedef
    typename ext::result_of<Comparator(T, value_type)>::type
    result_type;

  const difference_type n = end - begin;
  if (n == 0) return end;

  difference_type last  = n-1;
  difference_type first = 0;

  while (first <= last) {
    const difference_type medium = first + (last - first) / 2;
    const Iterator m = begin + medium;
    const result_type result = cmp(v, *m);

    if (result < 0)
      last = medium - 1;
    else if (result > 0)
      first = medium + 1;
    else
      return m;
  }

  return end;
}

template<class Iterator, class T>
Iterator binary_search(const Iterator begin, const Iterator end, const T& v) {
  return sml::algorithm::binary_search(begin, end, v, comparator());
}

}} // namespace sml::algorithm

#endif
