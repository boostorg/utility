// Copyright David Abrahams, Daniel Wallin 2003. Use, modification and 
// distribution is subject to the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/named_params.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <cassert>
#include <string.h>
//#include <iostream>
#include <boost/ref.hpp>
/*
BOOST_NAMED_PARAMS_FUN(int, f, 0, 3, f_keywords)
{
    return p[index];
}
*/

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

  struct X
  {
     X(const char*) {}
  };

  template<class T>
  struct convertible_from
  {
     template<class U>
     struct apply : boost::is_convertible<U, T>
     {};
  };

  struct f_keywords // vc6 is happier with inheritance than with a typedef
    : keywords<
          tester_t
          , arg<name_t, boost::mpl::false_, convertible_from<const char*> >
        , value_t
        , index_t
      >
  {};

  struct value_default
  {
      typedef double result_type;

      double operator()() const
      {
          return 666.222;
      }
  };

  template<class Params>
  int f_impl(const Params& p)
  { 
      p[tester](
          p[name]
        , p[value || value_default() ]
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

          // Only VC and its emulators fail this; they seem to have
          // problems with deducing the constness of string literal
          // arrays.
 #if defined(_MSC_VER)                                      \
     && (BOOST_WORKAROUND(BOOST_INTEL_CXX_VERSION, <= 700)  \
         || BOOST_WORKAROUND(BOOST_MSVC, < 1310))
# else 
          BOOST_STATIC_ASSERT((boost::is_same<Index,Index_>::value));
          BOOST_STATIC_ASSERT((boost::is_same<Value,Value_>::value));
          BOOST_STATIC_ASSERT((boost::is_same<Name,Name_>::value));
#endif
          assert(equal(n, n_));
          assert(equal(v, v_));
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

// GCC2 has a problem with char (&)[] deduction, so we'll cast string
// literals there.
#undef S
#if BOOST_WORKAROUND(__GNUC__, == 2)
# define S(s) (char const*)s
#else
# define S(s) s
#endif

int main()
{
   using test::f;
   using test::name;
   using test::value;
   using test::index;
   using test::tester;

   f(
       test::values(S("foo"), S("bar"), S("baz"))
     , S("foo"), S("bar"), S("baz")
   );

   int x = 56;
   f(
       test::values("foo", 666.222, 56)
     , index = boost::ref(x), name = "foo"
   );
   
   //f(index = 56, name = 55); // won't compile
   return 0;
}
