#ifndef SIMPLE_SUPPORT_CARCDR_HPP
#define SIMPLE_SUPPORT_CARCDR_HPP

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
		using type = std::integer_sequence<Int, Cdr...>;
	};

	template <typename IntSeq>
	using cdr = typename carcdr<IntSeq>::type;

	template <typename IntSeq>
	constexpr typename IntSeq::value_type car()
	{
		static_assert( IntSeq::size() > 0 );
		return carcdr<IntSeq>::car;
	}

	template <typename IntSeq>
	constexpr typename IntSeq::value_type car(typename IntSeq::value_type value)
	{
		if constexpr ( IntSeq::size() > 0 )
			return (void)value, carcdr<IntSeq>::car;
		else
			return value;
	}

	template <typename IntSeq, size_t N>
	constexpr typename IntSeq::value_type car()
	{
		static_assert( N < IntSeq::size() );
		if constexpr (N == 0)
			return car<IntSeq>();
		else
			return car<cdr<IntSeq>, N - 1>();
	}

	template <typename IntSeq, size_t N>
	constexpr typename IntSeq::value_type car(typename IntSeq::value_type value)
	{
		if constexpr ( N >= IntSeq::size() )
			return value;
		else if constexpr (N == 0)
			return car<IntSeq>(value);
		else
			return car<cdr<IntSeq>, N - 1>(value);
	}

} // namespace simple::support

#endif /* end of include guard */
