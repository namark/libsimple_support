#include <cassert>
#include <sstream>
#include <iostream>
#include "simple/support/tuple_utils.hpp"

int main()
{
	using simple::support::apply_for;
	auto t = std::tuple(0," one ",2," three");
	std::stringstream ss;

	apply_for({0,4}, [&ss](auto&& a)
	{
		ss << a;
	} ,t);
	assert(ss.str() == "0 one 2 three");
	ss.str("");
	ss.clear();

	apply_for({1,3}, [&ss](auto&& a)
	{
		ss << a;
	} ,t);
	assert(ss.str() == " one 2");
	ss.str("");
	ss.clear();

	apply_for(3, [&ss](auto&& a)
	{
		ss << a;
	} ,t);
	assert(ss.str() == " three");
	ss.str("");
	ss.clear();

	auto t2 = std::tuple(0,1,2,3);
	assert( 4 == apply_for(2, [](auto&& x)
				{ return x + 2; }, t2) );

	return 0;
}
