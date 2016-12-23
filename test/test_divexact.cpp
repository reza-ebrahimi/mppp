/* Copyright 2009-2016 Francesco Biscani (bluescarni@gmail.com)

This file is part of the mp++ library.

The mp++ library is free software; you can redistribute it and/or modify
it under the terms of either:

  * the GNU Lesser General Public License as published by the Free
    Software Foundation; either version 3 of the License, or (at your
    option) any later version.

or

  * the GNU General Public License as published by the Free Software
    Foundation; either version 3 of the License, or (at your option) any
    later version.

or both in parallel, as here.

The mp++ library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received copies of the GNU General Public License and the
GNU Lesser General Public License along with the mp++ library.  If not,
see https://www.gnu.org/licenses/. */

#include <cstddef>
#include <gmp.h>
#include <random>
#include <tuple>
#include <type_traits>

#include <mp++.hpp>

#include "test_utils.hpp"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

static int ntries = 1000;

using namespace mppp;
using namespace mppp_test;

using sizes = std::tuple<std::integral_constant<std::size_t, 1>, std::integral_constant<std::size_t, 2>,
                         std::integral_constant<std::size_t, 3>, std::integral_constant<std::size_t, 6>,
                         std::integral_constant<std::size_t, 10>>;

static std::mt19937 rng;

struct divexact_tester {
    template <typename S>
    inline void operator()(const S &) const
    {
        using integer = mp_integer<S::value>;
        // Start with zeroes.
        mpz_raii m1, m2, m3;
        integer n1, n2, n3;
        ::mpz_set_si(&m3.m_mpz, 1);
        n3 = integer(1);
        ::mpz_divexact(&m1.m_mpz, &m2.m_mpz, &m3.m_mpz);
        divexact(n1, n2, n3);
        REQUIRE((lex_cast(n1) == lex_cast(m1)));
        REQUIRE((lex_cast(divexact(n2, n3)) == lex_cast(m1)));
        ::mpz_set_si(&m3.m_mpz, -2);
        n3 = integer(-2);
        ::mpz_divexact(&m1.m_mpz, &m2.m_mpz, &m3.m_mpz);
        divexact(n1, n2, n3);
        REQUIRE((lex_cast(n1) == lex_cast(m1)));
        REQUIRE((lex_cast(divexact(n2, n3)) == lex_cast(m1)));
        // Simple tests.
        ::mpz_set_si(&m2.m_mpz, 8);
        n2 = integer(8);
        ::mpz_set_si(&m3.m_mpz, 2);
        n3 = integer(2);
        ::mpz_divexact(&m1.m_mpz, &m2.m_mpz, &m3.m_mpz);
        divexact(n1, n2, n3);
        REQUIRE((lex_cast(n1) == lex_cast(m1)));
        REQUIRE((lex_cast(divexact(n2, n3)) == lex_cast(m1)));
        ::mpz_set_si(&m2.m_mpz, 16);
        n2 = integer(16);
        ::mpz_set_si(&m3.m_mpz, -2);
        n3 = integer(-2);
        ::mpz_divexact(&m1.m_mpz, &m2.m_mpz, &m3.m_mpz);
        divexact(n1, n2, n3);
        REQUIRE((lex_cast(n1) == lex_cast(m1)));
        REQUIRE((lex_cast(divexact(n2, n3)) == lex_cast(m1)));
        ::mpz_set_si(&m2.m_mpz, -32);
        n2 = integer(-32);
        ::mpz_set_si(&m3.m_mpz, 4);
        n3 = integer(4);
        ::mpz_divexact(&m1.m_mpz, &m2.m_mpz, &m3.m_mpz);
        divexact(n1, n2, n3);
        REQUIRE((lex_cast(n1) == lex_cast(m1)));
        REQUIRE((lex_cast(divexact(n2, n3)) == lex_cast(m1)));
        ::mpz_set_si(&m2.m_mpz, -32);
        n2 = integer(-32);
        ::mpz_set_si(&m3.m_mpz, -4);
        n3 = integer(-4);
        ::mpz_divexact(&m1.m_mpz, &m2.m_mpz, &m3.m_mpz);
        divexact(n1, n2, n3);
        REQUIRE((lex_cast(n1) == lex_cast(m1)));
        REQUIRE((lex_cast(divexact(n2, n3)) == lex_cast(m1)));
        // Random testing.
        std::uniform_int_distribution<int> sdist(0, 1), mdist(1, 3);
        mpz_raii tmp;
        auto random_xy = [&](unsigned x) {
            for (int i = 0; i < ntries; ++i) {
                if (sdist(rng) && sdist(rng) && sdist(rng)) {
                    // Reset rop every once in a while.
                    n1 = integer{};
                }
                random_integer(tmp, x, rng);
                ::mpz_set(&m3.m_mpz, &tmp.m_mpz);
                n3 = integer(mpz_to_str(&tmp.m_mpz));
                if (n3.sign() == 0) {
                    continue;
                }
                if (sdist(rng)) {
                    ::mpz_neg(&m3.m_mpz, &m3.m_mpz);
                    n3.neg();
                }
                if (n3.is_static() && sdist(rng)) {
                    // Promote sometimes, if possible.
                    n3.promote();
                }
                n2 = n3;
                ::mpz_set(&m2.m_mpz, &m3.m_mpz);
                const auto mult = mdist(rng);
                mul(n2, n2, integer(mult));
                ::mpz_mul_si(&m2.m_mpz, &m2.m_mpz, mult);
                if (sdist(rng)) {
                    ::mpz_neg(&m2.m_mpz, &m2.m_mpz);
                    n2.neg();
                }
                if (n2.is_static() && sdist(rng)) {
                    // Promote sometimes, if possible.
                    n2.promote();
                }
                divexact(n1, n2, n3);
                ::mpz_divexact(&m1.m_mpz, &m2.m_mpz, &m3.m_mpz);
                REQUIRE((lex_cast(n1) == lex_cast(m1)));
                REQUIRE((lex_cast(divexact(n2, n3)) == lex_cast(m1)));
            }
        };

        random_xy(0);
        random_xy(1);
        random_xy(2);
        random_xy(3);
        random_xy(4);
    }
};

TEST_CASE("divexact")
{
    tuple_for_each(sizes{}, divexact_tester{});
}