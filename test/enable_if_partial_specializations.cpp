//
//    Copyright 2003 © The Trustees of Indiana University.  All rights
//    reserved.
//
//    See the file enable_if_LICENSE for licensing conditions.
//
//    Authors: Jaakko Järvi (jajarvi@osl.iu.edu)
//             Jeremiah Willcock (jewillco@osl.iu.edu)
//             Andrew Lumsdaine (lums@osl.iu.edu)
//

#include <boost/test/minimal.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_arithmetic.hpp>

using boost::enable_if;
using boost::disable_if;
using boost::is_arithmetic;

template <class T, class Enable = void>
struct tester;

template <class T>
struct tester<T, typename enable_if<is_arithmetic<T>::value>::type> {
  BOOST_STATIC_CONSTANT(bool, value = true);
};

template <class T>
struct tester<T, typename disable_if<is_arithmetic<T>::value>::type> {
  BOOST_STATIC_CONSTANT(bool, value = false);
};

int test_main(int, char*[])
{
 
  BOOST_TEST(tester<int>::value);
  BOOST_TEST(tester<double>::value);

  BOOST_TEST(!tester<char*>::value);
  BOOST_TEST(!tester<void*>::value);

  return 0;
}

