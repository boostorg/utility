//
//    Copyright 2003 © The Trustees of Indiana University.  All rights
//    reserved.
//
//    See the file enable_if_LICENSE for licensing conditions.
//
//    Authors: Jaakko Järvi (jajarvi at osl.iu.edu)
//             Jeremiah Willcock (jewillco at osl.iu.edu)
//             Andrew Lumsdaine (lums at osl.iu.edu)
//

#include <boost/test/minimal.hpp>
#include <boost/mpl/not.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_arithmetic.hpp>

using boost::lazy_enable_if;
using boost::lazy_disable_if;

using boost::lazy_enable_if_c;
using boost::lazy_disable_if_c;

using boost::is_arithmetic;

template <class T>
struct some_traits {
  BOOST_STATIC_CONSTANT(bool, value = false);
};

template <>
struct some_traits<int> {
  BOOST_STATIC_CONSTANT(bool, value = true);
  typedef bool type;
};

template <>
struct some_traits<double> {
  BOOST_STATIC_CONSTANT(bool, value = true);
  typedef bool type;
};

template <class T>
struct make_bool {
  typedef bool type;
};

template <>
struct make_bool<int> {};

template <>
struct make_bool<double> {};

namespace A {

  template<class T>
  typename lazy_enable_if<some_traits<T>, some_traits<T> >::type
  foo(T t) { return true; }

  template<class T>
  typename lazy_enable_if_c<some_traits<T>::value, some_traits<T> >::type
  foo2(T t) { return true; }
}

namespace B {
  template<class T>
  typename lazy_disable_if<some_traits<T>, make_bool<T> >::type
  foo(T t) { return false; }

  template<class T>
  typename lazy_disable_if_c<some_traits<T>::value, make_bool<T> >::type
  foo2(T t) { return false; }
}

int test_main(int, char*[])
{
  using namespace A;
  using namespace B;
  BOOST_TEST(foo(1));
  BOOST_TEST(foo(1.0));

  BOOST_TEST(!foo("1"));  
  BOOST_TEST(!foo(static_cast<void*>(0)));  

  BOOST_TEST(foo2(1));
  BOOST_TEST(foo2(1.0));

  BOOST_TEST(!foo2("1"));  
  BOOST_TEST(!foo2(static_cast<void*>(0)));  

  return 0;
}

