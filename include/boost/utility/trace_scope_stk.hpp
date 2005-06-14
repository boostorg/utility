#ifndef UTILITY_TRACE_SCOPE_STK_HPP
#define UTILITY_TRACE_SCOPE_STK_HPP
#include <vector>
#include "boost/utility/trace_scope.hpp"
namespace utility
{
struct trace_scope_stk
  : public trace_scope
  //Purpose:
  //  Simply keeps a stack variables of
  //  type trace_scope_stk
  {
  public:
        static
      std::string const&
    top_name(void)
      { return c_scope_names.back()
      ;}
        static
      std::string const
    top_result(bool result_bool)
      { std::string  result_string(c_scope_names.back())
      ; result_string += std::string(":result=")
      ; result_string += c_test_results[result_bool]
      ; return result_string
      ;}
    trace_scope_stk(std::string const& a_name)
      : trace_scope(a_name)
      { c_scope_names.push_back(a_name)
      ;}
    ~trace_scope_stk(void)
      { c_scope_names.pop_back()
      ;}
  private:
        static
      std::vector<std::string>
    c_scope_names
      ;
        static
      std::string const
    c_test_results[2]
      ;
  };
}//exit utility namespace
#endif

