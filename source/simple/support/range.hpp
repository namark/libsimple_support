#ifndef SIMPLE_SUPPORT_RANGE_HPP
#define SIMPLE_SUPPORT_RANGE_HPP

#include <algorithm>
#include <type_traits>
#include <istream>
#include <ostream>

#include "array.hpp"
#include "array_operators.hpp"

namespace simple::support
{

	template<typename Type, typename = std::nullptr_t>
	struct range_based_for_loopable : public std::false_type {};

	template<typename Type>
	struct range_based_for_loopable<Type,
	decltype(
		void(std::declval<Type>() != std::declval<Type>()),
		void(*std::declval<Type>()),
		void(++(std::declval<Type&>())),
		nullptr)>
	: public std::true_type
	{};

	template <typename Type>
	struct range
	{

		array<Type, 2> bounds;

		constexpr static range limit()
		{
			return { std::numeric_limits<Type>::min(), std::numeric_limits<Type>::max() };
		}

		constexpr Type& lower() noexcept { return bounds[0]; }
		constexpr Type& upper() noexcept { return bounds[1]; }
		constexpr const Type& lower() const noexcept { return bounds[0]; }
		constexpr const Type& upper() const noexcept { return bounds[1]; }

		constexpr bool operator ==(const range& other) const
		{
			return bounds == other.bounds;
		}

		template <typename T = Type, std::enable_if_t<range_based_for_loopable<T>{}>...>
		constexpr Type& begin() noexcept { return lower(); }
		template <typename T = Type, std::enable_if_t<range_based_for_loopable<T>{}>...>
		constexpr Type begin() const noexcept { return lower(); }
		template <typename T = Type, std::enable_if_t<range_based_for_loopable<T>{}>...>
		constexpr Type& end() noexcept { return upper(); }
		template <typename T = Type, std::enable_if_t<range_based_for_loopable<T>{}>...>
		constexpr Type end() const noexcept { return upper(); }

		template<typename Container>
		constexpr auto to_iterator_range(Container& container) const
		-> range<decltype(container.begin())>
		{
			range<decltype(container.begin())> result
			{
				container.begin() + lower(),
				container.begin() + upper()
			};
			clamp_in_place(result, {container.begin(), container.end()});
			return result;
		}

		constexpr bool valid() const { return lower() <= upper(); }
		constexpr void fix()
		{
			using std::min;
			using std::max;
			auto temp = bounds;
			lower() = min(temp[0], temp[1]);
			upper() = max(temp[0], temp[1]);
		}

		template <typename ValueType = Type, std::common_type_t<ValueType, Type>* = nullptr>
		constexpr bool contains(const ValueType& value) const
		{ return lower() < value && value < upper(); }

		template <typename ValueType = Type, std::common_type_t<ValueType, Type>* = nullptr>
		constexpr bool intersects(const ValueType& value) const
		{ return lower() <= value && value <= upper(); };

		template <typename ValueType = Type, std::common_type_t<ValueType, Type>* = nullptr>
		constexpr bool intersects_lower(const ValueType& value) const
		{ return lower() <= value && value < upper(); }

		template <typename ValueType = Type, std::common_type_t<ValueType, Type>* = nullptr>
		constexpr bool intersects_upper(const ValueType& value) const
		{ return lower() < value && value <= upper(); }

		constexpr bool contains(const range& other) const
		{ return lower() < other.lower() && other.upper() < upper(); }

		constexpr bool covers(const range& other) const
		{ return lower() <= other.lower() && other.upper() <= upper(); }

		constexpr bool intersects(const range& other) const
		{ return lower() <= other.upper() && upper() >= other.lower(); }
		// { return intersection(other).valid(); } // why is the cool way always inefficient :/

		constexpr range intersection(range other) const
		{
			using std::min;
			using std::max;
			other.lower() = max(other.lower(), lower());
			other.upper() = min(other.upper(), upper());
			return other;
		}

	};

	template <typename Type, typename ValueType = Type, std::common_type_t<ValueType, Type>* = nullptr>
	constexpr bool contains(const range<Type>& range, const ValueType& value)
	{ return range.contains(value); }

	template <typename Type, typename ValueType, std::common_type_t<ValueType, Type>* = nullptr>
	constexpr bool intersects(const range<Type>& range, const ValueType& value)
	{ return range.intersects(value); }

	template <typename Type, typename ValueType, std::common_type_t<ValueType, Type>* = nullptr>
	constexpr bool intersects_lower(const range<Type>& range, const ValueType& value)
	{ return range.intersects_lower(value); }

	template <typename Type, typename ValueType, std::common_type_t<ValueType, Type>* = nullptr>
	constexpr bool intersects_upper(const range<Type>& range, const ValueType& value)
	{ return range.intersects_upper(value); }

	template <typename Type>
	constexpr bool contains(const range<Type>& one, const range<Type>& other)
	{ return one.contains(other); }

	template <typename Type>
	constexpr bool covers(const range<Type>& one, const range<Type>& other)
	{ return one.covers(other); }

	template <typename Type>
	constexpr bool intersects(const range<Type>& one, const range<Type>& other)
	{ return one.intersects(other); }

	template <typename Type>
	constexpr range<Type> intersection(const range<Type>& one, range<Type> other)
	{ return one.intersection(other); }

	template <typename Type>
	constexpr
	range<Type>& clamp_in_place(range<Type>& v, const range<Type>& hilo)
	{
		using std::clamp;
		for(size_t i = 0; i < v.bounds.size(); ++i)
			clamp_in_place(v.bounds[i], hilo);
		return v;
	}

	template <typename Type, typename RangeValueType = Type>
	constexpr
	Type& clamp_in_place(Type& v, const range<RangeValueType>& hilo)
	{
		using std::clamp;
		v = clamp(v, hilo.lower(), hilo.upper());
		return v;
	}

	template <typename Type, typename RangeValueType = Type>
	constexpr
	Type clamp(Type v, const range<RangeValueType>& hilo)
	{
		clamp_in_place(v, hilo);
		return v;
	}

	template <typename Type>
	constexpr
	range<Type> clamp(range<Type> v, const range<Type>& hilo)
	{
		clamp_in_place(v, hilo);
		return v;
	}

	template<typename Index = std::size_t, typename Container>
	constexpr
	auto get_iterator_range(Container& container, const range<Index>& index_range)
	-> range<decltype(container.begin())>
	{
		return index_range.to_iterator_range(container);
	}

} // namespace simple::support

namespace simple
{
	template<typename T>
	struct support::define_array_operators<support::range<T>>
	{
		constexpr static auto enabled_operators = array_operator::none;
		constexpr static auto enabled_right_element_operators =
			array_operator::add |
			array_operator::sub |
			array_operator::mul |
			array_operator::div |
			array_operator::add_eq |
			array_operator::sub_eq |
			array_operator::mul_eq |
			array_operator::div_eq;
		constexpr static auto enabled_left_element_operators = array_operator::none;

		constexpr static size_t size = 2;
		constexpr static array<T,size>& get_array(support::range<T>& r) noexcept
		{ return r.bounds; }
		constexpr static const array<T,size>& get_array(const support::range<T>& r) noexcept
		{ return r.bounds; }
	};
} // namespace simple

#endif /* end of include guard */
