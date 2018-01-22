#ifndef SIMPLE_SUPPORT_INT_LITERALS_HPP
#define SIMPLE_SUPPORT_INT_LITERALS_HPP

#include <cstdint>
#include <type_traits>
#include <limits>

namespace simple::support::literals
{

namespace details
{

	constexpr char spacer = '\'';

	template <typename Int, uintmax_t base, char first, char... rest>
	constexpr Int parse() noexcept
	{
		constexpr uintmax_t digit = first +
		(
			( '0' <= first && first <= '9') ? (-'0') :
			( 'A' <= first && first <= 'Z' ) ? (-'A' + 10) :
			( 'a' <= first && first <= 'z' ) ? (-'a' + 10) :
			-first
		);
		constexpr auto size = sizeof...(rest);
		if constexpr(size > 0)
		{
			constexpr uintmax_t order = ( ... * (rest == spacer ? 1 : base)  );
			constexpr uintmax_t result = digit * order +
				static_cast<uintmax_t>(parse<Int, base, rest...>());
			static_assert( result <= std::numeric_limits<Int>::max(), "Integer literal out of range" );
			return static_cast<Int>(result);
		}
		return static_cast<Int>(digit);
	}

	template <typename Int>
	constexpr Int parse_second() noexcept
	{
		return 0;
	}

	template <typename Int, char first, char... rest>
	constexpr Int parse_second() noexcept
	{
		if constexpr (first == 'x')
			return parse<Int, 16, rest...>();
		else if constexpr (first == 'b')
			return parse<Int, 2, rest...>();
		else
			return parse<Int, 8, first, rest...>();
	}

	template <typename Int, char first, char... rest>
	constexpr Int parse_first() noexcept
	{
		if constexpr (first != '0')
			return parse<Int, 10, first, rest...>();
		else
			return parse_second<Int, rest...>();
	}

} // namespace details

#define SIMPLE_SUPPORT_DEFINE_UNSIGNED_INT_LITERAL_OPERATOR(type, name) \
	template<char... chars> \
	constexpr type operator"" name() \
	{ \
		return details::parse_first<type, chars...>(); \
	} \

SIMPLE_SUPPORT_DEFINE_UNSIGNED_INT_LITERAL_OPERATOR(uint8_t, _u8);
SIMPLE_SUPPORT_DEFINE_UNSIGNED_INT_LITERAL_OPERATOR(uint16_t, _u16);
SIMPLE_SUPPORT_DEFINE_UNSIGNED_INT_LITERAL_OPERATOR(uint32_t, _u32);
SIMPLE_SUPPORT_DEFINE_UNSIGNED_INT_LITERAL_OPERATOR(uint64_t, _u64);
SIMPLE_SUPPORT_DEFINE_UNSIGNED_INT_LITERAL_OPERATOR(unsigned char, _uc);
SIMPLE_SUPPORT_DEFINE_UNSIGNED_INT_LITERAL_OPERATOR(unsigned short, _us);

#undef SIMPLE_SUPPORT_DEFINE_UNSIGNED_INT_LITERAL_OPERATOR

} // namespace literals

#endif /* end of include guard */
