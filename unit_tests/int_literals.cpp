#include <cassert>
#include <iostream>

#include "simple/support/int_literals.hpp"

using namespace simple::support::literals;

int main()
{
	static_assert(128_u8 == uint8_t{128});
	static_assert(0177777_u16 == uint16_t{0177777});
	static_assert(0xffff'ffff_u32 == uint32_t{0xffff'ffff});
	static_assert(0x1'0000'0000_u64 == uint64_t{0x1'0000'0000});
	static_assert(0xffff'ffff'ffff'ffff_u64 == uint64_t{0xffff'ffff'ffff'ffff});
	return 0;
}
