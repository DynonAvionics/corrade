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

#include "System.h"

#ifdef CORRADE_TARGET_APPLE
#include <cstdlib> /* std::getenv() */
#endif

#ifndef CORRADE_TARGET_WINDOWS
#include "unistd.h"
#else
#include <windows.h>
#endif

namespace Corrade { namespace Utility { namespace System {

bool isSandboxed() {
    #if defined(CORRADE_TARGET_IOS) || defined(CORRADE_TARGET_ANDROID) || defined(CORRADE_TARGET_EMSCRIPTEN) || defined(CORRADE_TARGET_WINDOWS_RT)
    return true;
    #elif defined(CORRADE_TARGET_APPLE)
    return std::getenv("APP_SANDBOX_CONTAINER_ID");
    #else
    return false;
    #endif
}

void sleep(const std::size_t ms) {
    #ifndef CORRADE_TARGET_WINDOWS
    usleep(ms*1000);
    #else
    Sleep(ms);
    #endif
}

}}}
