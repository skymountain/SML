#ifndef _SML_ALGORITHM_BINARY_SEARCH_MIN_MAX_HPP
#define _SML_ALGORITHM_BINARY_SEARCH_MIN_MAX_HPP

#include <utility>
#include "sml/ext/functional.hpp"
#include "sml/op/comparator.hpp"

namespace sml { namespace algorithm {

template<class Iterator, class T, class Comparator>
std::pair<Iterator, Iterator>
binary_search_min_max(
  const Iterator begin,
  const Iterator end,
  const T&       v,
  Comparator     cmp
) {
  using sml::algorithm::binary_search_min_max;

  typedef typename std::iterator_traits<Iterator> traits_type;
  typedef typename traits_type::difference_type difference_type;
  typedef typename traits_type::value_type      value_type;
  typedef
    typename sml::ext::result_of<Comparator(T, value_type)>::type
    result_type;

  const difference_type n = end - begin;
  if (n == 0) return std::make_pair(end, end);

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
    // v == *m
    else {
      const Iterator f = binary_search_min_max(begin+first, m, v, cmp).first;

      const Iterator l_end = begin + last + 1;
      Iterator l = binary_search_min_max(m+1, l_end, v, cmp).second;
        
      if (l == l_end) l = m;

      return std::make_pair(f, l);
    }
  }

  return std::make_pair(end, end);
}

template<class Iterator, class T>
std::pair<Iterator, Iterator>
binary_search_min_max(const Iterator begin, const Iterator end, const T& v) {
  return
    sml::algorithm::binary_search_min_max(begin, end, v, sml::op::comparator());
}

}} // namespace sml::algorithm

#endif
