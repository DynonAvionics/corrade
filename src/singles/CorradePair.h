/*
    Corrade::Containers::Pair
        — a lightweight alternative to std::pair

    https://doc.magnum.graphics/corrade/classCorrade_1_1Containers_1_1Pair.html

    This is a single-header library generated from the Corrade project. With
    the goal being easy integration, it's deliberately free of all comments
    to keep the file size small. More info, detailed changelogs and docs here:

    -   Project homepage — https://magnum.graphics/corrade/
    -   Documentation — https://doc.magnum.graphics/corrade/
    -   GitHub project page — https://github.com/mosra/corrade
    -   GitHub Singles repository — https://github.com/mosra/magnum-singles

    v2020.06-1454-gfc3b7 (2023-08-27)
    -   Initial release

    Generated from Corrade {{revision}}, {{stats:loc}} / {{stats:preprocessed}} LoC
*/

#include "base.h"

/* We don't need much from configure.h here. The CORRADE_MSVC2015_COMPATIBILITY
   check is equivalent to the version check in UseCorrade.cmake. */
#pragma ACME enable Corrade_configure_h
#if defined(_MSC_VER) && _MSC_VER < 1910
#define CORRADE_MSVC2015_COMPATIBILITY
#endif
#ifdef __GNUC__
#define CORRADE_TARGET_GCC
#endif
#ifdef __clang__
#define CORRADE_TARGET_CLANG
#endif

#include "Corrade/Containers/Pair.h"
#ifdef CORRADE_PAIR_STL_COMPATIBILITY
// {{includes}}
#include "Corrade/Containers/PairStl.h"
#endif
