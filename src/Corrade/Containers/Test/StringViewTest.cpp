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

/* Deliberately including first to make sure it works without ArrayView being
   included first */
#include "Corrade/Containers/StringView.h"

#include "Corrade/Cpu.h"
#include "Corrade/Containers/Array.h"
#include "Corrade/Containers/StaticArray.h"
#include "Corrade/Containers/Test/StringViewTest.h"
#include "Corrade/TestSuite/Tester.h"
#include "Corrade/TestSuite/Compare/Container.h"
#include "Corrade/TestSuite/Compare/Numeric.h"
#include "Corrade/Utility/Algorithms.h"
#include "Corrade/Utility/Format.h"
#include "Corrade/Utility/Memory.h"
#include "Corrade/Utility/Test/cpuVariantHelpers.h"

namespace {

struct StrView {
    StrView(char* data, std::size_t size): data{data}, size{size} {}

    char* data;
    std::size_t size;
};

struct ConstStrView {
    constexpr ConstStrView(const char* data, std::size_t size): data{data}, size{size} {}

    const char* data;
    std::size_t size;
};

}

namespace Corrade { namespace Containers {

namespace Implementation {

template<> struct StringViewConverter<char, StrView> {
    static MutableStringView from(StrView other) {
        return MutableStringView{other.data, other.size};
    }

    static StrView to(MutableStringView other) {
        return {other.data(), other.size()};
    }
};

template<> struct StringViewConverter<const char, ConstStrView> {
    constexpr static StringView from(ConstStrView other) {
        return StringView{other.data, other.size};
    }

    constexpr static ConstStrView to(StringView other) {
        return {other.data(), other.size()};
    }
};

/* To keep the StringView API in reasonable bounds, the const-adding variants
   have to be implemented explicitly */
template<> struct StringViewConverter<const char, StrView> {
    static StringView from(StrView other) {
        return StringView{other.data, other.size};
    }
};
template<> struct StringViewConverter<char, ConstStrView> {
    constexpr static ConstStrView to(MutableStringView other) {
        return {other.data(), other.size()};
    }
};

}

namespace Test { namespace {

struct StringViewTest: TestSuite::Tester {
    explicit StringViewTest();

    void captureImplementations();
    void restoreImplementations();

    template<class T> void constructDefault();
    void constructDefaultConstexpr();
    template<class T> void construct();
    void constructConstexpr();
    template<class T, class From> void constructCharPointer();
    template<class T, class From> void constructCharArray();
    void constructCharPointerArrayDisallowed();
    void constructPointerNull();
    void constructPointerFlags();
    void constructEmpty();
    void constructNullptrSize();
    void constructFromMutable();
    void constructCopy();
    void constructLiteral();
    void constructLiteralEmpty();
    #ifdef CORRADE_TARGET_32BIT
    void constructTooLarge();
    #endif
    void constructNullptrNullTerminated();

    void constructZeroNullPointerAmbiguity();

    template<class T> void convertArrayView();
    void convertArrayViewConstexpr();
    template<class T> void convertVoidArrayView();
    void convertConstFromArrayView();
    void convertToConstArrayView();
    void convertConstFromArray();
    void convertExternalView();
    void convertConstFromExternalView();
    void convertToConstExternalView();

    void compareEquality();
    void compareNonEquality();

    /* operator+() tested in StringTest */
    /* operator*() tested in StringTest */

    void access();
    void accessMutable();
    void accessInvalid();

    void sliceInvalid();
    void sliceNullptr();
    void slice();
    void slicePointer();
    void sliceFlags();
    void sliceZeroNullPointerAmbiguity();

    void splitCharacter();
    void splitString();
    void splitStringEmpty();
    void splitFlags();
    void splitOnAny();
    void splitOnAnyFlags();
    void splitOnWhitespace();
    void splitNullView();

    void partitionCharacter();
    void partitionString();
    void partitionFlagsCharacter();
    void partitionFlagsString();
    void partitionNullViewCharacter();
    void partitionNullViewString();

    /* join() tested in StringTest */

    void hasPrefix();
    void hasPrefixEmpty();
    void hasSuffix();
    void hasSuffixEmpty();

    void exceptPrefix();
    void exceptPrefixFlags();
    void exceptPrefixInvalid();
    void exceptPrefixDisabledOverloads();
    void exceptSuffix();
    void exceptSuffixFlags();
    void exceptSuffixInvalid();
    void exceptSuffixDisabledOverloads();

    void trimmed();
    void trimmedFlags();
    void trimmedNullView();

    /* Tests also contains() */
    void findString();
    void findStringMultipleOccurences();
    void findStringWhole();
    void findCharacter();
    void findCharacterAligned();
    void findCharacterUnaligned();
    void findCharacterUnalignedLessThanTwoVectors();
    void findCharacterUnalignedLessThanOneVector();
    void findEmpty();
    void findFlags();
    void findOr();

    void findLastString();
    void findLastStringMultipleOccurences();
    void findLastStringWhole();
    void findLastCharacter();
    void findLastEmpty();
    void findLastFlags();
    void findLastOr();

    void findAny();
    void findAnyEmpty();
    void findAnyFlags();
    void findAnyOr();

    void findLastAny();
    void findLastAnyEmpty();
    void findLastAnyFlags();
    void findLastAnyOr();

    void countCharacter();
    void countCharacterAligned();
    void countCharacterUnaligned();
    void countCharacterUnalignedLessThanTwoVectors();
    void countCharacterUnalignedLessThanOneVector();

    void debugFlag();
    void debugFlags();
    void debug();

    private:
        #ifdef CORRADE_UTILITY_FORCE_CPU_POINTER_DISPATCH
        decltype(Implementation::stringFindCharacter) _findCharacterImplementation;
        decltype(Implementation::stringCountCharacter) _countCharacterImplementation;
        #endif
};

const struct {
    Cpu::Features features;
    std::size_t vectorSize;
    const char* extra;
    /* Cases that define a function pointer are not present in the library, see
       the pointed-to function documentation for more info */
    const char*(*function)(const char*, std::size_t, char);
} FindCharacterData[]{
    {Cpu::Scalar, 16, nullptr, nullptr},
    #if defined(CORRADE_ENABLE_SSE2) && defined(CORRADE_ENABLE_BMI1)
    {Cpu::Sse2|Cpu::Bmi1, 16, "branch on movemask (default)", nullptr},
    #ifdef CORRADE_UTILITY_FORCE_CPU_POINTER_DISPATCH
    {Cpu::Sse41|Cpu::Bmi1, 16, "branch on testzero",
        stringFindCharacterImplementationSse41TestZero},
    #endif
    #endif
    #if defined(CORRADE_ENABLE_AVX2) && defined(CORRADE_ENABLE_BMI1)
    {Cpu::Avx2|Cpu::Bmi1, 32, nullptr, nullptr},
    #endif
    /* The code uses ARM64 NEON instructions. 32-bit ARM isn't that important
       nowadays, so there it uses scalar code */
    #if defined(CORRADE_ENABLE_NEON) && !defined(CORRADE_TARGET_32BIT)
    {Cpu::Neon, 16, nullptr, nullptr},
    #endif
    #ifdef CORRADE_ENABLE_SIMD128
    {Cpu::Simd128, 16, nullptr, nullptr},
    #endif
};

const struct {
    Cpu::Features features;
    std::size_t vectorSize;
    const char* extra;
    /* Cases that define a function pointer are not present in the library, see
       the pointed-to function documentation for more info */
    std::size_t(*function)(const char*, std::size_t, char);
} CountCharacterData[]{
    {Cpu::Scalar, 16, nullptr, nullptr},
    #if defined(CORRADE_ENABLE_SSE2) && defined(CORRADE_ENABLE_POPCNT)
    #ifdef CORRADE_UTILITY_FORCE_CPU_POINTER_DISPATCH
    {Cpu::Sse2|Cpu::Popcnt, 16, "16bit popcnt",
        stringCountCharacterImplementationSse2Popcnt16},
    {Cpu::Sse2|Cpu::Popcnt, 16, "32bit popcnt + 32bit postamble",
        stringCountCharacterImplementationSse2Popcnt32},
    #endif
    /* The 64-bit variants of POPCNT instructions aren't exposed on 32-bit
       systems for some reason, skipping there. */
    #ifndef CORRADE_TARGET_32BIT
    #ifdef CORRADE_UTILITY_FORCE_CPU_POINTER_DISPATCH
    {Cpu::Sse2|Cpu::Popcnt, 16, "64bit popcnt + 16bit postamble",
        stringCountCharacterImplementationSse2PostamblePopcnt16},
    #endif
    {Cpu::Sse2|Cpu::Popcnt, 16, "64bit popcnt + 32bit postamble (default)",
        nullptr},
    #ifdef CORRADE_UTILITY_FORCE_CPU_POINTER_DISPATCH
    {Cpu::Sse2|Cpu::Popcnt, 16, "64bit popcnt + 64bit postamble, if cascade",
        stringCountCharacterImplementationSse2PostamblePopcnt64},
    {Cpu::Sse2|Cpu::Popcnt, 16, "64bit popcnt + 64bit postamble, switch",
        stringCountCharacterImplementationSse2PostamblePopcnt64Switch},
    {Cpu::Sse2|Cpu::Popcnt, 16, "64bit popcnt + 64bit postamble, loop",
        stringCountCharacterImplementationSse2PostamblePopcnt64Loop},
    #endif
    #endif
    #endif
    #if defined(CORRADE_ENABLE_AVX2) && defined(CORRADE_ENABLE_POPCNT) && !defined(CORRADE_TARGET_32BIT)
    #ifdef CORRADE_UTILITY_FORCE_CPU_POINTER_DISPATCH
    {Cpu::Avx2|Cpu::Popcnt, 32, "32bit popcnt",
        stringCountCharacterImplementationAvx2Popcnt32},
    #endif
    {Cpu::Avx2|Cpu::Popcnt, 32, "64bit popcnt (default)", nullptr},
    #endif
    #ifdef CORRADE_ENABLE_SIMD128
    {Cpu::Simd128, 16, nullptr, nullptr},
    #endif
};

StringViewTest::StringViewTest() {
    addTests({&StringViewTest::constructDefault<const char>,
              &StringViewTest::constructDefault<char>,
              &StringViewTest::constructDefaultConstexpr,
              &StringViewTest::construct<const char>,
              &StringViewTest::construct<char>,
              &StringViewTest::constructConstexpr,
              &StringViewTest::constructCharPointer<const char, const char>,
              &StringViewTest::constructCharPointer<const char, char>,
              &StringViewTest::constructCharPointer<char, char>,
              &StringViewTest::constructCharArray<const char, const char>,
              &StringViewTest::constructCharArray<const char, char>,
              &StringViewTest::constructCharArray<char, char>,
              &StringViewTest::constructCharPointerArrayDisallowed,
              &StringViewTest::constructPointerNull,
              &StringViewTest::constructPointerFlags,
              &StringViewTest::constructEmpty,
              &StringViewTest::constructNullptrSize,
              &StringViewTest::constructFromMutable,
              &StringViewTest::constructCopy,
              &StringViewTest::constructLiteral,
              &StringViewTest::constructLiteralEmpty,
              #ifdef CORRADE_TARGET_32BIT
              &StringViewTest::constructTooLarge,
              #endif
              &StringViewTest::constructNullptrNullTerminated,

              &StringViewTest::constructZeroNullPointerAmbiguity,

              &StringViewTest::convertArrayView<const char>,
              &StringViewTest::convertArrayView<char>,
              &StringViewTest::convertArrayViewConstexpr,
              &StringViewTest::convertConstFromArrayView,
              &StringViewTest::convertToConstArrayView,
              &StringViewTest::convertConstFromArray,
              &StringViewTest::convertVoidArrayView<const char>,
              &StringViewTest::convertVoidArrayView<char>,
              &StringViewTest::convertExternalView,
              &StringViewTest::convertConstFromExternalView,
              &StringViewTest::convertToConstExternalView,

              &StringViewTest::compareEquality,
              &StringViewTest::compareNonEquality,

              &StringViewTest::access,
              &StringViewTest::accessMutable,
              &StringViewTest::accessInvalid,

              &StringViewTest::sliceInvalid,
              &StringViewTest::sliceNullptr,
              &StringViewTest::slice,
              &StringViewTest::slicePointer,
              &StringViewTest::sliceFlags,
              &StringViewTest::sliceZeroNullPointerAmbiguity,

              &StringViewTest::splitCharacter,
              &StringViewTest::splitString,
              &StringViewTest::splitStringEmpty,
              &StringViewTest::splitFlags,
              &StringViewTest::splitOnAny,
              &StringViewTest::splitOnAnyFlags,
              &StringViewTest::splitOnWhitespace,
              &StringViewTest::splitNullView,

              &StringViewTest::partitionCharacter,
              &StringViewTest::partitionString,
              &StringViewTest::partitionFlagsCharacter,
              &StringViewTest::partitionFlagsString,
              &StringViewTest::partitionNullViewCharacter,
              &StringViewTest::partitionNullViewString,

              &StringViewTest::hasPrefix,
              &StringViewTest::hasPrefixEmpty,
              &StringViewTest::hasSuffix,
              &StringViewTest::hasSuffixEmpty,

              &StringViewTest::exceptPrefix,
              &StringViewTest::exceptPrefixFlags,
              &StringViewTest::exceptPrefixInvalid,
              &StringViewTest::exceptPrefixDisabledOverloads,
              &StringViewTest::exceptSuffix,
              &StringViewTest::exceptSuffixFlags,
              &StringViewTest::exceptSuffixInvalid,
              &StringViewTest::exceptSuffixDisabledOverloads,

              &StringViewTest::trimmed,
              &StringViewTest::trimmedFlags,
              &StringViewTest::trimmedNullView,

              &StringViewTest::findString,
              &StringViewTest::findStringMultipleOccurences,
              &StringViewTest::findStringWhole});

    addInstancedTests({&StringViewTest::findCharacter,
                       &StringViewTest::findCharacterAligned,
                       &StringViewTest::findCharacterUnaligned,
                       &StringViewTest::findCharacterUnalignedLessThanTwoVectors,
                       &StringViewTest::findCharacterUnalignedLessThanOneVector},
        Utility::Test::cpuVariantCount(FindCharacterData),
        &StringViewTest::captureImplementations,
        &StringViewTest::restoreImplementations);

    addTests({&StringViewTest::findEmpty,
              &StringViewTest::findFlags,
              &StringViewTest::findOr,

              &StringViewTest::findLastString,
              &StringViewTest::findLastStringMultipleOccurences,
              &StringViewTest::findLastStringWhole,
              &StringViewTest::findLastCharacter,
              &StringViewTest::findLastEmpty,
              &StringViewTest::findLastFlags,
              &StringViewTest::findLastOr,

              &StringViewTest::findAny,
              &StringViewTest::findAnyEmpty,
              &StringViewTest::findAnyFlags,
              &StringViewTest::findAnyOr,

              &StringViewTest::findLastAny,
              &StringViewTest::findLastAnyEmpty,
              &StringViewTest::findLastAnyFlags,
              &StringViewTest::findLastAnyOr});

    addInstancedTests({&StringViewTest::countCharacter,
                       &StringViewTest::countCharacterAligned,
                       &StringViewTest::countCharacterUnaligned,
                       &StringViewTest::countCharacterUnalignedLessThanTwoVectors,
                       &StringViewTest::countCharacterUnalignedLessThanOneVector},
        Utility::Test::cpuVariantCount(CountCharacterData),
        &StringViewTest::captureImplementations,
        &StringViewTest::restoreImplementations);

    addTests({&StringViewTest::debugFlag,
              &StringViewTest::debugFlags,
              &StringViewTest::debug});
}

using namespace Literals;

template<class> struct NameFor;
template<> struct NameFor<const char> {
    static const char* name() { return "StringView"; }
};
template<> struct NameFor<char> {
    static const char* name() { return "MutableStringView"; }
};

void StringViewTest::captureImplementations() {
    #ifdef CORRADE_UTILITY_FORCE_CPU_POINTER_DISPATCH
    _findCharacterImplementation = Implementation::stringFindCharacter;
    _countCharacterImplementation = Implementation::stringCountCharacter;
    #endif
}

void StringViewTest::restoreImplementations() {
    #ifdef CORRADE_UTILITY_FORCE_CPU_POINTER_DISPATCH
    Implementation::stringFindCharacter = _findCharacterImplementation;
    Implementation::stringCountCharacter = _countCharacterImplementation;
    #endif
}

template<class T> void StringViewTest::constructDefault() {
    setTestCaseTemplateName(NameFor<T>::name());

    const BasicStringView<T> a;
    const BasicStringView<T> b = nullptr;
    CORRADE_VERIFY(!a);
    CORRADE_VERIFY(!b);
    CORRADE_VERIFY(a.isEmpty());
    CORRADE_VERIFY(b.isEmpty());
    CORRADE_COMPARE(a.size(), 0);
    CORRADE_COMPARE(b.size(), 0);
    CORRADE_COMPARE(a.flags(), StringViewFlag::Global);
    CORRADE_COMPARE(b.flags(), StringViewFlag::Global);
    CORRADE_COMPARE(static_cast<const void*>(a.data()), nullptr);
    CORRADE_COMPARE(static_cast<const void*>(b.data()), nullptr);

    CORRADE_VERIFY(std::is_nothrow_default_constructible<BasicStringView<T>>::value);
}

void StringViewTest::constructDefaultConstexpr() {
    constexpr StringView ca;
    constexpr StringView cb = nullptr;
    constexpr bool boolConversionA = !!ca;
    constexpr bool boolConversionB = !!cb;
    constexpr bool emptyA = ca.isEmpty();
    constexpr bool emptyB = cb.isEmpty();
    constexpr std::size_t sizeA = ca.size();
    constexpr std::size_t sizeB = cb.size();
    constexpr StringViewFlags flagsA = ca.flags();
    constexpr StringViewFlags flagsB = cb.flags();
    constexpr const void* dataA = ca.data();
    constexpr const void* dataB = cb.data();
    CORRADE_VERIFY(!boolConversionA);
    CORRADE_VERIFY(!boolConversionB);
    CORRADE_VERIFY(emptyA);
    CORRADE_VERIFY(emptyB);
    CORRADE_COMPARE(sizeA, 0);
    CORRADE_COMPARE(sizeB, 0);
    CORRADE_COMPARE(flagsA, StringViewFlag::Global);
    CORRADE_COMPARE(flagsB, StringViewFlag::Global);
    CORRADE_COMPARE(dataA, nullptr);
    CORRADE_COMPARE(dataB, nullptr);
}

template<class T> void StringViewTest::construct() {
    setTestCaseTemplateName(NameFor<T>::name());

    char string[]{'h', 'e', 'l', 'l', '\0', '!', '!'}; /* 7 chars */
    const BasicStringView<T> view{string, 6};
    CORRADE_VERIFY(view);
    CORRADE_VERIFY(!view.isEmpty());
    CORRADE_COMPARE(view.size(), 6);
    CORRADE_COMPARE(view.flags(), StringViewFlags{});
    CORRADE_COMPARE(static_cast<const void*>(view.data()), &string[0]);

    CORRADE_VERIFY(std::is_nothrow_constructible<BasicStringView<T>, T*, std::size_t, StringViewFlags>::value);
}

void StringViewTest::constructConstexpr() {
    constexpr const char* string = "hell\0!!"; /* 7 chars + \0 at the end */
    constexpr StringView view = {string, 6, StringViewFlag::Global|StringViewFlag::NullTerminated};
    constexpr bool boolConversion = !!view;
    constexpr bool empty = view.isEmpty();
    constexpr std::size_t size = view.size();
    constexpr StringViewFlags flags = view.flags();
    constexpr const void* data = view.data();
    CORRADE_VERIFY(boolConversion);
    CORRADE_VERIFY(!empty);
    CORRADE_COMPARE(size, 6);
    CORRADE_COMPARE(flags, StringViewFlag::Global|StringViewFlag::NullTerminated);
    {
        #if defined(CORRADE_TARGET_MSVC) && !defined(CORRADE_TARGET_CLANG) && _MSC_VER >= 1910 && _MSC_VER < 1931 && defined(_DEBUG)
        CORRADE_EXPECT_FAIL("MSVC 2017+ does some crazy shit with constexpr data. But only in Debug builds. Fixed in 2022 19.31.");
        #endif
        CORRADE_COMPARE(data, string);
    }
}

template<class T, class From> void StringViewTest::constructCharPointer() {
    setTestCaseTemplateName({NameFor<T>::name(), NameFor<From>::name()});

    From string[] = "hello\0world!";
    From* pointer = string;
    const BasicStringView<T> view = pointer;
    CORRADE_VERIFY(view);
    CORRADE_VERIFY(!view.isEmpty());
    CORRADE_COMPARE(view.size(), 5); /* stops at the first null terminator */
    CORRADE_COMPARE(view.flags(), StringViewFlag::NullTerminated);
    CORRADE_COMPARE(static_cast<const void*>(view.data()), &string[0]);

    CORRADE_VERIFY(std::is_nothrow_constructible<BasicStringView<T>, From*>::value);
}

template<class T, class From> void StringViewTest::constructCharArray() {
    setTestCaseTemplateName({NameFor<T>::name(), NameFor<From>::name()});

    /* In all cases it should be interpreted the same as with a pointer, never
       with the array being taken as an ArrayView */
    From string[] = "hello\0world!";
    const BasicStringView<T> view = string;
    CORRADE_VERIFY(view);
    CORRADE_VERIFY(!view.isEmpty());
    CORRADE_COMPARE(view.size(), 5); /* stops at the first null terminator */
    CORRADE_COMPARE(view.flags(), StringViewFlag::NullTerminated);
    CORRADE_COMPARE(static_cast<const void*>(view.data()), &string[0]);

    CORRADE_VERIFY(std::is_nothrow_constructible<BasicStringView<T>, From*>::value);
}

void StringViewTest::constructCharPointerArrayDisallowed() {
    /* To verify the crazy ambiguity-preventing SFINAE doesn't accidentally
       allow creating a MutableStringView from a const pointer. Not using
       is_convertible to catch also accidental explicit conversions. */

    CORRADE_VERIFY(std::is_constructible<StringView, char*>::value);
    CORRADE_VERIFY(std::is_constructible<MutableStringView, char*>::value);
    CORRADE_VERIFY(std::is_constructible<StringView, const char*>::value);
    CORRADE_VERIFY(!std::is_constructible<MutableStringView, const char*>::value);

    CORRADE_VERIFY(std::is_constructible<StringView, char[]>::value);
    CORRADE_VERIFY(std::is_constructible<MutableStringView, char[]>::value);
    CORRADE_VERIFY(std::is_constructible<StringView, const char[]>::value);
    CORRADE_VERIFY(!std::is_constructible<MutableStringView, const char[]>::value);
}

void StringViewTest::constructPointerNull() {
    StringView view = static_cast<const char*>(nullptr);
    CORRADE_VERIFY(!view);
    CORRADE_VERIFY(view.isEmpty());
    CORRADE_COMPARE(view.size(), 0);
    CORRADE_COMPARE(view.flags(), StringViewFlag::Global);
    CORRADE_COMPARE(static_cast<const void*>(view.data()), nullptr);
}

void StringViewTest::constructPointerFlags() {
    char string[] = "hello\0world!";
    StringView view = {string, StringViewFlag::Global};
    CORRADE_VERIFY(view);
    CORRADE_VERIFY(!view.isEmpty());
    CORRADE_COMPARE(view.size(), 5); /* stops at the first null terminator */
    CORRADE_COMPARE(view.flags(), StringViewFlag::NullTerminated|StringViewFlag::Global);
    CORRADE_COMPARE(static_cast<const void*>(view.data()), &string[0]);

    CORRADE_VERIFY(std::is_nothrow_constructible<StringView, char*, StringViewFlags>::value);
}

void StringViewTest::constructEmpty() {
    StringView view = "";
    CORRADE_VERIFY(!view);
    CORRADE_VERIFY(view.isEmpty());
    CORRADE_COMPARE(view.size(), 0);
    CORRADE_COMPARE(view.flags(), StringViewFlag::NullTerminated);
    CORRADE_VERIFY(view.data());
    CORRADE_COMPARE(view.data()[0], '\0');
}

void StringViewTest::constructNullptrSize() {
    /* This should be allowed for e.g. passing a desired layout to a function
       that allocates the memory later */

    StringView view = {nullptr, 5};
    CORRADE_VERIFY(!view); /* because it's non-empty but null */
    CORRADE_VERIFY(!view.isEmpty());
    CORRADE_COMPARE(view.size(), 5);
    /* While a null pointer alone can be treated as global and never changing,
       this not */
    CORRADE_COMPARE(view.flags(), StringViewFlags{});
    CORRADE_COMPARE(static_cast<const void*>(view.data()), nullptr);

    constexpr StringView cview = {nullptr, 5};
    CORRADE_VERIFY(!view); /* because it's non-empty but null */
    CORRADE_VERIFY(!cview.isEmpty());
    CORRADE_COMPARE(cview.size(), 5);
    /* While a null pointer alone can be treated as global and never changing,
       this not */
    CORRADE_COMPARE(cview.flags(), StringViewFlags{});
    CORRADE_COMPARE(static_cast<const void*>(cview.data()), nullptr);
}

void StringViewTest::constructFromMutable() {
    char string[] = "hello\0world!";
    const MutableStringView a = string;
    const StringView b = a;
    CORRADE_VERIFY(b);
    CORRADE_VERIFY(!b.isEmpty());
    CORRADE_COMPARE(b.size(), 5); /* stops at the first null terminator */
    CORRADE_COMPARE(b.flags(), StringViewFlag::NullTerminated);
    CORRADE_COMPARE(static_cast<const void*>(b.data()), &string[0]);

    CORRADE_VERIFY(std::is_nothrow_constructible<StringView, MutableStringView>::value);

    /* It shouldn't be possible the other way around. Not using is_convertible
       to catch also accidental explicit conversions. */
    CORRADE_VERIFY(std::is_constructible<StringView, MutableStringView>::value);
    CORRADE_VERIFY(!std::is_constructible<MutableStringView, StringView>::value);
}

void StringViewTest::constructCopy() {
    StringView a{"hello\0!", 8, StringViewFlag::Global|StringViewFlag::NullTerminated};

    StringView b = a;
    CORRADE_COMPARE(b.data(), a.data());
    CORRADE_COMPARE(b.size(), a.size());
    CORRADE_COMPARE(b.flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);

    StringView c{"hey,", 3, StringViewFlag::Global};
    c = b;
    CORRADE_COMPARE(c.data(), a.data());
    CORRADE_COMPARE(c.size(), a.size());
    CORRADE_COMPARE(c.flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);

    CORRADE_VERIFY(std::is_copy_constructible<StringView>::value);
    CORRADE_VERIFY(std::is_copy_assignable<StringView>::value);
    #ifndef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
    CORRADE_VERIFY(std::is_trivially_copy_constructible<StringView>::value);
    CORRADE_VERIFY(std::is_trivially_copy_assignable<StringView>::value);
    #endif
    CORRADE_VERIFY(std::is_nothrow_copy_constructible<StringView>::value);
    CORRADE_VERIFY(std::is_nothrow_copy_assignable<StringView>::value);
}

void StringViewTest::constructLiteral() {
    StringView view = "hell\0!"_s;
    CORRADE_VERIFY(view);
    CORRADE_VERIFY(!view.isEmpty());
    CORRADE_COMPARE(view.size(), 6);
    CORRADE_COMPARE(view.flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
    CORRADE_COMPARE(view.data()[2], 'l');

    constexpr StringView cview = "hell\0!"_s;
    CORRADE_VERIFY(cview);
    CORRADE_VERIFY(!cview.isEmpty());
    CORRADE_COMPARE(cview.size(), 6);
    CORRADE_COMPARE(cview.flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
    CORRADE_COMPARE(cview.data()[2], 'l');
}

void StringViewTest::constructLiteralEmpty() {
    StringView view = ""_s;
    CORRADE_VERIFY(!view);
    CORRADE_VERIFY(view.isEmpty());
    CORRADE_COMPARE(view.size(), 0);
    CORRADE_COMPARE(view.flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
    CORRADE_VERIFY(view.data());
    CORRADE_COMPARE(view.data()[0], '\0');
}

#ifdef CORRADE_TARGET_32BIT
void StringViewTest::constructTooLarge() {
    CORRADE_SKIP_IF_NO_DEBUG_ASSERT();

    Containers::String out;
    Error redirectError{&out};
    StringView{nullptr, std::size_t{1} << (sizeof(std::size_t)*8 - 2)};
    CORRADE_COMPARE(out,
        "Containers::StringView: string expected to be smaller than 2^30 bytes, got 1073741824\n");
}
#endif

void StringViewTest::constructNullptrNullTerminated() {
    CORRADE_SKIP_IF_NO_DEBUG_ASSERT();

    Containers::String out;
    Error redirectError{&out};
    StringView{nullptr, 0, StringViewFlag::NullTerminated};
    CORRADE_COMPARE(out,
        "Containers::StringView: can't use StringViewFlag::NullTerminated with null data\n");
}

/* Without a corresponding SFINAE check in the std::nullptr_t constructor, this
   is ambiguous, but *only* if the size_t overload has a second 64-bit
   argument. If both would be the same, it wouldn't be ambigous, if the size_t
   overload second argument was 32-bit and the other 16-bit it wouldn't be
   either. */
int integerStringOverload(std::size_t, long long) {
    return 76;
}
int integerStringOverload(StringView, int) {
    return 39;
}

void StringViewTest::constructZeroNullPointerAmbiguity() {
    /* Obvious cases */
    CORRADE_COMPARE(integerStringOverload(25, 2), 76);
    CORRADE_COMPARE(integerStringOverload(nullptr, 2), 39);

    /* This should pick the integer overload, not convert 0 to nullptr */
    CORRADE_COMPARE(integerStringOverload(0, 3), 76);
}

template<class T> void StringViewTest::convertArrayView() {
    setTestCaseTemplateName(NameFor<T>::name());

    char data[] = "hello!";
    ArrayView<T> array = data;
    CORRADE_COMPARE(array.size(), 7); /* includes the null terminator */

    BasicStringView<T> string = array;
    CORRADE_COMPARE(string.size(), 7); /* keeps the same size */
    CORRADE_COMPARE(string.flags(), StringViewFlags{});
    CORRADE_COMPARE(static_cast<const void*>(string.data()), &data[0]);

    BasicStringView<T> string2 = {array, StringViewFlag::NullTerminated};
    CORRADE_COMPARE(string2.size(), 7); /* keeps the same size */
    CORRADE_COMPARE(string2.flags(), StringViewFlag::NullTerminated);
    CORRADE_COMPARE(static_cast<const void*>(string2.data()), &data[0]);

    ArrayView<T> array2 = string;
    CORRADE_COMPARE(array2.size(), 7); /* keeps the same size */
    CORRADE_COMPARE(static_cast<const void*>(array2.data()), &data[0]);
}

constexpr const char String[] = "hell\0!!"; /* 7 chars + \0 at the end */

void StringViewTest::convertArrayViewConstexpr() {
    constexpr StringView view = {arrayView(String).exceptSuffix(1), StringViewFlag::Global|StringViewFlag::NullTerminated};
    constexpr std::size_t size = view.size();
    constexpr StringViewFlags flags = view.flags();
    constexpr const void* data = view.data();
    CORRADE_COMPARE(size, 7);
    CORRADE_COMPARE(flags, StringViewFlag::Global|StringViewFlag::NullTerminated);
    CORRADE_COMPARE(data, String);
}

void StringViewTest::convertConstFromArrayView() {
    char data[] = "hello!";
    ArrayView<char> array = data;
    CORRADE_COMPARE(array.size(), 7); /* includes the null terminator */

    /* Tests mainly that this doesn't lead to an ambigous overload with the
       char* constructor */
    StringView string = array;
    CORRADE_COMPARE(string.size(), 7); /* keeps the same size */
    CORRADE_COMPARE(string.flags(), StringViewFlags{});
    CORRADE_COMPARE(static_cast<const void*>(string.data()), &data[0]);
}

void StringViewTest::convertToConstArrayView() {
    char data[] = "hello";
    MutableStringView a = data;
    CORRADE_COMPARE(static_cast<const void*>(a.data()), data);
    CORRADE_COMPARE(a.size(), 5);

    ArrayView<const char> b = a;
    CORRADE_COMPARE(b.data(), data);
    CORRADE_COMPARE(b.size(), 5);
}

void StringViewTest::convertConstFromArray() {
    Array<char> array{Corrade::InPlaceInit, {'h', 'e', 'l', 'l'}};
    CORRADE_COMPARE(array.size(), 4);

    /* Tests mainly that this doesn't lead to an ambigous overload with the
       char* constructor */
    StringView string = array;
    CORRADE_COMPARE(string.size(), 4); /* keeps the same size */
    CORRADE_COMPARE(string.flags(), StringViewFlags{});
    CORRADE_COMPARE(static_cast<const void*>(string.data()), &array[0]);
}

template<class T> void StringViewTest::convertVoidArrayView() {
    using VoidT = typename std::conditional<std::is_const<T>::value, const void, void>::type;

    setTestCaseTemplateName(NameFor<T>::name());

    char data[] = "hello!";
    BasicStringView<T> string = data;
    CORRADE_COMPARE(string.size(), 6); /* without the null terminator */
    CORRADE_COMPARE(string.flags(), StringViewFlag::NullTerminated);
    CORRADE_COMPARE(static_cast<const void*>(string.data()), &data[0]);

    ArrayView<VoidT> array = string;
    CORRADE_COMPARE(array.size(), 6); /* keeps the same size */
    CORRADE_COMPARE(static_cast<const void*>(array.data()), &data[0]);
}

void StringViewTest::convertExternalView() {
    char data[]{'h', 'e', 'l', 'l', 'o'};
    StrView a{data, 5};
    CORRADE_COMPARE(static_cast<const void*>(a.data), data);
    CORRADE_COMPARE(a.size, 5);

    MutableStringView b = a;
    CORRADE_COMPARE(static_cast<const void*>(b.data()), data);
    CORRADE_COMPARE(b.size(), 5);

    StrView c = b;
    CORRADE_COMPARE(static_cast<const void*>(c.data), data);
    CORRADE_COMPARE(c.size, 5);

    constexpr const char* cdata = "hello world!";
    constexpr ConstStrView ca{cdata, 12};
    CORRADE_COMPARE(ca.data, StringView{"hello world!"});
    {
        #if defined(CORRADE_TARGET_MSVC) && !defined(CORRADE_TARGET_CLANG) && _MSC_VER >= 1910 && _MSC_VER < 1931 && defined(_DEBUG)
        CORRADE_EXPECT_FAIL("MSVC 2017+ does some crazy shit with constexpr data. But only in Debug builds. Fixed in 2022 19.31.");
        #endif
        CORRADE_COMPARE(static_cast<const void*>(ca.data), cdata);
    }
    CORRADE_COMPARE(ca.size, 12);

    constexpr StringView cb = ca;
    CORRADE_COMPARE(cb, StringView{"hello world!"});
    {
        #if defined(CORRADE_TARGET_MSVC) && !defined(CORRADE_TARGET_CLANG) && _MSC_VER >= 1910 && _MSC_VER < 1931 && defined(_DEBUG)
        CORRADE_EXPECT_FAIL("MSVC 2017+ does some crazy shit with constexpr data. But only in Debug builds. Fixed in 2022 19.31.");
        #endif
        CORRADE_COMPARE(static_cast<const void*>(ca.data), cdata);
    }
    CORRADE_COMPARE(cb.size(), 12);

    constexpr ConstStrView cc = cb;
    CORRADE_COMPARE(cc.data, StringView{"hello world!"});
    {
        #if defined(CORRADE_TARGET_MSVC) && !defined(CORRADE_TARGET_CLANG) && _MSC_VER >= 1910 && _MSC_VER < 1931 && defined(_DEBUG)
        CORRADE_EXPECT_FAIL("MSVC 2017+ does some crazy shit with constexpr data. But only in Debug builds. Fixed in 2022 19.31.");
        #endif
        CORRADE_COMPARE(static_cast<const void*>(ca.data), cdata);
    }
    CORRADE_COMPARE(cc.size, 12);
}

void StringViewTest::convertConstFromExternalView() {
    char data[]{'h', 'e', 'l', 'l', 'o'};
    StrView a{data, 5};
    CORRADE_COMPARE(static_cast<const void*>(a.data), data);
    CORRADE_COMPARE(a.size, 5);

    StringView b = a;
    CORRADE_COMPARE(b.data(), data);
    CORRADE_COMPARE(b.size(), 5);
}

void StringViewTest::convertToConstExternalView() {
    char data[] = "hello";
    MutableStringView a = data;
    CORRADE_COMPARE(static_cast<const void*>(a.data()), data);
    CORRADE_COMPARE(a.size(), 5);

    ConstStrView b = a;
    CORRADE_COMPARE(b.data, data);
    CORRADE_COMPARE(b.size, 5);
}

void StringViewTest::compareEquality() {
    /* Trivial case */
    StringView a = "hello";
    CORRADE_VERIFY(a == a);

    /* One is null-terminated, the other is a substring, but should compare
       equal */
    const char bData[]{'h', 'e', 'l', 'l', 'o', '3'};
    StringView b{bData, 5};
    CORRADE_VERIFY(b == b);
    CORRADE_VERIFY(a == b);
    CORRADE_VERIFY(b == a);

    /* Verify we don't just compare a common prefix */
    StringView c = "hello!";
    CORRADE_VERIFY(a != c);
    CORRADE_VERIFY(c != a);

    /* Comparison with an empty view (which is nullptr) */
    StringView empty;
    CORRADE_VERIFY(empty == empty);
    CORRADE_VERIFY(a != empty);
    CORRADE_VERIFY(empty != a);

    /* Null terminator in the middle -- it should not stop at it */
    CORRADE_VERIFY("hello\0world"_s == (StringView{"hello\0world!", 11}));
    CORRADE_VERIFY("hello\0wOrld"_s != (StringView{"hello\0world!", 11}));

    /* C strings on either side */
    CORRADE_VERIFY(a == "hello");
    CORRADE_VERIFY("hello" == a);
    CORRADE_VERIFY(c != "hello");
    CORRADE_VERIFY("hello" != c);

    /* Comparing mutable / immutable views */
    char dData[] = "hello";
    MutableStringView d = dData;
    char eData[] = "hello!";
    MutableStringView e = eData;
    CORRADE_VERIFY(a == d);
    CORRADE_VERIFY(a != e);
    CORRADE_VERIFY(d == a);
    CORRADE_VERIFY(e != a);

    /* Mutable views and immutable C strings */
    CORRADE_VERIFY(d == "hello");
    CORRADE_VERIFY(e != "hello");
    CORRADE_VERIFY("hello" == d);
    CORRADE_VERIFY("hello" != e);
}

void StringViewTest::compareNonEquality() {
    /* Test same length w/ data difference and also same prefix + extra data */
    StringView a = "hell";
    StringView b = "hella";
    StringView hello = "hello";
    StringView c = "hello";
    StringView d = "helly";
    StringView e = "hello!";

    /* Less than */
    CORRADE_VERIFY(a < hello);      CORRADE_VERIFY(!(hello < a));
    CORRADE_VERIFY(b < hello);      CORRADE_VERIFY(!(hello < b));
    CORRADE_VERIFY(!(hello < c));   CORRADE_VERIFY(!(c < hello));
    CORRADE_VERIFY(hello < d);      CORRADE_VERIFY(!(d < hello));
    CORRADE_VERIFY(hello < e);      CORRADE_VERIFY(!(e < hello));

    /* Less than or equal */
    CORRADE_VERIFY(a <= hello);     CORRADE_VERIFY(!(hello <= a));
    CORRADE_VERIFY(b <= hello);     CORRADE_VERIFY(!(hello <= b));
    CORRADE_VERIFY(hello <= c);     CORRADE_VERIFY(c <= hello);
    CORRADE_VERIFY(hello <= d);     CORRADE_VERIFY(!(d <= hello));
    CORRADE_VERIFY(hello <= e);     CORRADE_VERIFY(!(e <= hello));

    /* Greater than or equal */
    CORRADE_VERIFY(!(a >= hello));  CORRADE_VERIFY(hello >= a);
    CORRADE_VERIFY(!(b >= hello));  CORRADE_VERIFY(hello >= b);
    CORRADE_VERIFY(hello >= c);     CORRADE_VERIFY(c >= hello);
    CORRADE_VERIFY(!(hello >= d));  CORRADE_VERIFY(d >= hello);
    CORRADE_VERIFY(!(hello >= e));  CORRADE_VERIFY(e >= hello);

    /* Greater than */
    CORRADE_VERIFY(!(a > hello));  CORRADE_VERIFY(hello > a);
    CORRADE_VERIFY(!(b > hello));  CORRADE_VERIFY(hello > b);
    CORRADE_VERIFY(!(hello > c));  CORRADE_VERIFY(!(c > hello));
    CORRADE_VERIFY(!(hello > d));  CORRADE_VERIFY(d > hello);
    CORRADE_VERIFY(!(hello > e));  CORRADE_VERIFY(e > hello);

    /* Null terminator in the middle -- it should not stop at it */
    CORRADE_VERIFY("null\0hella"_s < "null\0hello"_s);
    CORRADE_VERIFY("null\0hella"_s <= "null\0hello"_s);
    CORRADE_VERIFY(!("null\0hella"_s >= "null\0hello"_s));
    CORRADE_VERIFY(!("null\0hella"_s > "null\0hello"_s));

    CORRADE_VERIFY(!("null\0helly"_s < "null\0hello"_s));
    CORRADE_VERIFY(!("null\0helly"_s <= "null\0hello"_s));
    CORRADE_VERIFY("null\0helly"_s >= "null\0hello"_s);
    CORRADE_VERIFY("null\0helly"_s > "null\0hello"_s);

    /* Comparing with an empty view should also work */
    CORRADE_VERIFY(!(StringView{} < StringView{}));
    CORRADE_VERIFY(StringView{} < hello);
    CORRADE_VERIFY(StringView{} <= hello);
    CORRADE_VERIFY(StringView{} <= StringView{});
    CORRADE_VERIFY(StringView{} >= StringView{});
    CORRADE_VERIFY(hello >= StringView{});
    CORRADE_VERIFY(hello > StringView{});
    CORRADE_VERIFY(!(StringView{} > StringView{}));
}

void StringViewTest::access() {
    /* Use the flags so we ensure the size is always properly masked out */
    const char* string = "hello\0world!";
    const StringView view{string, 12, StringViewFlag::Global|StringViewFlag::NullTerminated};
    CORRADE_COMPARE(*view.begin(), 'h');
    CORRADE_COMPARE(*view.cbegin(), 'h');
    CORRADE_COMPARE(view.front(), 'h');
    CORRADE_COMPARE(*(view.end() - 1), '!');
    CORRADE_COMPARE(*(view.cend() - 1), '!');
    CORRADE_COMPARE(view.back(), '!');
    CORRADE_COMPARE(view[6], 'w');

    constexpr StringView cview = "hello\0world!"_s;

    /* constexpr data(), size(), isEmpty(), operator bool, flags() tested in
       constructConstexpr() already */

    constexpr const char& front = cview.front();
    CORRADE_COMPARE(front, 'h');

    constexpr const char& back = cview.back();
    CORRADE_COMPARE(back, '!');

    constexpr const char* begin = cview.begin();
    constexpr const char* cbegin = cview.cbegin();
    CORRADE_COMPARE(*begin, 'h');
    CORRADE_COMPARE(*cbegin, 'h');

    constexpr const char* end = cview.end();
    constexpr const char* cend = cview.cend();
    CORRADE_COMPARE(*(end - 1), '!');
    CORRADE_COMPARE(*(cend - 1), '!');

    constexpr char o = cview[4];
    CORRADE_COMPARE(o, 'o');
}

void StringViewTest::accessMutable() {
    /* Use the flags so we ensure the size is always properly masked out */
    char string[] = "hello\0world!";
    const MutableStringView view{string, 12, StringViewFlag::Global|StringViewFlag::NullTerminated};
    view[5] = ' ';
    *view.begin() = 'I';
    ++*view.begin();
    ++view.front();
    *(view.end() - 1) = '>';
    ++*(view.cend() - 1);
    ++view.back();
    CORRADE_COMPARE(view, StringView{"Kello world@"});
}

void StringViewTest::accessInvalid() {
    CORRADE_SKIP_IF_NO_DEBUG_ASSERT();

    /* Use an empty literal to have flags set, testing that the implementation
       uses size() and not _size */
    StringView a = ""_s;
    StringView aNotNullTerminated{"", 0};
    StringView b = "hello";
    StringView bNotNullTerminated = "hello!"_s.exceptSuffix(1);
    CORRADE_COMPARE(a.flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
    CORRADE_COMPARE(aNotNullTerminated.flags(), StringViewFlags{});
    CORRADE_COMPARE(b.flags(), StringViewFlag::NullTerminated);
    CORRADE_COMPARE(bNotNullTerminated.flags(), StringViewFlag::Global);

    /* Accessing the null terminator is fine if it's present */
    a[0];
    b[5];

    Containers::String out;
    Error redirectError{&out};
    /* front() / back() should return the first ever byte before the null
       terminator and the last ever byte before the null terminator. There
       isn't any, so it asserts. */
    a.front();
    a.back();
    a[1];
    aNotNullTerminated.front();
    aNotNullTerminated.back();
    aNotNullTerminated[0];
    b[6];
    bNotNullTerminated[5];
    CORRADE_COMPARE(out,
        "Containers::StringView::front(): view is empty\n"
        "Containers::StringView::back(): view is empty\n"
        "Containers::StringView::operator[](): index 1 out of range for 0 null-terminated bytes\n"
        "Containers::StringView::front(): view is empty\n"
        "Containers::StringView::back(): view is empty\n"
        "Containers::StringView::operator[](): index 0 out of range for 0 bytes\n"
        "Containers::StringView::operator[](): index 6 out of range for 5 null-terminated bytes\n"
        "Containers::StringView::operator[](): index 5 out of range for 5 bytes\n");
}

void StringViewTest::sliceInvalid() {
    CORRADE_SKIP_IF_NO_DEBUG_ASSERT();

    /* Basically the same as in ArrayViewTest::sliceInvalid() */

    /* Do it this way to avoid (reasonable) warnings about out-of-bounds array
       access with `a - 1`. Also use the flags so we ensure the size is always
       properly masked out. */
    const char* data = "Bhello";
    StringView a{data + 1, 5, StringViewFlag::Global|StringViewFlag::NullTerminated};

    Containers::String out;
    Error redirectError{&out};

    /* Testing both pointer and size versions */
    a.slice(a.data() - 1, a.data());
    a.slice(a.data() + 5, a.data() + 6);
    a.slice(5, 6);
    a.slice(a.data() + 2, a.data() + 1);
    a.slice(2, 1);

    CORRADE_COMPARE(out,
        "Containers::StringView::slice(): slice [-1:0] out of range for 5 elements\n"
        "Containers::StringView::slice(): slice [5:6] out of range for 5 elements\n"
        "Containers::StringView::slice(): slice [5:6] out of range for 5 elements\n"
        "Containers::StringView::slice(): slice [2:1] out of range for 5 elements\n"
        "Containers::StringView::slice(): slice [2:1] out of range for 5 elements\n");
}

void StringViewTest::sliceNullptr() {
    /* Basically the same as in ArrayViewTest::sliceNullptr() -- we want the
       same semantics as this is useful for parsers */

    MutableStringView a{nullptr, 5};

    MutableStringView b = a.prefix(nullptr);
    CORRADE_VERIFY(!b.data());
    CORRADE_COMPARE(b.size(), 0);

    MutableStringView c = a.suffix(nullptr);
    CORRADE_VERIFY(!c.data());
    CORRADE_COMPARE(c.size(), 5);

    constexpr MutableStringView ca{nullptr, 5};

    constexpr MutableStringView cb = ca.prefix(nullptr);
    CORRADE_VERIFY(!cb.data());
    CORRADE_COMPARE(cb.size(), 0);

    /* constexpr MutableStringView cc = ca.suffix(nullptr) won't compile because
       arithmetic on nullptr is not allowed */

    char data[5];
    MutableStringView d{data, 5};

    MutableStringView e = d.prefix(nullptr);
    CORRADE_VERIFY(!e.data());
    CORRADE_COMPARE(e.size(), 0);

    MutableStringView f = d.suffix(nullptr);
    CORRADE_VERIFY(!f.data());
    CORRADE_COMPARE(f.size(), 0);

    constexpr StringView cd = "things"_s;
    constexpr StringView ce = cd.prefix(nullptr);
    CORRADE_VERIFY(!ce.data());
    CORRADE_COMPARE(ce.size(), 0);

    constexpr StringView cf = cd.suffix(nullptr);
    CORRADE_VERIFY(!cf.data());
    CORRADE_COMPARE(cf.size(), 0);
}

void StringViewTest::slice() {
    /* Use the flags so we ensure the size is always properly masked out */
    char data[] = "hello";
    MutableStringView a{data, 5, StringViewFlag::Global|StringViewFlag::NullTerminated};

    CORRADE_COMPARE(a.slice(1, 4), "ell"_s);
    CORRADE_COMPARE(a.sliceSize(1, 3), "ell"_s);
    CORRADE_COMPARE(a.prefix(3), "hel"_s);
    CORRADE_COMPARE(a.exceptPrefix(2), "llo"_s);
    CORRADE_COMPARE(a.exceptSuffix(2), "hel"_s);

    constexpr StringView ca = "hello"_s;
    constexpr StringView cb1 = ca.slice(1, 4);
    CORRADE_COMPARE(cb1, "ell");

    constexpr StringView cb2 = ca.sliceSize(1, 3);
    CORRADE_COMPARE(cb2, "ell");

    constexpr StringView cc = ca.prefix(3);
    CORRADE_COMPARE(cc, "hel");

    constexpr StringView cd = ca.exceptPrefix(2);
    CORRADE_COMPARE(cd, "llo");

    constexpr StringView ce = ca.exceptSuffix(2);
    CORRADE_COMPARE(ce, "hel");
}

void StringViewTest::slicePointer() {
    /* Use the flags so we ensure the size is always properly masked out */
    char data[] = "hello";
    MutableStringView a{data, 5, StringViewFlag::Global|StringViewFlag::NullTerminated};

    CORRADE_COMPARE(a.slice(data + 1, data + 4), "ell"_s);
    CORRADE_COMPARE(a.sliceSize(data + 1, 3), "ell"_s);
    CORRADE_COMPARE(a.prefix(data + 3), "hel"_s);
    CORRADE_COMPARE(a.suffix(data + 2), "llo"_s);

    #if (defined(CORRADE_TARGET_MSVC) && !defined(CORRADE_TARGET_CLANG) && _MSC_VER <= 1900) || (defined(CORRADE_TARGET_GCC) && !defined(CORRADE_TARGET_CLANG) && __GNUC__ < 7)
    CORRADE_WARN("Not constexpr on MSVC 2015, GCC 4.8, 4.9, 5 or 6 probably because of arithmetic on the C string literal");
    #else
    constexpr const char* cdata = "hello";
    constexpr StringView ca{cdata, 5};
    constexpr StringView cb1 = ca.slice(cdata + 1, cdata + 4);
    CORRADE_COMPARE(cb1, "ell");

    constexpr StringView cb2 = ca.sliceSize(cdata + 1, 3);
    CORRADE_COMPARE(cb2, "ell");

    constexpr StringView cc = ca.prefix(cdata + 3);
    CORRADE_COMPARE(cc, "hel");

    constexpr StringView cd = ca.suffix(cdata + 2);
    CORRADE_COMPARE(cd, "llo");
    #endif
}

void StringViewTest::sliceFlags() {
    StringView globalNullTerminated = "hello"_s;
    CORRADE_COMPARE(globalNullTerminated.flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);

    StringView nullTerminated = "hello";
    CORRADE_COMPARE(nullTerminated.flags(), StringViewFlag::NullTerminated);

    StringView none{"hello", 5};
    CORRADE_COMPARE(none.flags(), StringViewFlags{});

    /* Null-terminated flag stays if it's a suffix */
    CORRADE_COMPARE(globalNullTerminated.prefix(5).flags(),
        StringViewFlag::Global|StringViewFlag::NullTerminated);
    CORRADE_COMPARE(globalNullTerminated.prefix(globalNullTerminated.data() + 5).flags(),
        StringViewFlag::Global|StringViewFlag::NullTerminated);

    CORRADE_COMPARE(nullTerminated.prefix(5).flags(),
        StringViewFlag::NullTerminated);
    CORRADE_COMPARE(nullTerminated.prefix(nullTerminated.data() + 5).flags(),
        StringViewFlag::NullTerminated);

    CORRADE_COMPARE(none.prefix(5).flags(), StringViewFlags{});
    CORRADE_COMPARE(none.prefix(none.data() + 5).flags(), StringViewFlags{});

    /* Global flag stays always */
    CORRADE_COMPARE(globalNullTerminated.prefix(4).flags(),
        StringViewFlag::Global);
    CORRADE_COMPARE(globalNullTerminated.prefix(globalNullTerminated.data() + 4).flags(),
        StringViewFlag::Global);

    CORRADE_COMPARE(nullTerminated.prefix(4).flags(), StringViewFlags{});
    CORRADE_COMPARE(nullTerminated.prefix(nullTerminated.data() + 4).flags(), StringViewFlags{});

    CORRADE_COMPARE(none.prefix(4).flags(), StringViewFlags{});
    CORRADE_COMPARE(none.prefix(none.data() + 4).flags(), StringViewFlags{});
}

void StringViewTest::sliceZeroNullPointerAmbiguity() {
    StringView a = "hello";

    /* These should all unambigously pick the std::size_t overloads, not the
       T* overloads */

    CORRADE_COMPARE(a.sliceSize(0, 3), "hel"_s);

    StringView c = a.prefix(0);
    CORRADE_COMPARE(c.size(), 0);
    CORRADE_COMPARE(c.data(), static_cast<const void*>(a.data()));

    /** @todo suffix(0), once the non-deprecated suffix(std::size_t size) is a
        thing */

    constexpr StringView ca = "hello"_s;

    constexpr StringView cb = ca.sliceSize(0, 3);
    CORRADE_COMPARE(cb, "hel");

    constexpr StringView cc = ca.prefix(0);
    CORRADE_COMPARE(cc.size(), 0);
    {
        #if defined(CORRADE_TARGET_MSVC) && !defined(CORRADE_TARGET_CLANG) && _MSC_VER >= 1910 && _MSC_VER < 1931 && defined(_DEBUG)
        CORRADE_EXPECT_FAIL("MSVC 2017+ does some crazy shit with constexpr data. But only in Debug builds. Fixed in 2022 19.31.");
        #endif
        CORRADE_COMPARE(cc.data(), static_cast<const void*>(ca.data()));
    }

    /** @todo suffix(0), once the non-deprecated suffix(std::size_t size) is a
        thing */
}

void StringViewTest::splitCharacter() {
    /* Empty */
    CORRADE_COMPARE_AS(""_s.split('/'),
        Array<StringView>{},
        TestSuite::Compare::Container);
    CORRADE_COMPARE_AS(""_s.splitWithoutEmptyParts('/'),
        Array<StringView>{},
        TestSuite::Compare::Container);

    /* Only delimiter */
    CORRADE_COMPARE_AS("/"_s.split('/'),
        arrayView({""_s, ""_s}),
        TestSuite::Compare::Container);
    CORRADE_COMPARE_AS("/"_s.splitWithoutEmptyParts('/'),
        Array<StringView>{},
        TestSuite::Compare::Container);

    /* No delimiters */
    CORRADE_COMPARE_AS("abcdef"_s.split('/'),
        arrayView({"abcdef"_s}),
        TestSuite::Compare::Container);
    CORRADE_COMPARE_AS("abcdef"_s.splitWithoutEmptyParts('/'),
        arrayView({"abcdef"_s}),
        TestSuite::Compare::Container);

    /* Common case */
    CORRADE_COMPARE_AS("ab/c/def"_s.split('/'),
        arrayView({"ab"_s, "c"_s, "def"_s}),
        TestSuite::Compare::Container);
    CORRADE_COMPARE_AS("ab/c/def"_s.splitWithoutEmptyParts('/'),
        arrayView({"ab"_s, "c"_s, "def"_s}),
        TestSuite::Compare::Container);

    /* Empty parts */
    CORRADE_COMPARE_AS("ab//c/def//"_s.split('/'),
        arrayView({"ab"_s, ""_s, "c"_s, "def"_s, ""_s, ""_s}),
        TestSuite::Compare::Container);
    CORRADE_COMPARE_AS("ab//c/def//"_s.splitWithoutEmptyParts('/'),
        arrayView({"ab"_s, "c"_s, "def"_s}),
        TestSuite::Compare::Container);
}

void StringViewTest::splitString() {
    /* Using a delimiter with repeated characters to verify it doesn't try to
       find a the next one in the middle of previous (repeated) delimiter */

    /* Empty */
    CORRADE_COMPARE_AS(""_s.split("::"),
        Array<StringView>{},
        TestSuite::Compare::Container);

    /* Only delimiter */
    CORRADE_COMPARE_AS("::"_s.split("::"),
        arrayView({""_s, ""_s}),
        TestSuite::Compare::Container);

    /* No delimiters */
    CORRADE_COMPARE_AS("abcdef"_s.split("::"),
        arrayView({"abcdef"_s}),
        TestSuite::Compare::Container);

    /* Common case */
    CORRADE_COMPARE_AS("ab::c::def"_s.split("::"),
        arrayView({"ab"_s, "c"_s, "def"_s}),
        TestSuite::Compare::Container);

    /* Empty parts */
    CORRADE_COMPARE_AS("ab::::c::def::::"_s.split("::"),
        arrayView({"ab"_s, ""_s, "c"_s, "def"_s, ""_s, ""_s}),
        TestSuite::Compare::Container);

    /** @todo splitWithoutEmptyParts() everywhere, once it takes the delimiter
        as a whole */
}

void StringViewTest::splitStringEmpty() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::String out;
    Error redirectError{&out};
    "hello"_s.split("");
    CORRADE_COMPARE(out, "Containers::StringView::split(): delimiter is empty\n");
}

void StringViewTest::splitFlags() {
    /* All flags come from the slice() implementation, so just verify the edge
       cases */

    /* Usual case -- all global, only the last null-terminated */
    {
        Array<StringView> a = "a/b/c"_s.split('/');
        CORRADE_COMPARE_AS(a, arrayView({"a"_s, "b"_s, "c"_s}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE(a[0].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a[1].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a[2].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
    } {
        Array<StringView> a = "a/b///c"_s.splitWithoutEmptyParts('/');
        CORRADE_COMPARE_AS(a, arrayView({"a"_s, "b"_s, "c"_s}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE(a[0].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a[1].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a[2].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
    }

    /* Found at the end -- last empty (if not skipped) is null-terminated */
    {
        Array<StringView> a = "a/b/"_s.split('/');
        CORRADE_COMPARE_AS(a, arrayView({"a"_s, "b"_s, ""_s}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE(a[0].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a[1].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a[2].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
    } {
        Array<StringView> a = "a/b//"_s.splitWithoutEmptyParts('/');
        CORRADE_COMPARE_AS(a, arrayView({"a"_s, "b"_s}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE(a[0].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a[1].flags(), StringViewFlag::Global);
    }

    /* Not found -- the only item is null-terminated */
    {
        Array<StringView> a = "ab"_s.split('/');
        CORRADE_COMPARE_AS(a, arrayView({"ab"_s}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE(a[0].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
    } {
        Array<StringView> a = "ab"_s.splitWithoutEmptyParts('/');
        CORRADE_COMPARE_AS(a, arrayView({"ab"_s}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE(a[0].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
    }
}

void StringViewTest::splitOnAny() {
    constexpr StringView delimiters = ".:;"_s;

    /* Empty */
    CORRADE_COMPARE_AS(""_s.splitOnAnyWithoutEmptyParts(delimiters),
        Array<StringView>{},
        TestSuite::Compare::Container);

    /* Only delimiters */
    CORRADE_COMPARE_AS(delimiters.splitOnAnyWithoutEmptyParts(delimiters),
        Array<StringView>{},
        TestSuite::Compare::Container);

    /* No delimiters */
    CORRADE_COMPARE_AS("abcdef"_s.splitOnAnyWithoutEmptyParts(delimiters),
        array({"abcdef"_s}),
        TestSuite::Compare::Container);

    /* Common case */
    CORRADE_COMPARE_AS("ab:c;def"_s.splitOnAnyWithoutEmptyParts(delimiters),
        array({"ab"_s, "c"_s, "def"_s}),
        TestSuite::Compare::Container);

    /* Empty parts */
    CORRADE_COMPARE_AS("ab:c;;def."_s.splitOnAnyWithoutEmptyParts(delimiters),
        array({"ab"_s, "c"_s, "def"_s}),
        TestSuite::Compare::Container);
}

void StringViewTest::splitOnAnyFlags() {
    constexpr StringView delimiters = ".:;"_s;

    /* All flags come from the slice() implementation, so just verify the edge
       cases */

    /* Usual case -- all global, only the last null-terminated */
    {
        Array<StringView> a = "a.:b;c"_s.splitOnAnyWithoutEmptyParts(delimiters);
        CORRADE_COMPARE_AS(a, arrayView({"a"_s, "b"_s, "c"_s}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE(a[0].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a[1].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a[2].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);

    /* Found at the end -- last is not null-terminated because there are
       characters after */
    } {
        Array<StringView> a = "a.b;::"_s.splitOnAnyWithoutEmptyParts(delimiters);
        CORRADE_COMPARE_AS(a, arrayView({"a"_s, "b"_s}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE(a[0].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a[1].flags(), StringViewFlag::Global);

    /* Not found -- the only item is null-terminated */
    } {
        Array<StringView> a = "ab"_s.splitOnAnyWithoutEmptyParts(delimiters);
        CORRADE_COMPARE_AS(a, arrayView({"ab"_s}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE(a[0].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
    }
}

void StringViewTest::splitOnWhitespace() {
    CORRADE_COMPARE_AS("ab c  \t \ndef\r"_s.splitOnWhitespaceWithoutEmptyParts(),
        array({"ab"_s, "c"_s, "def"_s}),
        TestSuite::Compare::Container);
}

void StringViewTest::splitNullView() {
    CORRADE_COMPARE_AS(StringView{}.split(' '),
        Array<StringView>{},
        TestSuite::Compare::Container);
    CORRADE_COMPARE_AS(StringView{}.splitWithoutEmptyParts(' '),
        Array<StringView>{},
        TestSuite::Compare::Container);
    CORRADE_COMPARE_AS(StringView{}.splitOnAnyWithoutEmptyParts(" "),
        Array<StringView>{},
        TestSuite::Compare::Container);
    CORRADE_COMPARE_AS(StringView{}.splitOnWhitespaceWithoutEmptyParts(),
        Array<StringView>{},
        TestSuite::Compare::Container);
}

void StringViewTest::partitionCharacter() {
    /* Empty */
    CORRADE_COMPARE_AS(""_s.partition('='),
        (Array3<StringView>{"", "", ""}),
        TestSuite::Compare::Container);
    CORRADE_COMPARE_AS(""_s.partitionLast('='),
        (Array3<StringView>{"", "", ""}),
        TestSuite::Compare::Container);

    /* Happy case */
    CORRADE_COMPARE_AS("ab=c"_s.partition('='),
        (Array3<StringView>{"ab", "=", "c"}),
        TestSuite::Compare::Container);
    CORRADE_COMPARE_AS("ab=c"_s.partitionLast('='),
        (Array3<StringView>{"ab", "=", "c"}),
        TestSuite::Compare::Container);

    /* Two occurrences */
    CORRADE_COMPARE_AS("ab=c=d"_s.partition('='),
        (Array3<StringView>{"ab", "=", "c=d"}),
        TestSuite::Compare::Container);
    CORRADE_COMPARE_AS("ab=c=d"_s.partitionLast('='),
        (Array3<StringView>{"ab=c", "=", "d"}),
        TestSuite::Compare::Container);

    /* Not found */
    CORRADE_COMPARE_AS("abc"_s.partition('='),
        (Array3<StringView>{"abc", "", ""}),
        TestSuite::Compare::Container);
    CORRADE_COMPARE_AS("abc"_s.partitionLast('='),
        (Array3<StringView>{"", "", "abc"}),
        TestSuite::Compare::Container);
}

void StringViewTest::partitionString() {
    /* Unlike with split(StringView), empty delimiter is not an error */
    CORRADE_COMPARE_AS("abc"_s.partition(""),
        (Array3<StringView>{"", "", "abc"}),
        TestSuite::Compare::Container);
    CORRADE_COMPARE_AS("abc"_s.partitionLast(""),
        (Array3<StringView>{"abc", "", ""}),
        TestSuite::Compare::Container);

    /* Empty string */
    CORRADE_COMPARE_AS(""_s.partition("::"),
        (Array3<StringView>{"", "", ""}),
        TestSuite::Compare::Container);
    CORRADE_COMPARE_AS(""_s.partitionLast("::"),
        (Array3<StringView>{"", "", ""}),
        TestSuite::Compare::Container);

    /* Happy case */
    CORRADE_COMPARE_AS("ab::c"_s.partition("::"),
        (Array3<StringView>{"ab", "::", "c"}),
        TestSuite::Compare::Container);
    CORRADE_COMPARE_AS("ab::c"_s.partitionLast("::"),
        (Array3<StringView>{"ab", "::", "c"}),
        TestSuite::Compare::Container);

    /* Two occurrences */
    CORRADE_COMPARE_AS("ab::c::d"_s.partition("::"),
        (Array3<StringView>{"ab", "::", "c::d"}),
        TestSuite::Compare::Container);
    CORRADE_COMPARE_AS("ab::c::d"_s.partitionLast("::"),
        (Array3<StringView>{"ab::c", "::", "d"}),
        TestSuite::Compare::Container);

    /* Not found */
    CORRADE_COMPARE_AS("abc"_s.partition("::"),
        (Array3<StringView>{"abc", "", ""}),
        TestSuite::Compare::Container);
    CORRADE_COMPARE_AS("abc"_s.partitionLast("::"),
        (Array3<StringView>{"", "", "abc"}),
        TestSuite::Compare::Container);
}

void StringViewTest::partitionFlagsCharacter() {
    /* All flags come from the slice() implementation, so just verify the edge
       cases */

    /* Usual case -- all global, only the last null-terminated */
    {
        Array3<StringView> a1 = "ab=c"_s.partition('=');
        Array3<StringView> a2 = "ab=c"_s.partitionLast('=');
        CORRADE_COMPARE_AS(a1, (Array3<StringView>{"ab", "=", "c"}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE_AS(a2, (Array3<StringView>{"ab", "=", "c"}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE(a1[0].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a2[0].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a1[1].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a2[1].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a1[2].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a2[2].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);

    /* Found at the end -- last two null-terminated */
    } {
        Array3<StringView> a1 = "ab="_s.partition('=');
        Array3<StringView> a2 = "ab="_s.partitionLast('=');
        CORRADE_COMPARE_AS(a1, (Array3<StringView>{"ab", "=", ""}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE_AS(a2, (Array3<StringView>{"ab", "=", ""}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE(a1[0].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a2[0].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a1[1].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a2[1].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a1[2].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a2[2].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);

    /* Not found -- all three null-terminated */
    } {
        Array3<StringView> a1 = "ab"_s.partition('=');
        Array3<StringView> a2 = "ab"_s.partitionLast('=');
        CORRADE_COMPARE_AS(a1, (Array3<StringView>{"ab", "", ""}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE_AS(a2, (Array3<StringView>{"", "", "ab"}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE(a1[0].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a2[0].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a1[1].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a2[1].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a1[2].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a2[2].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);

    /* Empty -- all three null-terminated as well */
    } {
        Array3<StringView> a1 = ""_s.partition('=');
        Array3<StringView> a2 = ""_s.partitionLast('=');
        CORRADE_COMPARE_AS(a1, (Array3<StringView>{"", "", ""}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE_AS(a2, (Array3<StringView>{"", "", ""}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE(a1[0].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a2[0].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a1[1].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a2[1].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a1[2].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a2[2].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);

    /* Null pointer -- all are null as well and thus inherit the Global flag */
    } {
        Array3<StringView> a1 = StringView{nullptr}.partition('=');
        Array3<StringView> a2 = StringView{nullptr}.partitionLast('=');
        CORRADE_COMPARE_AS(a1, (Array3<StringView>{"", "", ""}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE_AS(a2, (Array3<StringView>{"", "", ""}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE(a1[0].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a2[0].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a1[1].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a2[1].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a1[2].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a2[2].flags(), StringViewFlag::Global);
    }
}

void StringViewTest::partitionFlagsString() {
    /* All flags come from the slice() implementation, so just verify the edge
       cases. Same as partitionFlags(), just with a string separator. */

    /* Usual case -- all global, only the last null-terminated */
    {
        Array3<StringView> a1 = "ab::c"_s.partition("::");
        Array3<StringView> a2 = "ab::c"_s.partitionLast("::");
        CORRADE_COMPARE_AS(a1, (Array3<StringView>{"ab", "::", "c"}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE_AS(a2, (Array3<StringView>{"ab", "::", "c"}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE(a1[0].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a2[0].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a1[1].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a2[1].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a1[2].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a2[2].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);

    /* Found at the end -- last two null-terminated */
    } {
        Array3<StringView> a1 = "ab::"_s.partition("::");
        Array3<StringView> a2 = "ab::"_s.partitionLast("::");
        CORRADE_COMPARE_AS(a1, (Array3<StringView>{"ab", "::", ""}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE_AS(a2, (Array3<StringView>{"ab", "::", ""}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE(a1[0].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a2[0].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a1[1].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a2[1].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a1[2].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a2[2].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);

    /* Not found -- all three null-terminated */
    } {
        Array3<StringView> a1 = "ab"_s.partition("::");
        Array3<StringView> a2 = "ab"_s.partitionLast("::");
        CORRADE_COMPARE_AS(a1, (Array3<StringView>{"ab", "", ""}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE_AS(a2, (Array3<StringView>{"", "", "ab"}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE(a1[0].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a2[0].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a1[1].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a2[1].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a1[2].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a2[2].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);

    /* Empty -- all three null-terminated as well */
    } {
        Array3<StringView> a1 = ""_s.partition("::");
        Array3<StringView> a2 = ""_s.partitionLast("::");
        CORRADE_COMPARE_AS(a1, (Array3<StringView>{"", "", ""}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE_AS(a2, (Array3<StringView>{"", "", ""}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE(a1[0].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a2[0].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a1[1].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a2[1].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a1[2].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
        CORRADE_COMPARE(a2[2].flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);

    /* Null pointer -- all are null as well and thus inherit the Global flag */
    } {
        Array3<StringView> a1 = StringView{nullptr}.partition("::");
        Array3<StringView> a2 = StringView{nullptr}.partitionLast("::");
        CORRADE_COMPARE_AS(a1, (Array3<StringView>{"", "", ""}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE_AS(a2, (Array3<StringView>{"", "", ""}),
            TestSuite::Compare::Container);
        CORRADE_COMPARE(a1[0].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a2[0].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a1[1].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a2[1].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a1[2].flags(), StringViewFlag::Global);
        CORRADE_COMPARE(a2[2].flags(), StringViewFlag::Global);
    }
}

void StringViewTest::partitionNullViewCharacter() {
    /* Empty string -- all are non-null */
    CORRADE_COMPARE_AS(""_s.partition('='),
        (Array3<StringView>{"", "", ""}),
        TestSuite::Compare::Container);
    CORRADE_COMPARE_AS(""_s.partitionLast('='),
        (Array3<StringView>{"", "", ""}),
        TestSuite::Compare::Container);
    for(StringView a: ""_s.partition('='))
        CORRADE_VERIFY(a.data());
    for(StringView a: ""_s.partitionLast('='))
        CORRADE_VERIFY(a.data());

    /* Nullptr string -- all are null */
    CORRADE_COMPARE_AS(StringView{}.partition('='),
        (Array3<StringView>{"", "", ""}),
        TestSuite::Compare::Container);
    CORRADE_COMPARE_AS(StringView{}.partitionLast('='),
        (Array3<StringView>{"", "", ""}),
        TestSuite::Compare::Container);
    for(StringView a: StringView{}.partition('='))
        CORRADE_VERIFY(!a.data());
    for(StringView a: StringView{}.partitionLast('='))
        CORRADE_VERIFY(!a.data());
}

void StringViewTest::partitionNullViewString() {
    /* Empty string -- all are non-null */
    CORRADE_COMPARE_AS(""_s.partition("::"),
        (Array3<StringView>{"", "", ""}),
        TestSuite::Compare::Container);
    CORRADE_COMPARE_AS(""_s.partitionLast("::"),
        (Array3<StringView>{"", "", ""}),
        TestSuite::Compare::Container);
    for(StringView a: ""_s.partition("::"))
        CORRADE_VERIFY(a.data());
    for(StringView a: ""_s.partitionLast("::"))
        CORRADE_VERIFY(a.data());

    /* Nullptr string -- all are null */
    CORRADE_COMPARE_AS(StringView{}.partition("::"),
        (Array3<StringView>{"", "", ""}),
        TestSuite::Compare::Container);
    CORRADE_COMPARE_AS(StringView{}.partitionLast("::"),
        (Array3<StringView>{"", "", ""}),
        TestSuite::Compare::Container);
    for(StringView a: StringView{}.partition("::"))
        CORRADE_VERIFY(!a.data());
    for(StringView a: StringView{}.partitionLast("::"))
        CORRADE_VERIFY(!a.data());
}

void StringViewTest::hasPrefix() {
    CORRADE_VERIFY("overcomplicated"_s.hasPrefix("over"));
    CORRADE_VERIFY(!"overcomplicated"_s.hasPrefix("oven"));

    /* Null terminator in the middle -- it should not stop at it */
    CORRADE_VERIFY("hello\0world"_s.hasPrefix("hello\0w"_s));
    CORRADE_VERIFY(!"hello\0world"_s.hasPrefix("hello\0W"_s));

    CORRADE_VERIFY("hello"_s.hasPrefix('h'));
    CORRADE_VERIFY(!"hello"_s.hasPrefix('e'));
}

void StringViewTest::hasPrefixEmpty() {
    CORRADE_VERIFY(!""_s.hasPrefix("overcomplicated"));
    CORRADE_VERIFY("overcomplicated"_s.hasPrefix(""));
    CORRADE_VERIFY(""_s.hasPrefix(""));

    CORRADE_VERIFY(!""_s.hasPrefix('a'));
    CORRADE_VERIFY(!""_s.hasPrefix('\0'));
}

void StringViewTest::hasSuffix() {
    CORRADE_VERIFY("overcomplicated"_s.hasSuffix("complicated"));
    CORRADE_VERIFY(!"overcomplicated"_s.hasSuffix("somplicated"));
    CORRADE_VERIFY(!"overcomplicated"_s.hasSuffix("overcomplicated even more"));

    /* Null terminator in the middle -- it should not stop at it */
    CORRADE_VERIFY("hello\0world"_s.hasSuffix("o\0world"_s));
    CORRADE_VERIFY(!"hello\0world"_s.hasSuffix("o\0World"_s));

    CORRADE_VERIFY("hello"_s.hasSuffix('o'));
    CORRADE_VERIFY(!"hello"_s.hasSuffix('l'));
    CORRADE_VERIFY(!"hello"_s.hasSuffix('\0'));
}

void StringViewTest::hasSuffixEmpty() {
    CORRADE_VERIFY(!""_s.hasSuffix("overcomplicated"));
    CORRADE_VERIFY("overcomplicated"_s.hasSuffix(""));
    CORRADE_VERIFY(""_s.hasSuffix(""));

    CORRADE_VERIFY(!""_s.hasSuffix('a'));
    CORRADE_VERIFY(!""_s.hasSuffix('\0'));
}

void StringViewTest::exceptPrefix() {
    CORRADE_COMPARE("overcomplicated"_s.exceptPrefix("over"), "complicated");
    CORRADE_COMPARE("overcomplicated"_s.exceptPrefix(""), "overcomplicated");

    /* Only a null view results in a null output */
    CORRADE_VERIFY(""_s.exceptPrefix("").data());
    CORRADE_VERIFY(!StringView{}.exceptPrefix("").data());
}

void StringViewTest::exceptPrefixFlags() {
    CORRADE_COMPARE("overcomplicated"_s.exceptPrefix("over").flags(),
        StringViewFlag::Global|StringViewFlag::NullTerminated);
    CORRADE_COMPARE("overcomplicated"_s.exceptPrefix("").flags(),
        StringViewFlag::Global|StringViewFlag::NullTerminated);
}

void StringViewTest::exceptPrefixInvalid() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::String out;
    Error redirectOutput{&out};
    "overcomplicated"_s.exceptPrefix("complicated");
    CORRADE_COMPARE(out,
        "Containers::StringView::exceptPrefix(): string doesn't begin with complicated\n");
}

CORRADE_HAS_TYPE(CanExceptPrefixBeCalledWith, decltype(StringView{}.exceptPrefix(std::declval<T>())));

void StringViewTest::exceptPrefixDisabledOverloads() {
    /* I thought I could use std::is_invocable (or a C++11 backport of it) to
       easily test this. Boy I was wrong, that API is absolutely useless, while
       the CORRADE_HAS_TYPE() macro is the best thing ever.

       Same as exceptSuffixDisabledOverloads(), please keep consistent */

    CORRADE_VERIFY(CanExceptPrefixBeCalledWith<std::size_t>::value);
    /* Doing exceptPrefix(1) should work */
    CORRADE_VERIFY(CanExceptPrefixBeCalledWith<int>::value);
    /* Borderline dangerous, but decltype('a') == char, so this should be ok */
    CORRADE_VERIFY(CanExceptPrefixBeCalledWith<signed char>::value);
    /* exceptPrefix('a') should be disallowed because it's too easy to misuse
       e.g. as exceptPrefix(std::toupper('a')), resulting in exceptPrefix(65) */
    {
        #ifdef CORRADE_MSVC2015_COMPATIBILITY
        CORRADE_EXPECT_FAIL("MSVC 2015 fails at life.");
        #endif
        CORRADE_VERIFY(!CanExceptPrefixBeCalledWith<char>::value);
    }
}

void StringViewTest::exceptSuffix() {
    CORRADE_COMPARE("overcomplicated"_s.exceptSuffix("complicated"), "over");
    CORRADE_COMPARE("overcomplicated"_s.exceptSuffix(""), "overcomplicated");

    /* Only a null view results in a null output */
    CORRADE_VERIFY(""_s.exceptSuffix("").data());
    CORRADE_VERIFY(!StringView{}.exceptSuffix("").data());
}

void StringViewTest::exceptSuffixFlags() {
    CORRADE_COMPARE("overcomplicated"_s.exceptSuffix("complicated").flags(),
        StringViewFlag::Global);
    CORRADE_COMPARE("overcomplicated"_s.exceptSuffix("").flags(),
        StringViewFlag::Global|StringViewFlag::NullTerminated);
}

void StringViewTest::exceptSuffixInvalid() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::String out;
    Error redirectOutput{&out};
    "overcomplicated"_s.exceptSuffix("over");
    CORRADE_COMPARE(out,
        "Containers::StringView::exceptSuffix(): string doesn't end with over\n");
}

CORRADE_HAS_TYPE(CanExceptSuffixBeCalledWith, decltype(StringView{}.exceptSuffix(std::declval<T>())));

void StringViewTest::exceptSuffixDisabledOverloads() {
    /* I thought I could use std::is_invocable (or a C++11 backport of it) to
       easily test this. Boy I was wrong, that API is absolutely useless, while
       the CORRADE_HAS_TYPE() macro is the best thing ever.

       Same as exceptPrefixDisabledOverloads(), please keep consistent */

    CORRADE_VERIFY(CanExceptSuffixBeCalledWith<std::size_t>::value);
    /* Ding exceptSuffix(1) should work */
    CORRADE_VERIFY(CanExceptSuffixBeCalledWith<int>::value);
    /* Borderline dangerous, but decltype('a') == char, so this should be ok */
    CORRADE_VERIFY(CanExceptSuffixBeCalledWith<signed char>::value);
    /* exceptPrefix('a') should be disallowed because it's too easy to misuse
       e.g. as exceptPrefix(std::toupper('a')), resulting in exceptPrefix(65) */
    {
        #ifdef CORRADE_MSVC2015_COMPATIBILITY
        CORRADE_EXPECT_FAIL("MSVC 2015 fails at life.");
        #endif
        CORRADE_VERIFY(!CanExceptSuffixBeCalledWith<char>::value);
    }
}

void StringViewTest::trimmed() {
    /* Spaces at the end */
    CORRADE_COMPARE("abc \n "_s.trimmedPrefix(), "abc \n ");
    CORRADE_COMPARE("abc \n "_s.trimmedSuffix(), "abc");

    /* Spaces at the beginning */
    CORRADE_COMPARE(" \t abc"_s.trimmedPrefix(), "abc");
    CORRADE_COMPARE(" \t abc"_s.trimmedSuffix(), " \t abc");

    /* Spaces on both beginning and end */
    CORRADE_COMPARE(" \r abc \f "_s.trimmed(), "abc");

    /* No spaces */
    CORRADE_COMPARE("abc"_s.trimmed(), "abc");

    /* All spaces */
    CORRADE_COMPARE("\t\r\n\f\v "_s.trimmed(), "");

    /* Special characters */
    CORRADE_COMPARE("oubya"_s.trimmedPrefix("aeiyou"), "bya");
    CORRADE_COMPARE("oubya"_s.trimmedSuffix("aeiyou"), "oub");
    CORRADE_COMPARE("oubya"_s.trimmed("aeiyou"), "b");
}

void StringViewTest::trimmedFlags() {
    /* Characters at the end -- only trimmed prefix should stay NullTerminated */
    CORRADE_COMPARE("abc "_s.trimmedPrefix().flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
    CORRADE_COMPARE("abc "_s.trimmedSuffix().flags(), StringViewFlag::Global);
    CORRADE_COMPARE("abc "_s.trimmed().flags(), StringViewFlag::Global);

    /* Characters at the front -- all should stay NullTerminated */
    CORRADE_COMPARE(" abc"_s.trimmedPrefix().flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
    CORRADE_COMPARE(" abc"_s.trimmedSuffix().flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);
    CORRADE_COMPARE(" abc"_s.trimmed().flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);

    /* Null pointer -- should inherit just the Global flag */
    CORRADE_COMPARE(StringView{nullptr}.trimmed().flags(), StringViewFlag::Global);
}

void StringViewTest::trimmedNullView() {
    /* Trimmed empty string is non-null */
    CORRADE_VERIFY(""_s.trimmedPrefix().data());
    CORRADE_VERIFY(""_s.trimmedSuffix().data());
    CORRADE_VERIFY(""_s.trimmed().data());

    /* Trimmed nullptr string is null */
    CORRADE_VERIFY(!StringView{nullptr}.trimmedPrefix().data());
    CORRADE_VERIFY(!StringView{nullptr}.trimmedSuffix().data());
    CORRADE_VERIFY(!StringView{nullptr}.trimmed().data());
}

void StringViewTest::findString() {
    StringView a = "hello cursed\0world!"_s;

    /* Right at the start */
    {
        CORRADE_VERIFY(a.contains("hello"));

        StringView found = a.find("hello");
        CORRADE_COMPARE(found, "hello");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data());

    /* In the middle */
    } {
        CORRADE_VERIFY(a.contains("cursed"));

        StringView found = a.find("cursed");
        CORRADE_COMPARE(found, "cursed");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 6);

    /* Right at the end */
    } {
        CORRADE_VERIFY(a.contains("world!"));

        StringView found = a.find("world!");
        CORRADE_COMPARE(found, "world!");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 13);

    /* Almost, but not quite */
    } {
        CORRADE_VERIFY(!a.contains("world!!"));

        StringView found = a.find("world!!");
        CORRADE_VERIFY(!found.data());
        CORRADE_VERIFY(found.isEmpty());

    /* Should accept a null terminator in the middle */
    } {
        CORRADE_VERIFY(a.contains("cursed\0world"_s));

        StringView found = a.find("cursed\0world"_s);
        CORRADE_COMPARE(found, "cursed\0world"_s);
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 6);

    /* Should not stop comparing at it however */
    } {
        CORRADE_VERIFY(!a.contains("cursed\0W"_s));

        StringView found = a.find("cursed\0W"_s);
        CORRADE_VERIFY(!found.data());
        CORRADE_VERIFY(found.isEmpty());

    /* And should not read the final null terminator either */
    } {
        CORRADE_VERIFY(!a.contains("world!\0"_s));

        StringView found = a.find("world!\0"_s);
        CORRADE_VERIFY(!found.data());
        CORRADE_VERIFY(found.isEmpty());
    }
}

void StringViewTest::findStringMultipleOccurences() {
    StringView a = "so, hello hell hello! hello"_s;

    /* Multiple occurrences */
    {
        CORRADE_VERIFY(a.contains("hello"));

        StringView found = a.find("hello");
        CORRADE_COMPARE(found, "hello");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 4);

    /* First occurrence almost but not quite complete */
    } {
        CORRADE_VERIFY(a.contains("hello!"));

        StringView found = a.find("hello!");
        CORRADE_COMPARE(found, "hello!");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 15);
    }
}

void StringViewTest::findStringWhole() {
    StringView a = "hell"_s;

    /* Finding a substring that's the whole string should succeed */
    {
        CORRADE_VERIFY(a.contains("hell"));

        StringView found = a.find("hell");
        CORRADE_COMPARE(found, "hell");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data());

    /* But a larger string should fail */
    } {
        CORRADE_VERIFY(!a.contains("hello"));

        StringView found = a.find("hello");
        CORRADE_VERIFY(!found.data());
        CORRADE_VERIFY(found.isEmpty());
    }
}

void StringViewTest::findCharacter() {
    #ifdef CORRADE_UTILITY_FORCE_CPU_POINTER_DISPATCH
    auto&& data = FindCharacterData[testCaseInstanceId()];
    Implementation::stringFindCharacter = data.function ? data.function :
        Implementation::stringFindCharacterImplementation(data.features);
    #else
    auto&& data = Utility::Test::cpuVariantCompiled(FindCharacterData);
    #endif
    setTestCaseDescription(Utility::format(
        data.extra ? "{}, {}" : "{}",
        Utility::Test::cpuVariantName(data), data.extra));

    if(!Utility::Test::isCpuVariantSupported(data))
        CORRADE_SKIP("CPU features not supported");

    StringView a = "hello cursed\0world!"_s;

    /* Single character at the start */
    {
        CORRADE_VERIFY(a.contains('h'));

        StringView found = a.find('h');
        CORRADE_COMPARE(found, "h");
        CORRADE_COMPARE(static_cast<const void*>(found.data()), a.data());

    /* Single character in the middle */
    } {
        CORRADE_VERIFY(a.contains('c'));

        StringView found = a.find('c');
        CORRADE_COMPARE(found, "c");
        CORRADE_COMPARE(static_cast<const void*>(found.data()), a.data() + 6);

    /* Single character at the end */
    } {
        CORRADE_VERIFY(a.contains('!'));

        StringView found = a.find('!');
        CORRADE_COMPARE(found, "!");
        CORRADE_COMPARE(static_cast<const void*>(found.data()), a.data() + 18);

    /* No such character found */
    } {
        CORRADE_VERIFY(!a.contains('a'));

        StringView found = a.find('a');
        CORRADE_VERIFY(!found.data());
        CORRADE_VERIFY(found.isEmpty());

    /* Should not read the null terminator character either */
    } {
        /* There's a \0 in the middle, skip that */
        CORRADE_VERIFY(!a.exceptPrefix(15).contains('\0'));

        StringView found = a.exceptPrefix(15).find('\0');
        CORRADE_VERIFY(!found.data());
        CORRADE_VERIFY(found.isEmpty());

    /* Multiple occurrences */
    } {
        CORRADE_VERIFY(a.contains('o'));

        StringView found = a.find('o');
        CORRADE_COMPARE(found, "o");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 4);
    }
}

void StringViewTest::findCharacterAligned() {
    #ifdef CORRADE_UTILITY_FORCE_CPU_POINTER_DISPATCH
    auto&& data = FindCharacterData[testCaseInstanceId()];
    Implementation::stringFindCharacter = data.function ? data.function :
        Implementation::stringFindCharacterImplementation(data.features);
    #else
    auto&& data = Utility::Test::cpuVariantCompiled(FindCharacterData);
    #endif
    setTestCaseDescription(Utility::format(
        data.extra ? "{}, {}" : "{}",
        Utility::Test::cpuVariantName(data), data.extra));

    if(!Utility::Test::isCpuVariantSupported(data))
        CORRADE_SKIP("CPU features not supported");

    /* Allocating an array to not have it null-terminated or SSO'd in order to
       trigger ASan if the algorithm goes OOB. Also, aligned, with 12 vectors
       in total, corresponding to the code paths:
        - the first vector before the aligned four-at-a-time block, handled by
          the unaligned preamble
        - then two four-at-a-time blocks
        - then three more blocks after, handled by the aligned postamble
        - nothing left to be handled by the unaligned postamble

        +----+    +----+----+----+----+    +----+----+----+
        |deef|    | gg : hh :i  i: jj |x2  |k   | ll |   m|
        +----+    +----+----+----+----+    +----+----+----+
    */
    Containers::Array<char> a;
    if(data.vectorSize == 16)
        a = Utility::allocateAligned<char, 16>(Corrade::ValueInit, data.vectorSize*(1 + 4*2 + 3));
    else if(data.vectorSize == 32)
        a = Utility::allocateAligned<char, 32>(Corrade::ValueInit, data.vectorSize*(1 + 4*2 + 3));
    else CORRADE_INTERNAL_ASSERT_UNREACHABLE();
    MutableStringView string = arrayView(a);
    CORRADE_COMPARE_AS(string.data(), data.vectorSize,
        TestSuite::Compare::Aligned);

    /* First vector is treated separately. It should pick the first found of
       the two; test also the very first and very last. */
    string[0] = 'd';
    string[7] = 'e';
    string[data.vectorSize - 7] = 'e';
    string[data.vectorSize - 1] = 'f';
    CORRADE_COMPARE(string.find('d').data() - string.data(), 0);
    CORRADE_COMPARE(string.find('e').data() - string.data(), 7);
    CORRADE_COMPARE(string.find('f').data() - string.data(), data.vectorSize - 1);

    /* Then it's four vectors at a time. First four would be empty, second four
       would have the data. Test each of the four separately. For each it
       should pick the first found of the two. */
    string[data.vectorSize*5 + 3] = 'g';
    string[data.vectorSize*6 - 3] = 'g';
    string[data.vectorSize*6 + 7] = 'h';
    string[data.vectorSize*7 - 7] = 'h';
    string[data.vectorSize*7 + 0] = 'i';
    string[data.vectorSize*8 - 1] = 'i';
    string[data.vectorSize*8 + 2] = 'j';
    string[data.vectorSize*9 - 2] = 'j';
    CORRADE_COMPARE(string.find('g').data() - string.data(), data.vectorSize*5 + 3);
    CORRADE_COMPARE(string.find('h').data() - string.data(), data.vectorSize*6 + 7);
    CORRADE_COMPARE(string.find('i').data() - string.data(), data.vectorSize*7 + 0);
    CORRADE_COMPARE(string.find('j').data() - string.data(), data.vectorSize*8 + 2);

    /* Last less-than-four vectors are again treated separately. Again, for
       each it should pick the last found of the two; test also the very first
       and very last of the range. */
    string[data.vectorSize* 9 + 0] = 'k';
    string[data.vectorSize*10 + 4] = 'l';
    string[data.vectorSize*11 - 4] = 'l';
    string[data.vectorSize*12 - 1] = 'm';
    CORRADE_COMPARE(string.find('k').data() - string.data(), data.vectorSize*9 + 0);
    CORRADE_COMPARE(string.find('l').data() - string.data(), data.vectorSize*10 + 4);
    CORRADE_COMPARE(string.find('m').data() - string.data(), data.vectorSize*12 - 1);

    /* A character that's not found should be handled properly even after all
       these complex code paths */
    CORRADE_VERIFY(!string.find('n'));
}

void StringViewTest::findCharacterUnaligned() {
    #ifdef CORRADE_UTILITY_FORCE_CPU_POINTER_DISPATCH
    auto&& data = FindCharacterData[testCaseInstanceId()];
    Implementation::stringFindCharacter = data.function ? data.function :
        Implementation::stringFindCharacterImplementation(data.features);
    #else
    auto&& data = Utility::Test::cpuVariantCompiled(FindCharacterData);
    #endif
    setTestCaseDescription(Utility::format(
        data.extra ? "{}, {}" : "{}",
        Utility::Test::cpuVariantName(data), data.extra));

    if(!Utility::Test::isCpuVariantSupported(data))
        CORRADE_SKIP("CPU features not supported");

    /* Allocating an array to not have it null-terminated or SSO'd in order to
       trigger ASan if the algorithm goes OOB. Also, aligned, but then slicing:
        - the first unaligned vector having all bytes but one overlapping with
          the four-at-a-time block
        - there being just one four-at-a-time block (the if() branch that skips
          the block was sufficiently tested in firstCharacterAligned())
        - there being just one full vector after, and the last unaligned vector
          again overlapping with all but one byte with it

            +----+                +----+
            |f   |                |   j|
            +----+                +----+
             +----+----+----+----+----+
        | .. |g   :    :    :   h|i   | .. |
             +----+----+----+----+----+
    */
    Containers::Array<char> a;
    if(data.vectorSize == 16)
        a = Utility::allocateAligned<char, 16>(Corrade::ValueInit, data.vectorSize*(1 + 4 + 2));
    else if(data.vectorSize == 32)
        a = Utility::allocateAligned<char, 32>(Corrade::ValueInit, data.vectorSize*(1 + 4 + 2));
    else CORRADE_INTERNAL_ASSERT_UNREACHABLE();
    MutableStringView string = a.slice(data.vectorSize - 1, a.size() - (data.vectorSize - 1));
    CORRADE_COMPARE(string.size(), data.vectorSize*5 + 2);
    CORRADE_COMPARE_AS(string.data(), data.vectorSize,
        TestSuite::Compare::NotAligned);

    /* First byte should be handled by the initial unaligned check */
    string[0] = 'f';
    string[data.vectorSize - 1] = 'f';
    CORRADE_COMPARE(string.find('f').data() - string.data(), 0);

    /* The four-vectors-at-a-time should handle the aligned middle portion.
       Test just the very first and very last of the aligned range. */
    string[data.vectorSize*0 + 1] = 'g';
    string[data.vectorSize*4 + 0] = 'h';
    CORRADE_COMPARE_AS(string.data() + 1, data.vectorSize,
        TestSuite::Compare::Aligned);
    CORRADE_COMPARE(string.find('g').data() - string.data(), data.vectorSize*0 + 1);
    CORRADE_COMPARE(string.find('h').data() - string.data(), data.vectorSize*4 + 0);

    /* The byte right after the aligned block is handled by the "less than
       four vectors" block */
    string[data.vectorSize*4 - 1] = 'i';
    CORRADE_COMPARE_AS(string.data() + data.vectorSize*4 + 1, data.vectorSize,
        TestSuite::Compare::Aligned);
    CORRADE_COMPARE(string.find('i').data() - string.data(), data.vectorSize*4 - 1);

    /* Last byte should be handled by the final unaligned check */
    string[string.size() - 1] = 'j';
    CORRADE_COMPARE(string.find('j').data() - string.data(), data.vectorSize*5 + 1);

    /* A character that's not found should be handled properly even after all
       these complex code paths */
    CORRADE_VERIFY(!string.find('k'));
}

void StringViewTest::findCharacterUnalignedLessThanTwoVectors() {
    #ifdef CORRADE_UTILITY_FORCE_CPU_POINTER_DISPATCH
    auto&& data = FindCharacterData[testCaseInstanceId()];
    Implementation::stringFindCharacter = data.function ? data.function :
        Implementation::stringFindCharacterImplementation(data.features);
    #else
    auto&& data = Utility::Test::cpuVariantCompiled(FindCharacterData);
    #endif
    setTestCaseDescription(Utility::format(
        data.extra ? "{}, {}" : "{}",
        Utility::Test::cpuVariantName(data), data.extra));

    if(!Utility::Test::isCpuVariantSupported(data))
        CORRADE_SKIP("CPU features not supported");

    /* Allocating an array to not have it null-terminated or SSO'd in order to
       trigger ASan if the algorithm goes OOB. Also, aligned, but then slicing
       so there's just two unaligned blocks overlapping with two bytes. Cannot
       overlap with just one byte as that'd mean one of them has to be aligned.

           +-----+
          X|f  gh|
           +-----+
        | .. | .. | .. |
              +-----+
              |ghi j|Y
              +-----+
    */
    Containers::Array<char> a;
    if(data.vectorSize == 16)
        a = Utility::allocateAligned<char, 16>(Corrade::ValueInit, data.vectorSize*3);
    else if(data.vectorSize == 32)
        a = Utility::allocateAligned<char, 32>(Corrade::ValueInit, data.vectorSize*3);
    else CORRADE_INTERNAL_ASSERT_UNREACHABLE();
    MutableStringView string = a.sliceSize(1, data.vectorSize*2 - 2);
    /* The data pointer shouldn't be aligned, and the first (and only) aligned
       position inside shouldn't fit a whole vector */
    CORRADE_COMPARE_AS(string.data(), data.vectorSize,
        TestSuite::Compare::NotAligned);
    CORRADE_COMPARE_AS(a.data() + 2*data.vectorSize, static_cast<void*>(string.end()),
        TestSuite::Compare::Greater);

    /* First byte should be handled by the initial unaligned check */
    string[0] = 'f';
    CORRADE_COMPARE(string.find('f').data() - string.data(), 0);

    /* Bytes right before the end of the first vector should be handled by the
       initial unaligned check */
    string[data.vectorSize - 2] = 'g';
    string[data.vectorSize - 1] = 'h';
    CORRADE_COMPARE(string.find('g').data() - string.data(), data.vectorSize - 2);
    CORRADE_COMPARE(string.find('h').data() - string.data(), data.vectorSize - 1);

    /* A byte right after the end of the first vector should be handled by the
       final unaligned check */
    string[data.vectorSize] = 'i';
    CORRADE_COMPARE(string.find('i').data() - string.data(), data.vectorSize);

    /* Last byte should be handled by the final unaligned check */
    string[string.size() - 1] = 'j';
    CORRADE_COMPARE(string.find('j').data() - string.data(), string.size() - 1);

    /* Characters right before or right after the string shouldn't be found */
    *(string.begin() - 1) = 'X';
    CORRADE_VERIFY(!string.find('X'));

    *string.end() = 'Y';
    CORRADE_VERIFY(!string.find('Y'));
}

void StringViewTest::findCharacterUnalignedLessThanOneVector() {
    #ifdef CORRADE_UTILITY_FORCE_CPU_POINTER_DISPATCH
    auto&& data = FindCharacterData[testCaseInstanceId()];
    Implementation::stringFindCharacter = data.function ? data.function :
        Implementation::stringFindCharacterImplementation(data.features);
    #else
    auto&& data = Utility::Test::cpuVariantCompiled(FindCharacterData);
    #endif
    setTestCaseDescription(Utility::format(
        data.extra ? "{}, {}" : "{}",
        Utility::Test::cpuVariantName(data), data.extra));

    if(!Utility::Test::isCpuVariantSupported(data))
        CORRADE_SKIP("CPU features not supported");

    /* Allocating an array to not have it null-terminated or SSO'd in order to
       trigger ASan if the algorithm goes OOB. Deliberately pick an unaligned
       pointer even though it shouldn't matter here. */
    Containers::Array<char> a{Corrade::ValueInit, data.vectorSize};
    MutableStringView string = a.exceptPrefix(1);
    CORRADE_COMPARE_AS(string.data(), data.vectorSize,
        TestSuite::Compare::NotAligned);

    /* It should pick the first found of the two */
    string[7] = 'f';
    string[data.vectorSize/2 + 1] = 'f';
    CORRADE_COMPARE(string.find('f').data() - string.data(), 7);

    /* A character that's not found should be handled properly here as well */
    CORRADE_VERIFY(!string.find('g'));
}

void StringViewTest::findEmpty() {
    /* Finding an empty string inside a string should return a zero-sized view
       to the first byte */
    {
        StringView a = "hello";
        CORRADE_VERIFY(a.contains(""));

        StringView found = a.find("");
        CORRADE_COMPARE(found, "");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data());

    /* Finding an empty string inside an empty string should do the same */
    } {
        StringView a = "";
        CORRADE_VERIFY(a.contains(""));

        StringView found = a.find("");
        CORRADE_VERIFY(a.data());
        CORRADE_COMPARE(found, "");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data());

    /* Finding a null view inside an empty string should do the same */
    } {
        StringView a = "";
        CORRADE_VERIFY(a.contains(nullptr));

        StringView found = a.find(nullptr);
        CORRADE_VERIFY(a.data());
        CORRADE_COMPARE(found, "");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data());

    /* Finding an empty string inside a null view should behave the same as
       if nothing was found at all */
    } {
        StringView a{nullptr};
        CORRADE_VERIFY(!a.contains(""));

        StringView found = a.find("");
        CORRADE_VERIFY(found.isEmpty());
        CORRADE_VERIFY(!found.data());

    /* Finding an arbitrary string inside a null view should not crash or do
       anything crazy either */
    } {
        StringView a{nullptr};
        CORRADE_VERIFY(!a.contains("hello"));

        StringView found = a.find("hello");
        CORRADE_VERIFY(found.isEmpty());
        CORRADE_VERIFY(!found.data());

    /* Finding an arbitrary character inside a null view should not crash or do
       anything crazy either */
    } {
        StringView a{nullptr};
        CORRADE_VERIFY(!a.contains('h'));

        StringView found = a.find('h');
        CORRADE_VERIFY(found.isEmpty());
        CORRADE_VERIFY(!found.data());
    }
}

void StringViewTest::findFlags() {
    StringView a = "hello world"_s;

    /* Right at the start should preserve just the global flag */
    {
        StringView found = a.find("hello");
        CORRADE_COMPARE(found, "hello");
        CORRADE_COMPARE(found.flags(), StringViewFlag::Global);

    /* Same for chars */
    } {
        StringView found = a.find('h');
        CORRADE_COMPARE(found, "h");
        CORRADE_COMPARE(found.flags(), StringViewFlag::Global);

    /* At the end also null-terminated */
    } {
        StringView found = a.find("world");
        CORRADE_COMPARE(found, "world");
        CORRADE_COMPARE(found.flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);

    /* Same for chars */
    } {
        StringView found = a.find('d');
        CORRADE_COMPARE(found, "d");
        CORRADE_COMPARE(found.flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);

    /* Not found should be no flags, as the failed view isn't meant to be used
       for anything anyway */
    } {
        StringView found = a.find("world!");
        CORRADE_COMPARE(found.data(), static_cast<const void*>(nullptr));
        CORRADE_COMPARE(found.flags(), StringViewFlags{});

    /* Same for chars */
    } {
        StringView found = a.find('f');
        CORRADE_COMPARE(found.data(), static_cast<const void*>(nullptr));
        CORRADE_COMPARE(found.flags(), StringViewFlag{});

    /* Empty view should be global & null-terminated */
    } {
        StringView empty = ""_s;
        StringView found = empty.find("");
        CORRADE_COMPARE(found.data(), static_cast<const void*>(empty.data()));
        CORRADE_COMPARE(found.flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);

    /* Null view should be no flags, as the failed view isn't meant to be used
       for anything anyway */
    } {
        StringView null = nullptr;
        StringView found = null.find("");
        CORRADE_COMPARE(found.data(), static_cast<const void*>(nullptr));
        CORRADE_COMPARE(found.flags(), StringViewFlags{});
    }
}

void StringViewTest::findOr() {
    /* Duplicated word to ensure it's not delegated to findLastOr() */
    StringView a = "hello hello world";

    /* Verify the returned pointer vs the usual find() */
    {
        StringView found = a.findOr("hello", a.begin());
        CORRADE_COMPARE(found, "hello");
        CORRADE_COMPARE(found.data(), static_cast<const void*>(a.data()));
    } {
        StringView found = a.find("world!");
        CORRADE_COMPARE(found.data(), static_cast<const void*>(nullptr));
        CORRADE_VERIFY(found.isEmpty());
    } {
        StringView found = a.findOr("world!", a.begin());
        CORRADE_COMPARE(found.data(), static_cast<const void*>(a.begin()));
        CORRADE_VERIFY(found.isEmpty());
    }

    /* Single character */
    {
        StringView found = a.findOr('o', a.begin());
        CORRADE_COMPARE(found, "o");
        CORRADE_COMPARE(found.data(), static_cast<const void*>(a.data() + 4));
    } {
        StringView found = a.find('p');
        CORRADE_COMPARE(found.data(), static_cast<const void*>(nullptr));
        CORRADE_VERIFY(found.isEmpty());
    } {
        StringView found = a.findOr('p', a.end());
        CORRADE_COMPARE(found.data(), static_cast<const void*>(a.end()));
        CORRADE_VERIFY(found.isEmpty());
    }
}

void StringViewTest::findLastString() {
    /* Mostly similar to findString(), except that it doesn't check contains()
       (which is internally the same algorithm as find()) */

    StringView a = "hello cursed\0world!"_s;

    /* Right at the end */
    {
        StringView found = a.findLast("world!");
        CORRADE_COMPARE(found, "world!");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 13);

    /* In the middle */
    } {
        StringView found = a.findLast("cursed");
        CORRADE_COMPARE(found, "cursed");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 6);

    /* Right at the start */
    } {
        StringView found = a.findLast("hello");
        CORRADE_COMPARE(found, "hello");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data());

    /* Almost, but not quite */
    } {
        StringView found = a.findLast("world!!");
        CORRADE_VERIFY(!found.data());
        CORRADE_VERIFY(found.isEmpty());

    /* Should accept a null terminator in the middle */
    } {
        StringView found = a.findLast("cursed\0world"_s);
        CORRADE_COMPARE(found, "cursed\0world"_s);
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 6);

    /* Should not stop comparing at it however */
    } {
        StringView found = a.findLast("cursed\0W"_s);
        CORRADE_VERIFY(!found.data());
        CORRADE_VERIFY(found.isEmpty());

    /* And should not read the final null terminator either */
    } {
        StringView found = a.findLast("world!\0"_s);
        CORRADE_VERIFY(!found.data());
        CORRADE_VERIFY(found.isEmpty());
    }
}

void StringViewTest::findLastStringMultipleOccurences() {
    /* Mostly similar to findStringMultipleOccurences(), except that it doesn't
       check contains() (which is internally the same algorithm as find()) */

    StringView a = "so, hello hell hello! hello hell"_s;

    /* Multiple occurrences */
    {
        StringView found = a.findLast("hello");
        CORRADE_COMPARE(found, "hello");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 22);

    /* Last occurrence almost but not quite complete */
    } {
        StringView found = a.findLast("hello!");
        CORRADE_COMPARE(found, "hello!");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 15);
    }
}

void StringViewTest::findLastStringWhole() {
    /* Mostly similar to findStringWhole(), except that it doesn't check
       contains() (which is internally the same algorithm as find()) */

    StringView a = "hell"_s;

    /* Finding a substring that's the whole string should succeed */
    {
        StringView found = a.findLast("hell");
        CORRADE_COMPARE(found, "hell");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data());

    /* But a larger string should fail */
    } {
        StringView found = a.findLast("hello");
        CORRADE_VERIFY(!found.data());
        CORRADE_VERIFY(found.isEmpty());
    }
}

void StringViewTest::findLastCharacter() {
    /* Mostly similar to findCharacter(), except that it doesn't check
       contains() (which is internally the same algorithm as find()) */

    StringView a = "hello cursed\0world!"_s;

    /* Single character at the end */
    {
        StringView found = a.findLast('!');
        CORRADE_COMPARE(found, "!");
        CORRADE_COMPARE(static_cast<const void*>(found.data()), a.data() + 18);

    /* Single character in the middle */
    } {
        StringView found = a.findLast('c');
        CORRADE_COMPARE(found, "c");
        CORRADE_COMPARE(static_cast<const void*>(found.data()), a.data() + 6);

    /* Single character at the start */
    } {
        StringView found = a.findLast('h');
        CORRADE_COMPARE(found, "h");
        CORRADE_COMPARE(static_cast<const void*>(found.data()), a.data());

    /* No such character found */
    } {
        StringView found = a.findLast('a');
        CORRADE_VERIFY(!found.data());
        CORRADE_VERIFY(found.isEmpty());

    /* Should not read the null terminator character either */
    } {
        /* There's a \0 in the middle, skip that */
        StringView found = a.exceptPrefix(15).findLast('\0');
        CORRADE_VERIFY(!found.data());
        CORRADE_VERIFY(found.isEmpty());

    /* Multiple character occurrences */
    } {
        StringView found = a.findLast('o');
        CORRADE_COMPARE(found, "o");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 14);
    }
}

void StringViewTest::findLastEmpty() {
    /* Mostly similar to findEmpty(), except that it doesn't check contains()
       (which is internally the same algorithm as find()) */

    /* Finding an empty string inside a string should return a zero-sized view
       to one item after the last byte */
    {
        StringView a = "hello";
        StringView found = a.findLast("");
        CORRADE_COMPARE(found, "");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 5);

    /* Finding an empty string inside an empty string should do the same */
    } {
        StringView a = "";
        StringView found = a.findLast("");
        CORRADE_VERIFY(a.data());
        CORRADE_COMPARE(found, "");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data());

    /* Finding a null view inside an empty string should do the same */
    } {
        StringView a = "";
        StringView found = a.findLast(nullptr);
        CORRADE_VERIFY(a.data());
        CORRADE_COMPARE(found, "");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data());

    /* Finding an empty string inside a null view should behave the same as
       if nothing was found at all */
    } {
        StringView a{nullptr};
        StringView found = a.findLast("");
        CORRADE_VERIFY(found.isEmpty());
        CORRADE_VERIFY(!found.data());

    /* Finding an arbitrary string inside a null view should not crash or do
       anything crazy either */
    } {
        StringView a{nullptr};
        StringView found = a.findLast("hello");
        CORRADE_VERIFY(found.isEmpty());
        CORRADE_VERIFY(!found.data());

    /* Finding an arbitrary character inside a null view should not crash or do
       anything crazy either */
    } {
        StringView a{nullptr};
        StringView found = a.findLast('h');
        CORRADE_VERIFY(found.isEmpty());
        CORRADE_VERIFY(!found.data());
    }
}

void StringViewTest::findLastFlags() {
    /* Mostly similar to findFlags() */

    StringView a = "hello world"_s;

    /* Right at the start should preserve just the global flag */
    {
        StringView found = a.findLast("hello");
        CORRADE_COMPARE(found, "hello");
        CORRADE_COMPARE(found.flags(), StringViewFlag::Global);

    /* Same for chars */
    } {
        StringView found = a.findLast('h');
        CORRADE_COMPARE(found, "h");
        CORRADE_COMPARE(found.flags(), StringViewFlag::Global);

    /* At the end also null-terminated */
    } {
        StringView found = a.findLast("world");
        CORRADE_COMPARE(found, "world");
        CORRADE_COMPARE(found.flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);

    /* Same for chars */
    } {
        StringView found = a.findLast('d');
        CORRADE_COMPARE(found, "d");
        CORRADE_COMPARE(found.flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);

    /* Not found should be no flags, as the failed view isn't meant to be used
       for anything anyway */
    } {
        StringView found = a.findLast("world!");
        CORRADE_COMPARE(found.data(), static_cast<const void*>(nullptr));
        CORRADE_COMPARE(found.flags(), StringViewFlags{});

    /* Same for chars */
    } {
        StringView found = a.findLast('f');
        CORRADE_COMPARE(found.data(), static_cast<const void*>(nullptr));
        CORRADE_COMPARE(found.flags(), StringViewFlags{});

    /* Empty view should be global & null-terminated */
    } {
        StringView empty = ""_s;
        StringView found = empty.findLast("");
        CORRADE_COMPARE(found.data(), static_cast<const void*>(empty.data()));
        CORRADE_COMPARE(found.flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);

    /* Null view should be no flags, as the failed view isn't meant to be used
       for anything anyway */
    } {
        StringView null = nullptr;
        StringView found = null.findLast("");
        CORRADE_COMPARE(found.data(), static_cast<const void*>(nullptr));
        CORRADE_COMPARE(found.flags(), StringViewFlags{});
    }
}

void StringViewTest::findLastOr() {
    /* Mostly similar to findOr(). Duplicated word to ensure it's not delegated
       to findOr(). */
    StringView a = "hello hello world";

    /* Verify the returned pointer vs the usual find() */
    {
        StringView found = a.findLastOr("hello", a.begin());
        CORRADE_COMPARE(found, "hello");
        CORRADE_COMPARE(found.data(), static_cast<const void*>(a.data() + 6));
    } {
        StringView found = a.findLast("world!");
        CORRADE_COMPARE(found.data(), static_cast<const void*>(nullptr));
        CORRADE_VERIFY(found.isEmpty());
    } {
        StringView found = a.findLastOr("world!", a.begin());
        CORRADE_COMPARE(found.data(), static_cast<const void*>(a.begin()));
        CORRADE_VERIFY(found.isEmpty());
    }

    /* Single character */
    {
        StringView found = a.findLastOr('o', a.begin());
        CORRADE_COMPARE(found, "o");
        CORRADE_COMPARE(found.data(), static_cast<const void*>(a.data() + 13));
    } {
        StringView found = a.findLast('p');
        CORRADE_COMPARE(found.data(), static_cast<const void*>(nullptr));
        CORRADE_VERIFY(found.isEmpty());
    } {
        StringView found = a.findLastOr('p', a.end());
        CORRADE_COMPARE(found.data(), static_cast<const void*>(a.end()));
        CORRADE_VERIFY(found.isEmpty());
    }
}

void StringViewTest::findAny() {
    /* Duplicated characters to test it's not delegated to findLastAny() */
    StringView a = "hello\0world"_s;

    /* Right at the start. Characters not part of the string should not cause
       it to fail. */
    {
        CORRADE_VERIFY(a.containsAny("eh!"));

        StringView found = a.findAny("eh!");
        CORRADE_COMPARE(found, "h");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data());

    /* In the middle. Duplicate characters should not cause any issues. */
    } {
        CORRADE_VERIFY(a.containsAny("olo"));

        StringView found = a.findAny("olo");
        CORRADE_COMPARE(found, "l");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 2);

    /* Right at the end */
    } {
        CORRADE_VERIFY(a.containsAny("bud"));

        StringView found = a.findAny("bud");
        CORRADE_COMPARE(found, "d");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 10);

    /* Single character to search for, equivalent to just find(c) */
    } {
        CORRADE_VERIFY(a.containsAny("w"));

        StringView found = a.findAny("w");
        CORRADE_COMPARE(found, "w");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 6);

    /* No character from the set found */
    } {
        CORRADE_VERIFY(!a.containsAny("pub"));

        StringView found = a.findAny("pub");
        CORRADE_VERIFY(!found.data());
        CORRADE_VERIFY(found.isEmpty());

    /* Sbould accept a null terminator */
    } {
        CORRADE_VERIFY(a.containsAny("p\0b"_s));

        StringView found = a.findAny("p\0b"_s);
        CORRADE_COMPARE(found, "\0"_s);
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 5);

    /* Should not stop at it however */
    } {
        CORRADE_VERIFY(a.containsAny("p\0ttery"_s));

        StringView found = a.findAny("p\0ttery"_s);
        CORRADE_COMPARE(found, "e"_s);
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 1);

    /* And should not read the final null terminator either */
    } {
        /* There's a \0 in the middle, skip that */
        CORRADE_VERIFY(!a.exceptPrefix(8).containsAny("\0"_s));

        StringView found = a.exceptPrefix(8).findAny("\0"_s);
        CORRADE_VERIFY(!found.data());
        CORRADE_VERIFY(found.isEmpty());
    }
}

void StringViewTest::findAnyEmpty() {
    /* Finding an empty set should behave the same as if nothing was found at
       all */
    {
        StringView a = "hello";
        CORRADE_VERIFY(!a.containsAny(""));

        StringView found = a.findAny("");
        CORRADE_VERIFY(found.isEmpty());
        CORRADE_VERIFY(!found.data());

    /* Finding anything in an empty string should behave the same as as if
       nothing was found at all */
    } {
        StringView a = "";
        CORRADE_VERIFY(!a.containsAny("lel"));

        StringView found = a.findAny("lel");
        CORRADE_VERIFY(found.isEmpty());
        CORRADE_VERIFY(!found.data());

    /* Finding a null view inside an empty string should behave the same as
       if nothing was found at all */
    } {
        StringView a = "";
        CORRADE_VERIFY(!a.containsAny(nullptr));

        StringView found = a.findAny(nullptr);
        CORRADE_VERIFY(found.isEmpty());
        CORRADE_VERIFY(!found.data());

    /* Finding an empty string inside a null view should behave the same as
       if nothing was found at all */
    } {
        StringView a{nullptr};
        CORRADE_VERIFY(!a.containsAny(""));

        StringView found = a.findAny("");
        CORRADE_VERIFY(found.isEmpty());
        CORRADE_VERIFY(!found.data());
    }
}

void StringViewTest::findAnyFlags() {
    StringView a = "hello"_s;

    /* Right at the start should preserve just the global flag */
    {
        StringView found = a.findAny("ho!");
        CORRADE_COMPARE(found, "h");
        CORRADE_COMPARE(found.flags(), StringViewFlag::Global);

    /* At the end also null-terminated */
    } {
        StringView found = a.findAny("ow");
        CORRADE_COMPARE(found, "o");
        CORRADE_COMPARE(found.flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);

    /* Not found should be no flags, as the failed view isn't meant to be used
       for anything anyway */
    } {
        StringView found = a.find("pub");
        CORRADE_COMPARE(found.data(), static_cast<const void*>(nullptr));
        CORRADE_COMPARE(found.flags(), StringViewFlags{});
    }
}

void StringViewTest::findAnyOr() {
    /* Duplicated characters to test that it's not delegated to
       findLastAnyOr() */
    StringView a = "hello world";

    /* Verify the returned pointer vs the usual findAny() */
    {
        StringView found = a.findAnyOr("lol", a.begin());
        CORRADE_COMPARE(found, "l");
        CORRADE_COMPARE(found.data(), static_cast<const void*>(a.data() + 2));
    } {
        StringView found = a.findAny("pub");
        CORRADE_COMPARE(found.data(), static_cast<const void*>(nullptr));
        CORRADE_VERIFY(found.isEmpty());
    } {
        StringView found = a.findAnyOr("pub", a.begin());
        CORRADE_COMPARE(found.data(), static_cast<const void*>(a.begin()));
        CORRADE_VERIFY(found.isEmpty());
    }
}

void StringViewTest::findLastAny() {
    /* Mostly similar to findAny(), except that it doesn't check containsAny()
       (which is internally the same algorithm as findAny()). Duplicated
       characters to test that it's not delegated to findAny(). */
    StringView a = "hello\0world"_s;

    /* Right at the end. Characters not part of the string should not cause
       it to fail. */
    {
        StringView found = a.findLastAny("duh!");
        CORRADE_COMPARE(found, "d");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 10);

    /* In the middle. Duplicate characters should not cause any issues. */
    } {
        StringView found = a.findLastAny("olo");
        CORRADE_COMPARE(found, "l");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 9);

    /* Right at the start */
    } {
        StringView found = a.findLastAny("uhu");
        CORRADE_COMPARE(found, "h");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data());

    /* Single character to search for, equivalent to just find(c) */
    } {
        StringView found = a.findLastAny("w");
        CORRADE_COMPARE(found, "w");
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 6);

    /* No character from the set found */
    } {
        StringView found = a.findLastAny("pub");
        CORRADE_VERIFY(!found.data());
        CORRADE_VERIFY(found.isEmpty());

    /* Sbould accept a null terminator */
    } {
        StringView found = a.findLastAny("p\0b"_s);
        CORRADE_COMPARE(found, "\0"_s);
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 5);

    /* Should not stop at it however */
    } {
        StringView found = a.findLastAny("p\0ttery"_s);
        CORRADE_COMPARE(found, "r"_s);
        CORRADE_COMPARE((static_cast<const void*>(found.data())), a.data() + 8);

    /* And should not read the final null terminator either */
    } {
        /* There's a \0 in the middle, skip that */
        StringView found = a.exceptPrefix(8).findLastAny("\0"_s);
        CORRADE_VERIFY(!found.data());
        CORRADE_VERIFY(found.isEmpty());
    }
}

void StringViewTest::findLastAnyEmpty() {
    /* Mostly similar to findAnyEmpty(), except that it doesn't check
       containsAny() (which is internally the same algorithm as findAny()) */

    /* Finding an empty set should behave the same as if nothing was found at
       all */
    {
        StringView a = "hello";
        StringView found = a.findLastAny("");
        CORRADE_VERIFY(found.isEmpty());
        CORRADE_VERIFY(!found.data());

    /* Finding anything in an empty string should behave the same as as if
       nothing was found at all */
    } {
        StringView a = "";
        StringView found = a.findLastAny("lel");
        CORRADE_VERIFY(found.isEmpty());
        CORRADE_VERIFY(!found.data());

    /* Finding a null view inside an empty string should behave the same as
       if nothing was found at all */
    } {
        StringView a = "";
        StringView found = a.findLastAny(nullptr);
        CORRADE_VERIFY(found.isEmpty());
        CORRADE_VERIFY(!found.data());

    /* Finding an empty string inside a null view should behave the same as
       if nothing was found at all */
    } {
        StringView a{nullptr};
        StringView found = a.findLastAny("");
        CORRADE_VERIFY(found.isEmpty());
        CORRADE_VERIFY(!found.data());
    }
}

void StringViewTest::findLastAnyFlags() {
    /* Mostly similar to findAnyFlags() */

    StringView a = "hello"_s;

    /* Right at the start should preserve just the global flag */
    {
        StringView found = a.findLastAny("uhu");
        CORRADE_COMPARE(found, "h");
        CORRADE_COMPARE(found.flags(), StringViewFlag::Global);

    /* At the end also null-terminated */
    } {
        StringView found = a.findLastAny("ow!");
        CORRADE_COMPARE(found, "o");
        CORRADE_COMPARE(found.flags(), StringViewFlag::Global|StringViewFlag::NullTerminated);

    /* Not found should be no flags, as the failed view isn't meant to be used
       for anything anyway */
    } {
        StringView found = a.findLastAny("pub");
        CORRADE_COMPARE(found.data(), static_cast<const void*>(nullptr));
        CORRADE_COMPARE(found.flags(), StringViewFlags{});
    }
}

void StringViewTest::findLastAnyOr() {
    /* Mostly similar to findAnyOr(). Duplicated characters to test that it's
       not delegated to findAnyOr(). */
    StringView a = "hello world";

    /* Verify the returned pointer vs the usual findLastAny() */
    {
        StringView found = a.findLastAnyOr("lol", a.begin());
        CORRADE_COMPARE(found, "l");
        CORRADE_COMPARE(found.data(), static_cast<const void*>(a.data() + 9));
    } {
        StringView found = a.findLastAny("pub");
        CORRADE_COMPARE(found.data(), static_cast<const void*>(nullptr));
        CORRADE_VERIFY(found.isEmpty());
    } {
        StringView found = a.findLastAnyOr("pub", a.begin());
        CORRADE_COMPARE(found.data(), static_cast<const void*>(a.begin()));
        CORRADE_VERIFY(found.isEmpty());
    }
}

void StringViewTest::countCharacter() {
    #ifdef CORRADE_UTILITY_FORCE_CPU_POINTER_DISPATCH
    auto&& data = CountCharacterData[testCaseInstanceId()];
    Implementation::stringCountCharacter = data.function ? data.function :
        Implementation::stringCountCharacterImplementation(data.features);
    #else
    auto&& data = Utility::Test::cpuVariantCompiled(CountCharacterData);
    #endif
    setTestCaseDescription(Utility::format(
        data.extra ? "{}, {}" : "{}",
        Utility::Test::cpuVariantName(data), data.extra));

    if(!Utility::Test::isCpuVariantSupported(data))
        CORRADE_SKIP("CPU features not supported");

    StringView a = "hello cursed\0world!"_s;

    /* No occurence */
    CORRADE_COMPARE(a.count('H'), 0);

    /* The trailing \0 shouldn't be counted, but the other should */
    CORRADE_COMPARE(a.count('\0'), 1);

    /* Right at the start / end */
    CORRADE_COMPARE(a.count('h'), 1);
    CORRADE_COMPARE(a.count('!'), 1);

    /* Multiple occurences */
    CORRADE_COMPARE(a.count('l'), 3);
    CORRADE_COMPARE(a.count('o'), 2);
}

void StringViewTest::countCharacterAligned() {
    #ifdef CORRADE_UTILITY_FORCE_CPU_POINTER_DISPATCH
    auto&& data = CountCharacterData[testCaseInstanceId()];
    Implementation::stringCountCharacter = data.function ? data.function :
        Implementation::stringCountCharacterImplementation(data.features);
    #else
    auto&& data = Utility::Test::cpuVariantCompiled(CountCharacterData);
    #endif
    setTestCaseDescription(Utility::format(
        data.extra ? "{}, {}" : "{}",
        Utility::Test::cpuVariantName(data), data.extra));

    if(!Utility::Test::isCpuVariantSupported(data))
        CORRADE_SKIP("CPU features not supported");

    /* Allocating an array to not have it null-terminated or SSO'd in order to
       trigger ASan if the algorithm goes OOB. Also, aligned, with 12 vectors
       in total, corresponding to the code paths:
        - the first vector before the aligned run, handled by the unaligned
          preamble
        - then two 64-bytes-at-a-time blocks
        - then 64-bytes-minus-one-vector after, handled by the aligned
          postamble
        - nothing left to be handled by the unaligned postamble

        +----+    +----+----+----+----+    +----+----+----+
        |    |    |    :    :    :    |x2  |    |    |    |  for 128b vectors
        +----+    +----+----+----+----+    +----+----+----+

        +----+    +----+----+    +----+
        |    |    |    :    |x2  |    | for 256b vectors
        +----+    +----+----+    +----+
    */
    Containers::Array<char> a;
    if(data.vectorSize == 16)
        a = Utility::allocateAligned<char, 16>(Corrade::ValueInit, data.vectorSize + 64*2 + (64 - data.vectorSize));
    else if(data.vectorSize == 32)
        a = Utility::allocateAligned<char, 32>(Corrade::ValueInit, data.vectorSize + 64*2 + (64 - data.vectorSize));
    else CORRADE_INTERNAL_ASSERT_UNREACHABLE();
    MutableStringView string = arrayView(a);
    CORRADE_COMPARE_AS(string.data(), data.vectorSize,
        TestSuite::Compare::Aligned);

    /* Fill each character different and verify it's found just once each
       time. Assume there's not more than 256 bytes, otherwise we'd have some
       characters more than once. */
    CORRADE_COMPARE_AS(string.size(), 256,
        TestSuite::Compare::LessOrEqual);
    for(std::size_t i = 0; i != string.size(); ++i)
        string[i] = i;
    for(std::size_t i = 0; i != string.size(); ++i) {
        CORRADE_ITERATION(i);
        CORRADE_COMPARE(string.count(i), 1);
    }

    /* Then fill everything with the same character and verify it's found in
       all positions */
    for(char& i: string)
        i = 'E';
    CORRADE_COMPARE(string.count('E'), string.size());
}

void StringViewTest::countCharacterUnaligned() {
    #ifdef CORRADE_UTILITY_FORCE_CPU_POINTER_DISPATCH
    auto&& data = CountCharacterData[testCaseInstanceId()];
    Implementation::stringCountCharacter = data.function ? data.function :
        Implementation::stringCountCharacterImplementation(data.features);
    #else
    auto&& data = Utility::Test::cpuVariantCompiled(CountCharacterData);
    #endif
    setTestCaseDescription(Utility::format(
        data.extra ? "{}, {}" : "{}",
        Utility::Test::cpuVariantName(data), data.extra));

    if(!Utility::Test::isCpuVariantSupported(data))
        CORRADE_SKIP("CPU features not supported");

    /* Allocating an array to not have it null-terminated or SSO'd in order to
       trigger ASan if the algorithm goes OOB. Also, aligned, but then slicing:
        - the first unaligned vector having gradually one byte up to all but
          one byte overlapping with the aligned run, to verify those bytes
          don't get counted twice
        - no four-at-a-time blocks
        - then there being two vectors in the aligned less-than-four run
        - the last unaligned vector the again having gradually one byte up to
          all but one byte overlapping with the aligned run

         +----+       +----+
         |    |... ...|    |
         +----+       +----+
            +----+ +----+
         ...|    | |    |...
            +----+ +----+
             +----+----+
        | .. |    |    | .. |
             +----+----+
    */
    Containers::Array<char> a;
    if(data.vectorSize == 16)
        a = Utility::allocateAligned<char, 16>(Corrade::ValueInit, data.vectorSize*(1 + 2 + 1));
    else if(data.vectorSize == 32)
        a = Utility::allocateAligned<char, 32>(Corrade::ValueInit, data.vectorSize*(1 + 2 + 1));
    else CORRADE_INTERNAL_ASSERT_UNREACHABLE();
    MutableStringView string = a.slice(1, a.size() - 1);
    CORRADE_COMPARE(string.size(), data.vectorSize*4 - 2);
    CORRADE_COMPARE_AS(string.data(), data.vectorSize,
        TestSuite::Compare::NotAligned);

    /* Fill the first aligned vector with consecutive characters */
    for(std::size_t i = 0; i != data.vectorSize - 1; ++i) {
        /* Accessing second vector in the original aligned array, + 1 to avoid
           colliding with zero-filled default values */
        a[data.vectorSize + i] = i + 1;
    }
    /* The first vector should stay not filled */
    for(std::size_t i = 0; i != data.vectorSize; ++i) {
        CORRADE_ITERATION(i);
        CORRADE_COMPARE(a[i], 0);
    }
    /* With the string gradually getting cut from the beginning those should
       get found in both the initial unaligned vector and the first aligned
       vector, but counted just once */
    for(std::size_t i = 0; i != data.vectorSize - 1; ++i) {
        CORRADE_ITERATION(i);
        CORRADE_COMPARE(string.exceptPrefix(i).count(i + 1), 1);
    }

    /* Then do the same for the last aligned vector, but in reverse */
    for(std::size_t i = 0; i != data.vectorSize - 1; ++i) {
        /* Accessing third vector in the original aligned array, + 1 to avoid
           colliding with zero-filled default values */
        a[data.vectorSize*3 - i - 1] = i + 1;
    }
    /* The last vector should stay not filled */
    for(std::size_t i = 0; i != data.vectorSize; ++i) {
        CORRADE_ITERATION(i);
        CORRADE_COMPARE(a[data.vectorSize*3 + i], 0);
    }
    /* The count of found characters should be 2 now, because the same are at
       the front as well */
    for(std::size_t i = 0; i != data.vectorSize - 1; ++i) {
        CORRADE_ITERATION(i);
        CORRADE_COMPARE(string.exceptSuffix(i).count(i + 1), 2);
    }

    /* Fill everything with the same character and verify it's found in all
       positions in all possible unaligned variants */
    for(char& i: string)
        i = 'H';
    for(std::size_t i = 0; i != data.vectorSize - 1; ++i) {
        CORRADE_COMPARE(string.exceptPrefix(i).count('H'), string.size() - i);
        CORRADE_COMPARE(string.exceptSuffix(i).count('H'), string.size() - i);
        CORRADE_COMPARE(string.exceptPrefix(i)
                              .exceptSuffix(i).count('H'), string.size() - i*2);
    }
}

void StringViewTest::countCharacterUnalignedLessThanTwoVectors() {
    #ifdef CORRADE_UTILITY_FORCE_CPU_POINTER_DISPATCH
    auto&& data = CountCharacterData[testCaseInstanceId()];
    Implementation::stringCountCharacter = data.function ? data.function :
        Implementation::stringCountCharacterImplementation(data.features);
    #else
    auto&& data = Utility::Test::cpuVariantCompiled(CountCharacterData);
    #endif
    setTestCaseDescription(Utility::format(
        data.extra ? "{}, {}" : "{}",
        Utility::Test::cpuVariantName(data), data.extra));

    if(!Utility::Test::isCpuVariantSupported(data))
        CORRADE_SKIP("CPU features not supported");

    /* Allocating an array to not have it null-terminated or SSO'd in order to
       trigger ASan if the algorithm goes OOB. Also, aligned, but then slicing
       so there's just two unaligned blocks overlapping with two bytes. Cannot
       overlap with just one byte as that'd mean one of them has to be aligned.

           +-----+
           |     |
           +-----+
        | .. | .. | .. |
              +-----+
              |     |
              +-----+
    */
    Containers::Array<char> a;
    if(data.vectorSize == 16)
        a = Utility::allocateAligned<char, 16>(Corrade::ValueInit, data.vectorSize*3);
    else if(data.vectorSize == 32)
        a = Utility::allocateAligned<char, 32>(Corrade::ValueInit, data.vectorSize*3);
    else CORRADE_INTERNAL_ASSERT_UNREACHABLE();
    MutableStringView string = a.sliceSize(1, data.vectorSize*2 - 2);
    /* The data pointer shouldn't be aligned, and the first (and only) aligned
       position inside shouldn't fit a whole vector */
    CORRADE_COMPARE_AS(string.data(), data.vectorSize,
        TestSuite::Compare::NotAligned);
    CORRADE_COMPARE_AS(a.data() + 2*data.vectorSize, static_cast<void*>(string.end()),
        TestSuite::Compare::Greater);

    /* Fill the string with consecutive characters, each should be found
       exactly once even though it's matched in either one or both vectors. The
       countCharacterUnaligned() case above tested other misalignments of both
       the initial and final vector sufficiently as well as overlap between the
       unaligned vectors and the aligned vectors so there's no slicing here. */
    for(std::size_t i = 0; i != string.size(); ++i)
        string[i] = i;
    for(std::size_t i = 0; i != string.size(); ++i)
        CORRADE_COMPARE(string.count(i), 1);

    /* Then fill everything with the same character and verify it's found in
       all positions */
    /** @todo have Utility::fill(), finally */
    Utility::copy("#"_s*string.size(), string);
    CORRADE_COMPARE(string.count('#'), string.size());

    /* The counting shouldn't count outside of the string */
    Utility::copy("X"_s*a.size(), a);
    CORRADE_COMPARE(string.count('X'), string.size());

    /* And neither if the actual string doesn't contain the character at all */
    Utility::copy(" "_s*string.size(), string);
    CORRADE_COMPARE(string.count('X'), 0);
}

void StringViewTest::countCharacterUnalignedLessThanOneVector() {
    #ifdef CORRADE_UTILITY_FORCE_CPU_POINTER_DISPATCH
    auto&& data = CountCharacterData[testCaseInstanceId()];
    Implementation::stringCountCharacter = data.function ? data.function :
        Implementation::stringCountCharacterImplementation(data.features);
    #else
    auto&& data = Utility::Test::cpuVariantCompiled(CountCharacterData);
    #endif
    setTestCaseDescription(Utility::format(
        data.extra ? "{}, {}" : "{}",
        Utility::Test::cpuVariantName(data), data.extra));

    if(!Utility::Test::isCpuVariantSupported(data))
        CORRADE_SKIP("CPU features not supported");

    /* Allocating an array to not have it null-terminated or SSO'd in order to
       trigger ASan if the algorithm goes OOB. Deliberately pick an unaligned
       pointer even though it shouldn't matter here. */
    Containers::Array<char> a{Corrade::ValueInit, data.vectorSize};
    MutableStringView string = a.exceptPrefix(1);
    CORRADE_COMPARE_AS(string.data(), data.vectorSize,
        TestSuite::Compare::NotAligned);

    /* Fill the string with consecutive characters, each should be found
       exactly once by the switch case. */
    for(std::size_t i = 0; i != string.size(); ++i)
        string[i] = i;
    for(std::size_t i = 0; i != string.size(); ++i)
        CORRADE_COMPARE(string.count(i), 1);

    /* Then fill everything with the same character and verify it's found in
       all positions */
    for(char& i: string)
        i = 'X';
    CORRADE_COMPARE(string.count('X'), string.size());
}

void StringViewTest::debugFlag() {
    Containers::String out;

    Debug{&out} << StringViewFlag::Global << StringViewFlag(0xf0f0u);
    CORRADE_COMPARE(out, "Containers::StringViewFlag::Global Containers::StringViewFlag(0xf0f0)\n");
}

void StringViewTest::debugFlags() {
    Containers::String out;

    Debug{&out} << (StringViewFlag::Global|StringViewFlag::NullTerminated) << StringViewFlags{};
    CORRADE_COMPARE(out, "Containers::StringViewFlag::Global|Containers::StringViewFlag::NullTerminated Containers::StringViewFlags{}\n");
}

void StringViewTest::debug() {
    Containers::String out;
    /* The operator<< is implemented directly in Debug, testing here to have
       everything together */
    Debug{&out} << "lolwat, using string views to\0test string views?!"_s;
    CORRADE_COMPARE(out, "lolwat, using string views to\0test string views?!\n"_s);
}

}}}}

CORRADE_TEST_MAIN(Corrade::Containers::Test::StringViewTest)
