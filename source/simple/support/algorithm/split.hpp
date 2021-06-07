#ifndef SIMPLE_SUPPORT_ALGORITHM_SPLIT_HPP
#define SIMPLE_SUPPORT_ALGORITHM_SPLIT_HPP
#include "traits.hpp"
#include "../range.hpp"

namespace simple::support
{

	// cause we don't get a proper standard search until c++20, and even then it depends on super experimental ranges library
	// (how does stuff like that even get in i wonder -_-)
	template <typename It, typename NeedleIt>
	simple::support::range<It> search(It begin, It end, NeedleIt nbegin, NeedleIt nend)
	{
		while(true)
		{
			It found = begin;
			NeedleIt ncurrent = nbegin;
			do
				if(ncurrent == nend)
					return {begin, found};
				else if(found == end)
					return {end, end};
			while(*found++ == *ncurrent++);
			++begin;
		}
		return {end,end}; // unreachable
	}

	template <typename It, typename SepIt, typename OutIt>
	auto split(It begin, It end, SepIt s_begin, SepIt s_end, OutIt out)
	{
		assert(s_begin != s_end); // TODO: implement a version that skips consecutive separators, and will return character by character split in this case
		auto prev = begin;
		auto next = end;
		do
		{
			auto found = support::search(prev, end,
				s_begin, s_end);
			next = found.begin();
			*out++ = {prev, next};
			prev = found.end();
		}
		while(end != next);

		return out;
	}

	template <typename Range, typename Separator, typename OutIt,
		std::enable_if_t<
			is_range_v<Range> &&
			is_range_v<Separator>
		>* = nullptr
	>
	auto split(const Range& in, const Separator& seperator, OutIt out)
	{
		using std::begin;
		using std::end;
		return split(begin(in), end(in), begin(seperator), end(seperator), out);
	}

} // namespace simple::support

#endif /* end of include guard */
