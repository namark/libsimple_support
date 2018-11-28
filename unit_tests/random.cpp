#include <iostream>
#include <sstream>
#include <cassert>
#include <random>
#include <unordered_map>
#include "simple/support/random/engine/tiny.hpp"
#include "simple/support/random/distribution/naive.hpp"
#include "simple/support/random/distribution/diagonal.hpp"
#include "simple/support/misc.hpp"
#include "simple/support/algorithm.hpp"

using namespace simple::support;
using namespace random;
using namespace distribution;
using namespace engine;

std::random_device rd{};

template <typename Range>
double bit_entropy(Range& range)
{
	const auto size = range.end() - range.begin();
	static std::unordered_map<std::remove_reference_t<decltype(*range.begin())>, double> frequencies;

	for(auto&& ch : range)
		frequencies[ch] += 1;

	for(auto&& f : frequencies)
		f.second /= size;

	double entropy = 0;
	for(auto&& pair : frequencies)
	{
		const auto& f = pair.second;
		if(f != 0.0)
			entropy -= f*std::log2(f);
	}
	return entropy;
}

struct entropy_result
{
	double base;
	double byte_base;
	double variance;
	double byte_variance;
};

template<typename PRNG>
entropy_result get_entropy(PRNG prng)
{
	entropy_result result;

	std::vector<std::invoke_result_t<PRNG>> data(10000);
	std::generate(data.begin(), data.end(), prng);

	unsigned char* bytes = reinterpret_cast<unsigned char*>(data.data());
	const std::size_t byte_size = data.size() * sizeof(typename decltype(data)::value_type);
	auto byte_range = make_range(+bytes, bytes + byte_size);

	result.base = bit_entropy(data);
	result.byte_base = bit_entropy(byte_range);

	auto abs_minus = [](auto a, auto b)
	{
		return a > b ? a - b : b - a;
	};

	auto variance_range = variance(data, abs_minus);
	auto byte_variance_range = variance(byte_range, abs_minus);

	result.variance = bit_entropy(variance_range);
	result.byte_variance = bit_entropy(byte_variance_range);

	return result;
}

void TinyEngine()
{
	auto seed = rd();
	std::cout << "Tiny engine test seed: " << std::hex << std::showbase << seed << std::endl;
	tiny<unsigned long long> t{seed};
	tiny<unsigned long long> t2{seed};
	assert(t == t2);
	auto tiny_entropy = get_entropy(t);
	assert( tiny_entropy.byte_base > 7.995 );
	assert( tiny_entropy.byte_variance > 7.7 );

	t.discard(1000);
	assert(t2 != t);
	std::stringstream ss;
	ss << t;
	ss >> t2;
	assert(t2 == t);
	for(int i = 0; i < 10000; ++i)
		assert(t() == t2());

	t.seed({1,2});
	ss.clear();
	ss.str("1 2");
	tiny<unsigned long long> t3{};
	ss >> t3;
	assert( t == t3 );
}

void NaiveDistributions()
{
	auto seed = rd();
	std::cout << "Naive distribution test seed: " << std::hex << std::showbase << seed << std::endl;
	tiny<unsigned long long> t{seed};

	naive_int<int> ni(-128,127);
	auto ni_entropy = get_entropy([&ni, &t](){ return ni(t); });
	assert( ni_entropy.base > 7.97 );

	naive_real<float> nr(-128.0,127.0);
	auto nr_entropy = get_entropy([&nr, &t](){ return (int)nr(t); });
	assert( nr_entropy.base > 7.95 );
}


void DiagonalDistribution()
{
	auto seed = rd();
	std::cout << "Diagonal distribution test seed: " << std::hex << std::showbase << seed << std::endl;
	tiny<unsigned long long> t{seed};

	triangle<naive_real<double>> tri(0,1);
	for(int i = 0; i < 1000000; ++i)
	{
		auto a = tri(t);
		auto b = tri(t);
		assert((a + b <= 1.0));
	}

	tetrahedron<naive_real<double>> tetra(0,1);
	for(int i = 0; i < 1000000; ++i)
	{
		auto a = tetra(t);
		auto b = tetra(t);
		auto c = tetra(t);
		assert((a + b + c <= 1.0));
	}

	diagonal<naive_int<int>, 4, diagonal_side::middle> middle(0,100);
	for(int i = 0; i < 1000000; ++i)
	{
		auto a = middle(t);
		auto b = middle(t);
		auto c = middle(t);
		auto d = middle(t);
		assert((a + b + c + d == 100));
	}

	diagonal<naive_real<float>, 4, diagonal_side::upper> upper(0,100);
	for(int i = 0; i < 1000000; ++i)
	{
		auto a = upper(t);
		auto b = upper(t);
		auto c = upper(t);
		auto d = upper(t);
		assert(a <= 100);
		assert(b <= 100);
		assert(c <= 100);
		assert(d <= 100);
		assert((a + b + c + d >= 100));
	}

	diagonal<naive_int<int>, 3, diagonal_side::lower> li(0,255);
	auto li_entropy = get_entropy([&li, &t](){ return li(t); });
	assert( li_entropy.base > 7.3 );

	diagonal<naive_int<int>, 3, diagonal_side::upper> lu(0,255);
	auto lu_entropy = get_entropy([&lu, &t](){ return lu(t); });
	assert( lu_entropy.base > 7.9 );

	diagonal<naive_int<int>, 3, diagonal_side::middle> lm(0,255);
	auto lm_entropy = get_entropy([&lm, &t](){ return lm(t); });
	assert( lm_entropy.base > 7.65 );

	tetrahedron<std::uniform_real_distribution<double>> std_tetra(0,123);
	auto std_tetra2 = std_tetra;
	std::stringstream ss;
	ss << std_tetra2;
	ss >> std_tetra2;
	assert(std_tetra == std_tetra2);

}

void Noexcept()
{
	auto noexcept_seeder = []() noexcept {return 0;};
	static_assert(noexcept(tiny<unsigned int>{noexcept_seeder}));
	static_assert(!noexcept(tiny<unsigned int>{std::random_device{}}));
}

int main()
{
	TinyEngine();
	NaiveDistributions();
	DiagonalDistribution();
	Noexcept();
	return 0;
}
