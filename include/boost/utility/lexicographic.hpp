// Copyright (c) 2003 Jan Langer
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/utility
 
#ifndef BOOST_UTILITY_LEXICOGRAPHIC_HPP
#define BOOST_UTILITY_LEXICOGRAPHIC_HPP

namespace boost
{

  class lexicographic
  {
    public:
      enum result_type { minus = -1, equivalent = 0, plus = +1 };
    
    private:
      typedef void (lexicographic::*unspecified_bool_type) ();
      void safe_bool_conversion () {}
        
      template <typename T1, typename T2>
      result_type do_compare (T1 const &a, T2 const &b) const
      {
          if (a < b)
              return minus;
          else if (b < a)
              return plus;
          else 
              return equivalent;
      }
      template <typename T1, typename T2, typename Cmp>
      result_type do_compare (T1 const &a, T2 const &b, Cmp cmp) const
      {
          if (cmp (a, b))
              return minus;
          else if (cmp (b, a))
              return plus;
          else 
              return equivalent;
      }
      
    public:
      lexicographic () : m_value (equivalent) {}

      template <typename T1, typename T2>
      lexicographic (T1 const &a, T2 const &b) 
          : m_value (do_compare (a, b))
      {}
      template <typename T1, typename T2, typename Cmp>
      lexicographic (T1 const &a, T2 const &b, Cmp cmp) 
          : m_value (do_compare (a, b, cmp))
      {}
    
      template <typename T1, typename T2>
      lexicographic &operator () (T1 const &a, T2 const &b)
      {
          if (m_value == equivalent) 
            m_value = do_compare (a, b);
          return *this;
      }
      template <typename T1, typename T2, typename Cmp>
      lexicographic &operator () (T1 const &a, T2 const &b, Cmp cmp)
      {
          if (m_value == equivalent) 
            m_value = do_compare (a, b, cmp); 
          return *this;
      }

      result_type result () const 
      { 
          return m_value; 
      }
      
      operator unspecified_bool_type () const 
      { 
          return (m_value == minus) 
                      ? &lexicographic::safe_bool_conversion 
                      : 0; 
      }
      
      // somehow only needed old compilers
      bool operator ! () const
      {
          return m_value != minus;
      }

    private:
      result_type m_value;
  };

  bool operator == (lexicographic l1, lexicographic l2)
  {
      return l1.result () == l2.result ();
  }
  
  bool operator != (lexicographic l1, lexicographic l2)
  {
      return l1.result () != l2.result ();
  }
  
} // namespace boost

#endif // BOOST_UTILITY_LEXICOGRAPHIC_HPP
