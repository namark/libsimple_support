#ifndef SIMPLE_SUPPORT_TUPLE_UTILS_META_FIND_HPP
#define SIMPLE_SUPPORT_TUPLE_UTILS_META_FIND_HPP

#include <type_traits>
#include <tuple>
#include "common.hpp"
#include "meta_bind.hpp"

namespace simple::support
{

	template <size_t I, typename T, typename Functor>
	struct find_meta_result
	{
		static constexpr size_t value = I;
		using type = T;
		using functor = Functor;
	};
	template <size_t I, typename Functor>
	struct find_meta_result<I, tuple_void_t, Functor>
	{
		static constexpr size_t value = I;
		using functor = Functor;
	};

	template<typename Functor, typename Tuple, size_t begin = 0,
		typename Enable = void>
	struct find_meta {};
	template<typename Functor, typename Tuple, size_t begin>
	struct find_meta<Functor, Tuple, begin,
		std::enable_if_t<(begin < std::tuple_size_v<Tuple>)>>
	{
		private:
		using element_t = std::tuple_element_t<begin, Tuple>;
		using result = typename Functor::template bound<element_t>;

		using rebound = rebind_meta<Functor, result>;

		public:
		using type = std::conditional_t< result::value,
			find_meta_result<begin, element_t, rebound>,
			typename find_meta<rebound, Tuple, begin+1>::type >;

	};
	template<typename Functor, typename Tuple, size_t begin>
	struct find_meta<Functor, Tuple, begin,
		std::enable_if_t<(begin == std::tuple_size_v<Tuple>)>>
	{
		using type = find_meta_result<std::tuple_size_v<Tuple>, tuple_void_t, Functor>;
	};
	template<typename Functor, typename Tuple, size_t begin = 0>
	using find_meta_t = typename find_meta<Functor, Tuple, begin>::type;

	template<typename T, typename Tuple, size_t begin = 0>
	constexpr auto find_v = find_meta_t<
		bind_meta<std::is_same, T>,
		Tuple, begin
	>::value;

	template<template <typename...> typename Op, typename Tuple, size_t begin = 0>
	using find_if_t = typename find_meta_t<
		bind_meta<Op>, Tuple, begin
	>::type;

	template<template <typename...> typename Op, typename Tuple, size_t begin = 0>
	constexpr auto find_if_v = find_meta_t<
		bind_meta<Op>, Tuple, begin
	>::value;

} // namespace simple::support

#endif /* end of include guard */
