#include "simple/support/carcdr.hpp"

using namespace simple::support;

int main()
{
	using ints = lisp_list<int, -1, 10, 13, 999>;
	static_assert(car<ints> == -1);
	static_assert(std::is_same_v< cdr<ints>, lisp_list<int, 10, 13, 999> >);
	static_assert(car< cdr<cdr<ints>> > == 13);
	static_assert(car<ints, 3> == 999);
	static_assert(car<lisp_list<int>, 0, 321> == 321);
	static_assert(car<ints, 10, -9517> == -9517); // *shrug*

	return 0;
}
