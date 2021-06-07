#ifndef SIMPLE_SUPPORT_ALGORITHM_SET_OPS_HPP
#define SIMPLE_SUPPORT_ALGORITHM_SET_OPS_HPP
#include <functional> // all this for just std::less

namespace simple::support
{

	namespace detail
	{
		template <typename MinuendIt, typename SubtrahendIt, typename Less>
		constexpr bool not_in_set
		(
			SubtrahendIt& sub_begin, SubtrahendIt sub_end,
			MinuendIt minuend,
			Less&& less
		)
		{
			while(sub_begin != sub_end)
			{
				// TODO: I should use invoke, but it's not constexpr, and apply(forward_as_tuple) is unreadable
				if(less(*minuend, *sub_begin))
					return true;
				else if(less(*sub_begin, *minuend))
					++sub_begin;
				else
				{
					++sub_begin;
					return false;
				}
			}
			return true;
		}
	} // namespace detail

	// cause standard version is not allowed to work in place for some reason
	template <typename MinuendIt, typename SubtrahendIt, typename DifferenceIt, typename Less = std::less<>>
	constexpr DifferenceIt set_difference
	(
		MinuendIt begin, MinuendIt end,
		SubtrahendIt sub_begin, SubtrahendIt sub_end,
		DifferenceIt diff,
		Less&& less = std::less<>{}
	)
	{
		while(begin != end)
		{
			if(detail::not_in_set(sub_begin, sub_end, begin, less))
				*diff++ = *begin;
			++begin;
		}
		return diff;
	}

} // namespace simple::support

#endif /* end of include guard */
