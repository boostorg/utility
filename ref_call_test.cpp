#include <boost/ref.hpp>
#include <functional>
#include <algorithm>
#include <vector>
#include <cassert>

class generate_zero {
public:
  typedef int result_type;
  generate_zero() {}
  int operator()() const { return 0; }

private:
  generate_zero(const generate_zero&);
};

class generate_zero_no_result_type {
public:
  generate_zero_no_result_type() {}
  int operator()() const { return 0; }

private:
  generate_zero_no_result_type(const generate_zero_no_result_type&);
};

template<typename F>
void check_generate_zero(F f)
{
  assert(f() == 0);
}

class negate_with_result_type
{
public:
  typedef int result_type;

  negate_with_result_type() {}
  int operator()(int x) const { return -x; }

private:
  negate_with_result_type(const negate_with_result_type&);
};

class negate_with_result_of
{
public:
  template<typename T>
  struct result_of
  {
    typedef int type;
  };

  negate_with_result_of() {}
  int operator()(int x) const { return -x; }

private:
  negate_with_result_of(const negate_with_result_of&);
};

template<typename F>
void check_negate(F f)
{
  int x = 5;
  assert(f(x) == -x);
}

class add_with_result_type
{
public:
  typedef int result_type;

  add_with_result_type() {}
  int operator()(int x, int y) const { return x + y; }

private:
  add_with_result_type(const add_with_result_type&);
};

class add_with_result_of
{
public:
  template<typename F> struct result_of { typedef int type; };

  add_with_result_of() {}
  int operator()(int x, int y) const { return x + y; }

private:
  add_with_result_of(const add_with_result_of&);
};

template<typename F>
void check_sum(F f)
{
  int x = 3;
  int y = 5;
  assert(f(x, y) == x+y);
}

struct zero_negate_add_result_type
{
public:
  typedef int result_type;

  zero_negate_add_result_type() {}
  int operator()() const { return 0; }
  int operator()(int x) const { return -x; }
  int operator()(int x, int y) const { return x+y; }

private:
  zero_negate_add_result_type(const zero_negate_add_result_type&);
};

struct zero_negate_add_result_of
{
public:
  template<typename F> struct result_of { typedef int type; };

  zero_negate_add_result_of() {}
  int operator()() const { return 0; }
  int operator()(int x) const { return -x; }
  int operator()(int x, int y) const { return x+y; }

private:
  zero_negate_add_result_of(const zero_negate_add_result_of&);
};

int main()
{
  // Arity zero function objects
  generate_zero gz;
  generate_zero_no_result_type gznrt;
  check_generate_zero(boost::ref(gz));
  boost::ref(gznrt);

  // Arity 1 function objects
  negate_with_result_type nrt;
  negate_with_result_of nro;
  check_negate(boost::ref(nrt));
  check_negate(boost::ref(nro));

  // Arity 2 function objects
  add_with_result_type art;
  add_with_result_of aro;
  check_sum(boost::ref(art));
  check_sum(boost::ref(aro));

  // Arity overloading in function objects
  zero_negate_add_result_type znart;
  zero_negate_add_result_type znaro;
  check_generate_zero(boost::ref(znart));
  check_negate(boost::ref(znart));
  check_sum(boost::ref(znart));
  check_generate_zero(boost::ref(znaro));
  check_negate(boost::ref(znaro));
  check_sum(boost::ref(znaro));

  return 0;
}
