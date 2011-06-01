#ifndef _SML_ALGORITHM_MIN_MAX_HPP
#define _SML_ALGORITHM_MIN_MAX_HPP

#include <iterator>
#include <utility>
#include "sml/op/lesser.hpp"

namespace sml { namespace algorithm {

template<class Iterator, class Lesser>
std::pair<Iterator, Iterator>
min_max(const Iterator begin, const Iterator end, Lesser lesser) {
  using std::swap;
  typedef
    typename std::iterator_traits<Iterator>::difference_type
    difference_type;
  
  difference_type n = end - begin;
  if (n == 0) return std::make_pair(end, end);
  if (n == 1) return std::make_pair(begin, begin);

  Iterator min = begin, max = begin, it = begin;
  if (n % 2 == 0) {
    ++max, ++it;
    if (lesser(*max, *min)) {
      swap(max, min);
    }
  }

  for (++it; it != end; ++it) {
    Iterator jt = it++;
    if (lesser(*jt, *it)) {
      if (lesser(*jt, *min)) min = jt;
      if (lesser(*max, *it)) max = it;
    }
    else {
      if (lesser(*it, *min)) min = it;
      if (lesser(*max, *jt)) max = jt;
    }
  }

  return std::make_pair(min, max);
}

template<class Iterator>
std::pair<Iterator, Iterator>
min_max(const Iterator begin, const Iterator end) {
  return sml::algorithm::min_max(begin, end, sml::op::lesser());
}

}} // nemespace sml::algorithm

#endif
