#include "simple/support/array_utils.hpp"

using namespace simple;

void FilledArray()
{
	constexpr auto value = 123;
	constexpr auto flar = support::filled_array<8>(value);
	static_assert
	(
		flar[0] == value &&
		flar[1] == value &&
		flar[2] == value &&
		flar[3] == value &&
		flar[4] == value &&
		flar[5] == value &&
		flar[6] == value &&
		flar[7] == value
		, "filled_array must work!"
	);
}

constexpr size_t trivial_init(size_t index)
{
	return index;
}

void MakeArray()
{
	constexpr auto arr = support::make_array<8>(trivial_init);
	static_assert
	(
		arr[0] == 0 &&
		arr[1] == 1 &&
		arr[2] == 2 &&
		arr[3] == 3 &&
		arr[4] == 4 &&
		arr[5] == 5 &&
		arr[6] == 6 &&
		arr[7] == 7
		, "make_array must work!"
	);
}

int main()
{
	FilledArray();
	MakeArray();
	return 0;
}
