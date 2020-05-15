#ifndef SIMPLE_SUPPORT_TYPE_TRAITS_HPP
#define SIMPLE_SUPPORT_TYPE_TRAITS_HPP

#include <type_traits>

namespace simple::support
{

	template <template <typename...> typename, typename>
	struct is_template_instance : public std::false_type {};

	template <template <typename...> typename U, typename...Ts>
	struct is_template_instance<U, U<Ts...>> : public std::true_type {};

	template <template <typename...> typename U, typename...Ts>
	constexpr auto is_template_instance_v = is_template_instance<U, Ts...>::value;

	template <typename T>
	using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

} // namespace simple::support

#endif /* end of include guard */
