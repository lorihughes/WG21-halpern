/* copy_swap_helper.h                  -*-C++-*-
 *
 * Pablo Halpern, January 2016
 */

#ifndef INCLUDED_COPY_SWAP_HELPER_DOT_H
#define INCLUDED_COPY_SWAP_HELPER_DOT_H

#include <utility>
#include <memory>
#include <cstdlib>

namespace std {

inline namespace cpp17 {

template <class...> struct void_t_imp { typedef void type; };
template <class... T> using void_t = typename void_t_imp<T...>::type;

} // close cpp17

namespace experimental {
inline namespace fundamentals_v3 {

namespace internal {

template <typename T, typename A, typename = void_t<>>
struct uses_prefix_allocator : false_type { };

template <typename T, typename A>
struct uses_prefix_allocator<T,A,void_t<decltype(T(allocator_arg, declval<A>(),
                                                   declval<T>()))>>
    : true_type { };
          
template <typename T, typename A, typename V>
struct uses_prefix_allocator<T&,A,V> : uses_prefix_allocator<T,A> { };

template <typename T, typename A>
constexpr bool uses_prefix_allocator_v = uses_prefix_allocator<T,A>::value;
    
template <typename T, typename A, typename = void_t<>>
struct uses_suffix_allocator : false_type { };

template <typename T, typename A>
struct uses_suffix_allocator<T,A,void_t<decltype(T(declval<T>(), 
                                                   declval<A>()))>>
    : true_type { };

template <typename T, typename A>
constexpr bool uses_suffix_allocator_v = uses_suffix_allocator<T,A>::value;
    
template <typename T, typename = void_t<>>
struct has_get_allocator
    : false_type { };

template <typename T>
struct has_get_allocator<T, void_t<decltype(declval<T>().get_allocator())>>
    : true_type { };

template <typename T>
constexpr bool has_get_allocator_v = has_get_allocator<T>::value;

template <typename T, typename = void_t<> >
struct has_get_memory_resource
    : false_type { };

template <typename T>
struct has_get_memory_resource<T, 
                       void_t<decltype(declval<T>().get_memory_resource())>>
    : true_type { };

template <typename T>
constexpr bool has_get_memory_resource_v = has_get_memory_resource<T>::value;

template <typename Type, typename Alloc>
inline
typename enable_if< uses_prefix_allocator_v<remove_reference_t<Type>,Alloc>, 
                   remove_reference_t<Type>>::type
copy_swap_helper_imp(Type&& other, const Alloc& alloc)
{
    typedef remove_reference_t<Type> ObjType;
    return ObjType(allocator_arg, alloc, std::forward<Type>(other));
}

template <typename Type, typename Alloc>
inline
typename enable_if< uses_suffix_allocator_v<remove_reference_t<Type>,Alloc> &&
                   !uses_prefix_allocator_v<remove_reference_t<Type>,Alloc>,
                   remove_reference_t<Type>>::type
copy_swap_helper_imp(Type&& other, const Alloc& alloc)
{
    typedef remove_reference_t<Type> ObjType;
    return ObjType(std::forward<Type>(other), alloc);
}

template <typename Type, typename Alloc>
inline
typename enable_if<!uses_suffix_allocator_v<remove_reference_t<Type>,Alloc> &&
                   !uses_prefix_allocator_v<remove_reference_t<Type>,Alloc>,
                   remove_reference_t<Type>>::type
copy_swap_helper_imp(Type&& other, const Alloc&)
{
    static_assert(! uses_allocator<remove_reference_t<Type>,Alloc>::value,
                  "Type uses allocator but doesn't have appriate constructor");
    return std::forward<Type>(other);
}

} // close internal namespace

template <class T>
inline
typename std::enable_if< internal::has_get_memory_resource_v<T>,
                        remove_reference_t<T>>::type
copy_swap_helper(T&& other, remove_reference_t<T> const& alloc_source)
{
    using internal::copy_swap_helper_imp;
    return copy_swap_helper_imp(std::forward<T>(other),
                                alloc_source.get_memory_resource());
}

template <class T>
inline
typename enable_if< internal::has_get_allocator_v<T> &&
                   !internal::has_get_memory_resource_v<T>, 
                   remove_reference_t<T>>::type
copy_swap_helper(T&& other, remove_reference_t<T> const& alloc_source)
{
    using internal::copy_swap_helper_imp;
    return copy_swap_helper_imp(std::forward<T>(other),
                                alloc_source.get_allocator());
}

template <class T>
inline
typename enable_if<!internal::has_get_allocator_v<T> &&
                   !internal::has_get_memory_resource_v<T>,
                   remove_reference_t<T>>::type
copy_swap_helper(T&& other, remove_reference_t<T> const& alloc_source)
{
    return std::forward<T>(other);
}

template <class T>
inline
remove_reference_t<T> copy_swap_helper(T&& other)
{
    return copy_swap_helper(std::forward<T>(other), other);
}

template <class T>
inline
T& copy_swap(remove_reference_t<T>& lhs, T&& rhs)
{
    using std::swap;
    auto tmp = copy_swap_helper(std::forward<T>(rhs), lhs);
    swap(lhs, tmp);
    return lhs;
}
           
} // close fundamentals_v3
} // close experimental
} // close std

#endif // ! defined(INCLUDED_COPY_SWAP_HELPER_DOT_H)
