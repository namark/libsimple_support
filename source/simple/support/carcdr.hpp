#ifndef SIMPLE_SUPPORT_CARCDR_HPP
#define SIMPLE_SUPPORT_CARCDR_HPP

#include <tuple>
#include <utility>

namespace simple::support
{

	template <typename Int, Int... Values>
	using lisp_list = std::integer_sequence<Int, Values...>;

	template <typename IntSec>
	struct carcdr;

	template <typename Int, Int Car, Int... Cdr>
	struct carcdr<std::integer_sequence<Int, Car, Cdr...>>
	{
		constexpr static Int car = Car;
		using cdr = std::integer_sequence<Int, Cdr...>;
	};

	template <typename IntSeq>
	using cdr = typename carcdr<IntSeq>::cdr;

	template <typename IntSeq>
	constexpr typename IntSeq::value_type car() noexcept
	{
		static_assert( IntSeq::size() > 0 );
		return carcdr<IntSeq>::car;
	}

	template <typename IntSeq>
	constexpr typename IntSeq::value_type car(typename IntSeq::value_type value) noexcept
	{
		if constexpr ( IntSeq::size() > 0 )
			return (void)value, carcdr<IntSeq>::car;
		else
			return value;
	}

	template <typename IntSeq, size_t N>
	constexpr typename IntSeq::value_type car() noexcept
	{
		static_assert( N < IntSeq::size() );
		if constexpr (N == 0)
			return car<IntSeq>();
		else
			return car<cdr<IntSeq>, N - 1>();
	}

	template <typename IntSeq, size_t N>
	constexpr typename IntSeq::value_type car(typename IntSeq::value_type value) noexcept
	{
		if constexpr ( N >= IntSeq::size() )
			return value;
		else if constexpr (N == 0)
			return car<IntSeq>(value);
		else
			return car<cdr<IntSeq>, N - 1>(value);
	}

	template <typename... Rest, size_t... indices>
	constexpr auto subtuple(const std::tuple<Rest...>& tuple, std::integer_sequence<size_t, indices...>)
	{
		return std::make_tuple(std::get<indices>(tuple)...);
	}

	template <typename First, typename... Rest>
	constexpr auto tuple_car(const std::tuple<First, Rest...>& tuple)
	{
		return std::get<0>(tuple);
	}

	constexpr std::nullptr_t tuple_car(const std::tuple<>&)
	{
		return nullptr;
	}

	template <typename First, typename... Rest>
	constexpr auto tuple_cdr(const std::tuple<First, Rest...>& tuple)
	{
		using indices = std::make_index_sequence<
			std::tuple_size_v<std::decay_t<decltype(tuple)>>>;
		return subtuple(tuple, cdr<indices>{});
	}

} // namespace simple::support

#endif /* end of include guard */
