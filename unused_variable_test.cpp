// Copyright (c) 2014 Adam Wulkiewicz, Lodz, Poland.
//
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//#include <boost/utility/unused_variable.hpp>
#include <boost/utility.hpp>

template <typename T1, typename T2, typename T3, typename T4, typename T5>
void test(T1 const& t1, T2 const& t2, T3 const& t3, T4 const& t4, T5 const& t5)
{
    boost::ignore_unused_variable_warning(t1);
    boost::ignore_unused_variable_warning(t1, t2);
    boost::ignore_unused_variable_warning(t1, t2, t3);
    boost::ignore_unused_variable_warning(t1, t2, t3, t4);
    boost::ignore_unused_variable_warning(t1, t2, t3, t4, t5);
}

int main()
{
    test(0, 1.0f, 2.0, '3', "4");
    return 0;
}
