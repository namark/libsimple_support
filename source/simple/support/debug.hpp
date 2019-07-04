#ifndef SIMPLE_SUPPORT_DEBUG_HPP
#define SIMPLE_SUPPORT_DEBUG_HPP
#warning simple/support/debug.hpp included
#include <iostream>
#include <tuple>

namespace simple::support
{

	template <typename T>
	T&& print(T&& t)
	{
		std::cerr << t << std::flush;
		return std::forward<T>(t);
	}

	template <typename T>
	T&& println(T&& t)
	{
		std::cerr << t << std::endl;
		return std::forward<T>(t);
	}

	template <typename... T>
	auto print(T&&... t)
	{
		((void) (std::cerr << t), ...);
		std::cerr << std::flush;
		return std::forward_as_tuple(t...); // does this work??
	}

} // namespace simple::support

#endif /* end of include guard */
