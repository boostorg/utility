//-----------------------------------------------------------------------------
// boost swap.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_SWAP_HPP
#define BOOST_SWAP_HPP

#include "boost/move.hpp"

namespace boost {

//////////////////////////////////////////////////////////////////////////
// function template swap
//
// Swaps using Koenig lookup but falls back to move-swap for primitive
// types and on non-conforming compilers.
//

namespace detail { namespace move_swap {

template <typename T>
inline void swap(T& lhs, T& rhs)
{
    T tmp( move(lhs) );
    lhs = move(rhs);
    rhs = move(tmp);
}

#ifdef __GNUC__
using boost::detail::move_swap::swap;
#endif // __GNUC_ workaround

template <typename T>
inline void swap_impl(T& lhs, T& rhs)
{
#ifndef __GNUC__
    using boost::detail::move_swap::swap;
#endif // __GNUC__ workaround

    swap(lhs, rhs);
}

}} // namespace detail::move_swap

template <typename T>
inline void swap(T& lhs, T& rhs)
{
    detail::move_swap::swap_impl(lhs, rhs);
}

} // namespace boost

#endif // BOOST_SWAP_HPP
