#ifndef SIMPLE_SUPPORT_MATH_ROOT_HPP
#define SIMPLE_SUPPORT_MATH_ROOT_HPP

#include <cassert>
#include <limits>
#include <type_traits>
#include "../range.hpp"
#include "../algorithm.hpp"
#include "float.hpp"

namespace simple::support
{

	// i know i know BABYlonian... babel reads better
	//
	// this is a clever way of guessing the square root
	// that looks too complicated when explained in mathy ways
	//
	// if our number N is R*R (so R is the root we want)
	// and we guess certain very wrong number G as root
	// and also consider N/G
	// there are two possibilities
	// 1. G > R => R > N/G
	// 2. G < R => R < N/G
	//
	// as you can see R is always somewhere in between G and N/G
	// so we take the midpoint between the two as next guess
	// until G*G is close enough to R*R (which is N)
	template <typename Number, typename EqualityCheck>
	[[nodiscard]] constexpr
	Number babelonian_root2(Number n, Number g,
		EqualityCheck eq)
	{
		assert(n >= 0 && g > 0);
		do
			g = avarage(g, n/g);
		while( !eq(n, g*g) );
		return g;
	}

	template <typename Number, typename EqualityCheck>
	[[nodiscard]] constexpr
	Number babelonian_root2(Number n, EqualityCheck eq)
	{
		// the number itself is a good neutral initial guess
		// not biased towards large or small numbers
		return babelonian_root2(n, n,
			std::forward<EqualityCheck>(eq));
	}

	// for floating point types, need to handle some special cases
	// we do what standard sqrt does here more or less
	template <typename Number>
	[[nodiscard]] constexpr
	Number babelonian_root2_f(Number n) noexcept
	{
		static_assert(std::is_floating_point_v<Number>);

		if(n < -Number{0}) // no idea why -0 but that's what it says
			return Number{0}/Number{0}; // has quite nan? has signaling nan? i don't know, so there you go
		using limits = std::numeric_limits<Number>;
		if(n!=n || abs(n) < limits::min() || abs(n) == limits::infinity()) return n; // hand rolled std::isnormal

		return babelonian_root2(n, almost_equal<Number>);
	}


	// below are some shananigans for a magic root2 function
	// that uses builting sqrt functions if they are constexpr
	// and if not defaults to babelonian_root2_f

	namespace detail
	{

		template <typename Expression, int=(Expression::eval(),0)>
		constexpr bool is_constexpr(Expression)
		{ return true; }

		template <typename... AnythingElse>
		constexpr bool is_constexpr(AnythingElse...)
		{ return false; }

	} // namespace detail

#if defined __GNUC__
#define simple_support_math__builtin_sqrtf __builtin_sqrtf
#define simple_support_math__builtin_sqrtl __builtin_sqrtl
#define simple_support_math__builtin_sqrt __builtin_sqrt
#else
#include<cmath>
#define simple_support_math__builtin_sqrtf std::sqrt
#define simple_support_math__builtin_sqrt std::sqrt
#define simple_support_math__builtin_sqrtl std::sqrt
#endif

#define SIMPLE_SUPPORT_MATH_DEFINE_FLOAT_ROOT2(type, builtin)   \
	namespace detail                                            \
	{                                                           \
		struct builtin##_expression                             \
		{                                                       \
			constexpr static auto eval =                        \
				[](){ void(builtin(2)); };                      \
		};                                                      \
	} /* namespace detail */                                    \
                                                                \
	[[nodiscard]]                                               \
	constexpr type root2(type n) noexcept                       \
	{                                                           \
		using namespace detail;                                 \
		if constexpr ( is_constexpr(builtin##_expression{}))    \
			return builtin(n);                                  \
		else                                                    \
			return babelonian_root2_f(n);                       \
	}

	SIMPLE_SUPPORT_MATH_DEFINE_FLOAT_ROOT2(float, simple_support_math__builtin_sqrtf)
	SIMPLE_SUPPORT_MATH_DEFINE_FLOAT_ROOT2(double, simple_support_math__builtin_sqrt)
	SIMPLE_SUPPORT_MATH_DEFINE_FLOAT_ROOT2(long double, simple_support_math__builtin_sqrtl)
#undef SIMPLE_SUPPORT_MATH_DEFINE_FLOAT_ROOT2

#undef simple_support_math__builtin_sqrtf
#undef simple_support_math__builtin_sqrt
#undef simple_support_math__builtin_sqrtl

} // namespace simple::support

#endif /* end of include guard */
