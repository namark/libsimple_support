#ifndef SIMPLE_SUPPORT_LOGIC_HPP
#define SIMPLE_SUPPORT_LOGIC_HPP

#include <type_traits>

namespace simple { namespace support
{

	template <bool...>
	class bools;

	template <bool... vars>
	using and_logic =
		std::is_same<bools<true, vars...>, bools<vars..., true>>;

	template <bool... vars>
	using or_logic = std::integral_constant <bool,
		!std::is_same<bools<false, !vars...>, bools<!vars..., false>>::value >;

	template <bool... conditions>
	using enable_if_all = std::enable_if_t<and_logic<conditions...>::value>;

	template <bool... conditions>
	using enable_if_any = std::enable_if_t<or_logic<conditions...>::value>;

}} // namespace simple.support

#endif /* end of include guard */
