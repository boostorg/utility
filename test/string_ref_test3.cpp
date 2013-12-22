/*
 * Copyright Peter A. Bigot 2013.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */

#include <algorithm>
#include <stdexcept>
#include <boost/utility/string_ref.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

using boost::string_ref;
using namespace boost::unit_test;

void test_clear ()
{
  string_ref sv0;
  string_ref svn("1234");

  BOOST_CHECK_NE(sv0.data(), svn.data());
  BOOST_CHECK_NE(sv0.size(), svn.size());
  svn.clear();
  BOOST_CHECK_EQUAL(sv0.data(), svn.data());
  BOOST_CHECK_EQUAL(sv0.size(), svn.size());
}

void test_copy ()
{
  typedef string_ref::size_type szt;
  string_ref sv("123456789A");
  char buffer[4] = { 0 };
  char * const ebuffer = buffer + sizeof(buffer) / sizeof(*buffer);

  BOOST_CHECK_EQUAL(char(), buffer[0]);
  BOOST_CHECK_EQUAL('1', sv[0]);

  BOOST_CHECK_THROW(sv.copy(buffer, sizeof(buffer), string_ref::npos),
                    std::out_of_range);
  BOOST_CHECK_THROW(sv.copy(buffer, sizeof(buffer), sv.size()+1),
                    std::out_of_range);

  string_ref::size_type len = sv.copy(buffer, sizeof(buffer), 8);
  BOOST_CHECK_EQUAL(szt(2), len);
  BOOST_CHECK_EQUAL('9', buffer[0]);

  std::fill(buffer, ebuffer, 0);
  BOOST_CHECK_EQUAL(char(), buffer[0]);

  len = sv.copy(buffer, sizeof(buffer), sv.size());
  BOOST_CHECK_EQUAL(szt(0), len);
  BOOST_CHECK_EQUAL(char(), buffer[0]);

  len = sv.copy(buffer, sizeof(buffer), 0);
  BOOST_CHECK_EQUAL(szt(4), len);
  BOOST_CHECK(std::equal(buffer, ebuffer, sv.data()));
}

BOOST_AUTO_TEST_CASE( test_main )
{
  test_clear();
  test_copy();
}
