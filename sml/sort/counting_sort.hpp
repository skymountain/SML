#ifndef _SML_SORT_COUNTING_HPP
#define _SML_SORT_COUNTING_HPP

#include <vector>
#include <iterator>
#include "sml/ext/functional.hpp"
#include "sml/iterator/next.hpp"
#include "sml/iterator/prior.hpp"

namespace sml { namespace sorting {

template<class CountType, class InputIterator, class RandomAccessIterator, class Encoder>
RandomAccessIterator
counting_sort(
  const InputIterator  begin,
  const InputIterator  end,
  const RandomAccessIterator result,
  Encoder              encoder
) {
  using sml::iterator::next;
  using sml::iterator::prior;

  typedef typename std::iterator_traits<InputIterator>::value_type value_type;
  typedef typename sml::ext::result_of<Encoder(value_type)>::type encode_type;
  typedef
    typename std::vector<CountType>::iterator
    count_iterator;

  if (begin == end) return result;

  std::vector<CountType> count;
  for (InputIterator it = begin; it != end; ++it) {
    const encode_type idx = encoder(*it);
    const encode_type size = idx + 1;

    if (count.size() < size) {
      count.resize(size);
    }

    ++count[idx];
  }

  const count_iterator count_end = count.end();
  for (count_iterator it = next(count.begin()); it != count_end; ++it) {
    *it += *prior(it);
  }

  for (InputIterator it = end; it != begin; --it) {
    const InputIterator target = prior(it);
    const encode_type count_idx = encoder(*target);
    const CountType idx = --count[count_idx];

    *(result + idx) = *target;
  }

  return result;
}

template<class InputIterator, class RandomAccessIterator, class Encoder>
RandomAccessIterator
counting_sort(
  const InputIterator  begin,
  const InputIterator  end,
  const RandomAccessIterator result,
  Encoder              encoder
) {
  return sml::sorting::counting_sort<
    long, InputIterator, RandomAccessIterator, Encoder
  >(begin, end, result, encoder);
}

}} // namespace sml::sorting

#endif
