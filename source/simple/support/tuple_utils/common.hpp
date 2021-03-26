#ifndef SIMPLE_SUPPORT_TUPLE_UTILS_COMMON_HPP
#define SIMPLE_SUPPORT_TUPLE_UTILS_COMMON_HPP

#include <utility>

namespace simple::support
{

	constexpr struct tuple_void_t {} tuple_void;
	[[nodiscard]] constexpr bool operator==(const tuple_void_t, const tuple_void_t) noexcept { return true; }
	[[nodiscard]] constexpr bool operator!=(const tuple_void_t, const tuple_void_t) noexcept { return false; }

	template <typename T>
	constexpr decltype(auto) operator,(T&& x, const tuple_void_t)
	//TODO: noexcept(-_-)
	{
		return std::forward<T>(x);
	}

} // namespace simple::support


#endif /* end of include guard */
