#include <cassert>
#include <vector>
#include <numeric>
#include "simple/support/algorithm.hpp"


using namespace simple::support;

void MultidimentionalIteration()
{
	using std::rbegin;
	using std::rend;
	using Vector = std::array<int, 3>;
	const auto lower = Vector{13,3,-20};
	const auto upper = Vector{45,32,12};

	std::vector<Vector> test_data;
	std::vector<Vector> data;

	for(int k = lower[2]; k < upper[2]; ++k)
		for(int j = lower[1]; j < upper[1]; ++j)
			for(int i = lower[0]; i < upper[0]; ++i)
				test_data.push_back({i,j,k});

	auto i = lower;
	auto magnitude = i.begin();
	while(magnitude != i.end())
	{
		data.push_back(i);
		magnitude = advance_vector(i, lower, upper);
	}

	assert(data == test_data);


	test_data.clear();
	data.clear();
	Vector step = {1,2,3};

	for(int k = lower[2]; k < upper[2]; k += step[2])
		for(int j = lower[1]; j < upper[1]; j += step[1])
			for(int i = lower[0]; i < upper[0]; i += step[0])
				test_data.push_back({i,j,k});

	i = lower;
	magnitude = i.begin();
	while(magnitude != i.end())
	{
		data.push_back(i);
		magnitude = advance_vector(i, lower, upper, step);
	}

	assert(data == test_data);


	test_data.clear();
	data.clear();

	for(int k = 0; k < upper[2]; ++k)
		for(int j = 0; j < upper[1]; ++j)
			for(int i = 0; i < upper[0]; ++i)
				test_data.push_back({i,j,k});

	i = Vector{};
	magnitude = i.begin();
	while(magnitude != i.end())
	{
		data.push_back(i);
		magnitude = advance_vector(i, upper);
	}

	assert(data == test_data);

}

void ContainerAsNumber()
{
	using num = std::vector<int>;

	const std::vector<std::pair<num, int>> numbers {
		{{0,0,0,0}, 0}, // 0
		{{1,0,0,0}, 1}, // 1
		{{0,1,0,0}, 0}, // 2
		{{1,1,0,0}, 2}, // 3
		{{0,0,1,0}, 0}, // 4
		{{1,0,1,0}, 1}, // 5
		{{0,1,1,0}, 0}, // 6
		{{1,1,1,0}, 3}, // 7
		{{0,0,0,1}, 0}, // 8
		{{1,0,0,1}, 1}, // 9
		{{0,1,0,1}, 0}, // A
		{{1,1,0,1}, 2}, // B
		{{0,0,1,1}, 0}, // C
		{{1,0,1,1}, 1}, // D
		{{0,1,1,1}, 0}, // E
		{{1,1,1,1}, 4}, // F
	};

	num n = numbers.front().first;

	for(auto& [number, carry] : numbers)
	{
		assert(n == number);
		assert( next_number(n) - n.begin() == carry );
	}

	for(auto& [number, carry] : reverse_range(numbers))
	{
		assert( prev_number(n) - n.begin() == carry );
		assert( n == number );
	}
}

void IteratorRange()
{
	array<int, 10> arr {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	array<int, 5> sub_array{3, 4, 5, 6, 7};
	array<int, 5> sub_array_2{5, 6, 7, 8, 9};
	array<int, 5> sub_array_3{0, 1, 2, 3, 4};
	array<int, 5> rsub_array_2{9, 8, 7, 6, 5};

	int i = 0;
	for(auto&& element : get_iterator_range(arr, {3, 8}))
		assert(sub_array[i++] == element);

	i = 0;
	for(auto&& element : get_iterator_range(arr, {5, 800}))
		assert(sub_array_2[i++] == element);

	i = arr.size();;
	for(auto&& element : reverse_range(arr))
		assert(arr[--i] == element);


	i = 0;
	for(auto&& element : reverse(get_iterator_range(arr, {5, 800})))
		assert(rsub_array_2[i++] == element);


	i = 0;
	for(auto&& element : get_iterator_range<int>(arr, {-105, 5}))
		assert(sub_array_3[i++] == element);

	i = 0;
	for(auto&& element : get_iterator_range<int>(arr, {-105, 105}))
		assert(arr[i++] == element);
}

void Variance()
{
	array<int, 10> arr {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	auto var = arr;
	assert( variance(var).end() == var.end()-1 );
	assert( (var == array<int, 10>{1, 1, 1, 1, 1, 1, 1, 1, 1, 9}) );

	auto pair_sum = arr;
	variance(pair_sum, std::plus{});
	assert( (pair_sum == array<int, 10>{1, 3, 5, 7, 9, 11, 13, 15, 17, 9}) );

	array<int, 40> fib {1};
	std::adjacent_difference(fib.begin(), fib.end()-1, fib.begin()+1, std::plus{});
	auto varfib = fib;
	variance(varfib);
	assert( (std::equal(varfib.begin()+1, varfib.end() -1, fib.begin())) );
}

void Average()
{
	static_assert( average(1,2,3) == 2 );
	static_assert( average(1,2,3,4.f) == 10.f/4.f );
	static_assert( noexcept(average(1,2,3)) );
	static_assert( noexcept(average(1,2.0,3.f)) );
}

constexpr bool Constexprness()
{
	range<int> v{};
	range<int*> vitr{};
	(void)get_iterator_range(v.bounds, v);
	(void)make_range(v.bounds);
	(void)reverse_range(v.bounds);
	(void)reverse(vitr);
	auto itr = v.bounds.begin();
	advance_vector(itr,itr,itr);
	advance_vector(itr,itr,itr,itr);
	advance_vector(itr,itr,itr,itr,itr);
	advance_vector(v.bounds, v.bounds);
	advance_vector(v.bounds, v.bounds, v.bounds);
	advance_vector(v.bounds, v.bounds, v.bounds, v.bounds);
	next_number(v.bounds);
	prev_number(v.bounds);
	variance(v.bounds);
	void(wrap(1,1));
	void(midpoint(1,1));
	void(average(1,1));
	return true;
}

namespace adl_range_test
{
	struct segment
	{
		int* start;
		int size;
	};
	int* begin(const segment& x) { return x.start; }
	int* end(const segment& x) { return x.start + x.size; }
}

template <typename T> class show;
void TypeTraits()
{
	static_assert(is_range_v<int(&)[2]>);
	static_assert(!is_range_v<int[2]>);
	static_assert(is_range_v<std::array<int,2>>);
	static_assert(is_range_v<std::array<int,2>&>);
	static_assert(is_range_v<adl_range_test::segment>);
}

int main()
{
	MultidimentionalIteration();
	ContainerAsNumber();
	IteratorRange();
	Variance();
	TypeTraits();
	static_assert(Constexprness());
	return 0;
}
