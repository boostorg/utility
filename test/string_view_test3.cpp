#include <boost/config.hpp>
#ifndef BOOST_NO_CXX17_HDR_STRING_VIEW
#include <boost/utility/string_view.hpp>
#include <boost/core/lightweight_test.hpp>

// Test support for implicit conversion to std::basic_string_view
void test_implicit_string_view_cast()
{
    typedef char char_type;
    typedef boost::basic_string_view< char_type > string_view_type;
    typedef std::basic_string_view<char_type, std::char_traits<char_type>> std_string_view;

    char const* input = "static string";

    string_view_type boost_view(input);

    std_string_view std_view = boost_view;

    BOOST_TEST(std_view.data() == boost_view.data());
    BOOST_TEST(std_view.size() == boost_view.size());
}

// Test support for construction and assignment from std::basic_string_view
void test_string_view_construction()
{
    typedef char char_type;
    typedef boost::basic_string_view< char_type > string_view_type;
    typedef std::basic_string_view<char_type, std::char_traits<char_type>> std_string_view;

    char const* input = "static string";

    {
      std_string_view std_view = input;

      string_view_type boost_view(std_view);

      BOOST_TEST(std_view.data() == boost_view.data());
      BOOST_TEST(std_view.size() == boost_view.size());
    }

    {
      std_string_view std_view = input;

      string_view_type boost_view;
      boost_view = std_view;

      BOOST_TEST(std_view.data() == boost_view.data());
      BOOST_TEST(std_view.size() == boost_view.size());
    }
}

int main()
{
  test_implicit_string_view_cast();
  return boost::report_errors();
}
#else
int main() {}
#endif
