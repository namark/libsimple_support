#ifndef SIMPLE_SUPPORT_ALGORITHM_HPP
#define SIMPLE_SUPPORT_ALGORITHM_HPP
#include <iterator>
#include <cassert>

#include "range.hpp"

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

	} // namespace detail

	template <typename Itr, typename BoundsItr, typename StepItr>
	constexpr auto advance_vector(Itr begin, Itr end, BoundsItr lower_begin, BoundsItr upper_begin, StepItr step_begin) -> std::remove_reference_t<decltype
	(
		*begin, ++begin,
		*lower_begin, ++lower_begin,
		*step_begin, ++step_begin,
		begin
	)>
	{
		// TODO: do while loop can be more optimal for most use cases, but burdens the user
		while(begin != end)
		{
			if constexpr (std::is_same_v<StepItr, detail::increment_step>)
				++(*begin);
			else
				*begin += *step_begin;

			if(*begin < *upper_begin)
				break;
			*begin = *lower_begin;
			++begin;
			++lower_begin;
			++upper_begin;
			if constexpr (!std::is_same_v<StepItr, detail::increment_step>)
				++step_begin;
		}
		return begin;
	}

	template <typename Itr, typename BoundsItr>
	constexpr auto advance_vector(Itr begin, Itr end, BoundsItr lower_begin, BoundsItr upper_begin) -> std::remove_reference_t<decltype
	(
		*begin, ++begin,
		*lower_begin, ++lower_begin,
		begin
	)>
	{
		return advance_vector(begin, end, lower_begin, upper_begin, detail::increment_step{});
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

	template <typename Range, typename BoundsRange,
		std::enable_if_t<is_range_v<Range> && is_range_v<BoundsRange>>* = nullptr>
	constexpr auto advance_vector(Range& range, BoundsRange lower, BoundsRange upper)
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
	constexpr auto make_range(Container& container)
	{
		using std::begin;
		using std::end;
		return make_range(begin(container), end(container));
	}

	template <typename Container>
	constexpr auto make_range(Container&& container) = delete;

	template <typename Container>
	constexpr auto reverse_range(Container& container)
	{
		using std::rbegin;
		using std::rend;
		return make_range(rbegin(container), rend(container));
	}

	template <typename Container>
	constexpr auto reverse_range(Container&& container) = delete;


	template<typename Index = std::size_t, typename Container>
	constexpr
	auto get_iterator_range(Container&& container, const range<Index>& index_range)
	{
		using std::begin;
		using std::end;
		return make_range(std::forward<Container>(container)).sub_range(index_range);
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

	template <typename Number>
	constexpr Number wrap(Number x, Number upperLimit)
	noexcept(noexcept(Number((x + upperLimit) % upperLimit)))
	{
		return (x + upperLimit) % upperLimit;
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
