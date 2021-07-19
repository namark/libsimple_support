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

void SimplifiedToNumber()
{
	assert( 123 == *to_<int>("123") );
	assert( -0xabc == *to_<int>("-0xabc") );
	assert( -0123 == *to_<int>("-0123") );
	assert( 123.123f == *to_<float>(tos(123.123f)) );
	assert( -123.123 == *to_<double>(tos(-123.123)) );
	assert( 123.123l == *to_<long double>(tos(123.123l)) );
	assert( 123 == *to_<int>("123xyz") );

	assert(std::nullopt == to_<int>("abc123"));

	assert(std::nullopt ==
		to_<signed char>(std::to_string(std::numeric_limits<signed char>::max()) + '0'));
	assert(std::nullopt ==
		to_<unsigned char>(std::to_string(std::numeric_limits<unsigned char>::max()) + '0'));
}

void RangeReference()
{
	using std::begin;
	using std::end;

	const std::string prefix = "Permaprefix";

	std::string	stuff{prefix};

	range temporef{begin(stuff), end(stuff)};

	assert( std::equal(begin(temporef), end(temporef),
		begin(prefix), end(prefix)) );

	index_range permaref(stuff, {begin(stuff), end(stuff)});

	assert( std::equal(begin(permaref), end(permaref),
		begin(prefix), end(prefix)) );

	stuff += " now this a really really long long paragraph of text, that should by all means cause reallocation and stuff, so the iterators in temporef range will be invalidated, while permaref that uses indices will remain valid and usable... though it doesn't really matter if it actually relocates since there is no way to check any of this, you'll just have to believe me and trust me and never ever let me go...";

	// can't do this anymore
	// assert( std::equal(begin(temporef), end(temporef),
	// 	begin(prefix), end(prefix)) );
	// asan might not catch this, because of small string optimization, and since you are allowed to reinterpret anything as char* it might not even be UB dependent on the implementation, but then the assertion will fail at least
	// otherwise the assertion might not fail, if the old/freed memory happens to remain untouched by either the system or the allocator
	// so asan and assert together can kind of catch this I guess
	// TODO: __gnu_debug::string (but not std::string under _GLIBCXX_DEBUG, it's an obscure exception) does catch this reliably, so that's a thing to consider for unit testing purposes in general

	// all ok here
	assert( std::equal(begin(permaref), end(permaref),
		begin(prefix), end(prefix)) );

}

int main()
{
	StringToNumber();
	StringToNumericRange();
	NumericRangeToString();
	SimplifiedToNumber();
	RangeReference();
	return 0;
}
