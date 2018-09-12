// Copyright (C) 2007-2017 Free Software Foundation, Inc.
//
// This library is free software;
// you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.
// <http://www.gnu.org/licenses/>.

/**
 *  This is a modified version of Standard C++ Library header.
 *  More functions and methods made constexpr.
 *  c++17 specific features enabled unconditionally.
 *  operator!= will use same the operator on value_type if available.
 *  Anything that would be considered UB outside of std removed,
 *  including specializations of tuple interface; similar interface
 *  defined for use with ADL.
 */

#ifndef SIMPLE_SUPPORT_ARRAY_HPP
#define SIMPLE_SUPPORT_ARRAY_HPP

#include <utility>
#include <stdexcept>

namespace simple
{ namespace support
{

	template<typename Type, typename = std::nullptr_t>
	struct has_inequality_operator
	{ static constexpr bool value = false; };
	template<typename Type>
	struct has_inequality_operator<Type,
	decltype(void(std::declval<Type>() != std::declval<Type>()), nullptr)>
	{ static constexpr bool value = true; };

	template<typename Tp, std::size_t Nm>
	struct array_traits
	{
		typedef Tp Type[Nm];
		typedef std::is_swappable<Tp> Is_swappable;
		typedef std::is_nothrow_swappable<Tp> Is_nothrow_swappable;

		static constexpr Tp&
		S_ref(const Type& t, std::size_t n) noexcept
		{ return const_cast<Tp&>(t[n]); }

		static constexpr Tp*
		S_ptr(const Type& t) noexcept
		{ return const_cast<Tp*>(t); }
	};

	template<typename Tp>
	struct array_traits<Tp, 0>
	{
		struct Type { };
		typedef std::true_type Is_swappable;
		typedef std::true_type Is_nothrow_swappable;

		static constexpr Tp&
		S_ref(const Type&, std::size_t) noexcept
		{ return *static_cast<Tp*>(nullptr); }

		static constexpr Tp*
		S_ptr(const Type&) noexcept
		{ return nullptr; }
	};

	/**
	 *  @brief A standard container for storing a fixed size sequence of elements.
	 *
	 *  @ingroup sequences
	 *
	 *  Meets the requirements of a <a href="tables.html#65">container</a>, a
	 *  <a href="tables.html#66">reversible container</a>, and a
	 *  <a href="tables.html#67">sequence</a>.
	 *
	 *  Sets support random access iterators.
	 *
	 *  @tparam  Tp  Type of element. Required to be a complete type.
	 *  @tparam  N  Number of elements.
	 */
	template<typename Tp, std::size_t Nm>
	struct array
	{
		typedef Tp										value_type;
		typedef value_type*								pointer;
		typedef const value_type*						const_pointer;
		typedef value_type&								reference;
		typedef const value_type&						const_reference;
		typedef value_type*								iterator;
		typedef const value_type*						const_iterator;
		typedef std::size_t								size_type;
		typedef std::ptrdiff_t							difference_type;
		typedef std::reverse_iterator<iterator>			reverse_iterator;
		typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;

		// Support for zero-sized arrays mandatory.
		typedef array_traits<Tp, Nm> AT_Type;
		typename AT_Type::Type       M_elems;

		// No explicit construct/copy/destroy for aggregate type.

		// DR 776.
		void
		fill(const value_type& u)
		{ std::fill_n(begin(), size(), u); }

		void
		swap(array& other)
		noexcept(AT_Type::Is_nothrow_swappable::value)
		{ std::swap_ranges(begin(), end(), other.begin()); }

		// Iterators.
		constexpr iterator
		begin() noexcept
		{ return iterator(data()); }

		constexpr const_iterator
		begin() const noexcept
		{ return const_iterator(data()); }

		constexpr iterator
		end() noexcept
		{ return iterator(data() + Nm); }

		constexpr const_iterator
		end() const noexcept
		{ return const_iterator(data() + Nm); }

		constexpr reverse_iterator
		rbegin() noexcept
		{ return reverse_iterator(end()); }

		constexpr const_reverse_iterator
		rbegin() const noexcept
		{ return const_reverse_iterator(end()); }

		constexpr reverse_iterator
		rend() noexcept
		{ return reverse_iterator(begin()); }

		constexpr const_reverse_iterator
		rend() const noexcept
		{ return const_reverse_iterator(begin()); }

		constexpr const_iterator
		cbegin() const noexcept
		{ return const_iterator(data()); }

		constexpr const_iterator
		cend() const noexcept
		{ return const_iterator(data() + Nm); }

		constexpr const_reverse_iterator
		crbegin() const noexcept
		{ return const_reverse_iterator(end()); }

		constexpr const_reverse_iterator
		crend() const noexcept
		{ return const_reverse_iterator(begin()); }

		// Capacity.
		constexpr size_type
		size() const noexcept { return Nm; }

		constexpr size_type
		max_size() const noexcept { return Nm; }

		constexpr bool
		empty() const noexcept { return size() == 0; }

		// Element access.
		constexpr reference
		operator[](size_type n) noexcept
		{ return AT_Type::S_ref(M_elems, n); }

		constexpr const_reference
		operator[](size_type n) const noexcept
		{ return AT_Type::S_ref(M_elems, n); }

		constexpr reference
		at(size_type n)
		{
			if (n >= Nm)
				throw std::out_of_range("simple::array::at");
			return AT_Type::S_ref(M_elems, n);
		}

		constexpr const_reference
		at(size_type n) const
		{
			// Result of conditional expression must be an lvalue so use
			// boolean ? lvalue : (throw-expr, lvalue)
			return n < Nm ? AT_Type::S_ref(M_elems, n)
			: (throw std::out_of_range("simple::array::at"),
					AT_Type::S_ref(M_elems, 0));
		}

		constexpr reference
		front() noexcept
		{ return *begin(); }

		constexpr const_reference
		front() const noexcept
		{ return AT_Type::S_ref(M_elems, 0); }

		constexpr reference
		back() noexcept
		{ return Nm ? *(end() - 1) : *end(); }

		constexpr const_reference
		back() const noexcept
		{
			return Nm ? AT_Type::S_ref(M_elems, Nm - 1)
			: AT_Type::S_ref(M_elems, 0);
		}

		constexpr pointer
		data() noexcept
		{ return AT_Type::S_ptr(M_elems); }

		constexpr const_pointer
		data() const noexcept
		{ return AT_Type::S_ptr(M_elems); }
	};

	template<typename Tp, typename... Up>
	array(Tp, Up...)
	-> array<std::enable_if_t<(std::is_same_v<Tp, Up> && ...), Tp>,
	1 + sizeof...(Up)>;

	// Array comparisons.

	template<typename Tp, std::size_t Nm>
	constexpr inline bool
	operator==(const array<Tp, Nm>& one, const array<Tp, Nm>& two)
	{
		for(std::size_t i = 0; i < Nm; ++i)
			if(!(one[i] == two[i]))
				return false;
		return true;
	}

	template<typename Tp, std::size_t Nm,
	std::enable_if_t<has_inequality_operator<Tp>::value>* = nullptr>
	constexpr inline bool
	operator!=(const array<Tp, Nm>& one, const array<Tp, Nm>& two)
	{
		for(std::size_t i = 0; i < Nm; ++i)
			if(one[i] != two[i])
				return true;
		return false;
	}

	template<typename Tp, std::size_t Nm,
	std::enable_if_t<!has_inequality_operator<Tp>::value>* = nullptr>
	constexpr inline bool
	operator!=(const array<Tp, Nm>& one, const array<Tp, Nm>& two)
	{ return !(one == two); }

	template<typename Tp, std::size_t Nm>
	inline bool
	operator<(const array<Tp, Nm>& a, const array<Tp, Nm>& b)
	{
		return std::lexicographical_compare(a.begin(), a.end(),
				b.begin(), b.end());
	}

	template<typename Tp, std::size_t Nm>
	inline bool
	operator>(const array<Tp, Nm>& one, const array<Tp, Nm>& two)
	{ return two < one; }

	template<typename Tp, std::size_t Nm>
	inline bool
	operator<=(const array<Tp, Nm>& one, const array<Tp, Nm>& two)
	{ return !(one > two); }

	template<typename Tp, std::size_t Nm>
	inline bool
	operator>=(const array<Tp, Nm>& one, const array<Tp, Nm>& two)
	{ return !(one < two); }

	// Specialized algorithms.
	template<typename Tp, std::size_t Nm>
	inline
	// Constrained free swap overload, see p0185r1
	typename std::enable_if<
	simple::support::array_traits<Tp, Nm>::Is_swappable::value
	>::type
	swap(simple::support::array<Tp, Nm>& one, simple::support::array<Tp, Nm>& two)
	noexcept(noexcept(one.swap(two)))
	{ one.swap(two); }

	template<typename Tp, std::size_t Nm>
	typename std::enable_if<
	!simple::support::array_traits<Tp, Nm>::Is_swappable::value>::type
	swap(simple::support::array<Tp, Nm>&, simple::support::array<Tp, Nm>&) = delete;

	template<std::size_t Int, typename Tp, std::size_t Nm>
	constexpr Tp&
	get(simple::support::array<Tp, Nm>& arr) noexcept
	{
		static_assert(Int < Nm, "array index is within bounds");
		return simple::support::array_traits<Tp, Nm>::
		S_ref(arr.M_elems, Int);
	}

	template<std::size_t Int, typename Tp, std::size_t Nm>
	constexpr Tp&&
	get(simple::support::array<Tp, Nm>&& arr) noexcept
	{
		static_assert(Int < Nm, "array index is within bounds");
		return std::move(std::get<Int>(arr));
	}

	template<std::size_t Int, typename Tp, std::size_t Nm>
	constexpr const Tp&
	get(const simple::support::array<Tp, Nm>& arr) noexcept
	{
		static_assert(Int < Nm, "array index is within bounds");
		return simple::support::array_traits<Tp, Nm>::
		S_ref(arr.M_elems, Int);
	}

	// Tuple interface
	template<typename Tp>
	struct tuple_size;

	template<std::size_t I, typename Tp>
	struct tuple_element;

	template<typename Tp, std::size_t Nm>
	struct tuple_size<simple::support::array<Tp, Nm>>
	: public std::integral_constant<std::size_t, Nm> { };

	template<std::size_t I, typename Tp, std::size_t Nm>
	struct tuple_element<I, simple::support::array<Tp, Nm>>
	{
		public:
		static_assert(I < Nm, "index is out of bounds");
		typedef Tp type;
	};

}} // namespace simple::support

#endif /* end of include guard */
