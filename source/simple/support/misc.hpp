#ifndef SIMPLE_SUPPORT_MISC_CPP
#define SIMPLE_SUPPORT_MISC_CPP

#include <type_traits>
#include <limits>
#include <stdexcept>
#include <string>
#include <optional>

#include "range.hpp"

namespace simple::support
{

	template <typename N>
	N strton(const char* str, char ** end = nullptr)
	{
		static_assert(std::is_arithmetic_v<N>, "simple::support::strton expects an arithmetic type!");
		if constexpr (std::is_same_v<float, N>)
			return std::strtof(str, end);
		else if constexpr (std::is_same_v<double, N>)
			return std::strtod(str, end);
		else if constexpr (std::is_same_v<long double, N>)
			return std::strtold(str, end);
		else if constexpr (std::is_same_v<long, N>)
			return std::strtol(str, end, 0);
		else if constexpr (std::is_same_v<long long, N>)
			return std::strtoll(str, end, 0);
		else if constexpr (std::is_same_v<unsigned long, N>)
			return std::strtoul(str, end, 0);
		else if constexpr (std::is_same_v<unsigned long long, N>)
			return std::strtoull(str, end, 0);
		else if constexpr (std::is_signed_v<N>)
		{
			auto n = std::strtol(str, end, 0);
			constexpr auto limit = range<N>::limit();
			if(!limit.intersects(n))
			{
				errno = ERANGE;
				return n < 0 ? limit.lower() : limit.upper();
			}
			return static_cast<N>(n);
		}
		else if constexpr (std::is_unsigned_v<N>)
		{
			auto n = std::strtoul(str, end, 0);
			if(std::numeric_limits<N>::max() < n)
			{
				errno = ERANGE;
				return std::numeric_limits<N>::max();
			}
			return static_cast<N>(n);
		}
	}

	template <typename N>
	N ston(const std::string& s, std::size_t * start_end_index)
	{
		static_assert(std::is_arithmetic_v<N>, "simple::support::ston expects an arithmetic type!");
		errno = 0;
		const char* start = s.c_str() + (start_end_index ? *start_end_index : 0);
		char* end;
		N result = strton<N>(start, &end);
		if(ERANGE == errno)
			throw std::out_of_range("simple::support::ston");
		auto diff = end - start;
		if(!diff)
			throw std::invalid_argument("simple::support::ston");
		if(start_end_index)
			*start_end_index += diff;
		return result;
	}

	template <typename N>
	N ston(const std::string& s, std::size_t start_index = 0)
	{
		return ston<N>(s, &start_index);
	}

	template <typename Number>
	std::optional<Number> to_(const std::string& s)
	{
		static_assert(std::is_arithmetic_v<Number>, "simple::support::to_<Number> expects an arithmetic type!");
		errno = 0;
		const char* start = s.c_str();
		char* end;
		Number result = strton<Number>(start, &end);
		if(ERANGE == errno)
			return std::nullopt;
		auto diff = end - start;
		if(!diff)
			return std::nullopt;
		return result;
	}

	template <typename N>
	range<N> storn(const std::string& s, std::size_t * start_end_index)
	{
		static_assert(std::is_arithmetic_v<N>, "simple::support::storn expects an arithmetic type!");
		range<N> result{};
		std::size_t j = start_end_index ? *start_end_index : 0;
		result.lower() = ston<N>(s, &j);
		if(j == s.size())
			throw std::invalid_argument("simple::support::storn");
		auto delim = s[j];
		++j;
		result.upper() = ston<N>(s, &j);
		if(':' == delim)
			result.upper() += result.lower();
		if(start_end_index)
			*start_end_index = j;
		return result;
	}

	template <typename N>
	range<N> storn(const std::string& s, std::size_t start_index = 0)
	{
		return storn<N>(s, &start_index);
	}

	template <typename N>
	std::string to_string(range<N> r, const char& separator = '-')
	{
		using std::to_string;
		if(':' == separator)
			r.upper() -= r.lower();
		return to_string(r.lower()) + separator + to_string(r.upper());
	}

	template <typename It>
	std::string_view make_string_view(It begin, It end)
	{
		return {begin, static_cast<std::string_view::size_type>(end - begin)};
	}

} // namespace simple::support

#endif /* end of include guard */
