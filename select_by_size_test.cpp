#include <boost/static_assert.hpp>
#include <boost/utility/select_by_size.hpp>

using boost::utility::case_;

case_<0> helper(bool);
case_<1> helper(int);
case_<2> helper(unsigned);
case_<3> helper(long);
case_<4> helper(unsigned long);
case_<5> helper(float);
case_<6> helper(double);
case_<7> helper(const char*);

struct test {
    BOOST_SELECT_BY_SIZE(int, v0, helper(true));
    BOOST_SELECT_BY_SIZE(int, v1, helper(0));
    BOOST_SELECT_BY_SIZE(int, v2, helper(0U));
    BOOST_SELECT_BY_SIZE(int, v3, helper(0L));
    BOOST_SELECT_BY_SIZE(int, v4, helper(0UL));
    BOOST_SELECT_BY_SIZE(int, v5, helper(0.0F));
    BOOST_SELECT_BY_SIZE(int, v6, helper(0.0));
    BOOST_SELECT_BY_SIZE(int, v7, helper("hello"));
};

#include <iostream>

int main()
{
    using namespace boost::utility;

    BOOST_STATIC_ASSERT( select_by_size< sizeof(helper(true)) >::value == 0 );
    BOOST_STATIC_ASSERT( select_by_size< sizeof(helper(0)) >::value == 1 );
    BOOST_STATIC_ASSERT( select_by_size< sizeof(helper(0U)) >::value == 2 );
    BOOST_STATIC_ASSERT( select_by_size< sizeof(helper(0L)) >::value == 3 );
    BOOST_STATIC_ASSERT( select_by_size< sizeof(helper(0UL)) >::value == 4 );
    BOOST_STATIC_ASSERT( select_by_size< sizeof(helper(0.0F)) >::value == 5 );
    BOOST_STATIC_ASSERT( select_by_size< sizeof(helper(0.0)) >::value == 6 );
    BOOST_STATIC_ASSERT( select_by_size< sizeof(helper("hello")) >::value == 7 );

    BOOST_STATIC_ASSERT(test::v0 == 0);
    BOOST_STATIC_ASSERT(test::v1 == 1);
    BOOST_STATIC_ASSERT(test::v2 == 2);
    BOOST_STATIC_ASSERT(test::v3 == 3);
    BOOST_STATIC_ASSERT(test::v4 == 4);
    BOOST_STATIC_ASSERT(test::v5 == 5);
    BOOST_STATIC_ASSERT(test::v6 == 6);
    BOOST_STATIC_ASSERT(test::v7 == 7);

    return 0;
}





