+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 The Boost.NamedParams Library Reference |(logo)|__ INCOMPLETE
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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

.. contents::

concept ``keyword-expression``
------------------------------

.. parsed-literal::

    template<class Tag>
    *bound-argument* operator[](*keyword-expression*, keyword<Tag>)

**Requires**
    An argument tagged with ``Tag`` must be bound in the object.

**Returns**
    A reference to the bound argument tagged with ``Tag``.

**Throws**
    Nothrow.

**Complexity**
    O(1) runtime. O(1) compile time on good compilers. O(N), where N is
    the number of bound arguments, on old compilers such as VC6.


.. parsed-literal::

    *bound-argument* operator[](*keyword-expression*, *named-default-expression*)

**Requires**
    Nothing.

**Returns**
    A reference to the bound argument tagged with 
    *named-default-expression*::key_type. If no such argument is bound, the
    default value as given by the *named-default-expression* instance is 
    returned.

**Throws**
    Nothrow.

**Complexity**
    O(1) runtime. O(1) compile time on good compilers. O(N), where N is
    the number of bound arguments, on old compilers such as VC6.


concept ``named-default-expression``
------------------------------------

...


concept ``lazy-default-expression``
-----------------------------------

...

   

class template ``keyword``
--------------------------

.. parsed-literal::

    template<class Tag>
    struct keyword
    {
        template<class T>
        *keyword-expression* operator=(T&) const;

        template<class T>
        *keyword-expression* operator=(T const&) const;

        template<class Default>
        *named-default-expression* operator|(Default&) const;

        template<class Default>
        *named-default-expression* operator|(Default const&) const;

        template<class LazyDefault>
        *unspecified* operator||(LazyDefault const&) const;
    };

Denotes a argument keyword. ``Tag`` is a tag class, typically an incomplete type.

::

    template<class T> operator=(T&)
    template<class T> operator=(T const&)

**Requires**
    Nothing.

**Returns**
    An object that holds a cv reference to ``x``, tagged with the keyword
    ``Tag`` type. If ``T`` is an instance of ``boost::reference_wrapper<U cv>``
    the tagged result will hold a reference to ``U cv``.

**Complexity**
    O(1) compile time and run time.

**Throws**
    Nothrow.

.. parsed-literal::

    template<class Default>
    *named-default-expression* operator|(Default&) const

    template<class Default>
    *named-default-expression* operator|(Default const&) const

**Requires**
    Nothing.

**Returns**
    An object holding a reference to the given default value, tagged
    with ``Tag``.

.. parsed-literal::

    template<class LazyDefault>
    *lazy-default-expression* operator||(LazyDefault const& x) const;

**Requires**
    ``LazyDefault`` is a nullary function object, with the following
    properties:

    ======================== =======================================
    Expression               Result
    ======================== =======================================
    LazyDefault::result_type Must to Copyable
    x()                      Convertible to LazyDefault::result_type
    ======================== =======================================

**Returns**
    ...

.. parsed-literal::

    template<
        class Tag
      , class HasDefault = mpl::true\_
      , class Predicate = mpl::always<mpl::true\_>
    >
    struct named_param
    {
        typedef Tag key_type;
        typedef HasDefault has_default;
        typedef Predicate predicate;
    };

    template<class K0, class K1, ..., class KN>
    struct parameters
    {
        template<class T0, class T1, ..., class TN>
        *keyword-expression* operator()(T0 const&, T1 const&, ..., TN const&) const;
    };

* <K0, ..., KN> is either a tag type, as specified in ``keyword``, or an instantiation of
  ``named_param``.

::

   template<class T0, class T1, ..., class TN> operator()(T0 const&, T1 const&, ..., TN const&) const;

Composes all parameters, named and positional, into an object that can be used to query
for specific keywords.

