#ifndef _SML_CONTAINER_CHAIN_HASH_MAP_TYPES_HPP
#define _SML_CONTAINER_CHAIN_HASH_MAP_TYPES_HPP

#include <utility>
#include <cstddef>
#include "sml/memory/allocator.hpp"

namespace sml { namespace container { namespace chain_hash_detail {

template<class Types>
class element;

template<class Types>
class bucket;

template<class Types>
class bucket_iterator_base;

template<class Types>
class bucket_iterator;

template<class Types>
class const_bucket_iterator;

template<class Types>
class table_iterator_base;

template<class Types>
class table_iterator;

template<class Types>
class const_table_iterator;

template<class Key, class T, class Hash, class Pred, class Alloc>
struct map_types {

  typedef Key   key_type;
  typedef T     mapped_type;
  typedef Hash  hasher;
  typedef Pred  key_equal;
  typedef Alloc allocator_type;

  typedef std::pair<key_type const, mapped_type> value_type;
  typedef value_type const                       const_value_type;

  typedef value_type*       pointer;
  typedef value_type const* const_pointer;
  typedef value_type&       reference;
  typedef value_type const& const_reference;

  typedef std::ptrdiff_t    difference_type;
  typedef std::size_t       size_type;

  typedef
    sml::container::chain_hash_detail::element<map_types>
    element_type;
  typedef element_type*                   element_ptr;
  typedef element_type const*             const_element_ptr;

  typedef
    sml::container::chain_hash_detail::bucket<map_types>
    bucket_type;
  typedef bucket_type*                    bucket_ptr;
  typedef bucket_type const*              const_bucket_ptr;
  
  typedef
    sml::memory::allocator<
      typename allocator_type::template rebind<bucket_type>::other
    >
    bucket_allocator_type;

  typedef
    sml::memory::allocator<
      typename allocator_type::template rebind<element_type>::other
    >
    element_allocator_type;

  typedef
    typename sml::container::chain_hash_detail::bucket_iterator_base<
      map_types
    >
    bucket_iterator_base;

  typedef
    typename sml::container::chain_hash_detail::bucket_iterator<
      map_types
    >
    bucket_iterator;

  typedef
    typename sml::container::chain_hash_detail::const_bucket_iterator<
      map_types
    >
    const_bucket_iterator;

  typedef
    typename sml::container::chain_hash_detail::table_iterator_base<
      map_types
    >
    table_iterator_base;

  typedef
    typename sml::container::chain_hash_detail::table_iterator<
      map_types
    >
    table_iterator;

  typedef
    typename sml::container::chain_hash_detail::const_table_iterator<
      map_types
    >
    const_table_iterator;
};

}}} // namespace sml::container::chain_hash_detail

#endif
