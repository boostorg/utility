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
using boost::is_arithmetic;

template<class T>
typename enable_if<is_arithmetic<T>::value, bool>::type
arithmetic_object(T t) { return true; }

template<class T>
typename enable_if<!is_arithmetic<T>::value, bool>::type
arithmetic_object(T t) { return false; }


int test_main(int, char*[])
{
 
  BOOST_TEST(arithmetic_object(1));
  BOOST_TEST(arithmetic_object(1.0));

  BOOST_TEST(!arithmetic_object("1"));  
  BOOST_TEST(!arithmetic_object(static_cast<void*>(0)));  

  return 0;
}

