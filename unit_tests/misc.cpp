#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>

#include "simple/support/misc.hpp"

using namespace simple::support;

template <typename T>
constexpr auto tos (T v) { using std::to_string; return to_string(v); }

void StringToNumber()
{
	assert( 123 == ston<int>("123") );
	assert( -0xabc == ston<int>("-0xabc") );
	assert( -0123 == ston<int>("-0123") );
	assert( 123.123f == ston<float>(tos(123.123f)) );
	assert( -123.123 == ston<double>(tos(-123.123)) );
	assert( 123.123l == ston<long double>(tos(123.123l)) );
	assert( 123 == ston<int>("123xyz") );
	assert( 23 == ston<int>("123xyz", 1) );
	std::size_t i = 0;
	assert( 123 == ston<int>("123xyz", &i) );
	assert( 3 == i );
	i = 1;
	assert( 23 == ston<int>("123xyz", &i) );
	assert( 3 == i );

	try {
		ston<int>("abc123");
		assert( ((void)"ston did not throw an invalid argument exception", false) );
	} catch(const std::invalid_argument & e) { }

	try {
		ston<signed char>(std::to_string(std::numeric_limits<signed char>::max()) + '0');
		assert( ((void)"ston did not throw an out of range exception", false) );
	} catch(const std::out_of_range & e) { }

	try {
		ston<unsigned char>(std::to_string(std::numeric_limits<unsigned char>::max()) + '0');
		assert( ((void)"ston did not throw an out of range exception", false) );
	} catch(const std::out_of_range & e) { }
}

void StringToNumericRange()
{
	assert( (range<int>{12,34} == storn<int>("12-34")) );
	assert( (range<int>{12,24} == storn<int>("12:12")) );
	assert( (range<int>{-0xcd,-0xab} == storn<int>("-0xcd,-0xab")) );
	assert( (range<float>{12.12f,34.34f} == storn<float>(tos(12.12f) + '-' + tos(34.34f))) );
	assert( (range<double>{-12.12,12.12} == storn<double>(tos(-12.12) + ':' + tos(24.24))) );
	assert( (range<int>{12,34} == storn<int>("12-34.xyz")) );
	assert( (range<int>{2,34} == storn<int>("12-34.xyz", 1)) );
	std::size_t i = 0;
	assert( (range<int>{12,34} == storn<int>("12-34.xyz", &i)) );
	assert( 5 == i );
	i = 1;
	assert( (range<int>{2,34} == storn<int>("12-34.xyz", &i)) );
	assert( 5 == i );

	try {
		storn<int>("1232");
		assert( ((void)"storn did not throw an invalid argument exception", false) );
	} catch(const std::invalid_argument & e) { }

	try {
		storn<int>("12: ");
		assert( ((void)"storn did not throw an invalid argument exception", false) );
	} catch(const std::invalid_argument & e) { }

	try {
		storn<int>("12 : 32");
		assert( ((void)"storn did not throw an invalid argument exception", false) );
	} catch(const std::invalid_argument & e) { }
}

void NumericRangeToString()
{
	assert( "12-34" == tos(range<int>{12,34}) );
	assert( "12~34" == to_string(range<int>{12,34}, '~') );
	assert( "12:34" == to_string(range<int>{12,46}, ':') );
	assert( tos(-12.34) + '-' + tos(12.34) == tos(range<double>{-12.34,12.34}) );
}

int main()
{
	StringToNumber();
	StringToNumericRange();
	NumericRangeToString();
	return 0;
}
