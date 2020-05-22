#include <cassert>
#include <random>
#include <iostream>
#include <iomanip>
#include "simple/support/random.hpp"
#include "simple/support/math.hpp"
using namespace simple::support;

void FloatEquality()
{
	// ummm how exactly do i do this?

	auto seed = std::random_device{}();
	std::cout << "Float equality test seed: " << std::hex << std::showbase << seed << std::endl;
	random::engine::tiny<unsigned long long> random{seed};
	std::uniform_real_distribution<double> number{};

	for(int i = 0; i < 10'000'000; ++i)
	{
		double a = number(random);
		double b = number(random);

		// someting like this??
		assert( almost_equal(a/b * b, a) );
		assert( almost_equal(b/a * a, b) );
		assert( almost_equal(std::sqrt(a)*std::sqrt(a), a) );
		assert( almost_equal(std::sqrt(b)*std::sqrt(b), b) );
		assert(	!almost_equal(a,b) ); // ummmm... well...  i mean... if this fails - you win! =)
	};

}

void FloatTruncation()
{
	auto seed = std::random_device{}();
	std::cout << "Float trunc test seed: " << std::hex << std::showbase << seed << std::endl;
	random::engine::tiny<unsigned long long> random{seed};
	std::uniform_real_distribution<long double> sign{-1,1};

	int trunced = 0;
	std::uniform_real_distribution<long double> number{
		std::numeric_limits<long double>::min(),
		(long double)std::numeric_limits<std::intmax_t>::max(),
	};
	for(int i = 0; i < 10'000'000; ++i)
	{
		long double a = std::copysign(number(random), sign(random));
		assert( trunc_up_to_intmax(a) == std::trunc(a) );
		if(trunc_up_to_intmax(a) != a)
			++trunced;
	};
	assert(trunced > 0);

	trunced = 0;
	std::uniform_real_distribution<long double> bigger_number{
		(long double)std::numeric_limits<std::intmax_t>::max(),
		std::numeric_limits<long double>::max(),
	};
	for(int i = 0; i < 10'000'000; ++i)
	{
		long double a = std::copysign(bigger_number(random), sign(random));
		assert( trunc_up_to_intmax(a) == std::trunc(a) );
		if(trunc_up_to_intmax(a) != a)
			++trunced;
	};
	assert(0 == trunced);

}

void BabelonianSquareRoot()
{
	auto seed = std::random_device{}();
	std::cout << "Babelonian square root test seed: " << std::hex << std::showbase << seed << std::endl;
	random::engine::tiny<unsigned long long> random{seed};
	std::uniform_real_distribution<double> number{};

	double good = 0;
	double bad = 0;
	int awful = 0;
	for(int i = 0; i < 10'000'000; ++i)
	{
		double n = number(random);
		if((std::abs(babelonian_root2_f(n) - std::sqrt(n)) <
			(std::sqrt(n) * std::numeric_limits<double>::epsilon())))
			++good;
		else
			++bad;

		if(!almost_equal(babelonian_root2_f(n) , std::sqrt(n)))
			++awful;
	}
	assert( (good / (good+bad)) > 0.99 );
	assert( awful == 0 );

	// edge cases
	assert( std::isnan(babelonian_root2_f(-1.f)) );
	assert( std::isnan(babelonian_root2_f(0.f/0.f)) );

	float x = 0;
	assert( babelonian_root2_f(x) == x );
	x = std::numeric_limits<float>::min()/2;
	assert( babelonian_root2_f(x) == x );
	x = std::numeric_limits<float>::max()*2;
	assert( babelonian_root2_f(x) == x );
}

constexpr bool Constexpr()
{
	void(root2(2.0));
	return true;
}

int main()
{
	FloatEquality();
	FloatTruncation();
	BabelonianSquareRoot();
	static_assert(Constexpr(),"");
	return 0;
}
