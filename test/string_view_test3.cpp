/*
   © Copyright Beman Dawes 2015

   Distributed under the Boost Software License, Version 1.0
   See http://www.boost.org/LICENSE_1_0.txt

   For more information, see http://www.boost.org
*/

#ifndef _SCL_SECURE_NO_WARNINGS
# define _SCL_SECURE_NO_WARNINGS
#endif

#include <boost/utility/string_view.hpp>

#include <iostream>
#include <algorithm>
#include <string>

#define BOOST_LIGHTWEIGHT_TEST_OSTREAM std::cout
#include <boost/core/lightweight_test.hpp>
#include <boost/detail/lightweight_main.hpp>

namespace
{
  void swap_test()
  {
    std::cout << "swap test..." << std::endl;

    std::string s1("abcdefg");
    boost::string_view sv1(s1);
    std::string s2("xyz");
    boost::string_view sv2(s2);
    std::string s3;
    boost::string_view sv3(s3);

    boost::string_view sv1_org(sv1);
    boost::string_view sv2_org(sv2);
    boost::string_view sv3_org(sv3);

    sv1.swap(sv2);
    BOOST_TEST_EQ(sv1.data(), sv2_org.data());
    BOOST_TEST_EQ(sv2.data(), sv1_org.data());
    BOOST_TEST_EQ(sv1.size(), sv2_org.size());
    BOOST_TEST_EQ(sv2.size(), sv1_org.size());
    sv1.swap(sv2);
    BOOST_TEST_EQ(sv1.data(), sv1_org.data());
    BOOST_TEST_EQ(sv2.data(), sv2_org.data());
    BOOST_TEST_EQ(sv1.size(), sv1_org.size());
    BOOST_TEST_EQ(sv2.size(), sv2_org.size());

    sv3.swap(sv1);
    BOOST_TEST_EQ(sv3.data(), sv1_org.data());
    BOOST_TEST_EQ(sv1.data(), sv3_org.data());
    BOOST_TEST_EQ(sv3.size(), sv1_org.size());
    BOOST_TEST_EQ(sv1.size(), sv3_org.size());
    sv3.swap(sv1);
    BOOST_TEST_EQ(sv1.data(), sv1_org.data());
    BOOST_TEST_EQ(sv3.data(), sv3_org.data());
    BOOST_TEST_EQ(sv1.size(), sv1_org.size());
    BOOST_TEST_EQ(sv3.size(), sv3_org.size());
  }

  void copy_test()
  {
    std::cout << "copy test..." << std::endl;

    std::string s1("abcdefg");
    boost::string_view sv1(s1);
    const std::size_t sz = 128u;
    char a[sz];

    std::fill(a, a + sz, 'x');
    sv1.copy(a, sv1.size());
    BOOST_TEST(std::memcmp(sv1.data(), &a, sv1.size()) == 0);
    const char* p;
    for (p = a + sv1.size();
      p != a + sz && *p == 'x'; ++p) {}
    BOOST_TEST(p == a + sz);

    std::fill(a, a + sz, 'x');
    sv1.copy(a, sv1.size()-2, 2);
    BOOST_TEST(std::memcmp(sv1.data()+2, &a, sv1.size()-2) == 0);
    for (p = a + sv1.size() - 2;
      p != a + sz && *p == 'x'; ++p) {}
    BOOST_TEST(p == a + sz);
  }

  void compare_test()
  {
    std::cout << "compare test..." << std::endl;

  }

  void find_test()
  {
    std::cout << "find test..." << std::endl;

    //  find - test two modified and two new signatures
    std::string s1("ababcab");
    boost::string_view sv1(s1);
    std::string s2("abc");
    boost::string_view sv2(s2);
    std::string s3;
    boost::string_view sv3(s3);

    //  first signature
    BOOST_TEST_EQ(sv3.find(sv3), s3.find(s3));            // both strings empty
    BOOST_TEST_EQ(sv1.find(sv3), s1.find(s3));            // search string empty
    BOOST_TEST_EQ(sv3.find(sv2), s3.find(s2));            // searched string empty
    BOOST_TEST_EQ(sv1.find(sv3, sizeof(s1) + 2), s1.find(s3, sizeof(s1) + 2));
    BOOST_TEST_EQ(sv1.find(sv3, sizeof(s1) + 1), s1.find(s3, sizeof(s1) + 1));
    BOOST_TEST_EQ(sv1.find(sv3, sizeof(s1)), s1.find(s3, sizeof(s1)));
    BOOST_TEST_EQ(sv1.find(sv3, sizeof(s1)-  1), s1.find(s3, sizeof(s1)-  1));

    BOOST_TEST_EQ(sv1.find(sv2), s1.find(s2));
    for (std::string::size_type i = 0; i <= sizeof(s1) + 1; ++i)
    {
      //std::cout << i << std::endl;
      BOOST_TEST_EQ(sv1.find(sv2, i), s1.find(s2, i));
    }
    for (std::string::size_type i = 0; i <= sizeof(s1) + 1; ++i)
    {
      //std::cout << i << std::endl;
      BOOST_TEST_EQ(sv1.find(sv3, i), s1.find(s3, i));
    }

    //  second signature
    for (std::string::size_type i = 0; i <= sizeof(s1) + 1; ++i)
    {
      //std::cout << i << std::endl;
      BOOST_TEST_EQ(sv1.find('b', i), s1.find('b', i));
    }

    //  third signature
    for (std::string::size_type i = 0; i <= sizeof(s1) + 1; ++i)
      for (std::string::size_type j = 0; j <= 4; ++j)
      {
        //std::cout << i << " " << j << std::endl;
        BOOST_TEST_EQ(sv1.find("abc", i, j), s1.find("abc", i, j));
      }

    //  fourth signature
    BOOST_TEST_EQ(sv1.find("abc"), s1.find("abc"));
    for (std::string::size_type i = 0; i <= sizeof(s1) + 1; ++i)
      BOOST_TEST_EQ(sv1.find("abc", i), s1.find("abc", i));
  }

  void rfind_test()
  {
    std::cout << "rfind test..." << std::endl;

    //  rfind - test two modified and two new signatures
    std::string s1("ababcab");
    boost::string_view sv1(s1);
    std::string s2("ab");
    boost::string_view sv2(s2);
    std::string s3;
    boost::string_view sv3(s3);

     //  first signature
    BOOST_TEST_EQ(sv3.rfind(sv3), s3.rfind(s3));          // both strings empty
    BOOST_TEST_EQ(sv1.rfind(sv3), s1.rfind(s3));          // search string empty
    BOOST_TEST_EQ(sv3.rfind(sv2), s3.rfind(s2));          // searched string empty
    BOOST_TEST_EQ(sv1.find(sv3, s1.size() + 2), s1.find(s3, s1.size() + 2));
    BOOST_TEST_EQ(sv1.find(sv3, s1.size() + 1), s1.find(s3, s1.size() + 1));
    BOOST_TEST_EQ(sv1.rfind(sv3, s1.size()), s1.rfind(s3, s1.size()));
    BOOST_TEST_EQ(sv1.rfind(sv3, s1.size()-  1), s1.rfind(s3, s1.size()-  1));

    BOOST_TEST_EQ(sv1.rfind(sv2), s1.rfind(s2));
    for (std::string::size_type i = s1.size(); i <= s1.size(); --i)
    {
      // std::cout << i << ": " << sv1.rfind(sv2, i) << " " << s1.rfind(s2, i) << std::endl;
      BOOST_TEST_EQ(sv1.rfind(sv2, i), s1.rfind(s2, i));
    }
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
    {
      //std::cout << i << std::endl;
      BOOST_TEST_EQ(sv1.rfind(sv3, i), s1.rfind(s3, i));
    }

    //  second signature
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
    {
      //std::cout << i << std::endl;
      BOOST_TEST_EQ(sv1.rfind('b', i), s1.rfind('b', i));
    }

    //  third signature
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
      for (std::string::size_type j = 0; j <= 4; ++j)
      {
        //std::cout << i << " " << j << std::endl;
        BOOST_TEST_EQ(sv1.rfind("abc", i, j), s1.rfind("abc", i, j));
      }

    //  fourth signature
    BOOST_TEST_EQ(sv1.rfind("abc"), s1.rfind("abc"));
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
      BOOST_TEST_EQ(sv1.rfind("abc", i), s1.rfind("abc", i));
  }

  void find_first_of_test()
  {
    std::cout << "find_first_of test..." << std::endl;

    //  find_first_of - test two modified and two new signatures
    std::string s1("Hello World!");
    boost::string_view sv1(s1);
    std::string s2("o");
    boost::string_view sv2(s2);
    const char* s2c = "Good Bye";
    boost::string_view sv2c(s2c);
    std::string s3;
    boost::string_view sv3(s3);

    //  smoke test
    BOOST_TEST_EQ(sv1.find_first_of(sv2), s1.find_first_of(s2));
    BOOST_TEST_EQ(sv1.find_first_of(sv2, 5), s1.find_first_of(s2, 5));
    BOOST_TEST_EQ(sv1.find_first_of(s2c), s1.find_first_of(s2c));
    BOOST_TEST_EQ(sv1.find_first_of(s2c, 0, 4), s1.find_first_of(s2c, 0, 4));

    //  first signature
    BOOST_TEST_EQ(sv3.find_first_of(sv3), s3.find_first_of(s3));  // both strings empty
    BOOST_TEST_EQ(sv1.find_first_of(sv3), s1.find_first_of(s3));  // search string empty
    BOOST_TEST_EQ(sv3.find_first_of(sv2), s3.find_first_of(s2));  // searched string empty
    BOOST_TEST_EQ(sv1.find_first_of(sv3, s1.size() + 2), s1.find_first_of(s3, s1.size() + 2));
    BOOST_TEST_EQ(sv1.find_first_of(sv3, s1.size() + 1), s1.find_first_of(s3, s1.size() + 1));
    BOOST_TEST_EQ(sv1.find_first_of(sv3, s1.size()), s1.find_first_of(s3, s1.size()));
    BOOST_TEST_EQ(sv1.find_first_of(sv3, s1.size() -  1), s1.find_first_of(s3, s1.size() -  1));

    BOOST_TEST_EQ(sv1.find_first_of(sv2), s1.find_first_of(s2));
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
    {
      //std::cout << i << ": " << sv1.find_first_of(sv2, i) << " " << s1.find_first_of(s2, i) << std::endl;
      BOOST_TEST_EQ(sv1.find_first_of(sv2, i), s1.find_first_of(s2, i));
    }
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
    {
      //std::cout << i << ": " << sv1.find_first_of(sv3, i) << " " << s1.find_first_of(s3, i) << std::endl;
      BOOST_TEST_EQ(sv1.find_first_of(sv3, i), s1.find_first_of(s3, i));
    }

    //  second signature
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
    {
      //std::cout << i << ": " << sv1.find_first_of('o', i) << " " << s1.find_first_of('o', i) << std::endl;
      BOOST_TEST_EQ(sv1.find_first_of('o', i), s1.find_first_of('o', i));
    }

    //  third signature
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
      for (std::string::size_type j = 0; j <= std::strlen(s2c) + 1; ++j)
      {
        //std::cout << i << "," << j << ": " << sv1.find_first_of(s2c, i, j) << " " << s1.find_first_of(s2c, i, j) << std::endl;
        BOOST_TEST_EQ(sv1.find_first_of(s2c, i, j), s1.find_first_of(s2c, i, j));
      }

    //  fourth signature
    BOOST_TEST_EQ(sv1.find_first_of(s2c), s1.find_first_of(s2c));
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
    {
      //std::cout << i << ": " << sv1.find_first_of(s2c, i) << " " << s1.find_first_of(s2c, i) << std::endl;
      BOOST_TEST_EQ(sv1.find_first_of(s2c, i), s1.find_first_of(s2c, i));  
    }
  }

  void find_last_of_test()
  {
    std::cout << "find_last_of test..." << std::endl;

    //  find_last_of - test two modified and two new signatures
    std::string s1("Hello World!");
    boost::string_view sv1(s1);
    std::string s2("o");
    boost::string_view sv2(s2);
    const char* s2c = "Good Bye";
    boost::string_view sv2c(s2c);
    std::string s3;
    boost::string_view sv3(s3);

    //  smoke test
    BOOST_TEST_EQ(sv1.find_last_of(sv2), s1.find_last_of(s2));
    BOOST_TEST_EQ(sv1.find_last_of(sv2, 5), s1.find_last_of(s2, 5));
    BOOST_TEST_EQ(sv1.find_last_of(s2c), s1.find_last_of(s2c));
    BOOST_TEST_EQ(sv1.find_last_of(s2c, 0, 4), s1.find_last_of(s2c, 0, 4));

    //  first signature
    BOOST_TEST_EQ(sv3.find_last_of(sv3), s3.find_last_of(s3));  // both strings empty
    BOOST_TEST_EQ(sv1.find_last_of(sv3), s1.find_last_of(s3));  // search string empty
    BOOST_TEST_EQ(sv3.find_last_of(sv2), s3.find_last_of(s2));  // searched string empty
    BOOST_TEST_EQ(sv1.find_last_of(sv3, s1.size() + 2), s1.find_last_of(s3, s1.size() + 2));
    BOOST_TEST_EQ(sv1.find_last_of(sv3, s1.size() + 1), s1.find_last_of(s3, s1.size() + 1));
    BOOST_TEST_EQ(sv1.find_last_of(sv3, s1.size()), s1.find_last_of(s3, s1.size()));
    BOOST_TEST_EQ(sv1.find_last_of(sv3, s1.size() -  1), s1.find_last_of(s3, s1.size() -  1));

    BOOST_TEST_EQ(sv1.find_last_of(sv2), s1.find_last_of(s2));
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
    {
      //std::cout << i << ": " << sv1.find_last_of(sv2, i) << " " << s1.find_last_of(s2, i) << std::endl;
      BOOST_TEST_EQ(sv1.find_last_of(sv2, i), s1.find_last_of(s2, i));
    }
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
    {
      //std::cout << i << ": " << sv1.find_last_of(sv3, i) << " " << s1.find_last_of(s3, i) << std::endl;
      BOOST_TEST_EQ(sv1.find_last_of(sv3, i), s1.find_last_of(s3, i));
    }

    //  second signature
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
    {
      //std::cout << i << ": " << sv1.find_last_of('o', i) << " " << s1.find_last_of('o', i) << std::endl;
      BOOST_TEST_EQ(sv1.find_last_of('o', i), s1.find_last_of('o', i));
    }

    //  third signature
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
      for (std::string::size_type j = 0; j <= std::strlen(s2c) + 1; ++j)
      {
        //std::cout << i << "," << j << ": " << sv1.find_last_of(s2c, i, j) << " " << s1.find_last_of(s2c, i, j) << std::endl;
        BOOST_TEST_EQ(sv1.find_last_of(s2c, i, j), s1.find_last_of(s2c, i, j));
      }

    //  fourth signature
    BOOST_TEST_EQ(sv1.find_last_of(s2c), s1.find_last_of(s2c));
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
    {
      //std::cout << i << ": " << sv1.find_last_of(s2c, i) << " " << s1.find_last_of(s2c, i) << std::endl;
      BOOST_TEST_EQ(sv1.find_last_of(s2c, i), s1.find_last_of(s2c, i));  
    }
  }

  void find_first_not_of_test()
  {
    std::cout << "find_first_not_of test..." << std::endl;

    //  find_first_not_of - test two modified and two new signatures
    std::string s1("Hello World!");
    boost::string_view sv1(s1);
    std::string s2("o");
    boost::string_view sv2(s2);
    const char* s2c = "Good Bye";
    boost::string_view sv2c(s2c);
    std::string s3;
    boost::string_view sv3(s3);

    //  smoke test
    BOOST_TEST_EQ(sv1.find_first_not_of(sv2), s1.find_first_not_of(s2));
    BOOST_TEST_EQ(sv1.find_first_not_of(sv2, 5), s1.find_first_not_of(s2, 5));
    BOOST_TEST_EQ(sv1.find_first_not_of(s2c), s1.find_first_not_of(s2c));
    BOOST_TEST_EQ(sv1.find_first_not_of(s2c, 0, 4), s1.find_first_not_of(s2c, 0, 4));

    //  first signature
    BOOST_TEST_EQ(sv3.find_first_not_of(sv3), s3.find_first_not_of(s3));  // both strings empty
    BOOST_TEST_EQ(sv1.find_first_not_of(sv3), s1.find_first_not_of(s3));  // search string empty
    BOOST_TEST_EQ(sv3.find_first_not_of(sv2), s3.find_first_not_of(s2));  // searched string empty
    BOOST_TEST_EQ(sv1.find_first_not_of(sv3, s1.size() + 2), s1.find_first_not_of(s3, s1.size() + 2));
    BOOST_TEST_EQ(sv1.find_first_not_of(sv3, s1.size() + 1), s1.find_first_not_of(s3, s1.size() + 1));
    BOOST_TEST_EQ(sv1.find_first_not_of(sv3, s1.size()), s1.find_first_not_of(s3, s1.size()));
    BOOST_TEST_EQ(sv1.find_first_not_of(sv3, s1.size() -  1), s1.find_first_not_of(s3, s1.size() -  1));

    BOOST_TEST_EQ(sv1.find_first_not_of(sv2), s1.find_first_not_of(s2));
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
    {
      //std::cout << i << ": " << sv1.find_first_not_of(sv2, i) << " " << s1.find_first_not_of(s2, i) << std::endl;
      BOOST_TEST_EQ(sv1.find_first_not_of(sv2, i), s1.find_first_not_of(s2, i));
    }
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
    {
      //std::cout << i << ": " << sv1.find_first_not_of(sv3, i) << " " << s1.find_first_not_of(s3, i) << std::endl;
      BOOST_TEST_EQ(sv1.find_first_not_of(sv3, i), s1.find_first_not_of(s3, i));
    }

    //  second signature
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
    {
      //std::cout << i << ": " << sv1.find_first_not_of('o', i) << " " << s1.find_first_not_of('o', i) << std::endl;
      BOOST_TEST_EQ(sv1.find_first_not_of('o', i), s1.find_first_not_of('o', i));
    }

    //  third signature
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
      for (std::string::size_type j = 0; j <= std::strlen(s2c) + 1; ++j)
      {
        //std::cout << i << "," << j << ": " << sv1.find_first_not_of(s2c, i, j) << " " << s1.find_first_not_of(s2c, i, j) << std::endl;
        BOOST_TEST_EQ(sv1.find_first_not_of(s2c, i, j), s1.find_first_not_of(s2c, i, j));
      }

    //  fourth signature
    BOOST_TEST_EQ(sv1.find_first_not_of(s2c), s1.find_first_not_of(s2c));
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
    {
      //std::cout << i << ": " << sv1.find_first_not_of(s2c, i) << " " << s1.find_first_not_of(s2c, i) << std::endl;
      BOOST_TEST_EQ(sv1.find_first_not_of(s2c, i), s1.find_first_not_of(s2c, i));  
    }
  }

  void find_last_not_of_test()
  {
    std::cout << "find_last_not_of test..." << std::endl;

    //  find_last_not_of - test two modified and two new signatures
    std::string s1("Hello World!");
    boost::string_view sv1(s1);
    std::string s2("o");
    boost::string_view sv2(s2);
    const char* s2c = "Good Bye";
    boost::string_view sv2c(s2c);
    std::string s3;
    boost::string_view sv3(s3);

    //  smoke test
    BOOST_TEST_EQ(sv1.find_last_not_of(sv2), s1.find_last_not_of(s2));
    BOOST_TEST_EQ(sv1.find_last_not_of(sv2, 5), s1.find_last_not_of(s2, 5));
    BOOST_TEST_EQ(sv1.find_last_not_of(s2c), s1.find_last_not_of(s2c));
    BOOST_TEST_EQ(sv1.find_last_not_of(s2c, 0, 4), s1.find_last_not_of(s2c, 0, 4));

    //  first signature
    BOOST_TEST_EQ(sv3.find_last_not_of(sv3), s3.find_last_not_of(s3));  // both strings empty
    BOOST_TEST_EQ(sv1.find_last_not_of(sv3), s1.find_last_not_of(s3));  // search string empty
    BOOST_TEST_EQ(sv3.find_last_not_of(sv2), s3.find_last_not_of(s2));  // searched string empty
    BOOST_TEST_EQ(sv1.find_last_not_of(sv3, s1.size() + 2), s1.find_last_not_of(s3, s1.size() + 2));
    BOOST_TEST_EQ(sv1.find_last_not_of(sv3, s1.size() + 1), s1.find_last_not_of(s3, s1.size() + 1));
    BOOST_TEST_EQ(sv1.find_last_not_of(sv3, s1.size()), s1.find_last_not_of(s3, s1.size()));
    BOOST_TEST_EQ(sv1.find_last_not_of(sv3, s1.size() -  1), s1.find_last_not_of(s3, s1.size() -  1));

    BOOST_TEST_EQ(sv1.find_last_not_of(sv2), s1.find_last_not_of(s2));
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
    {
      //std::cout << i << ": " << sv1.find_last_not_of(sv2, i) << " " << s1.find_last_not_of(s2, i) << std::endl;
      BOOST_TEST_EQ(sv1.find_last_not_of(sv2, i), s1.find_last_not_of(s2, i));
    }
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
    {
      //std::cout << i << ": " << sv1.find_last_not_of(sv3, i) << " " << s1.find_last_not_of(s3, i) << std::endl;
      BOOST_TEST_EQ(sv1.find_last_not_of(sv3, i), s1.find_last_not_of(s3, i));
    }

    //  second signature
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
    {
      //std::cout << i << ": " << sv1.find_last_not_of('o', i) << " " << s1.find_last_not_of('o', i) << std::endl;
      BOOST_TEST_EQ(sv1.find_last_not_of('o', i), s1.find_last_not_of('o', i));
    }

    //  third signature
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
      for (std::string::size_type j = 0; j <= std::strlen(s2c) + 1; ++j)
      {
        //std::cout << i << "," << j << ": " << sv1.find_last_not_of(s2c, i, j) << " " << s1.find_last_not_of(s2c, i, j) << std::endl;
        BOOST_TEST_EQ(sv1.find_last_not_of(s2c, i, j), s1.find_last_not_of(s2c, i, j));
      }

    //  fourth signature
    BOOST_TEST_EQ(sv1.find_last_not_of(s2c), s1.find_last_not_of(s2c));
    for (std::string::size_type i = 0; i <= s1.size() + 1; ++i)
    {
      //std::cout << i << ": " << sv1.find_last_not_of(s2c, i) << " " << s1.find_last_not_of(s2c, i) << std::endl;
      BOOST_TEST_EQ(sv1.find_last_not_of(s2c, i), s1.find_last_not_of(s2c, i));  
    }
  }

}  // unnamed namespace

int cpp_main(int argc, char* argv[])
{
  typedef boost::string_view::traits_type string_traits;
  typedef boost::string_view::const_pointer const_pointer;

  swap_test();
  copy_test();
  compare_test();
  find_test();
  rfind_test();
  find_first_of_test();
  find_last_of_test();
  find_first_not_of_test();
  find_last_not_of_test();

  return boost::report_errors();
}
