//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: c++98, c++03, c++11, c++14, c++17
// <numeric>

// template <class _Tp>
// _Tp midpoint(_Tp __a, _Tp __b) noexcept
//

#include <stdint.h>
#include <limits>
#include <numeric>
#include <cassert>
#include "simple/support/algorithm.hpp"

using namespace simple::support;

template <typename T>
void signed_test()
{
    constexpr T zero{0};
    constexpr T one{1};
    constexpr T two{2};
    constexpr T three{3};
    constexpr T four{4};

    static_assert(std::is_same_v<decltype(midpoint(T(), T())), T>);
    // ASSERT_NOEXCEPT(          midpoint(T(), T()));
    using limits = std::numeric_limits<T>;

    static_assert(midpoint(one, three) == two, "");
    static_assert(midpoint(three, one) == two, "");

    assert(midpoint(zero, zero) == zero);
    assert(midpoint(zero, two)  == one);
    assert(midpoint(two, zero)  == one);
    assert(midpoint(two, two)   == two);

    assert(midpoint(one, four)    == two);
    assert(midpoint(four, one)    == three);
    assert(midpoint(three, four)  == three);
    assert(midpoint(four, three)  == four);

    assert(midpoint(T( 3), T( 4)) == T(3));
    assert(midpoint(T( 4), T( 3)) == T(4));
    assert(midpoint(T(-3), T( 4)) == T(0));
    assert(midpoint(T(-4), T( 3)) == T(-1));
    assert(midpoint(T( 3), T(-4)) == T(0));
    assert(midpoint(T( 4), T(-3)) == T(1));
    assert(midpoint(T(-3), T(-4)) == T(-3));
    assert(midpoint(T(-4), T(-3)) == T(-4));

    static_assert(midpoint(limits::min(), limits::max()) == T(-1), "");
    static_assert(midpoint(limits::max(), limits::min()) == T( 0), "");

    static_assert(midpoint(limits::min(), T(6)) == limits::min()/2 + 3, "");
    assert(       midpoint(T(6), limits::min()) == limits::min()/2 + 3);
    assert(       midpoint(limits::max(), T(6)) == limits::max()/2 + 4);
    static_assert(midpoint(T(6), limits::max()) == limits::max()/2 + 3, "");

    assert(       midpoint(limits::min(), T(-6)) == limits::min()/2 - 3);
    static_assert(midpoint(T(-6), limits::min()) == limits::min()/2 - 3, "");
    static_assert(midpoint(limits::max(), T(-6)) == limits::max()/2 - 2, "");
    assert(       midpoint(T(-6), limits::max()) == limits::max()/2 - 3);
}

template <typename T>
void unsigned_test()
{
    constexpr T zero{0};
    constexpr T one{1};
    constexpr T two{2};
    constexpr T three{3};
    constexpr T four{4};

    static_assert(std::is_same_v<decltype(midpoint(T(), T())), T>);
    // ASSERT_NOEXCEPT(          midpoint(T(), T()));
    using limits = std::numeric_limits<T>;
    const T half_way = (limits::max() - limits::min())/2;

    static_assert(midpoint(one, three) == two, "");
    static_assert(midpoint(three, one) == two, "");

    assert(midpoint(zero, zero) == zero);
    assert(midpoint(zero, two)  == one);
    assert(midpoint(two, zero)  == one);
    assert(midpoint(two, two)   == two);

    assert(midpoint(one, four)    == two);
    assert(midpoint(four, one)    == three);
    assert(midpoint(three, four)  == three);
    assert(midpoint(four, three)  == four);

    assert(midpoint(limits::min(), limits::max()) == T(half_way));
    assert(midpoint(limits::max(), limits::min()) == T(half_way + 1));

    static_assert(midpoint(limits::min(), T(6)) == limits::min()/2 + 3, "");
    assert(       midpoint(T(6), limits::min()) == limits::min()/2 + 3);
    assert(       midpoint(limits::max(), T(6)) == half_way + 4);
    static_assert(midpoint(T(6), limits::max()) == half_way + 3, "");

    static_assert(std::is_same_v<decltype(umidpoint(T(), T())), T>);
    // ASSERT_NOEXCEPT(          midpoint(T(), T()));
    static_assert(umidpoint(one, three) == two, "");
    static_assert(umidpoint(three, one) == two, "");

    assert(umidpoint(zero, zero) == zero);
    assert(umidpoint(zero, two)  == one);
    assert(umidpoint(two, zero)  == one);
    assert(umidpoint(two, two)   == two);

    assert(umidpoint(one, four)    == two);
    assert(umidpoint(four, one)    == three);
    assert(umidpoint(three, four)  == three);
    assert(umidpoint(four, three)  == four);

    assert(umidpoint(limits::min(), limits::max()) == T(half_way));
    assert(umidpoint(limits::max(), limits::min()) == T(half_way + 1));

    static_assert(umidpoint(limits::min(), T(6)) == limits::min()/2 + 3, "");
    assert(       umidpoint(T(6), limits::min()) == limits::min()/2 + 3);
    assert(       umidpoint(limits::max(), T(6)) == half_way + 4);
    static_assert(umidpoint(T(6), limits::max()) == half_way + 3, "");
}


int main(int, char**)
{
    signed_test<signed char>();
    signed_test<short>();
    signed_test<int>();
    signed_test<long>();
    signed_test<long long>();

    signed_test<int8_t>();
    signed_test<int16_t>();
    signed_test<int32_t>();
    signed_test<int64_t>();

    unsigned_test<unsigned char>();
    unsigned_test<unsigned short>();
    unsigned_test<unsigned int>();
    unsigned_test<unsigned long>();
    unsigned_test<unsigned long long>();

    unsigned_test<uint8_t>();
    unsigned_test<uint16_t>();
    unsigned_test<uint32_t>();
    unsigned_test<uint64_t>();

#ifndef _LIBCPP_HAS_NO_INT128
    // unsigned_test<__uint128_t>();
    // signed_test<__int128_t>();
#endif

//     int_test<char>();
    signed_test<ptrdiff_t>();
    unsigned_test<size_t>();

    return 0;
}
