#ifndef SIMPLE_SUPPORT_ALGORITHM_RANGE_WRAPPERS_HPP
#define SIMPLE_SUPPORT_ALGORITHM_RANGE_WRAPPERS_HPP
#include <algorithm>
#include <string_view>

namespace simple::support
{

	// TODO: use the adl pattern for begin and end - unqualified call that defaults to std versions, noexcept being the tricky part of course


	// TODO: check if string view has begin-end constructor(should since c++20) and replace this with alias
	class string_view : public std::string_view
	{
		public:
		using std::string_view::string_view;
		string_view(const_iterator begin, const_iterator end)
			: std::string_view(begin, end-begin)
		{}
	};


	template <typename Range>
	constexpr auto min_element(Range& range)
	//TODO: noexcept account for return value construction
	noexcept(noexcept(std::min_element(std::begin(range), std::end(range))))
	{
		return std::min_element(std::begin(range), std::end(range));
	}

	template <typename Range>
	constexpr auto min_element(Range&& range)
	//TODO: noexcept account for return value construction
	noexcept(noexcept(std::min_element(std::begin(range), std::end(range))))
	{
		return *std::min_element(std::begin(range), std::end(range));
	}

	template <typename Range>
	constexpr auto max_element(Range& range)
	//TODO: noexcept account for return value construction
	noexcept(noexcept(std::max_element(std::begin(range), std::end(range))))
	{
		return std::max_element(std::begin(range), std::end(range));
	}

	template <typename Range>
	constexpr auto max_element(Range&& range)
	//TODO: noexcept account for return value construction
	noexcept(noexcept(std::max_element(std::begin(range), std::end(range))))
	{
		return *std::max_element(std::begin(range), std::end(range));
	}

	template <typename Range, typename Compare>
	constexpr auto min_element(Range& range, Compare comp)
	//TODO: noexcept account for return value construction
	noexcept(noexcept(std::min_element(std::begin(range), std::end(range), comp)))
	{
		return std::min_element(std::begin(range), std::end(range), comp);
	}

	template <typename Range, typename Compare>
	constexpr auto min_element(Range&& range, Compare comp)
	//TODO: noexcept account for return value construction
	noexcept(noexcept(std::min_element(std::begin(range), std::end(range), comp)))
	{
		return *std::min_element(std::begin(range), std::end(range), comp);
	}

	template <typename Range, typename Compare>
	constexpr auto max_element(Range& range, Compare comp)
	//TODO: noexcept account for return value construction
	noexcept(noexcept(std::max_element(std::begin(range), std::end(range), comp)))
	{
		return std::max_element(std::begin(range), std::end(range), comp);
	}

	template <typename Range, typename Compare>
	constexpr auto max_element(Range&& range, Compare comp)
	//TODO: noexcept account for return value construction
	noexcept(noexcept(std::max_element(std::begin(range), std::end(range), comp)))
	{
		return *std::max_element(std::begin(range), std::end(range), comp);
	}

	template <typename Range, typename UnaryPredicate>
	constexpr auto all_of(const Range& range, UnaryPredicate p)
	noexcept(noexcept(std::all_of(std::begin(range), std::end(range), p)))
	{
		return std::all_of(std::begin(range), std::end(range), p);
	}
	template <typename ExecutionPolicy, typename Range, typename UnaryPredicate>
	constexpr auto all_of(ExecutionPolicy&& policy, const Range& range, UnaryPredicate p)
	noexcept(noexcept(std::all_of(std::begin(range), std::end(range), p)))
	{
		return std::all_of(std::forward<ExecutionPolicy>(policy),
			std::begin(range), std::end(range), p);
	}

	template <typename Range, typename UnaryPredicate>
	constexpr auto any_of(const Range& range, UnaryPredicate p)
	noexcept(noexcept(std::any_of(std::begin(range), std::end(range), p)))
	{
		return std::any_of(std::begin(range), std::end(range), p);
	}
	template <typename ExecutionPolicy, typename Range, typename UnaryPredicate>
	constexpr auto any_of(ExecutionPolicy&& policy, const Range& range, UnaryPredicate p)
	noexcept(noexcept(std::any_of(std::begin(range), std::end(range), p)))
	{
		return std::any_of(std::forward<ExecutionPolicy>(policy),
			std::begin(range), std::end(range), p);
	}

	template <typename Range, typename UnaryPredicate>
	constexpr auto none_of(const Range& range, UnaryPredicate p)
	noexcept(noexcept(std::none_of(std::begin(range), std::end(range), p)))
	{
		return std::none_of(std::begin(range), std::end(range), p);
	}
	template <typename ExecutionPolicy, typename Range, typename UnaryPredicate>
	constexpr auto none_of(ExecutionPolicy&& policy, const Range& range, UnaryPredicate p)
	noexcept(noexcept(std::none_of(std::begin(range), std::end(range), p)))
	{
		return std::none_of(std::forward<ExecutionPolicy>(policy),
			std::begin(range), std::end(range), p);
	}

} // namespace simple::support

#endif /* end of include guard */
