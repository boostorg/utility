#ifndef UTILITY_TRACE_SCOPE_HPP
#define UTILITY_TRACE_SCOPE_HPP
#include "boost/io/filters/mout.hpp"
#include <string>
namespace utility
{
  class
trace_scope
  //Purpose:
  //  Indicate on cout when a scope entered and exited.
  {
  public:
    trace_scope(char const* a_scopeName)
      : m_scopeName(a_scopeName)
      { init()
      ;}
    trace_scope(std::string const& a_scopeName)
      : m_scopeName(a_scopeName)
      { init()
      ;}
    ~trace_scope(void)
      {
      ; --mout()
      ; std::string l_context("===>Exit:")
      ; mout()<<l_context<<m_scopeName<<std::endl
      ;}
  private:
      void
    init(void)
      { 
      ; std::string l_context("===>Enter:")
      ; mout()<<l_context<<m_scopeName<<std::endl
      ; ++mout()
      ;}
      std::string
    m_scopeName
      ;
  };//end trace_scope class
}//exit utility namespace
#endif
