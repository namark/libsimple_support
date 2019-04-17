#ifndef SIMPLE_SUPPORT_BINARY_HPP
#define SIMPLE_SUPPORT_BINARY_HPP

#include <type_traits>
#include <cassert>
#include <climits>

#if !defined __GNUC__
#define SIMPLE_SUPPORT_BINARY_DISABLE_INTRINSICS
#endif

#if defined SIMPLE_SUPPORT_BINARY_DISABLE_INTRINSICS
#include <bitset>
#endif

namespace simple { namespace support
{

	template <typename Int, std::enable_if_t<std::is_integral_v<Int>>* = nullptr>
	constexpr int count_trailing_zeros(Int in)
	{
		assert(in && "Input must not be zero.");
		constexpr auto size = sizeof(Int);
#if !defined SIMPLE_SUPPORT_BINARY_DISABLE_INTRINSICS
		if constexpr (size == sizeof(unsigned int))
			return __builtin_ctz(in);
		if constexpr (size == sizeof(unsigned long))
			return __builtin_ctzl(in);
		else
			return __builtin_ctzll(in);
#else
		const std::bitset<size * CHAR_BIT> bin(in);
		int count = -1;
		while(!bin[++count]);
		return count;
#endif
	}

	template <typename Int, std::enable_if_t<std::is_integral_v<Int>>* = nullptr>
	constexpr int count_ones(Int in)
	{
		constexpr auto size = sizeof(Int);
#if !defined SIMPLE_SUPPORT_BINARY_DISABLE_INTRINSICS
		if constexpr (size == sizeof(unsigned int))
			return __builtin_popcount(in);
		if constexpr (size == sizeof(unsigned long))
			return __builtin_popcountl(in);
		else
			return __builtin_popcountll(in);
#else
		const std::bitset<size * CHAR_BIT> bin(in);
		int count = 0;
		for(size_t i = 0; i < bin.size() ; ++i)
			count += bin[i];
		return count;
#endif
	}

	template <typename T>
	constexpr std::size_t bit_count(const T&)
	{
		return sizeof(T) * CHAR_BIT;
	}

}} // namespace simple::support

#endif /* end of include guard */
