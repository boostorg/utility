// (C) Copyright Jonathan Turkanis 2004.
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies. This
// software is provided "as is" without express or implied warranty, and
// with no claim as to its suitability for any purpose.

#ifndef BOOST_UTILITY_SELECT_BY_SIZE_HPP_INCLUDED
#define BOOST_UTILITY_SELECT_BY_SIZE_HPP_INCLUDED

#include <boost/config.hpp>                 // BOOST_STATIC_CONSTANT.
#include <boost/mpl/int.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#ifndef BOOST_SELECT_BY_SIZE_MAX_CASE
#define BOOST_SELECT_BY_SIZE_MAX_CASE 30
#endif

namespace boost { namespace utility {

//--------------Definition of case_-------------------------------------------//

template<unsigned N> struct case_;
template<> struct case_<0> { char c; };
template<unsigned N> struct case_ { char c1; case_<N - 1> c2; };
typedef case_<true> yes_type;
typedef case_<false> no_type;

//--------------Definition of select_by_size----------------------------------//

template<unsigned Size> struct select_by_size;

// Specialize select_by_size for values sizeof(case_<0>), sizeof(case_<1>),
// ..., sizeof(case_(BOOST_SELECT_BY_SIZE_MAX_CASE>).
#define SELECT_BY_SIZE_SPEC(z, n, text)                                        \
    namespace detail {                                                         \
        static const int BOOST_PP_CAT(sizeof_case_, n) = sizeof(case_<n>);     \
    }                                                                          \
    template<>                                                                 \
    struct select_by_size< detail::BOOST_PP_CAT(sizeof_case_, n) >             \
        : mpl::int_<n> { };                                                    \
    /**/
BOOST_PP_REPEAT(BOOST_SELECT_BY_SIZE_MAX_CASE, SELECT_BY_SIZE_SPEC, _);
#undef SELECT_BY_SIZE_SPEC

} }             // End namespaces utility, boost.

//--------------Definition of SELECT_BY_SIZE----------------------------------//

#define BOOST_SELECT_BY_SIZE_ASSIGN(lhs, rhs) lhs = rhs
#define BOOST_SELECT_BY_SIZE(type_, name, expr)                                \
    BOOST_STATIC_CONSTANT(                                                     \
        unsigned,                                                              \
        BOOST_PP_CAT(boost_select_by_size_temp_, name) = sizeof(expr)          \
    );                                                                         \
    BOOST_STATIC_CONSTANT(                                                     \
        type_,                                                                 \
        name =                                                                 \
            ( boost::utility::select_by_size<                                  \
                BOOST_PP_CAT(boost_select_by_size_temp_, name)                 \
              >::type::value )                                                 \
    )                                                                          \
    /**/

#undef BOOST_SELECT_BY_SIZE_MAX_CASE

#endif          // #ifndef BOOST_UTILITY_SELECT_BY_SIZE_HPP_INCLUDED
