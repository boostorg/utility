#ifndef BOOST_REF_HPP_INCLUDED
# define BOOST_REF_HPP_INCLUDED

# if _MSC_VER+0 >= 1020
#  pragma once
# endif

# include <boost/config.hpp>
# include <boost/utility/addressof.hpp>
# include <boost/type_traits/ice.hpp>
# include <boost/type.hpp>

//
//  ref.hpp - ref/cref, useful helper functions
//
//  Copyright (C) 1999, 2000 Jaakko Järvi (jaakko.jarvi@cs.utu.fi)
//  Copyright (C) 2001, 2002 Peter Dimov
//  Copyright (C) 2002 David Abrahams
//  Copyright (C) 2003 Doug Gregor
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org/libs/bind/ref.html for documentation.
//

namespace boost
{

struct deduce_ref_result {};

namespace detail { namespace ref {

// ------ has_result_type ---
template<typename T>
type_traits::yes_type has_result_type_helper(type<typename T::result_type>*);

template<typename T> type_traits::no_type has_result_type_helper(...);

template<typename T>
struct has_result_type
{
  BOOST_STATIC_CONSTANT(bool,
                        value = (sizeof(has_result_type_helper<T>(0))
                                   == sizeof(type_traits::yes_type)));
};

// ------ has_argument_type ---
template<typename T>
type_traits::yes_type
has_argument_type_helper(type<typename T::argument_type>*);

template<typename T> type_traits::no_type has_argument_type_helper(...);

template<typename T>
struct has_argument_type
{
  BOOST_STATIC_CONSTANT(bool,
                        value = (sizeof(has_argument_type_helper<T>(0))
                                   == sizeof(type_traits::yes_type)));
};

// ------ has_first_and_second_argument_types ---
template<typename T>
type_traits::yes_type has_first_and_second_argument_types_helper(
                        type<typename T::first_argument_type>*,
                        type<typename T::second_argument_type>*);

template<typename T>
type_traits::no_type has_first_and_second_argument_types_helper(...);

template<typename T>
struct has_first_and_second_argument_types
{
  BOOST_STATIC_CONSTANT(bool,
    value = (sizeof(has_first_and_second_argument_types_helper<T>(0, 0))
             == sizeof(type_traits::yes_type)));
};

// ------ has_argN_type ---
template<typename T, int N>
struct has_argN_type
{
  BOOST_STATIC_CONSTANT(bool, value = false);
};

#define BOOST_REF_HAS_ARGN_TYPE(N)                                       \
template<typename T>                                                     \
type_traits::yes_type                                                    \
has_arg##N##_type_helper(type<typename T::arg##N##_type>*);              \
                                                                         \
template<typename T> type_traits::no_type has_arg##N##_type_helper(...); \
                                                                         \
template<typename T>                                                     \
struct has_arg##N##_type                                                 \
{                                                                        \
  BOOST_STATIC_CONSTANT(bool,                                            \
                        value = (sizeof(has_arg##N##_type_helper<T>(0))  \
                                   == sizeof(type_traits::yes_type)));   \
};                                                                       \
                                                                         \
template<typename T>                                                     \
struct has_argN_type<T, N>                                               \
{                                                                        \
  BOOST_STATIC_CONSTANT(bool, value = (has_arg##N##_type<T>::value));    \
};

BOOST_REF_HAS_ARGN_TYPE(1)
BOOST_REF_HAS_ARGN_TYPE(2)
BOOST_REF_HAS_ARGN_TYPE(3)
BOOST_REF_HAS_ARGN_TYPE(4)
BOOST_REF_HAS_ARGN_TYPE(5)
BOOST_REF_HAS_ARGN_TYPE(6)
BOOST_REF_HAS_ARGN_TYPE(7)
BOOST_REF_HAS_ARGN_TYPE(8)
BOOST_REF_HAS_ARGN_TYPE(9)
BOOST_REF_HAS_ARGN_TYPE(10)
#undef BOOST_REF_HAS_ARGN_TYPE

// ------ get_result_type0 ---
template<bool HasResultType, typename T> struct get_result_type0_impl;

template<typename T>
struct get_result_type0_impl<true, T>
{
  typedef typename T::result_type type;
};

template<typename T>
struct get_result_type0_impl<false, T>
{
  typedef void type;
};

template<typename T>
struct get_result_type0 :
  public get_result_type0_impl<(has_result_type<T>::value), T>
{
};

// ------ get_result_type1 ---
template<bool HasResultType, typename T, typename T1>
struct get_result_type1_impl;

template<typename T, typename T1>
struct get_result_type1_impl<true, T, T1>
{
  typedef typename T::result_type type;
};

template<typename T, typename T1>
struct get_result_type1_impl<false, T, T1>
{
  typedef typename T::template sig<T1>::type type;
};

template<typename T, typename T1>
struct get_result_type1 :
  public get_result_type1_impl<(has_result_type<T>::value), T, T1> {};

// ------ get_result_type2 ---
template<bool HasResultType, typename T, typename T1, typename T2>
struct get_result_type2_impl;

template<typename T, typename T1, typename T2>
struct get_result_type2_impl<true, T, T1, T2>
{
  typedef typename T::result_type type;
};

template<typename T, typename T1, typename T2>
struct get_result_type2_impl<false, T, T1, T2>
{
  typedef typename T::template sig<T1, T2>::type type;
};

template<typename T, typename T1, typename T2>
struct get_result_type2 :
  public get_result_type2_impl<(has_result_type<T>::value), T, T1, T2> {};

// ------ get_result_type3 ---
template<bool HasResultType, typename T, typename T1, typename T2, typename T3>
struct get_result_type3_impl;

template<typename T, typename T1, typename T2, typename T3>
struct get_result_type3_impl<true, T, T1, T2, T3>
{
  typedef typename T::result_type type;
};

template<typename T, typename T1, typename T2, typename T3>
struct get_result_type3_impl<false, T, T1, T2, T3>
{
  typedef typename T::template sig<T1, T2, T3>::type type;
};

template<typename T, typename T1, typename T2, typename T3>
struct get_result_type3 :
  public get_result_type3_impl<(has_result_type<T>::value), T, T1, T2, T3> {};

// ------ get_result_type ---
template<typename Result, typename Deducer>
struct get_result_type
{
  typedef Result type;
};

template<typename Deducer>
struct get_result_type<deduce_ref_result, Deducer>
{
  typedef typename Deducer::type type;
};

// ------ reference_wrapper_base
template<typename T, typename Result>
class reference_wrapper_base
{
public:
  operator T& () const { return *(this->t_); }
  T& get() const { return *(this->t_); }
  T* get_pointer() const { return this->t_; }

  typename get_result_type<Result, get_result_type0<T> >::type
  operator()() const
  {
    return get()();
  }

protected:
#if defined(BOOST_MSVC) && (BOOST_MSVC < 1300)
  explicit reference_wrapper_base(T& t) : t_(&t) {}
#else
  explicit reference_wrapper_base(T& t) : t_(addressof(t)) {}
#endif

private:
  T* t_;
};

// ------ reference_wrapper_with_first_and_second_argument_types
template<typename T, typename Result>
class reference_wrapper_with_first_and_second_argument_types
  : public reference_wrapper_base<T, Result>
{
  typedef reference_wrapper_base<T, Result> inherited;

protected:
  explicit reference_wrapper_with_first_and_second_argument_types(T& t)
    : inherited(t) {}

public:
  typedef typename T::first_argument_type first_argument_type;
  typedef typename T::second_argument_type second_argument_type;

  using inherited::operator();

  typename get_result_type<Result,
                           get_result_type2<T, first_argument_type,
                                            second_argument_type> >::type
  operator()(const first_argument_type& a1,
             const second_argument_type& a2) const
  {
    return this->get()(a1, a2);
  }
};

// ------ reference_wrapper_with_argument_type
template<typename T, typename Result, bool Available>
class maybe_also_first_and_second_argument_types;


template<typename T, typename Result>
class maybe_also_first_and_second_argument_types<T, Result, true>
  : public reference_wrapper_with_first_and_second_argument_types<T, Result>
{
  typedef reference_wrapper_with_first_and_second_argument_types<T, Result>
    inherited;

protected:
  explicit maybe_also_first_and_second_argument_types(T& t) : inherited(t) {}

public:
  using inherited::operator();
};

template<typename T, typename Result>
class maybe_also_first_and_second_argument_types<T, Result, false>
  : public reference_wrapper_base<T, Result>
{
  typedef reference_wrapper_base<T, Result> inherited;

protected:
  explicit maybe_also_first_and_second_argument_types(T& t) : inherited(t) {}

public:
  using inherited::operator();
};

template<typename T, typename Result>
class reference_wrapper_with_argument_type
  : public maybe_also_first_and_second_argument_types<
             T, Result,
             (has_first_and_second_argument_types<T>::value)>
{
  typedef maybe_also_first_and_second_argument_types<
            T, Result,
            (has_first_and_second_argument_types<T>::value)> inherited;

protected:
  explicit reference_wrapper_with_argument_type(T& t) : inherited(t) {}

public:
  typedef typename T::argument_type argument_type;

  using inherited::operator();

  typename get_result_type<Result, get_result_type1<T, argument_type> >::type
  operator()(const argument_type& a1) const
  {
    return this->get()(a1);
  }
};

// ------ reference_wrapper_with_argN_types
template<typename T, typename Result, int N, bool Exists>
class reference_wrapper_with_argN_types_impl;

template<typename T, typename Result, int N=1>
class reference_wrapper_with_argN_types
  : public reference_wrapper_with_argN_types_impl<T, Result, N,
                                                  (has_argN_type<T, N>::value)>
{
  typedef reference_wrapper_with_argN_types_impl<T, Result, N,
                                                 (has_argN_type<T, N>::value)>
    inherited;

protected:
  explicit reference_wrapper_with_argN_types(T& t) : inherited(t) {}
};

template<typename T, typename Result>
class reference_wrapper_with_argN_types_impl<T, Result, 1, true>
  : public reference_wrapper_with_argN_types<T, Result, 2>
{
  typedef reference_wrapper_with_argN_types<T, Result, 2> inherited;

protected:
  explicit reference_wrapper_with_argN_types_impl(T& t) : inherited(t) {}

public:
  typedef typename T::arg1_type arg1_type;

  using inherited::operator();

  typename get_result_type<Result, get_result_type1<T, arg1_type> >::type
  operator()(arg1_type a1) const
  {
    return this->get()(a1);
  }
};

template<typename T, typename Result>
class reference_wrapper_with_argN_types_impl<T, Result, 2, true>
  : public reference_wrapper_with_argN_types<T, Result, 3>
{
  typedef reference_wrapper_with_argN_types<T, Result, 3> inherited;

protected:
  explicit reference_wrapper_with_argN_types_impl(T& t) : inherited(t) {}

public:
  typedef typename T::arg1_type arg1_type;
  typedef typename T::arg2_type arg2_type;

  using inherited::operator();

  typename get_result_type<Result,
                           get_result_type2<T, arg1_type, arg2_type> >::type
  operator()(arg1_type a1, arg2_type a2) const
  {
    return this->get()(a1, a2);
  }
};

template<typename T, typename Result>
class reference_wrapper_with_argN_types_impl<T, Result, 3, true>
  : public reference_wrapper_with_argN_types<T, Result, 4>
{
  typedef reference_wrapper_with_argN_types<T, Result, 4> inherited;

protected:
  explicit reference_wrapper_with_argN_types_impl(T& t) : inherited(t) {}

public:
  typedef typename T::arg1_type arg1_type;
  typedef typename T::arg2_type arg2_type;
  typedef typename T::arg3_type arg3_type;

  using inherited::operator();

  typename get_result_type<Result,
                           get_result_type3<T, arg1_type, arg2_type, arg3_type>
                           >::type
  operator()(arg1_type a1, arg2_type a2, arg3_type a3) const
  {
    return this->get()(a1, a2, a3);
  }
};

// Passed the number of arguments we know how to handle
template<typename T, typename Result, int N>
class reference_wrapper_with_argN_types_impl<T, Result, N, true>
  : public reference_wrapper_base<T, Result>
{
  typedef reference_wrapper_base<T, Result> inherited;

protected:
  explicit reference_wrapper_with_argN_types_impl(T& t) : inherited(t) {}

public:
  using inherited::operator();
};

// Underlying function object does not have any more argument types
template<typename T, typename Result, int N>
class reference_wrapper_with_argN_types_impl<T, Result, N, false>
  : public reference_wrapper_base<T, Result>
{
  typedef reference_wrapper_base<T, Result> inherited;

protected:
  explicit reference_wrapper_with_argN_types_impl(T& t) : inherited(t) {}

public:
  using inherited::operator();
};

// ------ reference_wrapper_with_deduced_args
template<typename T, typename Result>
class reference_wrapper_with_deduced_args :
  public reference_wrapper_base<T, Result>
{
  typedef reference_wrapper_base<T, Result> inherited;

protected:
  explicit reference_wrapper_with_deduced_args(T& t) : inherited(t) {}

public:
  using inherited::operator();

  template<typename T1>
  typename get_result_type<Result, get_result_type1<T, T1> >::type
  operator()(T1& a1) const
    { return this->get()(a1); }

  template<typename T1, typename T2>
  typename get_result_type<Result, get_result_type2<T, T1, T2> >::type
  operator()(T1& a1, T2& a2) const
    { return this->get()(a1, a2); }

  template<typename T1, typename T2, typename T3>
  typename get_result_type<Result, get_result_type3<T, T1, T2, T3> >::type
  operator()(T1& a1, T2& a2, T3& a3) const
    { return this->get()(a1, a2, a3); }
};

// ------ reference_wrapper_operators
template<typename T, typename Result, bool Available>
class maybe_with_first_and_second_argument_types;

template<typename T, typename Result>
class maybe_with_first_and_second_argument_types<T, Result, true>
  : public reference_wrapper_with_first_and_second_argument_types<T, Result>
{
  typedef reference_wrapper_with_first_and_second_argument_types<T, Result>
    inherited;

protected:
  explicit maybe_with_first_and_second_argument_types(T& t) : inherited(t) {}
};

template<typename T, typename Result>
class maybe_with_first_and_second_argument_types<T, Result, false>
  : public reference_wrapper_with_deduced_args<T, Result>
{
  typedef reference_wrapper_with_deduced_args<T, Result> inherited;

protected:
  explicit maybe_with_first_and_second_argument_types(T& t) : inherited(t) {}
};

template<typename T, typename Result, bool Available>
class maybe_with_argument_type;

template<typename T, typename Result>
class maybe_with_argument_type<T, Result, true>
  : public reference_wrapper_with_argument_type<T, Result>
{
  typedef reference_wrapper_with_argument_type<T, Result> inherited;

protected:
  explicit maybe_with_argument_type(T& t) : inherited(t) {}
};

template<typename T, typename Result>
class maybe_with_argument_type<T, Result, false>
  : public maybe_with_first_and_second_argument_types<
             T, Result,
             (has_first_and_second_argument_types<T>::value)>
{
  typedef maybe_with_first_and_second_argument_types<
             T, Result,
             (has_first_and_second_argument_types<T>::value)>
    inherited;

protected:
  explicit maybe_with_argument_type(T& t) : inherited(t) {}
};

template<typename T, typename Result, bool Available>
class maybe_with_argN_types;

template<typename T, typename Result>
class maybe_with_argN_types<T, Result, true>
  : public reference_wrapper_with_argN_types<T, Result>
{
  typedef reference_wrapper_with_argN_types<T, Result> inherited;

protected:
  explicit maybe_with_argN_types(T& t) : inherited(t) {}
};

template<typename T, typename Result>
class maybe_with_argN_types<T, Result, false>
  : public maybe_with_argument_type<T, Result, (has_argument_type<T>::value)>
{
  typedef maybe_with_argument_type<T, Result, (has_argument_type<T>::value)>
    inherited;

protected:
  explicit maybe_with_argN_types(T& t) : inherited(t) {}
};

template<typename T, typename Result>
class reference_wrapper_operators
  : public maybe_with_argN_types<T, Result, (has_arg1_type<T>::value)>
{
  typedef maybe_with_argN_types<T, Result, (has_arg1_type<T>::value)>
    inherited;

protected:
  explicit reference_wrapper_operators(T& t) : inherited(t) {}
};

} } // end namespace detail::ref

template<class T, typename Result = deduce_ref_result>
class reference_wrapper
  : public detail::ref::reference_wrapper_operators<T, Result>
{
  typedef detail::ref::reference_wrapper_operators<T, Result> inherited;

public:
  typedef T type;

  explicit reference_wrapper(T& t) : inherited(t) {}
};

# if defined(__BORLANDC__) && (__BORLANDC__ <= 0x570)
#  define BOOST_REF_CONST
# else
#  define BOOST_REF_CONST const
# endif

template<class T> inline reference_wrapper<T> BOOST_REF_CONST ref(T & t)
{
    return reference_wrapper<T>(t);
}

template<class T> inline reference_wrapper<T const> BOOST_REF_CONST cref(T const & t)
{
    return reference_wrapper<T const>(t);
}

template<typename Result, class T>
inline reference_wrapper<T, Result> BOOST_REF_CONST ref(T & t)
{
    return reference_wrapper<T, Result>(t);
}

template<typename Result, class T>
inline reference_wrapper<T const, Result> BOOST_REF_CONST cref(T const & t)
{
    return reference_wrapper<T const, Result>(t);
}

# undef BOOST_REF_CONST

# ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template<typename T>
class is_reference_wrapper
{
 public:
    BOOST_STATIC_CONSTANT(bool, value = false);
};

template<typename T, typename Result>
class is_reference_wrapper<reference_wrapper<T, Result> >
{
 public:
    BOOST_STATIC_CONSTANT(bool, value = true);
};

template<typename T>
class unwrap_reference
{
 public:
    typedef T type;
};

template<typename T, typename Result>
class unwrap_reference<reference_wrapper<T, Result> >
{
 public:
    typedef T type;
};
# else // no partial specialization

} // namespace boost

namespace boost
{

namespace detail
{
  typedef char (&yes_reference_wrapper_t)[1];
  typedef char (&no_reference_wrapper_t)[2];

  no_reference_wrapper_t is_reference_wrapper_test(...);

  template<typename T, typename Result>
  yes_reference_wrapper_t
  is_reference_wrapper_test(type< reference_wrapper<T, Result> >);

  template<bool wrapped>
  struct reference_unwrapper
  {
      template <class T>
      struct apply
      {
          typedef T type;
      };
  };

  template<>
  struct reference_unwrapper<true>
  {
      template <class T>
      struct apply
      {
          typedef typename T::type type;
      };
  };
}

template<typename T>
class is_reference_wrapper
{
 public:
    BOOST_STATIC_CONSTANT(
        bool, value = (
            sizeof(detail::is_reference_wrapper_test(type<T>()))
            == sizeof(detail::yes_reference_wrapper_t)));
};

template <typename T>
class unwrap_reference
    : public detail::reference_unwrapper<
        is_reference_wrapper<T>::value
      >::template apply<T>
{};

# endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

} // namespace boost

#endif // #ifndef BOOST_REF_HPP_INCLUDED
