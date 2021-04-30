#ifndef SIMPLE_SUPPORT_TUPLE_UTILS_META_BIND_HPP
#define SIMPLE_SUPPORT_TUPLE_UTILS_META_BIND_HPP

#include <type_traits>
#include <tuple>

namespace simple::support
{

	template <typename... Bindings>
	struct first_binding_meta {};
	template <typename First, typename... Rest>
	struct first_binding_meta<First, Rest...>
	{
		using binding = First;
	};

	template <template <typename...> typename F,
		typename... Bindings>
	struct bind_meta : first_binding_meta<Bindings...>
	{
		template <typename... Ts> using function = F<Ts...>;

		template <typename... Rest>
		struct bound : F<Bindings..., Rest...> { };

		using bindings = std::tuple<Bindings...>;
	};

	template <template <typename...> typename F,
		typename Bindings>
	struct bind_tuple_meta;
	template <template <typename...> typename F,
		typename... Bindings>
	struct bind_tuple_meta<F, std::tuple<Bindings...>> :
		bind_meta<F, Bindings...> {};

	template <typename T, typename Enable = std::nullptr_t>
	struct has_meta_bindings : std::false_type {};
	template <typename T>
	struct has_meta_bindings<T,
		// NOTE: is_object here is not checked, just serves as a type context,
		// to fail if binding are not defined at all.
		// can't use decltype as you can't just put anything in a tuple apparently:
		// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=92570
		decltype(std::is_object_v<typename T::bindings>, nullptr)>
		: std::true_type {};
	template <typename T>
	constexpr auto has_meta_bindings_v = has_meta_bindings<T>::value;

	template <typename T, typename Enable = std::nullptr_t>
	struct has_meta_binding : std::false_type {};
	template <typename T>
	struct has_meta_binding<T,
		decltype(void(std::is_object_v<typename T::binding>), nullptr)>
		: std::true_type {};
	template <typename T>
	constexpr auto has_meta_binding_v = has_meta_binding<T>::value;

	template <typename T, typename Enabled = void>
	struct get_meta_bindings;
	template <typename T>
	struct get_meta_bindings<T,
		std::enable_if_t<has_meta_bindings_v<T>>>
	{
		using type = typename T::bindings;
	};
	template <typename T>
	struct get_meta_bindings<T,
		std::enable_if_t<has_meta_binding_v<T>>>
	{
		using type = std::tuple<typename T::binding>;
	};

	// NOTE: could maybe use concat_t and subtuple_t here for better readability, but didn't want to add dependencies
	template <typename Tuple1, typename Tuple2, typename Diff =
		std::make_index_sequence<
			std::tuple_size_v<Tuple2> - std::tuple_size_v<Tuple1>>>
	struct overlap_meta;
	template <typename... Over, typename... Under, std::size_t... Diff>
	struct overlap_meta<std::tuple<Over...>, std::tuple<Under...>,
		std::index_sequence<Diff...>>
	{
		using type = std::tuple<Over...,
			std::tuple_element_t<Diff+sizeof...(Over), std::tuple<Under...>>...>;
	};
	template <typename Tuple1, typename Tuple2>
	using overlap_t = typename overlap_meta<Tuple1, Tuple2>::type;

	template <typename Functor, typename Result,
		typename Enabled = void>
	struct rebind_meta {};
	// NOTE: use ::type _t pattern instead of inheriting to generate less new types ??
	template <typename Functor, typename Result>
	struct rebind_meta
	<
		Functor, Result,
		std::enable_if_t<has_meta_bindings_v<Result> || has_meta_binding_v<Result>>
	>
		: bind_tuple_meta
		<
			Functor::template function,
			overlap_t
			<
				typename get_meta_bindings<Result>::type,
				typename Functor::bindings
			>
		>
	{};
	template <typename Functor, typename Result>
	struct rebind_meta
	<
		Functor, Result,
		std::enable_if_t<
			not (has_meta_bindings_v<Result> || has_meta_binding_v<Result>) >
	>
		: Functor
	{};

} // namespace simple::support

#endif /* end of include guard */
