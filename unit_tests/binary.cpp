#include "simple/support/bits.hpp"
#include <climits>

using namespace simple::support;

template <int n>
constexpr void powers_of_two()
{
	static_assert(count_trailing_zeros(1 << n) == n);
	powers_of_two<n-1>();
};
template <>
constexpr void powers_of_two<0>(){}

int main()
{
	static_assert(count_ones(0) == 0);
	static_assert(count_ones(-1) == sizeof(int) * CHAR_BIT);
	static_assert(count_ones(0b1) == 1);
	static_assert(count_ones(0b1010101) == 4);
	static_assert(count_ones(0x1000'0101'1010'0000LL) == 5);
	static_assert(count_trailing_zeros(0b1) == 0);
	static_assert(count_trailing_zeros(0b1000) == 3);
	static_assert(count_trailing_zeros(0x1000'0101'1010'0000LL) == 5*4);
	static_assert(count_trailing_zeros(1 << 0) == 0);
	powers_of_two<sizeof(int) * CHAR_BIT - 1>();
	return 0;
}
