#ifndef _SML_EXT_MEMORY_HPP
#define _SML_EXT_MEMORY_HPP

#ifdef __GNUC__
#if __GNUC__ == 4 && __GNUC_MINOR__ <= 6
#include <tr1/memory>
namespace sml { namespace ext {

using std::tr1::shared_ptr;
using std::tr1::weak_ptr;
using std::tr1::enable_shared_from_this;

}} // namespace sml::ext
#endif
#endif

#endif
