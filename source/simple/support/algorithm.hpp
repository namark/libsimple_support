#ifndef SIMPLE_SUPPORT_ALGORITHM_HPP
#define SIMPLE_SUPPORT_ALGORITHM_HPP
#include <cassert>
#include <cmath>
#include <iterator>
#include <algorithm>

#include "range.hpp"
#include "arithmetic.hpp"

namespace simple::support
{

	namespace detail
	{

		class increment_step
		{
			public:
			void operator*() const noexcept;
			void operator++() const noexcept;
		};

		class null_lower_iterator
		{
			public:
			void operator*() const noexcept;
			void operator++() const noexcept;
		};

	} // namespace detail

	template <typename Itr, typename LowerItr, typename UpperItr, typename StepItr = detail::increment_step>
	constexpr auto advance_vector(Itr begin, Itr end,
			LowerItr lower_begin, UpperItr upper_begin,
			StepItr step_begin = detail::increment_step{})
	-> std::remove_reference_t<decltype
	(
		*begin, ++begin,
		*lower_begin, ++lower_begin,
		*upper_begin, ++upper_begin,
		*step_begin, ++step_begin,
		begin
	)>
	{
		using value_type = std::remove_reference_t<decltype(*begin)>;
		constexpr bool increment_step = std::is_same_v<StepItr, detail::increment_step>;
		constexpr bool null_lower_iterator = std::is_same_v<LowerItr, detail::null_lower_iterator>;
		// TODO: do while loop can be more optimal for most use cases, but burdens the user
		while(begin != end)
		{
			if constexpr (increment_step)
				++(*begin);
			else
				*begin += *step_begin;

			if(*begin < *upper_begin)
				break;
			if constexpr (null_lower_iterator)
				*begin = value_type{};
			else
				*begin = *lower_begin;

			++begin;
			++upper_begin;
			if constexpr (!null_lower_iterator)
				++lower_begin;
			if constexpr (!increment_step)
				++step_begin;
		}
		return begin;
	}

	template <typename Itr, typename UpperItr>
	constexpr auto advance_vector(Itr begin, Itr end, UpperItr upper_begin) -> std::remove_reference_t<decltype
	(
		*begin, ++begin,
		*upper_begin, ++upper_begin,
		begin
	)>
	{
		return advance_vector(begin, end, detail::null_lower_iterator{}, upper_begin, detail::increment_step{});
	}

	template <typename T, typename = std::nullptr_t>
	struct is_range
	: public std::false_type {};

	template <typename T>
	struct is_range<T, decltype(
		std::begin(std::declval<T>()),
		std::end(std::declval<T>()),
		nullptr)>
	: public std::true_type {};

	template <typename T>
	constexpr auto is_range_v = is_range<T>::value;

	template <typename Range, std::enable_if_t<is_range_v<Range>>* = nullptr>
	constexpr auto distance(Range&& range)
	{
		return std::distance(std::begin(range), std::end(range));
	}

	template <typename Range, typename LowerRange, typename UpperRange,
		std::enable_if_t<
			is_range_v<Range> &&
			is_range_v<LowerRange> &&
			is_range_v<UpperRange>
		>* = nullptr
	>
	constexpr auto advance_vector(Range& range, LowerRange lower, UpperRange upper)
	{
		assert(distance(range) == distance(upper));
		assert(distance(range) == distance(lower));
		return advance_vector(std::begin(range), std::end(range), std::begin(lower), std::begin(upper));
	}

	template <typename Range, typename BoundsRange, typename StepRange,
		std::enable_if_t<is_range_v<Range> && is_range_v<BoundsRange> && is_range_v<StepRange>>* = nullptr>
	constexpr auto advance_vector(Range& range, BoundsRange lower, BoundsRange upper, StepRange step)
	{
		assert(distance(range) == distance(upper));
		assert(distance(range) == distance(lower));
		assert(distance(range) == distance(step));
		return advance_vector(std::begin(range), std::end(range), std::begin(lower), std::begin(upper), std::begin(step));
	}

	template <typename Range, typename UpperRange,
		std::enable_if_t<
			is_range_v<Range> &&
			is_range_v<UpperRange>
		>* = nullptr
	>
	constexpr auto advance_vector(Range& range, UpperRange upper)
	{
		assert(distance(range) == distance(upper));
		return advance_vector(std::begin(range), std::end(range), std::begin(upper));
	}


	// TODO: next number can be implemented using advance_vector, wrapping the constants base in iterators,
	// but i'm not sure about it, would it optimize well??
	template <typename Itr, typename Num = int>
	constexpr Itr next_number(Itr begin, Itr end, Num base = 2)
	{
		while(begin != end)
		{
			++(*begin);
			if(*begin < base)
				break;
			*begin = 0;
			++begin;
		}
		return begin;
	}

	template <typename Range, typename Num = int>
	constexpr auto next_number(Range& range, Num base = 2)
	{
		using std::begin;
		using std::end;
		return next_number(begin(range), end(range), base);
	}

	template <typename Itr, typename Num = int>
	constexpr Itr prev_number(Itr begin, Itr end, Num base = 2)
	{
		while(begin != end)
		{
			--(*begin);
			if(*begin >= 0)
				break;
			*begin = base - 1;
			++begin;
		}
		return begin;
	}

	template <typename Range, typename Num = int>
	constexpr auto prev_number(Range& range, Num base = 2)
	{
		using std::begin;
		using std::end;
		return prev_number(begin(range), end(range), base);
	}


	template <typename Container>
	[[nodiscard]]
	constexpr auto make_range(Container& container)
	{
		using std::begin;
		using std::end;
		return make_range(begin(container), end(container));
	}

	template <typename Container>
	constexpr auto make_range(Container&& container) = delete;

	template <typename Container>
	[[nodiscard]]
	constexpr auto reverse_range(Container& container)
	{
		using std::rbegin;
		using std::rend;
		return make_range(rbegin(container), rend(container));
	}

	template <typename Container>
	constexpr auto reverse_range(Container&& container) = delete;


	template<typename Index = std::size_t, typename Container>
	[[nodiscard]] constexpr
	auto get_iterator_range(Container&& container, const range<Index>& index_range)
	{
		return make_range(std::forward<Container>(container)).sub_range(index_range);
	}

	template<typename It>
	[[nodiscard]] constexpr
	auto distance(const range<It>& rng, const It& origin)
	{
		using std::begin;
		using std::end;
		return range{ begin(rng) - origin, end(rng) - origin };
	}

	template <typename From, typename To, typename Origin>
	[[nodiscard]] constexpr
	auto map_range(const From& from, To&& to, Origin origin)
	{
		return get_iterator_range(to, distance(from, origin));
	}

	template <typename From, typename To>
	[[nodiscard]] constexpr
	auto map_range(const From& from, const To& to)
	{
		using std::begin;
		return get_iterator_range(to, distance(from, begin(from)));
	}

	template <typename Itr, typename BinaryOp>
	constexpr Itr variance(Itr begin, Itr end, BinaryOp bop) // TODO: oveloads that accept output iterator[s] and output range, with bound checking
	{
		auto prev = begin;
		while(++begin != end)
		{
			*prev = bop(*begin, *prev);
			prev = begin;
		}
		return prev;
	}

	template <typename Itr>
	constexpr Itr variance(Itr begin, Itr end)
	{
		return variance(begin, end, std::minus{});
	}

	template <typename Range, typename BinaryOp>
	constexpr auto variance(Range& range, BinaryOp bop)
	{
		using std::begin;
		using std::end;
		return make_range( begin(range), variance(begin(range), end(range), bop) );
	}

	template <typename Range>
	constexpr auto variance(Range& range)
	{
		using std::begin;
		using std::end;
		return make_range( begin(range), variance(begin(range), end(range)) );
	}

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

		Integer positive = diff / 2;

		auto overflew = b < a;
		return a + (overflew ? negative : positive);

		// TODO: this is for geom::vector
		// return a + overflew * idiff + !overflew * diff;

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

	// std::swap is not constexpr >.<
	template <typename T, std::enable_if_t<
		std::is_move_constructible_v<T> &&
		std::is_move_assignable_v<T>
	>* = nullptr>
	constexpr void swap(T& one, T& other)
	noexcept(
		std::is_nothrow_move_constructible_v<T> &&
		std::is_nothrow_move_assignable_v<T>
	)
	{
		T temp{std::move(other)};
		other = std::move(one);
		one = std::move(temp);
	}

	// rangey wrappers

	template <typename Range>
	constexpr auto min_element(Range& range)
	//TODO: noexcept account for return value construction
	noexcept(noexcept(std::min_element(std::begin(range), std::end(range))))
	{
		return std::min_element(std::begin(range), std::end(range));
	}

	template <typename Range>
	constexpr auto min_element(Range&& range)
	//TODO: noexcept account for return value construction
	noexcept(noexcept(std::min_element(std::begin(range), std::end(range))))
	{
		return *std::min_element(std::begin(range), std::end(range));
	}

	template <typename Range>
	constexpr auto max_element(Range& range)
	//TODO: noexcept account for return value construction
	noexcept(noexcept(std::max_element(std::begin(range), std::end(range))))
	{
		return std::max_element(std::begin(range), std::end(range));
	}

	template <typename Range>
	constexpr auto max_element(Range&& range)
	//TODO: noexcept account for return value construction
	noexcept(noexcept(std::max_element(std::begin(range), std::end(range))))
	{
		return *std::max_element(std::begin(range), std::end(range));
	}

	template <typename Range, typename Compare>
	constexpr auto min_element(Range& range, Compare comp)
	//TODO: noexcept account for return value construction
	noexcept(noexcept(std::min_element(std::begin(range), std::end(range), comp)))
	{
		return std::min_element(std::begin(range), std::end(range), comp);
	}

	template <typename Range, typename Compare>
	constexpr auto min_element(Range&& range, Compare comp)
	//TODO: noexcept account for return value construction
	noexcept(noexcept(std::min_element(std::begin(range), std::end(range), comp)))
	{
		return *std::min_element(std::begin(range), std::end(range), comp);
	}

	template <typename Range, typename Compare>
	constexpr auto max_element(Range& range, Compare comp)
	//TODO: noexcept account for return value construction
	noexcept(noexcept(std::max_element(std::begin(range), std::end(range), comp)))
	{
		return std::max_element(std::begin(range), std::end(range), comp);
	}

	template <typename Range, typename Compare>
	constexpr auto max_element(Range&& range, Compare comp)
	//TODO: noexcept account for return value construction
	noexcept(noexcept(std::max_element(std::begin(range), std::end(range), comp)))
	{
		return *std::max_element(std::begin(range), std::end(range), comp);
	}

	template <typename Range, typename UnaryPredicate>
	constexpr auto all_of(const Range& range, UnaryPredicate p)
	noexcept(noexcept(std::all_of(std::begin(range), std::end(range), p)))
	{
		return std::all_of(std::begin(range), std::end(range), p);
	}
	template <typename ExecutionPolicy, typename Range, typename UnaryPredicate>
	constexpr auto all_of(ExecutionPolicy&& policy, const Range& range, UnaryPredicate p)
	noexcept(noexcept(std::all_of(std::begin(range), std::end(range), p)))
	{
		return std::all_of(std::forward<ExecutionPolicy>(policy),
			std::begin(range), std::end(range), p);
	}

	template <typename Range, typename UnaryPredicate>
	constexpr auto any_of(const Range& range, UnaryPredicate p)
	noexcept(noexcept(std::any_of(std::begin(range), std::end(range), p)))
	{
		return std::any_of(std::begin(range), std::end(range), p);
	}
	template <typename ExecutionPolicy, typename Range, typename UnaryPredicate>
	constexpr auto any_of(ExecutionPolicy&& policy, const Range& range, UnaryPredicate p)
	noexcept(noexcept(std::any_of(std::begin(range), std::end(range), p)))
	{
		return std::any_of(std::forward<ExecutionPolicy>(policy),
			std::begin(range), std::end(range), p);
	}

	template <typename Range, typename UnaryPredicate>
	constexpr auto none_of(const Range& range, UnaryPredicate p)
	noexcept(noexcept(std::none_of(std::begin(range), std::end(range), p)))
	{
		return std::none_of(std::begin(range), std::end(range), p);
	}
	template <typename ExecutionPolicy, typename Range, typename UnaryPredicate>
	constexpr auto none_of(ExecutionPolicy&& policy, const Range& range, UnaryPredicate p)
	noexcept(noexcept(std::none_of(std::begin(range), std::end(range), p)))
	{
		return std::none_of(std::forward<ExecutionPolicy>(policy),
			std::begin(range), std::end(range), p);
	}

} // namespace simple::support

#endif /* end of include guard */
