// Copyright (C) 2002-2003 Doug Gregor (gregod@cs.rpi.edu)
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies.
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty,
// and with no claim as to its suitability for any purpose.

// For more information, see http://www.boost.org

#include <boost/tribool.hpp>
#include <cassert>
#include <iostream>

BOOST_TRIBOOL_THIRD_STATE(maybe)

int main()
{
  using namespace boost;

  tribool x; // false
  tribool y(true); // true
  tribool z(maybe); // maybe

  assert(!x);
  assert(x == false);
  assert(false == x);
  assert(x != true);
  assert(true != x);
  assert(maybe(x == maybe));
  assert(maybe(maybe == x));
  assert(maybe(x != maybe));
  assert(maybe(maybe != x));
  assert(x == x);
  assert(!(x != x));
  assert(!(x && true));
  assert(!(true && x));
  assert(x || true);
  assert(true || x);

  assert(y);
  assert(y == true);
  assert(true == y);
  assert(y != false);
  assert(false != y);
  assert(maybe(y == maybe));
  assert(maybe(maybe == y));
  assert(maybe(y != maybe));
  assert(maybe(maybe != y));
  assert(y == y);
  assert(!(y != y));

  assert(maybe(z || !z));
  assert(maybe(z == true));
  assert(maybe(true == z));
  assert(maybe(z == false));
  assert(maybe(false == z));
  assert(maybe(z == maybe));
  assert(maybe(maybe == z));
  assert(maybe(z != maybe));
  assert(maybe(maybe != z));
  assert(maybe(z == z));
  assert(maybe(z != z));

  assert(!(x == y));
  assert(x != y);
  assert(maybe(x == z));
  assert(maybe(x != z));
  assert(maybe(y == z));
  assert(maybe(y != z));

  assert(!(x && y));
  assert(x || y);
  assert(!(x && z));
  assert(maybe(y && z));
  assert(maybe(z && z));
  assert(maybe(z || z));
  assert(maybe(x || z));
  assert(y || z);

  assert(maybe(y && maybe));
  assert(maybe(maybe && y));
  assert(!(x && maybe));
  assert(!(maybe && x));

  assert(maybe || y);
  assert(y || maybe);
  assert(maybe(x || maybe));
  assert(maybe(maybe || x));

  // Test the if (z) ... else (!z) ... else ... idiom
  if (z) {
    assert(false);
  }
  else if (!z) {
    assert(false);
  }
  else {
    assert(true);
  }

  z = true;
  if (z) {
    assert(true);
  }
  else if (!z) {
    assert(false);
  }
  else {
    assert(false);
  }

  z = false;
  if (z) {
    assert(false);
  }
  else if (!z) {
    assert(true);
  }
  else {
    assert(false);
  }

  std::cout << "no errors detected\n";
  return 0;
}
