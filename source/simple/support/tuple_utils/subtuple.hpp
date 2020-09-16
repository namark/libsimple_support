#ifndef SIMPLE_SUPPORT_TUPLE_UTILS_SUBTUPLE_HPP
#define SIMPLE_SUPPORT_TUPLE_UTILS_SUBTUPLE_HPP
#include <utility>
#include <tuple>

namespace simple::support
{

	template <typename... Rest, size_t... indices>
	constexpr auto subtuple(const std::tuple<Rest...>& tuple, std::integer_sequence<size_t, indices...>)
	{
		return std::make_tuple(std::get<indices>(tuple)...);
	}

	template <typename... Rest, size_t... indices>
	constexpr auto tie_subtuple(const std::tuple<Rest...>& tuple, std::integer_sequence<size_t, indices...>)
	{
		return std::tie(std::get<indices>(tuple)...);
	}

	template <typename... Rest, size_t... indices>
	constexpr auto tie_subtuple(std::tuple<Rest...>& tuple, std::integer_sequence<size_t, indices...>)
	{
		return std::tie(std::get<indices>(tuple)...);
	}

	template <typename T>
	using tuple_indecies = std::make_index_sequence<
		std::tuple_size_v<std::decay_t<T>>
	>;

} // namespace simple::support

#endif /* end of include guard */
