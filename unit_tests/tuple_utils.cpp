#include <cassert>
#include <string>
#include <sstream>
#include "simple/support/tuple_utils.hpp"
#include "simple/support/function_utils.hpp"

using namespace simple::support;
using namespace std::literals;

void ApplyFor()
{
	auto t = std::tuple(0," one ",2," three");
	std::stringstream ss;

	apply_for({0,4}, [&ss](auto&& a)
	{
		ss << a;
	}, t);
	assert(ss.str() == "0 one 2 three");
	ss.str("");
	ss.clear();

	apply_for({1,3}, [&ss](auto&& a)
	{
		ss << a;
	}, t);
	assert(ss.str() == " one 2");
	ss.str("");
	ss.clear();

	apply_for(3, [&ss](auto&& a)
	{
		ss << a;
	}, t);
	assert(ss.str() == " three");
	ss.str("");
	ss.clear();

	auto t2 = std::tuple(0,1.1,2,3);

	apply_for(2, [](auto&& x)
			{ x += 2; }, t2) ;
	assert( std::tuple(0,1.1,4,3) == t2 );

	apply_for({1, 3}, [](auto&& x)
			{ x += 2; }, t2) ;
	assert( std::tuple(0,3.1,6,3) == t2 );

	auto t3 = std::tuple(0,1,2,3);
	assert( 4 == apply_for(2, [](auto&& x)
			{ return x + 2; }, t3) );

}

template <typename... T>
constexpr auto tuple_tie(std::tuple<T...>& t)
{
	return std::apply(std::tie<T...>, t);
}

void CarCdr()
{

	{ auto t = std::tuple(true, 1.5, 5);
		assert(tuple_car(t));
		assert(1.5 == tuple_car(tuple_cdr(t)));
	}

	{ auto t = std::tuple(1, 2, 3);
		auto tref = tuple_tie(t);

		assert(t == tref);

		tuple_car(tref) = -4;

		assert(tuple_car(t) == -4);
		assert(t == tref);

		auto tref_cdr = tuple_tie_cdr(tref);

		tuple_car(tref_cdr) = 13;

		assert(std::get<1>(t) == 13);
		assert(tref_cdr == std::tuple(13,3));

		auto tref_cdr2 = tuple_tie_cdr(t);
		tuple_car(tref_cdr2) = 12;

		assert(std::get<1>(t) == 12);
		assert(tref_cdr2 == tref_cdr);
		assert(tref_cdr2 == std::tuple(12,3));
		assert(tref_cdr == std::tuple(12,3));

		auto tref_cdr3 = tuple_tie_cdr(tref_cdr2);
		tuple_car(tref_cdr3) = 99;
		assert(std::get<2>(t) == 99);
		assert(tref_cdr3 == std::tuple(99));

		auto tref_cdr4 = tuple_cdr(tref_cdr2);
		tuple_car(tref_cdr4) = 88;
		assert(std::get<2>(t) != 88);
		assert(tref_cdr4 == std::tuple(88));
	}
}

void TransformBasic()
{

	assert
	((
		transform(std::plus<>{},
			std::tuple{1, "2"s, 3.4}, std::tuple{4, "3"s, 2.1})
		== std::tuple{5, "23"s, 5.5}
	));

	assert
	((
		transform(std::plus<>{},
			std::array{1, 2, 3}, std::tuple{3, 2, 1})
		== std::tuple{4, 4, 4}
	));

	assert
	((
		transform(std::plus<>(),
			std::tuple{1, 2, 3.3}, std::array{3, 2, 1})
		== std::tuple{4, 4, 4.3}
	));

	assert
	((
		transform(std::plus<>{},
			std::tuple{1, 2, 3.3, 4, 5}, std::array{3, 2, 1})
		== std::tuple{4, 4, 4.3}
	));

	assert
	((
		transform
		(
			[](auto... x) { return (x + ...); },
			std::tuple{"1"s, 2},
			std::tuple{"3"s, 4},
			std::tuple{"5"s, 6}
		)
		== std::tuple{"135"s, 12}
	));

}

struct counter
{
	static size_t inst;
	static size_t copy;
	static size_t move;

	counter() { ++inst; };
	counter(const counter&) {  ++copy; }
	counter(counter&&) {  ++move; }
};
size_t counter::inst = 0;
size_t counter::copy = 0;
size_t counter::move = 0;

void TransformCopyCount()
{

	transform([](auto&& x) { return std::forward<decltype(x)>(x); }, std::tuple{counter{}, counter{}, counter{}} );
	assert(3 == counter::inst);
	assert(0 == counter::copy);
	assert(0 < counter::move);

}

void TransformReturnVoid()
{

	assert
	((
		transform([](auto...){},
			std::tuple{1, 2, 3.3}, std::array{3, 2, 1})
		== std::tuple{tuple_void, tuple_void, tuple_void}
	));

	assert
	((
		transform
		(
			overloaded
			{
				[](std::string, int){},
				std::plus<>{},
			},
			std::tuple{1, "2"s, "3"s},
			std::tuple{3, 2,    "1"s}
		)
		== std::tuple{4, tuple_void, "31"s}
	));

}

template <size_t I, size_t S>
constexpr iteration_state<I,std::make_index_sequence<S>> i_state{};

struct
{
	template <size_t Index, typename Range, typename... Values>
	auto operator()(iteration_state<Index,Range>, Values... v)
	{
		return (Range::size() - Index) * (v + ...);
	}
} sum_mul_index;

void TransformIterationState()
{

	assert
	((
		transform([](auto i, auto, auto) { return i; },
			std::tuple{1, 2, 3.3}, std::array{3, 2, 1})
		== std::tuple{i_state<0,3>, i_state<1,3>, i_state<2,3>}
	));

	assert
	((
		transform([](auto i, auto, auto) { return i; },
			std::tuple{1, 2, 3.3, 4, 5}, std::array{3, 2, 1})
		== std::tuple{i_state<0,3>, i_state<1,3>, i_state<2,3>}
	));

	assert
	((
		transform(sum_mul_index,
			std::tuple{1, 2, 3.3}, std::array{3, 2, 1, 0, -1, -2})
		== std::tuple{4*3,4*2,4.3*1}
	));

}

struct unlike_tuple {};
template <typename F>
std::string transform(F&&, const unlike_tuple&)
{return "Don't be too greedy!"; }

struct like_tuple
{
	int a;
	std::string b;

	bool operator==(const like_tuple& other) const
	{ return a == other.a && b == other.b; }
};

template <>
struct std::tuple_size<like_tuple> : public std::integral_constant<size_t, 2> {};

template <size_t I, typename T,
	std::enable_if_t<std::is_same_v<std::decay_t<T>,like_tuple>>* = nullptr>
decltype(auto) get(T&& t) noexcept
{
	static_assert(I < 2);
	if constexpr (I == 0)
	{
		return std::forward<T>(t).a;
	}
	else
	{
		return std::forward<T>(t).b;
	}
}

void TransformNonTuple()
{

	assert
	((
		from_tuple<like_tuple>(transform(
			std::plus<>{}, like_tuple{1,"1"s}, std::tuple{2,"2"s} ))
		==
		like_tuple{3, "12"s}
	));

	assert( transform([](){ return "Too greedy!"; }, unlike_tuple{}) == "Don't be too greedy!" );

}

int main()
{
	ApplyFor();
	CarCdr();
	TransformBasic();
	TransformCopyCount();
	TransformReturnVoid();
	TransformIterationState();
	TransformNonTuple();
	return 0;
}
