// Copyright Daniel Wallin 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/result_check.hpp>
#include <boost/type_traits/is_same.hpp>

BOOST_RESULT_CHECK(2, add, operator+, _1 + _2)

struct X
{
};

X operator+(X const&, X const&);
X& operator+(X&, X&);

int main()
{
    using namespace boost;
  
    BOOST_MPL_ASSERT((check_add<X const&, X const&, is_same<mpl::_, X> >));
    BOOST_MPL_ASSERT((check_add<X&, X&, is_same<mpl::_, X&> >));
}

