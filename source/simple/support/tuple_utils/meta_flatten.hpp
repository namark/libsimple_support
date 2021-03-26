#ifndef SIMPLE_SUPPORT_TUPLE_UTILS_META_FLATTEN_HPP
#define SIMPLE_SUPPORT_TUPLE_UTILS_META_FLATTEN_HPP

#include "meta_find.hpp"
#include "pend.hpp"

namespace simple::support
{

	template <typename T>
	struct flatten_meta_operator { using type = std::tuple<T>; };

	template <typename T>
	struct flatten_meta_operator_default : flatten_meta_operator<T> {};
	template <typename... Ts>
	struct flatten_meta_operator_default<std::tuple<Ts...>>
	{ using type = std::tuple<Ts...>; };

	template <typename Result, typename Op, typename T>
	struct flatten_meta_find_adapter : std::false_type
	{
		using binding = concat_t<Result, typename Op::template function<T>::type>;
	};

	template <typename Tuple, template <typename...> typename Operator =
		flatten_meta_operator_default>
	using flatten_t = typename find_meta_t
	<
		bind_meta<flatten_meta_find_adapter, std::tuple<>, bind_meta<Operator>>,
		Tuple
	>::functor::binding;

} // namespace simple::support

#endif /* end of include guard */
