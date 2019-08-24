#ifndef SIMPLE_SUPPORT_MATH_FLOAT_HPP
#define SIMPLE_SUPPORT_MATH_FLOAT_HPP

#include <type_traits>
#include <limits>
#include <cmath>

namespace simple::support
{

	// taken (almost) straight from cppreference.com
	// no idea how it works
	template<class T>
	constexpr
	typename std::enable_if_t<std::is_floating_point_v<T>, bool>
	equal_ulp(T x, T y, size_t ulp)
	{
		// the machine epsilon has to be scaled to the magnitude of the values used
		// and multiplied by the desired precision in ULPs (units in the last place)
		return std::abs(x-y) <= std::numeric_limits<T>::epsilon() * std::abs(x+y) * ulp
			// unless the result is subnormal
			|| std::abs(x-y) < std::numeric_limits<T>::min();
	}

	template<class T>
	constexpr
	typename std::enable_if_t<std::is_floating_point_v<T>, bool>
	almost_equal(T x, T y)
	{
		return equal_ulp(x,y,1);
	}

} // namespace simple::support

#endif /* end of include guard */
