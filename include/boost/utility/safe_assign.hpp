//-----------------------------------------------------------------------------
// boost utility/safe_assign.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_UTILITY_SAFE_ASSIGN_HPP
#define BOOST_UTILITY_SAFE_ASSIGN_HPP

#include "boost/utility/addressof.hpp"
#include "boost/type_traits/alignment_of.hpp"

// The following are new/in-progress headers or fixes to existing headers:
#include "boost/aligned_storage.hpp"

namespace boost {

// safe_assign
//   Generic assignment w/ strong exception-safety guarantee.
//
// Rationale:
//   safe_assign is more efficient than safe_swap for assignment.
//
// !WARNING!
//   safe_assign CANNOT be safely used in the general case if the
//   destination argument's data members may be accessed
//   concurrently.
template <typename Destination, typename Source>
Destination& safe_assign(Destination& dest, const Source& src)
{
	typedef aligned_storage<
		  sizeof(Destination)
		, alignment_of<Destination>::value
		> aligned_storage_t;

	// Cache (for convienence) the addresses of lhs and rhs using addressof:
	Destination* address = boost::addressof(dest);

	// Now backup dest...
	aligned_storage_t backup;
	backup.memcpy_in(address);

	try
	{
		// ...and attempt to overwrite dest with a (possible converted) copy of src:
		new(address) Destination(src);
	}
	catch (...)
	{
		// In case of failure, restore dest using backup, and rethrow:
		backup.memcpy_out(address);
		throw;
	}

	// Since the copy succeeded, copy dest's new value off to the side,
	// and (temporarily) restore dest's old value using backup:
	aligned_storage_t new_value;
	new_value.memcpy_in(address);

	// Now destroy dest (which currently contains its _old_ value)...
	dest.~Destination();

	// ...and finally copy in its new (assigned) value:
	new_value.memcpy_out(address);
	return dest;
}

} // namespace boost

#endif // BOOST_UTILITY_SAFE_ASSIGN_HPP
