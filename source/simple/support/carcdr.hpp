#ifndef SIMPLE_SUPPORT_CARCDR_HPP
#define SIMPLE_SUPPORT_CARCDR_HPP

#include <cstdint>
#include <utility>

namespace simple::support
{
	using std::size_t;

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

	template <typename List, size_t = 0, auto ... Rest>
	std::nullptr_t car;

	template <typename List, size_t N,
		typename List::value_type nil>
	constexpr typename List::value_type
	car<List, N, nil> = []()
	{
		if constexpr ( N >= List::size() )
			return nil;
		else
			return car<List, N>;
	}();

	template <typename List, size_t N>
	constexpr typename List::value_type
	car<List, N> = []()
	{
		static_assert( N < List::size() );
		if constexpr (N == 0)
			return car<List>;
		else
			return car<cdr<List>, N - 1>;
	}();

	template <typename List>
	constexpr typename List::value_type
	car<List, 0> = []()
	{
		static_assert( List::size() > 0 );
		return carcdr<List>::car;
	}();

} // namespace simple::support

#endif /* end of include guard */
