#include <boost/result_of.hpp>
#include <utility>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

class int_result_type { typedef int result_type; };

class int_result_of
{
  template<typename F> struct result { typedef int type; };
};

class int_result_type_and_float_result_of
{
  typedef int result_type;
  template<typename F> struct result { typedef float type; };
};

struct X {};

int main()
{
  using namespace boost;

  typedef int (*func_ptr)(float, double);
  typedef int (X::*mem_func_ptr)(float);

  BOOST_STATIC_ASSERT((is_same<result_of<int_result_type(float)>::type, int>::value));
  BOOST_STATIC_ASSERT((is_same<result_of<int_result_of(double)>::type, int>::value));
  BOOST_STATIC_ASSERT((is_same<result_of<int_result_of(void)>::type, void>::value));
  BOOST_STATIC_ASSERT((is_same<result_of<int_result_type_and_float_result_of(char)>::type, int>::value));
  BOOST_STATIC_ASSERT((is_same<result_of<func_ptr(char, float)>::type, int>::value));
  BOOST_STATIC_ASSERT((is_same<result_of<mem_func_ptr(X,char)>::type, int>::value));
  return 0;
}
