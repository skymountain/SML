#ifndef _SML_RANDOMIZE_HPP
#define _SML_RANDOMIZE_HPP

#include <iterator>
#include <utility>

namespace sml {

template<class Iterator, class Random>
Iterator randomize (const Iterator begin, const Iterator end, Random& rand) {
  using std::swap;
  typedef
    typename std::iterator_traits<Iterator>::difference_type
    difference_type;

  for (difference_type i = end - begin - 1; i > 0; --i) {
    swap(*(begin+i), *(begin+(rand() % (i+1))));
  }
  return begin;
}

} // namespace sml

#endif
