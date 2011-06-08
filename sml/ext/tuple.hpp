#ifndef _SML_EXT_TUPLE_HPP
#define _SML_EXT_TUPLE_HPP

#ifdef __GNUC__
#if __GNUC__ == 4 && __GNUC_MINOR__ <= 6
#include <tr1/tuple>
namespace sml { namespace ext {

using std::tr1::tuple;
using std::tr1::make_tuple;
using std::tr1::get;

}} // namespace sml::ext
#endif
#endif

#endif
