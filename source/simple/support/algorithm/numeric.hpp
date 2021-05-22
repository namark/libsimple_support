#ifndef SIMPLE_SUPPORT_ALGORITHM_NUMERIC_HPP
#define SIMPLE_SUPPORT_ALGORITHM_NUMERIC_HPP
#include <cmath>
#include <type_traits>

#include "../arithmetic.hpp"

namespace simple::support
{

	template <typename Number,
		std::enable_if_t<!std::is_floating_point_v<Number>>* = nullptr>
	[[nodiscard]] constexpr
	Number wrap(Number x, Number upperLimit)
	noexcept(noexcept(Number((x + upperLimit) % upperLimit)))
	{
		return (x + upperLimit) % upperLimit;
	}

	template <typename Number,
		std::enable_if_t<std::is_floating_point_v<Number>>* = nullptr>
	[[nodiscard]] constexpr
	Number wrap(Number x, Number upperLimit)
	noexcept(noexcept(Number(std::fmod(x + upperLimit, upperLimit))))
	{
		return std::fmod(x + upperLimit, upperLimit);
	}

	template <typename... Numbers>
	[[nodiscard]] constexpr
	auto average(Numbers... n)
	//TODO: noexcept account for return value and default construction
	noexcept(noexcept((n + ...) / int(sizeof...(n))))
	{
		using Sum = decltype((n + ...));
		return (n + ...) / (Sum{} + sizeof...(n));
	}

	template <typename Number, typename Ratio>
	[[nodiscard]] constexpr
	Number way(Number from, Number to, Ratio ratio)
	noexcept(noexcept(Number(from + (to - from)*ratio)))
	{
		return from + (to - from)*ratio;
	}

	template <typename Number, typename Ratio>
	[[nodiscard]] constexpr
	Number wayback(Number from, Number to, Ratio ratio)
	noexcept(noexcept(Number(from - (from - to)*ratio)))
	{
		return from - (from - to)*ratio;
	}

	template <typename Number>
	[[nodiscard]] constexpr
	Number halfway(Number from, Number to)
	// TODO: implement in terms of way with rational 1/2
	noexcept(noexcept(Number(from - (from - to)/2)))
	{
		return from + (to - from)/2;
	}

	template <typename Number>
	[[nodiscard]] constexpr
	Number halfwayback(Number from, Number to)
	// TODO: implement in terms of wayback with rational 1/2
	noexcept(noexcept(Number(from - (from - to)/2)))
	{
		return from - (from - to)/2;
	}

	template <typename Integer,
		typename Unsigned = std::make_unsigned_t<Integer>>
	[[nodiscard]] constexpr
	Integer midpoint(Integer a, Integer b) noexcept
	// noexcept(noexcept(TODO))
	{
		using std::numeric_limits;
		constexpr Unsigned abs_int_min = -Unsigned(numeric_limits<Integer>::min());
		constexpr auto int_max = numeric_limits<Integer>::max();
		constexpr auto uint_max = numeric_limits<Unsigned>::max();

		static_assert(uint_max >= int_max && uint_max - int_max >= abs_int_min,
			"Unsigned includes Integer");

		static_assert(uint_max/2 <= int_max,
			"Halfing Unsigned brings it to positive Integer range");

		static_assert(std::is_unsigned_v<Integer> || int_max <= abs_int_min,
			"Can negate a positive Integer");

		Unsigned diff = Unsigned(b) - Unsigned(a);

		// 2's complement with carry as a sign
		Unsigned negative_2x = -diff; // neg
		negative_2x /= 2; // div
		Integer negative = -Integer(negative_2x); // neg
		// or... 0 - (0 - diff)/2, this is midpointception! TODO: -_-
		// can't use halfwayback here cause promotion ToT

		Integer positive = diff / 2;

		return a + (b < a ? negative : positive);

		// TODO: this is for geom::vector
		// auto overflew = b < a;
		// return a + overflew * negative + !overflew * positive;
		// better yet introduce a mask function, since intrinsic vector has element-wise ternary

	}

	template <typename Unsigned>
	[[nodiscard]] constexpr
	Unsigned umidpoint(Unsigned a, Unsigned b)
	// noexcept(noexcept(TODO))
	{

		Unsigned diff{};
		auto overflew = sub_overflow(diff,b,a);

		// manual idiv
		Unsigned idiff = -diff; // neg
		idiff /= Unsigned{2}; // div
		idiff = -idiff; // neg
		// or... 0 - (0 - diff)/2, this is midpointception! TODO: -_-
		// can't use halfwayback here cause promotion ToT

		diff /= 2;

		return a + (overflew ? idiff : diff);
		// return a + overflew * idiff + !overflew * diff;

	}

} // namespace simple::support

#endif /* end of include guard */
