#ifndef SIMPLE_SUPPORT_ALGORITHM_ADVANCE_VECTOR_HPP
#define SIMPLE_SUPPORT_ALGORITHM_ADVANCE_VECTOR_HPP
#include "utils.hpp"

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
		using std::begin;
		using std::end;
		return advance_vector(begin(range), end(range), begin(lower), begin(upper));
	}

	template <typename Range, typename BoundsRange, typename StepRange,
		std::enable_if_t<is_range_v<Range> && is_range_v<BoundsRange> && is_range_v<StepRange>>* = nullptr>
	constexpr auto advance_vector(Range& range, BoundsRange lower, BoundsRange upper, StepRange step)
	{
		assert(distance(range) == distance(upper));
		assert(distance(range) == distance(lower));
		assert(distance(range) == distance(step));
		using std::begin;
		using std::end;
		return advance_vector(begin(range), end(range), begin(lower), begin(upper), begin(step));
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
		using std::begin;
		using std::end;
		return advance_vector(begin(range), end(range), begin(upper));
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




} // namespace simple::support

#endif /* end of include guard */
