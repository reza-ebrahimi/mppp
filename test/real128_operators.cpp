// Copyright 2016-2017 Francesco Biscani (bluescarni@gmail.com)
//
// This file is part of the mp++ library.
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <mp++/mp++.hpp>
#include <type_traits>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace mppp;

using int_t = integer<1>;
using rat_t = rational<1>;

// Some tests involving constexpr result in an ICE on GCC < 6.
#if (defined(_MSC_VER) || defined(__clang__) || __GNUC__ >= 6) && MPPP_CPLUSPLUS >= 201402L

#define MPPP_ENABLE_CONSTEXPR_TESTS

#endif

#if defined(MPPP_ENABLE_CONSTEXPR_TESTS)

static constexpr real128 test_constexpr_incr()
{
    real128 retval;
    ++retval;
    retval++;
    return retval;
}

static constexpr real128 test_constexpr_decr()
{
    real128 retval;
    --retval;
    retval--;
    return retval;
}

#endif

TEST_CASE("real128 plus")
{
    real128 x;
    REQUIRE((std::is_same<decltype(+x), real128>::value));
    REQUIRE((std::is_same<decltype(real128{56} + real128{3}), real128>::value));
    REQUIRE((std::is_same<decltype(x + 3), real128>::value));
    REQUIRE((std::is_same<decltype(3. + x), real128>::value));
    REQUIRE((std::is_same<decltype(x + int_t{3}), real128>::value));
    REQUIRE((std::is_same<decltype(int_t{3} + x), real128>::value));
    REQUIRE((std::is_same<decltype(-x), real128>::value));
    REQUIRE((std::is_same<decltype(real128{56} - real128{3}), real128>::value));
    REQUIRE((std::is_same<decltype(x - 3), real128>::value));
    REQUIRE((std::is_same<decltype(3. - x), real128>::value));
    REQUIRE((std::is_same<decltype(x - int_t{3}), real128>::value));
    REQUIRE((std::is_same<decltype(int_t{3} - x), real128>::value));
    REQUIRE((std::is_same<decltype(real128{56} * real128{3}), real128>::value));
    REQUIRE((std::is_same<decltype(x * 3), real128>::value));
    REQUIRE((std::is_same<decltype(3. * x), real128>::value));
    REQUIRE((std::is_same<decltype(x * int_t{3}), real128>::value));
    REQUIRE((std::is_same<decltype(int_t{3} * x), real128>::value));
    REQUIRE((std::is_same<decltype(real128{56} / real128{3}), real128>::value));
    REQUIRE((std::is_same<decltype(x / 3), real128>::value));
    REQUIRE((std::is_same<decltype(3. / x), real128>::value));
    REQUIRE((std::is_same<decltype(x / int_t{3}), real128>::value));
    REQUIRE((std::is_same<decltype(int_t{3} / x), real128>::value));
    REQUIRE(((+x).m_value == 0));
    x = -145;
    REQUIRE(((+x).m_value == -145));
    real128 y{12};
    x = -5;
    REQUIRE(((x + y).m_value == 7));
    constexpr auto z1 = real128{56} + real128{3};
    constexpr auto z1a = +z1;
    REQUIRE((z1.m_value == 59));
    REQUIRE((z1a.m_value == 59));
    REQUIRE(((x + 3).m_value == -2));
    REQUIRE(((x + 2.).m_value == -3));
    REQUIRE(((3 + x).m_value == -2));
    REQUIRE(((2. + x).m_value == -3));
    constexpr auto z2 = real128{56} + 3;
    REQUIRE((z2.m_value == 59));
    constexpr auto z3 = 3.f + real128{56};
    REQUIRE((z3.m_value == 59));
    REQUIRE(((x + int_t{3}).m_value == -2));
    REQUIRE(((int_t{3} + x).m_value == -2));
    REQUIRE(((x + rat_t{3, 2}).m_value == real128{"-3.5"}.m_value));
    REQUIRE(((rat_t{3, 2} + x).m_value == real128{"-3.5"}.m_value));
    x = 5;
    REQUIRE(((++x).m_value == 6));
    REQUIRE(((x++).m_value == 6));
    REQUIRE(((x).m_value == 7));
#if defined(MPPP_ENABLE_CONSTEXPR_TESTS)
    constexpr auto z4 = test_constexpr_incr();
    REQUIRE((z4.m_value == 2));
#endif
    REQUIRE(((-real128{}).m_value == 0));
    REQUIRE((-real128{}).signbit());
    REQUIRE(((-real128{123}).m_value == -123));
    REQUIRE(((-real128{-123}).m_value == 123));
    constexpr auto z5 = -real128{-45};
    REQUIRE(((x - 3).m_value == 4));
    REQUIRE(((x - 2.).m_value == 5));
    REQUIRE(((3 - x).m_value == -4));
    REQUIRE(((2. - x).m_value == -5));
    constexpr auto z5a = real128{56} - 3;
    REQUIRE((z5a.m_value == 53));
    constexpr auto z5b = 3.f - real128{56};
    REQUIRE((z5b.m_value == -53));
    REQUIRE(((x - int_t{3}).m_value == 4));
    REQUIRE(((int_t{3} - x).m_value == -4));
    REQUIRE(((x - rat_t{3, 2}).m_value == real128{"5.5"}.m_value));
    REQUIRE(((rat_t{3, 2} - x).m_value == real128{"-5.5"}.m_value));
    REQUIRE((z5.m_value == 45));
    REQUIRE(((--x).m_value == 6));
    REQUIRE(((x--).m_value == 6));
    REQUIRE(((x).m_value == 5));
#if defined(MPPP_ENABLE_CONSTEXPR_TESTS)
    constexpr auto z6 = test_constexpr_decr();
    REQUIRE((z6.m_value == -2));
#endif
    REQUIRE(((x * 3).m_value == 15));
    REQUIRE(((x * 2.).m_value == 10));
    REQUIRE(((-3 * x).m_value == -15));
    REQUIRE(((2. * x).m_value == 10));
    constexpr auto z7 = real128{56} * 3;
    REQUIRE((z7.m_value == 168));
    constexpr auto z8 = 3.f * -real128{56};
    REQUIRE((z8.m_value == -168));
    REQUIRE(((x * int_t{3}).m_value == 15));
    REQUIRE(((int_t{3} * -x).m_value == -15));
    REQUIRE(((x * rat_t{3, 2}).m_value == real128{"7.5"}.m_value));
    REQUIRE(((rat_t{3, 2} * x).m_value == real128{"7.5"}.m_value));
    x = 12;
    REQUIRE(((x / 3).m_value == 4));
    REQUIRE(((x / 2.).m_value == 6));
    REQUIRE(((-6 / x).m_value == real128{"-.5"}.m_value));
    REQUIRE(((3. / x).m_value == real128{".25"}.m_value));
    constexpr auto z9 = real128{56} / 2;
    REQUIRE((z9.m_value == 28));
    constexpr auto z10 = 3.f / -real128{12};
    REQUIRE((z10.m_value == -real128{".25"}.m_value));
    REQUIRE(((x / int_t{3}).m_value == 4));
    REQUIRE(((int_t{3} / -x).m_value == -real128{".25"}.m_value));
    REQUIRE(((x / rat_t{3, 2}).m_value == 8));
    REQUIRE(((rat_t{3, 2} / x).m_value == real128{".125"}.m_value));
}