#include "simple/support/rational.hpp"

using simple::support::rational;
using simple::support::meta_constant;

constexpr bool Ratio()
{
	static_assert(int(10 * rational{1,2}) == 5);
	static_assert(int(9 * rational{1,2}) == 4);
	static_assert(double(9 * rational{1.0,2.0}) == 4.5);
	static_assert(int(rational{1,3} * 9) == 3);
	static_assert(int(rational{1,3} * rational{5,2} * 12) == 10);
	static_assert(int(13 * rational{5,2} * rational{2,5}) == 13);

	bool assertion = true;

	auto ratio = rational{1,2};
	assertion &= int(ratio) == 0;
	ratio *= 2;
	assertion &= int(ratio) == 1;
	ratio *= 3;
	assertion &= int(ratio) == 3;
	ratio *= rational{1,2};
	assertion &= int(ratio) == 1;
	ratio *= 5;
	assertion &= int(ratio) == 7;
	ratio *= 2;
	assertion &= int(ratio) == 15;

	auto half = rational(1, meta_constant<int,2>{});
	auto one = half + half;
	auto quarter = half * half;
	auto onenhalf = half + 1;

	assertion &= int(one) == 1;
	assertion &= int(quarter * 16) == 4;
	assertion &= int(quarter * 8) == 2;
	assertion &= int(quarter * 15) == 3;
	assertion &= (quarter * 2) == (quarter + quarter);
	assertion &= (quarter * 2) == rational(2, meta_constant<int,4>{});
	assertion &= onenhalf == rational(3, meta_constant<int,2>{});

	return assertion;
}

int main()
{
	static_assert(Ratio());
	return 0;
}
