// Copyright Daniel Wallin 2003. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE-1.0 or http://www.boost.org/LICENSE-1.0)

#include <iostream>
#include <boost/named_params.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <cassert>

namespace test {

using boost::keyword;
using boost::keywords;
   
struct name_t : keyword<name_t>
{
   // this should go in a wrapper type, like arg<keyword, predicate>
   typedef boost::is_convertible<boost::mpl::_1, const char*> predicate;
   using keyword<name_t>::operator=;
} name;

struct value_t : keyword<value_t>
{
   using keyword<value_t>::operator=;
} value;

struct index_t : keyword<index_t>
{
   using keyword<index_t>::operator=;
} index;

struct f_keywords // vc6 is happier with inheritance than with a typedef
  : keywords<
     name_t
   , value_t
   , index_t
    >
{};

template<class Params>
int f_impl(const Params& p)
{
   std::cout << "-------- f --------" << std::endl;
   // name has no default
   std::cout << "name = " << p[name] << std::endl;
   std::cout << "value = " << p[value | 666.222] << std::endl;
   std::cout << "index = " << p[index | 999] << std::endl;
   return 1;
}

template<class Name, class Value, class Index>
int f(const Name& name_, const Value& value_, const Index& index_,
      typename f_keywords::restrict<Name, Value, Index>::type x = f_keywords())
{
   return f_impl(x(name_, value_, index_));
}

template<class Name, class Value>
int f(const Name& name_, const Value& value_, 
      typename f_keywords::restrict<Name, Value>::type x = f_keywords())
{
   return f_impl(x(name_, value_));
}

template<class Name>
int f(const Name& name_, 
      typename f_keywords::restrict<Name>::type x = f_keywords())
{
   return f_impl(x(name_));
}

}

struct Foo { operator const char*() const { return "foo"; } };
int main()
{
   using test::f;
   using test::name;
   using test::value;
   using test::index;

   f("foo", "bar", "baz");
#if BOOST_MSVC == 1200  // sadly templated operator= just doesn't work.
   f(index(56), name("foo"));
#else 
   f(index = 56, name = Foo());
#endif 
   //f(index = 56, name = 55); // won't compile
   return 0;
}

