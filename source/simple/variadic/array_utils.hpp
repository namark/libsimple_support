#ifndef SIMPLE_VARIADIC_ARRAY_UTILS_HPP
#define SIMPLE_VARIADIC_ARRAY_UTILS_HPP

#include <array>
#include <utility>

namespace simple { namespace variadic
{

	template <typename T, size_t... Is>
	constexpr std::array<T, sizeof...(Is)> fill_array_indecies(T value, std::index_sequence<Is...>)
	{
		return {{ ((void)Is, value)... }};
	}

	template <size_t size, typename T>
	constexpr std::array<T,size> filled_array(T value)
	{
		return fill_array_indecies(value, std::make_index_sequence<size>{});
	}

	template <typename Function, size_t... Is>
	constexpr auto indecies_to_array(Function transform, std::index_sequence<Is...>) -> std::array<typename std::result_of<Function(std::size_t)>::type, sizeof...(Is)>
	{
		return {{ transform(Is)... }};
	}

	template <size_t size, typename Function>
	constexpr auto make_array(Function initialiser) -> std::array<typename std::result_of<Function(std::size_t)>::type, size>
	{
		return indecies_to_array(initialiser, std::make_index_sequence<size>{});
	}

}} // namespace simple.variadic

#endif /* end of include guard */
