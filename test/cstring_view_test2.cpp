/*
   Copyright (c) Marshall Clow 2012-2012.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#include <new>        // for placement new
#include <iostream>
#include <cstddef>    // for NULL, std::size_t, std::ptrdiff_t
#include <cstring>    // for std::strchr and std::strcmp
#include <cstdlib>    // for std::malloc and std::free

#include <boost/utility/cstring_view.hpp>
#include <boost/config.hpp>

#include <boost/core/lightweight_test.hpp>

typedef boost::cstring_view cstring_view;

void starts_with ( const char *arg ) {
    const size_t sz = std::strlen ( arg );
    cstring_view sr  ( arg );
    cstring_view sr2 ( arg );
    const char *p = arg + std::strlen ( arg ) - 1;
    while ( p >= arg ) {
        std::string foo ( arg, p + 1 );
        BOOST_TEST ( sr.starts_with ( foo ));
        --p;
        }

    char ch = *arg;
    sr2 = arg;
    if ( sz > 0 )
      BOOST_TEST ( sr2.starts_with ( ch ));
    BOOST_TEST ( !sr2.starts_with ( ++ch ));
    BOOST_TEST ( sr2.starts_with ( cstring_view ()));
    }

void reverse ( const char *arg ) {
//  Round trip
    cstring_view sr1 ( arg );
    std::string string1 ( sr1.rbegin (), sr1.rend ());
    cstring_view sr2 ( string1 );
    std::string string2 ( sr2.rbegin (), sr2.rend ());

    BOOST_TEST ( std::equal ( sr2.rbegin (), sr2.rend (), arg ));
    BOOST_TEST ( string2 == arg );
    BOOST_TEST ( std::equal ( sr1.begin (), sr1.end (), string2.begin ()));
    }

template <typename T>
class custom_allocator {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef void* void_pointer;
    typedef const void* const_void_pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef T& reference;
    typedef const T& const_reference;

    template<class U>
    struct rebind {
        typedef custom_allocator<U> other;
        };

    custom_allocator() BOOST_NOEXCEPT {}
    template <typename U>
    custom_allocator(custom_allocator<U> const&) BOOST_NOEXCEPT {}

    pointer allocate(size_type n) const {
        return static_cast<pointer>(std::malloc(sizeof(value_type) * n));
        }
    void deallocate(pointer p, size_type) const BOOST_NOEXCEPT {
        std::free(p);
        }

    pointer address(reference value) const BOOST_NOEXCEPT {
        return &value;
        }

    const_pointer address(const_reference value) const BOOST_NOEXCEPT {
        return &value;
        }

    BOOST_CONSTEXPR size_type max_size() const BOOST_NOEXCEPT {
        return (~(size_type)0u) / sizeof(value_type);
        }

#if !defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    template <class U, class... Args>
    void construct(U* ptr, Args&&... args) const {
        ::new((void*)ptr) U(static_cast<Args&&>(args)...);
        }
#else
    template <class U, class V>
    void construct(U* ptr, V&& value) const {
        ::new((void*)ptr) U(static_cast<V&&>(value));
        }
#endif
#else
    template <class U, class V>
    void construct(U* ptr, const V& value) const {
        ::new((void*)ptr) U(value);
        }
#endif

    template <class U>
    void construct(U* ptr) const {
        ::new((void*)ptr) U();
        }

    template <class U>
    void destroy(U* ptr) const {
        (void)ptr;
        ptr->~U();
        }
    };

template <typename T, typename U>
BOOST_CONSTEXPR bool operator==(const custom_allocator<T> &, const custom_allocator<U> &) BOOST_NOEXCEPT {
    return true;
    }
template <typename T, typename U>
BOOST_CONSTEXPR bool operator!=(const custom_allocator<T> &, const custom_allocator<U> &) BOOST_NOEXCEPT {
    return false;
    }

void to_string ( const char *arg ) {
    cstring_view sr1;
    std::string str1;
    std::string str2;

    str1.assign ( arg );
    sr1 = arg;
//  str2 = sr1.to_string<std::allocator<char> > ();
    str2 = sr1.to_string ();
    BOOST_TEST ( str1 == str2 );

    std::basic_string<char, std::char_traits<char>, custom_allocator<char> > str3 = sr1.to_string(custom_allocator<char>());
    BOOST_TEST ( std::strcmp(str1.c_str(), str3.c_str()) == 0 );

    }

void compare ( const char *arg ) {
    cstring_view sr1;
    std::string str1;
    std::string str2 = str1;

    str1.assign ( arg );
    sr1 = arg;
    BOOST_TEST ( sr1  == sr1);    // compare cstring_view and cstring_view
    BOOST_TEST ( sr1  == str1);   // compare string and cstring_view
    BOOST_TEST ( str1 == sr1 );   // compare cstring_view and string
    BOOST_TEST ( sr1  == arg );   // compare cstring_view and pointer
    BOOST_TEST ( arg  == sr1 );   // compare pointer and cstring_view

    if ( sr1.size () > 0 ) {
        (*str1.rbegin())++;
        BOOST_TEST ( sr1  != str1 );
        BOOST_TEST ( str1 != sr1 );
        BOOST_TEST ( sr1 < str1 );
        BOOST_TEST ( sr1 <= str1 );
        BOOST_TEST ( str1 > sr1 );
        BOOST_TEST ( str1 >= sr1 );

        (*str1.rbegin()) -= 2;
        BOOST_TEST ( sr1  != str1 );
        BOOST_TEST ( str1 != sr1 );
        BOOST_TEST ( sr1 > str1 );
        BOOST_TEST ( sr1 >= str1 );
        BOOST_TEST ( str1 < sr1 );
        BOOST_TEST ( str1 <= sr1 );
        }
    }

const char *test_strings [] = {
    "",
    "0",
    "abc",
    "AAA",  // all the same
    "adsfadadiaef;alkdg;aljt;j agl;sjrl;tjs;lga;lretj;srg[w349u5209dsfadfasdfasdfadsf",
    "abc\0asdfadsfasf",
    NULL
    };

int main()
{
    const char **p = &test_strings[0];

    while ( *p != NULL ) {
        starts_with ( *p );
        reverse ( *p );
        to_string ( *p );
        compare ( *p );

        p++;
        }

    return boost::report_errors();
}
