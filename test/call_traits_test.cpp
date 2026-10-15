//  boost::call_traits test program

//  (C) Copyright John Maddock 2000.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).


// standalone test program for <boost/call_traits.hpp>
// 18 Mar 2002:
//    Changed some names to prevent conflicts with some new type_traits additions.
// 03 Oct 2000:
//    Enabled extra tests for VC6.

#include <boost/call_traits.hpp>
#include <cstddef>
#include <iostream>
#include <iomanip>
#include <utility>
#include <typeinfo>
#include <algorithm>
#include <boost/core/lightweight_test.hpp>
#include <boost/core/lightweight_test_trait.hpp>

#ifdef BOOST_MSVC
#pragma warning(disable:4181) // : warning C4181: qualifier applied to reference type; ignored
#endif

// a user-defined enum
enum enum_UDT { one, two, three };

// abstract base class
struct test_abc
{
   test_abc();
   virtual ~test_abc() {}
   test_abc(const test_abc&);
   test_abc& operator=(const test_abc&);
   virtual void foo() = 0;
   virtual void foo2() = 0;
};

// incomplete type
struct incomplete_type;

// a way prevent warnings for unused variables
template<class T> inline void unused_variable(const T&) {}

//
// struct contained models a type that contains a type (for example std::pair)
// arrays are contained by value, and have to be treated as a special case:
//
template <class T>
struct contained
{
   // define our typedefs first, arrays are stored by value
   // so value_type is not the same as result_type:
   typedef typename boost::call_traits<T>::param_type       param_type;
   typedef typename boost::call_traits<T>::reference        reference;
   typedef typename boost::call_traits<T>::const_reference  const_reference;
   typedef T                                                value_type;
   typedef typename boost::call_traits<T>::value_type       result_type;

   // stored value:
   value_type v_;

   // constructors:
   contained() {}
   contained(param_type p) : v_(p){}
   // return byval:
   result_type value()const { return v_; }
   // return by_ref:
   reference get() { return v_; }
   const_reference const_get()const { return v_; }
   // pass value:
   void call(param_type){}
private:
   contained& operator=(const contained&);
};

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template <class T, std::size_t N>
struct contained<T[N]>
{
   typedef typename boost::call_traits<T[N]>::param_type       param_type;
   typedef typename boost::call_traits<T[N]>::reference        reference;
   typedef typename boost::call_traits<T[N]>::const_reference  const_reference;
   typedef T                                                   value_type[N];
   typedef typename boost::call_traits<T[N]>::value_type       result_type;

   value_type v_;

   contained(param_type p)
   {
      std::copy(p, p+N, v_);
   }
   // return byval:
   result_type value()const { return v_; }
   // return by_ref:
   reference get() { return v_; }
   const_reference const_get()const { return v_; }
   void call(param_type){}
private:
   contained& operator=(const contained&);
};
#endif

template <class T>
contained<typename boost::call_traits<T>::value_type> test_wrap_type(const T& t)
{
   typedef typename boost::call_traits<T>::value_type ct;
   return contained<ct>(t);
}

namespace test{

template <class T1, class T2>
std::pair<
   typename boost::call_traits<T1>::value_type,
   typename boost::call_traits<T2>::value_type>
      make_pair(const T1& t1, const T2& t2)
{
   return std::pair<
      typename boost::call_traits<T1>::value_type,
      typename boost::call_traits<T2>::value_type>(t1, t2);
}

} // namespace test

using namespace std;

//
// struct call_traits_checker:
// verifies behaviour of contained example:
//
template <class T>
struct call_traits_checker
{
   typedef typename boost::call_traits<T>::param_type param_type;
   void operator()(param_type);
};

template <class T>
void call_traits_checker<T>::operator()(param_type p)
{
   T t(p);
   contained<T> c(t);
   std::cout << "checking contained<" << typeid(T).name() << ">..." << std::endl;
   BOOST_TEST(t == c.value());
   BOOST_TEST(t == c.get());
   BOOST_TEST(t == c.const_get());
#ifndef __ICL
   //std::cout << "typeof contained<" << typeid(T).name() << ">::v_ is:           " << typeid(&contained<T>::v_).name() << std::endl;
   std::cout << "typeof contained<" << typeid(T).name() << ">::value() is:      " << typeid(&contained<T>::value).name() << std::endl;
   std::cout << "typeof contained<" << typeid(T).name() << ">::get() is:        " << typeid(&contained<T>::get).name() << std::endl;
   std::cout << "typeof contained<" << typeid(T).name() << ">::const_get() is:  " << typeid(&contained<T>::const_get).name() << std::endl;
   std::cout << "typeof contained<" << typeid(T).name() << ">::call() is:       " << typeid(&contained<T>::call).name() << std::endl;
   std::cout << std::endl;
#endif
}

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template <class T, std::size_t N>
struct call_traits_checker<T[N]>
{
   typedef typename boost::call_traits<T[N]>::param_type param_type;
   void operator()(param_type t)
   {
      contained<T[N]> c(t);
      std::cout << "checking contained<" << typeid(T[N]).name() << ">..." << std::endl;
      unsigned int i = 0;
      for(i = 0; i < N; ++i)
         BOOST_TEST(t[i] == c.value()[i]);
      for(i = 0; i < N; ++i)
         BOOST_TEST(t[i] == c.get()[i]);
      for(i = 0; i < N; ++i)
         BOOST_TEST(t[i] == c.const_get()[i]);

      std::cout << "typeof contained<" << typeid(T[N]).name() << ">::v_ is:         " << typeid(&contained<T[N]>::v_).name() << std::endl;
      std::cout << "typeof contained<" << typeid(T[N]).name() << ">::value is:      " << typeid(&contained<T[N]>::value).name() << std::endl;
      std::cout << "typeof contained<" << typeid(T[N]).name() << ">::get is:        " << typeid(&contained<T[N]>::get).name() << std::endl;
      std::cout << "typeof contained<" << typeid(T[N]).name() << ">::const_get is:  " << typeid(&contained<T[N]>::const_get).name() << std::endl;
      std::cout << "typeof contained<" << typeid(T[N]).name() << ">::call is:       " << typeid(&contained<T[N]>::call).name() << std::endl;
      std::cout << std::endl;
   }
};
#endif

//
// check_wrap:
template <class W, class U>
void check_wrap(const W& w, const U& u)
{
   std::cout << "checking " << typeid(W).name() << "..." << std::endl;
   BOOST_TEST(w.value() == u);
}

//
// check_make_pair:
// verifies behaviour of "make_pair":
//
template <class T, class U, class V>
void check_make_pair(T c, U u, V v)
{
   std::cout << "checking std::pair<" << typeid(c.first).name() << ", " << typeid(c.second).name() << ">..." << std::endl;
   BOOST_TEST(c.first == u);
   BOOST_TEST(c.second == v);
   std::cout << std::endl;
}


struct comparible_UDT
{
   int i_;
   comparible_UDT() : i_(2){}
   comparible_UDT(const comparible_UDT& other) : i_(other.i_){}
   comparible_UDT& operator=(const comparible_UDT& other)
   {
      i_ = other.i_;
      return *this;
   }
   bool operator == (const comparible_UDT& v) const { return v.i_ == i_; }
};

int main()
{
   call_traits_checker<comparible_UDT> c1;
   comparible_UDT u;
   c1(u);
   call_traits_checker<int> c2;
   call_traits_checker<enum_UDT> c2b;
   int i = 2;
   c2(i);
   c2b(one);
   int* pi = &i;
   int a[2] = {1,2};
#if defined(BOOST_MSVC6_MEMBER_TEMPLATES) && !defined(__ICL)
   call_traits_checker<int*> c3;
   c3(pi);
   call_traits_checker<int&> c4;
   c4(i);
   call_traits_checker<const int&> c5;
   c5(i);
#if !defined (BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !defined(__MWERKS__) && !defined(__SUNPRO_CC)
   call_traits_checker<int[2]> c6;
   c6(a);
#endif
#endif

   check_wrap(test_wrap_type(2), 2);
#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !defined(__SUNPRO_CC)
   check_wrap(test_wrap_type(a), a);
   check_make_pair(test::make_pair(a, a), a, a);
#endif

   // cv-qualifiers applied to reference types should have no effect
   // declare these here for later use with is_reference and remove_reference:
   typedef int& r_type;
   typedef const r_type cr_type;

   BOOST_TEST_TRAIT_SAME(comparible_UDT, boost::call_traits<comparible_UDT>::value_type);
   BOOST_TEST_TRAIT_SAME(comparible_UDT&, boost::call_traits<comparible_UDT>::reference);
   BOOST_TEST_TRAIT_SAME(const comparible_UDT&, boost::call_traits<comparible_UDT>::const_reference);
   BOOST_TEST_TRAIT_SAME(const comparible_UDT&, boost::call_traits<comparible_UDT>::param_type);
   BOOST_TEST_TRAIT_SAME(int, boost::call_traits<int>::value_type);
   BOOST_TEST_TRAIT_SAME(int&, boost::call_traits<int>::reference);
   BOOST_TEST_TRAIT_SAME(const int&, boost::call_traits<int>::const_reference);
   BOOST_TEST_TRAIT_SAME(const int, boost::call_traits<int>::param_type);
   BOOST_TEST_TRAIT_SAME(int*, boost::call_traits<int*>::value_type);
   BOOST_TEST_TRAIT_SAME(int*&, boost::call_traits<int*>::reference);
   BOOST_TEST_TRAIT_SAME(int*const&, boost::call_traits<int*>::const_reference);
   BOOST_TEST_TRAIT_SAME(int*const, boost::call_traits<int*>::param_type);
#if defined(BOOST_MSVC6_MEMBER_TEMPLATES)
   BOOST_TEST_TRAIT_SAME(int&, boost::call_traits<int&>::value_type);
   BOOST_TEST_TRAIT_SAME(int&, boost::call_traits<int&>::reference);
   BOOST_TEST_TRAIT_SAME(const int&, boost::call_traits<int&>::const_reference);
   BOOST_TEST_TRAIT_SAME(int&, boost::call_traits<int&>::param_type);
#if !(defined(__GNUC__) && ((__GNUC__ < 3) || (__GNUC__ == 3) && (__GNUC_MINOR__ < 1)))
   BOOST_TEST_TRAIT_SAME(int&, boost::call_traits<cr_type>::value_type);
   BOOST_TEST_TRAIT_SAME(int&, boost::call_traits<cr_type>::reference);
   BOOST_TEST_TRAIT_SAME(const int&, boost::call_traits<cr_type>::const_reference);
   BOOST_TEST_TRAIT_SAME(int&, boost::call_traits<cr_type>::param_type);
#else
   std::cout << "Your compiler cannot instantiate call_traits<int&const>, skipping four tests (4 errors)" << std::endl;
#endif
   BOOST_TEST_TRAIT_SAME(const int&, boost::call_traits<const int&>::value_type);
   BOOST_TEST_TRAIT_SAME(const int&, boost::call_traits<const int&>::reference);
   BOOST_TEST_TRAIT_SAME(const int&, boost::call_traits<const int&>::const_reference);
   BOOST_TEST_TRAIT_SAME(const int&, boost::call_traits<const int&>::param_type);
#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
   BOOST_TEST_TRAIT_SAME(const int*, boost::call_traits<int[3]>::value_type);
   BOOST_TEST_TRAIT_SAME(int(&)[3], boost::call_traits<int[3]>::reference);
   BOOST_TEST_TRAIT_SAME(const int(&)[3], boost::call_traits<int[3]>::const_reference);
   BOOST_TEST_TRAIT_SAME(const int*const, boost::call_traits<int[3]>::param_type);
   BOOST_TEST_TRAIT_SAME(const int*, boost::call_traits<const int[3]>::value_type);
   BOOST_TEST_TRAIT_SAME(const int(&)[3], boost::call_traits<const int[3]>::reference);
   BOOST_TEST_TRAIT_SAME(const int(&)[3], boost::call_traits<const int[3]>::const_reference);
   BOOST_TEST_TRAIT_SAME(const int*const, boost::call_traits<const int[3]>::param_type);
   // test with abstract base class:
   BOOST_TEST_TRAIT_SAME(test_abc, boost::call_traits<test_abc>::value_type);
   BOOST_TEST_TRAIT_SAME(test_abc&, boost::call_traits<test_abc>::reference);
   BOOST_TEST_TRAIT_SAME(const test_abc&, boost::call_traits<test_abc>::const_reference);
   BOOST_TEST_TRAIT_SAME(const test_abc&, boost::call_traits<test_abc>::param_type);
#else
   std::cout << "You're compiler does not support partial template specialiation, skipping 8 tests (8 errors)" << std::endl;
#endif
#else
   std::cout << "You're compiler does not support partial template specialiation, skipping 20 tests (20 errors)" << std::endl;
#endif
   // test with an incomplete type:
   BOOST_TEST_TRAIT_SAME(incomplete_type, boost::call_traits<incomplete_type>::value_type);
   BOOST_TEST_TRAIT_SAME(incomplete_type&, boost::call_traits<incomplete_type>::reference);
   BOOST_TEST_TRAIT_SAME(const incomplete_type&, boost::call_traits<incomplete_type>::const_reference);
   BOOST_TEST_TRAIT_SAME(const incomplete_type&, boost::call_traits<incomplete_type>::param_type);
   // test enum:
   BOOST_TEST_TRAIT_SAME(enum_UDT, boost::call_traits<enum_UDT>::value_type);
   BOOST_TEST_TRAIT_SAME(enum_UDT&, boost::call_traits<enum_UDT>::reference);
   BOOST_TEST_TRAIT_SAME(const enum_UDT&, boost::call_traits<enum_UDT>::const_reference);
   BOOST_TEST_TRAIT_SAME(const enum_UDT, boost::call_traits<enum_UDT>::param_type);

   return boost::report_errors();
}

//
// define call_traits tests to check that the assertions in the docs do actually work
// this is an compile-time only set of tests:
//
template <typename T, bool isarray = false>
struct call_traits_test
{
   typedef ::boost::call_traits<T> ct;
   typedef typename ct::param_type param_type;
   typedef typename ct::reference reference;
   typedef typename ct::const_reference const_reference;
   typedef typename ct::value_type value_type;
   static void assert_construct(param_type val);
};

template <typename T, bool isarray>
void call_traits_test<T, isarray>::assert_construct(typename call_traits_test<T, isarray>::param_type val)
{
   //
   // this is to check that the call_traits assertions are valid:
   T t(val);
   value_type v(t);
   reference r(t);
   const_reference cr(t);
   param_type p(t);
   value_type v2(v);
   value_type v3(r);
   value_type v4(p);
   reference r2(v);
   reference r3(r);
   const_reference cr2(v);
   const_reference cr3(r);
   const_reference cr4(cr);
   const_reference cr5(p);
   param_type p2(v);
   param_type p3(r);
   param_type p4(p);

   unused_variable(v2);
   unused_variable(v3);
   unused_variable(v4);
   unused_variable(r2);
   unused_variable(r3);
   unused_variable(cr2);
   unused_variable(cr3);
   unused_variable(cr4);
   unused_variable(cr5);
   unused_variable(p2);
   unused_variable(p3);
   unused_variable(p4);
}
#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template <typename T>
struct call_traits_test<T, true>
{
   typedef ::boost::call_traits<T> ct;
   typedef typename ct::param_type param_type;
   typedef typename ct::reference reference;
   typedef typename ct::const_reference const_reference;
   typedef typename ct::value_type value_type;
   static void assert_construct(param_type val);
};

template <typename T>
void call_traits_test<T, true>::assert_construct(typename boost::call_traits<T>::param_type val)
{
   //
   // this is to check that the call_traits assertions are valid:
   T t;
   value_type v(t);
   value_type v5(val);
   reference r = t;
   const_reference cr = t;
   reference r2 = r;
   #ifndef BOOST_BORLANDC
   // C++ Builder buglet:
   const_reference cr2 = r;
   #endif
   param_type p(t);
   value_type v2(v);
   const_reference cr3 = cr;
   value_type v3(r);
   value_type v4(p);
   param_type p2(v);
   param_type p3(r);
   param_type p4(p);

   unused_variable(v2);
   unused_variable(v3);
   unused_variable(v4);
   unused_variable(v5);
#ifndef BOOST_BORLANDC
   unused_variable(r2);
   unused_variable(cr2);
#endif
   unused_variable(cr3);
   unused_variable(p2);
   unused_variable(p3);
   unused_variable(p4);
}
#endif //BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
//
// now check call_traits assertions by instantiating call_traits_test:
template struct call_traits_test<int>;
template struct call_traits_test<const int>;
template struct call_traits_test<int*>;
#if defined(BOOST_MSVC6_MEMBER_TEMPLATES)
template struct call_traits_test<int&>;
template struct call_traits_test<const int&>;
#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !defined(__SUNPRO_CC)
template struct call_traits_test<int[2], true>;
#endif
#endif

