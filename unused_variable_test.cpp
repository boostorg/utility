// Copyright (c) 2014 Adam Wulkiewicz, Lodz, Poland.
//
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//#include <boost/utility/unused_variable.hpp>
#include <boost/utility.hpp>

void test1()
{
    int a;
    boost::ignore_unused_variable_warning(a);
}

void test2()
{
    int a, b;
    boost::ignore_unused_variable_warning(a, b);
}

void test3()
{
    int a, b, c;
    boost::ignore_unused_variable_warning(a, b, c);
}

void test4()
{
    int a, b, c, d;
    boost::ignore_unused_variable_warning(a, b, c, d);
}

void test5()
{
    int a, b, c, d, e;
    boost::ignore_unused_variable_warning(a, b, c, d, e);
}

int main()
{
    test1();
    test2();
    test3();
    test4();
    test5();

    return 0;
}
