// Copyright David Abrahams, Daniel Wallin 2003. Use, modification and 
// distribution is subject to the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt

#include <boost/named_params.hpp>
#include <cassert>
#include <string>
#include <boost/type_traits/is_convertible.hpp>

namespace test
{
  using boost::keyword;
  using boost::keywords;
  using boost::named_param;

  struct name_t; keyword<name_t> name;
  struct value_t; keyword<value_t> value;
  
  struct f_keywords
      : keywords<
            named_param<
                name_t
              , boost::mpl::true_
              , boost::is_convertible<boost::mpl::_, std::string>
            >
          , named_param<
                value_t
              , boost::mpl::true_
              , boost::is_convertible<boost::mpl::_, float>
            >
        >
  {};

  BOOST_NAMED_PARAMS_FUN(void, f, 0, 2, f_keywords)
  {
      std::string s = p[name | "bar"];
      float v = p[value | 3.f];
      
      assert(s == "foo");
      assert(v == 3.f);
  }

} // namespace test

int main()
{
    using test::name;
    using test::value;    
    using test::f;

    f("foo");
    f("foo", 3.f);
    f(value = 3.f, name = "foo");
}

