#ifndef SIMPLE_SUPPORT_TUPLE_UTILS_PEND_HPP
#define SIMPLE_SUPPORT_TUPLE_UTILS_PEND_HPP

#include <tuple>

namespace simple::support
{

	template <typename Tuple, typename... Ts>
	struct append_meta;

	template <typename... First, typename... Last>
	struct append_meta<std::tuple<First...>, Last...>
	{
		using type = std::tuple<First..., Last...>;
	};

	template <typename Tuple, typename... Ts>
	using append_t = typename append_meta<Tuple,Ts...>::type;

	// NOTE: reverse the order of params for prepend?
	// so that p(p(p(t, a), b), c) is same as p(t, a, b, c) ??
	template <typename Tuple, typename... Ts>
	struct prepend_meta;

	template <typename... First, typename... Last>
	struct prepend_meta<std::tuple<First...>, Last...>
	{
		using type = std::tuple<Last..., First...>;
	};

	template <typename Tuple, typename... Ts>
	using prepend_t = typename prepend_meta<Tuple,Ts...>::type;

	template <typename Tuple, typename T>
	struct concat_meta;

	template <typename... First, typename... Last>
	struct concat_meta<std::tuple<First...>, std::tuple<Last...>>
	{
		using type = std::tuple<First..., Last...>;
	};

	template <typename Tuple, typename T>
	using concat_t = typename concat_meta<Tuple,T>::type;


} // namespace simple::support

#endif /* end of include guard */
