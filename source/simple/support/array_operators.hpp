#ifndef SIMPLE_SUPPORT_OPERATOR_MIXINS_HPP
#define SIMPLE_SUPPORT_OPERATOR_MIXINS_HPP

#include <type_traits>
#include <functional>
#include <limits>
#include <cstdint>
#include <cmath>

#include "enum_flags_operators.hpp"
#include "type_traits.hpp"

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
		bitwise = 		bit_and | bit_and_eq | bit_or | bit_or_eq | bit_xor | bit_xor_eq | bit_not,
		in_place =		all ^ (binary | unary)
	};

	template<> struct define_enum_flags_operators<array_operator> : public std::true_type {};

	struct add_in_place
	{
		template <typename T1, typename T2 = T1,
			typename Result = decltype(std::declval<T1&>() += std::declval<const T2&>())>
		constexpr T1& operator()(T1& one, const T2& other) const { return one += other; }
	};

	struct sub_in_place
	{
		template <typename T1, typename T2 = T1,
			typename Result = decltype(std::declval<T1&>() -= std::declval<const T2&>()) >
		constexpr T1& operator()(T1& one, const T2& other) const { return one -= other; }
	};

	struct mul_in_place
	{
		template <typename T1, typename T2 = T1,
			typename Result = decltype(std::declval<T1&>() *= std::declval<const T2&>())>
		constexpr T1& operator()(T1& one, const T2& other) const { return one *= other; }
	};

	struct div_in_place
	{
		template <typename T1, typename T2 = T1,
			typename Result = decltype(std::declval<T1&>() /= std::declval<const T2&>())>
		constexpr T1& operator()(T1& one, const T2& other) const { return one /= other; }
	};

	struct mod_in_place
	{
		template <typename T1, typename T2,
			std::enable_if_t<
				std::is_floating_point_v<T1> && std::is_floating_point_v<T2>
			> * = nullptr,
			typename Result = decltype(std::declval<T1&>() =
				std::fmod(std::declval<T1&>(), std::declval<const T2&>())
			)
		>
		constexpr T1& operator()(T1& one, const T2& other) const
		{
			return one = std::fmod(one, other);
		}

		template <typename T1, typename T2,
			std::enable_if_t<
				not(std::is_floating_point_v<T1> && std::is_floating_point_v<T2>)
			> * = nullptr,
			typename Result = decltype(std::declval<T1&>() %= std::declval<const T2&>())
		>
		constexpr T1& operator()(T1& one, const T2& other) const
		{
			return one %= other;
		}

	};

	// TODO: got an opportunity to counter some insane fundamental integral promotion rules here
	// bool bit_op bool = int // bool is guaranteed to be 0 or 1 so logical bit op results can fit in bool, not to mention bit twiddling an int is implementation defined
	// (unsigned short) * (unsigned short) = int // overflows for large values, which is undefined behavior for int, should promote to unsigned instead, can't guarantee no overflow, but at least not freakin UB
	// there is no reason for unary plus to promote, other than if it's used as some sort of explicit promotion operator, but I don't really like that, since proper promotion rules should be based on operators and not just types. in my eyes it unary plus should be just a shorthand for copy construction
	// // that said it might be better to leave these kind of descisions to the element type instead, but the defaults are just so brokeeeeen T_T the temptatioooon ToT


	struct add
	{
		template <typename T1, typename T2 = T1, typename Result =
			decltype(std::declval<const T1&>() + std::declval<const T2&>())>
		constexpr Result operator()(const T1& one, const T2& other) const { return one + other; }
	};

	struct sub
	{
		template <typename T1, typename T2 = T1, typename Result =
			decltype(std::declval<const T1&>() - std::declval<const T2&>())>
		constexpr Result operator()(const T1& one, const T2& other) const { return one - other; }
	};

	struct mul
	{
		template <typename T1, typename T2 = T1, typename Result =
			decltype(std::declval<const T1&>() * std::declval<const T2&>())>
		constexpr Result operator()(const T1& one, const T2& other) const { return one * other; }
	};

	struct div
	{
		template <typename T1, typename T2 = T1, typename Result =
			decltype(std::declval<const T1&>() / std::declval<const T2&>())>
		constexpr Result operator()(const T1& one, const T2& other) const { return one / other; }
	};

	struct mod
	{
		template <typename T1, typename T2,
			std::enable_if_t<
				std::is_floating_point_v<T1> && std::is_floating_point_v<T2>
			> * = nullptr,
			typename Result = decltype(
				std::fmod(std::declval<const T1&>(), std::declval<const T2&>())
			)
		>
		constexpr Result operator()(const T1& one, const T2& other) const
		{
			return std::fmod(one, other);
		}
		template <typename T1, typename T2,
			std::enable_if_t<
				not(std::is_floating_point_v<T1> && std::is_floating_point_v<T2>)
			> * = nullptr,
			typename Result = decltype(
				std::declval<const T1&>() % std::declval<const T2&>()
			)
		>
		constexpr Result operator()(const T1& one, const T2& other) const
		{
			return one % other;
		}
	};

	struct bit_and_in_place
	{
		template <typename T1, typename T2 = T1,
			typename Result = decltype(std::declval<T1&>() &= std::declval<const T2&>())>
		constexpr T1& operator()(T1& one, const T2& other) const { return one &= other; }
	};

	struct bit_or_in_place
	{
		template <typename T1, typename T2 = T1,
			typename Result = decltype(std::declval<T1&>() |= std::declval<const T2&>())>
		constexpr T1& operator()(T1& one, const T2& other) const { return one |= other; }
	};

	struct bit_xor_in_place
	{
		template <typename T1, typename T2 = T1,
			typename Result = decltype(std::declval<T1&>() ^= std::declval<const T2&>())>
		constexpr T1& operator()(T1& one, const T2& other) const { return one ^= other; }
	};

	struct bit_and
	{
		template <typename T1, typename T2 = T1, typename Result =
			decltype(std::declval<const T1&>() & std::declval<const T2&>())>
		constexpr Result operator()(const T1& one, const T2& other) const { return one & other; }
	};

	struct bit_or
	{
		template <typename T1, typename T2 = T1, typename Result =
			decltype(std::declval<const T1&>() | std::declval<const T2&>())>
		constexpr Result operator()(const T1& one, const T2& other) const { return one | other; }
	};

	struct bit_xor
	{
		template <typename T1, typename T2 = T1, typename Result =
			decltype(std::declval<const T1&>() ^ std::declval<const T2&>())>
		constexpr Result operator()(const T1& one, const T2& other) const { return one ^ other; }
	};

	struct lshift
	{
		template <typename T1, typename T2 = T1, typename Result =
			decltype(std::declval<const T1&>() << std::declval<const T2&>())>
		constexpr Result operator()(const T1& one, const T2& other) const { return one << other; }
	};

	struct rshift
	{
		template <typename T1, typename T2 = T1, typename Result =
			decltype(std::declval<const T1&>() >> std::declval<const T2&>())>
		constexpr Result operator()(const T1& one, const T2& other) const { return one >> other; }
	};

	struct lshift_in_place
	{
		template <typename T1, typename T2 = T1,
			typename Result = decltype(std::declval<T1&>() <<= std::declval<const T2&>())>
		constexpr T1& operator()(T1& one, const T2& other) const { return one <<= other; }
	};

	struct rshift_in_place
	{
		template <typename T1, typename T2 = T1,
			typename Result = decltype(std::declval<T1&>() >>= std::declval<const T2&>())>
		constexpr T1& operator()(T1& one, const T2& other) const { return one >>= other; }
	};

	struct bit_not
	{
		template <typename Type, typename Result = decltype(~std::declval<const Type&>())>
		constexpr Result operator()(const Type& one) const { return ~one; }
	};

	struct negate
	{
		template <typename Type, typename Result = decltype(-std::declval<const Type&>())>
		constexpr Result operator()(const Type& one) const { return -one; }
	};

	// public trait to specialize
	template <typename Type>
	struct define_array_operators
	{
		// specify the size of the array for the purposes of these operators
		constexpr static size_t size = 0;

		// a way to get to an actual array-like type in case Type does not expose the interface
		constexpr static auto& get_array(Type& object);

		// operators to enable, only considering operands that define this trait
		constexpr static array_operator enabled_operators = array_operator::none;
		// binary and in_place operators to enable, considering element types on the right side
		constexpr static array_operator enabled_right_element_operators = array_operator::none;
		// binary and in_place operators to enable, considering element types on the left side
		constexpr static array_operator enabled_left_element_operators = array_operator::none;

		// this template is used to change the element type of the array
		// in context of deducing the result of binary operations,
		// if your array-like type is templated on the element type you can wire it through here
		// to support type promotion, expression templates or other special techniques (like boost safe_numerics)
		// Result - the deduced result element type
		// array_operator - the operator in question
		// Other - the element type of the other operand (for unary ops it's same as Type)
		template <typename Result, array_operator, typename Other = Type>
		using result = Type;

		// if this type is the same between two types they will be considered as operands for binary and in-place operators,
		// as long as corresponding element types support the operator,
		// this is the another thing you need in order to support type promotion, expression templates etc.
		// you can set this to the shape of the array (which would be size if there is no nesting),
		// along with a tag that identifies the template, if you want that extra safety
		// TODO: this is about operands so include operand in the name, perhaps simply operand_tag
		using compatibility_tag = Type;
	};

	// public trait to use
	// some common defaults for defining array operators on a type
	template <typename Type, size_t Size>
	struct trivial_array_accessor
	{
		constexpr static size_t size = Size; // explicitly specified size

		// the type provides array-like interface itself, no digging or wrapping necessary
		template <typename T>
		constexpr static T& get_array(T& object) noexcept { return object; }
		template <typename T>
		constexpr static const T& get_array(const T& object) noexcept { return object; }

		template <typename Result, array_operator, typename Other = Type>
		using result = Type; // the result of binary operators is the same exact type, in all cases
		// note that, while it can help somewhat, this does not (and can not) technically prevent type promotion and associated problems,
		// to get around that properly you would need a more sophisticated solution
		// see https://www.boost.org/doc/libs/1_75_0/libs/safe_numerics/doc/html/promotion_policy.html

		using compatibility_tag = Type; // operands of binary and in-place operators must be the same exact type
	};

	// public trait to specialize
	// allows you to specify an implicit conversion to be considered by binary and in-place operators,
	// since templates do not considered conversions otherwise
	template <typename From>
	struct array_operator_implicit_conversion { using type = support::remove_cvref_t<From>; };
	template <typename From>
	using array_operator_implicit_conversion_t =
		typename array_operator_implicit_conversion<support::remove_cvref_t<From>>::type;

namespace AOps_Details
{

	template <typename Operator, size_t Size, typename Type, typename Result>
	constexpr Result& array_unary_operator(Result& result, const Type& one)
	{
		auto op = Operator{};
		for(size_t i = 0; i < Size; ++i)
			result[i] = op(one[i]);
		return result;
	}

	template <typename Operator, size_t Size, typename Type, typename Other>
	constexpr Type& array_in_place_operator(Type& one, const Other& other)
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

	template <typename Operator, size_t Size, typename One, typename Other, typename Result>
	constexpr Result& array_binary_operator(Result& result, const One& one, const Other& other)
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
// FIXME: either get rid of macros or make better use of them

#define SIMPLE_SUPPORT_DEFINE_UNARY_OPERATOR(op_symbol, op_type, op_fun) \
template <typename Array, \
	typename OperatorDef = simple::support::define_array_operators<Array>, \
	std::enable_if_t<OperatorDef::enabled_operators && simple::support::array_operator::op_type>* = nullptr> \
[[nodiscard]] constexpr auto operator op_symbol (const Array& one) \
{ \
	using namespace simple::support; \
	using namespace simple::support::AOps_Details; \
	using element_t = simple::support::AOps_Details::array_element_t<OperatorDef, Array>; \
	using result_element_t = std::invoke_result_t<op_fun,element_t>; \
	using result_t = typename OperatorDef::template result<result_element_t, simple::support::array_operator::op_type, element_t>; \
	result_t result{}; \
	array_unary_operator \
		<op_fun, OperatorDef::size> \
		(OperatorDef::get_array(result), OperatorDef::get_array(one)); \
	return result; \
}

SIMPLE_SUPPORT_DEFINE_UNARY_OPERATOR(~, bit_not, simple::support::bit_not)
SIMPLE_SUPPORT_DEFINE_UNARY_OPERATOR(-, negate, simple::support::negate)

#undef SIMPLE_SUPPORT_DEFINE_UNARY_OPERATOR

#define SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR(op_symbol, op_type, op_fun) \
template <typename Array, typename Other,  void* = nullptr, \
	typename OperatorDef = simple::support::define_array_operators<Array>, \
	typename OtherOperatorDef = simple::support::define_array_operators<Other>, \
	typename Element = simple::support::AOps_Details::array_element_t<OperatorDef, Array>, \
	typename OtherElement = simple::support::AOps_Details::array_element_t<OtherOperatorDef, Other>, \
	std::enable_if_t<OperatorDef::enabled_operators && simple::support::array_operator::op_type>* = nullptr, \
	std::enable_if_t<OtherOperatorDef::enabled_operators && simple::support::array_operator::op_type>* = nullptr, \
	std::enable_if_t<std::is_same_v<typename OtherOperatorDef::compatibility_tag, typename OperatorDef::compatibility_tag>>* = nullptr, \
	std::enable_if_t<std::is_invocable_v<op_fun,Element,OtherElement>>* = nullptr> \
[[nodiscard]] constexpr auto operator op_symbol (const Array& one, const Other& other) \
{ \
	using namespace simple::support; \
	using namespace simple::support::AOps_Details; \
	using result_t = typename OperatorDef::template result<std::invoke_result_t<op_fun,Element,OtherElement>, simple::support::array_operator::op_type, OtherElement>; \
	result_t result{}; \
	using result_op_def = simple::support::define_array_operators<result_t>; \
	static_assert(result_op_def::size == OperatorDef::size); \
	array_binary_operator \
		<op_fun, OperatorDef::size> \
		(result_op_def::get_array(result), OperatorDef::get_array(one), OtherOperatorDef::get_array(other)); \
	return result; \
} \
\
template <typename T1, typename T2, \
	typename C1 = simple::support::array_operator_implicit_conversion_t<T1>, \
	typename C2 = simple::support::array_operator_implicit_conversion_t<T2>, \
	std::enable_if_t< \
		(!std::is_same_v<T1,C1> || !std::is_same_v<T2,C2>) \
	>* = nullptr \
> \
[[nodiscard]] constexpr auto operator op_symbol (T1&& one, T2&& other) \
	-> decltype(operator op_symbol<C1,C2,nullptr>(std::forward<T1>(one), std::forward<T2>(other))) \
{ \
	return operator op_symbol<C1,C2,nullptr>(std::forward<T1>(one), std::forward<T2>(other)); \
} \
\
template <typename Array, typename Other, \
	typename OperatorDef = simple::support::define_array_operators<Array>, \
	typename Element = simple::support::AOps_Details::array_element_t<OperatorDef, Array>, \
	std::enable_if_t<(OperatorDef::enabled_right_element_operators && simple::support::array_operator::op_type) \
		&& !std::is_same_v<Array, Other> \
		&& std::is_invocable_r_v<Element, op_fun, Element, Other>\
	>* = nullptr \
> \
[[nodiscard]] constexpr Array operator op_symbol \
( \
	const Array & one, \
	const Other & other \
 ) \
{ \
	using namespace simple::support; \
	using namespace simple::support::AOps_Details; \
	Array result{}; \
	array_right_element_binary_operator \
		<op_fun, OperatorDef::size> \
		(OperatorDef::get_array(result), OperatorDef::get_array(one), other); \
	return result; \
} \
\
template <typename Array, typename Other, \
	typename OperatorDef = simple::support::define_array_operators<Array>, \
	typename Element = simple::support::AOps_Details::array_element_t<OperatorDef, Array>, \
	std::enable_if_t<(OperatorDef::enabled_left_element_operators && simple::support::array_operator::op_type) \
		&& !std::is_same_v<Array, Other> \
		&& std::is_invocable_r_v<Element, op_fun, Other, Element>\
	>* = nullptr \
> \
[[nodiscard]] constexpr Array operator op_symbol \
( \
	const Other & one, \
	const Array & other \
) \
{ \
	using namespace simple::support; \
	using namespace simple::support::AOps_Details; \
	Array result{}; \
	array_left_element_binary_operator \
		<op_fun, OperatorDef::size> \
		(OperatorDef::get_array(result), OperatorDef::get_array(other), one); \
	return result; \
}

SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR(+, add, simple::support::add)
SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR(-, sub, simple::support::sub)
SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR(*, mul, simple::support::mul)
SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR(/, div, simple::support::div)
SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR(%, mod, simple::support::mod)
SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR(&, bit_and, simple::support::bit_and)
SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR(|, bit_or, simple::support::bit_or)
SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR(^, bit_xor, simple::support::bit_xor)
SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR(<<, lshift, simple::support::lshift)
SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR(>>, rshift, simple::support::rshift)

#undef SIMPLE_SUPPORT_DEFINE_BINARY_OPERATOR

#define SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR(op_symbol, op_type, op_fun) \
template <typename Array, typename Other, void* = nullptr, \
	typename OperatorDef = simple::support::define_array_operators<Array>, \
	typename OtherOperatorDef = simple::support::define_array_operators<Other>, \
	typename Element = simple::support::AOps_Details::array_element_t<OperatorDef, Array>, \
	typename OtherElement = simple::support::AOps_Details::array_element_t<OtherOperatorDef, Other>, \
	std::enable_if_t<OperatorDef::enabled_operators && simple::support::array_operator::op_type>* = nullptr, \
	std::enable_if_t<OtherOperatorDef::enabled_operators && simple::support::array_operator::op_type>* = nullptr, \
	std::enable_if_t<std::is_same_v<typename OtherOperatorDef::compatibility_tag, typename OperatorDef::compatibility_tag>>* = nullptr, \
	std::enable_if_t<std::is_invocable_v<op_fun,Element&,const OtherElement&>>* = nullptr> \
constexpr Array& operator op_symbol \
( \
	Array& one, \
	const Other& other \
) \
{ \
	using namespace simple::support; \
	using namespace simple::support::AOps_Details; \
	array_in_place_operator \
		<op_fun, OperatorDef::size> \
		(OperatorDef::get_array(one), OtherOperatorDef::get_array(other)); \
	return one; \
} \
\
template <typename Array, typename Other, \
	typename OtherConv = simple::support::array_operator_implicit_conversion_t<Other>, \
	std::enable_if_t< \
		(!std::is_same_v<OtherConv,Other>) \
	>* = nullptr \
> \
constexpr auto operator op_symbol (Array& one, const Other& other) \
	-> decltype(operator op_symbol<Array,OtherConv,nullptr>(one, other)) \
{ \
	return operator op_symbol<Array,OtherConv,nullptr>(one, other); \
} \
\
template <typename Array, typename Other, \
	typename OperatorDef = simple::support::define_array_operators<Array>, \
	typename Element = simple::support::AOps_Details::array_element_t<OperatorDef, Array>, \
	std::enable_if_t<(OperatorDef::enabled_right_element_operators && simple::support::array_operator::op_type) \
		&& !std::is_same_v<Array, Other> \
		&& std::is_invocable_r_v<Element&, op_fun, Element&, Other> \
	>* = nullptr \
> \
constexpr Array& operator op_symbol \
( \
	Array & one, \
	const Other & other \
) \
{ \
	using namespace simple::support; \
	using namespace simple::support::AOps_Details; \
	array_right_element_in_place_operator \
		<op_fun, OperatorDef::size> \
		(OperatorDef::get_array(one), other); \
	return one; \
} \
template <typename Array, typename Other, \
	typename OperatorDef = simple::support::define_array_operators<Array>, \
	typename Element = simple::support::AOps_Details::array_element_t<OperatorDef, Array>, \
	std::enable_if_t<(OperatorDef::enabled_left_element_operators && simple::support::array_operator::op_type) \
		&& !std::is_same_v<Array, Other> \
		&& std::is_invocable_r_v<Element&, op_fun, Other&, Element> \
	>* = nullptr \
> \
constexpr Other& operator op_symbol \
( \
	Other & one, \
	const Array & other \
) \
{ \
	using namespace simple::support; \
	using namespace simple::support::AOps_Details; \
	array_left_element_in_place_operator \
		<op_fun, OperatorDef::size> \
		(one, OperatorDef::get_array(other)); \
	return one; \
}

SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR(+=, add_eq, simple::support::add_in_place)
SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR(-=, sub_eq, simple::support::sub_in_place)
SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR(*=, mul_eq, simple::support::mul_in_place)
SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR(/=, div_eq, simple::support::div_in_place)
SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR(%=, mod_eq, simple::support::mod_in_place)
SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR(&=, bit_and_eq, simple::support::bit_and_in_place)
SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR(|=, bit_or_eq, simple::support::bit_or_in_place)
SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR(^=, bit_xor_eq, simple::support::bit_xor_in_place)
SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR(<<=, lshift_eq, simple::support::lshift_in_place)
SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR(>>=, rshift_eq, simple::support::rshift_in_place)

#undef SIMPLE_SUPPORT_DEFINE_IN_PLACE_OPERATOR

#endif /* end of include guard */
