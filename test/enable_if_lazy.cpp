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

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_arithmetic.hpp>

using boost::enable_if_c;
using boost::lazy_enable_if_c;
using boost::is_arithmetic;

// This class provides a reduced example of a traits class for computing
// the result of multiplying two types.  The exists constant is true when a
// multiplication operator exists between two types, and the type member is
// defined to the return type of this operator.  The return type member is
// not defined when a multiplication operator does not exist.

template <class T, class U>
struct mult_traits {
  BOOST_STATIC_CONSTANT(bool, exists = false);
};

template <>
struct mult_traits<int, int> {
  BOOST_STATIC_CONSTANT(bool, exists = true);
  typedef int type;
};

template <>
struct mult_traits<double, double> {
  BOOST_STATIC_CONSTANT(bool, exists = true);
  typedef double type;
};

// Next, a forwarding function mult() is defined.  It is enabled only when
// mult_traits<T, U>::exists is true.  The first version, using normal
// enable_if_c, works with only some compilers (it does not work in g++):

#if 0
template <class T, class U>
typename enable_if_c<
	  mult_traits<T, U>::exists, 
	  typename mult_traits<T, U>::type>::type
mult(const T& x, const U& y) {return x * y;}
#endif

// An improved version that works with more compilers uses lazy_enable_if_c.
// This template removes compiler errors from invalid code used as an
// argument to enable_if_c.

template <class T, class U>
typename lazy_enable_if_c<
	  mult_traits<T, U>::exists, 
	  mult_traits<T, U> >::type
mult(const T& x, const U& y) {return x * y;}


int test_main(int, char*[])
{
 
  BOOST_TEST(mult(1, 2) == 2);
  BOOST_TEST(mult(1.0, 3.0) == 3.0);

  return 0;
}

