#include <cassert>
#include <random>
#include <iostream>
#include "simple/support/range.hpp"

using namespace simple::support;

void Validity()
{
	range valid_range{-13,31};
	assert(valid_range.valid());
	valid_range.fix();
	assert(valid_range.valid());

	range invalid_range{valid_range.upper(), valid_range.lower()};
	const range const_invalid_range = invalid_range;
	assert(!invalid_range.valid());
	assert(!const_invalid_range.valid());
	invalid_range.fix();
	const range const_valid_range = const_invalid_range.fix();
	assert(invalid_range.valid());
	assert(invalid_range == valid_range);
	assert(const_valid_range.valid());
	assert(const_valid_range == valid_range);

	assert(( const_valid_range == range{31,-13}.fix() ));
}

void SetLikePredicates()
{
	range r{-13,31};

	assert(contains(r, 2));
	assert(!contains(r, 45));
	assert(!contains(r, r.lower()));
	assert(!contains(r, r.upper()));

	assert(intersects(r, 12));
	assert(!intersects(r, 42));
	assert(intersects(r, r.lower()));
	assert(intersects(r, r.upper()));

	assert(intersects_lower(r, 13));
	assert(!intersects_lower(r, 41));
	assert(intersects_lower(r, r.lower()));
	assert(!intersects_lower(r, r.upper()));

	assert(intersects_upper(r, 14));
	assert(!intersects_upper(r, 40));
	assert(!intersects_upper(r, r.lower()));
	assert(intersects_upper(r, r.upper()));

	assert(contains(r, {12, 21}));
	assert(!contains(r, {-21, 53}));
	assert(!contains(r, {r.lower(), 0}));
	assert(!contains(r, {0, r.upper()}));
	assert(!contains(r, r));

	assert(covers(r, {12, 21}));
	assert(!covers(r, {-21, 53}));
	assert(covers(r, {r.lower(), 0}));
	assert(!covers(r, {r.lower(), 53}));
	assert(covers(r, {0, r.upper()}));
	assert(!covers(r, {-21, r.upper()}));
	assert(covers(r, r));

	assert(overlaps(r, {12, 21}));
	assert(overlaps(r, {-21, 53}));
	assert(overlaps(r, {r.lower(), 0}));
	assert(overlaps(r, {r.lower(), 53}));
	assert(overlaps(r, {0, r.upper()}));
	assert(overlaps(r, {-21, r.upper()}));
	assert(!overlaps(r, {r.upper(), 102}));
	assert(!overlaps(r, {-102, r.lower()}));
	assert(overlaps(r, r));
	assert(!overlaps(r, {67, 102}));
	assert(!overlaps(r, {-67, -102}));

	assert(intersects(r, {12, 21}));
	assert(intersects(r, {-21, 53}));
	assert(intersects(r, {r.lower(), 0}));
	assert(intersects(r, {r.lower(), 53}));
	assert(intersects(r, {0, r.upper()}));
	assert(intersects(r, {-21, r.upper()}));
	assert(intersects(r, {r.upper(), 102}));
	assert(intersects(r, {-102, r.lower()}));
	assert(intersects(r, r));
	assert(!intersects(r, {67, 102}));
	assert(!intersects(r, {-67, -102}));

	assert(( intersection(r, {12,56}) == range{12,31} ));
	assert(( intersection(r, {12,22}) == range{12,22} ));
	assert(( intersection(r, {-31,13}) == range{-13,13} ));
	assert(( !intersection(r, {-31,-21}).valid() ));
	assert(( !intersection(r, {32,52}).valid() ));

}

void Clamping()
{
	assert(13 == clamp(44, {-13, 13}));
	assert(-13 == clamp(-44, {-13, 13}));
	int a = 44;
	clamp_in_place(a, {-13, 13});
	assert(13 == a);

	assert((clamp(range{-13, 13}, {-3, 3}) == range{-3, 3}));
	assert((clamp(range{-13, 2}, {-3, 3}) == range{-3, 2}));
	assert((clamp(range{0, 2}, {-3, 3}) == range{0, 2}));

	// clang version 7.0.0-svn345923-1~exp1~20181102032948.33 chokes on these without the bool casts
	range b{-13, 13};
	clamp_in_place(b, {-10, 10});
	assert(bool(range{-10, 10} == b));
	clamp_in_place(b, {-20, 5});
	assert(bool(range{-10, 5} == b));
	clamp_in_place(b, {-4, 50});
	assert(bool(range{-4, 5} == b));
	clamp_in_place(b, {-40, 50});
	assert(bool(range{-4, 5} == b));
}

void SubRange()
{
	range rng {0, 9};
	range sub_rng{3, 8};
	range sub_rng_2{5, 9};
	range sub_rng_3{0, 5};

	assert( sub_rng == rng.sub_range(range{3, 8}) );
	assert( sub_rng_2 == rng.sub_range(range{5, 800}) );
	assert( sub_rng_3 == rng.sub_range(range{-105, 5}) );
	assert( rng == rng.sub_range(range{-105, 105}) );

	auto limit = range<int>::limit();
	range inc{-1,1};

	assert( inc == inc.sub_range(limit) );

	range near_limit {limit.upper(), limit.upper()};
	near_limit -= 100;
	near_limit.upper() += 50;
	range overflow {25, 125};
	range sub_near_limit = near_limit;
	sub_near_limit.lower() += 25;
	assert( sub_near_limit == near_limit.sub_range(overflow) );
}

template <typename Larger, typename Smaller>
void limits_check_containment(std::mt19937_64 & generator)
{
	std::uniform_int_distribution<Smaller> uid(std::numeric_limits<Smaller>::lowest());

	constexpr auto bounds = range<Larger>::limit();
	assert(	bounds.lower() == std::numeric_limits<Larger>::lowest() &&
			bounds.upper() == std::numeric_limits<Larger>::max() );
	bool out_of_bounds = false;
	for(int i = 0; i < 100'000; ++i)
		if( out_of_bounds |= (!bounds.intersects(uid(generator))), out_of_bounds )
			break;
	assert(!out_of_bounds);
}

template <typename Larger, typename Smaller>
void limits_check_containment_f(std::mt19937_64 & generator)
{
	std::uniform_real_distribution<Smaller> uid(
			std::numeric_limits<Smaller>::min(),
			std::numeric_limits<Smaller>::max());
	std::uniform_real_distribution<Smaller> sign(-1,1);

	constexpr auto bounds = range<Larger>::limit();
	assert(	bounds.lower() == std::numeric_limits<Larger>::lowest() &&
			bounds.upper() == std::numeric_limits<Larger>::max() );
	bool out_of_bounds = false;
	for(int i = 0; i < 100'000; ++i)
	{
		auto value = std::copysign(uid(generator), sign(generator));
		if( out_of_bounds |= !bounds.intersects(value), out_of_bounds )
			break;
	}
	assert(!out_of_bounds);
}

void Limit()
{
	auto seed = std::random_device{}();
	std::cout << "Range limit random test seed: " << std::hex << std::showbase << seed << std::endl;
	std::mt19937_64 generator(seed);
	limits_check_containment<signed char, signed char>(generator);
	limits_check_containment<signed short, signed char>(generator);
	limits_check_containment<signed short, signed short>(generator);
	limits_check_containment<signed int, signed short>(generator);
	limits_check_containment<signed int, signed int>(generator);
	limits_check_containment<signed long, signed int>(generator);
	limits_check_containment<signed long, signed long>(generator);
	limits_check_containment<signed long long, signed long>(generator);
	limits_check_containment<signed long long, signed long long>(generator);
	limits_check_containment_f<float, float>(generator);
	limits_check_containment_f<double, float>(generator);
	limits_check_containment_f<double, double>(generator);
	limits_check_containment_f<long double, double>(generator);
	limits_check_containment_f<long double, long double>(generator);

	if(std::numeric_limits<signed long long>::lowest() < std::numeric_limits<signed char>::lowest())
		assert( !range<signed char>::limit().contains(std::numeric_limits<signed long long>::lowest()) );
	if(std::numeric_limits<signed long long>::max() > std::numeric_limits<signed char>::max())
		assert( !range<signed char>::limit().contains(std::numeric_limits<signed long long>::max()) );
}

void Arithmetic()
{
	{range<float> r{-1,1};
		assert(( r + 0.25 == range<float>{-0.75,1.25} ));
		r += 0.25;
		assert(( r == range<float>{-0.75,1.25} ));
	}

	{range<float> r{-1,1};
		assert(( r - 0.25 == range<float>{-1.25,0.75} ));
		r -= 0.25;
		assert(( r == range<float>{-1.25,0.75} ));
	}

	{range<float> r{-1,1};
		assert(( r * 13 == range<float>{-13,13} ));
		r *= 13;
		assert(( r == range<float>{-13,13} ));
	}

	{range<float> r{-1,1};
		assert(( r / 2 == range<float>{-0.5,0.5} ));
		r /= 2;
		assert(( r == range<float>{-0.5,0.5} ));
	}
}

void Iterator()
{
	static_assert(std::is_same_v<int*, range<int*>::iterator>);
	static_assert(std::is_same_v<void, range<int>::iterator>);
}

constexpr bool Constexprness()
{
	range<int> v{};
	if(v == v)
	{
		v.lower() = 3;
		v.upper() = 1;
	};
	if(!v.valid())
		v.fix();
	const range<int> cv = v;
	v = cv.fix();
	v = range<int>{}.fix();
	clamp_in_place(v, {12,13});
	int i = 1;
	clamp_in_place(i, v);
	clamp(v,v);
	clamp(i,v);
	contains(v, i);
	contains(v, v);
	covers(v, v);
	intersects(v, i);
	intersects(v, v);
	overlaps(v, v);
	intersects_lower(v, i);
	intersects_upper(v, i);
	v.contains(i);
	v.contains(v);
	v.covers(v);
	v.intersects(i);
	v.intersects(v);
	v.overlaps(v);
	v.intersects_lower(i);
	v.intersects_upper(i);
	(void)v.sub_range(v);
	v += 1;
	v = v+1;
	v -= 1;
	v = v-1;
	v *= 1;
	v = v*1;
	v /= 1;
	v = v/1;
	return true;
}

int main()
{
	Validity();
	SetLikePredicates();
	Clamping();
	SubRange();
	Limit();
	Arithmetic();
	Iterator();
	static_assert(Constexprness());
	return 0;
}
