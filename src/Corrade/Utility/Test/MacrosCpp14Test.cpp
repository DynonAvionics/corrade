/*
    This file is part of Corrade.

    Copyright © 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016,
                2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include "Corrade/TestSuite/Tester.h"
#include "Corrade/Utility/Macros.h"

namespace Corrade { namespace Utility { namespace Test { namespace {

struct MacrosCpp14Test: TestSuite::Tester {
    explicit MacrosCpp14Test();

    void constexpr14();
    void constexpr20();
};

MacrosCpp14Test::MacrosCpp14Test() {
    addTests({&MacrosCpp14Test::constexpr14,
              &MacrosCpp14Test::constexpr20});
}

CORRADE_CONSTEXPR14 int sumInAStupidWay(int number) {
    int sum = 0;
    for(int i = 0; i != number; ++i)
        sum += i;
    return sum;
}

struct ConstexprNoInit {
    CORRADE_CONSTEXPR20 explicit ConstexprNoInit(NoInitT) {}

    int a;
};

CORRADE_CONSTEXPR20 ConstexprNoInit constexprNoInit(int a) {
    ConstexprNoInit s{NoInit};
    s.a = a;
    return s;
}

void MacrosCpp14Test::constexpr14() {
    /* This should pass always */
    CORRADE_CONSTEXPR14 int sum = sumInAStupidWay(17);
    CORRADE_COMPARE(sum, 136);

    #ifdef CORRADE_MSVC2015_COMPATIBILITY
    CORRADE_SKIP("CORRADE_CONSTEXPR14 not available on MSVC2015.");
    #else
    constexpr int csum = sumInAStupidWay(17);
    CORRADE_COMPARE(csum, 136);
    #endif
}

void MacrosCpp14Test::constexpr20() {
    /* Compared to MacrosCpp20Test::constexpr20(), here the macro should
       compile to nothing, making it behave like a regular function */
    CORRADE_CONSTEXPR20 ConstexprNoInit a = constexprNoInit(42);
    CORRADE_COMPARE(a.a, 42);
}

}}}}

CORRADE_TEST_MAIN(Corrade::Utility::Test::MacrosCpp14Test)
