#ifndef Corrade_Utility_DebugAssert_h
#define Corrade_Utility_DebugAssert_h
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

/** @file
@brief Macro @ref CORRADE_DEBUG_ASSERT(), @ref CORRADE_CONSTEXPR_DEBUG_ASSERT(), @ref CORRADE_DEBUG_ASSERT_OUTPUT(), @ref CORRADE_DEBUG_ASSERT_UNREACHABLE(), @ref CORRADE_INTERNAL_DEBUG_ASSERT(), @ref CORRADE_INTERNAL_CONSTEXPR_DEBUG_ASSERT(), @ref CORRADE_INTERNAL_DEBUG_ASSERT_OUTPUT(), @ref CORRADE_INTERNAL_DEBUG_ASSERT_EXPRESSION(), @ref CORRADE_INTERNAL_DEBUG_ASSERT_UNREACHABLE()
@m_since_latest

Variants of asserts from @ref Corrade/Utility/Assert.h that get checked only in
debug builds. They live in a separate header to avoid release builds
unnecessarily pulling in debug output and other dependencies of regular
asserts.
*/

#include "Corrade/configure.h"
/* The *_ASSERT_UNREACHABLE() assertions use CORRADE_ASSERT_ABORT() directly as
   a fallback on certain platforms so if they're not overriden and a custom CORRADE_ASSERT_ABORT() isn't defined either, include the std::abort()
   definition. Done before including Assert.h which has CORRADE_ASSERT_ABORT()
   defined itself. */
#if (defined(CORRADE_NO_ASSERT) || (!defined(CORRADE_IS_DEBUG_BUILD) && defined(NDEBUG))) && (!defined(CORRADE_DEBUG_ASSERT_UNREACHABLE) || !defined(CORRADE_INTERNAL_DEBUG_ASSERT_UNREACHABLE)) && !defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_MSVC) && !defined(CORRADE_ASSERT_ABORT)
#include <cstdlib>
#endif
#if !defined(CORRADE_NO_ASSERT) && (defined(CORRADE_IS_DEBUG_BUILD) || !defined(NDEBUG)) && (!defined(CORRADE_DEBUG_ASSERT) || !defined(CORRADE_CONSTEXPR_DEBUG_ASSERT) || !defined(CORRADE_DEBUG_ASSERT_OUTPUT) || !defined(CORRADE_DEBUG_ASSERT_UNREACHABLE) || !defined(CORRADE_INTERNAL_DEBUG_ASSERT) || !defined(CORRADE_INTERNAL_CONSTEXPR_DEBUG_ASSERT) || !defined(CORRADE_INTERNAL_DEBUG_ASSERT_OUTPUT) || !defined(CORRADE_INTERNAL_DEBUG_ASSERT_UNREACHABLE))
#include "Corrade/Utility/Assert.h"
#endif

/* There's deliberately no namespace Corrade::Utility in order to avoid noise
   in Corrade Singles */

/**
@brief Whether debug assertions are disabled
@m_since_latest

Defined either if @ref CORRADE_NO_ASSERT is defined or if
@ref CORRADE_IS_DEBUG_BUILD is not defined and @cpp NDEBUG @ce is defined. When
defined, debug assertions are not checked at all. See documentation of
@ref CORRADE_DEBUG_ASSERT(), @ref CORRADE_CONSTEXPR_DEBUG_ASSERT(),
@ref CORRADE_DEBUG_ASSERT_OUTPUT(), @ref CORRADE_DEBUG_ASSERT_UNREACHABLE(),
@ref CORRADE_INTERNAL_DEBUG_ASSERT(),
@ref CORRADE_INTERNAL_CONSTEXPR_DEBUG_ASSERT(),
@ref CORRADE_INTERNAL_DEBUG_ASSERT_OUTPUT(),
@ref CORRADE_INTERNAL_DEBUG_ASSERT_EXPRESSION()
and @ref CORRADE_INTERNAL_DEBUG_ASSERT_UNREACHABLE() for detailed description
of given macro behavior.

@attention Compared to @ref CORRADE_NO_ASSERT, this macro is *not* meant to be
    defined by the user. Instead, control its behavior by (un)defining the
    macros it relies on.

@see @ref CORRADE_SKIP_IF_NO_DEBUG_ASSERT()
*/
#if defined(CORRADE_NO_ASSERT) || (!defined(CORRADE_IS_DEBUG_BUILD) && defined(NDEBUG)) || defined(DOXYGEN_GENERATING_OUTPUT)
#define CORRADE_NO_DEBUG_ASSERT
#ifdef DOXYGEN_GENERATING_OUTPUT
#undef CORRADE_NO_DEBUG_ASSERT
#endif
#endif

/** @hideinitializer
@brief Debug assertion macro
@m_since_latest

Expands to @ref CORRADE_ASSERT() if @ref CORRADE_NO_DEBUG_ASSERT is not
defined. Otherwise expands to @cpp do {} while(false) @ce.

You can override this implementation by placing your own
@cpp #define CORRADE_DEBUG_ASSERT @ce before including the
@ref Corrade/Utility/DebugAssert.h header.
*/
#ifndef CORRADE_DEBUG_ASSERT
#ifndef CORRADE_NO_DEBUG_ASSERT
#define CORRADE_DEBUG_ASSERT(condition, message, returnValue)               \
    CORRADE_ASSERT(condition, message, returnValue)
#else
#define CORRADE_DEBUG_ASSERT(condition, message, returnValue) do {} while(false)
#endif
#endif

/** @hideinitializer
@brief Constexpr debug assertion macro
@m_since_latest

Expands to @ref CORRADE_CONSTEXPR_ASSERT() if @ref CORRADE_NO_DEBUG_ASSERT is
not defined. Otherwise expands to @cpp static_cast<void>(0) @ce.

You can override this implementation by placing your own
@cpp #define CORRADE_CONSTEXPR_DEBUG_ASSERT @ce before including the
@ref Corrade/Utility/DebugAssert.h header.
*/
#ifndef CORRADE_CONSTEXPR_DEBUG_ASSERT
#ifndef CORRADE_NO_DEBUG_ASSERT
#define CORRADE_CONSTEXPR_DEBUG_ASSERT(condition, message)                  \
    CORRADE_CONSTEXPR_ASSERT(condition, message)
#else
#define CORRADE_CONSTEXPR_DEBUG_ASSERT(condition, message) static_cast<void>(0)
#endif
#endif

/** @hideinitializer
@brief Call output debug assertion macro
@m_since_latest

Expands to @ref CORRADE_ASSERT_OUTPUT() if @ref CORRADE_NO_DEBUG_ASSERT is not
defined. Otherwise expands to @cpp static_cast<void>(call) @ce.

You can override this implementation by placing your own
@cpp #define CORRADE_DEBUG_ASSERT_OUTPUT @ce before including the
@ref Corrade/Utility/DebugAssert.h header.
*/
#ifndef CORRADE_DEBUG_ASSERT_OUTPUT
#ifndef CORRADE_NO_DEBUG_ASSERT
#define CORRADE_DEBUG_ASSERT_OUTPUT(call, message, returnValue)             \
    CORRADE_ASSERT_OUTPUT(call, message, returnValue)
#else
#define CORRADE_DEBUG_ASSERT_OUTPUT(call, message, returnValue)             \
    static_cast<void>(call)
#endif
#endif

/** @hideinitializer
@brief Debug assert that the code is unreachable
@m_since_latest

Expands to @ref CORRADE_ASSERT_UNREACHABLE() if @ref CORRADE_NO_DEBUG_ASSERT is
not defined. Otherwise expands to a compiler builtin on GCC, Clang and
MSVC; calls @ref CORRADE_ASSERT_ABORT() (which is @ref std::abort() by default)
otherwise. A @cpp return @ce statement can thus be safely omitted in a code
path following this macro even in a release build without causing any compiler
warnings or errors.

You can override this implementation by placing your own
@cpp #define CORRADE_DEBUG_ASSERT_UNREACHABLE @ce before including the
@ref Corrade/Utility/DebugAssert.h header.
*/
#ifndef CORRADE_DEBUG_ASSERT_UNREACHABLE
#ifndef CORRADE_NO_DEBUG_ASSERT
#define CORRADE_DEBUG_ASSERT_UNREACHABLE(message, returnValue)              \
    CORRADE_ASSERT_UNREACHABLE(message, returnValue)
#else
#if defined(CORRADE_TARGET_GCC)
#define CORRADE_DEBUG_ASSERT_UNREACHABLE(message, returnValue) __builtin_unreachable()
#elif defined(CORRADE_TARGET_MSVC)
#define CORRADE_DEBUG_ASSERT_UNREACHABLE(message, returnValue) __assume(0)
#else
#define CORRADE_DEBUG_ASSERT_UNREACHABLE(message, returnValue) CORRADE_ASSERT_ABORT()
#endif
#endif
#endif

/** @hideinitializer
@brief Internal debug assertion macro
@m_since_latest

Expands to @ref CORRADE_INTERNAL_ASSERT() if @ref CORRADE_NO_DEBUG_ASSERT is
not defined. Otherwise expands to @cpp do {} while(false) @ce.

You can override this implementation by placing your own
@cpp #define CORRADE_INTERNAL_DEBUG_ASSERT @ce before including the
@ref Corrade/Utility/DebugAssert.h header.
*/
#ifndef CORRADE_INTERNAL_DEBUG_ASSERT
#ifndef CORRADE_NO_DEBUG_ASSERT
#define CORRADE_INTERNAL_DEBUG_ASSERT(condition)                            \
    CORRADE_INTERNAL_ASSERT(condition)
#else
#define CORRADE_INTERNAL_DEBUG_ASSERT(condition) do {} while(false)
#endif
#endif

/** @hideinitializer
@brief Internal constexpr debug assertion macro
@m_since_latest

Expands to @ref CORRADE_INTERNAL_CONSTEXPR_ASSERT() if
@ref CORRADE_NO_DEBUG_ASSERT is not defined. Otherwise expands to
@cpp static_cast<void>(0) @ce.

You can override this implementation by placing your own
@cpp #define CORRADE_INTERNAL_CONSTEXPR_DEBUG_ASSERT @ce before including the
@ref Corrade/Utility/DebugAssert.h header.
*/
#ifndef CORRADE_INTERNAL_CONSTEXPR_DEBUG_ASSERT
#ifndef CORRADE_NO_DEBUG_ASSERT
#define CORRADE_INTERNAL_CONSTEXPR_DEBUG_ASSERT(condition)                  \
    CORRADE_INTERNAL_CONSTEXPR_ASSERT(condition)
#else
#define CORRADE_INTERNAL_CONSTEXPR_DEBUG_ASSERT(condition) static_cast<void>(0)
#endif
#endif

/** @hideinitializer
@brief Internal call output debug assertion macro
@m_since_latest

Expands to @ref CORRADE_INTERNAL_ASSERT_OUTPUT() if
@ref CORRADE_NO_DEBUG_ASSERT is not defined. Otherwise expands to
@cpp static_cast<void>(call) @ce.

You can override this implementation by placing your own
@cpp #define CORRADE_INTERNAL_DEBUG_ASSERT_OUTPUT @ce before including the
@ref Corrade/Utility/DebugAssert.h header.
*/
#ifndef CORRADE_INTERNAL_DEBUG_ASSERT_OUTPUT
#ifndef CORRADE_NO_DEBUG_ASSERT
#define CORRADE_INTERNAL_DEBUG_ASSERT_OUTPUT(call)                          \
    CORRADE_INTERNAL_ASSERT_OUTPUT(call)
#else
#define CORRADE_INTERNAL_DEBUG_ASSERT_OUTPUT(call) static_cast<void>(call)
#endif
#endif

/** @hideinitializer
@brief Internal expression debug assertion macro
@m_since_latest

Expands to @ref CORRADE_INTERNAL_ASSERT_EXPRESSION() if
@ref CORRADE_NO_DEBUG_ASSERT is not defined. Otherwise expands to nothing,
leaving just the parenthesized expression out of it.

You can override this implementation by placing your own
@cpp #define CORRADE_INTERNAL_DEBUG_ASSERT_EXPRESSION @ce before including the
@ref Corrade/Utility/DebugAssert.h header.
*/
#ifndef CORRADE_INTERNAL_DEBUG_ASSERT_EXPRESSION
#ifndef CORRADE_NO_DEBUG_ASSERT
#define CORRADE_INTERNAL_DEBUG_ASSERT_EXPRESSION(...)                       \
    CORRADE_INTERNAL_ASSERT_EXPRESSION(__VA_ARGS__)
#else
/* Not (...) __VA_ARGS__, because the parentheses may be important */
#define CORRADE_INTERNAL_DEBUG_ASSERT_EXPRESSION
#endif
#endif

/** @hideinitializer
@brief Internal debug assert that the code is unreachable
@m_since_latest

Expands to @ref CORRADE_INTERNAL_ASSERT_UNREACHABLE() if
@ref CORRADE_NO_DEBUG_ASSERT is not defined. Otherwise expands to a compiler
builtin on GCC, Clang and MSVC; calls @ref CORRADE_ASSERT_ABORT() (which is
@ref std::abort() by default) otherwise. A @cpp return @ce statement can thus
be safely omitted in a code path following this macro even in a release build
without causing any compiler warnings or errors.

You can override this implementation by placing your own
@cpp #define CORRADE_INTERNAL_DEBUG_ASSERT_UNREACHABLE @ce before including the
@ref Corrade/Utility/DebugAssert.h header.
*/
#ifndef CORRADE_INTERNAL_DEBUG_ASSERT_UNREACHABLE
#ifndef CORRADE_NO_DEBUG_ASSERT
#define CORRADE_INTERNAL_DEBUG_ASSERT_UNREACHABLE()                         \
    CORRADE_INTERNAL_ASSERT_UNREACHABLE()
#else
#if defined(CORRADE_TARGET_GCC)
#define CORRADE_INTERNAL_DEBUG_ASSERT_UNREACHABLE() __builtin_unreachable()
#elif defined(CORRADE_TARGET_MSVC)
#define CORRADE_INTERNAL_DEBUG_ASSERT_UNREACHABLE() __assume(0)
#else
#define CORRADE_INTERNAL_DEBUG_ASSERT_UNREACHABLE() CORRADE_ASSERT_ABORT()
#endif
#endif
#endif

#endif
