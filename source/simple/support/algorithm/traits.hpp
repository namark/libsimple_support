#ifndef SIMPLE_SUPPORT_ALGORITHM_TRAITS_HPP
#define SIMPLE_SUPPORT_ALGORITHM_TRAITS_HPP
#include <iterator>

namespace simple::support
{

	namespace detail
	{

		using std::begin;
		using std::end;

		template <typename T, typename = std::nullptr_t>
		struct is_range_helper
		: public std::false_type {};
		template <typename T>
		struct is_range_helper<T, decltype(
			begin(std::declval<T>()) != end(std::declval<T>()),
			nullptr)>
		: public std::true_type {};

	} // namespace detail

	template <typename T, typename = std::nullptr_t>
	struct is_range : public detail::is_range_helper<T> {};
	template <typename T>
	constexpr auto is_range_v = is_range<T>::value;

} // namespace simple::support

#endif /* end of include guard */
