#ifndef SIMPLE_SUPPORT_ALGORITHM_HPP
#define SIMPLE_SUPPORT_ALGORITHM_HPP
#include <iterator>

#include "range.hpp"

namespace simple::support
{

	template <typename Itr, typename BoundsItr>
	constexpr Itr advance_vector(Itr begin, Itr end, BoundsItr lower_begin, BoundsItr upper_begin)
	{
		while(begin != end)
		{
			++(*begin); // TODO: add step parameter
			if(*begin < *upper_begin)
				break;
			*begin = *lower_begin;
			++begin;
			++lower_begin;
			++upper_begin;
		}
		return begin;
	}

	template <typename Range, typename BoundsRange>
	constexpr auto advance_vector(Range& range, BoundsRange lower, BoundsRange upper)
	{
		using std::begin;
		using std::end;
		using std::distance;
		auto range_begin = begin(range);
		auto range_end = end(range);
		auto upper_begin = begin(upper);
		auto upper_end = end(upper);
		auto lower_begin = begin(lower);
		auto lower_end = end(lower);
		assert(distance(range_begin, range_end) == distance(upper_begin, upper_end));
		assert(distance(range_begin, range_end) == distance(lower_begin, lower_end));
		return advance_vector(range_begin, range_end, lower_begin, upper_begin);
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
	Number wrap(Number x, Number upperLimit)
	{
		return (x + upperLimit) % upperLimit;
	}

} // namespace simple::support

#endif /* end of include guard */
