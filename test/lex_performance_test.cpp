// Copyright (c) 2003 Jan Langer
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
// at http://www.boost.org/LICENSE_1_0.txt)
                                                                                
//  See library home page at http://www.boost.org/libs/utility
                                                                                
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

#include <cstdlib>

#include <boost/utility/lexicographic.hpp>
#include <boost/timer.hpp>

bool cmp_lex (int a, int b)
{
	return boost::lexicographic 
		(a / 1000, b / 1000)
		(b / 100, a / 100)
		(a / 10, b / 10)
		(b, a);
}
bool cmp_lex_nl (int a, int b)
{
	return boost::lexicographic 
		(a, b)
		(b, a)
		(a, b)
		(b, a);
}

bool cmp_emul_nl (int a, int b)
{
	typedef boost::lexicographic::result_type result_type;
	result_type const equivalent = boost::lexicographic::equivalent; 
	result_type const minus = boost::lexicographic::minus; 
	result_type const plus = boost::lexicographic::plus; 

	// ctor
	result_type m = equivalent;
	if (a < b)
		m = minus;
	else if (b < a)
		m = plus;
	else
		m = equivalent;

	// first operator ()
	if (m == equivalent)
		if (b < a)
			m = minus;
		else if (a < b)
			m = plus;
		else
			m = equivalent;
	
	// second operator ()
	if (m == equivalent)
		if (a < b)
			m = minus;
		else if (b < a)
			m = plus;
		else
			m = equivalent;
	
	// third operator ()
	if (m == equivalent)
		if (b < a)
			m = minus;
		else if (a < b)
			m = plus;
		else
			m = equivalent;
	 
	return m == minus;
}

bool cmp_cascade (int a, int b)
{
	if (a / 1000 == b / 1000)
		if (b / 100 == a / 100)
			if (a / 10 == b / 10)
				return b < a;
			else
				return a / 10 < b / 10;
		else
			return b / 100 < a / 100;
	else
		return a / 1000 < b / 1000;
}
bool cmp_cascade_nl (int a, int b)
{
	if (a == b)
		if (b == a)
			if (a == b)
				return b < a;
			else
				return a < b;
		else
			return b < a;
	else
		return a < b;
}

typedef std::vector <int> int_vector;

void run (int_vector values, // make copy to keep original random order
	  bool (*cmp) (int, int),
	  std::string desc)
{
	boost::timer uhr;
	std::sort (values.begin (), values.end (), cmp);
	std::cout << desc << " - " << uhr.elapsed() << '\n';
}

int main ()
{
	int_vector::size_type const n = 400000;
	int_vector values;
	values.reserve (n);
	
	std::srand (std::time (0)); 
	for (unsigned int i = 0; i < n; ++i)
		values.push_back (std::rand ()); 

	run (values, cmp_lex, "boost::lexicographic, division");
	run (values, cmp_lex_nl, "boost::lexicographic, no division");
	run (values, cmp_emul_nl, "boost::lexicographic emulation, no division");
	run (values, cmp_cascade, "if cascade, division");
	run (values, cmp_cascade_nl, "if cascade, no division");
}
	
