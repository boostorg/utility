//-----------------------------------------------------------------------------
// boost utility/safe_swap.hpp header file
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

#ifndef BOOST_UTILITY_SAFE_SWAP_HPP
#define BOOST_UTILITY_SAFE_SWAP_HPP

#include "boost/utility/addressof.hpp"
#include "boost/type_traits/alignment_of.hpp"

// The following are new/in-progress headers or fixes to existing headers:
#include "boost/aligned_storage.hpp"

namespace boost {

// safe_swap
//   Generic swap w/ strong exception-safety guarantee.
//
// !WARNING!
//   safe_swap CANNOT be safely used in the general case if either
//   argument's data members may be accessed concurrently.
template <typename T>
void safe_swap(T& lhs, T& rhs)
{
	typedef aligned_storage<
		  sizeof(T)
		, alignment_of<T>::value
		> aligned_storage_t;

	// Cache (for convienence) the addresses of lhs and rhs using addressof:
	T* lhs_address = boost::addressof(lhs);
	T* rhs_address = boost::addressof(rhs);

	// Now backup lhs...
	aligned_storage_t lhs_backup;
	lhs_backup.memcpy_in(lhs_address);

	try
	{
		// ...and attempt to overwrite lhs with a copy of rhs:
		new(lhs_address) T(rhs);
	}
	catch (...)
	{
		// In event of error, restore lhs using backup, and rethrow:
		lhs_backup.memcpy_out(lhs_address);
		throw;
	}

	// Since the copy succeeded, copy lhs's new value off to the side,
	// and (temporarily) restore lhs's old value using backup:
	aligned_storage_t lhs_new;
	lhs_new.memcpy_in(lhs_address);
	lhs_backup.memcpy_out(lhs_address);

	// Now backup rhs...
	aligned_storage_t rhs_backup;
	rhs_backup.memcpy_in(rhs_address);

	try
	{
		// ...and attempt to overwrite rhs with a copy of lhs:
		new(rhs_address) T(lhs);
	}
	catch (...)
	{
		// In event of error, restore rhs using backup, and rethrow:
		rhs_backup.memcpy_out(rhs_address);
		throw;
	}

	// Since the copy succeeded, copy rhs's new value off to the side,
	// and (temporarily) restore rhs's old value using backup:
	aligned_storage_t rhs_new;
	rhs_new.memcpy_in(rhs_address);
	rhs_backup.memcpy_out(rhs_address);
	
	// Now destroy lhs and rhs (which currently contain their _old_ values)...
	lhs.~T();
	rhs.~T();

	// ...and finally copy in their new (swapped) values:
	lhs_new.memcpy_out(lhs_address);
	rhs_new.memcpy_out(rhs_address);
}

} // namespace boost

#endif // BOOST_UTILITY_SAFE_SWAP_HPP
