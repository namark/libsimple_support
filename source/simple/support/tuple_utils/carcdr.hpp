#ifndef SIMPLE_SUPPORT_TUPLE_UTILS_CARCDR_HPP
#define SIMPLE_SUPPORT_TUPLE_UTILS_CARCDR_HPP

#include "subtuple.hpp"
#include "../carcdr.hpp"

namespace simple::support
{

	template <typename Tuple>
	constexpr decltype(auto) tuple_car(Tuple&& tuple)
	noexcept(noexcept(std::get<0>(std::forward<Tuple>(tuple))))
	{
		return std::get<0>(std::forward<Tuple>(tuple));
	}

	constexpr std::nullptr_t tuple_car(const std::tuple<>&)
	{
		return nullptr;
	}

	template <typename First, typename... Rest>
	constexpr auto tuple_cdr(const std::tuple<First, Rest...>& tuple)
	{
		using indices = std::make_index_sequence<
			std::tuple_size_v<std::decay_t<decltype(tuple)>>>;
		return subtuple(tuple, cdr<indices>{});
	}

	template <typename First, typename... Rest>
	constexpr auto tuple_tie_cdr(const std::tuple<First, Rest...>& tuple)
	{
		using indices = tuple_indices<decltype(tuple)>;
		return tie_subtuple(tuple, cdr<indices>{});
	}

	template <typename First, typename... Rest>
	constexpr auto tuple_tie_cdr(std::tuple<First, Rest...>& tuple)
	{
		using indices = tuple_indices<decltype(tuple)>;
		return tie_subtuple(tuple, cdr<indices>{});
	}

	template <typename Tuple>
	using tuple_cdr_t = subtuple_t<Tuple,
		cdr<tuple_indices<Tuple>> >;

	template <typename Tuple>
	using tuple_car_t = std::tuple_element_t<0, Tuple>;

} // namespace simple::support

#endif /* end of include guard */
