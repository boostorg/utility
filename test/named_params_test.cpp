// Copyright David Abrahams, Daniel Wallin 2003. Use, modification and 
// distribution is subject to the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/named_params.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <cassert>
#include <string.h>

namespace test
{

  using boost::keyword;
  using boost::keywords;
  using boost::arg;

  struct name_t
  {
      // At some point maybe we should use has_xxx to allow this
      typedef boost::is_convertible<boost::mpl::_1, const char*> predicate;
  };

  keyword<name_t> name;

  struct value_t;
  keyword <value_t> value;

  struct index_t;
  keyword<index_t> index;

  struct tester_t;
  keyword<tester_t> tester;

  struct f_keywords // vc6 is happier with inheritance than with a typedef
    : keywords<
          tester_t
        , arg<name_t, boost::mpl::false_, boost::is_convertible<boost::mpl::_1, const char*> >
        , value_t
        , index_t
      >
  {};

  template<class Params>
  int f_impl(const Params& p)
  {
      p[tester](
          p[name]
        , p[value | 666.222]
        , p[index | 999]
      );
      return 1;
  }

  template<class Tester, class Name, class Value, class Index>
  int f(Tester const& t, const Name& name_, const Value& value_, const Index& index_,
        typename f_keywords::restrict<Tester, Name, Value, Index>::type x = f_keywords())
  {
      return f_impl(x(t, name_, value_, index_));
  }

  template<class Tester, class Name, class Value>
  int f(Tester const& t, const Name& name_, const Value& value_, 
        typename f_keywords::restrict<Tester, Name, Value>::type x = f_keywords())
  {
      return f_impl(x(t, name_, value_));
  }

  template<class Tester, class Name>
  int f(Tester const& t, const Name& name_, 
        typename f_keywords::restrict<Tester, Name>::type x = f_keywords())
  {
      return f_impl(x(t, name_));
  }

  template <class T>
  bool equal(T const& x, T const& y)
  {
      return x == y;
  }
  
  bool equal(char const* s1, char const* s2)
  {
      return !strcmp(s1,s2);
  }
  
  template <class Name, class Value, class Index>
  struct values_t
  {
      values_t(Name const& n, Value const& v, Index const& i)
        : n(n), v(v), i(i)
      {}
      
      template <class Name_, class Value_, class Index_>
      void operator()(Name_ const& n_, Value_ const& v_, Index_ const& i_) const
      {
          BOOST_STATIC_ASSERT((boost::is_same<Name,Name_>::value));
          assert(equal(n, n_));
          BOOST_STATIC_ASSERT((boost::is_same<Value,Value_>::value));
          assert(equal(v, v_));
          BOOST_STATIC_ASSERT((boost::is_same<Index,Index_>::value));
          assert(equal(i, i_));
      }
      
      Name const& n;
      Value const& v;
      Index const& i;
  };

  template <class Name, class Value, class Index>
  values_t<Name,Value,Index>
  values(Name const& n, Value const& v, Index const& i)
  {
      return values_t<Name,Value,Index>(n,v,i);
  }
}

struct Foo { operator const char*() const { return "foo"; } };
int main()
{
   using test::f;
   using test::name;
   using test::value;
   using test::index;
   using test::tester;

   f(
       test::values("foo", "bar", "baz")
     , "foo", "bar", "baz"
   );

   f(
       test::values("foo", 666.222, 56)
     , index = 56, name = "foo"
   );
   
   //f(index = 56, name = 55); // won't compile
   return 0;
}

