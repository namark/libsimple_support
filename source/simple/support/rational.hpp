#ifndef SIMPLE_SUPPORT_RATIONAL_HPP
#define SIMPLE_SUPPORT_RATIONAL_HPP

#include "array.hpp"
#include "array_operators.hpp"

namespace simple::support
{

	// TODO: find and use a proper rational type
	template <typename Int>
	struct rational : public simple::support::array<Int, 2>
	{
		enum
		{
			numerator,
			denominator
		};

		constexpr rational& operator*=(const Int& value)
		{
			(*this)[numerator] *= value;
			return *this;
		}

		constexpr operator Int() const
		{
			return (*this)[numerator] / (*this)[denominator];
		}
	};

	template <typename Int>
	constexpr rational<Int> operator*(rational<Int> ratio, const Int& value)
	{
		ratio *= value;
		return ratio;
	}

	template <typename Int>
	constexpr rational<Int> operator*(const Int& value, rational<Int> ratio)
	{
		ratio *= value;
		return ratio;
	}

	template <typename Int> rational(Int, Int) -> rational<Int>;

} // namespace simple::support

namespace simple
{
	template<typename T>
	struct support::define_array_operators<support::rational<T>> :
	public support::trivial_array_accessor<support::rational<T>, 2>
	{
		constexpr static auto enabled_right_element_operators = array_operator::none;
		constexpr static auto enabled_left_element_operators = array_operator::none;
		constexpr static auto enabled_operators =
			array_operator::mul |
			array_operator::mul_eq |
			array_operator::none;
	};
} // namespace simple

#endif /* end of include guard */
