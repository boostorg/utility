// (C) Copyright Jonathan Turkanis 2004.
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies. This
// software is provided "as is" without express or implied warranty, and
// with no claim as to its suitability for any purpose.

// Test program for <boost/utility/select_by_size.hpp>

#include <boost/static_assert.hpp>
#include <boost/config.hpp>

// Include "select_by_size.hpp" with BOOST_SELECT_BY_SIZE_MAX_CASE undefined.
#include <boost/utility/select_by_size.hpp>
using boost::utility::case_;

case_<false> helper(bool);
case_<true> helper(int);

struct test1 {
    // Define static bool constants v0 and v1.
    BOOST_SELECT_BY_SIZE(bool, v0, helper(true));
    BOOST_SELECT_BY_SIZE(bool, v1, helper(0));

    BOOST_STATIC_ASSERT(v0 == false);
    BOOST_STATIC_ASSERT(v1 == true);
};

// Include "select_by_size.hpp" a second time, defining more cases.
#define BOOST_SELECT_BY_SIZE_MAX_CASE 7
#include <boost/utility/select_by_size.hpp>

case_<2> helper(unsigned);
case_<3> helper(long);
case_<4> helper(unsigned long);
case_<5> helper(float);
case_<6> helper(double);
case_<7> helper(const char*);

struct test2 {
    // Define static int constants v0 through v7.
    BOOST_SELECT_BY_SIZE(int, v0, helper(true));
    BOOST_SELECT_BY_SIZE(int, v1, helper(0));
    BOOST_SELECT_BY_SIZE(int, v2, helper(0U));
    BOOST_SELECT_BY_SIZE(int, v3, helper(0L));
    BOOST_SELECT_BY_SIZE(int, v4, helper(0UL));
    BOOST_SELECT_BY_SIZE(int, v5, helper(0.0F));
    BOOST_SELECT_BY_SIZE(int, v6, helper(0.0));
    BOOST_SELECT_BY_SIZE(int, v7, helper("hello"));

    BOOST_STATIC_ASSERT(v0 == 0);
    BOOST_STATIC_ASSERT(v1 == 1);
    BOOST_STATIC_ASSERT(v2 == 2);
    BOOST_STATIC_ASSERT(v3 == 3);
    BOOST_STATIC_ASSERT(v4 == 4);
    BOOST_STATIC_ASSERT(v5 == 5);
    BOOST_STATIC_ASSERT(v6 == 6);
    BOOST_STATIC_ASSERT(v7 == 7);
};

int main() { return 0; }
