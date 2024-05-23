// boost/utility/cstring_view.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2022 Klemens D. Morgenstern
//                    (klemens dot morgenstern at gmx dot net)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_UTILITY_CSTRING_VIEW_HPP
#define BOOST_UTILITY_CSTRING_VIEW_HPP

#include <boost/utility/string_view.hpp>
#include <algorithm>
#include <type_traits>
#include <limits>

namespace boost
{

namespace detail
{
BOOST_CONSTEXPR static const char*     null_char_(char)     {return "";}
BOOST_CONSTEXPR static const wchar_t*  null_char_(wchar_t)  {return L"";}
#if !defined(BOOST_NO_CXX11_CHAR16_T)
BOOST_CONSTEXPR static const char16_t* null_char_(char16_t) {return u"";}
#endif
#if !defined(BOOST_NO_CXX11_CHAR32_T)
BOOST_CONSTEXPR static const char32_t* null_char_(char32_t) {return U"";}
#endif
#if defined(__cpp_char8_t) && __cpp_char8_t >= 201811L
BOOST_CONSTEXPR static const char8_t* null_char_(char8_t) {return u8"";}
#endif

}


//based on http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1402r0.pdf
///cstring_view is simply a forwarding adapter around string_view.
/** The amount of functionality is minimally and only implemente functions that are either necessary
 * like begin, end and size, or function that make sense to be used without obtaining the length first,
 * like starts_with. Do note that char_traits commonly use memcmp and similar functions internally.
 *
 * FOr anything else one should use a string_view, since constructing a string_view from a cstring
 * is one pointer c
*/
template<typename CharT, typename Traits = std::char_traits<CharT>>
struct basic_cstring_view
{
    using value_type             = CharT;
    using traits_type            = Traits;

    BOOST_CONSTEXPR basic_cstring_view() : view_(detail::null_char_(value_type{})) {};
    BOOST_CONSTEXPR basic_cstring_view(std::nullptr_t) = delete;

    BOOST_CONSTEXPR basic_cstring_view( const value_type* s ) : view_(s) {}

    template<typename Source,
            typename =
            typename std::enable_if<
                    std::is_same<const value_type,
                            typename std::remove_pointer<decltype(std::declval<Source>().c_str())>::type
                    >::value>::type>
    BOOST_CONSTEXPR basic_cstring_view(Source && src) : view_(src.c_str()) {}

    BOOST_CONSTEXPR typename std::basic_string_view<value_type, Traits>::const_pointer c_str() const BOOST_NOEXCEPT
    {
        return this->data();
    }

    using string_view_type = basic_string_view<value_type, Traits>;
    constexpr operator string_view_type() const {return view_;}

    using pointer                =       CharT *;
    using const_pointer          = const CharT *;
    using reference              =       CharT &;
    using const_reference        = const CharT &;
    using const_iterator         = const_pointer;
    using iterator               = const_iterator;
        using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;
    using reverse_iterator       = typename std::reverse_iterator<iterator>;
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;

    static BOOST_CONSTEXPR size_type npos = -1;

    BOOST_CONSTEXPR const_iterator begin()  const BOOST_NOEXCEPT {return view_;};
    BOOST_CONSTEXPR const_iterator end()    const BOOST_NOEXCEPT {return view_ + length();};
    BOOST_CONSTEXPR const_iterator cbegin() const BOOST_NOEXCEPT {return view_;};
    BOOST_CONSTEXPR const_iterator cend()   const BOOST_NOEXCEPT {return view_ + length();};
    BOOST_CONSTEXPR const_reverse_iterator rbegin()  const BOOST_NOEXCEPT {return std::make_reverse_iterator(view_ + length());};
    BOOST_CONSTEXPR const_reverse_iterator rend()    const BOOST_NOEXCEPT {return std::make_reverse_iterator(view_);};
    BOOST_CONSTEXPR const_reverse_iterator crbegin() const BOOST_NOEXCEPT {return std::make_reverse_iterator(view_ + length());};
    BOOST_CONSTEXPR const_reverse_iterator crend()   const BOOST_NOEXCEPT {return std::make_reverse_iterator(view_);};

    BOOST_CONSTEXPR size_type size() const BOOST_NOEXCEPT {return length(); }
    BOOST_CONSTEXPR size_type length() const BOOST_NOEXCEPT {return traits_type::length(view_); }
    BOOST_CONSTEXPR size_type max_size() const BOOST_NOEXCEPT {return std::numeric_limits<int64_t>::max() / sizeof(CharT); }
    BOOST_ATTRIBUTE_NODISCARD BOOST_CONSTEXPR bool empty() const BOOST_NOEXCEPT {return *view_ == *detail::null_char_(CharT{}); }

    BOOST_CONSTEXPR const_reference operator[](size_type pos) const  {return view_[pos] ;}
    BOOST_CONSTEXPR const_reference at(size_type pos) const
    {
        if (pos >= size())
            throw std::out_of_range("cstring-view out of range");
        return view_[pos];
    }
    BOOST_CONSTEXPR const_reference front() const  {return *view_;}
    BOOST_CONSTEXPR const_reference back()  const  {return view_[length() - 1];}
    BOOST_CONSTEXPR const_pointer data()    const BOOST_NOEXCEPT  {return view_;}
    BOOST_CONSTEXPR void remove_prefix(size_type n)  {view_ = view_ + n;}
    BOOST_CONSTEXPR void swap(basic_cstring_view& s) BOOST_NOEXCEPT  {std::swap(view_, s.view_);}

    size_type copy(value_type* s, size_type n, size_type pos = 0) const
    {
        return traits_type::copy(s, view_ + pos, n) - view_;
    }
    BOOST_CONSTEXPR basic_cstring_view substr(size_type pos = 0) const
    {
        return basic_cstring_view(view_ + pos);
    }
    BOOST_CXX14_CONSTEXPR string_view_type substr(size_type pos , size_type n) const
    {
        return string_view_type(view_ + pos, std::min(n, length() - pos));
    }

    BOOST_CXX14_CONSTEXPR int compare(basic_cstring_view x) const BOOST_NOEXCEPT
    {
        auto idx = 0u;
        for (; view_[idx] != null_char_()[0] && x[idx] != null_char_()[0]; idx++)
            if (!traits_type::eq(view_[idx], x[idx]))
                return traits_type::lt(view_[idx], x[idx]) ? -1 : 1;

        return traits_type::to_int_type(view_[idx]) -
               traits_type::to_int_type(x[idx]); // will compare to null char of either.
    }

    BOOST_CONSTEXPR bool starts_with(string_view_type x) const BOOST_NOEXCEPT
    {
        if (x.empty())
            return true;

        auto idx = 0u;
        for (; view_[idx] != null_char_()[0] && idx < x.size(); idx++)
            if (!traits_type::eq(view_[idx], x[idx]))
                return false;

        return idx == x.size() || view_[idx] != null_char_()[0];
    }
    BOOST_CONSTEXPR bool starts_with(value_type x)       const BOOST_NOEXCEPT
    {
      return traits_type::eq(view_[0], x);
    }

    friend BOOST_CXX14_CONSTEXPR bool operator==(basic_cstring_view x, basic_cstring_view y) BOOST_NOEXCEPT
    {
        std::size_t idx = 0u;
        for (idx = 0u; x[idx] != null_char_()[0] && y[idx] != null_char_()[0]; idx++)
            if (!traits_type::eq(x[idx], y[idx]))
                return false;
        return x[idx] == y[idx];
    }
    friend BOOST_CXX14_CONSTEXPR bool operator!=(basic_cstring_view x, basic_cstring_view y) BOOST_NOEXCEPT
    {
        std::size_t idx = 0u;
        for (idx = 0u; x[idx] != null_char_()[0] &&
                                   y[idx] != null_char_()[0]; idx++)
            if (!traits_type::eq(x[idx], y[idx]))
                return true;
        return x[idx] != y[idx];
    }
    friend BOOST_CXX14_CONSTEXPR bool operator< (basic_cstring_view x, basic_cstring_view y) BOOST_NOEXCEPT {return x.compare(y) <  0;}
    friend BOOST_CXX14_CONSTEXPR bool operator> (basic_cstring_view x, basic_cstring_view y) BOOST_NOEXCEPT {return x.compare(y) >  0;}
    friend BOOST_CXX14_CONSTEXPR bool operator<=(basic_cstring_view x, basic_cstring_view y) BOOST_NOEXCEPT {return x.compare(y) <= 0;}
    friend BOOST_CXX14_CONSTEXPR bool operator>=(basic_cstring_view x, basic_cstring_view y) BOOST_NOEXCEPT {return x.compare(y) >= 0;}

    // modifiers
    void clear() BOOST_NOEXCEPT { view_ =  null_char_(); }          // Boost extension

    std::basic_string<value_type, traits_type> to_string() const {
        return std::basic_string<CharT, Traits>(begin(), end());
    }

    template<typename Allocator>
    std::basic_string<value_type, traits_type, Allocator> to_string(const Allocator& a) const {
        return std::basic_string<value_type, traits_type, Allocator>(begin(), end(), a);
    }

  private:
    BOOST_CONSTEXPR static const_pointer   null_char_()         {return detail::null_char_(CharT{});}
    const_pointer view_;
};

using cstring_view    = basic_cstring_view<char>;
using wcstring_view   = basic_cstring_view<wchar_t>;

#if !defined(BOOST_NO_CXX11_CHAR16_T)
using u16cstring_view = basic_cstring_view<char16_t>;
#endif

#if !defined(BOOST_NO_CXX11_CHAR32_T)
using u32cstring_view = basic_cstring_view<char32_t>;
#endif

#if defined(__cpp_char8_t) && __cpp_char8_t >= 201811L
using u8cstring_view  = basic_cstring_view<char8_t>;
#endif

    // Forward declaration of Boost.ContainerHash function
    template <class It> std::size_t hash_range(It, It);

    template <class value_type, class traits>
    std::size_t hash_value(basic_cstring_view<value_type, traits> s) {
        return boost::hash_range(s.begin(), s.end());
    }

}


#endif //BOOST_UTILITY_CSTRING_VIEW_HPP