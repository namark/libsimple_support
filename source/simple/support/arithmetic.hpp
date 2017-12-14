#ifndef SIMPLE_SUPPORT_ARITHMETIC_HPP
#define SIMPLE_SUPPORT_ARITHMETIC_HPP

#include <type_traits>

#if !defined __GNUC__ || defined SIMPLE_PREVENT_INTRINSIC_OVERFLOW_CHECK
#define SIMPLE_ARITHMETIC_OVERFLOW_FALLBACK
#endif

#if defined SIMPLE_ARITHMETIC_OVERFLOW_FALLBACK
#include <limits>
#endif

namespace simple { namespace support
{

	template <typename Type, typename Result = void>
	using enable_if_overflow_defined = std::enable_if_t<
		std::is_integral<Type>::value
#if !defined SIMPLE_ASSUME_SIGNED_OVERFLOW_DEFINED
		&& !std::is_signed<Type>::value
#endif
		, void>;

	template<typename Int, enable_if_overflow_defined<Int>* = nullptr>
	constexpr inline bool slow_add_overflow(Int& result, Int one, Int two)
	{
		result = one + two;
		const Int max = std::numeric_limits<Int>::max();
		const Int min = std::numeric_limits<Int>::min();
		return (one >= 0)
			? two > (max - one)
			: two < (min - one);
	}

	template<typename Int, enable_if_overflow_defined<Int>* = nullptr>
	constexpr inline bool add_overflow(Int& result, Int one, Int two)
	{
#if defined SIMPLE_ARITHMETIC_OVERFLOW_FALLBACK
		return slow_add_overflow(result, one, two);
#else
		return __builtin_add_overflow(one, two, &result);
#endif
	}

	template<typename Int, enable_if_overflow_defined<Int>* = nullptr>
	constexpr inline bool slow_sub_overflow(Int& result, Int one, Int two)
	{
		result = one - two;
		const Int max = std::numeric_limits<Int>::max();
		const Int min = std::numeric_limits<Int>::min();
		return (two >= 0)
			? two + min > one
			: two + max < one;
	}

	template<typename Int, enable_if_overflow_defined<Int>* = nullptr>
	constexpr inline bool sub_overflow(Int& result, Int one, Int two)
	{
#if defined SIMPLE_ARITHMETIC_OVERFLOW_FALLBACK
		return slow_sub_overflow(result, one, two);
#else
		return __builtin_sub_overflow(one, two, &result);
#endif
	}

	template<typename Int, enable_if_overflow_defined<Int>* = nullptr>
	constexpr inline bool slow_mul_overflow(Int& result, Int one, Int two)
	{
		result = one * two;
		return (two != 0 && result / two != one);
	}

	template<typename Int, enable_if_overflow_defined<Int>* = nullptr>
	constexpr inline bool mul_overflow(Int& result, Int one, Int two)
	{
#if defined SIMPLE_ARITHMETIC_OVERFLOW_FALLBACK
		return slow_mul_overflow(result, one, two);
#else
		return __builtin_mul_overflow(one, two, &result);
#endif
	}

	template<typename Int, enable_if_overflow_defined<Int>* = nullptr>
	constexpr inline bool add_overflow(Int& one, Int two)
	{
		return add_overflow(one, one, two);
	}

	template<typename Int, enable_if_overflow_defined<Int>* = nullptr>
	constexpr inline bool sub_overflow(Int& one, Int two)
	{
		return sub_overflow(one, one, two);
	}

	template<typename Int, enable_if_overflow_defined<Int>* = nullptr>
	constexpr inline bool mul_overflow(Int& one, Int two)
	{
		return mul_overflow(one, one, two);
	}

}} // namespace simple::support

#endif /* end of include guard */
