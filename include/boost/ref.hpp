#ifndef BOOST_REF_HPP_INCLUDED
# define BOOST_REF_HPP_INCLUDED

# if _MSC_VER+0 >= 1020
#  pragma once
# endif

# include <boost/config.hpp>
# include <boost/utility/addressof.hpp>
# include <boost/type_traits/ice.hpp>
# include <boost/type.hpp>
# ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#  include <boost/result_of.hpp>
# endif
# include <boost/preprocessor/iterate.hpp>
# include <boost/pending/ct_if.hpp>

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

# ifndef BOOST_REF_NUM_ARGS
#  define BOOST_REF_NUM_ARGS 10
# endif

namespace boost
{

namespace detail { namespace ref {

template<typename T>
class reference_wrapper_without_result_type
{
public:
  template<typename F>
  struct result_of
# ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
      : boost::result_of<F>
# endif 
  {
  };

  operator T& () const { return *(this->t_); }
  T& get() const { return *(this->t_); }
  T* get_pointer() const { return this->t_; }

# if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)         \
    && !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x551))
#  define BOOST_PP_ITERATION_PARAMS_1 (3,(0,BOOST_REF_NUM_ARGS,<boost/detail/ref_iterate.hpp>))
#  include BOOST_PP_ITERATE()
# endif 

protected:
# if defined(BOOST_MSVC) && (BOOST_MSVC < 1300)
  explicit reference_wrapper_without_result_type(T& t) : t_(&t) {}
# else
  explicit reference_wrapper_without_result_type(T& t) : t_(addressof(t)) {}
# endif

private:
  T* t_;
};

# ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template<typename T>
class reference_wrapper_with_result_type
{
public:
  typedef typename T::result_type result_type;

  operator T& () const { return *(this->t_); }
  T& get() const { return *(this->t_); }
  T* get_pointer() const { return this->t_; }

  result_type operator()() const { return get()(); }

#  if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x551))
#   define BOOST_PP_ITERATION_PARAMS_1 (3,(0,BOOST_REF_NUM_ARGS,<boost/detail/ref_iterate.hpp>))
#   include BOOST_PP_ITERATE()
#  endif 

protected:
#  if defined(BOOST_MSVC) && (BOOST_MSVC < 1300)
  explicit reference_wrapper_with_result_type(T& t) : t_(&t) {}
#  else
  explicit reference_wrapper_with_result_type(T& t) : t_(addressof(t)) {}
#  endif

private:
  T* t_;
};

template<typename T>
class reference_wrapper_impl :
  public ct_if<(has_result_type<T>::value),
               reference_wrapper_with_result_type<T>,
               reference_wrapper_without_result_type<T> >::type
{
  typedef typename ct_if<(has_result_type<T>::value),
                         reference_wrapper_with_result_type<T>,
                         reference_wrapper_without_result_type<T> >::type
    inherited;

protected:
  reference_wrapper_impl(T& t) : inherited(t) {}
};
# else
template<typename T>
class reference_wrapper_impl : public reference_wrapper_without_result_type<T>
{
  typedef reference_wrapper_without_result_type<T> inherited;

protected:
  reference_wrapper_impl(T& t) : inherited(t) {}
};
# endif

} } // end namespace detail::ref

template<class T>
class reference_wrapper : public detail::ref::reference_wrapper_impl<T>
{
  typedef detail::ref::reference_wrapper_impl<T> inherited;

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

# undef BOOST_REF_CONST

# ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template<typename T>
class is_reference_wrapper
{
 public:
    BOOST_STATIC_CONSTANT(bool, value = false);
};

template<typename T>
class is_reference_wrapper<reference_wrapper<T> >
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

template<typename T>
class unwrap_reference<reference_wrapper<T> >
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

  template<typename T>
  yes_reference_wrapper_t
  is_reference_wrapper_test(type< reference_wrapper<T> >);

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
