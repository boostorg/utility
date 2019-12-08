/*
Copyright 2019 Glen Joseph Fernandes
(glenjofe@gmail.com)

Distributed under the Boost Software License, Version 1.0.
(http://www.boost.org/LICENSE_1_0.txt)
*/
#include <boost/core/lightweight_test.hpp>
#include <boost/utility/ostream_joiner.hpp>
#include <sstream>

int main()
{
    std::ostringstream o;
    boost::ostream_joiner<char> j = boost::make_ostream_joiner(o, ',');
    *j++ = 1;
    *j++ = '2';
    *j++ = "3";
    BOOST_TEST_EQ(o.str(), "1,2,3");
    return boost::report_errors();
}
