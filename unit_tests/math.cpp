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

void BabelonianSquareRoot()
{
	auto seed = std::random_device{}();
	std::cout << "Babelonion square root test seed: " << std::hex << std::showbase << seed << std::endl;
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
}

constexpr bool Constexpr()
{
	void(root2(2.0));
	return true;
}

int main()
{
	FloatEquality();
	BabelonianSquareRoot();
	static_assert(Constexpr(),"");
	return 0;
}
