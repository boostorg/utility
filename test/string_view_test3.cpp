/*
   © Copyright Beman Dawes 2015

   Distributed under the Boost Software License, Version 1.0
   See http://www.boost.org/LICENSE_1_0.txt

   For more information, see http://www.boost.org
*/

#include <boost/utility/string_view.hpp>

#include <iostream>
#include <algorithm>
#include <string>

#define BOOST_LIGHTWEIGHT_TEST_OSTREAM std::cout
#include <boost/core/lightweight_test.hpp>
#include <boost/detail/lightweight_main.hpp>

namespace
{
  void find_test()
  {
    //  find, two modified and two new signatures
    std::string s1("ababcab");
    boost::string_view sv1(s1);
    std::string s2("abc");
    boost::string_view sv2(s2);

    BOOST_TEST_EQ(sv1.find(sv2), 2);
    BOOST_TEST_EQ(sv1.find(sv2, 0), 2);
    BOOST_TEST_EQ(sv1.find(sv2, 1), 2);
    BOOST_TEST_EQ(sv1.find(sv2, 2), 2);
    BOOST_TEST_EQ(sv1.find(sv2, 3), boost::string_view::npos);
    BOOST_TEST_EQ(sv1.find(sv2, 4), boost::string_view::npos);
    BOOST_TEST_EQ(sv1.find(sv2, 5), boost::string_view::npos);
    BOOST_TEST_EQ(sv1.find(sv2, 6), boost::string_view::npos);
    BOOST_TEST_EQ(sv1.find(sv2, 7), boost::string_view::npos);
    BOOST_TEST_EQ(sv1.find(sv2, 8), boost::string_view::npos);

    BOOST_TEST_EQ(sv1.find('b'), 1);
    BOOST_TEST_EQ(sv1.find('b', 0), 1);
    BOOST_TEST_EQ(sv1.find('b', 1), 1);
    BOOST_TEST_EQ(sv1.find('b', 2), 3);
    BOOST_TEST_EQ(sv1.find('b', 3), 3);
    BOOST_TEST_EQ(sv1.find('b', 4), 6);
    BOOST_TEST_EQ(sv1.find('b', 5), 6);
    BOOST_TEST_EQ(sv1.find('b', 6), 6);
    BOOST_TEST_EQ(sv1.find('b', 7), boost::string_view::npos);
    BOOST_TEST_EQ(sv1.find('b', 8), boost::string_view::npos);

    BOOST_TEST_EQ(sv1.find("abc", 0, 2), 0);
    BOOST_TEST_EQ(sv1.find("abc", 1, 2), 2);
    BOOST_TEST_EQ(sv1.find("abc", 2, 2), 2);
    BOOST_TEST_EQ(sv1.find("abc", 3, 2), 5);
    BOOST_TEST_EQ(sv1.find("abc", 4, 2), 5);
    BOOST_TEST_EQ(sv1.find("abc", 5, 2), 5);
    BOOST_TEST_EQ(sv1.find("abc", 6, 2), boost::string_view::npos);
    BOOST_TEST_EQ(sv1.find("abc", 7, 2), boost::string_view::npos);
    BOOST_TEST_EQ(sv1.find("abc", 8, 2), boost::string_view::npos);

    BOOST_TEST_EQ(sv1.find("abc"), 2);
    BOOST_TEST_EQ(sv1.find("abc", 0), 2);
    BOOST_TEST_EQ(sv1.find("abc", 1), 2);
    BOOST_TEST_EQ(sv1.find("abc", 2), 2);
    BOOST_TEST_EQ(sv1.find("abc", 3), boost::string_view::npos);
    BOOST_TEST_EQ(sv1.find("abc", 4), boost::string_view::npos);
    BOOST_TEST_EQ(sv1.find("abc", 5), boost::string_view::npos);
    BOOST_TEST_EQ(sv1.find("abc", 6), boost::string_view::npos);
    BOOST_TEST_EQ(sv1.find("abc", 7), boost::string_view::npos);
    BOOST_TEST_EQ(sv1.find("abc", 8), boost::string_view::npos);
  }

}  // unnamed namespace

int cpp_main(int argc, char* argv[])
{
  typedef boost::string_view::traits_type string_traits;
  typedef boost::string_view::const_pointer const_pointer;

  //  swap
  //  copy
  //  compare, five new signatures

  find_test();
  
  return boost::report_errors();
}
