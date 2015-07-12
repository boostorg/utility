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
    //  find - test two modified and two new signatures
    std::string s1("ababcab");
    boost::string_view sv1(s1);
    std::string s2("abc");
    boost::string_view sv2(s2);
    std::string s3;
    boost::string_view sv3(s3);

    //  first signature
    BOOST_TEST_EQ(sv3.find(sv3), s3.find(s3));
    BOOST_TEST_EQ(sv2.find(sv3), s2.find(s3));
    BOOST_TEST_EQ(sv3.find(sv2), s3.find(s2));
    BOOST_TEST_EQ(sv1.find(sv3, 7), s1.find(s3, 7));
    BOOST_TEST_EQ(sv1.find(sv3, 5), s1.find(s3, 5));

    BOOST_TEST_EQ(sv1.find(sv2), s1.find(s2));
    for (std::string::size_type i = 0; i <= 8; ++i)
    {
      //std::cout << i << std::endl;
      BOOST_TEST_EQ(sv1.find(sv2, i), s1.find(s2, i));
    }
    for (std::string::size_type i = 0; i <= 8; ++i)
    {
      //std::cout << i << std::endl;
      BOOST_TEST_EQ(sv1.find(sv3, i), s1.find(s3, i));
    }

    //  second signature
    for (std::string::size_type i = 0; i <= 8; ++i)
    {
      //std::cout << i << std::endl;
      BOOST_TEST_EQ(sv1.find('b', i), s1.find('b', i));
    }

    //  third signature
    for (std::string::size_type i = 0; i <= 8; ++i)
      for (std::string::size_type j = 0; j <= 4; ++j)
      {
        //std::cout << i << " " << j << std::endl;
        BOOST_TEST_EQ(sv1.find("abc", i, j), s1.find("abc", i, j));
      }

    //  fourth signature
    BOOST_TEST_EQ(sv1.find("abc"), s1.find("abc"));
    for (std::string::size_type i = 0; i <= 8; ++i)
      BOOST_TEST_EQ(sv1.find("abc", i), s1.find("abc", i));
  }

  void rfind_test()
  {
    //  rfind - test two modified and two new signatures
    std::string s1("ababcab");
    boost::string_view sv1(s1);
    std::string s2("ab");
    boost::string_view sv2(s2);
    std::string s3;
    boost::string_view sv3(s3);

     //  first signature
    BOOST_TEST_EQ(sv3.rfind(sv3), s3.rfind(s3));
    BOOST_TEST_EQ(sv2.rfind(sv3), s2.rfind(s3));
    BOOST_TEST_EQ(sv3.rfind(sv2), s3.rfind(s2));
    BOOST_TEST_EQ(sv1.rfind(sv3, 7), s1.rfind(s3, 7));
    BOOST_TEST_EQ(sv1.rfind(sv3, 5), s1.rfind(s3, 5));

    BOOST_TEST_EQ(sv1.rfind(sv2), s1.rfind(s2));
    for (std::string::size_type i = s1.size(); i <= s1.size(); --i)
    {
      // std::cout << i << ": " << sv1.rfind(sv2, i) << " " << s1.rfind(s2, i) << std::endl;
      BOOST_TEST_EQ(sv1.rfind(sv2, i), s1.rfind(s2, i));
    }
    for (std::string::size_type i = 0; i <= 8; ++i)
    {
      //std::cout << i << std::endl;
      BOOST_TEST_EQ(sv1.rfind(sv3, i), s1.rfind(s3, i));
    }

    //  second signature
    for (std::string::size_type i = 0; i <= 8; ++i)
    {
      //std::cout << i << std::endl;
      BOOST_TEST_EQ(sv1.rfind('b', i), s1.rfind('b', i));
    }

    //  third signature
    for (std::string::size_type i = 0; i <= 8; ++i)
      for (std::string::size_type j = 0; j <= 4; ++j)
      {
        //std::cout << i << " " << j << std::endl;
        BOOST_TEST_EQ(sv1.rfind("abc", i, j), s1.rfind("abc", i, j));
      }

    //  fourth signature
    BOOST_TEST_EQ(sv1.rfind("abc"), s1.rfind("abc"));
    for (std::string::size_type i = 0; i <= 8; ++i)
      BOOST_TEST_EQ(sv1.rfind("abc", i), s1.rfind("abc", i));
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
  rfind_test();
  
  return boost::report_errors();
}
