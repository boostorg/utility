/*
   Copyright (c) Marshall Clow 2012-2012.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#include <iostream>
#include <algorithm>
#include <string>

#include <boost/utility/cstring_view.hpp>
#include <boost/container_hash/hash.hpp>

#include <boost/core/lightweight_test.hpp>

typedef boost::cstring_view cstring_view;

//  Should be equal
void interop ( const std::string &str, cstring_view ref ) {
//  BOOST_TEST ( str == ref );
    BOOST_TEST ( str.size () == ref.size ());
    BOOST_TEST ( std::equal ( str.begin (),  str.end (),  ref.begin ()));
    BOOST_TEST ( std::equal ( str.rbegin (), str.rend (), ref.rbegin ()));
    }

void null_tests ( const char *p ) {
//  All zero-length string-refs should be equal
    cstring_view sr1; // NULL, 0
    cstring_view sr2 ( "" );
    cstring_view sr3 ( p);
    sr3.remove_prefix(std::strlen(p));
    cstring_view sr4 ( p );
    sr4.clear ();

    BOOST_TEST ( sr1 == sr2 );
    BOOST_TEST ( sr1 == sr3 );
    BOOST_TEST ( sr2 == sr3 );
    BOOST_TEST ( sr1 == sr4 );
    }

//  make sure that substrings work just like strings
void test_substr ( const std::string &str ) {
    const size_t sz = str.size ();
    cstring_view ref ( str );

//  Substrings at the end
    for ( size_t i = 0; i <= sz; ++ i )
        interop ( str.substr ( i ), ref.substr ( i ));

//  Substrings at the beginning
    for ( size_t i = 0; i <= sz; ++ i )
        BOOST_TEST ( str.substr ( 0, i )
        == ref.substr ( 0, i ));

//  All possible substrings
    for ( size_t i = 0; i < sz; ++i )
        for ( size_t j = i; j < sz; ++j )
        {
            std::string l{str.substr ( i, j )},
                        r{ref.substr ( i, j )};
            BOOST_TEST ( str.substr ( i, j ) == ref.substr ( i, j ));
        }

    }

//  make sure that removing prefixes and suffixes work just like strings
void test_remove ( const std::string &str ) {
    const size_t sz = str.size ();
    std::string work;
    cstring_view ref;

    for ( size_t i = 1; i <= sz; ++i ) {
      work = str;
      ref  = str;
      while ( ref.size () >= i ) {
          interop ( work, ref );
          work.erase ( 0, i );
          ref.remove_prefix (i);
          }
      }

    }

void test_hash(const std::string& str) {
    cstring_view ref = str;
    BOOST_TEST(boost::hash_value(ref) == boost::hash_value(str));
    boost::hash<std::string> hstr;
    boost::hash<cstring_view> hsv;
    BOOST_TEST(hsv(ref) == hstr(str));
    }

const char *test_strings [] = {
    "",
    "1",
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
    "0123456789",
    NULL
    };

int main()
{
    const char **p = &test_strings[0];

    while ( *p != NULL ) {
        interop ( *p, *p );
        test_substr ( *p );
        test_remove ( *p );
        null_tests ( *p );
        test_hash( *p );

        p++;
        }

    return boost::report_errors();
}
