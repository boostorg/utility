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

int main()
{
  using namespace boost;

  tribool x; // false
  tribool y(true); // true
  tribool z(indeterminate); // indeterminate

  assert(!x);
  assert(x == false);
  assert(false == x);
  assert(x != true);
  assert(true != x);
  assert(indeterminate(x == indeterminate));
  assert(indeterminate(indeterminate == x));
  assert(indeterminate(x != indeterminate));
  assert(indeterminate(indeterminate != x));
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
  assert(indeterminate(y == indeterminate));
  assert(indeterminate(indeterminate == y));
  assert(indeterminate(y != indeterminate));
  assert(indeterminate(indeterminate != y));
  assert(y == y);
  assert(!(y != y));

  assert(indeterminate(z || !z));
  assert(indeterminate(z == true));
  assert(indeterminate(true == z));
  assert(indeterminate(z == false));
  assert(indeterminate(false == z));
  assert(indeterminate(z == indeterminate));
  assert(indeterminate(indeterminate == z));
  assert(indeterminate(z != indeterminate));
  assert(indeterminate(indeterminate != z));
  assert(indeterminate(z == z));
  assert(indeterminate(z != z));

  assert(!(x == y));
  assert(x != y);
  assert(indeterminate(x == z));
  assert(indeterminate(x != z));
  assert(indeterminate(y == z));
  assert(indeterminate(y != z));

  assert(!(x && y));
  assert(x || y);
  assert(!(x && z));
  assert(indeterminate(y && z));
  assert(indeterminate(z && z));
  assert(indeterminate(z || z));
  assert(indeterminate(x || z));
  assert(y || z);

  assert(indeterminate(y && indeterminate));
  assert(indeterminate(indeterminate && y));
  assert(!(x && indeterminate));
  assert(!(indeterminate && x));

  assert(indeterminate || y);
  assert(y || indeterminate);
  assert(indeterminate(x || indeterminate));
  assert(indeterminate(indeterminate || x));

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
