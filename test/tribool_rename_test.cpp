// Copyright (C) 2002 Doug Gregor (gregod@cs.rpi.edu)
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
  assert(!(x == maybe));
  assert(!(maybe == x));
  assert(x != maybe);
  assert(maybe != x);
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
  assert(!(y == maybe));
  assert(!(maybe == y));
  assert(y != maybe);
  assert(maybe != y);
  assert(y == y);
  assert(!(y != y));

  assert((z || !z) == maybe);
  assert(maybe(z == true));
  assert(maybe(true == z));
  assert(maybe(z == false));
  assert(maybe(false == z));
  assert(z == maybe);
  assert(maybe == z);
  assert(!(z != maybe));
  assert(!(maybe != z));
  assert((z == z) == maybe);
  assert((z != z) == maybe);

  assert(!(x == y));
  assert(x != y);
  assert((x == z) == maybe);
  assert((x != z) == maybe);
  assert((y == z) == maybe);
  assert((y != z) == maybe);

  assert(!(x && y));
  assert(x || y);
  assert(!(x && z));
  assert((y && z) == maybe);
  assert((z && z) == maybe);
  assert(maybe(z || z));
  assert(x || z == maybe);
  assert(y || z);

  assert(maybe(y && maybe));
  assert(maybe(maybe && y));
  assert(!(x && maybe));
  assert(!(maybe && x));

  assert(maybe || y);
  assert(y || maybe);
  assert(maybe(x || maybe));
  assert(maybe(maybe || x));

  if (z) {
    assert(false);
  }
  else if (!z) {
    assert(false);
  }
  else {
    assert(true);
  }

  std::cout << "no errors detected\n";
  return 0;
}
