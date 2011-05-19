#ifndef _SML_OP_COMPARATOR_HPP
#define _SML_OP_COMPARATOR_HPP

namespace sml { namespace op {

class comparator {
public:

  typedef int result_type;

  template<class T>
  int operator()(T& a, T& b) const {
    return a < b ? -1 : a > b ? 1 : 0;
  }

  template<class T>
  int operator()(const T& a, const T& b) const {
    return a < b ? -1 : a > b ? 1 : 0;
  }
}; // class comparator

}} // namespace sml::op

#endif
