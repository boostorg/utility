//object_tracked with DTOR check for double destruction (death)
#ifndef BOOST_DEBUG_UTILITY_CHK_DBL_DELETE_HPP_LJE20040129
#define BOOST_UTILITY_CHK_DBL_DELETE_HPP_LJE20040129
#include "boost/utility/object_tracked.hpp"
#include <boost/test/test_tools.hpp>
namespace utility
{
struct chk_dbl_die
: public object_tracked
{
    ~chk_dbl_die(void)
    {
        BOOST_CHECK(am_i_live());
    }
};
}//exit utility namespace
#endif
