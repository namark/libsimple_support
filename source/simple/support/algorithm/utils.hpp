#ifndef SIMPLE_SUPPORT_ALGORITHM_UTILS_HPP
#define SIMPLE_SUPPORT_ALGORITHM_UTILS_HPP
#include "traits.hpp"
#include "../range.hpp"

namespace simple::support
{

	template <typename Range, std::enable_if_t<is_range_v<Range>>* = nullptr>
	constexpr auto distance(Range&& range)
	{
		using std::begin;
		using std::end;
		return std::distance(begin(range), end(range));
	}

	template <typename Container>
	[[nodiscard]]
	constexpr auto make_range(Container& container)
	{
		using std::begin;
		using std::end;
		return make_range(begin(container), end(container));
	}

	template <typename Container>
	constexpr auto make_range(Container&& container) = delete;

	template <typename Container>
	[[nodiscard]]
	constexpr auto reverse_range(Container& container)
	{
		using std::rbegin;
		using std::rend;
		return make_range(rbegin(container), rend(container));
	}

	template <typename Container>
	constexpr auto reverse_range(Container&& container) = delete;

	template<typename Index = std::size_t, typename Container>
	[[nodiscard]] constexpr
	auto get_iterator_range(Container&& container, const range<Index>& index_range)
	{
		return make_range(std::forward<Container>(container)).sub_range(index_range);
	}

	// TODO: maybe useful if generalized to any range like type, and makes use of std::ditance
	template<typename It>
	[[deprecated("no reason to use this anymore, can just subtract the origin from the range now")]]
	[[nodiscard]] constexpr
	auto distance(const range<It>& rng, const It& origin)
	{
		using std::begin;
		using std::end;
		return range{ begin(rng) - origin, end(rng) - origin };
	}

	template <typename From, typename To, typename Origin>
	[[nodiscard]] constexpr
	auto map_range(const From& from, To&& to, Origin origin)
	{
		return get_iterator_range(to, distance(from, origin));
	}

	template <typename From, typename To>
	[[nodiscard]] constexpr
	auto map_range(const From& from, const To& to)
	{
		using std::begin;
		return get_iterator_range(to, distance(from, begin(from)));
	}

	// std::swap is not constexpr >.<
	template <typename T, std::enable_if_t<
		std::is_move_constructible_v<T> &&
		std::is_move_assignable_v<T>
	>* = nullptr>
	constexpr void swap(T& one, T& other)
	noexcept(
		std::is_nothrow_move_constructible_v<T> &&
		std::is_nothrow_move_assignable_v<T>
	)
	{
		T temp{std::move(other)};
		other = std::move(one);
		one = std::move(temp);
	}


} // namespace simple::support

#endif /* end of include guard */
