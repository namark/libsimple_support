#ifndef SIMPLE_SUPPORT_TUPLE_UTILS_APPLY_FOR_HPP
#define SIMPLE_SUPPORT_TUPLE_UTILS_APPLY_FOR_HPP

#include <tuple>
#include <functional>
#include <cassert>
#include "../range.hpp"
#include "common.hpp"

namespace simple::support
{

	template <typename T, typename = std::nullptr_t>
	struct has_tuple_interface : public std::false_type {};

	template <typename T>
	struct has_tuple_interface<T, decltype
	(
		void(std::tuple_size<std::decay_t<T>>::value),
		nullptr
	) > : public std::true_type {};

	template <typename T>
	constexpr bool has_tuple_interface_v = has_tuple_interface<T>::value;

	// wow this is a mess... you think it'll be optimized? O_o
	//TODO: return a variant
	//TODO: rename to transform
	template<typename F, typename First, size_t I = std::tuple_size_v<std::remove_reference_t<First>> - 1, typename... Rest>
	constexpr
	decltype(auto) apply_for(size_t index, F&& f, First&& first, Rest&&... rest)
	{
		using std::remove_reference_t;
		using First_t = remove_reference_t<First>;
		using std::apply;
		using std::get;
		using std::tuple_element_t;
		using std::forward;
		static_assert(I >= 0 && I < std::tuple_size_v<First_t>);
		if(I == index)
			// std::invoke is not constexpr -_-
			return apply(forward<F>(f), std::forward_as_tuple(
					forward<tuple_element_t<I, First_t>>(get<I>(first)),
					forward<tuple_element_t<I, remove_reference_t<Rest>>>(get<I>(rest))...
			));
		if constexpr (I > 0)
			return apply_for<F, decltype(forward<First_t>(first)), I - 1>(index, forward<F>(f),
					forward<First_t>(first), forward<remove_reference_t<Rest>>(rest)...);

		assert(!"simple::support::apply_for - index out of bounds");
	}

	//TODO: return a tuple of variants
	//TODO: rename to transform
	template<typename F, typename First, size_t I = std::tuple_size_v<std::remove_reference_t<First>> - 1, typename... Rest>
	constexpr
	void apply_for(range<size_t> index_range, F&& f, First&& first, Rest&&... rest)
	{
		for(size_t i = index_range.lower(); i < index_range.upper(); ++i)
		{
			apply_for(i, std::forward<F>(f),
				std::forward<std::remove_reference_t<First>>(first),
				std::forward<std::remove_reference_t<Rest>>(rest)...);
		}
	}

	template <typename First, typename... Rest>
	constexpr size_t min_tuple_size()
	{
		size_t result = std::tuple_size_v<First>;
		if constexpr(sizeof...(Rest) != 0)
			result = std::min(result, min_tuple_size<Rest...>());
		return result;
	}

	template <typename... T>
	constexpr size_t min_tuple_size_v = min_tuple_size<T...>();

	template <typename... T>
	using min_tuple_index_sequence = std::make_index_sequence<min_tuple_size_v<std::decay_t<T>...>>;

	template <size_t Index, typename Range>
	// TODO: enable_if_t<Range is integer_sequence and Index is in Range>
	struct iteration_state
	{
		constexpr static std::integral_constant<decltype(Index), Index> index{};
		constexpr static Range range{};
	};
	template <size_t I, typename R>
	[[nodiscard]] constexpr bool
	operator==(const iteration_state<I,R>, const iteration_state<I,R>)
	noexcept { return true; }
	template <size_t I, typename R>
	[[nodiscard]] constexpr bool
	operator!=(const iteration_state<I,R>, const iteration_state<I,R>) noexcept
	{ return false; }

	template<typename F>
	constexpr decltype(auto) transform(F&& f)
	//TODO: noexcept(-_-)
	{
		return std::apply(std::forward<F>(f));
	}

	template<size_t Index, typename F, typename... Tuples,
		std::enable_if_t<(has_tuple_interface_v<Tuples> && ...)>* = nullptr,
		std::enable_if_t<not std::is_invocable<F,
			decltype(get<Index>(std::declval<Tuples>()))...>::value>* = nullptr,
		std::enable_if_t<std::is_invocable<F, iteration_state<Index, min_tuple_index_sequence<Tuples...>>,
			decltype(get<Index>(std::declval<Tuples>()))...>::value>* = nullptr >
	constexpr
	decltype(auto) transform(F&& f, Tuples&&... tuples)
	//TODO: noexcept(-_-)
	{
		// std::invoke is not contexpr in C++17 :V
		auto info = iteration_state<Index, min_tuple_index_sequence<Tuples...>>{};
		return std::apply(std::forward<F>(f),
			std::forward_as_tuple(info, get<Index>(std::forward<Tuples>(tuples))...) );
	}

	template<size_t Index, typename F, typename... Tuples,
		std::enable_if_t<(has_tuple_interface_v<Tuples> && ...)>* = nullptr,
		std::enable_if_t<std::is_invocable<F,
			decltype(get<Index>(std::declval<Tuples>()))...>::value>* = nullptr >
	constexpr
	decltype(auto) transform(F&& f, Tuples&&... tuples)
	//TODO: noexcept(-_-)
	{
		// std::invoke is not contexpr in C++17 :V
		return std::apply(std::forward<F>(f),
			std::forward_as_tuple(get<Index>(std::forward<Tuples>(tuples))...) );
	}

	template<typename F, typename... Tuples, size_t... Indecies,
		std::enable_if_t<(has_tuple_interface_v<Tuples> && ...)>* = nullptr>
	constexpr
	decltype(auto) transform(F&& f, std::index_sequence<Indecies...>, Tuples&&... tuples)
	//TODO: noexcept(-_-)
	{
		//TODO: pass the index sequence through for use in iteration_state
		return std::tuple
		{
			(transform<Indecies>(std::forward<F>(f), std::forward<Tuples>(tuples)...),
				tuple_void )
			...
		};
	}

	template<typename F, typename... Tuples,
		std::enable_if_t<(has_tuple_interface_v<Tuples> && ...)>* = nullptr>
	constexpr
	decltype(auto) transform(F&& f, Tuples&&... tuples)
	//TODO: noexcept(-_-)
	{
		return transform(std::forward<F>(f),
			min_tuple_index_sequence<Tuples...>{} ,
			std::forward<Tuples>(tuples)...);
	}

	namespace detail
	{
		template <typename T, typename Tuple, std::size_t... I>
		constexpr T from_tuple_impl( Tuple&& t, std::index_sequence<I...> )
		{
			return T{get<I>(std::forward<Tuple>(t))...};
		}
	} // namespace detail

	template <typename T, typename Tuple>
	constexpr T from_tuple( Tuple&& t )
	{
		return detail::from_tuple_impl<T>(std::forward<Tuple>(t),
			std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
	}

} // namespace simple::support

#endif /* end of include guard */
