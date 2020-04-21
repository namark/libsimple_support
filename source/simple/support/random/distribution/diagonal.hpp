#ifndef SIMPLE_SUPPORT_RANDOM_DISTRIBUTION_DIAGONAL_HPP
#define SIMPLE_SUPPORT_RANDOM_DISTRIBUTION_DIAGONAL_HPP
#include <algorithm>
#include "../../arithmetic.hpp"
#include "../../algorithm.hpp"
#include "../../array.hpp"

namespace simple::support::random::distribution
{

	enum class diagonal_side
	{
		lower,
		middle,
		upper
	};

	template<typename Base, size_t Dimensions, diagonal_side Side>
	class diagonal : private Base
	{
		public:
		constexpr static auto dimensions = Dimensions;

		using Base::Base;
		using typename Base::result_type;
		using typename Base::param_type;

		template<typename Engine>
		constexpr typename Base::result_type operator()(Engine& engine)
		{
			if constexpr (Side == diagonal_side::middle)
			{
				if(!sub_overflow(current_index, current_index, size_t(1)))
					return buffer[current_index];

				buffer[0] = Base::min();
				buffer[dimensions] = Base::max();

				for(size_t i = 1; i < dimensions; ++i)
					buffer[i] = Base::operator()(engine);

				std::sort(std::begin(buffer), std::end(buffer));

				const auto variance_range = variance(buffer);

				for(auto&& var : variance_range)
					var += Base::min();

				current_index = dimensions - 1;
				return buffer[current_index];
			}
			else
			{
				auto min = Base::min();
				auto max = Base::max();

				auto& current = buffer[current_index];
				typename Base::result_type reserved = 0;
				for(size_t i = 1; i < dimensions; ++i)
				{
					auto previous = wrap(current_index - i, dimensions);
					reserved += buffer[previous] - min;
				}

				if constexpr (Side == diagonal_side::lower)
					max -= reserved;
				else if constexpr(Side == diagonal_side::upper)
					min = reserved == 0 ? min : std::max(min, max - reserved);

				current = Base::operator()(engine, typename Base::param_type{min,max});
				++current_index;
				current_index = wrap(current_index, dimensions);
				return current;
			}
		}

		constexpr void reset() noexcept(noexcept(Base::reset()))
		{
			Base::reset();
			current_index = 0;
			for(auto&& result : buffer)
				result = result_type{};
		}

		constexpr result_type min() const noexcept(noexcept(Base::min()))
		{
			return Base::min();
		}

		constexpr result_type max() const noexcept(noexcept(Base::max()))
		{
			return Base::max();
		}

		constexpr param_type param() const noexcept(noexcept(Base::param()))
		{
			return Base::param();
		}

		constexpr void param(param_type p) const
		noexcept(noexcept(Base::param(std::declval<param_type>())))
		{
			Base::param(std::move(p));
		}

		constexpr bool operator==(const diagonal& other) const
		noexcept(noexcept(std::declval<Base>() == std::declval<Base>()))
		{
			bool equals = static_cast<const Base&>(*this) == static_cast<const Base&>(other);
			return equals && current_index == other.current_index && buffer == other.buffer;
		}

		constexpr bool operator!=(const diagonal& other) const
		noexcept(noexcept(std::declval<Base>() == std::declval<Base>()))
		{
			return !(*this == other);
		}

		template <typename C, typename T, typename B, size_t D, diagonal_side S>
		friend std::basic_ostream<C,T>& operator<<(std::basic_ostream<C,T>&, const diagonal<B,D,S>&);

		template <typename C, typename T, typename B, size_t D, diagonal_side S>
		friend std::basic_istream<C,T>& operator>>(std::basic_istream<C,T>&, diagonal<B,D,S>&);

		private:
		constexpr static size_t buffer_size(size_t dimensions, diagonal_side side)
		{
			return side == diagonal_side::middle ? dimensions + 1 : dimensions;
		}
		array<result_type, buffer_size(dimensions, Side)> buffer{};
		size_t current_index = 0;
	};

	template <typename C, typename T, typename Base, size_t Dimensions, diagonal_side S>
	std::basic_ostream<C,T>& operator<<(std::basic_ostream<C,T>& os, const diagonal<Base,Dimensions,S>& d)
	{
		auto precision = os.precision();
		auto flags = os.flags();
		os.precision(std::numeric_limits<typename Base::result_type>::max_digits10);
		os.setf(std::ios::scientific, std::ios::floatfield);

		os << static_cast<const Base&>(d);
		os << ' ';
		os << d.current_index;
		for(size_t i = 0; i < Dimensions; ++i)
			os << ' ' << d.buffer[i];

		os.precision(precision);
		os.flags(flags);
		return os;
	}

	template <typename C, typename T, typename Base, size_t Dimensions, diagonal_side S>
	std::basic_istream<C,T>& operator>>(std::basic_istream<C,T>& os, diagonal<Base,Dimensions,S>& d)
	{
		auto precision = os.precision();
		auto flags = os.flags();
		os.precision(std::numeric_limits<typename Base::result_type>::max_digits10);
		os.setf(std::ios::scientific, std::ios::floatfield);

		os >> static_cast<Base&>(d);
		os >> d.current_index;
		for(size_t i = 0; i < Dimensions; ++i)
			os >> d.buffer[i];

		os.precision(precision);
		os.flags(flags);
		return os;
	}

	template<typename Base>
	using triangle = diagonal<Base, 2, diagonal_side::lower>;

	template<typename Base>
	using tetrahedron = diagonal<Base, 3, diagonal_side::lower>;

} // namespace simple::support::random::distribution

#endif /* end of include guard */
