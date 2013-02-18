// <functional> -*- C++ -*-

// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
// 2011 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/*
 * Copyright (c) 1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */

/** @file include/functional
 *  This is a Standard C++ Library header.
 */

#ifndef _XFUNCTIONAL
#define _XFUNCTIONAL 1

//#pragma GCC system_header

#include <utility>
#include <memory>
#include <bits/c++config.h>
// #include <bits/stl_function.h>

#include <typeinfo>
#include <new>
#include <tuple>
#include <type_traits>
#include <bits/functexcept.h>
#include <bits/functional_hash.h>
#include "polymorphic_allocator.h"

BEGIN_NAMESPACE_XSTD

_GLIBCXX_BEGIN_NAMESPACE_VERSION

_GLIBCXX_HAS_NESTED_TYPE(result_type)

  /// If we have found a result_type, extract it.
  template<bool _Has_result_type, typename _Functor>
    struct _Maybe_get_result_type
    { };

  template<typename _Functor>
    struct _Maybe_get_result_type<true, _Functor>
    { typedef typename _Functor::result_type result_type; };

  /**
   *  Base class for any function object that has a weak result type, as
   *  defined in 3.3/3 of TR1.
  */
  template<typename _Functor>
    struct _Weak_result_type_impl
    : _Maybe_get_result_type<__has_result_type<_Functor>::value, _Functor>
    { };

  /// Retrieve the result type for a function type.
  template<typename _Res, typename... _ArgTypes>
    struct _Weak_result_type_impl<_Res(_ArgTypes...)>
    { typedef _Res result_type; };

  template<typename _Res, typename... _ArgTypes>
    struct _Weak_result_type_impl<_Res(_ArgTypes......)>
    { typedef _Res result_type; };

  template<typename _Res, typename... _ArgTypes>
    struct _Weak_result_type_impl<_Res(_ArgTypes...) const>
    { typedef _Res result_type; };

  template<typename _Res, typename... _ArgTypes>
    struct _Weak_result_type_impl<_Res(_ArgTypes......) const>
    { typedef _Res result_type; };

  template<typename _Res, typename... _ArgTypes>
    struct _Weak_result_type_impl<_Res(_ArgTypes...) volatile>
    { typedef _Res result_type; };

  template<typename _Res, typename... _ArgTypes>
    struct _Weak_result_type_impl<_Res(_ArgTypes......) volatile>
    { typedef _Res result_type; };

  template<typename _Res, typename... _ArgTypes>
    struct _Weak_result_type_impl<_Res(_ArgTypes...) const volatile>
    { typedef _Res result_type; };

  template<typename _Res, typename... _ArgTypes>
    struct _Weak_result_type_impl<_Res(_ArgTypes......) const volatile>
    { typedef _Res result_type; };

  /// Retrieve the result type for a function reference.
  template<typename _Res, typename... _ArgTypes>
    struct _Weak_result_type_impl<_Res(&)(_ArgTypes...)>
    { typedef _Res result_type; };

  template<typename _Res, typename... _ArgTypes>
    struct _Weak_result_type_impl<_Res(&)(_ArgTypes......)>
    { typedef _Res result_type; };

  /// Retrieve the result type for a function pointer.
  template<typename _Res, typename... _ArgTypes>
    struct _Weak_result_type_impl<_Res(*)(_ArgTypes...)>
    { typedef _Res result_type; };

  template<typename _Res, typename... _ArgTypes>
    struct _Weak_result_type_impl<_Res(*)(_ArgTypes......)>
    { typedef _Res result_type; };

  /// Retrieve result type for a member function pointer.
  template<typename _Res, typename _Class, typename... _ArgTypes>
    struct _Weak_result_type_impl<_Res (_Class::*)(_ArgTypes...)>
    { typedef _Res result_type; };

  template<typename _Res, typename _Class, typename... _ArgTypes>
    struct _Weak_result_type_impl<_Res (_Class::*)(_ArgTypes......)>
    { typedef _Res result_type; };

  /// Retrieve result type for a const member function pointer.
  template<typename _Res, typename _Class, typename... _ArgTypes>
    struct _Weak_result_type_impl<_Res (_Class::*)(_ArgTypes...) const>
    { typedef _Res result_type; };

  template<typename _Res, typename _Class, typename... _ArgTypes>
    struct _Weak_result_type_impl<_Res (_Class::*)(_ArgTypes......) const>
    { typedef _Res result_type; };

  /// Retrieve result type for a volatile member function pointer.
  template<typename _Res, typename _Class, typename... _ArgTypes>
    struct _Weak_result_type_impl<_Res (_Class::*)(_ArgTypes...) volatile>
    { typedef _Res result_type; };

  template<typename _Res, typename _Class, typename... _ArgTypes>
    struct _Weak_result_type_impl<_Res (_Class::*)(_ArgTypes......) volatile>
    { typedef _Res result_type; };

  /// Retrieve result type for a const volatile member function pointer.
  template<typename _Res, typename _Class, typename... _ArgTypes>
    struct _Weak_result_type_impl<_Res (_Class::*)(_ArgTypes...)
				  const volatile>
    { typedef _Res result_type; };

  template<typename _Res, typename _Class, typename... _ArgTypes>
    struct _Weak_result_type_impl<_Res (_Class::*)(_ArgTypes......)
				  const volatile>
    { typedef _Res result_type; };

  /**
   *  Strip top-level cv-qualifiers from the function object and let
   *  _Weak_result_type_impl perform the real work.
  */
  template<typename _Functor>
    struct _Weak_result_type
    : _Weak_result_type_impl<typename remove_cv<_Functor>::type>
    { };

  /// Determines if the type _Tp derives from unary_function.
  template<typename _Tp>
    struct _Derives_from_unary_function : __sfinae_types
    {
    private:
      template<typename _T1, typename _Res>
	static __one __test(const volatile unary_function<_T1, _Res>*);

      // It's tempting to change "..." to const volatile void*, but
      // that fails when _Tp is a function type.
      static __two __test(...);

    public:
      static const bool value = sizeof(__test((_Tp*)0)) == 1;
    };

  /// Determines if the type _Tp derives from binary_function.
  template<typename _Tp>
    struct _Derives_from_binary_function : __sfinae_types
    {
    private:
      template<typename _T1, typename _T2, typename _Res>
	static __one __test(const volatile binary_function<_T1, _T2, _Res>*);

      // It's tempting to change "..." to const volatile void*, but
      // that fails when _Tp is a function type.
      static __two __test(...);

    public:
      static const bool value = sizeof(__test((_Tp*)0)) == 1;
    };

  /**
   * Invoke a function object, which may be either a member pointer or a
   * function object. The first parameter will tell which.
   */
  template<typename _Functor, typename... _Args>
    inline
    typename enable_if<
	     (!is_member_pointer<_Functor>::value
	      && !is_function<_Functor>::value
	      && !is_function<typename remove_pointer<_Functor>::type>::value),
	     typename result_of<_Functor(_Args&&...)>::type
	   >::type
    __invoke(_Functor& __f, _Args&&... __args)
    {
      return __f(std::forward<_Args>(__args)...);
    }

  template<typename _Functor, typename... _Args>
    inline
    typename enable_if<
             (is_member_pointer<_Functor>::value
              && !is_function<_Functor>::value
              && !is_function<typename remove_pointer<_Functor>::type>::value),
             typename result_of<_Functor(_Args&&...)>::type
           >::type
    __invoke(_Functor& __f, _Args&&... __args)
    {
      return mem_fn(__f)(std::forward<_Args>(__args)...);
    }

  // To pick up function references (that will become function pointers)
  template<typename _Functor, typename... _Args>
    inline
    typename enable_if<
	     (is_pointer<_Functor>::value
	      && is_function<typename remove_pointer<_Functor>::type>::value),
	     typename result_of<_Functor(_Args&&...)>::type
	   >::type
    __invoke(_Functor __f, _Args&&... __args)
    {
      return __f(std::forward<_Args>(__args)...);
    }

  /**
   *  Knowing which of unary_function and binary_function _Tp derives
   *  from, derives from the same and ensures that reference_wrapper
   *  will have a weak result type. See cases below.
   */
  template<bool _Unary, bool _Binary, typename _Tp>
    struct _Reference_wrapper_base_impl;

  // None of the nested argument types.
  template<typename _Tp>
    struct _Reference_wrapper_base_impl<false, false, _Tp>
    : _Weak_result_type<_Tp>
    { };

  // Nested argument_type only.
  template<typename _Tp>
    struct _Reference_wrapper_base_impl<true, false, _Tp>
    : _Weak_result_type<_Tp>
    {
      typedef typename _Tp::argument_type argument_type;
    };

  // Nested first_argument_type and second_argument_type only.
  template<typename _Tp>
    struct _Reference_wrapper_base_impl<false, true, _Tp>
    : _Weak_result_type<_Tp>
    {
      typedef typename _Tp::first_argument_type first_argument_type;
      typedef typename _Tp::second_argument_type second_argument_type;
    };

  // All the nested argument types.
   template<typename _Tp>
    struct _Reference_wrapper_base_impl<true, true, _Tp>
    : _Weak_result_type<_Tp>
    {
      typedef typename _Tp::argument_type argument_type;
      typedef typename _Tp::first_argument_type first_argument_type;
      typedef typename _Tp::second_argument_type second_argument_type;
    };

  _GLIBCXX_HAS_NESTED_TYPE(argument_type)
  _GLIBCXX_HAS_NESTED_TYPE(first_argument_type)
  _GLIBCXX_HAS_NESTED_TYPE(second_argument_type)

  /**
   *  Derives from unary_function or binary_function when it
   *  can. Specializations handle all of the easy cases. The primary
   *  template determines what to do with a class type, which may
   *  derive from both unary_function and binary_function.
  */
  template<typename _Tp>
    struct _Reference_wrapper_base
    : _Reference_wrapper_base_impl<
      __has_argument_type<_Tp>::value,
      __has_first_argument_type<_Tp>::value
      && __has_second_argument_type<_Tp>::value,
      _Tp>
    { };

  // - a function type (unary)
  template<typename _Res, typename _T1>
    struct _Reference_wrapper_base<_Res(_T1)>
    : unary_function<_T1, _Res>
    { };

  template<typename _Res, typename _T1>
    struct _Reference_wrapper_base<_Res(_T1) const>
    : unary_function<_T1, _Res>
    { };

  template<typename _Res, typename _T1>
    struct _Reference_wrapper_base<_Res(_T1) volatile>
    : unary_function<_T1, _Res>
    { };

  template<typename _Res, typename _T1>
    struct _Reference_wrapper_base<_Res(_T1) const volatile>
    : unary_function<_T1, _Res>
    { };

  // - a function type (binary)
  template<typename _Res, typename _T1, typename _T2>
    struct _Reference_wrapper_base<_Res(_T1, _T2)>
    : binary_function<_T1, _T2, _Res>
    { };

  template<typename _Res, typename _T1, typename _T2>
    struct _Reference_wrapper_base<_Res(_T1, _T2) const>
    : binary_function<_T1, _T2, _Res>
    { };

  template<typename _Res, typename _T1, typename _T2>
    struct _Reference_wrapper_base<_Res(_T1, _T2) volatile>
    : binary_function<_T1, _T2, _Res>
    { };

  template<typename _Res, typename _T1, typename _T2>
    struct _Reference_wrapper_base<_Res(_T1, _T2) const volatile>
    : binary_function<_T1, _T2, _Res>
    { };

  // - a function pointer type (unary)
  template<typename _Res, typename _T1>
    struct _Reference_wrapper_base<_Res(*)(_T1)>
    : unary_function<_T1, _Res>
    { };

  // - a function pointer type (binary)
  template<typename _Res, typename _T1, typename _T2>
    struct _Reference_wrapper_base<_Res(*)(_T1, _T2)>
    : binary_function<_T1, _T2, _Res>
    { };

  // - a pointer to member function type (unary, no qualifiers)
  template<typename _Res, typename _T1>
    struct _Reference_wrapper_base<_Res (_T1::*)()>
    : unary_function<_T1*, _Res>
    { };

  // - a pointer to member function type (binary, no qualifiers)
  template<typename _Res, typename _T1, typename _T2>
    struct _Reference_wrapper_base<_Res (_T1::*)(_T2)>
    : binary_function<_T1*, _T2, _Res>
    { };

  // - a pointer to member function type (unary, const)
  template<typename _Res, typename _T1>
    struct _Reference_wrapper_base<_Res (_T1::*)() const>
    : unary_function<const _T1*, _Res>
    { };

  // - a pointer to member function type (binary, const)
  template<typename _Res, typename _T1, typename _T2>
    struct _Reference_wrapper_base<_Res (_T1::*)(_T2) const>
    : binary_function<const _T1*, _T2, _Res>
    { };

  // - a pointer to member function type (unary, volatile)
  template<typename _Res, typename _T1>
    struct _Reference_wrapper_base<_Res (_T1::*)() volatile>
    : unary_function<volatile _T1*, _Res>
    { };

  // - a pointer to member function type (binary, volatile)
  template<typename _Res, typename _T1, typename _T2>
    struct _Reference_wrapper_base<_Res (_T1::*)(_T2) volatile>
    : binary_function<volatile _T1*, _T2, _Res>
    { };

  // - a pointer to member function type (unary, const volatile)
  template<typename _Res, typename _T1>
    struct _Reference_wrapper_base<_Res (_T1::*)() const volatile>
    : unary_function<const volatile _T1*, _Res>
    { };

  // - a pointer to member function type (binary, const volatile)
  template<typename _Res, typename _T1, typename _T2>
    struct _Reference_wrapper_base<_Res (_T1::*)(_T2) const volatile>
    : binary_function<const volatile _T1*, _T2, _Res>
    { };

  /**
   *  @brief Primary class template for reference_wrapper.
   *  @ingroup functors
   *  @{
   */
  template<typename _Tp>
    class reference_wrapper
    : public _Reference_wrapper_base<typename remove_cv<_Tp>::type>
    {
      _Tp* _M_data;

    public:
      typedef _Tp type;

      reference_wrapper(_Tp& __indata)
      : _M_data(std::__addressof(__indata))
      { }

      reference_wrapper(_Tp&&) = delete;

      reference_wrapper(const reference_wrapper<_Tp>& __inref):
      _M_data(__inref._M_data)
      { }

      reference_wrapper&
      operator=(const reference_wrapper<_Tp>& __inref)
      {
	_M_data = __inref._M_data;
	return *this;
      }

      operator _Tp&() const
      { return this->get(); }

      _Tp&
      get() const
      { return *_M_data; }

      template<typename... _Args>
	typename result_of<_Tp&(_Args&&...)>::type
	operator()(_Args&&... __args) const
	{
	  return __invoke(get(), std::forward<_Args>(__args)...);
	}
    };


  /// Denotes a reference should be taken to a variable.
  template<typename _Tp>
    inline reference_wrapper<_Tp>
    ref(_Tp& __t)
    { return reference_wrapper<_Tp>(__t); }

  /// Denotes a const reference should be taken to a variable.
  template<typename _Tp>
    inline reference_wrapper<const _Tp>
    cref(const _Tp& __t)
    { return reference_wrapper<const _Tp>(__t); }

  template<typename _Tp>
    void ref(const _Tp&&) = delete;

  template<typename _Tp>
    void cref(const _Tp&&) = delete;

  /// Partial specialization.
  template<typename _Tp>
    inline reference_wrapper<_Tp>
    ref(reference_wrapper<_Tp> __t)
    { return ref(__t.get()); }

  /// Partial specialization.
  template<typename _Tp>
    inline reference_wrapper<const _Tp>
    cref(reference_wrapper<_Tp> __t)
    { return cref(__t.get()); }

  // @} group functors

  template<typename _MemberPointer>
    class _Mem_fn;

  /**
   * Derives from @c unary_function or @c binary_function, or perhaps
   * nothing, depending on the number of arguments provided. The
   * primary template is the basis case, which derives nothing.
   */
  template<typename _Res, typename... _ArgTypes>
    struct _Maybe_unary_or_binary_function { };

  /// Derives from @c unary_function, as appropriate.
  template<typename _Res, typename _T1>
    struct _Maybe_unary_or_binary_function<_Res, _T1>
    : std::unary_function<_T1, _Res> { };

  /// Derives from @c binary_function, as appropriate.
  template<typename _Res, typename _T1, typename _T2>
    struct _Maybe_unary_or_binary_function<_Res, _T1, _T2>
    : std::binary_function<_T1, _T2, _Res> { };

  /// Implementation of @c mem_fn for member function pointers.
  template<typename _Res, typename _Class, typename... _ArgTypes>
    class _Mem_fn<_Res (_Class::*)(_ArgTypes...)>
    : public _Maybe_unary_or_binary_function<_Res, _Class*, _ArgTypes...>
    {
      typedef _Res (_Class::*_Functor)(_ArgTypes...);

      template<typename _Tp>
	_Res
	_M_call(_Tp& __object, const volatile _Class *,
		_ArgTypes... __args) const
	{ return (__object.*__pmf)(std::forward<_ArgTypes>(__args)...); }

      template<typename _Tp>
	_Res
	_M_call(_Tp& __ptr, const volatile void *, _ArgTypes... __args) const
	{ return ((*__ptr).*__pmf)(std::forward<_ArgTypes>(__args)...); }

    public:
      typedef _Res result_type;

      explicit _Mem_fn(_Functor __pmf) : __pmf(__pmf) { }

      // Handle objects
      _Res
      operator()(_Class& __object, _ArgTypes... __args) const
      { return (__object.*__pmf)(std::forward<_ArgTypes>(__args)...); }

      // Handle pointers
      _Res
      operator()(_Class* __object, _ArgTypes... __args) const
      { return (__object->*__pmf)(std::forward<_ArgTypes>(__args)...); }

      // Handle smart pointers, references and pointers to derived
      template<typename _Tp>
	_Res
	operator()(_Tp& __object, _ArgTypes... __args) const
	{
	  return _M_call(__object, &__object,
	      std::forward<_ArgTypes>(__args)...);
	}

    private:
      _Functor __pmf;
    };

  /// Implementation of @c mem_fn for const member function pointers.
  template<typename _Res, typename _Class, typename... _ArgTypes>
    class _Mem_fn<_Res (_Class::*)(_ArgTypes...) const>
    : public _Maybe_unary_or_binary_function<_Res, const _Class*,
					     _ArgTypes...>
    {
      typedef _Res (_Class::*_Functor)(_ArgTypes...) const;

      template<typename _Tp>
	_Res
	_M_call(_Tp& __object, const volatile _Class *,
		_ArgTypes... __args) const
	{ return (__object.*__pmf)(std::forward<_ArgTypes>(__args)...); }

      template<typename _Tp>
	_Res
	_M_call(_Tp& __ptr, const volatile void *, _ArgTypes... __args) const
	{ return ((*__ptr).*__pmf)(std::forward<_ArgTypes>(__args)...); }

    public:
      typedef _Res result_type;

      explicit _Mem_fn(_Functor __pmf) : __pmf(__pmf) { }

      // Handle objects
      _Res
      operator()(const _Class& __object, _ArgTypes... __args) const
      { return (__object.*__pmf)(std::forward<_ArgTypes>(__args)...); }

      // Handle pointers
      _Res
      operator()(const _Class* __object, _ArgTypes... __args) const
      { return (__object->*__pmf)(std::forward<_ArgTypes>(__args)...); }

      // Handle smart pointers, references and pointers to derived
      template<typename _Tp>
	_Res operator()(_Tp& __object, _ArgTypes... __args) const
	{
	  return _M_call(__object, &__object,
	      std::forward<_ArgTypes>(__args)...);
	}

    private:
      _Functor __pmf;
    };

  /// Implementation of @c mem_fn for volatile member function pointers.
  template<typename _Res, typename _Class, typename... _ArgTypes>
    class _Mem_fn<_Res (_Class::*)(_ArgTypes...) volatile>
    : public _Maybe_unary_or_binary_function<_Res, volatile _Class*,
					     _ArgTypes...>
    {
      typedef _Res (_Class::*_Functor)(_ArgTypes...) volatile;

      template<typename _Tp>
	_Res
	_M_call(_Tp& __object, const volatile _Class *,
		_ArgTypes... __args) const
	{ return (__object.*__pmf)(std::forward<_ArgTypes>(__args)...); }

      template<typename _Tp>
	_Res
	_M_call(_Tp& __ptr, const volatile void *, _ArgTypes... __args) const
	{ return ((*__ptr).*__pmf)(std::forward<_ArgTypes>(__args)...); }

    public:
      typedef _Res result_type;

      explicit _Mem_fn(_Functor __pmf) : __pmf(__pmf) { }

      // Handle objects
      _Res
      operator()(volatile _Class& __object, _ArgTypes... __args) const
      { return (__object.*__pmf)(std::forward<_ArgTypes>(__args)...); }

      // Handle pointers
      _Res
      operator()(volatile _Class* __object, _ArgTypes... __args) const
      { return (__object->*__pmf)(std::forward<_ArgTypes>(__args)...); }

      // Handle smart pointers, references and pointers to derived
      template<typename _Tp>
	_Res
	operator()(_Tp& __object, _ArgTypes... __args) const
	{
	  return _M_call(__object, &__object,
	      std::forward<_ArgTypes>(__args)...);
	}

    private:
      _Functor __pmf;
    };

  /// Implementation of @c mem_fn for const volatile member function pointers.
  template<typename _Res, typename _Class, typename... _ArgTypes>
    class _Mem_fn<_Res (_Class::*)(_ArgTypes...) const volatile>
    : public _Maybe_unary_or_binary_function<_Res, const volatile _Class*,
					     _ArgTypes...>
    {
      typedef _Res (_Class::*_Functor)(_ArgTypes...) const volatile;

      template<typename _Tp>
	_Res
	_M_call(_Tp& __object, const volatile _Class *,
		_ArgTypes... __args) const
	{ return (__object.*__pmf)(std::forward<_ArgTypes>(__args)...); }

      template<typename _Tp>
	_Res
	_M_call(_Tp& __ptr, const volatile void *, _ArgTypes... __args) const
	{ return ((*__ptr).*__pmf)(std::forward<_ArgTypes>(__args)...); }

    public:
      typedef _Res result_type;

      explicit _Mem_fn(_Functor __pmf) : __pmf(__pmf) { }

      // Handle objects
      _Res
      operator()(const volatile _Class& __object, _ArgTypes... __args) const
      { return (__object.*__pmf)(std::forward<_ArgTypes>(__args)...); }

      // Handle pointers
      _Res
      operator()(const volatile _Class* __object, _ArgTypes... __args) const
      { return (__object->*__pmf)(std::forward<_ArgTypes>(__args)...); }

      // Handle smart pointers, references and pointers to derived
      template<typename _Tp>
	_Res operator()(_Tp& __object, _ArgTypes... __args) const
	{
	  return _M_call(__object, &__object,
	      std::forward<_ArgTypes>(__args)...);
	}

    private:
      _Functor __pmf;
    };


  template<typename _Tp, bool>
    struct _Mem_fn_const_or_non
    {
      typedef const _Tp& type;
    };

  template<typename _Tp>
    struct _Mem_fn_const_or_non<_Tp, false>
    {
      typedef _Tp& type;
    };

  template<typename _Res, typename _Class>
    class _Mem_fn<_Res _Class::*>
    {
      // This bit of genius is due to Peter Dimov, improved slightly by
      // Douglas Gregor.
      template<typename _Tp>
	_Res&
	_M_call(_Tp& __object, _Class *) const
	{ return __object.*__pm; }

      template<typename _Tp, typename _Up>
	_Res&
	_M_call(_Tp& __object, _Up * const *) const
	{ return (*__object).*__pm; }

      template<typename _Tp, typename _Up>
	const _Res&
	_M_call(_Tp& __object, const _Up * const *) const
	{ return (*__object).*__pm; }

      template<typename _Tp>
	const _Res&
	_M_call(_Tp& __object, const _Class *) const
	{ return __object.*__pm; }

      template<typename _Tp>
	const _Res&
	_M_call(_Tp& __ptr, const volatile void*) const
	{ return (*__ptr).*__pm; }

      template<typename _Tp> static _Tp& __get_ref();

      template<typename _Tp>
	static __sfinae_types::__one __check_const(_Tp&, _Class*);
      template<typename _Tp, typename _Up>
	static __sfinae_types::__one __check_const(_Tp&, _Up * const *);
      template<typename _Tp, typename _Up>
	static __sfinae_types::__two __check_const(_Tp&, const _Up * const *);
      template<typename _Tp>
	static __sfinae_types::__two __check_const(_Tp&, const _Class*);
      template<typename _Tp>
	static __sfinae_types::__two __check_const(_Tp&, const volatile void*);

    public:
      template<typename _Tp>
	struct _Result_type
	: _Mem_fn_const_or_non<_Res,
	  (sizeof(__sfinae_types::__two)
	   == sizeof(__check_const<_Tp>(__get_ref<_Tp>(), (_Tp*)0)))>
	{ };

      template<typename _Signature>
	struct result;

      template<typename _CVMem, typename _Tp>
	struct result<_CVMem(_Tp)>
	: public _Result_type<_Tp> { };

      template<typename _CVMem, typename _Tp>
	struct result<_CVMem(_Tp&)>
	: public _Result_type<_Tp> { };

      explicit
      _Mem_fn(_Res _Class::*__pm) : __pm(__pm) { }

      // Handle objects
      _Res&
      operator()(_Class& __object) const
      { return __object.*__pm; }

      const _Res&
      operator()(const _Class& __object) const
      { return __object.*__pm; }

      // Handle pointers
      _Res&
      operator()(_Class* __object) const
      { return __object->*__pm; }

      const _Res&
      operator()(const _Class* __object) const
      { return __object->*__pm; }

      // Handle smart pointers and derived
      template<typename _Tp>
	typename _Result_type<_Tp>::type
	operator()(_Tp& __unknown) const
	{ return _M_call(__unknown, &__unknown); }

    private:
      _Res _Class::*__pm;
    };

  /**
   *  @brief Returns a function object that forwards to the member
   *  pointer @a pm.
   *  @ingroup functors
   */
  template<typename _Tp, typename _Class>
    inline _Mem_fn<_Tp _Class::*>
    mem_fn(_Tp _Class::* __pm)
    {
      return _Mem_fn<_Tp _Class::*>(__pm);
    }

  /**
   *  @brief Determines if the given type _Tp is a function object
   *  should be treated as a subexpression when evaluating calls to
   *  function objects returned by bind(). [TR1 3.6.1]
   *  @ingroup binders
   */
  template<typename _Tp>
    struct is_bind_expression
    : public false_type { };

  /**
   *  @brief Determines if the given type _Tp is a placeholder in a
   *  bind() expression and, if so, which placeholder it is. [TR1 3.6.2]
   *  @ingroup binders
   */
  template<typename _Tp>
    struct is_placeholder
    : public integral_constant<int, 0>
    { };

  /// The type of placeholder objects defined by libstdc++.
  template<int _Num> struct _Placeholder { };

  _GLIBCXX_END_NAMESPACE_VERSION

  /** @namespace std::placeholders
   *  @brief ISO C++ 0x entities sub namespace for functional.
   *  @ingroup binders
   *
   *  Define a large number of placeholders. There is no way to
   *  simplify this with variadic templates, because we're introducing
   *  unique names for each.
   */
  namespace placeholders
  {
  _GLIBCXX_BEGIN_NAMESPACE_VERSION
    extern const _Placeholder<1> _1;
    extern const _Placeholder<2> _2;
    extern const _Placeholder<3> _3;
    extern const _Placeholder<4> _4;
    extern const _Placeholder<5> _5;
    extern const _Placeholder<6> _6;
    extern const _Placeholder<7> _7;
    extern const _Placeholder<8> _8;
    extern const _Placeholder<9> _9;
    extern const _Placeholder<10> _10;
    extern const _Placeholder<11> _11;
    extern const _Placeholder<12> _12;
    extern const _Placeholder<13> _13;
    extern const _Placeholder<14> _14;
    extern const _Placeholder<15> _15;
    extern const _Placeholder<16> _16;
    extern const _Placeholder<17> _17;
    extern const _Placeholder<18> _18;
    extern const _Placeholder<19> _19;
    extern const _Placeholder<20> _20;
    extern const _Placeholder<21> _21;
    extern const _Placeholder<22> _22;
    extern const _Placeholder<23> _23;
    extern const _Placeholder<24> _24;
    extern const _Placeholder<25> _25;
    extern const _Placeholder<26> _26;
    extern const _Placeholder<27> _27;
    extern const _Placeholder<28> _28;
    extern const _Placeholder<29> _29;
  _GLIBCXX_END_NAMESPACE_VERSION
  }

  _GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   *  Partial specialization of is_placeholder that provides the placeholder
   *  number for the placeholder objects defined by libstdc++.
   *  @ingroup binders
   */
  template<int _Num>
    struct is_placeholder<_Placeholder<_Num> >
    : public integral_constant<int, _Num>
    { };

  /**
   * Used by _Safe_tuple_element to indicate that there is no tuple
   * element at this position.
   */
  struct _No_tuple_element;

  /**
   * Implementation helper for _Safe_tuple_element. This primary
   * template handles the case where it is safe to use @c
   * tuple_element.
   */
  template<int __i, typename _Tuple, bool _IsSafe>
    struct _Safe_tuple_element_impl
    : tuple_element<__i, _Tuple> { };

  /**
   * Implementation helper for _Safe_tuple_element. This partial
   * specialization handles the case where it is not safe to use @c
   * tuple_element. We just return @c _No_tuple_element.
   */
  template<int __i, typename _Tuple>
    struct _Safe_tuple_element_impl<__i, _Tuple, false>
    {
      typedef _No_tuple_element type;
    };

  /**
   * Like tuple_element, but returns @c _No_tuple_element when
   * tuple_element would return an error.
   */
 template<int __i, typename _Tuple>
   struct _Safe_tuple_element
   : _Safe_tuple_element_impl<__i, _Tuple,
			      (__i >= 0 && __i < tuple_size<_Tuple>::value)>
   { };

  /**
   *  Maps an argument to bind() into an actual argument to the bound
   *  function object [TR1 3.6.3/5]. Only the first parameter should
   *  be specified: the rest are used to determine among the various
   *  implementations. Note that, although this class is a function
   *  object, it isn't entirely normal because it takes only two
   *  parameters regardless of the number of parameters passed to the
   *  bind expression. The first parameter is the bound argument and
   *  the second parameter is a tuple containing references to the
   *  rest of the arguments.
   */
  template<typename _Arg,
	   bool _IsBindExp = is_bind_expression<_Arg>::value,
	   bool _IsPlaceholder = (is_placeholder<_Arg>::value > 0)>
    class _Mu;

  /**
   *  If the argument is reference_wrapper<_Tp>, returns the
   *  underlying reference. [TR1 3.6.3/5 bullet 1]
   */
  template<typename _Tp>
    class _Mu<reference_wrapper<_Tp>, false, false>
    {
    public:
      typedef _Tp& result_type;

      /* Note: This won't actually work for const volatile
       * reference_wrappers, because reference_wrapper::get() is const
       * but not volatile-qualified. This might be a defect in the TR.
       */
      template<typename _CVRef, typename _Tuple>
	result_type
	operator()(_CVRef& __arg, _Tuple&) const volatile
	{ return __arg.get(); }
    };

  /**
   *  If the argument is a bind expression, we invoke the underlying
   *  function object with the same cv-qualifiers as we are given and
   *  pass along all of our arguments (unwrapped). [TR1 3.6.3/5 bullet 2]
   */
  template<typename _Arg>
    class _Mu<_Arg, true, false>
    {
    public:
      template<typename _CVArg, typename... _Args>
	auto
	operator()(_CVArg& __arg,
		   tuple<_Args...>& __tuple) const volatile
	-> decltype(__arg(declval<_Args>()...))
	{
	  // Construct an index tuple and forward to __call
	  typedef typename _Build_index_tuple<sizeof...(_Args)>::__type
	    _Indexes;
	  return this->__call(__arg, __tuple, _Indexes());
	}

    private:
      // Invokes the underlying function object __arg by unpacking all
      // of the arguments in the tuple.
      template<typename _CVArg, typename... _Args, int... _Indexes>
	auto
	__call(_CVArg& __arg, tuple<_Args...>& __tuple,
	       const _Index_tuple<_Indexes...>&) const volatile
	-> decltype(__arg(declval<_Args>()...))
	{
	  return __arg(std::forward<_Args>(get<_Indexes>(__tuple))...);
	}
    };

  /**
   *  If the argument is a placeholder for the Nth argument, returns
   *  a reference to the Nth argument to the bind function object.
   *  [TR1 3.6.3/5 bullet 3]
   */
  template<typename _Arg>
    class _Mu<_Arg, false, true>
    {
    public:
      template<typename _Signature> class result;

      template<typename _CVMu, typename _CVArg, typename _Tuple>
	class result<_CVMu(_CVArg, _Tuple)>
	{
	  // Add a reference, if it hasn't already been done for us.
	  // This allows us to be a little bit sloppy in constructing
	  // the tuple that we pass to result_of<...>.
	  typedef typename _Safe_tuple_element<(is_placeholder<_Arg>::value
						- 1), _Tuple>::type
	    __base_type;

	public:
	  typedef typename add_rvalue_reference<__base_type>::type type;
	};

      template<typename _Tuple>
	typename result<_Mu(_Arg, _Tuple)>::type
	operator()(const volatile _Arg&, _Tuple& __tuple) const volatile
	{
	  return std::forward<typename result<_Mu(_Arg, _Tuple)>::type>(
	      ::std::get<(is_placeholder<_Arg>::value - 1)>(__tuple));
	}
    };

  /**
   *  If the argument is just a value, returns a reference to that
   *  value. The cv-qualifiers on the reference are the same as the
   *  cv-qualifiers on the _Mu object. [TR1 3.6.3/5 bullet 4]
   */
  template<typename _Arg>
    class _Mu<_Arg, false, false>
    {
    public:
      template<typename _Signature> struct result;

      template<typename _CVMu, typename _CVArg, typename _Tuple>
	struct result<_CVMu(_CVArg, _Tuple)>
	{
	  typedef typename add_lvalue_reference<_CVArg>::type type;
	};

      // Pick up the cv-qualifiers of the argument
      template<typename _CVArg, typename _Tuple>
	_CVArg&&
	operator()(_CVArg&& __arg, _Tuple&) const volatile
	{ return std::forward<_CVArg>(__arg); }
    };

  /**
   *  Maps member pointers into instances of _Mem_fn but leaves all
   *  other function objects untouched. Used by tr1::bind(). The
   *  primary template handles the non--member-pointer case.
   */
  template<typename _Tp>
    struct _Maybe_wrap_member_pointer
    {
      typedef _Tp type;

      static const _Tp&
      __do_wrap(const _Tp& __x)
      { return __x; }

      static _Tp&&
      __do_wrap(_Tp&& __x)
      { return static_cast<_Tp&&>(__x); }
    };

  /**
   *  Maps member pointers into instances of _Mem_fn but leaves all
   *  other function objects untouched. Used by tr1::bind(). This
   *  partial specialization handles the member pointer case.
   */
  template<typename _Tp, typename _Class>
    struct _Maybe_wrap_member_pointer<_Tp _Class::*>
    {
      typedef _Mem_fn<_Tp _Class::*> type;

      static type
      __do_wrap(_Tp _Class::* __pm)
      { return type(__pm); }
    };

  // Specialization needed to prevent "forming reference to void" errors when
  // bind<void>() is called, because argument deduction instantiates
  // _Maybe_wrap_member_pointer<void> outside the immediate context where
  // SFINAE applies.
  template<>
    struct _Maybe_wrap_member_pointer<void>
    {
      typedef void type;
    };

  // std::get<I> for volatile-qualified tuples
  template<size_t _Ind, typename... _Tp>
    inline auto
    __volget(volatile tuple<_Tp...>& __tuple)
    -> typename tuple_element<_Ind, tuple<_Tp...>>::type volatile&
    { return std::get<_Ind>(const_cast<tuple<_Tp...>&>(__tuple)); }

  // std::get<I> for const-volatile-qualified tuples
  template<size_t _Ind, typename... _Tp>
    inline auto
    __volget(const volatile tuple<_Tp...>& __tuple)
    -> typename tuple_element<_Ind, tuple<_Tp...>>::type const volatile&
    { return std::get<_Ind>(const_cast<const tuple<_Tp...>&>(__tuple)); }

  /// Type of the function object returned from bind().
  template<typename _Signature>
    struct _Bind;

   template<typename _Functor, typename... _Bound_args>
    class _Bind<_Functor(_Bound_args...)>
    : public _Weak_result_type<_Functor>
    {
      typedef _Bind __self_type;
      typedef typename _Build_index_tuple<sizeof...(_Bound_args)>::__type
	_Bound_indexes;

      _Functor _M_f;
      tuple<_Bound_args...> _M_bound_args;

      // Call unqualified
      template<typename _Result, typename... _Args, int... _Indexes>
	_Result
	__call(tuple<_Args...>&& __args, _Index_tuple<_Indexes...>)
	{
	  return _M_f(_Mu<_Bound_args>()
		      (get<_Indexes>(_M_bound_args), __args)...);
	}

      // Call as const
      template<typename _Result, typename... _Args, int... _Indexes>
	_Result
	__call_c(tuple<_Args...>&& __args, _Index_tuple<_Indexes...>) const
	{
	  return _M_f(_Mu<_Bound_args>()
		      (get<_Indexes>(_M_bound_args), __args)...);
	}

      // Call as volatile
      template<typename _Result, typename... _Args, int... _Indexes>
	_Result
	__call_v(tuple<_Args...>&& __args,
		 _Index_tuple<_Indexes...>) volatile
	{
	  return _M_f(_Mu<_Bound_args>()
		      (__volget<_Indexes>(_M_bound_args), __args)...);
	}

      // Call as const volatile
      template<typename _Result, typename... _Args, int... _Indexes>
	_Result
	__call_c_v(tuple<_Args...>&& __args,
		   _Index_tuple<_Indexes...>) const volatile
	{
	  return _M_f(_Mu<_Bound_args>()
		      (__volget<_Indexes>(_M_bound_args), __args)...);
	}

     public:
      template<typename... _Args>
	explicit _Bind(const _Functor& __f, _Args&&... __args)
	: _M_f(__f), _M_bound_args(std::forward<_Args>(__args)...)
	{ }

      template<typename... _Args>
	explicit _Bind(_Functor&& __f, _Args&&... __args)
	: _M_f(std::move(__f)), _M_bound_args(std::forward<_Args>(__args)...)
	{ }

      _Bind(const _Bind&) = default;

      _Bind(_Bind&& __b)
      : _M_f(std::move(__b._M_f)), _M_bound_args(std::move(__b._M_bound_args))
      { }

      // Call unqualified
      template<typename... _Args, typename _Result
	= decltype( std::declval<_Functor>()(
	      _Mu<_Bound_args>()( std::declval<_Bound_args&>(),
				  std::declval<tuple<_Args...>&>() )... ) )>
	_Result
	operator()(_Args&&... __args)
	{
	  return this->__call<_Result>(
	      std::forward_as_tuple(std::forward<_Args>(__args)...),
	      _Bound_indexes());
	}

      // Call as const
      template<typename... _Args, typename _Result
	= decltype( std::declval<typename enable_if<(sizeof...(_Args) >= 0),
		       typename add_const<_Functor>::type>::type>()(
	      _Mu<_Bound_args>()( std::declval<const _Bound_args&>(),
				  std::declval<tuple<_Args...>&>() )... ) )>
	_Result
	operator()(_Args&&... __args) const
	{
	  return this->__call_c<_Result>(
	      std::forward_as_tuple(std::forward<_Args>(__args)...),
	      _Bound_indexes());
	}

      // Call as volatile
      template<typename... _Args, typename _Result
	= decltype( std::declval<typename enable_if<(sizeof...(_Args) >= 0),
                       typename add_volatile<_Functor>::type>::type>()(
	      _Mu<_Bound_args>()( std::declval<volatile _Bound_args&>(),
				  std::declval<tuple<_Args...>&>() )... ) )>
	_Result
	operator()(_Args&&... __args) volatile
	{
	  return this->__call_v<_Result>(
	      std::forward_as_tuple(std::forward<_Args>(__args)...),
	      _Bound_indexes());
	}

      // Call as const volatile
      template<typename... _Args, typename _Result
	= decltype( std::declval<typename enable_if<(sizeof...(_Args) >= 0),
                       typename add_cv<_Functor>::type>::type>()(
	      _Mu<_Bound_args>()( std::declval<const volatile _Bound_args&>(),
				  std::declval<tuple<_Args...>&>() )... ) )>
	_Result
	operator()(_Args&&... __args) const volatile
	{
	  return this->__call_c_v<_Result>(
	      std::forward_as_tuple(std::forward<_Args>(__args)...),
	      _Bound_indexes());
	}
    };

  /// Type of the function object returned from bind<R>().
  template<typename _Result, typename _Signature>
    struct _Bind_result;

  template<typename _Result, typename _Functor, typename... _Bound_args>
    class _Bind_result<_Result, _Functor(_Bound_args...)>
    {
      typedef _Bind_result __self_type;
      typedef typename _Build_index_tuple<sizeof...(_Bound_args)>::__type
	_Bound_indexes;

      _Functor _M_f;
      tuple<_Bound_args...> _M_bound_args;

      // sfinae types
      template<typename _Res>
	struct __enable_if_void : enable_if<is_void<_Res>::value, int> { };
      template<typename _Res>
	struct __disable_if_void : enable_if<!is_void<_Res>::value, int> { };

      // Call unqualified
      template<typename _Res, typename... _Args, int... _Indexes>
	_Result
	__call(tuple<_Args...>&& __args, _Index_tuple<_Indexes...>,
	    typename __disable_if_void<_Res>::type = 0)
	{
	  return _M_f(_Mu<_Bound_args>()
		      (get<_Indexes>(_M_bound_args), __args)...);
	}

      // Call unqualified, return void
      template<typename _Res, typename... _Args, int... _Indexes>
	void
	__call(tuple<_Args...>&& __args, _Index_tuple<_Indexes...>,
	    typename __enable_if_void<_Res>::type = 0)
	{
	  _M_f(_Mu<_Bound_args>()
	       (get<_Indexes>(_M_bound_args), __args)...);
	}

      // Call as const
      template<typename _Res, typename... _Args, int... _Indexes>
	_Result
	__call(tuple<_Args...>&& __args, _Index_tuple<_Indexes...>,
	    typename __disable_if_void<_Res>::type = 0) const
	{
	  return _M_f(_Mu<_Bound_args>()
		      (get<_Indexes>(_M_bound_args), __args)...);
	}

      // Call as const, return void
      template<typename _Res, typename... _Args, int... _Indexes>
	void
	__call(tuple<_Args...>&& __args, _Index_tuple<_Indexes...>,
	    typename __enable_if_void<_Res>::type = 0) const
	{
	  _M_f(_Mu<_Bound_args>()
	       (get<_Indexes>(_M_bound_args),  __args)...);
	}

      // Call as volatile
      template<typename _Res, typename... _Args, int... _Indexes>
	_Result
	__call(tuple<_Args...>&& __args, _Index_tuple<_Indexes...>,
	    typename __disable_if_void<_Res>::type = 0) volatile
	{
	  return _M_f(_Mu<_Bound_args>()
		      (__volget<_Indexes>(_M_bound_args), __args)...);
	}

      // Call as volatile, return void
      template<typename _Res, typename... _Args, int... _Indexes>
	void
	__call(tuple<_Args...>&& __args, _Index_tuple<_Indexes...>,
	    typename __enable_if_void<_Res>::type = 0) volatile
	{
	  _M_f(_Mu<_Bound_args>()
	       (__volget<_Indexes>(_M_bound_args), __args)...);
	}

      // Call as const volatile
      template<typename _Res, typename... _Args, int... _Indexes>
	_Result
	__call(tuple<_Args...>&& __args, _Index_tuple<_Indexes...>,
	    typename __disable_if_void<_Res>::type = 0) const volatile
	{
	  return _M_f(_Mu<_Bound_args>()
		      (__volget<_Indexes>(_M_bound_args), __args)...);
	}

      // Call as const volatile, return void
      template<typename _Res, typename... _Args, int... _Indexes>
	void
	__call(tuple<_Args...>&& __args,
	       _Index_tuple<_Indexes...>,
	    typename __enable_if_void<_Res>::type = 0) const volatile
	{
	  _M_f(_Mu<_Bound_args>()
	       (__volget<_Indexes>(_M_bound_args), __args)...);
	}

    public:
      typedef _Result result_type;

      template<typename... _Args>
	explicit _Bind_result(const _Functor& __f, _Args&&... __args)
	: _M_f(__f), _M_bound_args(std::forward<_Args>(__args)...)
	{ }

      template<typename... _Args>
	explicit _Bind_result(_Functor&& __f, _Args&&... __args)
	: _M_f(std::move(__f)), _M_bound_args(std::forward<_Args>(__args)...)
	{ }

      _Bind_result(const _Bind_result&) = default;

      _Bind_result(_Bind_result&& __b)
      : _M_f(std::move(__b._M_f)), _M_bound_args(std::move(__b._M_bound_args))
      { }

      // Call unqualified
      template<typename... _Args>
	result_type
	operator()(_Args&&... __args)
	{
	  return this->__call<_Result>(
	      std::forward_as_tuple(std::forward<_Args>(__args)...),
	      _Bound_indexes());
	}

      // Call as const
      template<typename... _Args>
	result_type
	operator()(_Args&&... __args) const
	{
	  return this->__call<_Result>(
	      std::forward_as_tuple(std::forward<_Args>(__args)...),
	      _Bound_indexes());
	}

      // Call as volatile
      template<typename... _Args>
	result_type
	operator()(_Args&&... __args) volatile
	{
	  return this->__call<_Result>(
	      std::forward_as_tuple(std::forward<_Args>(__args)...),
	      _Bound_indexes());
	}

      // Call as const volatile
      template<typename... _Args>
	result_type
	operator()(_Args&&... __args) const volatile
	{
	  return this->__call<_Result>(
	      std::forward_as_tuple(std::forward<_Args>(__args)...),
	      _Bound_indexes());
	}
    };

  /**
   *  @brief Class template _Bind is always a bind expression.
   *  @ingroup binders
   */
  template<typename _Signature>
    struct is_bind_expression<_Bind<_Signature> >
    : public true_type { };

  /**
   *  @brief Class template _Bind is always a bind expression.
   *  @ingroup binders
   */
  template<typename _Result, typename _Signature>
    struct is_bind_expression<_Bind_result<_Result, _Signature> >
    : public true_type { };

  template<typename _Functor, typename... _ArgTypes>
    struct _Bind_helper
    {
      typedef _Maybe_wrap_member_pointer<typename decay<_Functor>::type>
	__maybe_type;
      typedef typename __maybe_type::type __functor_type;
      typedef _Bind<__functor_type(typename decay<_ArgTypes>::type...)> type;
    };

  /**
   *  @brief Function template for std::bind.
   *  @ingroup binders
   */
  template<typename _Functor, typename... _ArgTypes>
    inline
    typename _Bind_helper<_Functor, _ArgTypes...>::type
    bind(_Functor&& __f, _ArgTypes&&... __args)
    {
      typedef _Bind_helper<_Functor, _ArgTypes...> __helper_type;
      typedef typename __helper_type::__maybe_type __maybe_type;
      typedef typename __helper_type::type __result_type;
      return __result_type(__maybe_type::__do_wrap(std::forward<_Functor>(__f)),
			   std::forward<_ArgTypes>(__args)...);
    }

  template<typename _Result, typename _Functor, typename... _ArgTypes>
    struct _Bindres_helper
    {
      typedef _Maybe_wrap_member_pointer<typename decay<_Functor>::type>
	__maybe_type;
      typedef typename __maybe_type::type __functor_type;
      typedef _Bind_result<_Result,
			   __functor_type(typename decay<_ArgTypes>::type...)>
	type;
    };

  /**
   *  @brief Function template for std::bind<R>.
   *  @ingroup binders
   */
  template<typename _Result, typename _Functor, typename... _ArgTypes>
    inline
    typename _Bindres_helper<_Result, _Functor, _ArgTypes...>::type
    bind(_Functor&& __f, _ArgTypes&&... __args)
    {
      typedef _Bindres_helper<_Result, _Functor, _ArgTypes...> __helper_type;
      typedef typename __helper_type::__maybe_type __maybe_type;
      typedef typename __helper_type::type __result_type;
      return __result_type(__maybe_type::__do_wrap(std::forward<_Functor>(__f)),
			   std::forward<_ArgTypes>(__args)...);
    }

  /**
   *  @brief Exception class thrown when class template function's
   *  operator() is called with an empty target.
   *  @ingroup exceptions
   */
  class bad_function_call : public std::exception
  {
  public:
    virtual ~bad_function_call() throw();
  };

  /**
   *  Trait identifying "location-invariant" types, meaning that the
   *  address of the object (or any of its members) will not escape.
   *  Also implies a trivial copy constructor and assignment operator.
   */
  template<typename _Tp>
    struct __is_location_invariant
    : integral_constant<bool, (is_pointer<_Tp>::value
			       || is_member_pointer<_Tp>::value)>
    { };

  class _Undefined_class;

  union _Nocopy_types
  {
    void*       _M_object;
    const void* _M_const_object;
    void (*_M_function_pointer)();
    void (_Undefined_class::*_M_member_pointer)();
  };

  union _Any_data
  {
    void*       _M_access()       { return &_M_pod_data[0]; }
    const void* _M_access() const { return &_M_pod_data[0]; }

    template<typename _Tp>
      _Tp&
      _M_access()
      { return *static_cast<_Tp*>(_M_access()); }

    template<typename _Tp>
      const _Tp&
      _M_access() const
      { return *static_cast<const _Tp*>(_M_access()); }

    _Nocopy_types _M_unused;
    char _M_pod_data[sizeof(_Nocopy_types)];
  };

  enum _Manager_operation
  {
    __get_type_info,
    __get_functor_ptr,
    __clone_functor,
    __destroy_functor,

    // PGH 2012/12/27 new _Manager_operation codes
    __get_manager_without_alloc,
    __get_manager_with_alloc,
    __get_invoker,
    __clone_functor_with_alloc,
    __get_allocator_rsrc_ptr
  };

  // Simple type wrapper that helps avoid annoying const problems
  // when casting between void pointers and pointers-to-pointers.
  template<typename _Tp>
    struct _Simple_type_wrapper
    {
      _Simple_type_wrapper(_Tp __value) : __value(__value) { }

      _Tp __value;
    };

  template<typename _Tp>
    struct __is_location_invariant<_Simple_type_wrapper<_Tp> >
    : __is_location_invariant<_Tp>
    { };

  // Converts a reference to a function object into a callable
  // function object.
  template<typename _Functor>
    inline _Functor&
    __callable_functor(_Functor& __f)
    { return __f; }

  template<typename _Member, typename _Class>
    inline _Mem_fn<_Member _Class::*>
    __callable_functor(_Member _Class::* &__p)
    { return mem_fn(__p); }

  template<typename _Member, typename _Class>
    inline _Mem_fn<_Member _Class::*>
    __callable_functor(_Member _Class::* const &__p)
    { return mem_fn(__p); }

  // PGH Shared pointer to allocator resource
  template <typename __A>
  static bool _M_is_same_alloc_rsrc(polyalloc::allocator_resource *__rsrca_p,
                                    const __A& __b)
  {
    typedef POLYALLOC_RESOURCE_ADAPTOR(__A) _Adaptor;
    _Adaptor* __adaptor_p = dynamic_cast<_Adaptor*>(__rsrca_p);
    if (! __adaptor_p)
      return false;

    return __adaptor_p->get_allocator() == __b;
  }

  template <typename __T>
  static bool _M_is_same_alloc_rsrc(polyalloc::allocator_resource *__rsrca_p, 
                          const polyalloc::polymorphic_allocator<__T>& __b)
    {
      polyalloc::allocator_resource *__rsrcb_p = __b.resource();
      return __rsrca_p == __rsrcb_p || *__rsrca_p == *__rsrcb_p;
    }

  template <typename _Rsrc>
  static bool _M_is_same_alloc_rsrc(polyalloc::allocator_resource *__rsrca_p,
                                    _Rsrc* __rsrcb_p)
    {
      return __rsrca_p == __rsrcb_p || *__rsrca_p == *__rsrcb_p;
    }

  template<typename _Signature>
    class function;

  typedef shared_ptr<polyalloc::allocator_resource> _Shared_alloc_rsrc_ptr;

  struct _Noop_alloc_rsrc_deleter {
    void operator()(polyalloc::allocator_resource*) { }
  };

  /// Base class of all polymorphic function object wrappers.
  class _Function_base
  {
  public:
    static const std::size_t _M_max_size = sizeof(_Nocopy_types);
    static const std::size_t _M_max_align = __alignof__(_Nocopy_types);

    static const _Shared_alloc_rsrc_ptr& _M_default_alloc_rsrc()
    {
      // TBD: This code should change once we distinguish between the
      // default-default resource and the current-default resource.  We cannot
      // keep a static pointer to the current-default resource because it may
      // change.
      static _Shared_alloc_rsrc_ptr dfltRsrcPtr(
        polyalloc::allocator_resource::default_resource(),
        _Noop_alloc_rsrc_deleter());
      return dfltRsrcPtr;
    }

    // PGH Functor with allocator
    template<typename _Functor>
      class _Functor_wrapper
      {
        uses_allocator_construction_wrapper<_Functor> _M_functor;
        _Shared_alloc_rsrc_ptr                        _M_alloc_resource;

      public:
        _Functor_wrapper(const _Functor& __f,
                         const _Shared_alloc_rsrc_ptr& __a)
          : _M_functor(allocator_arg, &*__a, __f), _M_alloc_resource(__a) { }
        _Functor_wrapper(_Functor&& __f, const _Shared_alloc_rsrc_ptr& __a)
          : _M_functor(allocator_arg, &*__a, move(__f))
          , _M_alloc_resource(__a) { }

        void _M_delete_self()
        {
          // Move allocator pointer to stack before destroying original.
          _Shared_alloc_rsrc_ptr __alloc_rsrc = std::move(_M_alloc_resource);
          this->~_Functor_wrapper();
          // Dealloate self using allocator
          __alloc_rsrc->deallocate(this, sizeof(_Functor_wrapper));
        }

        _Functor      & _M_get_functor()       { return _M_functor.value(); }
        _Functor const& _M_get_functor() const { return _M_functor.value(); }

        // Return reference to avoid bumping ref-count unnecessarily
        const _Shared_alloc_rsrc_ptr& _M_get_alloc_resource() const
          { return _M_alloc_resource; }
      };

      // PGH: bundle of a source function object and a target allocator.  Used
      // as an argument to pass both arguments into a single call to the
      // manager function for cloning a function object using a new allocator.
      // This struct is not a permanent part of any larger data structure.
      struct _Functor_and_alloc
      {
        _Any_data              _M_f;  // source function object
        _Shared_alloc_rsrc_ptr _M_a;  // new allocator

        _Any_data _M_as_any_data()
          {
            _Any_data ret;
            ret._M_access<_Functor_and_alloc*>() = this;
            return ret;
          }

        template <typename __A>
        _Functor_and_alloc(const _Any_data& __f, const __A& __alloc)
          : _M_f(__f)
          , _M_a(_M_make_shared_alloc(__alloc))
          {
          }

        template <typename __A>
        _Functor_and_alloc(const _Any_data& __f, _Shared_alloc_rsrc_ptr&& __r)
          : _M_f(__f)
          , _M_a(std::move(__r))
          {
          }

        template <typename __A>
        _Functor_and_alloc(const _Any_data& __f,
                           const _Shared_alloc_rsrc_ptr& __r)
          : _M_f(__f)
          , _M_a(__r)
          {
          }
      };

    template<typename _Functor, bool __uses_custom_alloc>
      class _Base_manager
      {
        typedef _Functor_wrapper<_Functor> _M_functor_wrapper;

      protected:
        // True if _Functor could be stored locally (i.e., not on the heap)
        // when no allocator is used.
	static const bool __stored_locally_without_alloc =
	(__is_location_invariant<_Functor>::value
	 && sizeof(_Functor) <= _M_max_size
	 && __alignof__(_Functor) <= _M_max_align
	 && (_M_max_align % __alignof__(_Functor) == 0));

        // True if _Functor is actually stored locally.
	static const bool __stored_locally = (__stored_locally_without_alloc
                                              && !__uses_custom_alloc);

	typedef integral_constant<bool, __stored_locally> _Local_storage;

	// Retrieve a pointer to the function object
	static _Functor*
	_M_get_pointer(const _Any_data& __source)
	{
          const _Functor* __ptr =
            (__stored_locally ? 
             std::__addressof(__source._M_access<_Functor>()) :
             std::__addressof(
               __source._M_access<_M_functor_wrapper*>()->_M_get_functor()));
	  return const_cast<_Functor*>(__ptr);
	}

        static inline _M_functor_wrapper*
        _M_make_functor_wrapper(const _Functor&               __f,
                                const _Shared_alloc_rsrc_ptr& __a)
        {
          // Allocate clone from allocator resource and construct it
          _M_functor_wrapper *__ret = static_cast<_M_functor_wrapper *>(
            __a->allocate(sizeof(_M_functor_wrapper),
                          __alignof(_M_functor_wrapper)));
          return new (__ret) _M_functor_wrapper(__f, __a);
        }

	// Clone a location-invariant function object that fits within
	// an _Any_data structure.
	static void
	_M_clone(_Any_data& __dest, const _Any_data& __source, true_type)
	{
	  new (__dest._M_access()) _Functor(__source._M_access<_Functor>());
	}

	// Clone a function object that is not location-invariant or
	// that cannot fit into an _Any_data structure.
	static void
	_M_clone(_Any_data& __dest, const _Any_data& __source, false_type)
	{
          // Ignore allocator in source
          const _Functor& __f =
            __source._M_access<_M_functor_wrapper*>()->_M_get_functor();

          if (__stored_locally_without_alloc)
            // Function object fits locally
            new (__dest._M_access()) _Functor(__f);
          else
            // Return result in __dest, using default allocator
            __dest._M_access<_M_functor_wrapper*>() =
              _M_make_functor_wrapper(__f, 
                                      _Function_base::_M_default_alloc_rsrc());
	}

	// Clone a location-invariant function object that fits within
	// an _Any_data structure.
	static void
	_M_clone_with_alloc(_Any_data& __dest, const _Any_data& __source,
                            const _Shared_alloc_rsrc_ptr& __a, true_type)
	{
          const _Functor& __f = __source._M_access<_Functor>();

          // Return result in __dest
	  __dest._M_access<_M_functor_wrapper*>() =
            _M_make_functor_wrapper(__f, __a);
	}

	// Clone a function object that is not location-invariant or
	// that cannot fit into an _Any_data structure.
	static void
	_M_clone_with_alloc(_Any_data& __dest, const _Any_data& __source,
                            const _Shared_alloc_rsrc_ptr& __a, false_type)
	{
          const _Functor& __f =
            __source._M_access<_M_functor_wrapper*>()->_M_get_functor();

          // Return result in __dest
	  __dest._M_access<_M_functor_wrapper*>() =
            _M_make_functor_wrapper(__f, __a);
	}

	// Destroying a location-invariant object may still require
	// destruction.
	static void
	_M_destroy(_Any_data& __victim, true_type)
	{
	  __victim._M_access<_Functor>().~_Functor();
	}

	// Destroying an object located on the heap.
	static void
	_M_destroy(_Any_data& __victim, false_type)
	{
          // Delete functor wrapper using its own allocator.
          __victim._M_access<_M_functor_wrapper*>()->_M_delete_self();
	}

        // Return the default allocator if __uses_allocator is false
        static void
        _M_get_allocator_resource(_Any_data&       __resource,
                                  const _Any_data& __source,
                                  false_type)
        {
          __resource._M_access<const _Shared_alloc_rsrc_ptr*>() =
            &_M_default_alloc_rsrc();
        }

        // Return custom allocator if __uses_allocator is true
        static void
        _M_get_allocator_resource(_Any_data&       __resource,
                                  const _Any_data& __source,
                                  true_type)
        {
          __resource._M_access<const _Shared_alloc_rsrc_ptr*>() =
            &__source._M_access<_M_functor_wrapper*>()->
            _M_get_alloc_resource();
        }

      public:
	static bool
	_M_manager(_Any_data& __dest, const _Any_data& __source,
		   _Manager_operation __op)
	{
          typedef bool (*_Manager_type)(_Any_data&, const _Any_data&,
                                        _Manager_operation);

	  switch (__op)
	    {
#ifdef __GXX_RTTI
	    case __get_type_info:
	      __dest._M_access<const type_info*>() = &typeid(_Functor);
	      break;
#endif
	    case __get_functor_ptr:
	      __dest._M_access<_Functor*>() = _M_get_pointer(__source);
	      break;

	    case __clone_functor:
	      _M_clone(__dest, __source, _Local_storage());
	      break;

	    case __destroy_functor:
	      _M_destroy(__dest, _Local_storage());
	      break;

            // PGH 2012/12/27
            // Invoking _M_manager with an unknown __op is a no-op.  The
            // following OP codes are new for allocator-enabled function
            // objects.  Since these cases were previously no-ops, the caller
            // should handle the situation where __dest is unchanged, for
            // binary compatibility with std::function objects that were
            // created by code that was compiled prior to these changes.
            case __get_manager_without_alloc:
              // Return a pointer to this manager function, for a function
              // object that does not use a custom allocator.
              __dest._M_access<_Manager_type>() =
                _Base_manager<_Functor, false>::_M_manager;
              break;

            case __get_manager_with_alloc:
              // Return a pointer to this manager function, for a function
              // object that uses a custom allocator.
              __dest._M_access<_Manager_type>() =
                _Base_manager<_Functor, true>::_M_manager;
              break;

            case __get_invoker:
              // Return a pointer to the invoker corresponding to this manager.
              __dest._M_access<void*>() = nullptr;
              break;

            case __clone_functor_with_alloc:
              {
                _Functor_and_alloc *__fa =
                  __source._M_access<_Functor_and_alloc *>();
                _M_clone_with_alloc(__dest, __fa->_M_f, __fa->_M_a,
                                    _Local_storage());
              }
	      break;

            case __get_allocator_rsrc_ptr:
              typedef integral_constant<bool,
                                        __uses_custom_alloc> _Custom_allocator;
              _M_get_allocator_resource(__dest, __source, _Custom_allocator());
              break;
            }
	  return false;
	}

	static void
	_M_init_functor(_Any_data& __functor, _Functor&& __f,
                        _Shared_alloc_rsrc_ptr&& __a)
        { _M_init_functor(__functor, std::move(__f),
                          std::move(__a), _Local_storage()); }

	template<typename _Signature>
	  static bool
	  _M_not_empty_function(const function<_Signature>& __f)
	  { return static_cast<bool>(__f); }

	template<typename _Tp>
	  static bool
	  _M_not_empty_function(const _Tp*& __fp)
	  { return __fp; }

	template<typename _Class, typename _Tp>
	  static bool
	  _M_not_empty_function(_Tp _Class::* const& __mp)
	  { return __mp; }

	template<typename _Tp>
	  static bool
	  _M_not_empty_function(const _Tp&)
	  { return true; }

      private:
	static void
	_M_init_functor(_Any_data& __functor, _Functor&& __f,
                        _Shared_alloc_rsrc_ptr&& __a, true_type /* local */)
	{ new (__functor._M_access()) _Functor(std::move(__f)); }

	static void
	_M_init_functor(_Any_data& __functor, _Functor&& __f,
                        _Shared_alloc_rsrc_ptr&& __a, false_type /* local */)
	{ 
          // Allocate a functor from the allocator resource and construct it.
          _M_functor_wrapper *__pfunctor = static_cast<_M_functor_wrapper *>(
            __a->allocate(sizeof(_M_functor_wrapper),
                          __alignof__(_M_functor_wrapper)));
          new (__pfunctor) _M_functor_wrapper(std::move(__f), std::move(__a));
          __functor._M_access<_M_functor_wrapper*>() = __pfunctor;
        }
      };

    template<typename _Functor, bool __uses_custom_alloc>
      class _Ref_manager : public _Base_manager<_Functor*,
                                                __uses_custom_alloc>
      {
	typedef _Function_base::_Base_manager<_Functor*,
                                              __uses_custom_alloc> _Base;

    public:
	static bool
	_M_manager(_Any_data& __dest, const _Any_data& __source,
		   _Manager_operation __op)
	{
          typedef bool (*_Manager_type)(_Any_data&, const _Any_data&,
                                        _Manager_operation);

	  switch (__op)
	    {
#ifdef __GXX_RTTI
	    case __get_type_info:
	      __dest._M_access<const type_info*>() = &typeid(_Functor);
	      break;
#endif
	    case __get_functor_ptr:
	      __dest._M_access<_Functor*>() = *_Base::_M_get_pointer(__source);
	      return is_const<_Functor>::value;
	      break;

            // PGH 2012/12/27
            // Invoking _M_manager with an unknown __op is a no-op.  The
            // following OP codes are new for allocator-enabled function
            // objects.  Since these cases were previously no-ops, the caller
            // should handle the situation where __dest is unchanged, for
            // binary compatibility with std::function objects that were
            // created by code that was compiled prior to these changes.
            case __get_manager_without_alloc:
              // Return a pointer to this manager function, for a function
              // object that does not use a custom allocator.
              __dest._M_access<_Manager_type>() =
                _Ref_manager<_Functor, false>::_M_manager;
              break;

            case __get_manager_with_alloc:
              // Return a pointer to this manager function, for a function
              // object that uses a custom allocator.
              __dest._M_access<_Manager_type>() =
                _Ref_manager<_Functor, true>::_M_manager;
              break;

            case __get_invoker:
              // Return a pointer to the invoker corresponding to this manager.
              __dest._M_access<void*>() = nullptr;
              break;

	    default:
	      _Base::_M_manager(__dest, __source, __op);
	    }
	  return false;
	}

	static void
	_M_init_functor(_Any_data& __functor, reference_wrapper<_Functor> __f,
                        _Shared_alloc_rsrc_ptr&& __a)
	{
	  // TBD: Use address_of function instead.
	  _Base::_M_init_functor(__functor, &__f.get(), std::move(__a));
	}
      };

    _Function_base() : _M_manager(0) { }

    ~_Function_base()
    {
      if (_M_manager)
	_M_manager(_M_functor, _M_functor, __destroy_functor);
    }


    bool _M_empty() const { return !_M_manager; }

    typedef bool (*_Manager_type)(_Any_data&, const _Any_data&,
				  _Manager_operation);

    _Any_data     _M_functor;
    _Manager_type _M_manager;
  };

  template <typename __A>
  static _Shared_alloc_rsrc_ptr _M_make_shared_alloc(const __A& __alloc)
  {
    return std::allocate_shared<POLYALLOC_RESOURCE_ADAPTOR(__A) >(__alloc,
                                                                  __alloc);
  }

  template <typename __T>
  static _Shared_alloc_rsrc_ptr _M_make_shared_alloc(
    const polyalloc::polymorphic_allocator<__T>& __alloc)
    {
      if (__alloc.resource() ==
          polyalloc::allocator_resource::default_resource())
        return _Function_base::_M_default_alloc_rsrc();
      else
        return _Shared_alloc_rsrc_ptr(__alloc.resource(),
                                      _Noop_alloc_rsrc_deleter(), __alloc);
    }

  template <typename _Rsrc>
  static _Shared_alloc_rsrc_ptr _M_make_shared_alloc(_Rsrc* __p)
    {
      // Compiler failure if __Rsrc is not derived from allocator_resource
      XSTD::polyalloc::polymorphic_allocator<char> __a(__p);
      return _M_make_shared_alloc(__a);
    }

  static _Shared_alloc_rsrc_ptr _M_make_shared_alloc(_Shared_alloc_rsrc_ptr __p)
    {
      return __p;
    }

  template<typename _Signature, typename _Functor, bool __uses_custom_alloc>
    class _Function_handler;

  template<typename _Signature, typename _Functor, bool __uses_custom_alloc>
    struct _Function_handler_wrapper :
      _Function_handler<_Signature, _Functor, __uses_custom_alloc>
    {
      static bool
      _M_manager(_Any_data& __dest, const _Any_data& __source,
		 _Manager_operation __op)
      {
        typedef bool (*_Manager_type)(_Any_data&, const _Any_data&,
                                      _Manager_operation);

        typedef _Function_handler_wrapper<_Signature, _Functor,
                                          __uses_custom_alloc> _Self;
        typedef _Function_handler<_Signature, _Functor, __uses_custom_alloc>
          _Base;

	switch (__op)
	  {
#ifdef __GXX_RTTI
	  case __get_type_info:
	    __dest._M_access<const type_info*>() = &typeid(_Functor);
	    break;
#endif
          // PGH 2012/12/27
          // Invoking _M_manager with an unknown __op is a no-op.  The
          // following OP codes are new for allocator-enabled function
          // objects.  Since these cases were previously no-ops, the caller
          // should handle the situation where __dest is unchanged, for
          // binary compatibility with std::function objects that were
          // created by code that was compiled prior to these changes.
          case __get_manager_without_alloc:
            // Return a pointer to this manager function, for a function
            // object that does not use a custom allocator.
            __dest._M_access<_Manager_type>() =
              _Function_handler_wrapper<_Signature, _Functor,
                                        false>::_M_manager;
            break;

          case __get_manager_with_alloc:
            // Return a pointer to this manager function, for a function
            // object that uses a custom allocator.
            __dest._M_access<_Manager_type>() =
              _Function_handler_wrapper<_Signature, _Functor,
                                        true>::_M_manager;
            break;

          case __get_invoker:
            // Return a pointer to the invoker corresponding to this manager.
            typedef decltype(&_Self::_M_invoke) _Invoker_type;
            __dest._M_access<_Invoker_type>() = &_Self::_M_invoke;
            break;

	  default:
	    _Base::_M_manager(__dest, __source, __op);
	  }
	return false;
      }
      
    };

  template<typename _Res, typename _Functor, bool __uses_custom_alloc,
           typename... _ArgTypes>
    class _Function_handler<_Res(_ArgTypes...), _Functor, __uses_custom_alloc>
    : public _Function_base::_Base_manager<_Functor, __uses_custom_alloc>
    {
      typedef _Function_base::_Base_manager<_Functor,
                                            __uses_custom_alloc> _Base;

    public:
      static _Res
      _M_invoke(const _Any_data& __functor, _ArgTypes... __args)
      {
	return (*_Base::_M_get_pointer(__functor))(
	    std::forward<_ArgTypes>(__args)...);
      }
    };

  template<typename _Functor, bool __uses_custom_alloc, typename... _ArgTypes>
    class _Function_handler<void(_ArgTypes...), _Functor, __uses_custom_alloc>
    : public _Function_base::_Base_manager<_Functor, __uses_custom_alloc>
    {
      typedef _Function_base::_Base_manager<_Functor,
                                            __uses_custom_alloc> _Base;

     public:
      static void
      _M_invoke(const _Any_data& __functor, _ArgTypes... __args)
      {
	(*_Base::_M_get_pointer(__functor))(
	    std::forward<_ArgTypes>(__args)...);
      }
    };

  template<typename _Res, typename _Functor, bool __uses_custom_alloc,
           typename... _ArgTypes>
    class _Function_handler<_Res(_ArgTypes...), reference_wrapper<_Functor>,
                            __uses_custom_alloc>
    : public _Function_base::_Ref_manager<_Functor, __uses_custom_alloc>
    {
      typedef _Function_base::_Ref_manager<_Functor,__uses_custom_alloc> _Base;

     public:
      static _Res
      _M_invoke(const _Any_data& __functor, _ArgTypes... __args)
      {
	return __callable_functor(**_Base::_M_get_pointer(__functor))(
	      std::forward<_ArgTypes>(__args)...);
      }
    };

  template<typename _Functor, bool __uses_custom_alloc, typename... _ArgTypes>
    class _Function_handler<void(_ArgTypes...), reference_wrapper<_Functor>,
                            __uses_custom_alloc>
    : public _Function_base::_Ref_manager<_Functor, __uses_custom_alloc>
    {
      typedef _Function_base::_Ref_manager<_Functor,__uses_custom_alloc> _Base;

     public:
      static void
      _M_invoke(const _Any_data& __functor, _ArgTypes... __args)
      {
	__callable_functor(**_Base::_M_get_pointer(__functor))(
	    std::forward<_ArgTypes>(__args)...);
      }
    };

  template<typename _Class, typename _Member, bool __uses_custom_alloc,
           typename _Res, typename... _ArgTypes>
    class _Function_handler<_Res(_ArgTypes...), _Member _Class::*,
                            __uses_custom_alloc>
    : public _Function_handler<void(_ArgTypes...), _Member _Class::*,
                               __uses_custom_alloc>
    {
      typedef _Function_handler<void(_ArgTypes...), _Member _Class::*,
                                __uses_custom_alloc> _Base;

     public:
      static _Res
      _M_invoke(const _Any_data& __functor, _ArgTypes... __args)
      {
	return mem_fn(_Base::_M_get_pointer(__functor)->__value)(
	    std::forward<_ArgTypes>(__args)...);
      }
    };

template<typename _Class, typename _Member, bool __uses_custom_alloc,
         typename... _ArgTypes>
  class _Function_handler<void(_ArgTypes...), _Member _Class::*,
                          __uses_custom_alloc>
    : public _Function_base::_Base_manager<
                   _Simple_type_wrapper< _Member _Class::* >,
                   __uses_custom_alloc>
    {
      typedef _Member _Class::* _Functor;
      typedef _Simple_type_wrapper<_Functor> _Wrapper;
      typedef _Function_base::_Base_manager<_Wrapper,
                                            __uses_custom_alloc> _Base;

     public:
      static bool
      _M_manager(_Any_data& __dest, const _Any_data& __source,
		 _Manager_operation __op)
      {
        typedef bool (*_Manager_type)(_Any_data&, const _Any_data&,
                                      _Manager_operation);

	switch (__op)
	  {
#ifdef __GXX_RTTI
	  case __get_type_info:
	    __dest._M_access<const type_info*>() = &typeid(_Functor);
	    break;
#endif
	  case __get_functor_ptr:
	    __dest._M_access<_Functor*>() =
	      &_Base::_M_get_pointer(__source)->__value;
	    break;

	  default:
	    _Base::_M_manager(__dest, __source, __op);
	  }
	return false;
      }

      static void
      _M_invoke(const _Any_data& __functor, _ArgTypes... __args)
      {
	mem_fn(_Base::_M_get_pointer(__functor)->__value)(
	    std::forward<_ArgTypes>(__args)...);
      }
    };

  /**
   *  @brief Primary class template for std::function.
   *  @ingroup functors
   *
   *  Polymorphic function wrapper.
   */
  template<typename _Res, typename... _ArgTypes>
    class function<_Res(_ArgTypes...)>
    : public _Maybe_unary_or_binary_function<_Res, _ArgTypes...>,
      private _Function_base
    {
      typedef _Res _Signature_type(_ArgTypes...);

      // PGH Shared pointer to allocator resource
      typedef shared_ptr<polyalloc::allocator_resource> _Shared_alloc_rsrc_ptr;

      struct _Useless { };

    public:
      typedef _Res result_type;

      // [3.7.2.1] construct/copy/destroy

      /**
       *  @brief Default construct creates an empty function call wrapper.
       *  @post @c !(bool)*this
       */
      function() : _Function_base() { }

      /**
       *  @brief Creates an empty function call wrapper.
       *  @post @c !(bool)*this
       */
      function(nullptr_t) : _Function_base() { }

      /**
       *  @brief %Function copy constructor.
       *  @param x A %function object with identical call signature.
       *  @post @c (bool)*this == (bool)x
       *
       *  The newly-created %function contains a copy of the target of @a
       *  x (if it has one).
       */
      function(const function& __x);

      /**
       *  @brief %Function move constructor.
       *  @param x A %function object rvalue with identical call signature.
       *
       *  The newly-created %function contains the target of @a x
       *  (if it has one).
       */
      function(function&& __x); // PGH: replace imp

      /**
       *  @brief Builds a %function that targets a copy of the incoming
       *  function object.
       *  @param f A %function object that is callable with parameters of
       *  type @c T1, @c T2, ..., @c TN and returns a value convertible
       *  to @c Res.
       *
       *  The newly-created %function object will target a copy of @a
       *  f. If @a f is @c reference_wrapper<F>, then this function
       *  object will contain a reference to the function object @c
       *  f.get(). If @a f is a NULL function pointer or NULL
       *  pointer-to-member, the newly-created object will be empty.
       *
       *  If @a f is a non-NULL function pointer or an object of type @c
       *  reference_wrapper<F>, this function will not throw.
       */
      template<typename _Functor>
	function(_Functor __f,
		 typename enable_if<
			   !is_integral<_Functor>::value, _Useless>::type
		   = _Useless());

      // PGH: allocator_arg_t versions
      template <typename __A>
      function(allocator_arg_t, const __A&);

      template <typename __A>
      function(allocator_arg_t, const __A&, nullptr_t);

      template <typename __A>
      function(allocator_arg_t, const __A&, const function& __x);

      template <typename __A>
      function(allocator_arg_t, const __A&, function&& __x);

      template<typename _Functor, typename __A>
	function(allocator_arg_t, const __A&, _Functor __f,
		 typename enable_if<
			   !is_integral<_Functor>::value, _Useless>::type
		   = _Useless());

      ~function();

      /**
       *  @brief %Function assignment operator.
       *  @param x A %function with identical call signature.
       *  @post @c (bool)*this == (bool)x
       *  @returns @c *this
       *
       *  The target of @a x is copied to @c *this. If @a x has no
       *  target, then @c *this will be empty.
       *
       *  If @a x targets a function pointer or a reference to a function
       *  object, then this operation will not throw an %exception.
       */
      function&
      operator=(const function& __x)
      {
	function(__x).swap(*this);
	return *this;
      }

      /**
       *  @brief %Function move-assignment operator.
       *  @param x A %function rvalue with identical call signature.
       *  @returns @c *this
       *
       *  The target of @a x is moved to @c *this. If @a x has no
       *  target, then @c *this will be empty.
       *
       *  If @a x targets a function pointer or a reference to a function
       *  object, then this operation will not throw an %exception.
       */
      function&
      operator=(function&& __x)
      {
	function(std::move(__x)).swap(*this);
	return *this;
      }

      /**
       *  @brief %Function assignment to zero.
       *  @post @c !(bool)*this
       *  @returns @c *this
       *
       *  The target of @c *this is deallocated, leaving it empty.
       */
      function&
      operator=(nullptr_t)
      {
	if (_M_manager)
	  {
	    _M_manager(_M_functor, _M_functor, __destroy_functor);
	    _M_manager = 0;
	    _M_invoker = 0;
	  }
	return *this;
      }

      /**
       *  @brief %Function assignment to a new target.
       *  @param f A %function object that is callable with parameters of
       *  type @c T1, @c T2, ..., @c TN and returns a value convertible
       *  to @c Res.
       *  @return @c *this
       *
       *  This  %function object wrapper will target a copy of @a
       *  f. If @a f is @c reference_wrapper<F>, then this function
       *  object will contain a reference to the function object @c
       *  f.get(). If @a f is a NULL function pointer or NULL
       *  pointer-to-member, @c this object will be empty.
       *
       *  If @a f is a non-NULL function pointer or an object of type @c
       *  reference_wrapper<F>, this function will not throw.
       */
      template<typename _Functor>
	typename enable_if<!is_integral<_Functor>::value, function&>::type
	operator=(_Functor&& __f)
	{
	  function(std::forward<_Functor>(__f)).swap(*this);
	  return *this;
	}

      /// @overload
      template<typename _Functor>
	typename enable_if<!is_integral<_Functor>::value, function&>::type
	operator=(reference_wrapper<_Functor> __f)
	{
	  function(__f).swap(*this);
	  return *this;
	}

      // [3.7.2.2] function modifiers

      /**
       *  @brief Swap the targets of two %function objects.
       *  @param f A %function with identical call signature.
       *
       *  Swap the targets of @c this function object and @a f. This
       *  function will not throw an %exception.
       */
      void swap(function& __x)
      {
        // TBD: Assert same allocator
        __do_swap(__x);
      }

      // [3.7.2.3] function capacity

      /**
       *  @brief Determine if the %function wrapper has a target.
       *
       *  @return @c true when this %function object contains a target,
       *  or @c false when it is empty.
       *
       *  This function will not throw an %exception.
       */
      explicit operator bool() const
      { return !_M_empty(); }

      // [3.7.2.4] function invocation

      /**
       *  @brief Invokes the function targeted by @c *this.
       *  @returns the result of the target.
       *  @throws bad_function_call when @c !(bool)*this
       *
       *  The function call operator invokes the target function object
       *  stored by @c this.
       */
      _Res operator()(_ArgTypes... __args) const;

#ifdef __GXX_RTTI
      // [3.7.2.5] function target access
      /**
       *  @brief Determine the type of the target of this function object
       *  wrapper.
       *
       *  @returns the type identifier of the target function object, or
       *  @c typeid(void) if @c !(bool)*this.
       *
       *  This function will not throw an %exception.
       */
      const type_info& target_type() const;

      /**
       *  @brief Access the stored target function object.
       *
       *  @return Returns a pointer to the stored target function object,
       *  if @c typeid(Functor).equals(target_type()); otherwise, a NULL
       *  pointer.
       *
       * This function will not throw an %exception.
       */
      template<typename _Functor>       _Functor* target();

      /// @overload
      template<typename _Functor> const _Functor* target() const;
#endif

      polyalloc::allocator_resource* get_allocator_resource() const;

    private:
      typedef _Res (*_Invoker_type)(const _Any_data&, _ArgTypes...);
      _Invoker_type _M_invoker;

      // PGH: Swap without testing for validity
      void __do_swap(function& __x)
      {
	std::swap(_M_functor, __x._M_functor);
	std::swap(_M_manager, __x._M_manager);
	std::swap(_M_invoker, __x._M_invoker);
      }

      _Shared_alloc_rsrc_ptr
      _M_get_shared_alloc_rsrc_ptr() const
        {
          const _Shared_alloc_rsrc_ptr *ret_p = nullptr;
          if (_M_manager)
          {
            // __get_allocator_rsrc_ptr returns a pointer to a shared_ptr
            _Any_data __alloc_data;
            __alloc_data._M_access<const _Shared_alloc_rsrc_ptr*>() = nullptr;
            _M_manager(__alloc_data, _M_functor, __get_allocator_rsrc_ptr);
            ret_p = __alloc_data._M_access<const _Shared_alloc_rsrc_ptr*>();
          }
          else if (_M_invoker == _M_null_invoker_with_alloc)
            // shared_ptr is embedded in _M_functor.  Note that this is NOT a
            // pointer to a shared_ptr, unlike the result of
            // __get_allocator_rsrc_ptr, above.
            return _M_functor._M_access<const _Shared_alloc_rsrc_ptr>();

          if (ret_p)
            return *ret_p;
          else
            return _Shared_alloc_rsrc_ptr(
              polyalloc::allocator_resource::default_resource(),
              _Noop_alloc_rsrc_deleter());
        }

      // Special invoker for empty function that holds an allocator.
      static _Res _M_null_invoker_with_alloc(const _Any_data&, _ArgTypes...);

      // Make an empty function object with an allocator shared_ptr
      void _M_make_empty(_Shared_alloc_rsrc_ptr&& __alloc);
      
      // Make an empty function object an allocator
      template <typename __A>
        void _M_make_empty(const __A& __alloc);

      // Make an empty function object with the default allocator
      template <typename __T>
        void _M_make_empty(const std::allocator<__T>& __alloc);

      // Make a copy with an allocator
      template<typename __A>
        void _M_make_copy(const __A& __alloc, const function& __x);
  };

  // Out-of-line member definitions.
  template<typename _Res, typename... _ArgTypes>
      _Res function<_Res(_ArgTypes...)>::
         _M_null_invoker_with_alloc(const _Any_data&, _ArgTypes...)
  {
    // assert(0);  // Should never be executed
    __throw_bad_function_call();
  }

  template<typename _Res, typename... _ArgTypes>
  void
  function<_Res(_ArgTypes...)>::_M_make_empty(_Shared_alloc_rsrc_ptr&& __alloc)
    {
      // When constructing an empty function with an allocator, we squirrel
      // a shared_ptr to the allocator resource away in _M_functor.  It
      // would be nice if we could set _M_manager to something that will
      // manage that allocator, but _M_manager must be set to nullptr
      // because that is how the code has always detected empty function
      // objects.  Instead, we set _M_invoker to _M_null_invoker_with_alloc
      // as a signal to the destructor and other functions that this object
      // is not completely empty, but actually contains an allocator.
      static_assert(sizeof(_Shared_alloc_rsrc_ptr) <= sizeof(_Any_data),
                    "Cannot store a shared_ptr in _Any_data object");
      if (__alloc.get() == polyalloc::allocator_resource::default_resource())
        return;  // Default allocator.  Nothing to store.
      new (_M_functor._M_access()) _Shared_alloc_rsrc_ptr(std::move(__alloc));
      _M_invoker = _M_null_invoker_with_alloc;
      _M_manager = nullptr;
    }

  template<typename _Res, typename... _ArgTypes>
    template <typename __A>
      void
      function<_Res(_ArgTypes...)>::_M_make_empty(const __A& __alloc)
      {
        _M_make_empty(_M_make_shared_alloc(__alloc));
      }

  template<typename _Res, typename... _ArgTypes>
    template <typename __T>
      void
      function<_Res(_ArgTypes...)>::_M_make_empty(
        const std::allocator<__T>& __alloc)
    {
      // No need to store std::allocator
    }

  template<typename _Res, typename... _ArgTypes>
    template<typename __A>
      void
      function<_Res(_ArgTypes...)>::_M_make_copy(const __A&      __alloc,
                                                 const function& __x)
        {
          if (__x._M_manager)
          {
            _Any_data __out_param;
            polyalloc::allocator_resource *const __dflt_rsrc =
              polyalloc::allocator_resource::default_resource();

            // Get a variant of __x._M_manager that uses an allocator.
            // Note that __get_manager_with_alloc is a new op, so we must set a
            // default value in case it is a no-op for __x.
            __out_param._M_access<_Manager_type>() = nullptr;
            if (_M_is_same_alloc_rsrc(__dflt_rsrc, __alloc))
              // Default allocator
              __x._M_manager(__out_param, __out_param,
                             __get_manager_without_alloc);
            else
              // Non-default allocator
              __x._M_manager(__out_param, __out_param,
                             __get_manager_with_alloc);
            _M_manager = __out_param._M_access<_Manager_type>();
            if (! _M_manager)
            {
              // Copying a legacy function for which __get_manager_with_alloc
              // is not implemented
              _M_invoker = __x._M_invoker;
              _M_manager = __x._M_manager;
              __x._M_manager(_M_functor, __x._M_functor, __clone_functor);
            }
              
            // Get a variant of __x._M_invoker that uses an allocator.
            __out_param._M_access<_Invoker_type>() = nullptr;
            this->_M_manager(__out_param, __out_param, __get_invoker);
            _M_invoker = __out_param._M_access<_Invoker_type>();
          
            // Construct clone of __x._M_functor with allocator
            _Shared_alloc_rsrc_ptr __x_rsrc = __x._M_get_shared_alloc_rsrc_ptr();
            if (_M_is_same_alloc_rsrc(__x_rsrc.get(), __alloc))
              __x._M_manager(_M_functor,
                             _Functor_and_alloc(__x._M_functor,
                                         std::move(__x_rsrc))._M_as_any_data(),
                             __clone_functor_with_alloc);
            else
              __x._M_manager(_M_functor,
                             _Functor_and_alloc(__x._M_functor,
                                                __alloc)._M_as_any_data(),
                             __clone_functor_with_alloc);
          }
          else if (_M_null_invoker_with_alloc == __x._M_invoker)
          {
            _Shared_alloc_rsrc_ptr __x_rsrc = __x._M_get_shared_alloc_rsrc_ptr();
            if (_M_is_same_alloc_rsrc(__x_rsrc.get(), __alloc))
              _M_make_empty(__x_rsrc);
            else
              _M_make_empty(__alloc);
          }
          else
            _M_make_empty(__alloc);
        }

  template<typename _Res, typename... _ArgTypes>
    function<_Res(_ArgTypes...)>::
    function(const function& __x)
    : _Function_base()
    {
      if (static_cast<bool>(__x))
	{
          // Get a variant of __x._M_manager that doesn't use an allocator
          // Note that __get_manager_without_alloc is a new op, so we must set a
          // default value in case it is a no-op for __x.
          _Any_data __out_param;
          __out_param._M_access<_Manager_type>() = __x._M_manager;
          __x._M_manager(__out_param, __out_param,
                         __get_manager_without_alloc);
	  _M_manager = __out_param._M_access<_Manager_type>();

          // Get a variant of __x._M_invoker that does not use an allocator.
          __out_param._M_access<_Invoker_type>() = __x._M_invoker;
          this->_M_manager(__out_param, __out_param, __get_invoker);
          _M_invoker = __out_param._M_access<_Invoker_type>();
          
	  __x._M_manager(_M_functor, __x._M_functor, __clone_functor);
	}
    }

  template<typename _Res, typename... _ArgTypes>
    function<_Res(_ArgTypes...)>::
    function(function&& __x)
    : _Function_base()
    {
      // Create empty function with same allocator as __x, then swap.
      _M_make_empty(__x._M_get_shared_alloc_rsrc_ptr());
      this->swap(__x);
    }

  template<typename _Res, typename... _ArgTypes>
    template<typename _Functor>
      function<_Res(_ArgTypes...)>::
      function(_Functor __f,
	       typename enable_if<
			!is_integral<_Functor>::value, _Useless>::type)
      : _Function_base()
      {
	typedef _Function_handler_wrapper<_Signature_type, _Functor, false>
          _My_handler;

	if (_My_handler::_M_not_empty_function(__f))
	  {
	    _M_invoker = &_My_handler::_M_invoke;
	    _M_manager = &_My_handler::_M_manager;
            _Shared_alloc_rsrc_ptr dfltAllocRsrcPtr = _M_default_alloc_rsrc();
	    _My_handler::_M_init_functor(_M_functor, std::move(__f),
                                         std::move(dfltAllocRsrcPtr));
	  }
      }

  // PGH: allocator_arg_t versions
  template<typename _Res, typename... _ArgTypes>
    template<typename __A>
      function<_Res(_ArgTypes...)>::
      function(allocator_arg_t, const __A& __alloc)
      : _Function_base()
      {
        _M_make_empty(__alloc);
      }

  template<typename _Res, typename... _ArgTypes>
    template<typename __A>
      function<_Res(_ArgTypes...)>::
      function(allocator_arg_t, const __A& __alloc, nullptr_t)
      : _Function_base()
      {
        _M_make_empty(__alloc);
      }

  template<typename _Res, typename... _ArgTypes>
    template<typename __A>
      function<_Res(_ArgTypes...)>::
      function(allocator_arg_t, const __A& __alloc, const function& __x)
      : _Function_base()
        {
          _M_make_copy(__alloc, __x);
        }

  template<typename _Res, typename... _ArgTypes>
    template<typename __A>
      function<_Res(_ArgTypes...)>::
      function(allocator_arg_t, const __A& __alloc, function&& __x)
      : _Function_base()
        {
          _Shared_alloc_rsrc_ptr __x_rsrc = __x._M_get_shared_alloc_rsrc_ptr();

          if (_M_is_same_alloc_rsrc(__x_rsrc.get(), __alloc))
          {
            // Create an empty function with the correct allocator, then swap
            _M_make_empty(__x_rsrc);
            __do_swap(__x);
          }
          else
            _M_make_copy(__alloc, __x);
        }

  template<typename _Res, typename... _ArgTypes>
    template<typename _Functor, typename __A>
      function<_Res(_ArgTypes...)>::
      function(allocator_arg_t, const __A& __alloc, _Functor __f,
		 typename enable_if<
                   !is_integral<_Functor>::value, _Useless>::type)
      {
        polyalloc::allocator_resource *const __dflt_rsrc =
          polyalloc::allocator_resource::default_resource();

        if (_M_is_same_alloc_rsrc(__dflt_rsrc, __alloc))
        {
          typedef _Function_handler_wrapper<_Signature_type, _Functor, false>
            _My_handler;

          // Default allocator
          if (_My_handler::_M_not_empty_function(__f))
          {
	    _M_invoker = &_My_handler::_M_invoke;
	    _M_manager = &_My_handler::_M_manager;
            _Shared_alloc_rsrc_ptr dfltAllocRsrcPtr = _M_default_alloc_rsrc();
	    _My_handler::_M_init_functor(_M_functor, std::move(__f),
                                         std::move(dfltAllocRsrcPtr));
	  }
        }
        else
        {
          // Non-default allocator
          typedef _Function_handler_wrapper<_Signature_type, _Functor, true>
            _My_handler;

          if (_My_handler::_M_not_empty_function(__f))
          {
	    _M_invoker = &_My_handler::_M_invoke;
	    _M_manager = &_My_handler::_M_manager;
	    _My_handler::_M_init_functor(_M_functor, std::move(__f),
                                         _M_make_shared_alloc(__alloc));
	  }
          else
            _M_make_empty(__alloc);
        }
      }

  template<typename _Res, typename... _ArgTypes>
    function<_Res(_ArgTypes...)>::~function()
      {
        // If *this is empty (i.e., cannot be invoked) but is holding on to
        // an allocator, then the allocator is squirreled away in a shared_ptr
        // in _M_functor.  We must destroy it to avoid possibly leaking
        // memory.
        if (_M_empty() && &_M_null_invoker_with_alloc == _M_invoker)
        {
          _M_functor._M_access<_Shared_alloc_rsrc_ptr>().
            ~_Shared_alloc_rsrc_ptr();
          _M_invoker = nullptr;
        }
      }

  template<typename _Res, typename... _ArgTypes>
    _Res
    function<_Res(_ArgTypes...)>::
    operator()(_ArgTypes... __args) const
    {
      if (_M_empty())
	__throw_bad_function_call();
      return _M_invoker(_M_functor, std::forward<_ArgTypes>(__args)...);
    }

#ifdef __GXX_RTTI
  template<typename _Res, typename... _ArgTypes>
    const type_info&
    function<_Res(_ArgTypes...)>::
    target_type() const
    {
      if (_M_manager)
	{
	  _Any_data __typeinfo_result;
	  _M_manager(__typeinfo_result, _M_functor, __get_type_info);
	  return *__typeinfo_result._M_access<const type_info*>();
	}
      else
	return typeid(void);
    }

  template<typename _Res, typename... _ArgTypes>
    template<typename _Functor>
      _Functor*
      function<_Res(_ArgTypes...)>::
      target()
      {
	if (typeid(_Functor) == target_type() && _M_manager)
	  {
	    _Any_data __ptr;
	    if (_M_manager(__ptr, _M_functor, __get_functor_ptr)
		&& !is_const<_Functor>::value)
	      return 0;
	    else
	      return __ptr._M_access<_Functor*>();
	  }
	else
	  return 0;
      }

  template<typename _Res, typename... _ArgTypes>
    template<typename _Functor>
      const _Functor*
      function<_Res(_ArgTypes...)>::
      target() const
      {
	if (typeid(_Functor) == target_type() && _M_manager)
	  {
	    _Any_data __ptr;
	    _M_manager(__ptr, _M_functor, __get_functor_ptr);
	    return __ptr._M_access<const _Functor*>();
	  }
	else
	  return 0;
      }
#endif

  template<typename _Res, typename... _Args>
    inline
    polyalloc::allocator_resource*
    function<_Res(_Args...)>::get_allocator_resource() const
      {
        return _M_get_shared_alloc_rsrc_ptr().get();
      }


  // [20.7.15.2.6] null pointer comparisons

  /**
   *  @brief Compares a polymorphic function object wrapper against 0
   *  (the NULL pointer).
   *  @returns @c true if the wrapper has no target, @c false otherwise
   *
   *  This function will not throw an %exception.
   */
  template<typename _Res, typename... _Args>
    inline bool
    operator==(const function<_Res(_Args...)>& __f, nullptr_t)
    { return !static_cast<bool>(__f); }

  /// @overload
  template<typename _Res, typename... _Args>
    inline bool
    operator==(nullptr_t, const function<_Res(_Args...)>& __f)
    { return !static_cast<bool>(__f); }

  /**
   *  @brief Compares a polymorphic function object wrapper against 0
   *  (the NULL pointer).
   *  @returns @c false if the wrapper has no target, @c true otherwise
   *
   *  This function will not throw an %exception.
   */
  template<typename _Res, typename... _Args>
    inline bool
    operator!=(const function<_Res(_Args...)>& __f, nullptr_t)
    { return static_cast<bool>(__f); }

  /// @overload
  template<typename _Res, typename... _Args>
    inline bool
    operator!=(nullptr_t, const function<_Res(_Args...)>& __f)
    { return static_cast<bool>(__f); }

  // [20.7.15.2.7] specialized algorithms

  /**
   *  @brief Swap the targets of two polymorphic function object wrappers.
   *
   *  This function will not throw an %exception.
   */
  template<typename _Res, typename... _Args>
    inline void
    swap(function<_Res(_Args...)>& __x, function<_Res(_Args...)>& __y)
    { __x.swap(__y); }

_GLIBCXX_END_NAMESPACE_VERSION
END_NAMESPACE_XSTD

#endif // _XFUNCTIONAL

// Local Variables:
// c-basic-offset: 2
// End:
