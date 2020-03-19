#include <cassert>
#include <string_view>
#include "simple/support/enum.hpp"

using namespace simple::support;

enum class digits
{
	zero,
	one,
	two,
	three,
	four,

	invalid
};

using digit = mapped_enum
<
	digits,
	digits::invalid,
	2,
	std::string_view
>;

template <> digit::guts::map_type digit::guts::map
{{
	{"0", "zero"},
	{"1", "one"},
	{"2", "two"},
	{"3", "three"},
	{"4", "four"},
}};

int main()
{
	assert( digit("one") == digit("1") );
	assert( digit("one") == digits::one );
	switch(digit("two"))
	{
		case digits::zero:    assert(false); break;
		case digits::one:     assert(false); break;
		case digits::two:     assert(true); break;
		case digits::three:   assert(false); break;
		case digits::four:    assert(false); break;
		case digits::invalid: assert(false); break;
		default:              assert(false); break;
	}
	switch(digit("three"))
	{
		case digits::zero:    assert(false); break;
		case digits::one:     assert(false); break;
		case digits::two:     assert(false); break;
		case digits::three:   assert(true); break;
		case digits::four:    assert(false); break;
		case digits::invalid: assert(false); break;
		default:              assert(false); break;
	}
	switch(digit("four... but nah, not really"))
	{
		case digits::zero:    assert(false); break;
		case digits::one:     assert(false); break;
		case digits::two:     assert(false); break;
		case digits::three:   assert(false); break;
		case digits::four:    assert(false); break;
		case digits::invalid: assert(true); break;
		default:              assert(false); break;
	}

	assert(to_integer(digit("two")) == 2);
	assert(to_integer(digits::two) == 2);

	return 0;
}
