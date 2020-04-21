#ifndef SIMPLE_SUPPORT_RANDOM_ENGINE_TINY_HPP
#define SIMPLE_SUPPORT_RANDOM_ENGINE_TINY_HPP
#include <limits>
#include <climits>
#include <type_traits>
#include <iterator>
#include <array>
#include "basic_tiny.hpp"

namespace simple::support::random::engine
{

	template<typename UInt, UInt lucky = 13,
		std::enable_if_t<std::is_unsigned_v<UInt>> * = nullptr>
	class tiny : private basic_tiny<UInt, lucky>
	{

		public:
		using Base = basic_tiny<UInt, lucky>;
		using typename Base::result_type;
		using typename Base::buffer_type;
		constexpr static result_type min() { return std::numeric_limits<result_type>::min(); }
		constexpr static result_type max() { return std::numeric_limits<result_type>::max(); }

		private:

		template<typename Generator>
		using can_generate_t = decltype(
			std::declval<Generator>().generate(std::begin(std::declval<buffer_type&>()),
					std::end(std::declval<buffer_type&>()))
		)*;
		template<typename Engine>

		using returns_result_t = std::enable_if_t<
			std::is_convertible_v<std::invoke_result_t<Engine>, result_type> >*;

		public:

		constexpr tiny() = default;
		constexpr explicit tiny(result_type seed) noexcept : Base{seed} {};
		constexpr tiny(Base seed) noexcept : Base{std::move(seed)} {};

		template<typename Generator, can_generate_t<Generator> = nullptr>
		constexpr explicit tiny(Generator& gen)
		{
			seed(gen);
		}

		template<typename Engine, returns_result_t<Engine> = nullptr>
		constexpr explicit tiny(Engine& engine) noexcept(noexcept(std::declval<Engine>()()))
		{
			seed(engine);
		}

		template<typename Engine, returns_result_t<Engine> = nullptr>
		constexpr explicit tiny(Engine&& engine) noexcept(noexcept(std::declval<Engine>()())) : tiny(engine)
		{}

		constexpr void seed(result_type seed) noexcept { this->buffer = {seed}; }
		constexpr void seed(const std::array<result_type, 2>& seed) noexcept
		{ std::copy(seed.begin(), seed.end(), this->buffer); }

		template<typename Generator, can_generate_t<Generator> = nullptr>
		constexpr void seed(Generator& gen) // noexcept(bla bla bla)
		{
			gen.generate(std::begin(this->buffer), std::end(this->buffer));
		}

		template<typename Engine, returns_result_t<Engine> = nullptr>
		constexpr void seed(Engine& engine) noexcept(noexcept(std::declval<Engine>()()))
		{
			for(auto&& a : this->buffer)
				a = engine();
		}

		template<typename Engine, returns_result_t<Engine> = nullptr>
		constexpr void seed(Engine&& engine) noexcept(noexcept(std::declval<Engine>()()))
		{
			seed(engine);
		}

		constexpr void discard(unsigned long long count) noexcept
		{
			for(unsigned long long i = 0; i < count; ++i)
				Base::operator()();
		}

		constexpr result_type operator()() noexcept
		{
			return Base::operator()();
		}

		constexpr bool operator==(const tiny& other) const noexcept
		{
			return this->Base::operator==(other);
		}

		constexpr bool operator!=(const tiny& other) const noexcept
		{
			return this->Base::operator!=(other);
		}

		template <typename C, typename T, typename I>
		friend std::basic_ostream<C,T> & operator<<(std::basic_ostream<C,T>&, const tiny<I>&);

		template <typename C, typename T, typename I>
		friend std::basic_istream<C,T>& operator>>(std::basic_istream<C,T>&, tiny<I>&);

	};

	template <typename C, typename T, typename I>
	std::basic_ostream<C,T> & operator<<(std::basic_ostream<C,T>& os, const tiny<I>& t)
	{
		os << t.buffer[0];
		os << ' ';
		os << t.buffer[1];
		return os;
	}

	template <typename C, typename T, typename I>
	std::basic_istream<C,T>& operator>>(std::basic_istream<C,T>& is, tiny<I>& t)
	{
		is >> t.buffer[0];
		is >> t.buffer[1];
		return is;
	}

} // namespace simple::support::random::engine

#endif /* end of include guard */
