#include <cassert>
#include <vector>
#include "simple/support/algorithm.hpp"

using namespace simple::support;


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

	int i = 0;
	for(auto&& element : get_iterator_range(arr, {3, 8}))
		assert(sub_array[i++] == element);

	i = 0;
	for(auto&& element : get_iterator_range(arr, {5, 800}))
		assert(sub_array_2[i++] == element);

	i = 0;
	for(auto&& element : get_iterator_range<int>(arr, {-105, 5}))
		assert(sub_array_3[i++] == element);

	i = 0;
	for(auto&& element : get_iterator_range<int>(arr, {-105, 105}))
		assert(arr[i++] == element);
}

constexpr bool Constexprness()
{
	range<int> v{};
	get_iterator_range(v.bounds, v);
	make_range(v.bounds);
	reverse_range(v.bounds);
	next_number(v.bounds);
	prev_number(v.bounds);
	return true;
}

int main()
{
	ContainerAsNumber();
	IteratorRange();
	static_assert(Constexprness());
	return 0;
}
