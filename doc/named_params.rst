++++++++++++++++++++++++++++++++++++++++++
 The Boost.NamedParams Library |(logo)|__
++++++++++++++++++++++++++++++++++++++++++

.. |(logo)| image:: ../../../c++boost.gif
   :alt: Boost

__ ../../../index.htm

-------------------------------------


:Authors: David Abrahams, Daniel Wallin
:Contact: dave@boost-consulting.com, dalwan01@student.umu.se
:organizations: `Boost Consulting`_, 
:date: $Date$
:copyright: Copyright David Abrahams, Daniel Wallin 2003. 
:license: Use, modification and distribution is subject to the
          Boost Software License, Version 1.0. (See accompanying
          file LICENSE_1_0.txt or copy at
          http://www.boost.org/LICENSE_1_0.txt)

.. _`Boost Consulting`: http://www.boost-consulting.com
.. _`Open Systems Lab`: http://www.osl.iu.edu
 
.. contents:: Outline
.. section-numbering::


Introduction
============

In C++ function arguments are given meaning by their position in
the parameter list. This protocol is fine when there are few
parameters with default values, but as the number of parameters
grows, so does the inconvenience of passing arguments in the
correct order, especially in the presence of default values:

   * It can become difficult for readers to understand the meaning of
     arguments at the call site::

       window* w = new_window("alert", true, true, false, 77, 65);

   * Since meaning is given by position, we have to choose some
     (often arbitrary) order for parameters with default values,
     making some combinations of defaults unusable::

        window* new_window(
            char const* name, bool border = true
          , bool opaque = true, bool movable = false
          , int width = 100, int height = 100);
      
        const bool movability = true;
        window* w = new_window("alert2", movability); // error!

   * Default values can not depend on the values of other function
     parameters::

        window* new_window(
            char const* name, bool border, ...
          , int width = 100, int heigh = width); // error!

   * Template types can not be deduced from the default values, so
     we have to resort to overloading to provide default values for
     parameters with template type::

        template<class T> void f(T x = 0);

        f(); // error!

This library is an attempt to address the problems outlined above
by associating each parameter with a keyword identifier.  Using
this library, users can identify parameters by name instead of just
argument position::

  window* w = new_window("alert2", movable = movability); // OK!


.. DWA Daniel, we explicitly *don't* need ref() for the case
   described below.  It's only when we want to pass by reference
   without a keyword that we need it.

   You also can't start talking about forwarding functions without
   introducing them first!

   The tutorial has to come before all the nasty details below.
   I'm going to comment on that and leave the next stuff alone

Tutorial 
========

.. DWA you need some set-up here describing the problem you're
   going to solve.

This example shows how to wrap a function::

    void foo(char const* name, float value);

to give both parameters names and default values.

Defining the keywords
---------------------

First we define the named parameter keywords. This is done by creating
"tag" types for each keyword, and declaring ``keyword<``\ *tag*\
``>`` objects::

     struct name_t; // tag types
     struct value_t;

     namespace {
       boost::keyword<name_t> name;    // keyword objects
       boost::keyword<value_t> value;
     }

Placing these keyword objects in an unnamed namespace will prevent
link errors when you declare keywords in header files [**Note**:
the tag types should generally *not* be declared in an unnamed
namespace].  We also need to create a keywords list for our
function.  These keywords should be declared in the same order as
their corresponding parameters appear in the function's parameter
list::

     struct foo_keywords
       : boost::keywords<
             name_t
           , value_t
         >
     {};

Defining the forwarding functions
---------------------------------

::

     template<class Params>
     void foo_impl(const Params&);

     void foo()
     {
         foo_impl(foo_keywords());
     }

     template<class A0>
     void foo(const A0& a0)
     {
         foo_impl(foo_keywords(a0));
     }

     template<class A0, class A1>
     void foo(const A0& a0, const A1& a1)
     {
         foo_impl(foo_keywords(a0, a1));
     }

Defining the implementation function
------------------------------------

::

     template<class Params>
     void foo_impl(const Params& parms)
     {
         std::cout << parms[name] << " = " << parms[value] << "\n";
     }

That's it. The user calls the ``foo()`` forwarding functions, with
either positional or named parameters. For instance::

     foo("bar", 3.14f);
     foo(value = 6.28f, "baz")

Should print::

     bar = 3.14
     baz = 6.28

But we still don't have any default values, leaving any of the
parameters out results in a compilation error::

     foo()
     foo("bar")
     foo(value = 3)

All fails.

Fortunatly, adding default values to parameters is easy::

     template<class Params>
     void foo_impl(const Params& parms)
     {
         std::cout
             << parms[name | "unnamed"] << " = "
             << parms[value | 0] << "\n";
     }

We are using ``operator|`` to denote the default value of a named
parameter.

Going back a little to the ``foo()`` call that didn't compile::

     foo()
     foo("bar")
     foo(value = 3)

Now compiles, and prints::

     unnamed = 0
     bar = 0
     unnamed = 3

``reference_wrapper<>``
=======================

Our forwarding functions need to take their parameters by const
reference. This is because we need to be able to pass the temporaries
created from the operator= call. Because of this, passing non-const
references that aren't bound to a keyword isn't possible without some help.

.. DWA What is "something something ?" supposed to mean?
.. It's was suppose to mean "more?"..

::

    float x;
    foo(value = x); // held type is float&
    foo(x); // held type is float const&, need help!
    foo(ref(x)); // held type is float&

Instances of boost::reference_wrapper<> will un unwrapped to it's held
reference type.

SFINAE restrictions
===================

Sometimes it is necessary to restrict the types on which the forwarding
functions can be instantiated. This can be accomplished in C++ by using
SFINAE [#sfinae]_. If type substitution
during the instantiation of a function template results in an invalid
type, no compilation error is emitted; instead the overload is removed
from the overload set. By producing an invalid type in the function
signature depending on the result of some condition, whether or not an
overload is considered during overload resolution can be controlled.
This technique is formalized in the ``enable_if`` pattern [#enable_if]_.

.. [#sfinae] Substitution Failure Is Not An Error.  Some discussion
   of SFINAE goes here.

.. [#enable_if] Some discussion of ``enable_if`` goes here.

  .. more?

.. DWA What about tutorial for your macro?
   Daniel: Under BOOST_NAMED_PARAMS_FUN(), should it be moved?
           or do we need a more verbose tutorial?

Let's say we want to restrict our ``foo()`` so that the ``name``
parameter must be convertible to ``const char*``.

::

     template<
         class Keyword
       , class HasDefaultValue
       , class Predicate
     >
     struct arg;

::

     struct foo_keywords
         : keywords<
               arg<
                   name_t
                 , mpl::false_
                 , is_convertible<mpl::_
                 , const char*>
               >
             , value_t
           >
     {};

::

     template<class A0>
     void foo(const A0& a0
        , foo_keywords::restrict<A0>::type x = foo_keywords())
     {
         foo_impl(x(a0));
     }

     template<class A0, class A1>
     void foo(const A0& a0, const A1& a1
        , foo_keywords::restrict<A0,A1>::type x = foo_keywords())
     {
         foo_impl(x(a0, a1));
     }

``BOOST_NAMED_PARAMS_FUN()``
============================

To reduce the work needed to write functions which has named parameters,
we supply a macro that generates the boilerplate code.

Synopsis::

     BOOST_NAMED_PARAMS(
         return_type, function_name, keywords_type
       , min_arity, max_arity
     );

Applying this to our original example, we get::

     BOOST_NAMED_PARAMS_FUN(void, foo, foo_keywords, 0, 2)
     {
         std::cout
             << parms[name | "unnamed"] << " = "
             << parms[value | 0] << "\n";
     }

