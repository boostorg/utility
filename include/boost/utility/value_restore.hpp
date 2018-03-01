#ifndef BOOST_UTILITY_VALUE_RESTORE_HPP
#define BOOST_UTILITY_VALUE_RESTORE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// value_restore.hpp:

// (C) Copyright 2003-4 Pavel Vozenilek and Robert Ramey - http://www.rrsd.com.
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/utility for updates, documentation, and revision history.

// Inspired by Daryle Walker's iostate_saver concept.  This saves the original
// value of a variable when a value_restore is constructed and restores
// upon destruction.  Useful for being sure that state is restored to
// variables upon exit from scope.


#include <boost/config.hpp>
#ifndef BOOST_NO_EXCEPTIONS
    #include <exception>
#endif
#ifndef BOOST_NO_CXX11_SMART_PTR
    #include <boost/move/unique_ptr.hpp>
#else
    #include <memory>
#endif

#include <boost/call_traits.hpp>
#include <boost/noncopyable.hpp>
#include <boost/type_traits/has_nothrow_copy.hpp>
#include <boost/core/no_exceptions_support.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

namespace boost {

template<class T>
// T requirements:
//  - POD or object semantic (cannot be reference, function, ...)
//  - copy constructor
//  - operator = (no-throw one preferred)
class value_restore : private boost::noncopyable
{
private:
    const T previous_value;
    T & previous_ref;

    struct restore {
        static void invoke(T & previous_ref, const T & previous_value){
            previous_ref = previous_value; // won't throw
        }
    };

    struct restore_with_exception {
        static void invoke(T & previous_ref, const T & previous_value){
            BOOST_TRY{
                previous_ref = previous_value;
            } 
            BOOST_CATCH(::std::exception &) { 
                // we must ignore it - we are in destructor
            }
            BOOST_CATCH_END
        }
    };

public:
    value_restore(
        T & object
    ) : 
        previous_value(object),
        previous_ref(object) 
    {}
    
    void operator() () {
        #ifndef BOOST_NO_EXCEPTIONS
            typedef typename mpl::eval_if<
                has_nothrow_copy< T >,
                mpl::identity<restore>,
                mpl::identity<restore_with_exception>
            >::type typex;
            typex::invoke(previous_ref, previous_value);
        #else
            previous_ref = previous_value;
        #endif
    }

}; // value_restore<>

template <typename T>
value_restore<T> make_value_restore(T & object) {
    value_restore<T> value(object);
    return value;
}

#ifndef BOOST_NO_CXX11_TEMPLATE_ALIASES

    #ifndef BOOST_NO_CXX11_SMART_PTR
        template <typename T>
        using value_restore_ptr = std::unique_ptr<T, value_restore<T> >;
    #else
        template <typename T>
        using value_restore_ptr = boost::movelib::unique_ptr<T, value_restore<T> >;
    #endif

    template <typename T>
    value_restore_ptr<T> make_value_restore_ptr(T & object) {
        value_restore_ptr<T> ptr(&object, value_restore<T>(object));
        return ptr;
    }

#endif

} // boost

#endif //BOOST_UTILITY_VALUE_RESTORE_HPP
