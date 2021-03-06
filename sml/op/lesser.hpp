#ifndef _SML_OP_LESSER_HPP
#define _SML_OP_LESSER_HPP

namespace sml { namespace op {

class lesser {
public:

  typedef bool result_type;

  template<class T>
  bool operator()(T& a, T& b) const {
    return a < b;
  }

  template<class T>
  bool operator()(const T& a, const T& b) const {
    return a < b;
  }

}; // class lesser

}} // namespace sml::op

#endif
