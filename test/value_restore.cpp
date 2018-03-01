// Copyright 2010, Niels Dekker.
// Copyright 2018, Oleg Abrosimov.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Test program for boost::value_restore<T>.
//
// 1 Mart 2018 (Created) Oleg Abrosimov

#include <boost/utility/value_restore.hpp>
#include <boost/core/lightweight_test.hpp>

#include <string>

namespace
{
  template <class T>
  void test_value_restore(T& magic_value, const T& new_value)
  {
    const T old_value = magic_value;
    // require different values for test
    BOOST_TEST( old_value != new_value );
    {
#ifndef BOOST_NO_CXX11_TEMPLATE_ALIASES
      boost::value_restore_ptr<T> ptr = boost::make_value_restore_ptr(magic_value);
#else
    #ifndef BOOST_NO_CXX11_SMART_PTR
      typedef std::unique_ptr<T, boost::value_restore<T> > value_restore_ptr;
    #else
      typedef boost::movelib::unique_ptr<T, boost::value_restore<T> > value_restore_ptr;
    #endif
      value_restore_ptr ptr = 
        value_restore_ptr(&magic_value, boost::make_value_restore(magic_value));
#endif
      magic_value = new_value;
      BOOST_TEST( magic_value == new_value );
      BOOST_TEST( *ptr == new_value );
    }
    BOOST_TEST( magic_value != new_value );
    BOOST_TEST( magic_value == old_value );
  }

  struct foo
  {
    int data;
  };

  bool operator==(const foo& lhs, const foo& rhs)
  {
    return lhs.data == rhs.data;
  }
}


// Tests boost::initialize for a fundamental type, a type with a
// user-defined constructor, and a user-defined type without 
// a user-defined constructor.
int main()
{

  int magic_number = 42;
  const int new_number = 43;
  test_value_restore(magic_number, new_number);

  // test for pointers
  test_value_restore(&magic_number, &new_number);

  std::string magic_string = "magic value";
  const std::string new_string = "magic value 2";
  test_value_restore(magic_string, new_string);

  foo magic_foo = { 42 };
  const foo new_magic_foo = { 43 };
  test_value_restore(magic_foo, new_magic_foo);

  return boost::report_errors();
}
