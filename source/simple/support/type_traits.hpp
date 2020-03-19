#ifndef SIMPLE_SUPPORT_TYPE_TRAITS_HPP
#define SIMPLE_SUPPORT_TYPE_TRAITS_HPP

#include <type_traits>

namespace simple::support
{

	template <typename, template <typename...> typename>
	struct is_template_instance : public std::false_type {};

	template <typename...Ts, template <typename...> typename U>
	struct is_template_instance<U<Ts...>, U> : public std::true_type {};

	template <typename...Ts, template <typename...> typename U>
	constexpr auto is_template_instance_v = is_template_instance<Ts...,U>::value;

} // namespace simple::support

#endif /* end of include guard */
