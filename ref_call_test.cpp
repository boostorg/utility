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

class negate_with_arg_type
{
public:
  typedef int result_type;
  typedef int argument_type;

  negate_with_arg_type() {}
  int operator()(int x) const { return -x; }

private:
  negate_with_arg_type(const negate_with_arg_type&);
};

class negate_with_arg1_type
{
public:
  typedef int result_type;
  typedef int arg1_type;

  negate_with_arg1_type() {}
  int operator()(int x) const { return -x; }

private:
  negate_with_arg1_type(const negate_with_arg1_type&);
};

class negate_with_no_arg
{
public:
  typedef int result_type;

  negate_with_no_arg() {}
  int operator()(int x) const { return -x; }

private:
  negate_with_no_arg(const negate_with_no_arg&);
};

class negate_with_arg_type_sig
{
public:
  template<typename T>
  struct sig
  {
    typedef int type;
  };

  typedef int argument_type;

  negate_with_arg_type_sig() {}
  int operator()(int x) const { return -x; }

private:
  negate_with_arg_type_sig(const negate_with_arg_type_sig&);
};

class negate_with_arg1_type_sig
{
public:
  template<typename T>
  struct sig
  {
    typedef int type;
  };

  typedef int arg1_type;

  negate_with_arg1_type_sig() {}
  int operator()(int x) const { return -x; }

private:
  negate_with_arg1_type_sig(const negate_with_arg1_type_sig&);
};

class negate_with_no_arg_sig
{
public:
  template<typename T>
  struct sig
  {
    typedef int type;
  };

  negate_with_no_arg_sig() {}
  int operator()(int x) const { return -x; }

private:
  negate_with_no_arg_sig(const negate_with_no_arg_sig&);
};

class negate_with_arg_type_nrt
{
public:
  typedef int argument_type;

  negate_with_arg_type_nrt() {}
  int operator()(int x) const { return -x; }

private:
  negate_with_arg_type_nrt(const negate_with_arg_type_nrt&);
};

class negate_with_arg1_type_nrt
{
public:
  typedef int arg1_type;

  negate_with_arg1_type_nrt() {}
  int operator()(int x) const { return -x; }

private:
  negate_with_arg1_type_nrt(const negate_with_arg1_type_nrt&);
};

class negate_with_no_arg_nrt
{
public:
  negate_with_no_arg_nrt() {}
  int operator()(int x) const { return -x; }

private:
  negate_with_no_arg_nrt(const negate_with_no_arg_nrt&);
};

template<typename F>
void check_negate(F f)
{
  assert(f(5) == -5);
}

template<typename F>
void check_negate_deduced(F f)
{
  int x = 5;
  assert(f(x) == -x);
}

class add_with_fs_arg_types
{
public:
  typedef int result_type;
  typedef int first_argument_type;
  typedef int second_argument_type;

  add_with_fs_arg_types() {}
  int operator()(int x, int y) const { return x + y; }

private:
  add_with_fs_arg_types(const add_with_fs_arg_types&);
};

class add_with_argN_types
{
public:
  typedef int result_type;
  typedef int arg1_type;
  typedef int arg2_type;

  add_with_argN_types() {}
  int operator()(int x, int y) const { return x + y; }

private:
  add_with_argN_types(const add_with_argN_types&);
};

class add_with_no_args
{
public:
  typedef int result_type;

  add_with_no_args() {}
  int operator()(int x, int y) const { return x + y; }

private:
  add_with_no_args(const add_with_no_args&);
};

class add_with_fs_arg_types_sig
{
public:
  template<typename T1, typename T2> struct sig { typedef int type; };
  typedef int first_argument_type;
  typedef int second_argument_type;

  add_with_fs_arg_types_sig() {}
  int operator()(int x, int y) const { return x + y; }

private:
  add_with_fs_arg_types_sig(const add_with_fs_arg_types_sig&);
};

class add_with_argN_types_sig
{
public:
  template<typename T1, typename T2> struct sig { typedef int type; };
  typedef int arg1_type;
  typedef int arg2_type;

  add_with_argN_types_sig() {}
  int operator()(int x, int y) const { return x + y; }

private:
  add_with_argN_types_sig(const add_with_argN_types_sig&);
};

class add_with_no_args_sig
{
public:
  template<typename T1, typename T2> struct sig { typedef int type; };

  add_with_no_args_sig() {}
  int operator()(int x, int y) const { return x + y; }

private:
  add_with_no_args_sig(const add_with_no_args_sig&);
};

class add_with_fs_arg_types_nrt
{
public:
  typedef int first_argument_type;
  typedef int second_argument_type;

  add_with_fs_arg_types_nrt() {}
  int operator()(int x, int y) const { return x + y; }

private:
  add_with_fs_arg_types_nrt(const add_with_fs_arg_types_nrt&);
};

class add_with_argN_types_nrt
{
public:
  typedef int arg1_type;
  typedef int arg2_type;

  add_with_argN_types_nrt() {}
  int operator()(int x, int y) const { return x + y; }

private:
  add_with_argN_types_nrt(const add_with_argN_types_nrt&);
};

class add_with_no_args_nrt
{
public:
  add_with_no_args_nrt() {}
  int operator()(int x, int y) const { return x + y; }

private:
  add_with_no_args_nrt(const add_with_no_args&);
};

template<typename F>
void check_sum(F f)
{
  assert(f(3, 5) == 8);
}

template<typename F>
void check_sum_deduced(F f)
{
  int x = 3;
  int y = 5;
  assert(f(x, y) == x+y);
}

struct zero_negate_add_fs_arg_types
{
public:
  typedef int result_type;
  typedef int argument_type;
  typedef int first_argument_type;
  typedef int second_argument_type;

  zero_negate_add_fs_arg_types() {}
  int operator()() const { return 0; }
  int operator()(int x) const { return -x; }
  int operator()(int x, int y) const { return x+y; }

private:
  zero_negate_add_fs_arg_types(const zero_negate_add_fs_arg_types&);
};

struct zero_negate_add_argN_types
{
public:
  typedef int result_type;
  typedef int arg1_type;
  typedef int arg2_type;

  zero_negate_add_argN_types() {}
  int operator()() const { return 0; }
  int operator()(int x) const { return -x; }
  int operator()(int x, int y) const { return x+y; }

private:
  zero_negate_add_argN_types(const zero_negate_add_argN_types&);
};

struct zero_negate_add_no_arg
{
public:
  typedef int result_type;

  zero_negate_add_no_arg() {}
  int operator()() const { return 0; }
  int operator()(int x) const { return -x; }
  int operator()(int x, int y) const { return x+y; }

private:
  zero_negate_add_no_arg(const zero_negate_add_no_arg&);
};

struct zero_negate_add_fs_arg_types_sig
{
public:
  template<typename T1 = void, typename T2 = void>
  struct sig
  {
    typedef int type;
  };

  typedef int argument_type;
  typedef int first_argument_type;
  typedef int second_argument_type;

  zero_negate_add_fs_arg_types_sig() {}
  int operator()() const { return 0; }
  int operator()(int x) const { return -x; }
  int operator()(int x, int y) const { return x+y; }

private:
  zero_negate_add_fs_arg_types_sig(const zero_negate_add_fs_arg_types_sig&);
};

struct zero_negate_add_argN_types_sig
{
public:
  template<typename T1 = void, typename T2 = void>
  struct sig
  {
    typedef int type;
  };

  typedef int arg1_type;
  typedef int arg2_type;

  zero_negate_add_argN_types_sig() {}
  int operator()() const { return 0; }
  int operator()(int x) const { return -x; }
  int operator()(int x, int y) const { return x+y; }

private:
  zero_negate_add_argN_types_sig(const zero_negate_add_argN_types_sig&);
};

struct zero_negate_add_no_arg_sig
{
public:
  template<typename T1 = void, typename T2 = void>
  struct sig
  {
    typedef int type;
  };

  zero_negate_add_no_arg_sig() {}
  int operator()() const { return 0; }
  int operator()(int x) const { return -x; }
  int operator()(int x, int y) const { return x+y; }

private:
  zero_negate_add_no_arg_sig(const zero_negate_add_no_arg_sig&);
};


struct zero_negate_add_fs_arg_types_nrt
{
public:
  typedef int argument_type;
  typedef int first_argument_type;
  typedef int second_argument_type;

  zero_negate_add_fs_arg_types_nrt() {}
  int operator()() const { return 0; }
  int operator()(int x) const { return -x; }
  int operator()(int x, int y) const { return x+y; }

private:
  zero_negate_add_fs_arg_types_nrt(const zero_negate_add_fs_arg_types_nrt&);
};

struct zero_negate_add_argN_types_nrt
{
public:
  typedef int arg1_type;
  typedef int arg2_type;

  zero_negate_add_argN_types_nrt() {}
  int operator()() const { return 0; }
  int operator()(int x) const { return -x; }
  int operator()(int x, int y) const { return x+y; }

private:
  zero_negate_add_argN_types_nrt(const zero_negate_add_argN_types_nrt&);
};

struct zero_negate_add_no_arg_nrt
{
public:
  zero_negate_add_no_arg_nrt() {}
  int operator()() const { return 0; }
  int operator()(int x) const { return -x; }
  int operator()(int x, int y) const { return x+y; }

private:
  zero_negate_add_no_arg_nrt(const zero_negate_add_no_arg_nrt&);
};

int main()
{
  // Arity zero function objects
  generate_zero gz;
  generate_zero_no_result_type gznrt;
  check_generate_zero(boost::ref(gz));
  check_generate_zero(boost::ref<int>(gznrt));

  // Arity 1 function objects
  negate_with_arg_type na;
  negate_with_arg1_type na1;
  negate_with_no_arg nn;
  negate_with_arg_type_sig nas;
  negate_with_arg1_type_sig na1s;
  negate_with_no_arg_sig nns;
  negate_with_arg_type_nrt nanrt;
  negate_with_arg1_type_nrt na1nrt;
  negate_with_no_arg_nrt nnnrt;
  check_negate(boost::ref(na));
  check_negate(boost::ref(na1));
  check_negate_deduced(boost::ref(nn));
  check_negate(boost::ref(nas));
  check_negate(boost::ref(na1s));
  check_negate_deduced(boost::ref(nns));
  check_negate(boost::ref<int>(nanrt));
  check_negate(boost::ref<int>(na1nrt));
  check_negate_deduced(boost::ref<int>(nnnrt));

  // Arity 2 function objects
  add_with_fs_arg_types aa;
  add_with_fs_arg_types a2;
  add_with_no_args an;
  add_with_fs_arg_types_sig aas;
  add_with_fs_arg_types_sig a2s;
  add_with_no_args_sig ans;
  add_with_fs_arg_types_nrt aanrt;
  add_with_fs_arg_types_nrt a2nrt;
  add_with_no_args_nrt annrt;
  check_sum(boost::ref(aa));
  check_sum(boost::ref(a2));
  check_sum_deduced(boost::ref(an));
  check_sum(boost::ref(aas));
  check_sum(boost::ref(a2s));
  check_sum_deduced(boost::ref(ans));
  check_sum(boost::ref<int>(aanrt));
  check_sum(boost::ref<int>(a2nrt));
  check_sum_deduced(boost::ref<int>(annrt));

  // Arity overloading in function objects
  zero_negate_add_argN_types znaa;
  zero_negate_add_fs_arg_types znafs;
  zero_negate_add_no_arg znana;
  check_generate_zero(boost::ref(znaa));
  check_negate(boost::ref(znaa));
  check_sum(boost::ref(znaa));
  check_generate_zero(boost::ref(znafs));
  check_negate(boost::ref(znafs));
  check_sum(boost::ref(znafs));
  check_generate_zero(boost::ref(znana));
  check_negate_deduced(boost::ref(znana));
  check_sum_deduced(boost::ref(znana));

  zero_negate_add_argN_types_sig znaas;
  zero_negate_add_fs_arg_types_sig znafss;
  zero_negate_add_no_arg_sig znanas;
  //  check_generate_zero(boost::ref(znaas));
  check_negate(boost::ref(znaas));
  check_sum(boost::ref(znaas));
  //  check_generate_zero(boost::ref(znafss));
  check_negate(boost::ref(znafss));
  check_sum(boost::ref(znafss));
  //  check_generate_zero(boost::ref(znanas));
  check_negate_deduced(boost::ref(znanas));
  check_sum_deduced(boost::ref(znanas));

  zero_negate_add_argN_types znaanrt;
  zero_negate_add_fs_arg_types znafsnrt;
  zero_negate_add_no_arg znananrt;
  check_generate_zero(boost::ref<int>(znaanrt));
  check_negate(boost::ref<int>(znaanrt));
  check_sum(boost::ref(znaanrt));
  check_generate_zero(boost::ref<int>(znafsnrt));
  check_negate(boost::ref<int>(znafsnrt));
  check_sum(boost::ref<int>(znafsnrt));
  check_generate_zero(boost::ref<int>(znananrt));
  check_negate_deduced(boost::ref<int>(znananrt));
  check_sum_deduced(boost::ref<int>(znananrt));

  return 0;
}
