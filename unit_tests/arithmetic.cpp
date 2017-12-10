#include <cassert>
#include <limits>
#include "simple/support/arithmetic.hpp"

using namespace simple::support;
using Largest = unsigned long long;

template <typename Unsigned>
void checkUnsigned()
{
	Unsigned result;
	const auto max = std::numeric_limits<Unsigned>::max();
	const auto min = std::numeric_limits<Unsigned>::min();
	const Unsigned half = max/2;
	const Unsigned quart = max/4;

	assert(add_overflow(result, max, Unsigned(1)));
	assert(min == result);

	assert(!add_overflow(result, half, quart));
	assert(Largest(half) + Largest(quart) == result);

	assert(sub_overflow(result, min, Unsigned(1)));
	assert(max == result);

	assert(!sub_overflow(result, half, quart));
	assert(Largest(half) - Largest(quart) == result);

	assert(mul_overflow(result, quart, Unsigned(6)));
	assert(Unsigned(Largest(quart) * 6ull) == result);

	assert(!mul_overflow(result, quart, Unsigned(3)));
	assert(Largest(quart) * 3ull == result);

}

template <typename Signed>
void checkSigned()
{
	checkUnsigned<Signed>();

	Signed result;
	const auto max = std::numeric_limits<Signed>::max();
	const auto min = std::numeric_limits<Signed>::min();
	const Signed half = max/2;
	const Signed quart = max/4;

	// complementary negative number tests

	assert(add_overflow(result, min, Signed(-1)));
	assert(max == result);

	assert(sub_overflow(result, max, Signed(-1)));
	assert(min == result);

	assert(!add_overflow(result, Signed(-half), Signed(-quart)));
	assert(Largest(half) + Largest(quart) == Signed(-result));

	assert(!sub_overflow(result, Signed(-half), Signed(-quart)));
	assert(Largest(half) - Largest(quart) == Signed(-result));

	assert(mul_overflow(result, Signed(-quart), Signed(6)));
	assert(Signed(Signed(-quart) * Signed(6)) == result);

	assert(mul_overflow(result, quart, Signed(-6)));
	assert(Signed(quart * Signed(-6)) == result);


	// crossing zero tests

	assert(!add_overflow(result, Signed(-quart), half));
	assert(half - quart == result);

	assert(!add_overflow(result, quart, Signed(-half)));
	assert(quart - half == result);

	assert(!sub_overflow(result, Signed(-quart), Signed(-half)));
	assert(half - quart == result);

	assert(!sub_overflow(result, quart, half));
	assert(quart - half == result);

	assert(!mul_overflow(result, Signed(-quart), Signed(-2)));
	assert(Signed(Largest(-quart) * Largest(-2)) == result);

	assert(!mul_overflow(result, Signed(quart), Signed(-2)));
	assert(Signed(Largest(quart) * Largest(-2)) == result);

}

void Overflow()
{

	checkUnsigned<unsigned char>();
	checkUnsigned<unsigned short>();
	checkUnsigned<unsigned int>();
	checkUnsigned<unsigned long>();
	checkUnsigned<unsigned long long>();

#if defined SIMPLE_ASSUME_SIGNED_OVERFLOW_DEFINED
	checkSigned<signed char>();
	checkSigned<signed short>();
	checkSigned<signed int>();
	checkSigned<signed long>();
	checkSigned<signed long long>();
#endif

}

int main()
{
	Overflow();
	return 0;
}
