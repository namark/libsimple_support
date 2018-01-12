#ifndef SIMPLE_SUPPORT_OPERATOR_MIXINS_HPP
#define SIMPLE_SUPPORT_OPERATOR_MIXINS_HPP

#include <type_traits>
#include <functional>
#include <limits>
#include <cstdint>
#include <cmath>

#include "enum_flags_operators.hpp"

namespace simple::support
{
	using std::size_t;

	enum class array_operator : std::uint32_t
	{
		none = 0,
		add_eq = 		1u << 0,
		add =			1u << 1,
		sub_eq =		1u << 2,
		sub =			1u << 3,
		mul_eq =		1u << 4,
		mul =			1u << 5,
		div_eq =		1u << 6,
		div =			1u << 7,
		mod_eq =		1u << 8,
		mod =			1u << 9,
		bit_and_eq =	1u << 10,
		bit_and =		1u << 11,
		bit_or_eq =		1u << 12,
		bit_or =		1u << 13,
		bit_xor_eq =	1u << 14,
		bit_xor =		1u << 15,
		bit_not =		1u << 16,
		negate =		1u << 17,
		lshift_eq =		1u << 18,
		lshift =		1u << 19,
		rshift_eq =		1u << 20,
		rshift =		1u << 21,
		all = 			std::numeric_limits<std::underlying_type_t<array_operator>>::max(),
		binary =		add | mul | sub | div | mod | bit_and | bit_or | bit_xor | lshift | rshift,
		unary =			bit_not | negate,
		in_place =		all ^ (binary | unary)
	};

	template<> struct define_enum_flags_operators<array_operator> : public std::true_type {};

	template <typename Type>
	struct add_in_place
	{
		constexpr Type& operator()(Type& one, const Type& other) const { return one += other; }
	};

	template <typename Type>
	struct sub_in_place
	{
		constexpr Type& operator()(Type& one, const Type& other) const { return one -= other; }
	};

	template <typename Type>
	struct mul_in_place
	{
		constexpr Type& operator()(Type& one, const Type& other) const { return one *= other; }
	};

	template <typename Type>
	struct div_in_place
	{
		constexpr Type& operator()(Type& one, const Type& other) const { return one /= other; }
	};

	template <typename Type>
	struct mod_in_place
	{
		constexpr Type& operator()(Type& one, const Type& other) const
		{
			if constexpr (std::is_floating_point_v<Type>)
				return one = std::fmod(one, other);
			else
				return one %= other;
		}
	};

	template <typename Type>
	struct mod
	{
		constexpr Type operator()(const Type& one, const Type& other) const
		{
			if constexpr (std::is_floating_point_v<Type>)
				return std::fmod(one, other);
			else
				return one % other;
		}
	};

	template <typename Type>
	struct bit_and_in_place
	{
		constexpr Type& operator()(Type& one, const Type& other) const { return one &= other; }
	};

	template <typename Type>
	struct bit_or_in_place
	{
		constexpr Type& operator()(Type& one, const Type& other) const { return one |= other; }
	};

	template <typename Type>
	struct bit_xor_in_place
	{
		constexpr Type& operator()(Type& one, const Type& other) const { return one ^= other; }
	};

	template <typename Type>
	struct lshift
	{
		constexpr Type operator()(const Type& one, const Type& other) const { return one << other; }
	};

	template <typename Type>
	struct rshift
	{
		constexpr Type operator()(const Type& one, const Type& other) const { return one >> other; }
	};

	template <typename Type>
	struct lshift_in_place
	{
		constexpr Type& operator()(Type& one, const Type& other) const { return one <<= other; }
	};

	template <typename Type>
	struct rshift_in_place
	{
		constexpr Type& operator()(Type& one, const Type& other) const { return one >>= other; }
	};

	template <typename Type>
	struct bit_not
	{
		constexpr Type operator()(const Type& one) const { return ~one; }
	};

	template <typename Type>
	struct define_array_operators
	{
		constexpr static size_t size = 0;
		constexpr static auto& get_array(Type& object);
		constexpr static array_operator enabled_operators = array_operator::none;
		constexpr static array_operator enabled_right_element_operators = array_operator::none;
		constexpr static array_operator enabled_left_element_operators = array_operator::none;
	};

	template <typename Type, size_t Size>
	struct trivial_array_accessor
	{
		constexpr static size_t size = Size;
		constexpr static Type& get_array(Type& object) noexcept { return object; }
		constexpr static const Type& get_array(const Type& object) noexcept { return object; }
	};

namespace AOps_Details
{

	template <typename Operator, size_t Size, typename Type>
	constexpr Type& array_unary_operator(Type& result, const Type& one)
	{
		auto op = Operator{};
		for(size_t i = 0; i < Size; ++i)
			result[i] = op(one[i]);
		return result;
	}

	template <typename Operator, size_t Size, typename Type>
	constexpr Type& array_in_place_operator(Type& one, const Type& other)
	{
		auto op = Operator{};
		for(size_t i = 0; i < Size; ++i)
			op(one[i], other[i]);
		return one;
	}

	template <typename Operator, size_t Size, typename Type, typename Element>
	constexpr Type& array_right_element_in_place_operator(Type& one, const Element& element)
	{
		auto op = Operator{};
		for(size_t i = 0; i < Size; ++i)
			op(one[i], element);
		return one;
	}

	template <typename Operator, size_t Size, typename Type, typename Element>
	constexpr Element& array_left_element_in_place_operator(Element& element, const Type& array)
	{
		auto op = Operator{};
		for(size_t i = 0; i < Size; ++i)
			op(element, array[i]);
		return element;
	}

	template <typename Operator, size_t Size, typename Type>
	constexpr Type& array_binary_operator(Type& result, const Type& one, const Type& other)
	{
		auto op = Operator{};
		for(size_t i = 0; i < Size; ++i)
			result[i] = op(one[i], other[i]);
		return result;
	}

	template <typename Operator, size_t Size, typename Type, typename Element>
	constexpr Type& array_right_element_binary_operator(Type& result, const Type& one, const Element& element)
	{
		auto op = Operator{};
		for(size_t i = 0; i < Size; ++i)
			result[i] = op(one[i], element);
		return result;
	}

	template <typename Operator, size_t Size, typename Type, typename Element>
	constexpr Type& array_left_element_binary_operator(Type& result, const Type& one, const Element& element)
	{
		auto op = Operator{};
		for(size_t i = 0; i < Size; ++i)
			result[i] = op(element, one[i]);
		return result;
	}

	template <typename OperatorDef, typename Type>
	using array_element_t = std::remove_reference_t<decltype(OperatorDef::get_array(std::declval<Type&>())[0])>;

} // namespace AOps_Details

} // namespace simple::support

// well, ended up with  macros after all... and still lots of duplication

#define SIMPLE_SUPPORT_DEFINE_UNARY_OPERATOR(op_symbol, op_type, op_fun) \
template <typename Array, \
	typename OperatorDef = simple::support::define_array_operators<Array>, \
	std::enable_if_t<OperatorDef::enabled_operators && simple::support::array_operator::op_type>* = nullptr> \
constexpr Array operator op_symbol (const Array& one) \
{ \
	using namespace simple::support; \
	using namespace simple::support::AOps_Details; \
	Array result{}; \
	array_unary_operator \
		<op_fun<array_element_t<OperatorDef, Array>>, OperatorDef::size> \
		(OperatorDef::get_array(result), OperatorDef::get_array(one)); \
	return result; \
}

#define SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR(op_symbol, op_type, op_fun) \
template <typename Array, \
	typename OperatorDef = simple::support::define_array_operators<Array>, \
	std::enable_if_t<OperatorDef::enabled_operators && simple::support::array_operator::op_type>* = nullptr> \
constexpr Array operator op_symbol (const Array& one, const Array& other) \
{ \
	using namespace simple::support; \
	using namespace simple::support::AOps_Details; \
	Array result{}; \
	array_binary_operator \
		<op_fun<array_element_t<OperatorDef, Array>>, OperatorDef::size> \
		(OperatorDef::get_array(result), OperatorDef::get_array(one), OperatorDef::get_array(other)); \
	return result; \
} \
\
template <typename Array, \
	typename OperatorDef = simple::support::define_array_operators<Array>, \
	std::enable_if_t<OperatorDef::enabled_right_element_operators && simple::support::array_operator::op_type>* = nullptr> \
constexpr Array operator op_symbol \
( \
	const Array& one, \
	const simple::support::AOps_Details::array_element_t<OperatorDef, Array> & other \
 ) \
{ \
	using namespace simple::support; \
	using namespace simple::support::AOps_Details; \
	Array result{}; \
	array_right_element_binary_operator \
		<op_fun<array_element_t<OperatorDef, Array>>, OperatorDef::size> \
		(OperatorDef::get_array(result), OperatorDef::get_array(one), other); \
	return result; \
} \
\
template <typename Array, \
	typename OperatorDef = simple::support::define_array_operators<Array>, \
	std::enable_if_t<OperatorDef::enabled_left_element_operators && simple::support::array_operator::op_type>* = nullptr> \
constexpr Array operator op_symbol \
( \
 	const simple::support::AOps_Details::array_element_t<OperatorDef, Array> & one, \
	const Array& other \
) \
{ \
	using namespace simple::support; \
	using namespace simple::support::AOps_Details; \
	Array result{}; \
	array_left_element_binary_operator \
		<op_fun<array_element_t<OperatorDef, Array>>, OperatorDef::size> \
	(OperatorDef::get_array(result), OperatorDef::get_array(other), one); \
	return result; \
}

#define SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR(op_symbol, op_type, op_fun) \
template <typename Array, \
	typename OperatorDef = simple::support::define_array_operators<Array>, \
	std::enable_if_t<OperatorDef::enabled_operators && simple::support::array_operator::op_type>* = nullptr> \
constexpr Array& operator op_symbol (Array& one, const Array& other) \
{ \
	using namespace simple::support; \
	using namespace simple::support::AOps_Details; \
	array_in_place_operator \
		<op_fun<array_element_t<OperatorDef, Array>>, OperatorDef::size> \
		(OperatorDef::get_array(one), OperatorDef::get_array(other)); \
	return one; \
} \
template <typename Array, \
	typename OperatorDef = simple::support::define_array_operators<Array>, \
	std::enable_if_t<OperatorDef::enabled_right_element_operators && simple::support::array_operator::op_type>* = nullptr> \
constexpr Array& operator op_symbol \
( \
	Array& one, \
  	const simple::support::AOps_Details::array_element_t<OperatorDef, Array> & other \
) \
{ \
	using namespace simple::support; \
	using namespace simple::support::AOps_Details; \
	array_right_element_in_place_operator \
		<op_fun<array_element_t<OperatorDef, Array>>, OperatorDef::size> \
		(OperatorDef::get_array(one), other); \
	return one; \
} \
template <typename Array, \
	typename OperatorDef = simple::support::define_array_operators<Array>, \
	std::enable_if_t<OperatorDef::enabled_left_element_operators && simple::support::array_operator::op_type>* = nullptr> \
constexpr auto operator op_symbol \
( \
	simple::support::AOps_Details::array_element_t<OperatorDef, Array>& one, \
	const Array& other \
) \
-> simple::support::AOps_Details::array_element_t<OperatorDef, Array> & \
{ \
	using namespace simple::support; \
	using namespace simple::support::AOps_Details; \
	array_left_element_in_place_operator \
		<op_fun<array_element_t<OperatorDef, Array>>, OperatorDef::size> \
		(one, OperatorDef::get_array(other)); \
	return one; \
}

SIMPLE_SUPPORT_DEFINE_UNARY_OPERATOR(~, bit_not, simple::support::bit_not);
SIMPLE_SUPPORT_DEFINE_UNARY_OPERATOR(-, negate, std::negate);
SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR(+, add, std::plus)
SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR(-, sub, std::minus)
SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR(*, mul, std::multiplies)
SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR(/, div, std::divides)
SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR(%, mod, simple::support::mod)
SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR(&, bit_and, std::bit_and)
SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR(|, bit_or, std::bit_or)
SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR(^, bit_xor, std::bit_xor)
SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR(<<, lshift, simple::support::lshift)
SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR(>>, rshift, simple::support::rshift)
SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR(+=, add_eq, simple::support::add_in_place);
SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR(-=, sub_eq, simple::support::sub_in_place);
SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR(*=, mul_eq, simple::support::mul_in_place);
SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR(/=, div_eq, simple::support::div_in_place);
SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR(%=, mod_eq, simple::support::mod_in_place);
SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR(&=, bit_and_eq, simple::support::bit_and_in_place);
SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR(|=, bit_or_eq, simple::support::bit_or_in_place);
SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR(^=, bit_xor_eq, simple::support::bit_xor_in_place);
SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR(<<=, lshift_eq, simple::support::lshift_in_place);
SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR(>>=, rshift_eq, simple::support::rshift_in_place);

#undef SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR
#undef SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR

#endif /* end of include guard */
