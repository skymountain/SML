#ifndef _SML_STD_FUNCTIONAL_HPP
#define _SML_STD_FUNCTIONAL_HPP

#ifdef __GNUC__
#if __GNUC__ == 4 && __GNUC_MINOR__ <= 6
#include <tr1/functional>
namespace sml { namespace ext {

using std::tr1::result_of;

}} // namespace sml::ext
#endif
#endif

#endif
