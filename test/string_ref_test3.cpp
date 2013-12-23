/*
 * Copyright Peter A. Bigot 2013.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */

#include <algorithm>
#include <stdexcept>
#include <boost/utility/string_ref.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

using boost::string_ref;
using namespace boost::unit_test;

void test_clear ()
{
  string_ref sv0;
  string_ref svn("1234");

  BOOST_CHECK_NE(sv0.data(), svn.data());
  BOOST_CHECK_NE(sv0.size(), svn.size());
  svn.clear();
  BOOST_CHECK_EQUAL(sv0.data(), svn.data());
  BOOST_CHECK_EQUAL(sv0.size(), svn.size());
}

void test_copy ()
{
  typedef string_ref::size_type szt;
  string_ref sv("123456789A");
  char buffer[4] = { 0 };
  char * const ebuffer = buffer + sizeof(buffer) / sizeof(*buffer);

  BOOST_CHECK_EQUAL(char(), buffer[0]);
  BOOST_CHECK_EQUAL('1', sv[0]);

  BOOST_CHECK_THROW(sv.copy(buffer, sizeof(buffer), string_ref::npos),
                    std::out_of_range);
  BOOST_CHECK_THROW(sv.copy(buffer, sizeof(buffer), sv.size()+1),
                    std::out_of_range);

  string_ref::size_type len = sv.copy(buffer, sizeof(buffer), 8);
  BOOST_CHECK_EQUAL(szt(2), len);
  BOOST_CHECK_EQUAL('9', buffer[0]);

  std::fill(buffer, ebuffer, 0);
  BOOST_CHECK_EQUAL(char(), buffer[0]);

  len = sv.copy(buffer, sizeof(buffer), sv.size());
  BOOST_CHECK_EQUAL(szt(0), len);
  BOOST_CHECK_EQUAL(char(), buffer[0]);

  len = sv.copy(buffer, sizeof(buffer), 0);
  BOOST_CHECK_EQUAL(szt(4), len);
  BOOST_CHECK(std::equal(buffer, ebuffer, sv.data()));
}

void test_find ()
{
  typedef string_ref::size_type szt;
  const char pstr[] = "abcdabc\0abcd";
  string_ref sv(pstr, sizeof(pstr)-1);
  const char * pabc = "abc";
  string_ref svabc(pabc);
  string_ref svnot("not");

  BOOST_CHECK_EQUAL(szt(12), sv.size());
  BOOST_CHECK_EQUAL(sv.find(svabc), 0);
  BOOST_CHECK_EQUAL(sv.find(pabc), 0);
  BOOST_CHECK(sv.find(svnot) == string_ref::npos);
  BOOST_CHECK_EQUAL(sv.find('a'), 0);
  BOOST_CHECK_EQUAL(sv.find('d'), 3);
  BOOST_CHECK_EQUAL(sv.find('\0'), 7);
  BOOST_CHECK(sv.find('n') == string_ref::npos);
}

void test_rfind ()
{
  typedef string_ref::size_type szt;
  const char pstr[] = "abcdabc\0abcd";
  string_ref sv(pstr, sizeof(pstr)-1);
  const char * pabc = "abc";
  string_ref svabc(pabc);
  string_ref svnot("not");

  BOOST_CHECK_EQUAL(szt(12), sv.size());
  BOOST_CHECK_EQUAL(szt(8), sv.rfind(svabc));
  BOOST_CHECK_EQUAL(szt(8), sv.rfind(pabc));
  BOOST_CHECK(sv.rfind(svnot) == string_ref::npos);
  BOOST_CHECK_EQUAL(szt(8), sv.rfind('a'));
  BOOST_CHECK_EQUAL(szt(7), sv.rfind('\0'));
  BOOST_CHECK_EQUAL(szt(11), sv.rfind('d'));
  BOOST_CHECK(sv.rfind('n') == string_ref::npos);
}

void test_ffo ()
{
  typedef string_ref::size_type szt;
  const char pstr[] = "abcdabc\0abcd";
  string_ref sv(pstr, sizeof(pstr)-1);
  const char pabc[] = "abc";
  const char pdef[] = "def";
  const char pghi[] = "ghi";
  string_ref svabc(pabc);
  string_ref svdef(pdef);
  string_ref svghiz(pghi, sizeof(pghi));
  string_ref svnot("not");

  BOOST_CHECK_EQUAL(szt(0), sv.find_first_of(svabc));
  BOOST_CHECK_EQUAL(szt(3), sv.find_first_of(svdef));
  BOOST_CHECK_EQUAL(szt(7), sv.find_first_of(svghiz));
  BOOST_CHECK(sv.find_first_of(svnot) == string_ref::npos);
  BOOST_CHECK_EQUAL(szt(0), sv.find_first_of('a'));
  BOOST_CHECK_EQUAL(szt(3), sv.find_first_of('d'));
  BOOST_CHECK_EQUAL(szt(7), sv.find_first_of('\0'));
  BOOST_CHECK(sv.find_first_of('n') == string_ref::npos);
}

void test_flo ()
{
  typedef string_ref::size_type szt;
  const char pstr[] = "abcdabc\0abcd";
  string_ref sv(pstr, sizeof(pstr)-1);
  const char pabc[] = "abc";
  const char pdef[] = "def";
  const char pghi[] = "ghi";
  string_ref svabc(pabc);
  string_ref svdef(pdef);
  string_ref svghiz(pghi, sizeof(pghi));
  string_ref svnot("not");

  BOOST_CHECK_EQUAL(szt(10), sv.find_last_of(svabc));
  BOOST_CHECK_EQUAL(szt(11), sv.find_last_of(svdef));
  BOOST_CHECK_EQUAL(szt(7), sv.find_last_of(svghiz));
  BOOST_CHECK(sv.find_last_of(svnot) == string_ref::npos);
  BOOST_CHECK_EQUAL(szt(8), sv.find_last_of('a'));
  BOOST_CHECK_EQUAL(szt(11), sv.find_last_of('d'));
  BOOST_CHECK_EQUAL(szt(7), sv.find_last_of('\0'));
  BOOST_CHECK(sv.find_last_of('n') == string_ref::npos);
}

void test_ffno ()
{
  typedef string_ref::size_type szt;
  const char pstr[] = "abcdabc\0abcd";
  string_ref sv(pstr, sizeof(pstr)-1);
  const char pabc[] = "abc";
  const char pabcd[] = "abcd";
  const char pdef[] = "def";
  const char pghi[] = "ghi";
  string_ref svabc(pabc);
  string_ref svabcd(pabcd);
  string_ref svabcdz(pabcd, sizeof(pabcd));
  string_ref svdef(pdef);
  string_ref svghi(pghi);

  BOOST_CHECK_EQUAL(szt(3), sv.find_first_not_of(svabc));
  BOOST_CHECK_EQUAL(szt(7), sv.find_first_not_of(svabcd));
  BOOST_CHECK(string_ref::npos == sv.find_first_not_of(svabcdz));
  BOOST_CHECK_EQUAL(szt(1), sv.find_first_not_of('a'));
  BOOST_CHECK_EQUAL(szt(0), sv.find_first_not_of('b'));
}

void test_flno ()
{
  typedef string_ref::size_type szt;
  const char pstr[] = "abcdabc\0abcd";
  string_ref sv(pstr, sizeof(pstr)-1);
  const char pabc[] = "abc";
  const char pabcd[] = "abcd";
  const char pdef[] = "def";
  const char pghi[] = "ghi";
  string_ref svabcz(pabc, sizeof(pabc));
  string_ref svabcd(pabcd);
  string_ref svabcdz(pabcd, sizeof(pabcd));
  string_ref svdef(pdef);
  string_ref svghi(pghi);

  BOOST_CHECK_EQUAL(szt(11), sv.find_last_not_of(svabcz));
  BOOST_CHECK_EQUAL(szt(10), sv.find_last_not_of(svdef));
  BOOST_CHECK_EQUAL(szt(7), sv.find_last_not_of(svabcd));
  BOOST_CHECK(string_ref::npos == sv.find_last_not_of(svabcdz));
  BOOST_CHECK_EQUAL(szt(11), sv.find_last_not_of('c'));
  BOOST_CHECK_EQUAL(szt(10), sv.find_last_not_of('d'));
  BOOST_CHECK_EQUAL(szt(3), svabcz.find_last_not_of('d'));
  BOOST_CHECK_EQUAL(szt(2), svabcz.find_last_not_of('\0'));
}


BOOST_AUTO_TEST_CASE( test_main )
{
  test_clear();
  test_copy();
  test_find();
  test_rfind();
  test_ffo();
  test_flo();
  test_flno();
}
