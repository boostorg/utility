++++++++++++++++++++++++++++++++++++++++++
 The Boost.NamedParams Library |(logo)|__
++++++++++++++++++++++++++++++++++++++++++

.. |(logo)| image:: ../../../c++boost.gif
   :alt: Boost
   :class: boost-logo

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

     #include <boost/named_params.hpp>

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

     struct foo_parameters
       : boost::parameters<
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
         foo_impl(foo_parameters()());
     }

     template<class A0>
     void foo(const A0& a0)
     {
         foo_impl(foo_parameters()(a0));
     }

     template<class A0, class A1>
     void foo(const A0& a0, const A1& a1)
     {
         foo_impl(foo_parameters()(a0, a1));
     }

Defining the implementation function
------------------------------------

::

     template<class Params>
     void foo_impl(const Params& params)
     {
         std::cout << params[name] << " = " << params[value] << "\n";
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
     void foo_impl(const Params& params)
     {
         std::cout
             << params[name | "unnamed"] << " = "
             << params[value | 0] << "\n";
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

Limitations of the Approach
===========================

Because the keywords' ``operator=`` returns a temporary, and
temporaries cannot be bound to non-``const`` reference parameters,
our forwarding functions need to take their arguments by ``const``
reference [#forwarding]_. As a result, an argument which is bound
to a keyword with ``operator=`` can be transparently passed by
non-const reference, but positional arguments are always passed by
``const`` reference unless we use the `Boost.Ref`_ library to
indicate otherwise::

    #include <boost/ref.hpp>

    float x;
    foo(value = x);     // held type is float&
    foo(x);             // held type is float const&, need help!
    foo(boost::ref(x)); // held type is float&

.. _`Boost.Ref`: ../../bind/ref.hpp


Instances of ``boost::reference_wrapper<>`` generated by
``boost::ref`` will be unwrapped automatically by the library.

Controlling Overload Resolution
===============================

The parameters of our templated forwarding functions are completely
general; in fact, they're a perfect match for any argument type
whatsoever.  The problems with exposing such general function
templates have been the subject of much discussion; especially in
the presence of `unqualified calls`__.  Probably the safest thing
to do is to isolate the forwarding functions in a namespace
containing no types [#using]_, but often we'd *like* our functions
to play nicely with argument-dependent lookup and other function
overloads.  In that case, it's neccessary to somehow remove the
functions from the overload set when the passed argument types
don't meet their needs.

__ http://anubis.dkuug.dk/jtc1/sc22/wg21/docs/lwg-defects.html#225

This sort of overload control can be accomplished in C++ by taking
advantage of SFINAE_ (Substitution Failure Is Not An Error). If
type substitution during the instantiation of a function template
results in an invalid type, no compilation error is emitted;
instead the overload is removed from the overload set. By producing
an invalid type in the function signature depending on the result
of some condition, whether or not an overload is considered during
overload resolution can be controlled.  The technique is formalized
in the |enable_if| utility.

The named parameters library provides built-in SFINAE support
through the following class template::

     template<
         class KeywordTag
       , class HasDefaultValue // mpl::true_ or mpl::false_
       , class Predicate
     >
     struct named_param;

The key parameter, ``Predicate`` shall be a unary MPL lambda
expression or `Metafunction Class`_ that, when applied to the
actual type the argument, indicates whether that argument type
meets the function's requirements for that parameter position.

.. _`Metafunction Class`: ../../mpl/doc/ref/Metafunction_Class.html

.. _SFINAE: http://www.semantics.org/once_weakly/w02_SFINAE.pdf

.. |enable_if| replace:: ``enable_if``
.. _enable_if: ../enable_if.html

For example, let's say we want to restrict our ``foo()`` so that
the ``name`` parameter must be convertible to ``const char*``.
We'll replace our use of the ``name_t`` tag with a specialization
of ``boost::named_param``:

.. parsed-literal::

     struct foo_parameters
       : boost::parameters<
             **boost::named_param<
                 name_t
               , mpl::false\_
               , is_convertible<mpl::\_, const char\*>
             >**
           , value_t
         >
     {};

Now we can add an additional optional argument to each of our
``foo`` overloads

.. parsed-literal::

     template<class A0>
     void foo(
         const A0& a0
       , **foo_parameters::restrict<A0>::type x = foo_parameters()**
     )
     {
         foo_impl(x(a0));
     }

     template<class A0, class A1>
     void foo(
         const A0& a0, const A1& a1
       , **foo_parameters::restrict<A0,A1>::type x = foo_parameters()**
     )
     {
         foo_impl(x(a0, a1));
     }

These additional parameters are not intended to be used directly
by callers; they merely trigger SFINAE by becoming illegal types
when the ``name`` argument is not convertible to ``const char*``.

Lazy Evaluation of Defaults
===========================

If computing an argument's default value is expensive, it's best
avoided when the argument is supplied by the user. In that case,
the default value can be lazily evaluated using the following
syntax:

.. parsed-literal::

    params[keyword **|| nullary_function**];

``nullary_function`` must be a function object that is callable
without arguments, and that indicates its return type via a nested
``result_type``.  Boost.Bind can be used to produce an appropriate
function object from a regular function pointer::

  // expensive default computation function
  float default_span(float x, float theta);

  // implementation of bar()
  template <class Params>
  void bar_impl(Params const& params)
  {
      // Extract arguments
      float x_ = params[x];
      float theta_ = params[theta | pi];
      float span = params[span || boost::bind(default_span, x_, theta_)];
      ...
  }

Automatic Overload Generation
=============================

To reduce the work needed to write functions with named parameters,
we supply a macro that generates the boilerplate code.

Synopsis::

     BOOST_NAMED_PARAMS_FUN(
         return_type, function_name
       , min_arity, max_arity, parameters_type
     );

To generate all the forwarding functions and the implementation
function for our example, we need only apply
``BOOST_NAMED_PARAMS_FUN`` this way::

     BOOST_NAMED_PARAMS_FUN(void, foo, 0, 2, foo_parameters)
     {
         std::cout
             << p[name | "unnamed"] << " = "
             << p[value | 0] << "\n";
     }

Portability
===========

Boost.NamedParams has been confirmed to work on the following compilers:

   - Microsoft VC6 sp5, VC7 [#norestrict]_
   - Microsoft VC7.1
   - GCC3.3.1 (cygwin), GCC2.95.3 (cygwin), GCC3.2 (mingw)
   - Metrowerks Codewarrior Pro8 and Pro9 (Windows)
   - Intel C++ 5.0,6.0,7.1,8.0 (Windows)
   - Comeau 4.3.3

-----------------------------

.. [#forwarding] One could provide overloads for ``const`` and
   non-``const`` reference versions of each parameter, but that
   would quickly become unmanageable.  It's known as "the
   forwarding problem" and has been described in detail in this
   paper__.  The combinatorial explosion is avoided for the
   parameter of keywords' ``operator=`` because they take only a
   single argument.

   __ http://anubis.dkuug.dk/jtc1/sc22/wg21/docs/papers/2002/n1385.htm


.. [#using] You can always give the illusion that the function
   lives in an outer namespace by applying a *using-declaration*::

      namespace foo_overloads
      {
        // foo declarations here
        void foo() { ... }
        ...
      }
      using foo_overloads::foo;  

.. [#norestrict] Restrictions doesn't work on these compilers because
   of lack of SFINAE support.

