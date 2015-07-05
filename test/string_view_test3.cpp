/*
   Copyright (c) Beman Dawes 2015

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

namespace
{

}  // unnamed namespace

int main()
{
  typedef boost::string_view::traits_type string_traits;
  
  return boost::report_errors();
}
