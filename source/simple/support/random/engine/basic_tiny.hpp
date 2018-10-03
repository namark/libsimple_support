#ifndef SIMPLE_SUPPORT_RANDOM_ENGINE_BASIC_TINY_HPP
#define SIMPLE_SUPPORT_RANDOM_ENGINE_BASIC_TINY_HPP
#include <climits>


namespace simple::support::random::engine
{

	template <typename UInt, UInt lucky = 13>
	struct basic_tiny
	{
		using result_type = UInt;
		using buffer_type = result_type[2];

		constexpr static auto bits = sizeof(result_type) * CHAR_BIT;
		constexpr static auto half_bits = bits >> 1;
		constexpr static auto remnant_bits = bits - half_bits;

		buffer_type buffer;

		constexpr result_type operator()() noexcept
		{
			buffer[1] += buffer[0]<<half_bits | buffer[0]>>remnant_bits;
			buffer[0] += buffer[1] + lucky;
			return buffer[1];
		}

		constexpr bool operator==(const basic_tiny& other) const noexcept
		{
			return buffer[0] == other.buffer[0] && buffer[1] == other.buffer[1];
		}

		constexpr bool operator!=(const basic_tiny& other) const noexcept
		{
			return !(*this == other);
		}

	};

} // namespace simple::support::random::engine

#endif /* end of include guard */
