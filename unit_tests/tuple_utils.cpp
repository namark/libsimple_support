#include <cassert>
#include <sstream>
#include <iostream>
#include "simple/support/tuple_utils.hpp"

using namespace simple::support;

void ApplyFor()
{
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

	auto t2 = std::tuple(0,1.1,2,3);

	apply_for(2, [](auto&& x)
			{ x += 2; }, t2) ;
	assert( std::tuple(0,1.1,4,3) == t2 );

	apply_for({1, 3}, [](auto&& x)
			{ x += 2; }, t2) ;
	assert( std::tuple(0,3.1,6,3) == t2 );

	auto t3 = std::tuple(0,1,2,3);
	assert( 4 == apply_for(2, [](auto&& x)
			{ return x + 2; }, t3) );

}

template <typename... T>
constexpr auto tuple_tie(std::tuple<T...>& t)
{
	return std::apply(std::tie<T...>, t);
}

void CarCdr()
{

	{ auto t = std::tuple(true, 1.5, 5);
		assert(tuple_car(t));
		assert(1.5 == tuple_car(tuple_cdr(t)));
	}

	{ auto t = std::tuple(1, 2, 3);
		auto tref = tuple_tie(t);

		assert(t == tref);

		tuple_car(tref) = -4;

		assert(tuple_car(t) == -4);
		assert(t == tref);

		auto tref_cdr = tuple_tie_cdr(tref);

		tuple_car(tref_cdr) = 13;

		assert(std::get<1>(t) == 13);
		assert(tref_cdr == std::tuple(13,3));

		auto tref_cdr2 = tuple_tie_cdr(t);
		tuple_car(tref_cdr2) = 12;

		assert(std::get<1>(t) == 12);
		assert(tref_cdr2 == tref_cdr);
		assert(tref_cdr2 == std::tuple(12,3));
		assert(tref_cdr == std::tuple(12,3));
	}
}

int main()
{
	ApplyFor();
	CarCdr();
	return 0;
}
