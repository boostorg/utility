/*
 * Copyright Peter A. Bigot 2013.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */

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

BOOST_AUTO_TEST_CASE( test_main )
{
  test_clear();
}
