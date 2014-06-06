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

  BOOST_CHECK(NULL != sv0.data());
  BOOST_CHECK_EQUAL(0, sv0.size());
  BOOST_CHECK_NE(sv0.data(), svn.data());
  BOOST_CHECK_NE(sv0.size(), svn.size());
  svn.clear();
  BOOST_CHECK(NULL != svn.data());
  BOOST_CHECK_EQUAL(sv0.size(), svn.size());
  // Mandated by implementation not specification:
  BOOST_CHECK_NE(static_cast<const void*>(sv0.data()),
                 static_cast<const void*>(svn.data()));
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
  BOOST_CHECK_EQUAL(sv.find(svabc, 1), 4);
  BOOST_CHECK_EQUAL(sv.find(svabc, 5), 8);
  BOOST_CHECK(sv.find(svnot) == string_ref::npos);
  BOOST_CHECK(sv.find(svabc, 9) == string_ref::npos);
  BOOST_CHECK_EQUAL(sv.find(pabc), 0);
  BOOST_CHECK_EQUAL(sv.find(pabc, 1), 4);
  BOOST_CHECK_EQUAL(sv.find(pabc, 5), 8);
  BOOST_CHECK(sv.find(svnot.data()) == string_ref::npos);
  BOOST_CHECK_EQUAL(sv.find('a'), 0);
  BOOST_CHECK_EQUAL(sv.find('a', 1), 4);
  BOOST_CHECK_EQUAL(sv.find('a', 3), 4);
  BOOST_CHECK_EQUAL(sv.find('a', 4), 4);
  BOOST_CHECK_EQUAL(sv.find('a', 5), 8);
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
  const char * pcd = "cd";
  string_ref svcd(pcd);
  string_ref svnot("not");

  BOOST_CHECK_EQUAL(szt(12), sv.size());
  BOOST_CHECK_EQUAL(szt(8), sv.rfind(svabc));
  BOOST_CHECK_EQUAL(szt(8), sv.rfind(svabc, 9));
  BOOST_CHECK_EQUAL(szt(8), sv.rfind(svabc, 8));
  BOOST_CHECK_EQUAL(szt(4), sv.rfind(svabc, 7));
  BOOST_CHECK_EQUAL(szt(4), sv.rfind(svabc, 5));
  BOOST_CHECK_EQUAL(szt(4), sv.rfind(svabc, 4));
  BOOST_CHECK_EQUAL(szt(0), sv.rfind(svabc, 0));
  BOOST_CHECK_EQUAL(szt(2), sv.rfind(svcd, 5));
  BOOST_CHECK(sv.rfind(svcd, 1) == string_ref::npos);
  BOOST_CHECK_EQUAL(szt(8), sv.rfind(pabc));
  BOOST_CHECK_EQUAL(szt(8), sv.rfind(pabc, 9));
  BOOST_CHECK_EQUAL(szt(8), sv.rfind(pabc, 8));
  BOOST_CHECK_EQUAL(szt(4), sv.rfind(pabc, 7));
  BOOST_CHECK_EQUAL(szt(4), sv.rfind(pabc, 5));
  BOOST_CHECK_EQUAL(szt(4), sv.rfind(pabc, 4));
  BOOST_CHECK_EQUAL(szt(0), sv.rfind(pabc, 0));
  BOOST_CHECK(sv.rfind(svnot) == string_ref::npos);
  BOOST_CHECK_EQUAL(szt(8), sv.rfind('a'));
  BOOST_CHECK_EQUAL(szt(8), sv.rfind('a', 9));
  BOOST_CHECK_EQUAL(szt(8), sv.rfind('a', 8));
  BOOST_CHECK_EQUAL(szt(4), sv.rfind('a', 7));
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
  BOOST_CHECK_EQUAL(szt(4), sv.find_first_of(svabc, 3));
  BOOST_CHECK_EQUAL(szt(4), sv.find_first_of(svabc, 4));
  BOOST_CHECK_EQUAL(szt(5), sv.find_first_of(svabc, 5));
  BOOST_CHECK_EQUAL(szt(3), sv.find_first_of(svdef));
  BOOST_CHECK_EQUAL(szt(3), sv.find_first_of(svdef, 3));
  BOOST_CHECK_EQUAL(szt(11), sv.find_first_of(svdef, 4));
  BOOST_CHECK_EQUAL(szt(7), sv.find_first_of(svghiz));
  BOOST_CHECK(sv.find_first_of(svnot) == string_ref::npos);
  BOOST_CHECK_EQUAL(szt(0), sv.find_first_of('a'));
  BOOST_CHECK_EQUAL(szt(4), sv.find_first_of('a', 1));
  BOOST_CHECK_EQUAL(szt(8), sv.find_first_of('a', 7));
  BOOST_CHECK_EQUAL(szt(8), sv.find_first_of('a', 8));
  BOOST_CHECK(sv.find_first_of('a', 9) == string_ref::npos);
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
  BOOST_CHECK_EQUAL(szt(7), sv.find_last_of(svghiz, 8));
  BOOST_CHECK_EQUAL(szt(7), sv.find_last_of(svghiz, 7));
  BOOST_CHECK(sv.find_last_of(svghiz, 6) == string_ref::npos);
  BOOST_CHECK(sv.find_last_of(svnot) == string_ref::npos);
  BOOST_CHECK_EQUAL(szt(8), sv.find_last_of('a'));
  BOOST_CHECK_EQUAL(szt(11), sv.find_last_of('d'));
  BOOST_CHECK_EQUAL(szt(3), sv.find_last_of('d', 4));
  BOOST_CHECK_EQUAL(szt(3), sv.find_last_of('d', 3));
  BOOST_CHECK(sv.find_last_of('d', 2) == string_ref::npos);
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
  BOOST_CHECK_EQUAL(szt(7), sv.find_first_not_of(svabcd, 6));
  BOOST_CHECK_EQUAL(szt(7), sv.find_first_not_of(svabcd, 7));
  BOOST_CHECK(string_ref::npos == sv.find_first_not_of(svabcd, 8));
  BOOST_CHECK(string_ref::npos == sv.find_first_not_of(svabcdz));
  BOOST_CHECK_EQUAL(szt(1), sv.find_first_not_of('a'));
  BOOST_CHECK_EQUAL(szt(11), sv.find_first_not_of('c', 11));
  BOOST_CHECK(string_ref::npos == sv.find_first_not_of('d', 11));
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
  BOOST_CHECK_EQUAL(szt(7), sv.find_last_not_of(svabcd, 8));
  BOOST_CHECK_EQUAL(szt(7), sv.find_last_not_of(svabcd, 7));
  BOOST_CHECK(string_ref::npos == sv.find_last_not_of(svabcd, 6));
  BOOST_CHECK(string_ref::npos == sv.find_last_not_of(svabcdz));
  BOOST_CHECK_EQUAL(szt(11), sv.find_last_not_of('c'));
  BOOST_CHECK_EQUAL(szt(10), sv.find_last_not_of('d'));
  BOOST_CHECK_EQUAL(szt(4), sv.find_last_not_of('d', 4));
  BOOST_CHECK_EQUAL(szt(2), sv.find_last_not_of('d', 3));
  BOOST_CHECK_EQUAL(szt(0), sv.find_last_not_of('d', 0));
  BOOST_CHECK(string_ref::npos == sv.find_last_not_of('a', 0));
  BOOST_CHECK_EQUAL(szt(3), svabcz.find_last_not_of('d'));
  BOOST_CHECK_EQUAL(szt(2), svabcz.find_last_not_of('\0'));
}

void test_swap ()
{
  const char * const cp1 = "one";
  const char * const cp2 = "not one";
  string_ref sv1(cp1);
  string_ref sv2(cp2);
  BOOST_CHECK_NE(sv1.data(), sv2.data());
  BOOST_CHECK_NE(sv1.size(), sv2.size());
  BOOST_CHECK_EQUAL(sv1.data(), cp1);
  BOOST_CHECK_EQUAL(sv1.size(), 3);
  BOOST_CHECK_EQUAL(sv2.data(), cp2);
  BOOST_CHECK_EQUAL(sv2.size(), 7);
  sv1.swap(sv2);
  BOOST_CHECK_EQUAL(sv2.data(), cp1);
  BOOST_CHECK_EQUAL(sv2.size(), 3);
  BOOST_CHECK_EQUAL(sv1.data(), cp2);
  BOOST_CHECK_EQUAL(sv1.size(), 7);
}

void test_compare ()
{
  typedef string_ref::size_type szt;
  const char pe[] = "abc";
  const char pl[] = "aac";
  const char pg[] = "acc";

  const char str[] = "abcdabc\0abcd";
  string_ref svstr(str, sizeof(str)-1);
  string_ref svstrz(str);
  BOOST_CHECK_EQUAL(szt(13), sizeof(str));
  BOOST_CHECK_EQUAL(szt(12), svstr.size());
  BOOST_CHECK_EQUAL(szt(7), svstrz.size());

  string_ref svpl(pl);
  string_ref svpe(pe);
  string_ref svpg(pg);

  BOOST_CHECK(svstr.compare(svstr) == 0);

  BOOST_CHECK(svstr.compare(svpe) > 0);
  BOOST_CHECK(svpe.compare(svstr) < 0);

  BOOST_CHECK(svstr.compare(0, svpe.size(), svpe) == 0);
  BOOST_CHECK(svstr.compare(0, svpl.size(), svpl) > 0);
  BOOST_CHECK(svstr.compare(0, svpg.size(), svpg) < 0);
  BOOST_CHECK(svstr.compare(0, svpe.size(), pe) == 0);
  BOOST_CHECK(svstr.compare(0, svpl.size(), pl) > 0);
  BOOST_CHECK(svstr.compare(0, svpg.size(), pg) < 0);

  BOOST_CHECK(svstr.substr(4).compare(svpe) > 0);
  BOOST_CHECK(svstr.compare(4, string_ref::npos, svpe) > 0);

  BOOST_CHECK(svpe.compare(svstr.substr(4)) < 0);
  BOOST_CHECK(svpe.compare(svstr.substr(4, 3)) == 0);

  BOOST_CHECK_EQUAL(svstr.data(), svstrz.data());
  BOOST_CHECK(svstr.compare(svstrz) > 0);
  BOOST_CHECK(svstr.compare(str) > 0);
  BOOST_CHECK(svstrz.compare(svstr) < 0);

  BOOST_CHECK(svstr.compare(0, string_ref::npos, svstrz) > 0);
  BOOST_CHECK(svstr.compare(0, string_ref::npos, svstr) == 0);
  BOOST_CHECK(svstr.compare(0, string_ref::npos, svstr.data()) > 0);
  BOOST_CHECK(svstr.compare(0, string_ref::npos, svstr.data(), svstr.size()) == 0);
  BOOST_CHECK(svstr.compare(0, string_ref::npos, svstr.data(), svstr.size()-1) > 0);
  BOOST_CHECK(svstr.compare(0, svstr.size()-1, svstr.data(), svstr.size()) < 0);
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
  test_swap();
  test_compare();
}
