#ifndef SIMPLE_SUPPORT_RATIONAL_HPP
#define SIMPLE_SUPPORT_RATIONAL_HPP

#include "type_traits.hpp"

namespace simple::support
{

	// TODO: find and use a proper rational type
	// no, not the math-scientific,always canonical,pure algebra rational.
	// A fully customizable, widely applicable, "don't pay for what not use" rational.
	// If there is a calculation trick I can do with ratios by hand, I should be able to it with rational here
	//
	// or I guess if we are doing this
	// TODO: implement the rest of the ops trivially, disregarding overflow
	// TODO: diagnose overflow
	// TODO: tackle the actual hard problems, there is a whole world here of reducing the integers with common factors, canonicalizing on every move is a total overkill
	// TODO: SFINAE Denominator to be either void meta_constant maybe, currently it's a bit loosey-goosey

	// want c++20 non type template paramssss ToT
	template <typename T, T Value>
	struct meta_constant
	{
		static constexpr T value = Value;

		constexpr operator T() { return value; };
	};

	template <typename T, T left, T right>
	constexpr auto operator* (meta_constant<T, left>, meta_constant<T, right>)
	{ return meta_constant<T, left * right>{}; }

	template <typename Num, typename Denom>
	struct rational_base
	{
		// protected:
		Num num;
		using den = Denom;
	};

	template <typename Num>
	struct rational_base<Num, void>
	{
		Num num;
		Num den;
	};


	template <typename Num, typename Denom = void>
	class rational : private rational_base<Num, Denom>
	{
		using base = rational_base<Num, Denom>;

		// common

		public:

		constexpr rational() = default;

		constexpr rational& operator*=(const Num& value)
		{
			numerator() *= value;
			return *this;
		}

		constexpr rational& operator+=(const Num& other)
		{
			numerator() += other * denominator();
			return *this;
		}
		constexpr rational& operator-=(const Num& other)
		{
			numerator() -= other * denominator();
			return *this;
		}

		constexpr rational& operator ++() { return operator+=(Num{} + 1); }
		constexpr rational& operator --() { return operator-=(Num{} + 1); }

		constexpr rational operator ++(int)
		{
			auto old = *this;
			operator++();
			return old;
		}
		constexpr rational operator --(int)
		{
			auto old = *this;
			operator--();
			return old;
		}

		constexpr explicit operator Num() const
		{ return numerator() / denominator(); }

		constexpr Num& numerator() &
		{ return this->num; }
		constexpr const Num& numerator() const &
		{ return this->num; }
		constexpr Num&& numerator() &&
		{ return this->num; }
		constexpr const Num&& numerator() const &&
		{ return this->num; }

		constexpr decltype(auto) denominator()
		{
			if constexpr (std::is_same_v<Denom,void>)
				return (this->den); // unknown denominator
			else
				return typename base::den{}; // known denominator
		}

		constexpr decltype(auto) denominator() const
		{
			if constexpr (std::is_same_v<Denom,void>)
				return (this->den); // unknown denominator
			else
				return typename base::den{}; // known denominator
		}

		// unknown denominator

		template <typename Numerator,
			typename D = Denom, std::enable_if_t<std::is_same_v<D,void>>* = nullptr >
		constexpr rational(Numerator numerator, Numerator denominator) : base{numerator, denominator}
		{}

		template <typename OtherNum, typename OtherDenom,
			typename D = Denom, std::enable_if_t<std::is_same_v<D, void>>* = nullptr>
		constexpr rational& operator*=(const rational<OtherNum, OtherDenom>& value)
		{
			numerator() *= value.numerator();
			denominator() *= value.denominator();
			return *this;
		}

		// known denominator

		template <typename Denominator,
			typename D = Denom, std::enable_if_t<not std::is_same_v<D,void>>* = nullptr >
		constexpr rational(Num numerator, Denominator) : base{numerator}
		{}


		template <typename D = Denom, std::enable_if_t<not std::is_same_v<D, void>>* = nullptr>
		constexpr rational& operator+=(const rational& other)
		{
			numerator() += other.numerator();
			return *this;
		}

	};

	// unknown denominator
	template <typename Num> rational(Num, Num) -> rational<Num, void>;

	// known denominator
	template <typename Num, typename Denom> rational(Num, Denom) -> rational<Num, Denom>;

	template <typename Num, typename Denom,
		std::enable_if_t<not std::is_same_v<Denom,void>>* = nullptr>
	[[nodiscard]] constexpr
	rational<Num, Denom> next(rational<Num,Denom> current)
	{
		current.numerator() += (Num{} + 1);
	}
	template <typename Num, typename Denom,
		std::enable_if_t<not std::is_same_v<Denom,void>>* = nullptr>
	[[nodiscard]] constexpr
	rational<Num, Denom> prev(rational<Num,Denom> current)
	{
		current.numerator() -= (Num{} + 1);
	}

	template <typename Num, typename Denom,
		std::enable_if_t<not std::is_same_v<Denom,void>>* = nullptr>
	[[nodiscard]] constexpr
	rational<Num, Denom> advance(rational<Num,Denom> current, Num offset)
	{
		current.numerator() += offset;
	}

	template <typename Num, typename Denom,
		std::enable_if_t<not std::is_same_v<Denom,void>>* = nullptr>
	[[nodiscard]] constexpr
	bool operator== (const rational<Num,Denom>& one, const rational<Num,Denom>& other)
	{ return one.numerator() == other.numerator(); }
	template <typename Num, typename Denom,
		std::enable_if_t<not std::is_same_v<Denom,void>>* = nullptr>
	[[nodiscard]] constexpr
	bool operator< (const rational<Num,Denom>& one, const rational<Num,Denom>& other)
	{ return one.numerator() < other.numerator(); }

	// common

	template <typename Num, typename Denom>
	constexpr auto operator*(const rational<Num,Denom>& multiplicand, const rational<Num,Denom>& multiplier)
	{
		return rational
		(
			multiplicand.numerator() * multiplier.numerator(),
			multiplicand.denominator() * multiplier.denominator()
		);
	}

	// TODO: there is some boost library that does this stuff...
	// is a boost library though...

	template <typename Num, typename Denom>
	constexpr rational<Num,Denom> operator*(rational<Num,Denom> multiplicand, const non_deduced<Num>& multiplier)
	{
		multiplicand *= multiplier;
		return multiplicand;
	}
	template <typename Num, typename Denom>
	constexpr rational<Num,Denom> operator+(rational<Num,Denom> addend, const non_deduced<Num>& adder)
	{
		addend += adder;
		return addend;
	}

	template <typename Num, typename Denom>
	constexpr rational<Num,Denom> operator*(const non_deduced<Num>& multiplier, rational<Num, Denom> multiplicand)
	{
		multiplicand *= multiplier;
		return multiplicand;
	}
	template <typename Num, typename Denom>
	constexpr rational<Num,Denom> operator+(const non_deduced<Num>& addend, rational<Num, Denom> adder)
	{
		addend += adder;
		return addend;
	}

	template <typename Num, typename Denom>
	constexpr auto operator+(rational<Num,Denom> addend, const rational<Num,Denom>& adder)
		-> decltype(addend += adder, rational<Num,Denom>{})
	{ return addend += adder; }

	template <typename Num, typename Denom>
	[[nodiscard]] constexpr
	auto operator!=(rational<Num, Denom> one, rational<Num,Denom> other)
		-> decltype(!(one == other))
	{ return !(one == other); }

	template <typename Num, typename Denom>
	[[nodiscard]] constexpr
	auto operator> (rational<Num, Denom> left, rational<Num,Denom> right)
		-> decltype(right < left)
	{ return right < left; }

	template <typename Num, typename Denom>
	[[nodiscard]] constexpr
	auto operator>= (rational<Num, Denom> left, rational<Num,Denom> right)
		-> decltype(!(left < right))
	{ return !(left < right); }

	template <typename Num, typename Denom>
	[[nodiscard]] constexpr
	auto operator<= (rational<Num, Denom> left, rational<Num,Denom> right)
		-> decltype(!(left > right))
	{ return !(left > right); }


} // namespace simple::support

#endif /* end of include guard */
