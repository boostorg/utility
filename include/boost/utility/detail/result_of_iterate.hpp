// Boost result_of library

//  Copyright Douglas Gregor 2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  Copyright Daniel Walker, Eric Niebler, Michel Morin 2008-2012.
//  Copyright Kohei Takahashi 2016,2019.
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or
//  copy at http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org/libs/utility
#if !defined(BOOST_PP_IS_ITERATING) && !defined(BOOST_RESULT_OF_USE_VARIADICS)
# error Boost result_of - do not include this file!
#endif

#if !defined(BOOST_RESULT_OF_USE_VARIADICS)

// CWPro8 requires an argument in a function type specialization
#if BOOST_WORKAROUND(__MWERKS__, BOOST_TESTED_AT(0x3002)) && BOOST_PP_ITERATION() == 0
# define BOOST_RESULT_OF_ARGS void
#else
# define BOOST_RESULT_OF_ARGS BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(),T)
#endif

#define BOOST_RESULT_OF_typename_T  BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PP_ITERATION(),typename T)
#define BOOST_RESULT_OF_typename_T1 BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PP_DEC(BOOST_PP_ITERATION()),typename T)
#define BOOST_RESULT_OF_enum_T      BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(),T)
#define BOOST_RESULT_OF_enum_T1     BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(BOOST_PP_ITERATION()),T)
#define BOOST_RESULT_OF_trailing_T  BOOST_PP_ENUM_TRAILING_PARAMS(BOOST_PP_ITERATION(),T)
#define BOOST_RESULT_OF_declval_T   BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), boost::declval<T, >() BOOST_PP_INTERCEPT)

#define BOOST_RESULT_OF_callable2      BOOST_PP_CAT(result_of_callable_fun_2_, BOOST_PP_ITERATION())
#define BOOST_RESULT_OF_callable       BOOST_PP_CAT(result_of_callable_fun_, BOOST_PP_ITERATION())
#define BOOST_RESULT_OF_select_wrapper BOOST_PP_CAT(result_of_select_call_wrapper_type_, BOOST_PP_ITERATION())
#define BOOST_RESULT_OF_is_callable    BOOST_PP_CAT(result_of_is_callable_, BOOST_PP_ITERATION())

#else

#define BOOST_RESULT_OF_ARGS        T...
#define BOOST_RESULT_OF_typename_T  , typename... T
#define BOOST_RESULT_OF_typename_T1 , typename... T
#define BOOST_RESULT_OF_enum_T      T...
#define BOOST_RESULT_OF_enum_T1     T...
#define BOOST_RESULT_OF_trailing_T  , T...
#define BOOST_RESULT_OF_declval_T   boost::declval<T>()...

#define BOOST_RESULT_OF_callable2      result_of_callable_fun_2
#define BOOST_RESULT_OF_callable       result_of_callable_fun
#define BOOST_RESULT_OF_select_wrapper result_of_select_call_wrapper_type
#define BOOST_RESULT_OF_is_callable    result_of_is_callable

#endif

#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x551))
template<typename F BOOST_RESULT_OF_typename_T>
struct tr1_result_of<F(BOOST_RESULT_OF_ARGS)>
    : conditional<
        is_pointer<F>::value || is_member_function_pointer<F>::value
        , boost::detail::tr1_result_of_impl<
            typename remove_cv<F>::type,
            typename remove_cv<F>::type(BOOST_RESULT_OF_ARGS),
            (boost::detail::result_of_has_result_type<F>::value)>
        , boost::detail::tr1_result_of_impl<
            F,
            F(BOOST_RESULT_OF_ARGS),
            (boost::detail::result_of_has_result_type<F>::value)> >::type { };
#endif

#ifndef BOOST_RESULT_OF_TR1_ONLY

#ifdef BOOST_RESULT_OF_USE_DECLTYPE
template<typename F BOOST_RESULT_OF_typename_T>
struct result_of<F(BOOST_RESULT_OF_ARGS)>
    : detail::cpp0x_result_of<F(BOOST_RESULT_OF_ARGS)> { };
#endif // BOOST_RESULT_OF_USE_DECLTYPE

#ifdef BOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK
template<typename F BOOST_RESULT_OF_typename_T>
struct result_of<F(BOOST_RESULT_OF_ARGS)>
    : conditional<detail::result_of_has_result_type<F>::value || detail::result_of_has_result<F>::value,
               tr1_result_of<F(BOOST_RESULT_OF_ARGS)>,
               detail::cpp0x_result_of<F(BOOST_RESULT_OF_ARGS)> >::type { };
#endif // BOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK

#if defined(BOOST_RESULT_OF_USE_DECLTYPE) || defined(BOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK)

namespace detail {

template<typename F BOOST_RESULT_OF_typename_T>
struct cpp0x_result_of<F(BOOST_RESULT_OF_enum_T)>
    : conditional<
          is_member_function_pointer<F>::value
        , detail::tr1_result_of_impl<
            typename remove_cv<F>::type,
            typename remove_cv<F>::type(BOOST_RESULT_OF_enum_T), false
          >
        , detail::cpp0x_result_of_impl<
              F(BOOST_RESULT_OF_enum_T)
          >
      >::type
{};

#ifdef BOOST_NO_SFINAE_EXPR

template<typename F>
struct BOOST_RESULT_OF_callable2;

template<typename R BOOST_RESULT_OF_typename_T>
struct BOOST_RESULT_OF_callable2<R(BOOST_RESULT_OF_enum_T)> {
    R operator()(BOOST_RESULT_OF_enum_T) const;
    typedef result_of_private_type const &(*pfn_t)(...);
    operator pfn_t() const volatile;
};

template<typename F>
struct BOOST_RESULT_OF_callable;

template<typename F>
struct BOOST_RESULT_OF_callable<F *>
  : BOOST_RESULT_OF_callable2<F>
{};

template<typename F>
struct BOOST_RESULT_OF_select_wrapper
  : conditional<
        is_class<typename remove_reference<F>::type>::value,
        result_of_wrap_callable_class<F>,
        type_identity<BOOST_RESULT_OF_callable<typename remove_cv<typename remove_reference<F>::type>::type> >
    >::type
{};

template<typename F BOOST_RESULT_OF_typename_T>
struct BOOST_RESULT_OF_is_callable {
    typedef typename BOOST_RESULT_OF_select_wrapper<F>::type wrapper_t;
    static const bool value = (
        sizeof(result_of_no_type) == sizeof(detail::result_of_is_private_type(
            (boost::declval<wrapper_t>()(BOOST_RESULT_OF_declval_T), result_of_weird_type())
        ))
    );
    typedef integral_constant<bool, value> type;
};

template<typename F BOOST_RESULT_OF_typename_T>
struct cpp0x_result_of_impl<F(BOOST_RESULT_OF_enum_T), true>
    : lazy_enable_if<
          BOOST_RESULT_OF_is_callable<F BOOST_RESULT_OF_trailing_T>
        , cpp0x_result_of_impl<F(BOOST_RESULT_OF_enum_T), false>
      >
{};

template<typename F BOOST_RESULT_OF_typename_T>
struct cpp0x_result_of_impl<F(BOOST_RESULT_OF_enum_T), false>
{
  typedef decltype(boost::declval<F>()(BOOST_RESULT_OF_declval_T)) type;
};

#else // BOOST_NO_SFINAE_EXPR

template<typename F BOOST_RESULT_OF_typename_T>
struct cpp0x_result_of_impl<F(BOOST_RESULT_OF_enum_T),
                            typename result_of_always_void<decltype(
                                boost::declval<F>()(BOOST_RESULT_OF_declval_T)
                            )>::type> {
  typedef decltype(boost::declval<F>()(BOOST_RESULT_OF_declval_T)) type;
};

#endif // BOOST_NO_SFINAE_EXPR

} // namespace detail

#else // defined(BOOST_RESULT_OF_USE_DECLTYPE) || defined(BOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK)

#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x551))
template<typename F BOOST_RESULT_OF_typename_T>
struct result_of<F(BOOST_RESULT_OF_ARGS)>
    : tr1_result_of<F(BOOST_RESULT_OF_ARGS)> { };
#endif

#endif // defined(BOOST_RESULT_OF_USE_DECLTYPE)

#endif // defined(BOOST_RESULT_OF_TR1_ONLY)

#undef BOOST_RESULT_OF_ARGS

#if defined(BOOST_RESULT_OF_USE_VARIADICS) || (BOOST_PP_ITERATION() >= 1)

namespace detail {

template<typename R,  typename FArgs BOOST_RESULT_OF_typename_T>
struct tr1_result_of_impl<R (*)(BOOST_RESULT_OF_enum_T), FArgs, false>
{
  typedef R type;
};

template<typename R,  typename FArgs BOOST_RESULT_OF_typename_T>
struct tr1_result_of_impl<R (&)(BOOST_RESULT_OF_enum_T), FArgs, false>
{
  typedef R type;
};

#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x551))
template<typename R, typename FArgs, typename C BOOST_RESULT_OF_typename_T1>
struct tr1_result_of_impl<R (C::*)
                     (BOOST_RESULT_OF_enum_T1),
                 FArgs, false>
{
  typedef R type;
};

template<typename R, typename FArgs, typename C BOOST_RESULT_OF_typename_T1>
struct tr1_result_of_impl<R (C::*)
                     (BOOST_RESULT_OF_enum_T1)
                     const,
                 FArgs, false>
{
  typedef R type;
};

template<typename R, typename FArgs, typename C BOOST_RESULT_OF_typename_T1>
struct tr1_result_of_impl<R (C::*)
                     (BOOST_RESULT_OF_enum_T1)
                     volatile,
                 FArgs, false>
{
  typedef R type;
};

template<typename R, typename FArgs, typename C BOOST_RESULT_OF_typename_T1>
struct tr1_result_of_impl<R (C::*)
                     (BOOST_RESULT_OF_enum_T1)
                     const volatile,
                 FArgs, false>
{
  typedef R type;
};
#endif

}
#endif
