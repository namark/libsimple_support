#ifndef SIMPLE_SUPPORT_ALGORITHM_VARIANCE_HPP
#define SIMPLE_SUPPORT_ALGORITHM_VARIANCE_HPP
#include "iterator"

namespace simple::support
{

	// TODO: somehow the parameter counts and types line up and don't conflict between iterator pairs and range overloads, but it's fragile, better use the is_range_v trait

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

} // namespace simple::support

#endif /* end of include guard */
