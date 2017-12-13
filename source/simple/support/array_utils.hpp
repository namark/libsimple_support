#ifndef SIMPLE_SUPPORT_ARRAY_UTILS_HPP
#define SIMPLE_SUPPORT_ARRAY_UTILS_HPP

#include "array.hpp"
// #include <array>
#include <utility>

namespace simple { namespace support
{

	template <typename T, size_t N>
	using target_array = array<T, N>;

	// template <typename T, size_t N>
	// using target_array = std::array<T, N>;

	template <typename T, size_t... Is>
	constexpr target_array<T, sizeof...(Is)> fill_array_indecies(T value, std::index_sequence<Is...>)
	{
		return {{ ((void)Is, value)... }};
	}

	template <size_t size, typename T>
	constexpr target_array<T,size> filled_array(T value)
	{
		return fill_array_indecies(value, std::make_index_sequence<size>{});
	}

	template <typename Function, size_t... Is>
	constexpr auto indecies_to_array(Function transform, std::index_sequence<Is...>) -> target_array<typename std::result_of<Function(std::size_t)>::type, sizeof...(Is)>
	{
		return {{ transform(Is)... }};
	}

	template <size_t size, typename Function>
	constexpr auto make_array(Function initialiser) -> target_array<typename std::result_of<Function(std::size_t)>::type, size>
	{
		return indecies_to_array(initialiser, std::make_index_sequence<size>{});
	}

}} // namespace simple.support

#endif /* end of include guard */
