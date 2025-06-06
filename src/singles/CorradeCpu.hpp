/*
    Corrade::Cpu
        — compile-time and runtime CPU feature detection and dispatch

    https://doc.magnum.graphics/corrade/namespaceCorrade_1_1Cpu.html

    This is a single-header library generated from the Corrade project. With
    the goal being easy integration, it's deliberately free of all comments
    to keep the file size small. More info, detailed changelogs and docs here:

    -   Project homepage — https://magnum.graphics/corrade/
    -   Documentation — https://doc.magnum.graphics/corrade/
    -   GitHub project page — https://github.com/mosra/corrade
    -   GitHub Singles repository — https://github.com/mosra/magnum-singles

    The library has a separate non-inline implementation part, enable it *just
    once* like this:

        #define CORRADE_CPU_IMPLEMENTATION
        #include <CorradeCpu.hpp>

    If you need the deinlined symbols to be exported from a shared library,
    `#define CORRADE_UTILITY_EXPORT` as appropriate. To enable the IFUNC
    functionality, `#define CORRADE_CPU_USE_IFUNC` before including the file.

    v2020.06-1890-g77f9f (2025-04-11)
    -   Include guard for the implementation part to prevent double definitions
    v2020.06-1846-gc4cdf (2025-01-07)
    -   SFINAE is now done in template args as that's simpler for the compiler
    -   Fixed warnings on ARM with C++20
    v2020.06-1687-g6b5f (2024-06-29)
    -   FreeBSD and Emscripten compatibility fixes
    v2020.06-1454-gfc3b7 (2023-08-27)
    -   Added BMI2 detection on x86
    -   Fixed an issue on GCC 12+ and Clang, where only one of multiple
        CORRADE_ENABLE_ macro annotations would get used
    -   Fixed a potential build issue on x86 if none of the extra instruction
        sets are enabled at compile time
    -   Compatibility with C++20 which removes the <ciso646> header
    v2020.06-1040-g30cd2 (2022-09-05)
    -   Fixed a build issue on platforms that are neither x86, ARM nor WASM
    -   Renamed to CorradeCpu.hpp to imply the separate implementation part
        consistently with other header-only libraries
    v2020.06-1018-gef42a6 (2022-08-13)
    -   Properly checking XSAVE prerequisites for AVX-512
    v2020.06-1015-g8cbd6 (2022-08-02)
    -   Initial release

    Generated from Corrade {{revision}}, {{stats:loc}} / {{stats:preprocessed}} LoC
*/

#include "base.h"

/* Detect platforms based on preprocessor defines. Corrade itself does that
   through CMake and bakes that into configure.h, which we can't do here. */
/* https://stackoverflow.com/a/8249232 */
#ifdef __ANDROID__
#define CORRADE_TARGET_ANDROID
#endif
/* https://stackoverflow.com/a/41508246 */
#ifdef __EMSCRIPTEN__
#define CORRADE_TARGET_EMSCRIPTEN
#endif
/* https://stackoverflow.com/a/8249232 */
#ifdef __APPLE__
#define CORRADE_TARGET_APPLE
#endif
/* We need CORRADE_MSVC2017_COMPATIBILITY from configure.h, which is a
   #cmakedefine there so it wouldn't be pulled in below. Equivalent to the
   version check in UseCorrade.cmake. */
#if defined(_MSC_VER) && _MSC_VER < 1920
#define CORRADE_MSVC2017_COMPATIBILITY
#endif

/* Supply the configure.h template instead to avoid using baked-in defines,
   remove parts that aren't used here */
#pragma ACME disable CORRADE_TARGET_CXX14
#pragma ACME disable CORRADE_TARGET_CXX17
#pragma ACME disable CORRADE_TARGET_CXX20
#pragma ACME disable CORRADE_LONG_DOUBLE_SAME_AS_DOUBLE
#pragma ACME disable CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
/* This is to remove the static_assert() from configure.h.cmake. The macro
   shouldn't be used for anything inside Cpu.hpp, if it is, this has to be
   removed */
#pragma ACME enable CORRADE_MSVC_COMPATIBILITY
#include "Corrade/configure.h.cmake"
#pragma ACME enable Corrade_configure_h

/* Contains just Cpu::Features forward declaration, which we don't need */
#pragma ACME enable Corrade_Corrade_h

/* We need just the INLINE macros and _CORRADE_HELPER_PASTE2 from Macros.h */
#pragma ACME disable _CORRADE_HELPER_PASTE2
#pragma ACME disable CORRADE_ALWAYS_INLINE
#pragma ACME disable CORRADE_NEVER_INLINE

/* For the deinlined runtimeFeatures() implementation on ARM. Don't want the
   whole visibility header here, as it contains also Utility-specific CPU
   dispatch macros that would collide with other singles. */
#pragma ACME enable Corrade_Utility_visibility_h
#ifndef CORRADE_UTILITY_EXPORT
#define CORRADE_UTILITY_EXPORT
#endif
#include "Corrade/Cpu.h"
/* The extra guard has to be here to prevent double definitions in cases like

    #define CORRADE_CPU_IMPLEMENTATION
    #include <CorradeCpu.hpp>
    #include <CorradeString.hpp>

   where CorradeString.hpp contains `#include <CorradeCpu.hpp>` again. Note
   that even the stb_* libs don't handle this -- including them twice with the
   implementation macro defined *will* lead to double definitions. */
#if defined(CORRADE_CPU_IMPLEMENTATION) && !defined(CorradeCpu_hpp_implementation)
#define CorradeCpu_hpp_implementation
// {{includes}}
#include "Corrade/Cpu.cpp"
#endif
