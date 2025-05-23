#ifndef Corrade_Containers_StringStlView_h
#define Corrade_Containers_StringStlView_h
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
@brief STL @ref std::string_view compatibility for @ref Corrade::Containers::String and @ref Corrade::Containers::StringView
@m_since_latest

Including this header allows you to convert a
@ref Corrade::Containers::String / @ref Corrade::Containers::StringView from
and to a C++17 @ref std::string_view, and to have sequences of
@ref std::string_view convertible to a
@ref Corrade::Containers::StringIterable. A separate
@ref Corrade/Containers/StringStl.h header provides compatibility with
@ref std::string, @ref Corrade/Containers/StringStlHash.h then provides a
@ref std::hash specialization. See
@ref Containers-String-stl "String STL compatibility",
@ref Containers-BasicStringView-stl "StringView STL compatibility" and
@ref Containers-StringIterable-stl "StringIterable STL compatibility" for more
information.
*/

/* Unlike with Utility/StlForwardString.h, even libstdc++ 11 and libc++ 13 have
   a full definition of std::string_view including default parameters in the
   <string_view> header, so we can't forward-declare it. Didn't check with
   MSVC, but I assume a similar case. */
#include <string_view>

#include "Corrade/Containers/String.h"
#include "Corrade/Containers/StringView.h"

/* Listing these namespaces doesn't add anything to the docs, so don't */
#ifndef DOXYGEN_GENERATING_OUTPUT
namespace Corrade { namespace Containers { namespace Implementation {

/* Since the insanely huge header is included already anyway, it's not that
   much extra pain to just have the implementations fully inline as well.
   Better than having to compile a dedicated C++17 file just for this, bloat
   our binaries and have their symbol tables polluted by tons of STL stuff. */

template<> struct StringConverter<std::string_view> {
    static String from(std::string_view other) {
        return String{other.data(), other.size()};
    }
    static std::string_view to(const String& other) {
        return std::string_view{other.data(), other.size()};
    }
};

template<> struct StringViewConverter<const char, std::string_view> {
    static StringView from(std::string_view other) {
        return StringView{other.data(), other.size()};
    }
    static std::string_view to(StringView other) {
        return std::string_view{other.data(), other.size()};
    }
};

/* There's no mutable variant of std::string_view, so this goes just one
   direction */
template<> struct StringViewConverter<char, std::string_view> {
    static std::string_view to(MutableStringView other) {
        return std::string_view{other.data(), other.size()};
    }
};

#ifndef CORRADE_SINGLES_NO_ADVANCED_STRING_APIS
template<class> struct StringIterableConverter;
template<> struct StringIterableConverter<std::string_view> {
    static StringView accessor(const void* data, const void*, std::ptrdiff_t, std::size_t) {
        return *static_cast<const std::string_view*>(data);
    }
};
#endif

}}}
#endif

#endif
