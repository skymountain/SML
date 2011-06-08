#ifndef _SML_UTILITY_NONCOPYABLE_HPP
#define _SML_UTILITY_NONCOPYABLE_HPP

namespace sml { namespace utility {

class noncopyable {

protected:

  noncopyable() {}
  ~noncopyable() {}

private:
  noncopyable(const noncopyable&);
  const noncopyable& operator=(const noncopyable&);
};

}} // namespace sml::utility

#endif
