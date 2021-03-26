#ifndef SIMPLE_SUPPORT_TUPLE_UTILS_SUBTUPLE_HPP
#define SIMPLE_SUPPORT_TUPLE_UTILS_SUBTUPLE_HPP
#include <utility>
#include <tuple>

namespace simple::support
{
	template <typename Tuple, typename Range>
	struct subtuple_meta;
	template <typename Tuple, size_t... indices>
	struct subtuple_meta<Tuple, std::integer_sequence<size_t, indices...>>
	{
		using type = std::tuple<std::tuple_element_t<indices, Tuple>...>;
	};
	template <typename Tuple, typename Range>
	using subtuple_t = typename subtuple_meta<Tuple, Range>::type;;

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
	using tuple_indices = std::make_index_sequence<
		std::tuple_size_v<std::decay_t<T>>
	>;

	template <typename T>
	using tuple_indecies [[deprecated("use tuple_indices cause, well, spelling")]] = std::make_index_sequence<
		std::tuple_size_v<std::decay_t<T>>
	>;

} // namespace simple::support

#endif /* end of include guard */
