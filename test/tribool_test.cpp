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
  assert(!(x == indeterminate));
  assert(!(indeterminate == x));
  assert(x != indeterminate);
  assert(indeterminate != x);
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
  assert(!(y == indeterminate));
  assert(!(indeterminate == y));
  assert(y != indeterminate);
  assert(indeterminate != y);
  assert(y == y);
  assert(!(y != y));

  assert((z || !z) == indeterminate);
  assert(!(z == true));
  assert(!(true == z));
  assert(!(z == false));
  assert(!(false == z));
  assert(z == indeterminate);
  assert(indeterminate == z);
  assert(!(z != indeterminate));
  assert(!(indeterminate != z));
  assert((z == z) == indeterminate);
  assert((z != z) == indeterminate);

  assert(!(x == y));
  assert(x != y);
  assert((x == z) == indeterminate);
  assert((x != z) == indeterminate);
  assert((y == z) == indeterminate);
  assert((y != z) == indeterminate);

  assert(!(x && y));
  assert(x || y);
  assert(!(x && z));
  assert((y && z) == indeterminate);
  assert((z && z) == indeterminate);
  assert((z || z) == indeterminate);
  assert(x || z == indeterminate);
  assert(y || z);

  std::cout << "no errors detected\n";
  return 0;
}
