#ifndef SIMPLE_SUPPORT_TUPLE_UTILS_APPLY_FOR_HPP
#define SIMPLE_SUPPORT_TUPLE_UTILS_APPLY_FOR_HPP

#include <tuple>
#include <functional>
#include "../range.hpp"

namespace simple::support
{

	// wow this is a mess... you think it'll be optimized? O_o
	//TODO: return a variant??
	template<typename F, typename First, size_t I = std::tuple_size_v<std::remove_reference_t<First>> - 1, typename... Rest>
	[[nodiscard]] constexpr
	decltype(auto) apply_for(size_t index, F&& f, First&& first, Rest&&... rest)
	{
		using std::remove_reference_t;
		using First_t = remove_reference_t<First>;
		using std::apply;
		using std::get;
		using std::tuple_element_t;
		using std::forward;
		static_assert(I >= 0 && I < std::tuple_size_v<First_t>);
		if(I == index)
			// std::invoke is not constexpr -_-
			return apply(forward<F>(f), std::forward_as_tuple(
					forward<tuple_element_t<I, First_t>>(get<I>(first)),
					forward<tuple_element_t<I, remove_reference_t<Rest>>>(get<I>(rest))...
			));
		if constexpr (I > 0)
			return apply_for<F, decltype(forward<First_t>(first)), I - 1>(index, forward<F>(f),
					forward<First_t>(first), forward<remove_reference_t<Rest>>(rest)...);

		throw std::logic_error("simple::support::apply_for - this should never happen");
	}

	template<typename F, typename First, size_t I = std::tuple_size_v<std::remove_reference_t<First>> - 1, typename... Rest>
	constexpr
	void apply_for(range<size_t> index_range, F&& f, First&& first, Rest&&... rest)
	{
		for(size_t i = index_range.lower(); i < index_range.upper(); ++i)
		{
			apply_for(i, std::forward<F>(f),
				std::forward<std::remove_reference_t<First>>(first),
				std::forward<std::remove_reference_t<Rest>>(rest)...);
		}
	}

} // namespace simple::support

#endif /* end of include guard */
