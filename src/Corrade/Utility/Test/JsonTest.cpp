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

#include <climits> /* *LONG_MAX */
#include <cmath> /* NAN, INFINITY */

#include "Corrade/Containers/Array.h"
#include "Corrade/Containers/GrowableArray.h"
#include "Corrade/Containers/Optional.h"
#include "Corrade/Containers/Pair.h"
#include "Corrade/Containers/StridedArrayView.h"
#include "Corrade/Containers/StridedBitArrayView.h"
#include "Corrade/Containers/String.h"
#include "Corrade/Containers/StringIterable.h"
#include "Corrade/TestSuite/Tester.h"
#include "Corrade/TestSuite/Compare/Container.h"
#include "Corrade/TestSuite/Compare/String.h"
#include "Corrade/Utility/Format.h"
#include "Corrade/Utility/Json.h"
#include "Corrade/Utility/Path.h"

#include "configure.h"

namespace Corrade { namespace Utility { namespace Test { namespace {

using namespace Containers::Literals;

struct JsonTest: TestSuite::Tester {
    public:
        explicit JsonTest();

        void singleObject();
        void singleArray();
        void singleNull();
        void singleBoolean();
        void singleNumber();
        void singleString();

        void simpleObject();
        void simpleArray();
        void nested();

        void error();
        void errorOption();

        void parseObjects();
        void parseArrays();
        void parseNulls();
        void parseBools();
        void parseDoubles();
        void parseFloats();
        void parseUnsignedInts();
        void parseInts();
        void parseUnsignedLongs();
        void parseLongs();
        void parseSizes();
        void parseStringKeys();
        void parseStrings();

        void parseOption();
        void parseSubtree();

        void parseEmptyObjectOrArray();
        void parseBitArray();
        #ifdef CORRADE_BUILD_DEPRECATED
        void parseBoolArrayDeprecated();
        #endif
        void parseDoubleArray();
        void parseFloatArray();
        void parseUnsignedIntArray();
        void parseIntArray();
        void parseUnsignedLongArray();
        void parseLongArray();
        void parseSizeArray();
        void parseStringArray();

        void reparseNumberDifferentType();
        void reparseSingleNumberDifferentType();
        void reparseNumberArrayDifferentType();
        void reparseNumberDifferentTypeFailKeepOriginalValue();

        void parsedObjectChildAccess();
        void parsedObjectFind();
        void parsedObjectFindEmpty();
        void parsedObjectAccessEmpty();
        void parsedArrayFind();
        void parsedArrayFindEmpty();
        void parsedArrayAccessEmpty();

        void parseError();
        void parseOptionError();
        void parseSingleError();
        void parseArrayError();
        void parseTokenNotOwned();

        void tokenFromTokenData();
        void tokenFromTokenDataNotOwned();

        void iterator();
        void iteratorInvalid();
        void iteratorObject();
        void iteratorObjectInvalid();
        void iteratorArray();
        void iteratorArrayInvalid();

        void iterate();
        void iterateOutOfRange();
        void iterateObject();
        void iterateObjectTokens();
        void iterateObjectNotObject();
        void iterateObjectNotParsed();
        void iterateObjectKeyNotParsed();
        void iterateArray();
        void iterateArrayTokens();
        void iterateArrayNotArray();
        void iterateArrayNotParsed();

        void findObjectKey();
        void findObjectKeyNotFound();
        void findObjectKeyNotObject();
        void findObjectKeyNotParsed();
        void findObjectKeyKeyNotParsed();
        void findArrayIndex();
        void findArrayIndexNotFound();
        void findArrayIndexNotArray();
        void findArrayIndexNotParsed();

        void commonArrayType();
        void commonArrayTypeParsedHeterogeneous();
        void commonArrayTypeParsedArrayNotParsed();
        void commonArrayTypeNotArray();

        void asTypeWrongType();
        void asTypeNotParsed();
        void asTypeWrongParsedType();

        void asBitArray();
        #ifdef CORRADE_BUILD_DEPRECATED
        void asBoolArrayDeprecated();
        #endif
        void asBitArrayNotAllSame();
        void asBitArrayNotAllParsed();
        void asBitArrayUnexpectedSize();
        void asDoubleArray();
        void asDoubleArrayNotAllSame();
        void asDoubleArrayUnexpectedSize();
        void asFloatArray();
        void asFloatArrayNotAllSame();
        void asFloatArrayUnexpectedSize();
        void asUnsignedIntArray();
        void asUnsignedIntArrayNotAllSame();
        void asUnsignedIntArrayUnexpectedSize();
        void asIntArray();
        void asIntArrayNotAllSame();
        void asIntArrayUnexpectedSize();
        void asUnsignedLongArray();
        void asUnsignedLongArrayNotAllSame();
        void asUnsignedLongArrayUnexpectedSize();
        void asLongArray();
        void asLongArrayNotAllSame();
        void asLongArrayUnexpectedSize();
        void asSizeArray();
        void asSizeArrayNotAllSame();
        void asSizeArrayUnexpectedSize();
        void asStringArray();
        void asStringArrayNotAllSame();
        void asStringArrayNotAllParsed();
        void asStringArrayUnexpectedSize();
        void asTypeArrayNotArray();
        void asTypeArrayNotParsed();

        void fromStringFilenameOffsetError();
        void fromStringFilenameOffsetErrorSubsequentLine();
        void fromStringFilenameOffsetParseOptionError();
        void fromStringFilenameOffsetParseError();

        void fromFile();
        void fromFileReadError();
        void fromFileOptionReadError();
        void fromFileError();
        void fromFileParseOptionError();
        void fromFileParseError();

        #ifndef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
        void tokenConstructCopy();
        #endif
        void tokenDataConstructInvalid();
        #ifndef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
        void tokenDataConstructCopy();
        void tokenOffsetSizeConstructCopy();
        #endif
        void constructCopy();
        void constructMove();

        void debugTokenType();
        void debugTokenParsedType();

        void fromTokenDataNull();
        void fromTokenDataBool();
        void fromTokenDataDouble();
        void fromTokenDataFloat();
        void fromTokenDataUnsignedInt();
        void fromTokenDataInt();
        void fromTokenDataUnsignedLong();
        void fromTokenDataLong();
        void fromTokenDataTheOtherUnsignedLongType();
        void fromTokenDataTheOtherLongType();
        void fromTokenDataSize();
        void fromTokenDataString();
        void fromTokenDataStringEscaped();
        void fromTokenDataArrayEmpty();
        void fromTokenDataArray();
        void fromTokenDataObjectEmpty();
        void fromTokenDataObject();
        void fromTokenDataNested();
        void fromTokenDataNoTokens();
        void fromTokenDataInvalidTokenArraySizes();
        void fromTokenDataTokenOffsetSizeOutOfBounds();
        void fromTokenDataStringTokenDataSizeTooSmall();
        void fromTokenDataEscapedStringOutOfBounds();
        void fromTokenDataObjectKeyWithNoValue();
        void fromTokenDataObjectKeyNotString();
        void fromTokenDataObjectStringNotKey();
        void fromTokenDataStringValueKey();
        void fromTokenDataInvalidObjectChildCount();
        void fromTokenDataInvalidArrayChildCount();
        void fromTokenDataInvalidNestedObjectChildCount();
        void fromTokenDataInvalidNestedArrayChildCount();
        void fromTokenDataExtraRootTokens();
        void fromTokenDataParseEmpty();
};

const struct {
    const char* name;
    const char* data;
    const char* message;
} ErrorData[] {
    {"empty",
        " \n\r \n \t\t  ",
        "file too short, expected a value at <in>:3:6"},
    {"object end alone",
        "\n} ",
        "expected a value but got } at <in>:2:1"},
    {"array end alone",
        "]",
        "expected a value but got ] at <in>:1:1"},
    {"object end after array",
        "\n   [ \n\n } ",
        "unexpected } at <in>:4:2 for an array starting at <in>:2:4"},
    {"array end after object",
        "\n   { \n\n ] ",
        "unexpected ] at <in>:4:2 for an object starting at <in>:2:4"},
    {"number as a key",
        "{\n    5:",
        "expected \" or } but got 5 at <in>:2:5"},
    {"object as a key",
        "{\n    {",
        "expected \" or } but got { at <in>:2:5"},
    {"object end after key",
        "{\n  \"hello\"\n}",
        "expected : but got } at <in>:3:1"},
    {"object end after colon",
        "{\n  \"hello\":\n}",
        "expected a value but got } at <in>:3:1"},
    {"misplaced colon",
        "{\n  \"key\" \"value\":",
        "expected : but got \" at <in>:2:9"},
    {"nothing after key",
        "{\n  \"hello\"\n",
        "file too short, expected : at <in>:3:1"},
    {"nothing after colon",
        "{\n  \"hello\":\n",
        "file too short, expected a value at <in>:3:1"},
    {"nothing after object value",
        "\n   {  \"hello\": 3\n",
        "file too short, expected closing } for object starting at <in>:2:4"},
    {"nothing after array value",
        "\n   [  3\n",
        "file too short, expected closing ] for array starting at <in>:2:4"},
    {"stray comma before object end",
        "{\"hello\": 3,\n   }",
        "expected \" but got } at <in>:2:4"},
    {"stray comma before array end",
        "[3,\n   ]",
        "expected a value but got ] at <in>:2:4"},
    {"colon after object value",
        "{\n  \"hello\": \"hi\":",
        "expected , or } but got : at <in>:2:16"},
    {"colon in an array",
        "[\n  \"hello\":",
        "expected , or ] but got : at <in>:2:10"},
    {"\\v in a string",
        "  \n\"vertical\n \\vtab\n\" ",
        "unexpected string escape sequence \\v at <in>:3:2"},
    {"unterminated string",
        "  \n\"hello!! \n\\\" ",
        "file too short, unterminated string literal starting at <in>:2:1"},
    {"comment",
        "\n\n    /* JSON, wake up! */",
        "unexpected / at <in>:3:5"},
    /* supported by strtod(), but not by JSON, so it should fail. OTOH, -.5e5
       will fail only later during parse */
    {"fractional number without a leading zero",
        "\n\n\t  .5e5",
        "unexpected . at <in>:3:4"},
    /* supported by strto*(), but not by JSON, so it should fail */
    {"explicitly positive number",
        "\n\n\t  +1.5",
        "unexpected + at <in>:3:4"},
    /* supported by strtod(), but not by JSON, so it should fail. OTOH, -INF
       will fail only later during parse */
    {"INF",
        "\n\n\t  INF",
        "unexpected I at <in>:3:4"},
    /* supported by strtod(), but not by JSON, so it should fail. OTOH, -INF
       will fail only later during parse */
    {"NAN",
        "\n\n\t  NAN",
        "unexpected N at <in>:3:4"},
    {"BOM", /** @todo handle this gracefully? */
        "\xef\xbb\xbf",
        "unexpected \xef at <in>:1:1"},
    {"comma after a root literal",
        "false,",
        "expected document end but got , at <in>:1:6"},
    {"comma after a root number",
        "56,",
        "expected document end but got , at <in>:1:3"},
    {"comma after a root string",
        "\"hey\",",
        "expected document end but got , at <in>:1:6"},
    {"comma after a root object",
        "{},",
        "expected document end but got , at <in>:1:3"},
    {"comma after a root array",
        "[],",
        "expected document end but got , at <in>:1:3"},
};

const struct {
    const char* name;
    bool singleValue;
    Containers::StringViewFlags flags;
} ParseObjectData[]{
    /* All variants use Global unless said otherwise, to verify that the parsed
       result is cached and reused */
    {"", false,
        Containers::StringViewFlag::Global},
    {"single value", true,
        Containers::StringViewFlag::Global},
    {"non-global but null-terminated input string", false,
        Containers::StringViewFlag::NullTerminated},
};

const struct {
    const char* name;
    bool singleValue;
    Containers::StringViewFlags flags;
} ParseArrayData[]{
    /* All variants use Global unless said otherwise, to verify that the parsed
       result is cached and reused */
    {"", false,
        Containers::StringViewFlag::Global},
    {"single value", true,
        Containers::StringViewFlag::Global},
    {"non-global but null-terminated input string", false,
        Containers::StringViewFlag::NullTerminated},
};

const struct {
    const char* name;
    bool singleValue;
    Containers::StringViewFlags flags;
} ParseNullData[]{
    /* All variants use Global unless said otherwise, to verify that the parsed
       result is cached and reused */
    {"", false,
        Containers::StringViewFlag::Global},
    {"single value", true,
        Containers::StringViewFlag::Global},
    {"non-global but null-terminated input string", false,
        Containers::StringViewFlag::NullTerminated},
};

const struct {
    const char* name;
    const char* json;
    bool singleValue;
    bool expected;
    Containers::StringViewFlags flags;
} ParseBoolData[]{
    /* All variants use Global unless said otherwise, to verify that the parsed
       result is cached and reused */
    {"true", "true", false, true,
        Containers::StringViewFlag::Global},
    {"false", "false", false, false,
        Containers::StringViewFlag::Global},
    {"single value", "true", true, true,
        Containers::StringViewFlag::Global},
    {"non-global but null-terminated input string", "true", false, true,
        Containers::StringViewFlag::NullTerminated},
};

const struct {
    const char* name;
    const char* json;
    bool singleValue;
    double expected;
    Containers::StringViewFlags flags;
} ParseDoubleOrFloatData[]{
    /* All variants use Global unless said otherwise, to verify that the parsed
       result is cached and reused */
    {"", "35.7", false, 35.7,
        Containers::StringViewFlag::Global},
    {"negative", "-35.7", false, -35.7,
        Containers::StringViewFlag::Global},
    {"negative zero", "-0", false, -0.0, /** @todo check this more precisely */
        Containers::StringViewFlag::Global},
    {"exponent", "-3550.0e-2", false, -35.5,
        Containers::StringViewFlag::Global},
    {"exponent uppercase", "-35.5E2", false, -3550,
        Containers::StringViewFlag::Global},
    {"exponent explicit plus", "-35.5E+2", false, -3550,
        Containers::StringViewFlag::Global},
    {"127 characters",
       "1234.567890123456789012345678901234567890" /* 40 chars on a line */
        "1234567890123456789012345678901234567890"
        "1234567890123456789012345678901234567890123456", false, 1234.567890123456789,
        Containers::StringViewFlag::Global},
    {"single value", "35.7", true, 35.7,
        Containers::StringViewFlag::Global},
    {"non-global but null-terminated input string", "35.7", false, 35.7,
        Containers::StringViewFlag::NullTerminated},
};

const struct {
    const char* name;
    const char* json;
    bool singleValue;
    std::uint32_t expected;
    Containers::StringViewFlags flags;
} ParseUnsignedIntData[]{
    /* All variants use Global unless said otherwise, to verify that the parsed
       result is cached and reused */
    {"", "357", false, 357,
        Containers::StringViewFlag::Global},
    {"zero", "0", false, 0,
        Containers::StringViewFlag::Global},
    /* Verifying both "full bits" and also a value that needs the whole width,
       because apparently if 64 "full bits" get parsed into an int and then
       expanded back to 64bits, you get the correct number. Sigh. */
    {"max value, full bit width", "4000000000", false, 4000000000,
        Containers::StringViewFlag::Global},
    {"max value, all bits set", "4294967295", false, 4294967295,
        Containers::StringViewFlag::Global},
    {"127 characters",
      // 1234567890123456789012345678901234567890 (40 chars on a line)
        "0000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000901234567", false, 901234567,
        Containers::StringViewFlag::Global},
    {"single value", "357", true, 357,
        Containers::StringViewFlag::Global},
    {"non-global but null-terminated input string", "357", false, 357,
        Containers::StringViewFlag::NullTerminated},
};

const struct {
    const char* name;
    const char* json;
    bool singleValue;
    std::int32_t expected;
    Containers::StringViewFlags flags;
} ParseIntData[]{
    /* All variants use Global unless said otherwise, to verify that the parsed
       result is cached and reused */
    {"", "357", false, 357,
        Containers::StringViewFlag::Global},
    {"negative", "-464", false, -464,
        Containers::StringViewFlag::Global},
    /* Verifying both "full bits" and also a value that needs the whole width,
       because apparently if 64 "full bits" get parsed into an int and then
       expanded back to 64bits, you get the correct number. Sigh. */
    {"min value, full bit width", "-2000000000", false, -2000000000,
        Containers::StringViewFlag::Global},
    /* -2147483648 causes an "unary minus operator applied to unsigned type,
       result still unsigned" warning on MSVC, there's simply no way to enter
       that value as a literal. Don't ask how it's with a min negative 64bit
       value, even more terrible. */
    {"min value, all bits set", "-2147483648", false, -2147483647 - 1,
        Containers::StringViewFlag::Global},
    {"max value, full bit width", "2000000000", false, 2000000000,
        Containers::StringViewFlag::Global},
    {"max value, all bits set", "2147483647", false, 2147483647,
        Containers::StringViewFlag::Global},
    {"127 characters",
      // 1234567890123456789012345678901234567890 (40 chars on a line)
       "-0000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000090123456", false, -90123456,
        Containers::StringViewFlag::Global},
    {"single value", "-357", true, -357,
        Containers::StringViewFlag::Global},
    {"non-global but null-terminated input string", "357", false, 357,
        Containers::StringViewFlag::NullTerminated},
};

const struct {
    const char* name;
    const char* json;
    bool singleValue;
    std::uint64_t expected;
    Containers::StringViewFlags flags;
} ParseUnsignedLongData[]{
    /* All variants use Global unless said otherwise, to verify that the parsed
       result is cached and reused */
    {"", "357", false, 357,
        Containers::StringViewFlag::Global},
    {"zero", "0", false, 0,
        Containers::StringViewFlag::Global},
    /* Verifying both "full bits" and also a value that needs the whole width,
       because apparently if 64 "full bits" get parsed into an int and then
       expanded back to 64bits, you get the correct number. Sigh. */
    {"max 52bit value, full bit width", "4000000000000000", false, 4000000000000000ull,
        Containers::StringViewFlag::Global},
    {"max 52bit value, all bits set", "4503599627370495", false, 4503599627370495ull,
        Containers::StringViewFlag::Global},
    {"127 characters",
      // 1234567890123456789012345678901234567890 (40 chars on a line)
        "0000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000"
        "00000000000000000000000000000002345678901234567", false, 2345678901234567ull,
        Containers::StringViewFlag::Global},
    {"single value", "357", true, 357,
        Containers::StringViewFlag::Global},
    {"non-global but null-terminated input string", "357", false, 357,
        Containers::StringViewFlag::NullTerminated},
};

const struct {
    const char* name;
    const char* json;
    bool singleValue;
    std::int64_t expected;
    Containers::StringViewFlags flags;
} ParseLongData[]{
    /* All variants use Global unless said otherwise, to verify that the parsed
       result is cached and reused */
    {"", "357", false, 357,
        Containers::StringViewFlag::Global},
    {"negative", "-464", false, -464,
        Containers::StringViewFlag::Global},
    /* Verifying both "full bits" and also a value that needs the whole width,
       because apparently if 64 "full bits" get parsed into an int and then
       expanded back to 64bits, you get the correct number. Sigh. */
    {"min 53bit value, full bit width", "-4000000000000000", false, -4000000000000000ll,
        Containers::StringViewFlag::Global},
    {"min 53bit value, all bits set", "-4503599627370496", false, -4503599627370496ll,
        Containers::StringViewFlag::Global},
    {"max 53bit value, full bit width", "4000000000000000", false, 4000000000000000ll,
        Containers::StringViewFlag::Global},
    {"max 53bit value, all bits set", "4503599627370495", false, 4503599627370495ll,
        Containers::StringViewFlag::Global},
    {"127 characters",
      // 1234567890123456789012345678901234567890 (40 chars on a line)
        "-0000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000"
        "0000000000000000000000000000000234567890123456", false, -234567890123456ll,
        Containers::StringViewFlag::Global},
    {"single value", "-357", true, -357,
        Containers::StringViewFlag::Global},
    {"non-global but null-terminated input string", "357", false, 357,
        Containers::StringViewFlag::NullTerminated},
};

const struct {
    TestSuite::TestCaseDescriptionSourceLocation name;
    Containers::StringView json;
    bool singleValue;
    const char* expected;
    Containers::StringViewFlags expectedFlags;
    const char* expectFail;
} ParseStringData[]{
    {"",
        "\"hello!\"", false,
        "hello!", {}, nullptr},
    {"empty",
        "\"\"", false,
        "", {}, nullptr},
    {"escapes",
        "\"\\\"\\\\\\/\\b\\f\\n\\r\\t\"", false,
        "\"\\/\b\f\n\r\t", {}, nullptr},
    /* Adapted from UnicodeTest::utf32utf8(), converting the input to hex */
    {"1-character Unicode escape",
        "\"\\u007f\"", false,
        "\x7f", {}, nullptr},
    {"2-character Unicode escape",
        "\"\\u03ac\"", false,
        "\xce\xac", {}, nullptr},
    {"3-character Unicode escape",
        "\"\\uae09\"", false,
        "\xea\xb8\x89", {}, nullptr},
    {"uppercase Unicode escape",
        "\"\\uAE09\"", false,
        "\xea\xb8\x89", {}, nullptr},
    {"4-character Unicode escape",
        /* From https://en.wikipedia.org/wiki/JSON#Character_encoding */
        "\"\\ud83d\\ude10\"", false,
        "\xf0\x9f\x98\x90", {},
        "UTF-16 surrogate pairs are not decoded properly at the moment."},
    {"SSO string with escapes",
        "\"\\\\\"", false,
        "\\", {}, nullptr},
    {"non-SSO string with escapes",
        "\"this is a very long escaped\\nstring, \\\"yes\\\"!\"", false,
        "this is a very long escaped\nstring, \"yes\"!", {}, nullptr},
    {"global literal",
        "\"hello!\""_s, false,
        "hello!", Containers::StringViewFlag::Global|Containers::StringViewFlag::NullTerminated, nullptr},
    {"global non-null-terminated literal",
        "\"hello!\"!"_s.exceptSuffix(1), false,
        "hello!", Containers::StringViewFlag::Global, nullptr},
    {"global escaped literal",
        "\"hell\\\"o\\\"!\""_s, false,
        "hell\"o\"!", {}, nullptr},
    {"single value",
        "\"hello!\"", true,
        "hello!", {}, nullptr},
    {"single escaped value",
        "\"hell\\\"o\\\"!\"", true,
        "hell\"o\"!", {}, nullptr},
    {"single global value",
        "\"hello!\""_s, true,
        "hello!", Containers::StringViewFlag::Global, nullptr},
    {"single global escaped value",
        "\"hell\\\"o\\\"!\""_s, true,
        "hell\"o\"!", {}, nullptr}
};

const struct {
    const char* name;
    Json::Option option;
    std::size_t tokenParsed, tokenParsed2, tokenNotParsedCount;
    JsonToken::ParsedType parsedType;
    const char* tokenData;
} ParseOptionData[]{
    {"objects", Json::Option::ParseLiterals,
        2, 12, 27 - 10,
        JsonToken::ParsedType::Other, "{}"},
    {"arrays", Json::Option::ParseLiterals,
        4, 14, 27 - 10,
        JsonToken::ParsedType::Other, "[]"},
    {"nulls", Json::Option::ParseLiterals,
        6, 16, 27 - 10,
        JsonToken::ParsedType::Other, "null"},
    {"bools", Json::Option::ParseLiterals,
        8, 18, 27 - 10,
        JsonToken::ParsedType::Other, "true"},
    {"doubles", Json::Option::ParseDoubles,
        20, 24, 27 - 2,
        JsonToken::ParsedType::Double, "35"},
    {"floats", Json::Option::ParseFloats,
        20, 24, 27 - 2,
        JsonToken::ParsedType::Float, "35"},
    {"string keys", Json::Option::ParseStringKeys,
        25, 21, 27 - 13,
        JsonToken::ParsedType::Other, "\"string\""},
    {"strings", Json::Option::ParseStrings,
        26, 22, 27 - 15,
        JsonToken::ParsedType::Other, "\"hello\""},
};

const struct {
    const char* name;
    bool(Json::*function)(JsonToken);
    std::size_t parseRoot;
    std::size_t tokenParsed, tokenParsedDeep, tokenNotParsed, tokenNotParsedCount;
    JsonToken::ParsedType parsedType;
    const char* tokenData;
} ParseSubtreeData[]{
    {"objects", &Json::parseLiterals, 1,
        24, 17, 30, 32 - 12,
        JsonToken::ParsedType::Other, "{}"},
    {"arrays", &Json::parseLiterals, 1,
        25, 18, 31, 32 - 12,
        JsonToken::ParsedType::Other, "[]"},
    {"nulls", &Json::parseLiterals, 1,
        3, 9, 26, 32 - 12,
        JsonToken::ParsedType::Other, "null"},
    {"bools", &Json::parseLiterals, 1,
        5, 11, 27, 32 - 12,
        JsonToken::ParsedType::Other, "true"},
    {"doubles", &Json::parseDoubles, 1,
        20, 13, 28, 32 - 2,
        JsonToken::ParsedType::Double, "35"},
    {"floats", &Json::parseFloats, 1,
        20, 13, 28, 32 - 2,
        JsonToken::ParsedType::Float, "35"},
    {"unsigned ints", &Json::parseUnsignedInts, 1,
        20, 13, 28, 32 - 2,
        JsonToken::ParsedType::UnsignedInt, "35"},
    {"ints", &Json::parseInts, 1,
        20, 13, 28, 32 - 2,
        JsonToken::ParsedType::Int, "35"},
    {"unsigned longs", &Json::parseUnsignedLongs, 1,
        20, 13, 28, 32 - 2,
        JsonToken::ParsedType::UnsignedLong, "35"},
    {"longs", &Json::parseLongs, 1,
        20, 13, 28, 32 - 2,
        JsonToken::ParsedType::Long, "35"},
    {"sizes", &Json::parseSizes, 1,
        20, 13, 28, 32 - 2,
        JsonToken::ParsedType::Size, "35"},
    {"string keys", &Json::parseStringKeys, 6,
        6, 14, 21, 32 - 5,
        JsonToken::ParsedType::Other, "\"nested\""},
    {"strings", &Json::parseStrings, 1,
        23, 16, 29, 32 - 11,
        JsonToken::ParsedType::Other, "\"hello\""}
};

const struct {
    const char* name;
    const char* json;
    Containers::Optional<std::size_t>(*function)(Json&);
} EmptyObjectOrArrayData[]{
    {"object", "{}", [](Json& json) {
        const auto out = json.parseObject(json.root());
        return out && out->begin() == out->end() ? Containers::optional(std::size_t{}) : Containers::NullOpt;
    }},
    {"array", "[]", [](Json& json) {
        const auto out = json.parseArray(json.root());
        return out && out->begin() == out->end() ? Containers::optional(std::size_t{}) : Containers::NullOpt;
    }},
    {"bool array", "[]", [](Json& json) {
        const auto out = json.parseBitArray(json.root());
        return out ? Containers::optional(out->size()) : Containers::NullOpt;
    }},
    #ifdef CORRADE_BUILD_DEPRECATED
    {"bool array, deprecated", "[]", [](Json& json) {
        CORRADE_IGNORE_DEPRECATED_PUSH
        const auto out = json.parseBoolArray(json.root());
        CORRADE_IGNORE_DEPRECATED_POP
        return out ? Containers::optional(out->size()) : Containers::NullOpt;
    }},
    #endif
    {"double array", "[]", [](Json& json) {
        const auto out = json.parseDoubleArray(json.root());
        return out ? Containers::optional(out->size()) : Containers::NullOpt;
    }},
    {"float array", "[]", [](Json& json) {
        const auto out = json.parseFloatArray(json.root());
        return out ? Containers::optional(out->size()) : Containers::NullOpt;
    }},
    {"unsigned int array", "[]", [](Json& json) {
        const auto out = json.parseUnsignedIntArray(json.root());
        return out ? Containers::optional(out->size()) : Containers::NullOpt;
    }},
    {"int array", "[]", [](Json& json) {
        const auto out = json.parseIntArray(json.root());
        return out ? Containers::optional(out->size()) : Containers::NullOpt;
    }},
    {"unsigned long array", "[]", [](Json& json) {
        const auto out = json.parseUnsignedLongArray(json.root());
        return out ? Containers::optional(out->size()) : Containers::NullOpt;
    }},
    {"long array", "[]", [](Json& json) {
        const auto out = json.parseLongArray(json.root());
        return out ? Containers::optional(out->size()) : Containers::NullOpt;
    }},
    {"size array", "[]", [](Json& json) {
        const auto out = json.parseSizeArray(json.root());
        return out ? Containers::optional(out->size()) : Containers::NullOpt;
    }},
    {"string array", "[]", [](Json& json) {
        const auto out = json.parseStringArray(json.root());
        return out ? Containers::optional(out->size()) : Containers::NullOpt;
    }},
};

const struct {
    const char* name;
    Containers::StringViewFlags flags;
} ParseStringArrayData[]{
    {"", {}},
    {"global literal", Containers::StringViewFlag::Global},
};

const struct {
    const char* name;
    const char* jsonArray;
    const char* jsonObject;
    std::size_t tokenIndex;
} ParseObjectArrayEmptyData[]{
    {"root token", "[]", "{}", 0},
    {"nested", "{\"a\": [], \"b\": 3}", "[16, {}, 23]", 2},
};

const struct {
    const char* name;
    bool(Json::*function)(JsonToken);
    Containers::StringView json; /* testing \0 bytes in strings */
    const char* message;
} ParseErrorData[]{
    {"invalid null literal", &Json::parseLiterals,
        "no!",
        "parseLiterals(): invalid null literal no! at <in>:3:6"},
    {"invalid true literal", &Json::parseLiterals,
        "toomuch",
        "parseLiterals(): invalid bool literal toomuch at <in>:3:6"},
    {"invalid false literal", &Json::parseLiterals,
        "foe",
        "parseLiterals(): invalid bool literal foe at <in>:3:6"},
    {"double literal too long", &Json::parseDoubles, /* 40 chars on a line */
       "1234.567890123456789012345678901234567890"
        "1234567890123456789012345678901234567890"
        "12345678901234567890123456789012345678901234567",
        "parseDoubles(): too long numeric literal 1234.567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567 at <in>:3:6"},
    {"float literal too long", &Json::parseFloats, /* 40 chars on a line */
       "1234.567890123456789012345678901234567890"
        "1234567890123456789012345678901234567890"
        "12345678901234567890123456789012345678901234567",
        "parseFloats(): too long numeric literal 1234.567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567 at <in>:3:6"},
    {"unsigned int literal too long", &Json::parseUnsignedInts,
      // 1234567890123456789012345678901234567890 (40 chars on a line)
        "0000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000012345678",
        "parseUnsignedInts(): too long numeric literal 00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000012345678 at <in>:3:6"},
    {"int literal too long", &Json::parseInts,
      // 1234567890123456789012345678901234567890 (40 chars on a line)
       "-0000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000001234567",
        "parseInts(): too long numeric literal -0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001234567 at <in>:3:6"},
    {"unsigned long literal too long", &Json::parseUnsignedLongs,
      // 1234567890123456789012345678901234567890 (40 chars on a line)
        "0000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000012345678",
        "parseUnsignedLongs(): too long numeric literal 00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000012345678 at <in>:3:6"},
    {"long literal too long", &Json::parseLongs,
      // 1234567890123456789012345678901234567890 (40 chars on a line)
       "-0000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000001234567",
        "parseLongs(): too long numeric literal -0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001234567 at <in>:3:6"},
    {"invalid double literal", &Json::parseDoubles,
        "78.5x",
        "parseDoubles(): invalid floating-point literal 78.5x at <in>:3:6"},
    {"invalid float literal", &Json::parseFloats,
        "78.5x",
        "parseFloats(): invalid floating-point literal 78.5x at <in>:3:6"},
    {"invalid unsigned integer literal", &Json::parseUnsignedInts,
        "78x",
        "parseUnsignedInts(): invalid unsigned integer literal 78x at <in>:3:6"},
    {"invalid integer literal", &Json::parseInts,
        "-78x",
        "parseInts(): invalid integer literal -78x at <in>:3:6"},
    {"invalid unsigned long literal", &Json::parseUnsignedLongs,
        "78x",
        "parseUnsignedLongs(): invalid unsigned integer literal 78x at <in>:3:6"},
    {"invalid long literal", &Json::parseLongs,
        "-78x",
        "parseLongs(): invalid integer literal -78x at <in>:3:6"},
    {"unsigned integer literal with an exponent", &Json::parseUnsignedInts,
        "78e5",
        "parseUnsignedInts(): invalid unsigned integer literal 78e5 at <in>:3:6"},
    {"integer literal with an exponent", &Json::parseInts,
        "78e5",
        "parseInts(): invalid integer literal 78e5 at <in>:3:6"},
    {"unsigned long literal with an exponent", &Json::parseUnsignedLongs,
        "78e5",
        "parseUnsignedLongs(): invalid unsigned integer literal 78e5 at <in>:3:6"},
    {"long literal with an exponent", &Json::parseLongs,
        "78e5",
        "parseLongs(): invalid integer literal 78e5 at <in>:3:6"},
    {"unsigned integer literal with a period", &Json::parseUnsignedInts,
        "78.0",
        "parseUnsignedInts(): invalid unsigned integer literal 78.0 at <in>:3:6"},
    {"integer literal with a period", &Json::parseInts,
        "78.0",
        "parseInts(): invalid integer literal 78.0 at <in>:3:6"},
    {"unsigned long literal with a period", &Json::parseUnsignedLongs,
        "78.0",
        "parseUnsignedLongs(): invalid unsigned integer literal 78.0 at <in>:3:6"},
    {"long literal with a period", &Json::parseLongs,
        "78.0",
        "parseLongs(): invalid integer literal 78.0 at <in>:3:6"},
    {"unsigned integer literal with a minus", &Json::parseUnsignedInts,
        "-78",
        /** @todo what the fuck stroul(), returning 18446744073709551538?! */
        "parseUnsignedInts(): too large integer literal -78 at <in>:3:6"},
    {"unsigned long literal with a minus", &Json::parseUnsignedLongs,
        "-78",
        /** @todo what the fuck stroull(), returning 18446744073709551538?! */
        "parseUnsignedLongs(): too large integer literal -78 at <in>:3:6"},
    /* std::strtoull() returns 1 in this case, very useful */
    /** @todo fix once we have our own parsing routines */
    {"large unsigned long literal with a minus", &Json::parseUnsignedLongs,
        "-18446744073709551615",
        nullptr},
    {"unsigned integer literal too large", &Json::parseUnsignedInts,
        "4294967296",
        "parseUnsignedInts(): too large integer literal 4294967296 at <in>:3:6"},
    {"integer literal too small", &Json::parseInts,
        "-2147483649",
        "parseInts(): too small or large integer literal -2147483649 at <in>:3:6"},
    {"integer literal too large", &Json::parseInts,
        "2147483648",
        "parseInts(): too small or large integer literal 2147483648 at <in>:3:6"},
    {"unsigned long literal too large", &Json::parseUnsignedLongs,
        "4503599627370496",
        "parseUnsignedLongs(): too large integer literal 4503599627370496 at <in>:3:6"},
    {"long literal too small", &Json::parseLongs,
        "-4503599627370497",
        "parseLongs(): too small or large integer literal -4503599627370497 at <in>:3:6"},
    {"long literal too large", &Json::parseLongs,
        "4503599627370496",
        "parseLongs(): too small or large integer literal 4503599627370496 at <in>:3:6"},
    /* NAN or INF without a leading - fails during parse already */
    {"negative double INF literal", &Json::parseDoubles,
        "-INF",
        /* *Has to* be handled on to avoid clashing with the NaN bit pattern
           reusal */
        "parseDoubles(): invalid floating-point literal -INF at <in>:3:6"},
    {"negative float INF literal", &Json::parseFloats,
        "-INF",
        nullptr},
    {"negative double NaN literal", &Json::parseDoubles,
        "-NAN",
        /* *Has to* be handled on to avoid clashing with the NaN bit pattern
           reusal */
        "parseDoubles(): invalid floating-point literal -NAN at <in>:3:6"},
    {"negative float NaN literal", &Json::parseFloats,
        "-NAN",
        nullptr},
    /* Those should fail but unfortunately they don't */
    /** @todo fix once we have our own parsing routines, checking
        post-conversion would be an unnecessary maintenance overhead */
    {"double literal with leading zero", &Json::parseDoubles,
        "01.5",
        nullptr},
    {"float literal with leading zero", &Json::parseFloats,
        "-01.5",
        nullptr},
    /* Leading + and leading period fails the initial tokenization already */
    {"negative double literal with leading period", &Json::parseDoubles,
        "-.5",
        nullptr},
    {"negative float literal with leading period", &Json::parseFloats,
        "-.5",
        nullptr},
    {"double literal with trailing period", &Json::parseDoubles,
        "-1.",
        nullptr},
    {"float literal with trailing period", &Json::parseFloats,
        "1.",
        nullptr},
    {"unsigned int literal with leading zero", &Json::parseUnsignedInts,
        "045",
        nullptr},
    {"int literal with leading zero", &Json::parseInts,
        "-045",
        nullptr},
    {"unsigned long literal with leading zero", &Json::parseUnsignedLongs,
        "045",
        nullptr},
    {"long literal with leading zero", &Json::parseLongs,
        "-045",
        nullptr},
    {"hexadecimal double literal", &Json::parseDoubles,
        "0x355P6",
        nullptr},
    {"hexadecimal float literal", &Json::parseDoubles,
        "0X35p-6",
        nullptr},
    {"hexadecimal unsigned int literal", &Json::parseUnsignedInts,
        "0xabc",
        "parseUnsignedInts(): invalid unsigned integer literal 0xabc at <in>:3:6"},
    {"hexadecimal int literal", &Json::parseInts,
        "-0XABC",
        "parseInts(): invalid integer literal -0XABC at <in>:3:6"},
    {"hexadecimal unsigned long literal", &Json::parseUnsignedLongs,
        "0XABC",
        "parseUnsignedLongs(): invalid unsigned integer literal 0XABC at <in>:3:6"},
    {"hexadecimal long literal", &Json::parseLongs,
        "-0xabc",
        "parseLongs(): invalid integer literal -0xabc at <in>:3:6"},
    {"invalid unicode escape", &Json::parseStrings,
        "\"it's \\undefined\"",
        "parseStrings(): invalid unicode escape sequence \\undef at <in>:3:12"},
    {"unicode escape with 0 chars", &Json::parseStrings,
        "\"it's \\u\"",
        "parseStrings(): invalid unicode escape sequence \\u at <in>:3:12"},
    {"unicode escape with 3 chars", &Json::parseStrings,
        "\"it's \\uabc\"",
        "parseStrings(): invalid unicode escape sequence \\uabc at <in>:3:12"},
    /* UTF-16 surrogate pairs are not decoded at the moment */
    {"invalid UTF-16 surrogate", &Json::parseStrings,
        "\"\\uffff\\uffff\"",
        nullptr},
    /* These are deliberately not handled at the moment */
    {"zero byte", &Json::parseStrings,
        "\"\0\""_s,
        nullptr},
    {"unescaped newline", &Json::parseStrings,
        "\"\n\""_s, /** @todo probably also others? */
        nullptr},
    {"wrong start of a UTF-8 sequence", &Json::parseStrings,
        "\"\xb0\"", /* taken from the UnicodeTest */
        nullptr},
    {"garbage inside a UTF-8 sequence", &Json::parseStrings,
        "\"\xea\x40\xb8\"", /* taken from the UnicodeTest */
        nullptr},
    {"incomplete UTF-8 sequence", &Json::parseStrings,
        "\"\xce\"", /* taken from the UnicodeTest */
        nullptr}
};

const struct {
    const char* name;
    Json::Option option;
    const char* json;
    const char* message;
} ParseOptionErrorData[]{
    {"literals", Json::Option::ParseLiterals,
        "none",
        "parseLiterals(): invalid null literal none at <in>:1:1"},
    {"doubles", Json::Option::ParseDoubles,
        "-haha",
        "parseDoubles(): invalid floating-point literal -haha at <in>:1:1"},
    {"floats", Json::Option::ParseFloats,
        "-haha",
        "parseFloats(): invalid floating-point literal -haha at <in>:1:1"},
    {"string keys", Json::Option::ParseStringKeys,
        "{\"it's \\undefined\": null}",
        "parseStringKeys(): invalid unicode escape sequence \\undef at <in>:1:8"},
    {"strings", Json::Option::ParseStrings,
        "\"it's \\undefined\"",
        "parseStrings(): invalid unicode escape sequence \\undef at <in>:1:7"},
};

const struct {
    const char* name;
    bool(*function)(Json&);
    const char* json;
    const char* message;
} ParseSingleErrorData[]{
    {"object but an array",
        [](Json& json) { return !!json.parseObject(json.root()); },
        "[]",
        "parseObject(): expected an object, got Utility::JsonToken::Type::Array at <in>:3:6"},
    {"object but a bad key",
        [](Json& json) { return !!json.parseObject(json.root()); },
        "{\"it's \\undefined\": 3}",
        "parseObject(): invalid unicode escape sequence \\undef at <in>:3:13"},
    {"array but an object",
        [](Json& json) { return !!json.parseArray(json.root()); },
        "{}",
        "parseArray(): expected an array, got Utility::JsonToken::Type::Object at <in>:3:6"},
    {"null",
        [](Json& json) { return !!json.parseNull(json.root()); },
        "none",
        "parseNull(): invalid null literal none at <in>:3:6"},
    {"null but a numeric token",
        [](Json& json) { return !!json.parseNull(json.root()); },
        "35.7",
        "parseNull(): expected a null, got Utility::JsonToken::Type::Number at <in>:3:6"},
    {"bool",
        [](Json& json) { return !!json.parseBool(json.root()); },
        "fail",
        "parseBool(): invalid bool literal fail at <in>:3:6"},
    {"bool but a null token",
        [](Json& json) { return !!json.parseBool(json.root()); },
        "null",
        "parseBool(): expected a bool, got Utility::JsonToken::Type::Null at <in>:3:6"},
    {"double",
        [](Json& json) { return !!json.parseDouble(json.root()); },
        "75x",
        "parseDouble(): invalid floating-point literal 75x at <in>:3:6"},
    {"double but a string token",
        [](Json& json) { return !!json.parseDouble(json.root()); },
        "\"75\"",
        "parseDouble(): expected a number, got Utility::JsonToken::Type::String at <in>:3:6"},
    {"float",
        [](Json& json) { return !!json.parseFloat(json.root()); },
        "75x",
        "parseFloat(): invalid floating-point literal 75x at <in>:3:6"},
    {"float but a bool token",
        [](Json& json) { return !!json.parseFloat(json.root()); },
        "false",
        "parseFloat(): expected a number, got Utility::JsonToken::Type::Bool at <in>:3:6"},
    {"unsigned int",
        [](Json& json) { return !!json.parseUnsignedInt(json.root()); },
        "75x",
        "parseUnsignedInt(): invalid unsigned integer literal 75x at <in>:3:6"},
    {"unsigned int but a null token",
        [](Json& json) { return !!json.parseUnsignedInt(json.root()); },
        "null",
        "parseUnsignedInt(): expected a number, got Utility::JsonToken::Type::Null at <in>:3:6"},
    {"int",
        [](Json& json) { return !!json.parseInt(json.root()); },
        "75x",
        "parseInt(): invalid integer literal 75x at <in>:3:6"},
    {"int but an array token",
        [](Json& json) { return !!json.parseInt(json.root()); },
        "[]",
        "parseInt(): expected a number, got Utility::JsonToken::Type::Array at <in>:3:6"},
    {"unsigned long",
        [](Json& json) { return !!json.parseUnsignedLong(json.root()); },
        "75x",
        "parseUnsignedLong(): invalid unsigned integer literal 75x at <in>:3:6"},
    {"unsigned long but an object token",
        [](Json& json) { return !!json.parseUnsignedLong(json.root()); },
        "{}",
        "parseUnsignedLong(): expected a number, got Utility::JsonToken::Type::Object at <in>:3:6"},
    {"long",
        [](Json& json) { return !!json.parseLong(json.root()); },
        "75x",
        "parseLong(): invalid integer literal 75x at <in>:3:6"},
    {"long but a string token",
        [](Json& json) { return !!json.parseLong(json.root()); },
        "\"75\"",
        "parseLong(): expected a number, got Utility::JsonToken::Type::String at <in>:3:6"},
    {"size",
        [](Json& json) { return !!json.parseSize(json.root()); },
        "75x",
        "parseSize(): invalid unsigned integer literal 75x at <in>:3:6"},
    {"size but a bool token",
        [](Json& json) { return !!json.parseSize(json.root()); },
        "true",
        "parseSize(): expected a number, got Utility::JsonToken::Type::Bool at <in>:3:6"},
    {"string",
        [](Json& json) { return !!json.parseString(json.root()); },
        "\"it's \\undefined\"",
        "parseString(): invalid unicode escape sequence \\undef at <in>:3:12"},
    {"string but a null token",
        [](Json& json) { return !!json.parseString(json.root()); },
        "null",
        "parseString(): expected a string, got Utility::JsonToken::Type::Null at <in>:3:6"}
};

const struct {
    TestSuite::TestCaseDescriptionSourceLocation name;
    bool(*function)(Json&);
    const char* json;
    const char* message;
} ParseArrayErrorData[]{
    {"bit",
        [](Json& json) { return !!json.parseBitArray(json.root()); },
        "[true,\n   fail, false]",
        "parseBitArray(): invalid bool literal fail"},
    #ifdef CORRADE_BUILD_DEPRECATED
    {"bool, deprecated",
        [](Json& json) {
            CORRADE_IGNORE_DEPRECATED_PUSH
            return !!json.parseBoolArray(json.root());
            CORRADE_IGNORE_DEPRECATED_POP
        },
        "[true,\n   fail, false]",
        "parseBitArray(): invalid bool literal fail"},
    #endif
    {"bit but an object inside",
        [](Json& json) { return !!json.parseBitArray(json.root()); },
        "[true, false,\n   {}, true]\n",
        "parseBitArray(): expected a bool, got Utility::JsonToken::Type::Object"},
    {"bit but an object",
        [](Json& json) { return !!json.parseBitArray(json.root()); },
        "\n   {}",
        "parseBitArray(): expected an array, got Utility::JsonToken::Type::Object"},
    {"bit but unexpected size",
        [](Json& json) { return !!json.parseBitArray(json.root(), 4); },
        "\n   [true, false, true]",
        "parseBitArray(): expected a 4-element array, got 3"},
    {"double",
        [](Json& json) { return !!json.parseDoubleArray(json.root()); },
        "[5.3,\n   3.f, 4]",
        "parseDoubleArray(): invalid floating-point literal 3.f"},
    {"double but a bool inside",
        [](Json& json) { return !!json.parseDoubleArray(json.root()); },
        "[1.0, 2,\n   false, 5.5]\n",
        "parseDoubleArray(): expected a number, got Utility::JsonToken::Type::Bool"},
    {"double but a bool",
        [](Json& json) { return !!json.parseDoubleArray(json.root()); },
        "\n   true",
        "parseDoubleArray(): expected an array, got Utility::JsonToken::Type::Bool"},
    {"double but unexpected size",
        [](Json& json) { return !!json.parseDoubleArray(json.root(), 4); },
        "\n   [5.3, -3.0, 4]",
        "parseDoubleArray(): expected a 4-element array, got 3"},
    {"float",
        [](Json& json) { return !!json.parseFloatArray(json.root()); },
        "[5.3,\n   3.f, 4]",
        "parseFloatArray(): invalid floating-point literal 3.f"},
    {"float but a null inside",
        [](Json& json) { return !!json.parseFloatArray(json.root()); },
        "[1.0, 2,\n   null, 5.5]\n",
        "parseFloatArray(): expected a number, got Utility::JsonToken::Type::Null"},
    {"float but a null",
        [](Json& json) { return !!json.parseFloatArray(json.root()); },
        "\n   null",
        "parseFloatArray(): expected an array, got Utility::JsonToken::Type::Null"},
    {"float but unexpected size",
        [](Json& json) { return !!json.parseFloatArray(json.root(), 4); },
        "\n   [5.3, -3.0, 4]",
        "parseFloatArray(): expected a 4-element array, got 3"},
    {"unsigned int",
        [](Json& json) { return !!json.parseUnsignedIntArray(json.root()); },
        "[53,\n   3.f, 4]",
        "parseUnsignedIntArray(): invalid unsigned integer literal 3.f"},
    {"unsigned int but a string inside",
        [](Json& json) { return !!json.parseUnsignedIntArray(json.root()); },
        "[10, 2,\n   \"56\", 55]\n",
        "parseUnsignedIntArray(): expected a number, got Utility::JsonToken::Type::String"},
    {"unsigned int but a string",
        [](Json& json) { return !!json.parseUnsignedIntArray(json.root()); },
        "\n   \"56\"",
        "parseUnsignedIntArray(): expected an array, got Utility::JsonToken::Type::String"},
    {"unsigned int but unexpected size",
        [](Json& json) { return !!json.parseUnsignedIntArray(json.root(), 4); },
        "\n   [53, 3, 4]",
        "parseUnsignedIntArray(): expected a 4-element array, got 3"},
    {"int",
        [](Json& json) { return !!json.parseIntArray(json.root()); },
        "[-53,\n   3.f, 4]",
        "parseIntArray(): invalid integer literal 3.f"},
    {"int but a bool inside",
        [](Json& json) { return !!json.parseIntArray(json.root()); },
        "[10, -2,\n   true, 55]\n",
        "parseIntArray(): expected a number, got Utility::JsonToken::Type::Bool"},
    {"int but a bool",
        [](Json& json) { return !!json.parseIntArray(json.root()); },
        "\n   true",
        "parseIntArray(): expected an array, got Utility::JsonToken::Type::Bool"},
    {"int but unexpected size",
        [](Json& json) { return !!json.parseIntArray(json.root(), 4); },
        "\n   [-53, 3, 4]",
        "parseIntArray(): expected a 4-element array, got 3"},
    {"unsigned long",
        [](Json& json) { return !!json.parseUnsignedLongArray(json.root()); },
        "[53,\n   3.f, 4]",
        "parseUnsignedLongArray(): invalid unsigned integer literal 3.f"},
    {"unsigned long but a string inside",
        [](Json& json) { return !!json.parseUnsignedLongArray(json.root()); },
        "[10, 2,\n   \"56\", 55]\n",
        "parseUnsignedLongArray(): expected a number, got Utility::JsonToken::Type::String"},
    {"unsigned long but a string",
        [](Json& json) { return !!json.parseUnsignedLongArray(json.root()); },
        "\n   \"56\"",
        "parseUnsignedLongArray(): expected an array, got Utility::JsonToken::Type::String"},
    {"unsigned long but unexpected size",
        [](Json& json) { return !!json.parseUnsignedLongArray(json.root(), 4); },
        "\n   [53, 3, 4]",
        "parseUnsignedLongArray(): expected a 4-element array, got 3"},
    {"long",
        [](Json& json) { return !!json.parseLongArray(json.root()); },
        "[-53,\n   3.f, 4]",
        "parseLongArray(): invalid integer literal 3.f"},
    {"long but a bool inside",
        [](Json& json) { return !!json.parseLongArray(json.root()); },
        "[10, -2,\n   true, 55]\n",
        "parseLongArray(): expected a number, got Utility::JsonToken::Type::Bool"},
    {"long but a bool",
        [](Json& json) { return !!json.parseLongArray(json.root()); },
        "\n   true",
        "parseLongArray(): expected an array, got Utility::JsonToken::Type::Bool"},
    {"long but unexpected size",
        [](Json& json) { return !!json.parseLongArray(json.root(), 4); },
        "\n   [-53, 3, 4]",
        "parseLongArray(): expected a 4-element array, got 3"},
    {"size",
        [](Json& json) { return !!json.parseSizeArray(json.root()); },
        "[53,\n   3.f, 4]",
        #ifndef CORRADE_TARGET_32BIT
        "parseUnsignedLongArray(): invalid unsigned integer literal 3.f"
        #else
        "parseUnsignedIntArray(): invalid unsigned integer literal 3.f"
        #endif
        },
    {"size but a string inside",
        [](Json& json) { return !!json.parseSizeArray(json.root()); },
        "[10, 2,\n   \"56\", 55]\n",
        #ifndef CORRADE_TARGET_32BIT
        "parseUnsignedLongArray(): expected a number, got Utility::JsonToken::Type::String"
        #else
        "parseUnsignedIntArray(): expected a number, got Utility::JsonToken::Type::String"
        #endif
        },
    {"size but a string",
        [](Json& json) { return !!json.parseSizeArray(json.root()); },
        "\n   \"56\"",
        #ifndef CORRADE_TARGET_32BIT
        "parseUnsignedLongArray(): expected an array, got Utility::JsonToken::Type::String"
        #else
        "parseUnsignedIntArray(): expected an array, got Utility::JsonToken::Type::String"
        #endif
        },
    {"size but unexpected size",
        [](Json& json) { return !!json.parseSizeArray(json.root(), 4); },
        "\n   [53, 3, 4]",
        #ifndef CORRADE_TARGET_32BIT
        "parseUnsignedLongArray(): expected a 4-element array, got 3"
        #else
        "parseUnsignedIntArray(): expected a 4-element array, got 3"
        #endif
        },
    {"string",
        [](Json& json) { return !!json.parseStringArray(json.root()); },
        "[\"\",\n  \"\\undefined it is\", \"yes\"]",
        "parseStringArray(): invalid unicode escape sequence \\undef"},
    {"string but a literal inside",
        [](Json& json) { return !!json.parseStringArray(json.root()); },
        "[\"hello\", \"five\",\n   false, \"null\"]\n",
        "parseStringArray(): expected a string, got Utility::JsonToken::Type::Bool"},
    {"string but a string",
        [](Json& json) { return !!json.parseStringArray(json.root()); },
        "\n   \"yeh\"",
        "parseStringArray(): expected an array, got Utility::JsonToken::Type::String"},
    {"string but unexpected size",
        [](Json& json) { return !!json.parseStringArray(json.root(), 4); },
        "\n   [\"hello\", \",\", \"world\"]",
        "parseStringArray(): expected a 4-element array, got 3"},
};

const struct {
    TestSuite::TestCaseDescriptionSourceLocation name;
    const char* json;
    Containers::Optional<JsonToken::Type> expected;
    Containers::Optional<JsonToken::ParsedType> expectedParsed;
} CommonArrayTypeData[]{
    {"empty array",
        "[]",
        {}, {}},
    {"single string",
        R"(["hey"])",
        JsonToken::Type::String, JsonToken::ParsedType::Other},
    {"nulls",
        "[null, null, null]",
        JsonToken::Type::Null, JsonToken::ParsedType::Other},
    {"numbers",
        "[3.5, -777, 0, 1, 26]",
        /* This is the "default" numeric type used by
           JsonTest::commonArrayType(). Others are tested in
           commonArrayTypeParsedHeterogeneous() and
           commonArrayTypeParsedArrayNotParsed(). */
        JsonToken::Type::Number, JsonToken::ParsedType::Float},
    /* It's important that the nested object is first, to verify it correctly
       skips to the second array element instead of going inside the first */
    {"nested objects",
        R"([{"key": [], "another": 5}, {}, {"yes": true}])",
        JsonToken::Type::Object, JsonToken::ParsedType::Other},
    {"nested arrays", "[[[[]], 5], [], [true, 25]]",
        JsonToken::Type::Array, JsonToken::ParsedType::Other},
    {"bools + nulls",
        "[true, null, false, false]",
        {}, JsonToken::ParsedType::Other},
    {"numbers + nested arrays",
        "[25, [25, 26], 27]",
        {}, {}},
    {"bools + nested objects",
        "[false, {\"hey\": false}, true]",
        {}, JsonToken::ParsedType::Other},
};

JsonTest::JsonTest() {
    addTests({&JsonTest::singleObject,
              &JsonTest::singleArray,
              &JsonTest::singleNull,
              &JsonTest::singleBoolean,
              &JsonTest::singleNumber,
              &JsonTest::singleString,

              &JsonTest::simpleObject,
              &JsonTest::simpleArray,
              &JsonTest::nested});

    addInstancedTests({&JsonTest::error},
        Containers::arraySize(ErrorData));

    addTests({&JsonTest::errorOption});

    addInstancedTests({&JsonTest::parseObjects},
        Containers::arraySize(ParseObjectData));

    addInstancedTests({&JsonTest::parseArrays},
        Containers::arraySize(ParseArrayData));

    addInstancedTests({&JsonTest::parseNulls},
        Containers::arraySize(ParseNullData));

    addInstancedTests({&JsonTest::parseBools},
        Containers::arraySize(ParseBoolData));

    addInstancedTests({&JsonTest::parseDoubles},
        Containers::arraySize(ParseDoubleOrFloatData));

    addInstancedTests({&JsonTest::parseFloats},
        Containers::arraySize(ParseDoubleOrFloatData));

    addInstancedTests({&JsonTest::parseUnsignedInts},
        Containers::arraySize(ParseUnsignedIntData));

    addInstancedTests({&JsonTest::parseInts},
        Containers::arraySize(ParseIntData));

    addInstancedTests({&JsonTest::parseUnsignedLongs},
        Containers::arraySize(ParseUnsignedLongData));

    addInstancedTests({&JsonTest::parseLongs},
        Containers::arraySize(ParseLongData));

    addInstancedTests({&JsonTest::parseSizes},
        #ifndef CORRADE_TARGET_32BIT
        Containers::arraySize(ParseUnsignedLongData)
        #else
        Containers::arraySize(ParseUnsignedIntData)
        #endif
    );

    addInstancedTests({&JsonTest::parseStringKeys,
                       &JsonTest::parseStrings},
        Containers::arraySize(ParseStringData));

    addInstancedTests({&JsonTest::parseOption},
        Containers::arraySize(ParseOptionData));

    addInstancedTests({&JsonTest::parseSubtree},
        Containers::arraySize(ParseSubtreeData));

    addInstancedTests({&JsonTest::parseEmptyObjectOrArray},
        Containers::arraySize(EmptyObjectOrArrayData));

    addTests({&JsonTest::parseBitArray,
              #ifdef CORRADE_BUILD_DEPRECATED
              &JsonTest::parseBoolArrayDeprecated,
              #endif
              &JsonTest::parseDoubleArray,
              &JsonTest::parseFloatArray,
              &JsonTest::parseUnsignedIntArray,
              &JsonTest::parseIntArray,
              &JsonTest::parseUnsignedLongArray,
              &JsonTest::parseLongArray,
              &JsonTest::parseSizeArray});

    addInstancedTests({&JsonTest::parseStringArray},
        Containers::arraySize(ParseStringArrayData));

    addTests({&JsonTest::reparseNumberDifferentType,
              &JsonTest::reparseSingleNumberDifferentType,
              &JsonTest::reparseNumberArrayDifferentType,
              &JsonTest::reparseNumberDifferentTypeFailKeepOriginalValue,

              &JsonTest::parsedObjectChildAccess,
              &JsonTest::parsedObjectFind});

    addInstancedTests({&JsonTest::parsedObjectFindEmpty,
                       &JsonTest::parsedObjectAccessEmpty},
        Containers::arraySize(ParseObjectArrayEmptyData));

    addTests({&JsonTest::parsedArrayFind});

    addInstancedTests({&JsonTest::parsedArrayFindEmpty,
                       &JsonTest::parsedArrayAccessEmpty},
        Containers::arraySize(ParseObjectArrayEmptyData));

    addInstancedTests({&JsonTest::parseError},
        Containers::arraySize(ParseErrorData));

    addInstancedTests({&JsonTest::parseOptionError},
        Containers::arraySize(ParseOptionErrorData));

    addInstancedTests({&JsonTest::parseSingleError},
        Containers::arraySize(ParseSingleErrorData));

    addInstancedTests({&JsonTest::parseArrayError},
        Containers::arraySize(ParseArrayErrorData));

    addTests({&JsonTest::parseTokenNotOwned,

              &JsonTest::tokenFromTokenData,
              &JsonTest::tokenFromTokenDataNotOwned,

              &JsonTest::iterator,
              &JsonTest::iteratorInvalid,
              &JsonTest::iteratorObject,
              &JsonTest::iteratorObjectInvalid,
              &JsonTest::iteratorArray,
              &JsonTest::iteratorArrayInvalid,

              &JsonTest::iterate,
              &JsonTest::iterateOutOfRange,
              &JsonTest::iterateObject,
              &JsonTest::iterateObjectTokens,
              &JsonTest::iterateObjectNotObject,
              &JsonTest::iterateObjectNotParsed,
              &JsonTest::iterateObjectKeyNotParsed,
              &JsonTest::iterateArray,
              &JsonTest::iterateArrayTokens,
              &JsonTest::iterateArrayNotArray,
              &JsonTest::iterateArrayNotParsed,

              &JsonTest::findObjectKey,
              &JsonTest::findObjectKeyNotFound,
              &JsonTest::findObjectKeyNotObject,
              &JsonTest::findObjectKeyNotParsed,
              &JsonTest::findObjectKeyKeyNotParsed,
              &JsonTest::findArrayIndex,
              &JsonTest::findArrayIndexNotFound,
              &JsonTest::findArrayIndexNotArray,
              &JsonTest::findArrayIndexNotParsed});

    addInstancedTests({&JsonTest::commonArrayType},
        Containers::arraySize(CommonArrayTypeData));

    addTests({&JsonTest::commonArrayTypeParsedHeterogeneous,
              &JsonTest::commonArrayTypeParsedArrayNotParsed,
              &JsonTest::commonArrayTypeNotArray,

              &JsonTest::asTypeWrongType,
              &JsonTest::asTypeNotParsed,
              &JsonTest::asTypeWrongParsedType,

              &JsonTest::asBitArray,
              #ifdef CORRADE_BUILD_DEPRECATED
              &JsonTest::asBoolArrayDeprecated,
              #endif
              &JsonTest::asBitArrayNotAllSame,
              &JsonTest::asBitArrayNotAllParsed,
              &JsonTest::asBitArrayUnexpectedSize,
              &JsonTest::asDoubleArray,
              &JsonTest::asDoubleArrayNotAllSame,
              &JsonTest::asDoubleArrayUnexpectedSize,
              &JsonTest::asFloatArray,
              &JsonTest::asFloatArrayNotAllSame,
              &JsonTest::asFloatArrayUnexpectedSize,
              &JsonTest::asUnsignedIntArray,
              &JsonTest::asUnsignedIntArrayNotAllSame,
              &JsonTest::asUnsignedIntArrayUnexpectedSize,
              &JsonTest::asIntArray,
              &JsonTest::asIntArrayNotAllSame,
              &JsonTest::asIntArrayUnexpectedSize,
              &JsonTest::asUnsignedLongArray,
              &JsonTest::asUnsignedLongArrayNotAllSame,
              &JsonTest::asUnsignedLongArrayUnexpectedSize,
              &JsonTest::asLongArray,
              &JsonTest::asLongArrayNotAllSame,
              &JsonTest::asLongArrayUnexpectedSize,
              &JsonTest::asSizeArray,
              &JsonTest::asSizeArrayNotAllSame,
              &JsonTest::asSizeArrayUnexpectedSize});

    addInstancedTests({&JsonTest::asStringArray},
        Containers::arraySize(ParseStringArrayData));

    addTests({&JsonTest::asStringArrayNotAllSame,
              &JsonTest::asStringArrayNotAllParsed,
              &JsonTest::asStringArrayUnexpectedSize,
              &JsonTest::asTypeArrayNotArray,
              &JsonTest::asTypeArrayNotParsed,

              &JsonTest::fromStringFilenameOffsetError,
              &JsonTest::fromStringFilenameOffsetErrorSubsequentLine,
              &JsonTest::fromStringFilenameOffsetParseOptionError,
              &JsonTest::fromStringFilenameOffsetParseError,

              &JsonTest::fromFile,
              &JsonTest::fromFileReadError,
              &JsonTest::fromFileOptionReadError,
              &JsonTest::fromFileError,
              &JsonTest::fromFileParseOptionError,
              &JsonTest::fromFileParseError,

              #ifndef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
              &JsonTest::tokenConstructCopy,
              #endif
              &JsonTest::tokenDataConstructInvalid,
              #ifndef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
              &JsonTest::tokenDataConstructCopy,
              &JsonTest::tokenOffsetSizeConstructCopy,
              #endif
              &JsonTest::constructCopy,
              &JsonTest::constructMove,

              &JsonTest::debugTokenType,
              &JsonTest::debugTokenParsedType,

              &JsonTest::fromTokenDataNull,
              &JsonTest::fromTokenDataBool,
              &JsonTest::fromTokenDataDouble,
              &JsonTest::fromTokenDataFloat,
              &JsonTest::fromTokenDataUnsignedInt,
              &JsonTest::fromTokenDataInt,
              &JsonTest::fromTokenDataUnsignedLong,
              &JsonTest::fromTokenDataLong,
              &JsonTest::fromTokenDataTheOtherUnsignedLongType,
              &JsonTest::fromTokenDataTheOtherLongType,
              &JsonTest::fromTokenDataSize,
              &JsonTest::fromTokenDataString,
              &JsonTest::fromTokenDataStringEscaped,
              &JsonTest::fromTokenDataArrayEmpty,
              &JsonTest::fromTokenDataArray,
              &JsonTest::fromTokenDataObjectEmpty,
              &JsonTest::fromTokenDataObject,
              &JsonTest::fromTokenDataNested,
              &JsonTest::fromTokenDataNoTokens,
              &JsonTest::fromTokenDataInvalidTokenArraySizes,
              &JsonTest::fromTokenDataTokenOffsetSizeOutOfBounds,
              &JsonTest::fromTokenDataStringTokenDataSizeTooSmall,
              &JsonTest::fromTokenDataEscapedStringOutOfBounds,
              &JsonTest::fromTokenDataObjectKeyWithNoValue,
              &JsonTest::fromTokenDataObjectKeyNotString,
              &JsonTest::fromTokenDataObjectStringNotKey,
              &JsonTest::fromTokenDataStringValueKey,
              &JsonTest::fromTokenDataInvalidObjectChildCount,
              &JsonTest::fromTokenDataInvalidArrayChildCount,
              &JsonTest::fromTokenDataInvalidNestedObjectChildCount,
              &JsonTest::fromTokenDataInvalidNestedArrayChildCount,
              &JsonTest::fromTokenDataExtraRootTokens,
              &JsonTest::fromTokenDataParseEmpty});
}

void JsonTest::error() {
    auto&& data = ErrorData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    Containers::String out;
    Error redirectError{&out};
    CORRADE_VERIFY(!Json::fromString(data.data));
    CORRADE_COMPARE(out, format("Utility::Json: {}\n", data.message));
}

void JsonTest::errorOption() {
    /* The particular corner cases got all tested in error(), here just
       verifying that the tokenization error gets correctly propagated also
       when using the overload with (empty) Json::Option (which is a separate
       codepath to allow for the parsing code to be DCEd if not used). Parsing
       errors when using Option are tested in parseOptionError(). */

    Containers::String out;
    Error redirectError{&out};
    CORRADE_VERIFY(!Json::fromString("]", Json::Options{}));
    CORRADE_COMPARE(out, "Utility::Json: expected a value but got ] at <in>:1:1\n");
}

void JsonTest::singleObject() {
    Containers::Optional<Json> json = Json::fromString(" {  \n \r  } ");
    CORRADE_VERIFY(json);
    CORRADE_COMPARE(json->tokens().size(), 1);

    JsonToken object = json->tokens()[0];
    CORRADE_COMPARE(&json->root().token(), &object.token());
    CORRADE_COMPARE(object.data(), "{  \n \r  }");
    CORRADE_COMPARE(object.type(), JsonToken::Type::Object);
    CORRADE_VERIFY(!object.isParsed());
    CORRADE_COMPARE(object.childCount(), 0);
    CORRADE_COMPARE(object.children().size(), 0);
    CORRADE_VERIFY(!object.firstChild());
    CORRADE_VERIFY(object.next() == json->tokens().end());
    CORRADE_VERIFY(!object.parent());
}

void JsonTest::singleArray() {
    Containers::Optional<Json> json = Json::fromString(" [  \n \r  ] ");
    CORRADE_VERIFY(json);
    CORRADE_COMPARE(json->tokens().size(), 1);

    JsonToken array = json->tokens()[0];
    CORRADE_COMPARE(&json->root().token(), &array.token());
    CORRADE_COMPARE(array.data(), "[  \n \r  ]");
    CORRADE_COMPARE(array.type(), JsonToken::Type::Array);
    CORRADE_VERIFY(!array.isParsed());
    CORRADE_COMPARE(array.childCount(), 0);
    CORRADE_COMPARE(array.children().size(), 0);
    CORRADE_VERIFY(!array.firstChild());
    CORRADE_VERIFY(array.next() == json->tokens().end());
    CORRADE_VERIFY(!array.parent());
}

void JsonTest::singleNull() {
    /* Detects only the first letter and assumes sanity by default */
    Containers::Optional<Json> json = Json::fromString(" nULLtotallyinvalidyes\n ");
    CORRADE_VERIFY(json);
    CORRADE_COMPARE(json->tokens().size(), 1);

    JsonToken null = json->tokens()[0];
    CORRADE_COMPARE(&json->root().token(), &null.token());
    CORRADE_COMPARE(null.data(), "nULLtotallyinvalidyes");
    CORRADE_COMPARE(null.type(), JsonToken::Type::Null);
    CORRADE_VERIFY(!null.isParsed());
    CORRADE_COMPARE(null.childCount(), 0);
    CORRADE_COMPARE(null.children().size(), 0);
    CORRADE_VERIFY(!null.firstChild());
    CORRADE_VERIFY(null.next() == json->tokens().end());
    CORRADE_VERIFY(!null.parent());
}

void JsonTest::singleBoolean() {
    /* Detects only the first letter and assumes sanity by default */
    Containers::Optional<Json> json = Json::fromString(" fALsetotallyinvalidyes\n ");
    CORRADE_VERIFY(json);
    CORRADE_COMPARE(json->tokens().size(), 1);

    JsonToken boolean = json->tokens()[0];
    CORRADE_COMPARE(&json->root().token(), &boolean.token());
    CORRADE_COMPARE(boolean.data(), "fALsetotallyinvalidyes");
    CORRADE_COMPARE(boolean.type(), JsonToken::Type::Bool);
    CORRADE_VERIFY(!boolean.isParsed());
    CORRADE_COMPARE(boolean.childCount(), 0);
    CORRADE_COMPARE(boolean.children().size(), 0);
    CORRADE_VERIFY(!boolean.firstChild());
    CORRADE_VERIFY(boolean.next() == json->tokens().end());
    CORRADE_VERIFY(!boolean.parent());
}

void JsonTest::singleNumber() {
    /* Detects only the first letter and assumes sanity by default */
    Containers::Optional<Json> json = Json::fromString(" -hahahahah\n ");
    CORRADE_VERIFY(json);
    CORRADE_COMPARE(json->tokens().size(), 1);

    JsonToken number = json->tokens()[0];
    CORRADE_COMPARE(&json->root().token(), &number.token());
    CORRADE_COMPARE(number.data(), "-hahahahah");
    CORRADE_COMPARE(number.type(), JsonToken::Type::Number);
    CORRADE_VERIFY(!number.isParsed());
    CORRADE_COMPARE(number.childCount(), 0);
    CORRADE_COMPARE(number.children().size(), 0);
    CORRADE_VERIFY(!number.firstChild());
    CORRADE_VERIFY(number.next() == json->tokens().end());
    CORRADE_VERIFY(!number.parent());
}

void JsonTest::singleString() {
    /* Assumes sanity of unicode escapes by default */
    Containers::Optional<Json> json = Json::fromString(" \"\\uNICODE yay\\\"\" \n ");
    CORRADE_VERIFY(json);
    CORRADE_COMPARE(json->tokens().size(), 1);

    JsonToken string = json->tokens()[0];
    CORRADE_COMPARE(&json->root().token(), &string.token());
    CORRADE_COMPARE(string.data(), "\"\\uNICODE yay\\\"\"");
    CORRADE_COMPARE(string.type(), JsonToken::Type::String);
    CORRADE_VERIFY(!string.isParsed());
    CORRADE_COMPARE(string.childCount(), 0);
    CORRADE_COMPARE(string.children().size(), 0);
    CORRADE_VERIFY(!string.firstChild());
    CORRADE_VERIFY(string.next() == json->tokens().end());
    CORRADE_VERIFY(!string.parent());
}

void JsonTest::simpleObject() {
    Containers::Optional<Json> json = Json::fromString(R"(
        {"key1": "hello",
         "key2":null,
         "key3"   :-375.26e5,
         "key4":   0,
         "key5": false,
         "key6" : "abc",
         "key7": [],
         "key8": true}
    )");
    CORRADE_VERIFY(json);
    CORRADE_COMPARE(json->tokens().size(), 17);

    /* Verify data */
    JsonToken object = json->tokens()[0];
    /* GCC 4.8 can't handle raw string literals in macros */
    const char* objectData = R"({"key1": "hello",
         "key2":null,
         "key3"   :-375.26e5,
         "key4":   0,
         "key5": false,
         "key6" : "abc",
         "key7": [],
         "key8": true})";
    CORRADE_COMPARE(object.data(), objectData);
    CORRADE_COMPARE(object.type(), JsonToken::Type::Object);

    JsonToken string1 = json->tokens()[2];
    CORRADE_COMPARE(string1.data(), "\"hello\"");
    CORRADE_COMPARE(string1.type(), JsonToken::Type::String);

    JsonToken null = json->tokens()[4];
    CORRADE_COMPARE(null.data(), "null");
    CORRADE_COMPARE(null.type(), JsonToken::Type::Null);

    JsonToken number1 = json->tokens()[6];
    CORRADE_COMPARE(number1.data(), "-375.26e5");
    CORRADE_COMPARE(number1.type(), JsonToken::Type::Number);

    JsonToken number2 = json->tokens()[8];
    CORRADE_COMPARE(number2.data(), "0");
    CORRADE_COMPARE(number2.type(), JsonToken::Type::Number);

    JsonToken bool1 = json->tokens()[10];
    CORRADE_COMPARE(bool1.data(), "false");
    CORRADE_COMPARE(bool1.type(), JsonToken::Type::Bool);

    JsonToken string2 = json->tokens()[12];
    CORRADE_COMPARE(string2.data(), "\"abc\"");
    CORRADE_COMPARE(string2.type(), JsonToken::Type::String);

    JsonToken array = json->tokens()[14];
    CORRADE_COMPARE(array.data(), "[]");
    CORRADE_COMPARE(array.type(), JsonToken::Type::Array);

    JsonToken bool2 = json->tokens()[16];
    CORRADE_COMPARE(bool2.data(), "true");
    CORRADE_COMPARE(bool2.type(), JsonToken::Type::Bool);

    /* No tokens should be parsed */
    for(JsonToken i: json->tokens()) {
        CORRADE_ITERATION(i.data());
        CORRADE_VERIFY(!i.isParsed());
    }

    /* Verify keys */
    for(std::size_t i = 0; i != 8; ++i) {
        CORRADE_ITERATION(i);
        JsonToken key = json->tokens()[1 + 2*i];
        /* Unlike objects and arrays, the key token data don't contain the
           nested value even the value is a child */
        CORRADE_COMPARE(key.data(), format("\"key{}\"", i + 1));
        CORRADE_COMPARE(key.type(), JsonToken::Type::String);
        CORRADE_VERIFY(!key.isParsed());
    }

    /* Verify traversal */
    CORRADE_COMPARE(object.childCount(), 16);
    CORRADE_VERIFY(object.firstChild());
    CORRADE_VERIFY(object.firstChild()->firstChild() == string1);
    CORRADE_COMPARE(object.children().size(), 16);
    CORRADE_COMPARE(&object.children().front().children().front().token(), &string1.token());
    CORRADE_COMPARE(&object.children().back().token(), &bool2.token());
    CORRADE_VERIFY(object.next() == json->tokens().end());
    CORRADE_VERIFY(!object.parent());

    /* The object values should ... */
    JsonIterator prevKey;
    JsonIterator prevValue;
    for(JsonIterator key = object.firstChild(); key != json->tokens().end(); key = key->next()) {
        CORRADE_ITERATION(key->data());
        /* Have exactly one child */
        CORRADE_COMPARE(key->childCount(), 1);
        CORRADE_COMPARE(key->children().size(), 1);
        /* All the same parent */
        CORRADE_VERIFY(key->parent() == object);
        /* Next should always point to the key */
        if(prevKey) CORRADE_VERIFY(prevKey->next() == key);
        prevKey = key;

        /* The value having no nested children */
        JsonIterator value = key->firstChild();
        CORRADE_VERIFY(value);
        CORRADE_COMPARE(value->childCount(), 0);
        CORRADE_COMPARE(value->children().size(), 0);
        /* Key being the parent */
        CORRADE_VERIFY(value->parent() == key);
        /* Next should always point to the next key */
        if(prevValue) CORRADE_VERIFY(prevValue->next() == key);
        prevValue = value;
    }
    CORRADE_VERIFY(prevValue == json->tokens().back());
}

void JsonTest::simpleArray() {
    Containers::Optional<Json> json = Json::fromString(R"(
        ["hello", null,   -375.26e5,0,   false, "abc",{}, true ]
    )");
    CORRADE_VERIFY(json);
    CORRADE_COMPARE(json->tokens().size(), 9);

    /* Verify data */
    JsonToken array = json->tokens()[0];
    /* GCC 4.8 can't handle raw string literals in macros */
    const char* arrayData = R"(["hello", null,   -375.26e5,0,   false, "abc",{}, true ])";
    CORRADE_COMPARE(array.data(), arrayData);
    CORRADE_COMPARE(array.type(), JsonToken::Type::Array);

    JsonToken string1 = json->tokens()[1];
    CORRADE_COMPARE(string1.data(), "\"hello\"");
    CORRADE_COMPARE(string1.type(), JsonToken::Type::String);

    JsonToken null = json->tokens()[2];
    CORRADE_COMPARE(null.data(), "null");
    CORRADE_COMPARE(null.type(), JsonToken::Type::Null);

    JsonToken number1 = json->tokens()[3];
    CORRADE_COMPARE(number1.data(), "-375.26e5");
    CORRADE_COMPARE(number1.type(), JsonToken::Type::Number);

    JsonToken number2 = json->tokens()[4];
    CORRADE_COMPARE(number2.data(), "0");
    CORRADE_COMPARE(number2.type(), JsonToken::Type::Number);

    JsonToken bool1 = json->tokens()[5];
    CORRADE_COMPARE(bool1.data(), "false");
    CORRADE_COMPARE(bool1.type(), JsonToken::Type::Bool);

    JsonToken string2 = json->tokens()[6];
    CORRADE_COMPARE(string2.data(), "\"abc\"");
    CORRADE_COMPARE(string2.type(), JsonToken::Type::String);

    JsonToken object = json->tokens()[7];
    CORRADE_COMPARE(object.data(), "{}");
    CORRADE_COMPARE(object.type(), JsonToken::Type::Object);

    JsonToken bool2 = json->tokens()[8];
    CORRADE_COMPARE(bool2.data(), "true");
    CORRADE_COMPARE(bool2.type(), JsonToken::Type::Bool);

    /* No tokens should be parsed */
    for(JsonToken i: json->tokens()) {
        CORRADE_ITERATION(i.data());
        CORRADE_VERIFY(!i.isParsed());
    }

    /* Verify traversal */
    CORRADE_COMPARE(array.childCount(), 8);
    CORRADE_VERIFY(array.firstChild() == string1);
    CORRADE_COMPARE(array.children().size(), 8);
    CORRADE_COMPARE(&array.children().front().token(), &string1.token());
    CORRADE_COMPARE(&array.children().back().token(), &bool2.token());
    CORRADE_VERIFY(array.next() == json->tokens().end());
    CORRADE_VERIFY(!array.parent());

    /* The array children should ... */
    const JsonTokenData* prev = nullptr;
    for(JsonToken i: array.children()) {
        CORRADE_ITERATION(i.data());
        /* Have no children */
        CORRADE_COMPARE(i.childCount(), 0);
        CORRADE_COMPARE(i.children().size(), 0);
        CORRADE_VERIFY(!i.firstChild());
        /* All the same parent */
        CORRADE_VERIFY(i.parent() == array);
        /* Next should always point to ... the next */
        if(prev) CORRADE_VERIFY(JsonToken{*json, *prev}.next() == i);
        prev = &i.token();
    }
    CORRADE_COMPARE(prev, &json->tokens().back().token());
}

void JsonTest::nested() {
    Containers::Optional<Json> json = Json::fromString(R"(
        [{"hello": 5,
          "yes": true,
          "matrix": [[0, 1],
                     [2, 3]],
          "braces": {"again": {}}},
          -15.75,
          "bye!",
          []]
    )");
    CORRADE_VERIFY(json);
    CORRADE_COMPARE(json->tokens().size(), 21);

    /* Verify data */
    JsonToken array = json->tokens()[0];
    /* GCC 4.8 can't handle raw string literals in macros */
    const char* arrayData = R"([{"hello": 5,
          "yes": true,
          "matrix": [[0, 1],
                     [2, 3]],
          "braces": {"again": {}}},
          -15.75,
          "bye!",
          []])";
    CORRADE_COMPARE(array.data(), arrayData);
    CORRADE_COMPARE(array.type(), JsonToken::Type::Array);

    JsonToken object = json->tokens()[1];
    /* GCC 4.8 can't handle raw string literals in macros */
    const char* objectData = R"({"hello": 5,
          "yes": true,
          "matrix": [[0, 1],
                     [2, 3]],
          "braces": {"again": {}}})";
    CORRADE_COMPARE(object.data(), objectData);
    CORRADE_COMPARE(object.type(), JsonToken::Type::Object);

    JsonToken hello = json->tokens()[2];
    CORRADE_COMPARE(hello.data(), "\"hello\"");
    CORRADE_COMPARE(hello.type(), JsonToken::Type::String);

    JsonToken five = json->tokens()[3];
    CORRADE_COMPARE(five.data(), "5");
    CORRADE_COMPARE(five.type(), JsonToken::Type::Number);

    JsonToken yes = json->tokens()[4];
    CORRADE_COMPARE(yes.data(), "\"yes\"");
    CORRADE_COMPARE(yes.type(), JsonToken::Type::String);

    JsonToken true_ = json->tokens()[5];
    CORRADE_COMPARE(true_.data(), "true");
    CORRADE_COMPARE(true_.type(), JsonToken::Type::Bool);

    JsonToken matrix = json->tokens()[6];
    CORRADE_COMPARE(matrix.data(), "\"matrix\"");
    CORRADE_COMPARE(matrix.type(), JsonToken::Type::String);

    JsonToken matrixArray1 = json->tokens()[7];
    /* GCC 4.8 can't handle raw string literals in macros */
    const char* matrixArray1Data = R"([[0, 1],
                     [2, 3]])";
    CORRADE_COMPARE(matrixArray1.data(), matrixArray1Data);
    CORRADE_COMPARE(matrixArray1.type(), JsonToken::Type::Array);

    JsonToken matrixArray2 = json->tokens()[8];
    CORRADE_COMPARE(matrixArray2.data(), "[0, 1]");
    CORRADE_COMPARE(matrixArray2.type(), JsonToken::Type::Array);

    JsonToken zero = json->tokens()[9];
    CORRADE_COMPARE(zero.data(), "0");
    CORRADE_COMPARE(zero.type(), JsonToken::Type::Number);

    JsonToken one = json->tokens()[10];
    CORRADE_COMPARE(one.data(), "1");
    CORRADE_COMPARE(one.type(), JsonToken::Type::Number);

    JsonToken matrixArray3 = json->tokens()[11];
    CORRADE_COMPARE(matrixArray3.data(), "[2, 3]");
    CORRADE_COMPARE(matrixArray3.type(), JsonToken::Type::Array);

    JsonToken two = json->tokens()[12];
    CORRADE_COMPARE(two.data(), "2");
    CORRADE_COMPARE(two.type(), JsonToken::Type::Number);

    JsonToken three = json->tokens()[13];
    CORRADE_COMPARE(three.data(), "3");
    CORRADE_COMPARE(three.type(), JsonToken::Type::Number);

    JsonToken braces = json->tokens()[14];
    CORRADE_COMPARE(braces.data(), "\"braces\"");
    CORRADE_COMPARE(braces.type(), JsonToken::Type::String);

    JsonToken bracesObject = json->tokens()[15];
    CORRADE_COMPARE(bracesObject.data(), "{\"again\": {}}");
    CORRADE_COMPARE(bracesObject.type(), JsonToken::Type::Object);

    JsonToken again = json->tokens()[16];
    CORRADE_COMPARE(again.data(), "\"again\"");
    CORRADE_COMPARE(again.type(), JsonToken::Type::String);

    JsonToken emptyObject = json->tokens()[17];
    CORRADE_COMPARE(emptyObject.data(), "{}");
    CORRADE_COMPARE(emptyObject.type(), JsonToken::Type::Object);

    JsonToken number = json->tokens()[18];
    CORRADE_COMPARE(number.data(), "-15.75");
    CORRADE_COMPARE(number.type(), JsonToken::Type::Number);

    JsonToken bye = json->tokens()[19];
    CORRADE_COMPARE(bye.data(), "\"bye!\"");
    CORRADE_COMPARE(bye.type(), JsonToken::Type::String);

    JsonToken emptyArray = json->tokens()[20];
    CORRADE_COMPARE(emptyArray.data(), "[]");
    CORRADE_COMPARE(emptyArray.type(), JsonToken::Type::Array);

    /* No tokens should be parsed */
    for(JsonToken i: json->tokens()) {
        CORRADE_ITERATION(i.data());
        CORRADE_VERIFY(!i.isParsed());
    }

    /* Verify child counts */
    CORRADE_COMPARE(array.childCount(), 20);
    CORRADE_COMPARE(object.childCount(), 16);
    CORRADE_COMPARE(matrix.childCount(), 7);
    CORRADE_COMPARE(matrixArray1.childCount(), 6);
    CORRADE_COMPARE(matrixArray2.childCount(), 2);
    CORRADE_COMPARE(matrixArray3.childCount(), 2);
    CORRADE_COMPARE(braces.childCount(), 3);
    CORRADE_COMPARE(bracesObject.childCount(), 2);
    for(JsonToken key: {hello, yes, again}) {
        CORRADE_ITERATION(key.data());
        CORRADE_COMPARE(key.childCount(), 1);
    }
    for(JsonToken value: {three, true_, zero, one, two, three, number, emptyObject, bye, emptyArray}) {
        CORRADE_ITERATION(value.data());
        CORRADE_COMPARE(value.childCount(), 0);
    }

    /* Verify first childs */
    CORRADE_VERIFY(array.firstChild() == object);
    CORRADE_VERIFY(object.firstChild() == hello);
    CORRADE_VERIFY(hello.firstChild() == five);
    CORRADE_VERIFY(yes.firstChild() == true_);
    CORRADE_VERIFY(matrix.firstChild() == matrixArray1);
    CORRADE_VERIFY(matrixArray1.firstChild() == matrixArray2);
    CORRADE_VERIFY(matrixArray2.firstChild() == zero);
    CORRADE_VERIFY(matrixArray3.firstChild() == two);
    CORRADE_VERIFY(braces.firstChild() == bracesObject);
    CORRADE_VERIFY(bracesObject.firstChild() == again);
    CORRADE_VERIFY(again.firstChild() == emptyObject);

    /* Verify next tokens */
    CORRADE_VERIFY(array.next() == json->tokens().end());
    CORRADE_VERIFY(object.next() == number);
    CORRADE_VERIFY(hello.next() == yes);
    CORRADE_VERIFY(yes.next() == matrix);
    CORRADE_VERIFY(matrix.next() == braces);
    CORRADE_VERIFY(matrixArray1.next() == braces);
    CORRADE_VERIFY(matrixArray2.next() == matrixArray3);
    CORRADE_VERIFY(matrixArray3.next() == braces);
    CORRADE_VERIFY(braces.next() == number);
    CORRADE_VERIFY(bracesObject.next() == number);
    CORRADE_VERIFY(again.next() == number);
    CORRADE_VERIFY(emptyObject.next() == number);
    CORRADE_VERIFY(number.next() == bye);
    CORRADE_VERIFY(bye.next() == emptyArray);
    CORRADE_VERIFY(emptyArray.next() == json->tokens().end());
}

void JsonTest::parseObjects() {
    auto&& data = ParseObjectData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    Containers::String jsonData = "{\"a\": [[\"b\"]], \"c\": 3, \"d\": {\"e\": {}}}";
    /* This constructor has ArrayView as the first argument, thus losing all
       flags from the input */
    Containers::Optional<Json> json = Json::fromString({jsonData, data.flags});
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(!json->root().isParsed());
    CORRADE_COMPARE(json->root().type(), JsonToken::Type::Object);
    CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::None);
    CORRADE_COMPARE(json->root().data(), jsonData);
    /* If global, should point to the original string */
    if(data.flags & Containers::StringViewFlag::Global)
        CORRADE_COMPARE(json->root().data().data(), static_cast<const void*>(jsonData.data()));
    /* Token data should also inherit the Global flag, and potentially
       NullTerminated if the input is null-terminated */
    CORRADE_COMPARE(json->root().data().flags(), data.flags);

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);
        if(!data.singleValue) {
            CORRADE_VERIFY(json->parseLiterals(json->root()));

            /* The keys should not be parsed */
            CORRADE_VERIFY(!json->tokens()[1].isParsed());
            CORRADE_VERIFY(!json->tokens()[5].isParsed());
            CORRADE_VERIFY(!json->tokens()[7].isParsed());
            CORRADE_VERIFY(!json->tokens()[9].isParsed());
        } else {
            Containers::Optional<JsonObjectView> out = json->parseObject(json->root());
            CORRADE_VERIFY(out);

            /* Direct keys should be parsed, nested keys not */
            CORRADE_VERIFY(json->tokens()[1].isParsed());
            CORRADE_VERIFY(json->tokens()[5].isParsed());
            CORRADE_VERIFY(json->tokens()[7].isParsed());
            CORRADE_VERIFY(!json->tokens()[9].isParsed());

            /* The view should span the whole object */
            Containers::Array<Containers::StringView> keys;
            for(JsonObjectItem i: *out)
                arrayAppend(keys, i.key());
            CORRADE_COMPARE_AS(keys, Containers::arrayView({
                "a"_s, "c"_s, "d"_s
            }), TestSuite::Compare::Container);
        }

        /* The token data should not get corrupted by this */
        CORRADE_VERIFY(json->root().isParsed());
        CORRADE_COMPARE(json->root().type(), JsonToken::Type::Object);
        CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::Other);
        CORRADE_COMPARE(json->root().data(), jsonData);
        if(data.flags & Containers::StringViewFlag::Global)
            CORRADE_COMPARE(json->root().data().data(), static_cast<const void*>(jsonData.data()));
        /* Token data should still inherit the Global flag even after parsing */
        CORRADE_COMPARE(json->root().data().flags(), data.flags);
    }
}

void JsonTest::parseArrays() {
    auto&& data = ParseArrayData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    Containers::String jsonData = "[\"a\", [{}], 3]";
    /* This constructor has ArrayView as the first argument, thus losing all
       flags from the input */
    Containers::Optional<Json> json = Json::fromString({jsonData, data.flags});
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(!json->root().isParsed());
    CORRADE_COMPARE(json->root().type(), JsonToken::Type::Array);
    CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::None);
    CORRADE_COMPARE(json->root().data(), jsonData);
    /* If global, should point to the original string */
    if(data.flags & Containers::StringViewFlag::Global)
        CORRADE_COMPARE(json->root().data().data(), static_cast<const void*>(jsonData.data()));
    /* Token data should also inherit the Global flag, and potentially
       NullTerminated if the input is null-terminated */
    CORRADE_COMPARE(json->root().data().flags(), data.flags);

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);
        if(!data.singleValue) {
            CORRADE_VERIFY(json->parseLiterals(json->root()));
        } else {
            Containers::Optional<JsonArrayView> out = json->parseArray(json->root());
            CORRADE_VERIFY(out);

            /* The view should span the whole array */
            Containers::Array<Containers::StringView> tokens;
            for(JsonArrayItem i: *out)
                arrayAppend(tokens, i.value().data());
            CORRADE_COMPARE_AS(tokens, Containers::arrayView({
                "\"a\""_s, "[{}]"_s, "3"_s
            }), TestSuite::Compare::Container);
        }

        /* The token data should not get corrupted by this */
        CORRADE_VERIFY(json->root().isParsed());
        CORRADE_COMPARE(json->root().type(), JsonToken::Type::Array);
        CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::Other);
        CORRADE_COMPARE(json->root().data(), jsonData);
        if(data.flags & Containers::StringViewFlag::Global)
            CORRADE_COMPARE(json->root().data().data(), static_cast<const void*>(jsonData.data()));
        /* Token data should still inherit the Global flag even after parsing */
        CORRADE_COMPARE(json->root().data().flags(), data.flags);
    }
}

void JsonTest::parseNulls() {
    auto&& data = ParseNullData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    Containers::String jsonData = "null";
    /* This constructor has ArrayView as the first argument, thus losing all
       flags from the input */
    Containers::Optional<Json> json = Json::fromString({jsonData, data.flags});
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(!json->root().isParsed());
    CORRADE_COMPARE(json->root().type(), JsonToken::Type::Null);
    CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::None);
    /* If global, should point to the original string so we can change it
       below */
    if(data.flags & Containers::StringViewFlag::Global)
        CORRADE_COMPARE(json->root().data().data(), static_cast<void*>(jsonData.data()));
    /* Token data should also inherit the Global flag, and potentially
       NullTerminated if the input is null-terminated */
    CORRADE_COMPARE(json->root().data().flags(), data.flags);

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);
        if(!data.singleValue)
            CORRADE_VERIFY(json->parseLiterals(json->root()));
        else
            CORRADE_COMPARE(json->parseNull(json->root()), nullptr);

        /* The token data should not get corrupted by this */
        CORRADE_VERIFY(json->root().isParsed());
        CORRADE_COMPARE(json->root().type(), JsonToken::Type::Null);
        CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::Other);
        CORRADE_COMPARE(json->root().data(), jsonData);
        if(data.flags & Containers::StringViewFlag::Global)
            CORRADE_COMPARE(json->root().data().data(), static_cast<void*>(jsonData.data()));
        /* Token data should still inherit the Global flag even after parsing */
        CORRADE_COMPARE(json->root().data().flags(), data.flags);
        CORRADE_COMPARE(json->root().asNull(), nullptr);

        /* Corrupt the original string, if global. Next time it should use the
           cached value. */
        if(data.flags & Containers::StringViewFlag::Global)
            jsonData[0] = 'x';
    }
}

void JsonTest::parseBools() {
    auto&& data = ParseBoolData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    Containers::String jsonData = data.json;
    /* This constructor has ArrayView as the first argument, thus losing all
       flags from the input */
    Containers::Optional<Json> json = Json::fromString({jsonData, data.flags});
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(!json->root().isParsed());
    CORRADE_COMPARE(json->root().type(), JsonToken::Type::Bool);
    CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::None);
    /* If global, should point to the original string so we can change it
       below */
    if(data.flags & Containers::StringViewFlag::Global)
        CORRADE_COMPARE(json->root().data().data(), static_cast<void*>(jsonData.data()));
    /* Token data should also inherit the Global flag, and potentially
       NullTerminated if the input is null-terminated */
    CORRADE_COMPARE(json->root().data().flags(), data.flags);

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);
        if(!data.singleValue)
            CORRADE_VERIFY(json->parseLiterals(json->root()));
        else
            CORRADE_COMPARE(json->parseBool(json->root()), data.expected);

        /* The token data should not get corrupted by this */
        CORRADE_VERIFY(json->root().isParsed());
        CORRADE_COMPARE(json->root().type(), JsonToken::Type::Bool);
        CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::Other);
        CORRADE_COMPARE(json->root().data(), jsonData);
        if(data.flags & Containers::StringViewFlag::Global)
            CORRADE_COMPARE(json->root().data().data(), static_cast<void*>(jsonData.data()));
        /* Token data should still inherit the Global flag even after parsing */
        CORRADE_COMPARE(json->root().data().flags(), data.flags);
        CORRADE_COMPARE(json->root().asBool(), data.expected);

        /* Corrupt the original string, if global. Next time it should use the
           cached value. */
        if(data.flags & Containers::StringViewFlag::Global)
            jsonData[0] = 'x';
    }
}

void JsonTest::parseDoubles() {
    auto&& data = ParseDoubleOrFloatData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    Containers::String jsonData = data.json;
    /* This constructor has ArrayView as the first argument, thus losing all
       flags from the input */
    Containers::Optional<Json> json = Json::fromString({jsonData, data.flags});
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(!json->root().isParsed());
    CORRADE_COMPARE(json->root().type(), JsonToken::Type::Number);
    CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::None);
    /* If global, should point to the original string so we can change it
       below */
    if(data.flags & Containers::StringViewFlag::Global)
        CORRADE_COMPARE(json->root().data().data(), static_cast<void*>(jsonData.data()));
    /* Token data should also inherit the Global flag, and potentially
       NullTerminated if the input is null-terminated */
    CORRADE_COMPARE(json->root().data().flags(), data.flags);

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);
        if(!data.singleValue)
            CORRADE_VERIFY(json->parseDoubles(json->root()));
        else
            CORRADE_COMPARE(json->parseDouble(json->root()), data.expected);

        /* The token data should not get corrupted by this */
        CORRADE_VERIFY(json->root().isParsed());
        CORRADE_COMPARE(json->root().type(), JsonToken::Type::Number);
        CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::Double);
        CORRADE_COMPARE(json->root().data(), jsonData);
        if(data.flags & Containers::StringViewFlag::Global)
            CORRADE_COMPARE(json->root().data().data(), static_cast<void*>(jsonData.data()));
        /* Token data should still inherit the Global flag even after parsing */
        CORRADE_COMPARE(json->root().data().flags(), data.flags);
        CORRADE_COMPARE(json->root().asDouble(), data.expected);

        /* Corrupt the original string, if global. Next time it should use the
           cached value. */
        if(data.flags & Containers::StringViewFlag::Global)
            jsonData[0] = 'x';
    }
}

void JsonTest::parseFloats() {
    auto&& data = ParseDoubleOrFloatData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    Containers::String jsonData = data.json;
    /* This constructor has ArrayView as the first argument, thus losing all
       flags from the input */
    Containers::Optional<Json> json = Json::fromString({jsonData, data.flags});
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(!json->root().isParsed());
    CORRADE_COMPARE(json->root().type(), JsonToken::Type::Number);
    CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::None);
    /* If global, should point to the original string so we can change it
       below */
    if(data.flags & Containers::StringViewFlag::Global)
        CORRADE_COMPARE(json->root().data().data(), static_cast<void*>(jsonData.data()));
    /* Token data should also inherit the Global flag, and potentially
       NullTerminated if the input is null-terminated */
    CORRADE_COMPARE(json->root().data().flags(), data.flags);

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);
        if(!data.singleValue)
            CORRADE_VERIFY(json->parseFloats(json->root()));
        else
            CORRADE_COMPARE(json->parseFloat(json->root()), float(data.expected));

        /* The token data should not get corrupted by this */
        CORRADE_VERIFY(json->root().isParsed());
        CORRADE_COMPARE(json->root().type(), JsonToken::Type::Number);
        CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::Float);
        CORRADE_COMPARE(json->root().data(), jsonData);
        if(data.flags & Containers::StringViewFlag::Global)
            CORRADE_COMPARE(json->root().data().data(), static_cast<void*>(jsonData.data()));
        /* Token data should still inherit the Global flag even after parsing */
        CORRADE_COMPARE(json->root().data().flags(), data.flags);
        CORRADE_COMPARE(json->root().asFloat(), float(data.expected));

        /* Corrupt the original string, if global. Next time it should use the
           cached value. */
        if(data.flags & Containers::StringViewFlag::Global)
            jsonData[0] = 'x';
    }
}

void JsonTest::parseUnsignedInts() {
    auto&& data = ParseUnsignedIntData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    Containers::String jsonData = data.json;
    /* This constructor has ArrayView as the first argument, thus losing all
       flags from the input */
    Containers::Optional<Json> json = Json::fromString({jsonData, data.flags});
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(!json->root().isParsed());
    CORRADE_COMPARE(json->root().type(), JsonToken::Type::Number);
    CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::None);
    /* If global, should point to the original string so we can change it
       below */
    if(data.flags & Containers::StringViewFlag::Global)
        CORRADE_COMPARE(json->root().data().data(), static_cast<void*>(jsonData.data()));
    /* Token data should also inherit the Global flag, and potentially
       NullTerminated if the input is null-terminated */
    CORRADE_COMPARE(json->root().data().flags(), data.flags);

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);
        if(!data.singleValue)
            CORRADE_VERIFY(json->parseUnsignedInts(json->root()));
        else
            CORRADE_COMPARE(json->parseUnsignedInt(json->root()), data.expected);

        /* The token data should not get corrupted by this */
        CORRADE_VERIFY(json->root().isParsed());
        CORRADE_COMPARE(json->root().type(), JsonToken::Type::Number);
        CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::UnsignedInt);
        CORRADE_COMPARE(json->root().data(), jsonData);
        if(data.flags & Containers::StringViewFlag::Global)
            CORRADE_COMPARE(json->root().data().data(), static_cast<void*>(jsonData.data()));
        /* Token data should still inherit the Global flag even after parsing */
        CORRADE_COMPARE(json->root().data().flags(), data.flags);
        CORRADE_COMPARE(json->root().asUnsignedInt(), data.expected);

        /* Corrupt the original string, if global. Next time it should use the
           cached value. */
        if(data.flags & Containers::StringViewFlag::Global)
            jsonData[0] = 'x';
    }
}

void JsonTest::parseInts() {
    auto&& data = ParseIntData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    Containers::String jsonData = data.json;
    /* This constructor has ArrayView as the first argument, thus losing all
       flags from the input */
    Containers::Optional<Json> json = Json::fromString({jsonData, data.flags});
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(!json->root().isParsed());
    CORRADE_COMPARE(json->root().type(), JsonToken::Type::Number);
    CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::None);
    /* If global, should point to the original string so we can change it
       below */
    if(data.flags & Containers::StringViewFlag::Global)
        CORRADE_COMPARE(json->root().data().data(), static_cast<void*>(jsonData.data()));
    /* Token data should also inherit the Global flag, and potentially
       NullTerminated if the input is null-terminated */
    CORRADE_COMPARE(json->root().data().flags(), data.flags);

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);
        if(!data.singleValue)
            CORRADE_VERIFY(json->parseInts(json->root()));
        else
            CORRADE_COMPARE(json->parseInt(json->root()), data.expected);

        /* The token data should not get corrupted by this */
        CORRADE_VERIFY(json->root().isParsed());
        CORRADE_COMPARE(json->root().type(), JsonToken::Type::Number);
        CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::Int);
        CORRADE_COMPARE(json->root().data(), jsonData);
        if(data.flags & Containers::StringViewFlag::Global)
            CORRADE_COMPARE(json->root().data().data(), static_cast<void*>(jsonData.data()));
        /* Token data should still inherit the Global flag even after parsing */
        CORRADE_COMPARE(json->root().data().flags(), data.flags);
        CORRADE_COMPARE(json->root().asInt(), data.expected);

        /* Corrupt the original string, if global. Next time it should use the
           cached value. */
        if(data.flags & Containers::StringViewFlag::Global)
            jsonData[0] = 'x';
    }
}

void JsonTest::parseUnsignedLongs() {
    auto&& data = ParseUnsignedLongData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    Containers::String jsonData = data.json;
    /* This constructor has ArrayView as the first argument, thus losing all
       flags from the input */
    Containers::Optional<Json> json = Json::fromString({jsonData, data.flags});
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(!json->root().isParsed());
    CORRADE_COMPARE(json->root().type(), JsonToken::Type::Number);
    CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::None);
    /* If global, should point to the original string so we can change it
       below */
    if(data.flags & Containers::StringViewFlag::Global)
        CORRADE_COMPARE(json->root().data().data(), static_cast<void*>(jsonData.data()));
    /* Token data should also inherit the Global flag, and potentially
       NullTerminated if the input is null-terminated */
    CORRADE_COMPARE(json->root().data().flags(), data.flags);

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);
        if(!data.singleValue)
            CORRADE_VERIFY(json->parseUnsignedLongs(json->root()));
        else
            CORRADE_COMPARE(json->parseUnsignedLong(json->root()), data.expected);

        /* The token data should not get corrupted by this */
        CORRADE_VERIFY(json->root().isParsed());
        CORRADE_COMPARE(json->root().type(), JsonToken::Type::Number);
        CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::UnsignedLong);
        CORRADE_COMPARE(json->root().data(), jsonData);
        if(data.flags & Containers::StringViewFlag::Global)
            CORRADE_COMPARE(json->root().data().data(), static_cast<void*>(jsonData.data()));
        /* Token data should still inherit the Global flag even after parsing */
        CORRADE_COMPARE(json->root().data().flags(), data.flags);
        CORRADE_COMPARE(json->root().asUnsignedLong(), data.expected);

        /* Corrupt the original string, if global. Next time it should use the
           cached value. */
        if(data.flags & Containers::StringViewFlag::Global)
            jsonData[0] = 'x';
    }
}

void JsonTest::parseLongs() {
    auto&& data = ParseLongData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    Containers::String jsonData = data.json;
    /* This constructor has ArrayView as the first argument, thus losing all
       flags from the input */
    Containers::Optional<Json> json = Json::fromString({jsonData,  data.flags});
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(!json->root().isParsed());
    CORRADE_COMPARE(json->root().type(), JsonToken::Type::Number);
    CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::None);
    /* If global, should point to the original string so we can change it
       below */
    if(data.flags & Containers::StringViewFlag::Global)
        CORRADE_COMPARE(json->root().data().data(), static_cast<void*>(jsonData.data()));
    /* Token data should also inherit the Global flag, and potentially
       NullTerminated if the input is null-terminated */
    CORRADE_COMPARE(json->root().data().flags(), data.flags);

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);
        if(!data.singleValue)
            CORRADE_VERIFY(json->parseLongs(json->root()));
        else
            CORRADE_COMPARE(json->parseLong(json->root()), data.expected);

        /* The token data should not get corrupted by this */
        CORRADE_VERIFY(json->root().isParsed());
        CORRADE_COMPARE(json->root().type(), JsonToken::Type::Number);
        CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::Long);
        CORRADE_COMPARE(json->root().data(), jsonData);
        if(data.flags & Containers::StringViewFlag::Global)
            CORRADE_COMPARE(json->root().data().data(), static_cast<void*>(jsonData.data()));
        /* Token data should still inherit the Global flag even after parsing */
        CORRADE_COMPARE(json->root().data().flags(), data.flags);
        CORRADE_COMPARE(json->root().asLong(), data.expected);

        /* Corrupt the original string, if global. Next time it should use the
           cached value. */
        if(data.flags & Containers::StringViewFlag::Global)
            jsonData[0] = 'x';
    }
}

void JsonTest::parseSizes() {
    #ifndef CORRADE_TARGET_32BIT
    auto&& data = ParseUnsignedLongData[testCaseInstanceId()];
    #else
    auto&& data = ParseUnsignedIntData[testCaseInstanceId()];
    #endif
    setTestCaseDescription(data.name);

    Containers::String jsonData = data.json;
    /* This constructor has ArrayView as the first argument, thus losing all
       flags from the input */
    Containers::Optional<Json> json = Json::fromString({jsonData,  data.flags});
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(!json->root().isParsed());
    CORRADE_COMPARE(json->root().type(), JsonToken::Type::Number);
    CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::None);
    CORRADE_COMPARE(json->root().data(), data.json);
    /* If global, should point to the original string so we can change it
       below */
    if(data.flags & Containers::StringViewFlag::Global)
        CORRADE_COMPARE(json->root().data().data(), static_cast<void*>(jsonData.data()));
    /* Token data should also inherit the Global flag, and potentially
       NullTerminated if the input is null-terminated */
    CORRADE_COMPARE(json->root().data().flags(), data.flags);

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);
        if(!data.singleValue)
            CORRADE_VERIFY(json->parseSizes(json->root()));
        else
            CORRADE_COMPARE(json->parseSize(json->root()), data.expected);

        /* The token data should not get corrupted by this */
        CORRADE_VERIFY(json->root().isParsed());
        CORRADE_COMPARE(json->root().type(), JsonToken::Type::Number);
        CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::Size);
        CORRADE_COMPARE(json->root().data(), jsonData);
        if(data.flags & Containers::StringViewFlag::Global)
            CORRADE_COMPARE(json->root().data().data(), static_cast<void*>(jsonData.data()));
        /* Token data should still inherit the Global flag even after parsing */
        CORRADE_COMPARE(json->root().data().flags(), data.flags);
        CORRADE_COMPARE(json->root().asSize(), data.expected);

        /* Corrupt the original string, if global. Next time it should use the
           cached value. */
        if(data.flags & Containers::StringViewFlag::Global)
            jsonData[0] = 'x';
    }
}

void JsonTest::parseStringKeys() {
    auto&& data = ParseStringData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    /* Fake-propagate original global flags here. This constructor has
       ArrayView as the first argument, thus losing all flags from the
       input. */
    Containers::String jsonData = format("{{{}: null}}", data.json);
    Containers::Optional<Json> json = Json::fromString({jsonData, data.json.flags()});
    CORRADE_VERIFY(json);
    JsonToken token = json->tokens()[1];
    CORRADE_VERIFY(!token.isParsed());
    CORRADE_COMPARE(token.type(), JsonToken::Type::String);
    CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::None);
    /* If global, should point to the original string so we can change it
       below */
    if(data.json.flags() & Containers::StringViewFlag::Global)
        CORRADE_COMPARE(token.data().data(), static_cast<void*>(jsonData.data() + 1));
    else
        CORRADE_COMPARE(token.data(), data.json);
    /* Token data should also inherit the Global flag. NullTerminated is never
       inherited as this is formatted into an object key with data after */
    CORRADE_COMPARE(token.data().flags(), data.json.flags() & ~Containers::StringViewFlag::NullTerminated);

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);
        if(!data.singleValue)
            CORRADE_VERIFY(json->parseStringKeys(json->root()));
        else
            CORRADE_COMPARE(*json->parseString(token), data.expected);

        /* The token data should not get corrupted by this */
        CORRADE_VERIFY(token.isParsed());
        CORRADE_COMPARE(token.type(), JsonToken::Type::String);
        CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::Other);
        if(data.json.flags() & Containers::StringViewFlag::Global) {
            CORRADE_COMPARE(token.data().data(), static_cast<void*>(jsonData.data() + 1));
            CORRADE_COMPARE(token.data().size(), data.json.size());
        } else {
            CORRADE_COMPARE(token.data(), data.json);
        }
        /* Token data should still inherit the Global flag even after parsing */
        CORRADE_COMPARE(json->root().data().flags(), data.json.flags());
        {
            CORRADE_EXPECT_FAIL_IF(data.expectFail, Containers::StringView{data.expectFail});
            CORRADE_COMPARE(token.asString(), data.expected);
        }
        CORRADE_COMPARE(token.asString().flags() & ~Containers::StringViewFlag::NullTerminated,
            data.expectedFlags & ~Containers::StringViewFlag::NullTerminated);

        /* If the input is global but escaped (i.e., expected no longer global),
           corrupt the original string. Next time it should use the cached
           string. */
        if((data.json.flags() & Containers::StringViewFlag::Global) &&
          !(data.expectedFlags & Containers::StringViewFlag::Global))
            jsonData[2] = 'x';
    }
}

void JsonTest::parseStrings() {
    auto&& data = ParseStringData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    /* Fake-propagate original global flags here */
    Containers::String jsonData = data.json;
    Containers::Optional<Json> json = Json::fromString({jsonData.data(), jsonData.size(), data.json.flags()});
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(!json->root().isParsed());
    CORRADE_COMPARE(json->root().type(), JsonToken::Type::String);
    CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::None);
    /* If global, should point to the original string so we can change it
       below */
    if(data.json.flags() & Containers::StringViewFlag::Global)
        CORRADE_COMPARE(json->root().data().data(), static_cast<void*>(jsonData.data()));
    else
        CORRADE_COMPARE(json->root().data(), data.json);
    /* Token data should also inherit the Global flag, and potentially
       NullTerminated if the input is null-terminated */
    CORRADE_COMPARE(json->root().data().flags(), data.json.flags());

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);
        if(!data.singleValue)
            CORRADE_VERIFY(json->parseStrings(json->root()));
        else
            CORRADE_COMPARE(*json->parseString(json->root()), data.expected);

        /* The token data should not get corrupted by this */
        CORRADE_VERIFY(json->root().isParsed());
        CORRADE_COMPARE(json->root().type(), JsonToken::Type::String);
        CORRADE_COMPARE(json->root().parsedType(), JsonToken::ParsedType::Other);
        if(data.json.flags() & Containers::StringViewFlag::Global) {
            CORRADE_COMPARE(json->root().data().data(), static_cast<void*>(jsonData.data()));
            CORRADE_COMPARE(json->root().data().size(), jsonData.size());
        } else {
            CORRADE_COMPARE(json->root().data(), jsonData);
        }
        /* Token data should still inherit the Global flag even after parsing */
        CORRADE_COMPARE(json->root().data().flags(), data.json.flags());
        {
            CORRADE_EXPECT_FAIL_IF(data.expectFail, Containers::StringView{data.expectFail});
            CORRADE_COMPARE(json->root().asString(), data.expected);
        }
        CORRADE_COMPARE(json->root().asString().flags() & ~Containers::StringViewFlag::NullTerminated,
            data.expectedFlags & ~Containers::StringViewFlag::NullTerminated);

        /* If the input is global but escaped (i.e., expected no longer global),
           corrupt the original string. Next time it should use the cached
           string. */
        if((data.json.flags() & Containers::StringViewFlag::Global) &&
          !(data.expectedFlags & Containers::StringViewFlag::Global))
            jsonData[1] = 'x';
    }
}

void JsonTest::parseOption() {
    auto&& data = ParseOptionData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    Containers::Optional<Json> json = Json::fromString(R"({
        "object": {},
        "array": [],
        "null": null,
        "bool": true,
        "nested": {
            "object": {},
            "array": [],
            "null": null,
            "bool": true,
            "number": 35,
            "string": "hello"
        },
        "number": 35,
        "string": "hello"
    })", data.option);
    CORRADE_VERIFY(json);
    CORRADE_COMPARE(json->tokens().size(), 27);

    JsonToken tokenParsed = json->tokens()[data.tokenParsed];
    CORRADE_COMPARE(tokenParsed.data(), data.tokenData);
    CORRADE_COMPARE(tokenParsed.parsedType(), data.parsedType);

    JsonToken tokenParsed2 = json->tokens()[data.tokenParsed2];
    CORRADE_COMPARE(tokenParsed2.data(), data.tokenData);
    CORRADE_COMPARE(tokenParsed2.parsedType(), data.parsedType);

    /* Verify tokens of other type are not parsed by accident */
    std::size_t notParsedCount = 0;
    for(JsonToken token: json->tokens())
        if(!token.isParsed()) ++notParsedCount;
    CORRADE_COMPARE(notParsedCount, data.tokenNotParsedCount);
}

void JsonTest::parseSubtree() {
    auto&& data = ParseSubtreeData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    Containers::Optional<Json> json = Json::fromString(R"([
        {
            "null": null,
            "bool": true,
            "nested": {
                "null": null,
                "bool": true,
                "number": 35,
                "nested": [
                    "hello",
                    {},
                    []
                ]
            },
            "number": 35,
            "nested": [
                "hello",
                {},
                []
            ]
        },
        null,
        true,
        35,
        "hello",
        {},
        []
    ])");
    CORRADE_VERIFY(json);
    CORRADE_COMPARE(json->tokens().size(), 32);
    CORRADE_VERIFY(((*json).*data.function)(json->tokens()[data.parseRoot]));

    JsonToken tokenParsed = json->tokens()[data.tokenParsed];
    CORRADE_COMPARE(tokenParsed.data(), data.tokenData);
    CORRADE_COMPARE(tokenParsed.parsedType(), data.parsedType);

    JsonToken tokenParsedDeep = json->tokens()[data.tokenParsedDeep];
    CORRADE_COMPARE(tokenParsedDeep.data(), data.tokenData);
    CORRADE_COMPARE(tokenParsedDeep.parsedType(), data.parsedType);

    JsonToken tokenNotParsedOut = json->tokens()[data.tokenNotParsed];
    CORRADE_COMPARE(tokenNotParsedOut.data(), data.tokenData);
    CORRADE_VERIFY(!tokenNotParsedOut.isParsed());

    /* Verify tokens of other type are not parsed by accident */
    std::size_t notParsedCount = 0;
    for(JsonToken token: json->tokens())
        if(!token.isParsed()) ++notParsedCount;
    CORRADE_COMPARE(notParsedCount, data.tokenNotParsedCount);
}

void JsonTest::parseEmptyObjectOrArray() {
    auto&& data = EmptyObjectOrArrayData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    Containers::Optional<Json> json = Json::fromString(data.json);
    CORRADE_VERIFY(json);

    Containers::Optional<std::size_t> size = data.function(*json);
    CORRADE_COMPARE(size, 0);
    CORRADE_VERIFY(json->root().isParsed());
}

void JsonTest::parseBitArray() {
    Containers::String jsonData = "[true, false, true, false]";
    Containers::Optional<Json> json = Json::fromString({jsonData,  Containers::StringViewFlag::Global});
    CORRADE_VERIFY(json);

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);

        Containers::Optional<Containers::StridedBitArrayView1D> out = json->parseBitArray(json->root());
        CORRADE_VERIFY(out);
        CORRADE_COMPARE_AS(*out, Containers::stridedArrayView({
            true, false, true, false
        }).sliceBit(0), TestSuite::Compare::Container);

        CORRADE_VERIFY(json->root().isParsed());
        for(JsonArrayItem i: json->root().asArray()) {
            CORRADE_ITERATION(i.index());
            CORRADE_VERIFY(i.value().isParsed());
        }

        /* Corrupt the original string. Next time it should use the cached
           values. */
        jsonData[jsonData.size() - 2] = 'x';
    }
}

#ifdef CORRADE_BUILD_DEPRECATED
void JsonTest::parseBoolArrayDeprecated() {
    Containers::String jsonData = "[true, false, true, false]";
    Containers::Optional<Json> json = Json::fromString({jsonData,  Containers::StringViewFlag::Global});
    CORRADE_VERIFY(json);

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);

        CORRADE_IGNORE_DEPRECATED_PUSH
        Containers::Optional<Containers::StridedArrayView1D<const bool>> out = json->parseBoolArray(json->root());
        CORRADE_IGNORE_DEPRECATED_POP
        CORRADE_VERIFY(out);
        CORRADE_COMPARE_AS(*out, Containers::arrayView({
            true, false, true, false
        }), TestSuite::Compare::Container);

        CORRADE_VERIFY(json->root().isParsed());
        for(JsonArrayItem i: json->root().asArray()) {
            CORRADE_ITERATION(i.index());
            CORRADE_VERIFY(i.value().isParsed());
        }

        /* Corrupt the original string. Next time it should use the cached
           values. */
        jsonData[jsonData.size() - 2] = 'x';
    }
}
#endif

void JsonTest::parseDoubleArray() {
    Containers::String jsonData = "[35.7, -42.4, 0, 1e5]";
    Containers::Optional<Json> json = Json::fromString({jsonData,  Containers::StringViewFlag::Global});
    CORRADE_VERIFY(json);

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);

        Containers::Optional<Containers::StridedArrayView1D<const double>> out = json->parseDoubleArray(json->root());
        CORRADE_VERIFY(out);
        CORRADE_COMPARE_AS(*out, Containers::arrayView({
            35.7, -42.4, 0.0, 1.0e5
        }), TestSuite::Compare::Container);

        CORRADE_VERIFY(json->root().isParsed());
        for(JsonArrayItem i: json->root().asArray()) {
            CORRADE_ITERATION(i.index());
            CORRADE_COMPARE(i.value().parsedType(), JsonToken::ParsedType::Double);
        }

        /* Corrupt the original string. Next time it should use the cached
           values. */
        jsonData[jsonData.size() - 2] = 'x';
    }
}

void JsonTest::parseFloatArray() {
    Containers::String jsonData = "[35.7, -42.4, 0, 1e5]";
    Containers::Optional<Json> json = Json::fromString({jsonData,  Containers::StringViewFlag::Global});
    CORRADE_VERIFY(json);

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);

        Containers::Optional<Containers::StridedArrayView1D<const float>> out = json->parseFloatArray(json->root());
        CORRADE_VERIFY(out);
        CORRADE_COMPARE_AS(*out, Containers::arrayView({
            35.7f, -42.4f, 0.0f, 1.0e5f
        }), TestSuite::Compare::Container);

        CORRADE_VERIFY(json->root().isParsed());
        for(JsonArrayItem i: json->root().asArray()) {
            CORRADE_ITERATION(i.index());
            CORRADE_COMPARE(i.value().parsedType(), JsonToken::ParsedType::Float);
        }

        /* Corrupt the original string. Next time it should use the cached
           values. */
        jsonData[jsonData.size() - 2] = 'x';
    }
}

void JsonTest::parseUnsignedIntArray() {
    Containers::String jsonData = "[357, 424, 0, 1234567890]";
    Containers::Optional<Json> json = Json::fromString({jsonData,  Containers::StringViewFlag::Global});
    CORRADE_VERIFY(json);

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);

        Containers::Optional<Containers::StridedArrayView1D<const std::uint32_t>> out = json->parseUnsignedIntArray(json->root());
        CORRADE_VERIFY(out);
        CORRADE_COMPARE_AS(*out, Containers::arrayView({
            357u, 424u, 0u, 1234567890u
        }), TestSuite::Compare::Container);

        CORRADE_VERIFY(json->root().isParsed());
        for(JsonArrayItem i: json->root().asArray()) {
            CORRADE_ITERATION(i.index());
            CORRADE_COMPARE(i.value().parsedType(), JsonToken::ParsedType::UnsignedInt);
        }

        /* Corrupt the original string. Next time it should use the cached
           values. */
        jsonData[jsonData.size() - 2] = 'x';
    }
}

void JsonTest::parseIntArray() {
    Containers::String jsonData = "[357, -424, 0, 1234567890]";
    Containers::Optional<Json> json = Json::fromString({jsonData,  Containers::StringViewFlag::Global});
    CORRADE_VERIFY(json);

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);

        Containers::Optional<Containers::StridedArrayView1D<const std::int32_t>> out = json->parseIntArray(json->root());
        CORRADE_VERIFY(out);
        CORRADE_COMPARE_AS(*out, Containers::arrayView({
            357, -424, 0, 1234567890
        }), TestSuite::Compare::Container);

        CORRADE_VERIFY(json->root().isParsed());
        for(JsonArrayItem i: json->root().asArray()) {
            CORRADE_ITERATION(i.index());
            CORRADE_COMPARE(i.value().parsedType(), JsonToken::ParsedType::Int);
        }

        /* Corrupt the original string. Next time it should use the cached
           values. */
        jsonData[jsonData.size() - 2] = 'x';
    }
}

void JsonTest::parseUnsignedLongArray() {
    Containers::String jsonData = "[357, 424, 0, 123456789012345]";
    Containers::Optional<Json> json = Json::fromString({jsonData,  Containers::StringViewFlag::Global});
    CORRADE_VERIFY(json);

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);

        Containers::Optional<Containers::StridedArrayView1D<const std::uint64_t>> out = json->parseUnsignedLongArray(json->root());
        CORRADE_VERIFY(out);
        CORRADE_COMPARE_AS(*out, Containers::arrayView<std::uint64_t>({
            357ull, 424ull, 0ull, 123456789012345ull
        }), TestSuite::Compare::Container);

        CORRADE_VERIFY(json->root().isParsed());
        for(JsonArrayItem i: json->root().asArray()) {
            CORRADE_ITERATION(i.index());
            CORRADE_COMPARE(i.value().parsedType(), JsonToken::ParsedType::UnsignedLong);
        }

        /* Corrupt the original string. Next time it should use the cached
           values. */
        jsonData[jsonData.size() - 2] = 'x';
    }
}

void JsonTest::parseLongArray() {
    Containers::String jsonData = "[357, -424, 0, -123456789012345]";
    Containers::Optional<Json> json = Json::fromString({jsonData,  Containers::StringViewFlag::Global});
    CORRADE_VERIFY(json);

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);

        Containers::Optional<Containers::StridedArrayView1D<const std::int64_t>> out = json->parseLongArray(json->root());
        CORRADE_VERIFY(out);
        CORRADE_COMPARE_AS(*out, Containers::arrayView<std::int64_t>({
            357ll, -424ll, 0ll, -123456789012345ll
        }), TestSuite::Compare::Container);

        CORRADE_VERIFY(json->root().isParsed());
        for(JsonArrayItem i: json->root().asArray()) {
            CORRADE_ITERATION(i.index());
            CORRADE_COMPARE(i.value().parsedType(), JsonToken::ParsedType::Long);
        }

        /* Corrupt the original string. Next time it should use the cached
           values. */
        jsonData[jsonData.size() - 2] = 'x';
    }
}

void JsonTest::parseSizeArray() {
    Containers::String jsonData =
        #ifndef CORRADE_TARGET_32BIT
        "[357, 424, 0, 123456789012345]"
        #else
        "[357, 424, 0, 1234567890]"
        #endif
        ;
    Containers::Optional<Json> json = Json::fromString({jsonData,  Containers::StringViewFlag::Global});
    CORRADE_VERIFY(json);

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);

        Containers::Optional<Containers::StridedArrayView1D<const std::size_t>> out = json->parseSizeArray(json->root());
        CORRADE_VERIFY(out);
        #ifndef CORRADE_TARGET_32BIT
        CORRADE_COMPARE_AS(*out, Containers::arrayView<std::size_t>({
            357ull, 424ull, 0ull, 123456789012345ull
        }), TestSuite::Compare::Container);
        #else
        CORRADE_COMPARE_AS(*out, Containers::arrayView<std::size_t>({
            357u, 424u, 0u, 1234567890u
        }), TestSuite::Compare::Container);
        #endif

        CORRADE_VERIFY(json->root().isParsed());
        for(JsonArrayItem i: json->root().asArray()) {
            CORRADE_ITERATION(i.index());
            CORRADE_COMPARE(i.value().parsedType(), JsonToken::ParsedType::Size);
        }

        /* Corrupt the original string. Next time it should use the cached
           values. */
        jsonData[jsonData.size() - 2] = 'x';
    }
}

void JsonTest::parseStringArray() {
    auto&& data = ParseStringArrayData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    Containers::String jsonData = "[\"hello\", \",\\t\", \"world\", \"!\"]";
    Containers::Optional<Json> json = Json::fromString({jsonData,  data.flags});
    CORRADE_VERIFY(json);

    /* Calling the parse function several times should have the same observed
       behavior, internally it should just skip parsing */
    for(std::size_t iteration: {0, 1}) {
        CORRADE_ITERATION(iteration);

        Containers::Optional<Containers::StringIterable> out = json->parseStringArray(json->root());
        CORRADE_VERIFY(out);
        CORRADE_COMPARE_AS(*out, (Containers::StringIterable{
            "hello", ",\t", "world", "!"
        }), TestSuite::Compare::Container);

        CORRADE_VERIFY(json->root().isParsed());
        for(JsonArrayItem i: json->root().asArray()) {
            CORRADE_ITERATION(i.index());
            CORRADE_VERIFY(i.value().isParsed());
        }

        /* The second string is escaped so it's not global, the others are
           global only if the input was global as well */
        CORRADE_COMPARE((*out)[0].flags() & ~Containers::StringViewFlag::NullTerminated,
            data.flags & ~Containers::StringViewFlag::NullTerminated);
        CORRADE_COMPARE((*out)[1].flags(), Containers::StringViewFlag::NullTerminated);
        CORRADE_COMPARE((*out)[2].flags() & ~Containers::StringViewFlag::NullTerminated,
            data.flags & ~Containers::StringViewFlag::NullTerminated);
        CORRADE_COMPARE((*out)[3].flags() & ~Containers::StringViewFlag::NullTerminated,
            data.flags & ~Containers::StringViewFlag::NullTerminated);

        /* Corrupt the original string. Next time it should use the cached
           values. */
        jsonData[jsonData.size() - 2] = 'x';
    }
}

void JsonTest::reparseNumberDifferentType() {
    /* It should be possible to reparse a token with different numeric types
       several times over */

    Containers::Optional<Json> json = Json::fromString("35");
    CORRADE_VERIFY(json);

    JsonToken token = json->root();
    CORRADE_VERIFY(json->parseDoubles(token));
    CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::Double);
    CORRADE_COMPARE(token.asDouble(), 35.0);

    CORRADE_VERIFY(json->parseFloats(token));
    CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::Float);
    CORRADE_COMPARE(token.asFloat(), 35.0f);

    CORRADE_VERIFY(json->parseUnsignedInts(token));
    CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::UnsignedInt);
    CORRADE_COMPARE(token.asUnsignedInt(), 35);

    CORRADE_VERIFY(json->parseInts(token));
    CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::Int);
    CORRADE_COMPARE(token.asInt(), 35);

    CORRADE_VERIFY(json->parseUnsignedLongs(token));
    CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::UnsignedLong);
    CORRADE_COMPARE(token.asUnsignedLong(), 35);

    CORRADE_VERIFY(json->parseLongs(token));
    CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::Long);
    CORRADE_COMPARE(token.asLong(), 35);

    CORRADE_VERIFY(json->parseSizes(token));
    CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::Size);
    CORRADE_COMPARE(token.asSize(), 35);

    /* ... and back again */
    CORRADE_VERIFY(json->parseDoubles(token));
    CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::Double);
    CORRADE_COMPARE(token.asDouble(), 35.0);
}

void JsonTest::reparseSingleNumberDifferentType() {
    /* It should be possible to reparse a token with different numeric types
       several times over */

    Containers::Optional<Json> json = Json::fromString("35");
    CORRADE_VERIFY(json);

    JsonToken token = json->root();
    CORRADE_COMPARE(json->parseDouble(token), 35.0);
    CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::Double);

    CORRADE_COMPARE(json->parseFloat(token), 35.0f);
    CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::Float);

    CORRADE_COMPARE(json->parseUnsignedInt(token), 35);
    CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::UnsignedInt);

    CORRADE_COMPARE(json->parseInt(token), 35);
    CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::Int);

    CORRADE_COMPARE(json->parseUnsignedLong(token), 35);
    CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::UnsignedLong);

    CORRADE_COMPARE(json->parseLong(token), 35);
    CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::Long);

    CORRADE_COMPARE(json->parseSize(token), 35);
    CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::Size);

    /* ... and back again */
    CORRADE_COMPARE(json->parseDouble(token), 35.0);
    CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::Double);
}

void JsonTest::reparseNumberArrayDifferentType() {
    /* It should be possible to reparse a token with different numeric types
       several times over */

    Containers::Optional<Json> json = Json::fromString("[35, 17]");
    CORRADE_VERIFY(json);

    JsonToken token = json->root();
    {
        Containers::Optional<Containers::StridedArrayView1D<const double>> out = json->parseDoubleArray(token);
        CORRADE_VERIFY(out);
        CORRADE_COMPARE_AS(*out, Containers::arrayView({
            35.0, 17.0
        }), TestSuite::Compare::Container);
    } {
        Containers::Optional<Containers::StridedArrayView1D<const float>> out = json->parseFloatArray(token);
        CORRADE_VERIFY(out);
        CORRADE_COMPARE_AS(*out, Containers::arrayView({
            35.0f, 17.0f
        }), TestSuite::Compare::Container);
    } {
        Containers::Optional<Containers::StridedArrayView1D<const std::uint32_t>> out = json->parseUnsignedIntArray(token);
        CORRADE_VERIFY(out);
        CORRADE_COMPARE_AS(*out, Containers::arrayView({
            35u, 17u
        }), TestSuite::Compare::Container);
    } {
        Containers::Optional<Containers::StridedArrayView1D<const std::int32_t>> out = json->parseIntArray(token);
        CORRADE_VERIFY(out);
        CORRADE_COMPARE_AS(*out, Containers::arrayView({
            35, 17
        }), TestSuite::Compare::Container);
    } {
        Containers::Optional<Containers::StridedArrayView1D<const std::uint64_t>> out = json->parseUnsignedLongArray(token);
        CORRADE_VERIFY(out);
        CORRADE_COMPARE_AS(*out, Containers::arrayView<std::uint64_t>({
            35, 17
        }), TestSuite::Compare::Container);
    } {
        Containers::Optional<Containers::StridedArrayView1D<const std::int64_t>> out = json->parseLongArray(token);
        CORRADE_VERIFY(out);
        CORRADE_COMPARE_AS(*out, Containers::arrayView<std::int64_t>({
            35, 17
        }), TestSuite::Compare::Container);
    } {
        Containers::Optional<Containers::StridedArrayView1D<const std::size_t>> out = json->parseSizeArray(token);
        CORRADE_VERIFY(out);
        CORRADE_COMPARE_AS(*out, Containers::arrayView<std::size_t>({
            35, 17
        }), TestSuite::Compare::Container);
    }

    /* ... and back again */
    {
        Containers::Optional<Containers::StridedArrayView1D<const double>> out = json->parseDoubleArray(token);
        CORRADE_VERIFY(out);
        CORRADE_COMPARE_AS(*out, Containers::arrayView({
            35.0, 17.0
        }), TestSuite::Compare::Container);
    }
}

void JsonTest::reparseNumberDifferentTypeFailKeepOriginalValue() {
    Containers::String jsonData = "35.7";
    /* This constructor has ArrayView as the first argument, thus losing all
       flags from the input */
    Containers::Optional<Json> json = Json::fromString({jsonData, Containers::StringViewFlag::NullTerminated|Containers::StringViewFlag::Global});
    CORRADE_VERIFY(json);

    /* Parse the token and corrupt the original data so it cannot be reparsed.
       Corrupt not the first letter but a later one so the parsing still
       produces some (but broken) value. */
    JsonToken token = json->root();
    CORRADE_COMPARE(json->parseFloat(token), 35.7f);
    jsonData[1] = 'x';

    /* Reparsing as a different type should fail but preserve the original
       parsed type and value untouched */
    {
        Containers::String out;
        Error redirectError{&out};
        CORRADE_VERIFY(!json->parseDouble(token));
        CORRADE_COMPARE(out, "Utility::Json::parseDouble(): invalid floating-point literal 3x.7 at <in>:1:1\n");
        CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::Float);
        CORRADE_COMPARE(token.asFloat(), 35.7f);
    } {
        Containers::String out;
        Error redirectError{&out};
        CORRADE_VERIFY(!json->parseUnsignedInt(token));
        CORRADE_COMPARE(out, "Utility::Json::parseUnsignedInt(): invalid unsigned integer literal 3x.7 at <in>:1:1\n");
        CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::Float);
        CORRADE_COMPARE(token.asFloat(), 35.7f);
    } {
        Containers::String out;
        Error redirectError{&out};
        CORRADE_VERIFY(!json->parseInt(token));
        CORRADE_COMPARE(out, "Utility::Json::parseInt(): invalid integer literal 3x.7 at <in>:1:1\n");
        CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::Float);
        CORRADE_COMPARE(token.asFloat(), 35.7f);
    } {
        Containers::String out;
        Error redirectError{&out};
        CORRADE_VERIFY(!json->parseUnsignedLong(token));
        CORRADE_COMPARE(out, "Utility::Json::parseUnsignedLong(): invalid unsigned integer literal 3x.7 at <in>:1:1\n");
        CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::Float);
        CORRADE_COMPARE(token.asFloat(), 35.7f);
    } {
        Containers::String out;
        Error redirectError{&out};
        CORRADE_VERIFY(!json->parseLong(token));
        CORRADE_COMPARE(out, "Utility::Json::parseLong(): invalid integer literal 3x.7 at <in>:1:1\n");
        CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::Float);
        CORRADE_COMPARE(token.asFloat(), 35.7f);
    }

    /* De-corrupt the data, parse as an int and corrupt again so we can test
       parseFloat() failure also, Not testing with a double because it may
       attempt to reinterpret the partially broken float as a double and arrive
       at something almost reasonable, making the test suspiciously pass. */
    jsonData[1] = '7';
    jsonData[2] = '5';
    CORRADE_COMPARE(json->parseUnsignedLong(token), 3757);
    CORRADE_COMPARE(token.asUnsignedLong(), 3757);
    jsonData[1] = 'x';
    {
        Containers::String out;
        Error redirectError{&out};
        CORRADE_VERIFY(!json->parseFloat(token));
        CORRADE_COMPARE(out, "Utility::Json::parseFloat(): invalid floating-point literal 3x57 at <in>:1:1\n");
        CORRADE_COMPARE(token.parsedType(), JsonToken::ParsedType::UnsignedLong);
        CORRADE_COMPARE(token.asUnsignedLong(), 3757);
    }
}

void JsonTest::parsedObjectChildAccess() {
    /* Verify that child count and first child access of object keys stays the
       same after the contents get parsed -- for object keys the child count is
       implicitly the count of the value + 1, and it shouldn't get clobbered
       when the value gets parsed */

    Containers::Optional<Json> json = Json::fromString(R"({
        "null": null,
        "bool": false,
        "float": 3.5,
        "double": 3.5,
        "unsigned int": 15,
        "int": -15,
        "unsigned long": 15,
        "long": -15,
        "string": "string",
        "escaped\nstring": "escaped\nstring",
        "array": [],
        "object": {}
    })");
    CORRADE_VERIFY(json);

    JsonToken object = json->root();
    for(JsonToken i: *json->parseObject(object)) {
        CORRADE_ITERATION(i.data());
        CORRADE_COMPARE(i.childCount(), 1);
        CORRADE_VERIFY(i.firstChild());
        CORRADE_VERIFY(!i.firstChild()->isParsed());
    }

    /* Parse the values in reverse order so in case some causes the child count
       to be misreported, it doesn't affect (-> crash) search of the remaining
       keys */
    CORRADE_VERIFY(json->parseObject(object["object"]));
    CORRADE_VERIFY(json->parseArray(object["array"]));
    CORRADE_VERIFY(json->parseString(object["escaped\nstring"]));
    CORRADE_VERIFY(json->parseString(object["string"]));
    CORRADE_VERIFY(json->parseLong(object["long"]));
    CORRADE_VERIFY(json->parseUnsignedLong(object["unsigned long"]));
    CORRADE_VERIFY(json->parseInt(object["int"]));
    CORRADE_VERIFY(json->parseUnsignedInt(object["unsigned int"]));
    CORRADE_VERIFY(json->parseDouble(object["double"]));
    CORRADE_VERIFY(json->parseFloat(object["float"]));
    CORRADE_VERIFY(json->parseBool(object["bool"]));
    CORRADE_VERIFY(json->parseNull(object["null"]));

    for(JsonToken i: object.asObject()) {
        CORRADE_ITERATION(i.data());
        CORRADE_COMPARE(i.childCount(), 1);
        CORRADE_VERIFY(i.firstChild());
        CORRADE_VERIFY(i.firstChild()->isParsed());
    }
}

void JsonTest::parsedObjectFind() {
    Containers::Optional<Json> json = Json::fromString(R"({
        "a": "key",
        "another": 3
    })");
    CORRADE_VERIFY(json);

    Containers::Optional<JsonObjectView> object = json->parseObject(json->root());
    CORRADE_VERIFY(object);

    JsonIterator another = object->find("another");
    CORRADE_VERIFY(another);
    CORRADE_COMPARE(another->data(), "3");
    CORRADE_COMPARE((*object)["another"].data(), "3");

    CORRADE_VERIFY(!object->find("different"));
}

void JsonTest::parsedObjectFindEmpty() {
    auto&& data = ParseObjectArrayEmptyData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    /* The view delegates to find() of the parent token, but the parent is
       inaccessible if the token has no children so it has to special-case
       that. The operator[] then asserts for this case, which is tested
       separately below. */

    Containers::Optional<Json> json = Json::fromString(data.jsonObject);
    CORRADE_VERIFY(json);

    Containers::Optional<JsonObjectView> object = json->parseObject(json->tokens()[data.tokenIndex]);
    CORRADE_VERIFY(object);

    CORRADE_VERIFY(!object->find("something"));
}

void JsonTest::parsedObjectAccessEmpty() {
    auto&& data = ParseObjectArrayEmptyData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(data.jsonObject);
    CORRADE_VERIFY(json);

    Containers::Optional<JsonObjectView> object = json->parseObject(json->tokens()[data.tokenIndex]);
    CORRADE_VERIFY(object);

    Containers::String out;
    Error redirectError{&out};
    (*object)["something"];
    CORRADE_COMPARE(out, "Utility::JsonObjectView::operator[](): view is empty\n");
}

void JsonTest::parsedArrayFind() {
    Containers::Optional<Json> json = Json::fromString(R"([
        "value",
        "another",
        3
    ])");
    CORRADE_VERIFY(json);

    Containers::Optional<JsonArrayView> array = json->parseArray(json->root());
    CORRADE_VERIFY(array);

    JsonIterator value = array->find(2);
    CORRADE_VERIFY(value);
    CORRADE_COMPARE(value->data(), "3");
    CORRADE_COMPARE((*array)[2].data(), "3");

    CORRADE_VERIFY(!array->find(3));
}

void JsonTest::parsedArrayFindEmpty() {
    auto&& data = ParseObjectArrayEmptyData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    /* The view delegates to find() of the parent token, but the parent is
       inaccessible if the token has no children so it has to special-case
       that. The operator[] then asserts for this case, which is tested
       separately below. */

    Containers::Optional<Json> json = Json::fromString(data.jsonArray);
    CORRADE_VERIFY(json);

    Containers::Optional<JsonArrayView> array = json->parseArray(json->tokens()[data.tokenIndex]);
    CORRADE_VERIFY(array);

    CORRADE_VERIFY(!array->find(0));
}

void JsonTest::parsedArrayAccessEmpty() {
    auto&& data = ParseObjectArrayEmptyData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(data.jsonArray);
    CORRADE_VERIFY(json);

    Containers::Optional<JsonArrayView> array = json->parseArray(json->tokens()[data.tokenIndex]);
    CORRADE_VERIFY(array);

    Containers::String out;
    Error redirectError{&out};
    (*array)[0];
    CORRADE_COMPARE(out, "Utility::JsonArrayView::operator[](): view is empty\n");
}

void JsonTest::parseError() {
    auto&& data = ParseErrorData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    Containers::Optional<Json> json = Json::fromString(format("\n\n     {}", data.json));
    CORRADE_VERIFY(json);

    JsonToken token = json->root();
    JsonToken::Type type = token.type();

    Containers::String out;
    Error redirectError{&out};
    CORRADE_EXPECT_FAIL_IF(!data.message, "Not implemented yet.");
    CORRADE_VERIFY(!((*json).*data.function)(json->root()));
    if(!data.message) return;
    CORRADE_COMPARE(out, format("Utility::Json::{}\n", data.message));

    /* Verify that the JSON token doesn't get corrupted by the error */
    CORRADE_VERIFY(!token.isParsed());
    CORRADE_COMPARE(token.type(), type);
    CORRADE_COMPARE(token.data(), data.json);
    CORRADE_COMPARE(token.childCount(), 0);
}

void JsonTest::parseOptionError() {
    /* The particular corner cases got all tested in parseError(), here just
       verifying that the error gets correctly propagated also when using
       Json::Option. Tokenization errors when using Json::Option are tested
       in errorOption(). */

    auto&& data = ParseOptionErrorData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    Containers::String out;
    Error redirectError{&out};
    CORRADE_VERIFY(!Json::fromString(data.json, data.option));
    CORRADE_COMPARE(out, format("Utility::Json::{}\n", data.message));
}

void JsonTest::parseSingleError() {
    /* The particular corner cases got all tested in parseError(), here just
       verifying that the error gets correctly propagated also when using
       Json::parseWhatever() */

    auto&& data = ParseSingleErrorData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    Containers::Optional<Json> json = Json::fromString(format("\n\n     {}", data.json));
    CORRADE_VERIFY(json);

    Containers::String out;
    Error redirectError{&out};
    CORRADE_VERIFY(!data.function(*json));
    CORRADE_COMPARE(out, format("Utility::Json::{}\n", data.message));
}

void JsonTest::parseArrayError() {
    /* The particular corner cases got all tested in parseError(), here just
       verifying that the error gets correctly propagated also when using
       Json::parseWhateverArray() */

    auto&& data = ParseArrayErrorData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    Containers::Optional<Json> json = Json::fromString(data.json);
    CORRADE_VERIFY(json);

    Containers::String out;
    Error redirectError{&out};
    CORRADE_VERIFY(!data.function(*json));
    CORRADE_COMPARE(out, format("Utility::Json::{} at <in>:2:4\n", data.message));
}

void JsonTest::parseTokenNotOwned() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString("{}");
    Containers::Optional<Json> json2 = Json::fromString("{}");
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json2);

    /* Taking a token from the second instance but then parsing it with the
       first */
    JsonToken token = json2->root();

    Containers::String out;
    Error redirectError{&out};
    json->parseLiterals(token);
    json->parseDoubles(token);
    json->parseFloats(token);
    json->parseUnsignedInts(token);
    json->parseInts(token);
    json->parseUnsignedLongs(token);
    json->parseLongs(token);
    json->parseSizes(token);
    json->parseStringKeys(token);
    json->parseStrings(token);

    json->parseObject(token);
    json->parseArray(token);
    json->parseNull(token);
    json->parseBool(token);
    json->parseDouble(token);
    json->parseFloat(token);
    json->parseUnsignedInt(token);
    json->parseInt(token);
    json->parseUnsignedLong(token);
    json->parseLong(token);
    json->parseSize(token);
    json->parseString(token);

    json->parseBitArray(token);
    json->parseDoubleArray(token);
    json->parseFloatArray(token);
    json->parseUnsignedIntArray(token);
    json->parseIntArray(token);
    json->parseUnsignedLongArray(token);
    json->parseLongArray(token);
    json->parseSizeArray(token);
    json->parseStringArray(token);
    const char* expected =
        "Utility::Json::parseLiterals(): token not owned by the instance\n"
        "Utility::Json::parseDoubles(): token not owned by the instance\n"
        "Utility::Json::parseFloats(): token not owned by the instance\n"
        "Utility::Json::parseUnsignedInts(): token not owned by the instance\n"
        "Utility::Json::parseInts(): token not owned by the instance\n"
        "Utility::Json::parseUnsignedLongs(): token not owned by the instance\n"
        "Utility::Json::parseLongs(): token not owned by the instance\n"
        #ifndef CORRADE_TARGET_32BIT
        "Utility::Json::parseUnsignedLongs(): token not owned by the instance\n"
        #else
        "Utility::Json::parseUnsignedInts(): token not owned by the instance\n"
        #endif
        "Utility::Json::parseStringKeys(): token not owned by the instance\n"
        "Utility::Json::parseStrings(): token not owned by the instance\n"

        "Utility::Json::parseObject(): token not owned by the instance\n"
        "Utility::Json::parseArray(): token not owned by the instance\n"
        "Utility::Json::parseNull(): token not owned by the instance\n"
        "Utility::Json::parseBool(): token not owned by the instance\n"
        "Utility::Json::parseDouble(): token not owned by the instance\n"
        "Utility::Json::parseFloat(): token not owned by the instance\n"
        "Utility::Json::parseUnsignedInt(): token not owned by the instance\n"
        "Utility::Json::parseInt(): token not owned by the instance\n"
        "Utility::Json::parseUnsignedLong(): token not owned by the instance\n"
        "Utility::Json::parseLong(): token not owned by the instance\n"
        "Utility::Json::parseSize(): token not owned by the instance\n"
        "Utility::Json::parseString(): token not owned by the instance\n"

        "Utility::Json::parseBitArray(): token not owned by the instance\n"
        "Utility::Json::parseDoubleArray(): token not owned by the instance\n"
        "Utility::Json::parseFloatArray(): token not owned by the instance\n"
        "Utility::Json::parseUnsignedIntArray(): token not owned by the instance\n"
        "Utility::Json::parseIntArray(): token not owned by the instance\n"
        "Utility::Json::parseUnsignedLongArray(): token not owned by the instance\n"
        "Utility::Json::parseLongArray(): token not owned by the instance\n"
        #ifndef CORRADE_TARGET_32BIT
        "Utility::Json::parseUnsignedLongArray(): token not owned by the instance\n"
        #else
        "Utility::Json::parseUnsignedIntArray(): token not owned by the instance\n"
        #endif
        "Utility::Json::parseStringArray(): token not owned by the instance\n";
    CORRADE_COMPARE(out, expected);
}

void JsonTest::tokenFromTokenData() {
    Containers::Optional<Json> json = Json::fromString("{\"a\": [0, 1], \"b\": false}");
    CORRADE_VERIFY(json);

    JsonToken token = json->tokens()[2];
    CORRADE_COMPARE(token.data(), "[0, 1]");

    const JsonTokenData& data = token.token();
    JsonToken tokenFromData{*json, data};
    CORRADE_COMPARE(&tokenFromData.token(), &token.token());
    CORRADE_COMPARE(tokenFromData.data(), "[0, 1]");
}

void JsonTest::tokenFromTokenDataNotOwned() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::StringView jsonData = "{\"a\": [0, 1], \"b\": false}"_s;
    Containers::Optional<Json> json1 = Json::fromString(jsonData);
    /* Same contents (even the same global view) but a different instance, so
       sharing tokens shouldn't be possible */
    Containers::Optional<Json> json2 = Json::fromString(jsonData);
    CORRADE_VERIFY(json1);
    CORRADE_VERIFY(json2);

    const JsonTokenData& data1 = json1->tokens()[2].token();
    const JsonTokenData& data2 = json2->tokens()[2].token();
    JsonTokenData data1Copy = json1->tokens()[2].token();
    JsonTokenData data2Copy = json2->tokens()[2].token();

    /* Both are fine */
    JsonToken{*json1, data1};
    JsonToken{*json2, data2};

    Containers::String out;
    Error redirectError{&out};
    JsonToken{*json1, data2};
    JsonToken{*json2, data1};
    /* Copies shouldn't work either as the token position is lost that way */
    JsonToken{*json1, data1Copy};
    JsonToken{*json2, data2Copy};
    CORRADE_COMPARE_AS(out,
        "Utility::JsonToken: token not owned by given Json instance\n"
        "Utility::JsonToken: token not owned by given Json instance\n"
        "Utility::JsonToken: token not owned by given Json instance\n"
        "Utility::JsonToken: token not owned by given Json instance\n",
        TestSuite::Compare::String);
}

void JsonTest::iterator() {
    Containers::Optional<Json> json = Json::fromString("{\"a\": [0, 1], \"b\": false}");
    CORRADE_VERIFY(json);

    JsonIterator a = json->root().children().begin();
    JsonIterator a2 = json->root().children().begin();
    JsonIterator a3 = json->root().children().begin();
    JsonIterator b = ++json->root().children().begin();
    JsonIterator b2 = ++json->root().children().begin();
    JsonIterator c;
    JsonIterator d = json->root().children().end();
    /* Construction from a token reference */
    JsonIterator e = json->root();

    CORRADE_VERIFY(a);
    CORRADE_VERIFY(b);
    CORRADE_VERIFY(!c);
    CORRADE_VERIFY(!d);
    CORRADE_VERIFY(e);
    CORRADE_VERIFY(a == a);
    CORRADE_VERIFY(c == c);
    CORRADE_VERIFY(a != b);
    CORRADE_VERIFY(a != c);
    CORRADE_VERIFY(b != a);
    CORRADE_VERIFY(++a2 == b);
    CORRADE_VERIFY(++e == a);
    /* Decrementing the first child iterator should go to the root object */
    CORRADE_VERIFY(--a3 == json->tokens()[0]);
    CORRADE_VERIFY(--b2 == a);
    CORRADE_VERIFY(--d == json->tokens()[6]);
    /* The operator->() is implemented by reinterpreting the JsonIterator as
       JsonToken, verify also that the member access gives expected values */
    CORRADE_COMPARE((*b).data(), "[0, 1]");
    CORRADE_COMPARE(&(*b).token(), &json->tokens()[2].token());
    CORRADE_COMPARE(b->data(), "[0, 1]");
    CORRADE_COMPARE(&b->token(), &json->tokens()[2].token());
}

void JsonTest::iteratorInvalid() {
    CORRADE_SKIP_IF_NO_DEBUG_ASSERT();

    /* Verify it works the same both on the default-constructed iterator and
       invalid iterator returned from APIs */
    Containers::Optional<Json> json = Json::fromString("{}");
    CORRADE_VERIFY(json);

    JsonIterator a = json->root().parent();
    JsonIterator b;
    JsonIterator c = json->root().children().end();
    JsonIterator d = json->root();
    JsonIterator e = json->root().children().end();

    Containers::String out;
    Error redirectError{&out};
    *a;
    *b;
    *c;
    /* I know, I know, but there's simply no other safe-to-call API that
       wouldn't lead to an immediate crash. The JsonToken is trivially
       destructible so this should be fine. */
    a->~JsonToken();
    b->~JsonToken();
    c->~JsonToken();
    ++b;
    --b;
    --d;
    ++e;
    CORRADE_COMPARE_AS(out,
        "Utility::JsonIterator::operator*(): the iterator is invalid\n"
        "Utility::JsonIterator::operator*(): the iterator is invalid\n"
        "Utility::JsonIterator::operator*(): the iterator is invalid\n"
        "Utility::JsonIterator::operator->(): the iterator is invalid\n"
        "Utility::JsonIterator::operator->(): the iterator is invalid\n"
        "Utility::JsonIterator::operator->(): the iterator is invalid\n"
        "Utility::JsonIterator::operator++(): the iterator is invalid\n"
        "Utility::JsonIterator::operator--(): the iterator is invalid\n"
        "Utility::JsonIterator::operator--(): advancing past the begin of the token stream\n"
        "Utility::JsonIterator::operator++(): advancing past the end of the token stream\n",
        TestSuite::Compare::String);
}

void JsonTest::iteratorObject() {
    Containers::Optional<Json> json = Json::fromString("{\"a\": 0, \"b\": 1}", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->root().isParsed());

    JsonObjectIterator a = json->root().asObject().begin();
    JsonObjectIterator b = ++json->root().asObject().begin();

    CORRADE_VERIFY(a == a);
    CORRADE_VERIFY(a != b);
    CORRADE_VERIFY(b != a);
    CORRADE_VERIFY(++a == b);
    CORRADE_COMPARE((*b).value().data(), "1");
}

void JsonTest::iteratorObjectInvalid() {
    CORRADE_SKIP_IF_NO_DEBUG_ASSERT();

    Containers::Optional<Json> json = Json::fromString("{}", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->root().isParsed());

    JsonObjectIterator a = json->root().asObject().end();

    Containers::String out;
    Error redirectError{&out};
    *a;
    ++a;
    CORRADE_COMPARE_AS(out,
        "Utility::JsonObjectIterator::operator*(): dereferencing iterator at the end of the token stream\n"
        "Utility::JsonObjectIterator::operator++(): advancing past the end of the token stream\n",
        TestSuite::Compare::String);
}

void JsonTest::iteratorArray() {
    Containers::Optional<Json> json = Json::fromString("[0, 1, 2]", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->root().isParsed());

    JsonArrayIterator a = json->root().asArray().begin();
    JsonArrayIterator b = ++json->root().asArray().begin();

    CORRADE_VERIFY(a == a);
    CORRADE_VERIFY(a != b);
    CORRADE_VERIFY(b != a);
    CORRADE_VERIFY(++a == b);
    CORRADE_COMPARE((*b).value().data(), "1");
}

void JsonTest::iteratorArrayInvalid() {
    CORRADE_SKIP_IF_NO_DEBUG_ASSERT();

    Containers::Optional<Json> json = Json::fromString("[]", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->root().isParsed());

    JsonArrayIterator a = json->root().asArray().end();

    Containers::String out;
    Error redirectError{&out};
    *a;
    ++a;
    CORRADE_COMPARE_AS(out,
        "Utility::JsonArrayIterator::operator*(): dereferencing iterator at the end of the token stream\n"
        "Utility::JsonArrayIterator::operator++(): advancing past the end of the token stream\n",
        TestSuite::Compare::String);
}

void JsonTest::iterate() {
    Containers::Optional<Json> json = Json::fromString("{\"a\": [0, 1], \"b\": false}");
    CORRADE_VERIFY(json);

    JsonView children = json->root().children();
    CORRADE_COMPARE(children.size(), json->root().childCount());
    CORRADE_COMPARE(children.size(), 6);
    CORRADE_VERIFY(!children.isEmpty());
    CORRADE_VERIFY(children.cbegin() == children.begin());
    CORRADE_VERIFY(children.cend() == children.end());
    CORRADE_VERIFY(children.begin() != children.end());
    CORRADE_COMPARE(children.begin()->data(), "\"a\"");
    CORRADE_COMPARE(children.front().data(), "\"a\"");
    CORRADE_COMPARE(children[0].data(), "\"a\"");
    CORRADE_COMPARE(children[3].data(), "1");
    CORRADE_COMPARE((--children.end())->data(), "false");
    CORRADE_COMPARE(children.back().data(), "false");
    CORRADE_COMPARE(children[5].data(), "false");

    Containers::Array<Containers::StringView> data;
    for(JsonToken a: children)
        arrayAppend(data, a.data());

    CORRADE_COMPARE_AS(data, (Containers::arrayView<Containers::StringView>({
        "\"a\"",
        "[0, 1]", /* the array token spans it as a whole */
        "0",
        "1",
        "\"b\"",
        "false"
    })), TestSuite::Compare::Container);

    JsonView empty = children[2].children();
    CORRADE_COMPARE(empty.size(), 0);
    CORRADE_VERIFY(empty.isEmpty());
    CORRADE_VERIFY(empty.begin() == empty.end());
    CORRADE_VERIFY(empty.cbegin() == empty.cend());
}

void JsonTest::iterateOutOfRange() {
    CORRADE_SKIP_IF_NO_DEBUG_ASSERT();

    Containers::Optional<Json> json = Json::fromString("{\"a\": [0, 1], \"b\": false}");
    CORRADE_VERIFY(json);

    JsonView children = json->root().children();
    JsonView empty = children[3].children();
    CORRADE_COMPARE(children.size(), 6);
    CORRADE_COMPARE(empty.size(), 0);

    Containers::String out;
    Error redirectError{&out};
    children[6];
    empty.front();
    empty.back();
    empty[0];
    CORRADE_COMPARE_AS(out,
        "Utility::JsonView::operator[](): index 6 out of range for 6 elements\n"
        "Utility::JsonView::front(): view is empty\n"
        "Utility::JsonView::back(): view is empty\n"
        "Utility::JsonView::operator[](): index 0 out of range for 0 elements\n",
        TestSuite::Compare::String);
}

void JsonTest::iterateObject() {
    Containers::Optional<Json> json = Json::fromString(R"({
        "hello": 3,
        "this": ["is"],
        "an": {"object": true}
    })", Json::Option::ParseLiterals|Json::Option::ParseStringKeys);
    CORRADE_VERIFY(json);

    JsonObjectView object = json->root().asObject();
    CORRADE_VERIFY(object.cbegin() == object.begin());
    CORRADE_VERIFY(object.cend() == object.end());
    CORRADE_VERIFY(object.begin() != object.end());
    /* find() and operator[]() tested in parsedObjectFind() */

    Containers::Array<Containers::Pair<Containers::StringView, Containers::StringView>> data;
    for(JsonObjectItem a: object)
        arrayAppend(data, InPlaceInit, a.key(), a.value().data());

    CORRADE_COMPARE_AS(data, (Containers::arrayView<Containers::Pair<Containers::StringView, Containers::StringView>>({
        {"hello", "3"},
        {"this", "[\"is\"]"},
        {"an", "{\"object\": true}"}
    })), TestSuite::Compare::Container);
}

void JsonTest::iterateObjectTokens() {
    Containers::Optional<Json> json = Json::fromString(R"({
        "hello": 3,
        "this": ["is"],
        "an": {"object": true}
    })", Json::Option::ParseLiterals|Json::Option::ParseStringKeys);
    CORRADE_VERIFY(json);

    Containers::Array<Containers::StringView> data;
    for(JsonToken a: json->root().asObject())
        arrayAppend(data, a.data());

    CORRADE_COMPARE_AS(data, Containers::arrayView({
        "\"hello\""_s,
        "\"this\""_s,
        "\"an\""_s
    }), TestSuite::Compare::Container);
}

void JsonTest::iterateObjectNotObject() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString("[]", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);

    Containers::String out;
    Error redirectError{&out};
    json->root().asObject();
    CORRADE_COMPARE(out, "Utility::JsonToken::asObject(): token is a parsed Utility::JsonToken::Type::Array\n");
}

void JsonTest::iterateObjectNotParsed() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString("{}");
    CORRADE_VERIFY(json);

    Containers::String out;
    Error redirectError{&out};
    json->root().asObject();
    CORRADE_COMPARE(out, "Utility::JsonToken::asObject(): token is an unparsed Utility::JsonToken::Type::Object\n");
}

void JsonTest::iterateObjectKeyNotParsed() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString("{\"key\": false}", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);

    Containers::String out;
    Error redirectError{&out};
    (*json->root().asObject().begin()).key();
    CORRADE_COMPARE(out, "Utility::JsonObjectItem::key(): string isn't parsed\n");
}

void JsonTest::iterateArray() {
    Containers::Optional<Json> json = Json::fromString(R"([
        "hello",
        ["this", "is"],
        {"an": "array"}
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);

    JsonArrayView array = json->root().asArray();
    CORRADE_VERIFY(array.cbegin() == array.begin());
    CORRADE_VERIFY(array.cend() == array.end());
    CORRADE_VERIFY(array.begin() != array.end());
    /* find() and operator[]() tested in parsedArrayFind() */

    Containers::Array<Containers::Pair<std::size_t, Containers::StringView>> data;
    for(JsonArrayItem a: array)
        arrayAppend(data, InPlaceInit, a.index(), a.value().data());

    CORRADE_COMPARE_AS(data, (Containers::arrayView<Containers::Pair<std::size_t, Containers::StringView>>({
        {0, "\"hello\""},
        {1, "[\"this\", \"is\"]"},
        {2, "{\"an\": \"array\"}"}
    })), TestSuite::Compare::Container);
}

void JsonTest::iterateArrayTokens() {
    Containers::Optional<Json> json = Json::fromString(R"([
        "hello",
        ["this", "is"],
        {"an": "array"}
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);

    Containers::Array<Containers::StringView> tokens;
    for(JsonToken a: json->root().asArray())
        arrayAppend(tokens, a.data());

    CORRADE_COMPARE_AS(tokens, Containers::arrayView({
        "\"hello\""_s,
        "[\"this\", \"is\"]"_s,
        "{\"an\": \"array\"}"_s
    }), TestSuite::Compare::Container);
}

void JsonTest::iterateArrayNotArray() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString("{}", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);

    Containers::String out;
    Error redirectError{&out};
    json->root().asArray();
    CORRADE_COMPARE(out, "Utility::JsonToken::asArray(): token is a parsed Utility::JsonToken::Type::Object\n");
}

void JsonTest::iterateArrayNotParsed() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString("[]");
    CORRADE_VERIFY(json);

    Containers::String out;
    Error redirectError{&out};
    json->root().asArray();
    CORRADE_COMPARE(out, "Utility::JsonToken::asArray(): token is an unparsed Utility::JsonToken::Type::Array\n");
}

void JsonTest::findObjectKey() {
    Containers::Optional<Json> json = Json::fromString(R"({
        "hello": 3,
        "this": ["or", "that"],
        "wherever": true
    })", Json::Option::ParseLiterals|Json::Option::ParseStringKeys);
    CORRADE_VERIFY(json);

    JsonIterator found = json->root().find("this");
    CORRADE_VERIFY(found);
    CORRADE_COMPARE(found->data(), "[\"or\", \"that\"]");
    CORRADE_COMPARE(json->root()["this"].data(), "[\"or\", \"that\"]");

    /* It especially shouldn't look into subobjects or whatnot */
    CORRADE_VERIFY(!json->root().find("that"));
    /* operator[] tested below as it asserts */
}

void JsonTest::findObjectKeyNotFound() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"({
        "hello": 3,
        "this": ["or", "that"],
        "wherever": true
    })", Json::Option::ParseLiterals|Json::Option::ParseStringKeys);
    CORRADE_VERIFY(json);

    Containers::String out;
    Error redirectError{&out};
    json->root()["that"];
    CORRADE_COMPARE(out, "Utility::JsonToken::operator[](): key that not found\n");
}

void JsonTest::findObjectKeyNotObject() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString("[]", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);

    Containers::String out;
    Error redirectError{&out};
    json->root().find("this");
    json->root()["this"];
    CORRADE_COMPARE(out,
        "Utility::JsonToken::find(): token is a parsed Utility::JsonToken::Type::Array, expected a parsed object\n"
        /* operator[]() delegates to find(), so the error is the same */
        "Utility::JsonToken::find(): token is a parsed Utility::JsonToken::Type::Array, expected a parsed object\n");
}

void JsonTest::findObjectKeyNotParsed() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString("{}");
    CORRADE_VERIFY(json);

    Containers::String out;
    Error redirectError{&out};
    json->root().find("this");
    json->root()["this"];
    CORRADE_COMPARE(out,
        "Utility::JsonToken::find(): token is an unparsed Utility::JsonToken::Type::Object, expected a parsed object\n"
        /* operator[]() delegates to find(), so the error is the same */
        "Utility::JsonToken::find(): token is an unparsed Utility::JsonToken::Type::Object, expected a parsed object\n");
}

void JsonTest::findObjectKeyKeyNotParsed() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"({
        "hello": 3,
        "this": ["or", "that"],
        "wherever": true
    })", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    /* Parse "hello" and "wherever" but not "this" */
    CORRADE_VERIFY(json->parseStrings(json->tokens()[1]));
    CORRADE_VERIFY(json->parseStrings(json->tokens()[6]));

    Containers::String out;
    Error redirectError{&out};
    json->root().find("this");
    json->root()["this"];
    CORRADE_COMPARE(out,
        "Utility::JsonToken::find(): key string isn't parsed\n"
        /* operator[]() delegates to find(), so the error is the same */
        "Utility::JsonToken::find(): key string isn't parsed\n");
}

void JsonTest::findArrayIndex() {
    Containers::Optional<Json> json = Json::fromString(R"([
        "hello",
        ["this", "is"],
        {"an": "array"}
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);

    JsonIterator found = json->root().find(1);
    CORRADE_VERIFY(found);
    CORRADE_COMPARE(found->data(), "[\"this\", \"is\"]");
    CORRADE_COMPARE(json->root()[1].data(), "[\"this\", \"is\"]");

    /* It especially shoulnd't count also nested tokens */
    CORRADE_VERIFY(!json->root().find(3));
    /* operator[] tested below as it asserts */
}

void JsonTest::findArrayIndexNotFound() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        "hello",
        ["this", "is"],
        {"an": "array"}
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);

    Containers::String out;
    Error redirectError{&out};
    json->root()[3];
    CORRADE_COMPARE(out, "Utility::JsonToken::operator[](): index 3 not found\n");
}

void JsonTest::findArrayIndexNotArray() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString("{}", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);

    Containers::String out;
    Error redirectError{&out};
    json->root().find(1);
    json->root()[1];
    CORRADE_COMPARE(out,
        "Utility::JsonToken::find(): token is a parsed Utility::JsonToken::Type::Object, expected a parsed array\n"
        /* operator[]() delegates to find(), so the error is the same */
        "Utility::JsonToken::find(): token is a parsed Utility::JsonToken::Type::Object, expected a parsed array\n");
}

void JsonTest::findArrayIndexNotParsed() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString("[]");
    CORRADE_VERIFY(json);

    Containers::String out;
    Error redirectError{&out};
    json->root().find(1);
    json->root()[1];
    CORRADE_COMPARE(out,
        "Utility::JsonToken::find(): token is an unparsed Utility::JsonToken::Type::Array, expected a parsed array\n"
        /* operator[]() delegates to find(), so the error is the same */
        "Utility::JsonToken::find(): token is an unparsed Utility::JsonToken::Type::Array, expected a parsed array\n");
}

void JsonTest::commonArrayType() {
    auto&& data = CommonArrayTypeData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    Containers::Optional<Json> json = Json::fromString(data.json);
    CORRADE_VERIFY(json);
    CORRADE_COMPARE(json->root().commonArrayType(), data.expected);
    /* By default nothing is parsed */
    CORRADE_COMPARE(json->root().commonParsedArrayType(), Containers::NullOpt);

    json->parseFloats(json->root());
    json->parseLiterals(json->root());
    json->parseStrings(json->root());
    /* The array type detection should work the same way after parsing */
    CORRADE_COMPARE(json->root().commonArrayType(), data.expected);
    CORRADE_COMPARE(json->root().commonParsedArrayType(), data.expectedParsed);
}

void JsonTest::commonArrayTypeParsedHeterogeneous() {
    Containers::Optional<Json> json = Json::fromString("[37.5, 4294967295, -33]");
    CORRADE_VERIFY(json);
    CORRADE_COMPARE(json->root().commonArrayType(), JsonToken::Type::Number);

    /* Parse everything as a double */
    json->parseLiterals(json->root());
    json->parseDoubles(json->root());
    CORRADE_COMPARE(json->root().commonParsedArrayType(), JsonToken::ParsedType::Double);

    /* Parse the middle element as a float, now it's no longer a common type */
    json->parseUnsignedInts(json->root()[1]);
    CORRADE_COMPARE(json->root().commonParsedArrayType(), Containers::NullOpt);
}

void JsonTest::commonArrayTypeParsedArrayNotParsed() {
    Containers::Optional<Json> json = Json::fromString("[37, -2147483648, -33]");
    CORRADE_VERIFY(json);
    CORRADE_COMPARE(json->root().commonArrayType(), JsonToken::Type::Number);

    /* Should still result in a */
    json->parseInts(json->root());
    CORRADE_COMPARE(json->root().commonParsedArrayType(), JsonToken::ParsedType::Int);
}

void JsonTest::commonArrayTypeNotArray() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString("35");
    CORRADE_VERIFY(json);

    Containers::String out;
    Error redirectError{&out};
    json->root().commonArrayType();
    json->root().commonParsedArrayType();
    CORRADE_COMPARE(out,
        "Utility::JsonToken::commonArrayType(): token is a Utility::JsonToken::Type::Number, expected an array\n"
        "Utility::JsonToken::commonParsedArrayType(): token is a Utility::JsonToken::Type::Number, expected an array\n");
}

void JsonTest::asTypeWrongType() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString("[{}]", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);

    Containers::String out;
    Error redirectError{&out};
    json->tokens()[0].asObject();
    json->tokens()[1].asArray();
    json->tokens()[1].asNull();
    json->tokens()[1].asBool();
    json->tokens()[1].asDouble();
    json->tokens()[1].asFloat();
    json->tokens()[1].asUnsignedInt();
    json->tokens()[1].asInt();
    json->tokens()[1].asUnsignedLong();
    json->tokens()[1].asLong();
    json->tokens()[1].asSize();
    json->tokens()[1].asString();
    const char* expected =
        "Utility::JsonToken::asObject(): token is a parsed Utility::JsonToken::Type::Array\n"
        "Utility::JsonToken::asArray(): token is a parsed Utility::JsonToken::Type::Object\n"
        "Utility::JsonToken::asNull(): token is a parsed Utility::JsonToken::Type::Object\n"
        "Utility::JsonToken::asBool(): token is a parsed Utility::JsonToken::Type::Object\n"
        "Utility::JsonToken::asDouble(): token is a Utility::JsonToken::Type::Object parsed as Utility::JsonToken::ParsedType::Other\n"
        "Utility::JsonToken::asFloat(): token is a Utility::JsonToken::Type::Object parsed as Utility::JsonToken::ParsedType::Other\n"
        "Utility::JsonToken::asUnsignedInt(): token is a Utility::JsonToken::Type::Object parsed as Utility::JsonToken::ParsedType::Other\n"
        "Utility::JsonToken::asInt(): token is a Utility::JsonToken::Type::Object parsed as Utility::JsonToken::ParsedType::Other\n"
        "Utility::JsonToken::asUnsignedLong(): token is a Utility::JsonToken::Type::Object parsed as Utility::JsonToken::ParsedType::Other\n"
        "Utility::JsonToken::asLong(): token is a Utility::JsonToken::Type::Object parsed as Utility::JsonToken::ParsedType::Other\n"
        "Utility::JsonToken::asSize(): token is a Utility::JsonToken::Type::Object parsed as Utility::JsonToken::ParsedType::Other\n"
        "Utility::JsonToken::asString(): token is a parsed Utility::JsonToken::Type::Object\n";
    CORRADE_COMPARE(out, expected);
}

void JsonTest::asTypeNotParsed() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        nOOO, fALSE, -yey, "\uhh", {}
    ])");
    CORRADE_VERIFY(json);

    Containers::String out;
    Error redirectError{&out};
    json->tokens()[5].asObject();
    json->tokens()[0].asArray();
    json->tokens()[1].asNull();
    json->tokens()[2].asBool();
    json->tokens()[3].asDouble();
    json->tokens()[3].asFloat();
    json->tokens()[3].asUnsignedInt();
    json->tokens()[3].asInt();
    json->tokens()[3].asUnsignedLong();
    json->tokens()[3].asLong();
    json->tokens()[3].asSize();
    json->tokens()[4].asString();
    const char* expected =
        "Utility::JsonToken::asObject(): token is an unparsed Utility::JsonToken::Type::Object\n"
        "Utility::JsonToken::asArray(): token is an unparsed Utility::JsonToken::Type::Array\n"
        "Utility::JsonToken::asNull(): token is an unparsed Utility::JsonToken::Type::Null\n"
        "Utility::JsonToken::asBool(): token is an unparsed Utility::JsonToken::Type::Bool\n"
        "Utility::JsonToken::asDouble(): token is a Utility::JsonToken::Type::Number parsed as Utility::JsonToken::ParsedType::None\n"
        "Utility::JsonToken::asFloat(): token is a Utility::JsonToken::Type::Number parsed as Utility::JsonToken::ParsedType::None\n"
        "Utility::JsonToken::asUnsignedInt(): token is a Utility::JsonToken::Type::Number parsed as Utility::JsonToken::ParsedType::None\n"
        "Utility::JsonToken::asInt(): token is a Utility::JsonToken::Type::Number parsed as Utility::JsonToken::ParsedType::None\n"
        "Utility::JsonToken::asUnsignedLong(): token is a Utility::JsonToken::Type::Number parsed as Utility::JsonToken::ParsedType::None\n"
        "Utility::JsonToken::asLong(): token is a Utility::JsonToken::Type::Number parsed as Utility::JsonToken::ParsedType::None\n"
        "Utility::JsonToken::asSize(): token is a Utility::JsonToken::Type::Number parsed as Utility::JsonToken::ParsedType::None\n"
        "Utility::JsonToken::asString(): token is an unparsed Utility::JsonToken::Type::String\n";
    CORRADE_COMPARE(out, expected);
}

void JsonTest::asTypeWrongParsedType() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        35.7, -35.7, 25, -17
    ])");
    CORRADE_VERIFY(json);
    CORRADE_COMPARE(json->tokens().size(), 5);

    json->parseDoubles(json->tokens()[1]);
    json->parseFloats(json->tokens()[2]);
    json->parseUnsignedInts(json->tokens()[3]);
    json->parseInts(json->tokens()[4]);

    /* Deliberately trying to get doubles as floats or ints as longs. Currently
       that fails but might be deemed too restrictive in future and relaxed. */
    Containers::String out;
    Error redirectError{&out};
    json->tokens()[2].asDouble();
    json->tokens()[1].asFloat();
    json->tokens()[4].asUnsignedInt();
    json->tokens()[3].asInt();
    json->tokens()[3].asUnsignedLong();
    json->tokens()[4].asLong();
    json->tokens()[4].asSize();
    const char* expected =
        "Utility::JsonToken::asDouble(): token is a Utility::JsonToken::Type::Number parsed as Utility::JsonToken::ParsedType::Float\n"
        "Utility::JsonToken::asFloat(): token is a Utility::JsonToken::Type::Number parsed as Utility::JsonToken::ParsedType::Double\n"
        "Utility::JsonToken::asUnsignedInt(): token is a Utility::JsonToken::Type::Number parsed as Utility::JsonToken::ParsedType::Int\n"
        "Utility::JsonToken::asInt(): token is a Utility::JsonToken::Type::Number parsed as Utility::JsonToken::ParsedType::UnsignedInt\n"
        "Utility::JsonToken::asUnsignedLong(): token is a Utility::JsonToken::Type::Number parsed as Utility::JsonToken::ParsedType::UnsignedInt\n"
        "Utility::JsonToken::asLong(): token is a Utility::JsonToken::Type::Number parsed as Utility::JsonToken::ParsedType::Int\n"
        "Utility::JsonToken::asSize(): token is a Utility::JsonToken::Type::Number parsed as Utility::JsonToken::ParsedType::Int\n";
    CORRADE_COMPARE(out, expected);
}

void JsonTest::asBitArray() {
    Containers::Optional<Json> json = Json::fromString(R"([
        true, false, true
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);

    Containers::StridedBitArrayView1D out = json->root().asBitArray();
    CORRADE_COMPARE_AS(out,
        Containers::stridedArrayView({true, false, true}).sliceBit(0),
        TestSuite::Compare::Container);
}

#ifdef CORRADE_BUILD_DEPRECATED
void JsonTest::asBoolArrayDeprecated() {
    Containers::Optional<Json> json = Json::fromString(R"([
        true, false, true
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);

    CORRADE_IGNORE_DEPRECATED_PUSH
    Containers::StridedArrayView1D<const bool> out = json->root().asBoolArray();
    CORRADE_IGNORE_DEPRECATED_POP
    CORRADE_COMPARE_AS(out,
        Containers::arrayView({true, false, true}),
        TestSuite::Compare::Container);
}
#endif

void JsonTest::asBitArrayNotAllSame() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        true, false, 0
    ])", Json::Option::ParseLiterals|Json::Option::ParseFloats);
    CORRADE_VERIFY(json);

    Containers::String out;
    Error redirectError{&out};
    json->root().asBitArray();
    CORRADE_COMPARE(out,
        "Utility::JsonToken::asBitArray(): token 2 is a parsed Utility::JsonToken::Type::Number\n");
}

void JsonTest::asBitArrayNotAllParsed() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        true, false, true
    ])");
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseArray(json->tokens()[0]));
    CORRADE_VERIFY(json->parseLiterals(json->tokens()[1]));
    CORRADE_VERIFY(json->parseLiterals(json->tokens()[2]));

    Containers::String out;
    Error redirectError{&out};
    json->root().asBitArray();
    CORRADE_COMPARE(out,
        "Utility::JsonToken::asBitArray(): token 2 is an unparsed Utility::JsonToken::Type::Bool\n");
}

void JsonTest::asBitArrayUnexpectedSize() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        true, false, true
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);

    Containers::String out;
    Error redirectError{&out};
    json->root().asBitArray(4);
    CORRADE_COMPARE(out,
        "Utility::JsonToken::asBitArray(): expected a 4-element array, got 3\n");
}

void JsonTest::asDoubleArray() {
    Containers::Optional<Json> json = Json::fromString(R"([
        35.5, -17.25, 0.25
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseDoubles(json->root()));

    Containers::StridedArrayView1D<const double> out = json->root().asDoubleArray();
    CORRADE_COMPARE_AS(out,
        Containers::arrayView({35.5, -17.25, 0.25}),
        TestSuite::Compare::Container);
}

void JsonTest::asDoubleArrayNotAllSame() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        35.5, -17.25, 1
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseDoubles(json->tokens()[1]));
    CORRADE_VERIFY(json->parseDoubles(json->tokens()[2]));

    Containers::String out;
    Error redirectError{&out};
    json->root().asDoubleArray();
    CORRADE_COMPARE(out,
        "Utility::JsonToken::asDoubleArray(): token 2 is a Utility::JsonToken::Type::Number parsed as Utility::JsonToken::ParsedType::None\n");
}

void JsonTest::asDoubleArrayUnexpectedSize() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        35.5, -17.25, 0.25
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseDoubles(json->root()));

    Containers::String out;
    Error redirectError{&out};
    json->root().asDoubleArray(4);
    CORRADE_COMPARE(out,
        "Utility::JsonToken::asDoubleArray(): expected a 4-element array, got 3\n");
}

void JsonTest::asFloatArray() {
    Containers::Optional<Json> json = Json::fromString(R"([
        35.5, -17.25, 0.25
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseFloats(json->root()));

    Containers::StridedArrayView1D<const float> out = json->root().asFloatArray();
    CORRADE_COMPARE_AS(out,
        Containers::arrayView({35.5f, -17.25f, 0.25f}),
        TestSuite::Compare::Container);
}

void JsonTest::asFloatArrayNotAllSame() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        35.5, -17.25, 1
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseFloats(json->tokens()[1]));
    CORRADE_VERIFY(json->parseFloats(json->tokens()[2]));

    Containers::String out;
    Error redirectError{&out};
    json->root().asFloatArray();
    CORRADE_COMPARE(out,
        "Utility::JsonToken::asFloatArray(): token 2 is a Utility::JsonToken::Type::Number parsed as Utility::JsonToken::ParsedType::None\n");
}

void JsonTest::asFloatArrayUnexpectedSize() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        35.5, -17.25, 0.25
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseFloats(json->root()));

    Containers::String out;
    Error redirectError{&out};
    json->root().asFloatArray(4);
    CORRADE_COMPARE(out,
        "Utility::JsonToken::asFloatArray(): expected a 4-element array, got 3\n");
}

void JsonTest::asUnsignedIntArray() {
    Containers::Optional<Json> json = Json::fromString(R"([
        35, 17, 25
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseUnsignedInts(json->root()));

    Containers::StridedArrayView1D<const std::uint32_t> out = json->root().asUnsignedIntArray();
    CORRADE_COMPARE_AS(out,
        Containers::arrayView<std::uint32_t>({35, 17, 25}),
        TestSuite::Compare::Container);
}

void JsonTest::asUnsignedIntArrayNotAllSame() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        35, 17, 0.25
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseUnsignedInts(json->tokens()[1]));
    CORRADE_VERIFY(json->parseUnsignedInts(json->tokens()[2]));

    Containers::String out;
    Error redirectError{&out};
    json->root().asUnsignedIntArray();
    CORRADE_COMPARE(out,
        "Utility::JsonToken::asUnsignedIntArray(): token 2 is a Utility::JsonToken::Type::Number parsed as Utility::JsonToken::ParsedType::None\n");
}

void JsonTest::asUnsignedIntArrayUnexpectedSize() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        35, 17, 25
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseUnsignedInts(json->root()));

    Containers::String out;
    Error redirectError{&out};
    json->root().asUnsignedIntArray(4);
    CORRADE_COMPARE(out,
        "Utility::JsonToken::asUnsignedIntArray(): expected a 4-element array, got 3\n");
}

void JsonTest::asIntArray() {
    Containers::Optional<Json> json = Json::fromString(R"([
        35, -17, 25
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseInts(json->root()));

    Containers::StridedArrayView1D<const std::int32_t> out = json->root().asIntArray();
    CORRADE_COMPARE_AS(out,
        Containers::arrayView<std::int32_t>({35, -17, 25}),
        TestSuite::Compare::Container);
}

void JsonTest::asIntArrayNotAllSame() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        35, -17, 0.25
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseInts(json->tokens()[1]));
    CORRADE_VERIFY(json->parseInts(json->tokens()[2]));

    Containers::String out;
    Error redirectError{&out};
    json->root().asIntArray();
    CORRADE_COMPARE(out,
        "Utility::JsonToken::asIntArray(): token 2 is a Utility::JsonToken::Type::Number parsed as Utility::JsonToken::ParsedType::None\n");
}

void JsonTest::asIntArrayUnexpectedSize() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        35, -17, 25
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseInts(json->root()));

    Containers::String out;
    Error redirectError{&out};
    json->root().asIntArray(4);
    CORRADE_COMPARE(out,
        "Utility::JsonToken::asIntArray(): expected a 4-element array, got 3\n");
}

void JsonTest::asUnsignedLongArray() {
    Containers::Optional<Json> json = Json::fromString(R"([
        35, 17, 25
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseUnsignedLongs(json->root()));

    Containers::StridedArrayView1D<const std::uint64_t> out = json->root().asUnsignedLongArray();
    CORRADE_COMPARE_AS(out,
        Containers::arrayView<std::uint64_t>({35, 17, 25}),
        TestSuite::Compare::Container);
}

void JsonTest::asUnsignedLongArrayNotAllSame() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        35, 17, 0.25
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseUnsignedLongs(json->tokens()[1]));
    CORRADE_VERIFY(json->parseUnsignedLongs(json->tokens()[2]));

    Containers::String out;
    Error redirectError{&out};
    json->root().asUnsignedLongArray();
    CORRADE_COMPARE(out,
        "Utility::JsonToken::asUnsignedLongArray(): token 2 is a Utility::JsonToken::Type::Number parsed as Utility::JsonToken::ParsedType::None\n");
}

void JsonTest::asUnsignedLongArrayUnexpectedSize() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        35, 17, 25
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseUnsignedLongs(json->root()));

    Containers::String out;
    Error redirectError{&out};
    json->root().asUnsignedLongArray(4);
    CORRADE_COMPARE(out,
        "Utility::JsonToken::asUnsignedLongArray(): expected a 4-element array, got 3\n");
}

void JsonTest::asLongArray() {
    Containers::Optional<Json> json = Json::fromString(R"([
        35, -17, 25
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseLongs(json->root()));

    Containers::StridedArrayView1D<const std::int64_t> out = json->root().asLongArray();
    CORRADE_COMPARE_AS(out,
        Containers::arrayView<std::int64_t>({35, -17, 25}),
        TestSuite::Compare::Container);
}

void JsonTest::asLongArrayNotAllSame() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        35, -17, 0.25
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseLongs(json->tokens()[1]));
    CORRADE_VERIFY(json->parseLongs(json->tokens()[2]));

    Containers::String out;
    Error redirectError{&out};
    json->root().asLongArray();
    CORRADE_COMPARE(out,
        "Utility::JsonToken::asLongArray(): token 2 is a Utility::JsonToken::Type::Number parsed as Utility::JsonToken::ParsedType::None\n");
}

void JsonTest::asLongArrayUnexpectedSize() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        35, -17, 25
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseLongs(json->root()));

    Containers::String out;
    Error redirectError{&out};
    json->root().asLongArray(4);
    CORRADE_COMPARE(out,
        "Utility::JsonToken::asLongArray(): expected a 4-element array, got 3\n");
}

void JsonTest::asSizeArray() {
    Containers::Optional<Json> json = Json::fromString(R"([
        35, 17, 25
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseSizes(json->root()));

    Containers::StridedArrayView1D<const std::size_t> out = json->root().asSizeArray();
    CORRADE_COMPARE_AS(out,
        Containers::arrayView<std::size_t>({35, 17, 25}),
        TestSuite::Compare::Container);
}

void JsonTest::asSizeArrayNotAllSame() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        35, 17, 0.25
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseSizes(json->tokens()[1]));
    CORRADE_VERIFY(json->parseSizes(json->tokens()[2]));

    Containers::String out;
    Error redirectError{&out};
    json->root().asSizeArray();
    #ifndef CORRADE_TARGET_32BIT
    CORRADE_COMPARE(out,
        "Utility::JsonToken::asUnsignedLongArray(): token 2 is a Utility::JsonToken::Type::Number parsed as Utility::JsonToken::ParsedType::None\n");
    #else
    CORRADE_COMPARE(out,
        "Utility::JsonToken::asUnsignedIntArray(): token 2 is a Utility::JsonToken::Type::Number parsed as Utility::JsonToken::ParsedType::None\n");
    #endif
}

void JsonTest::asSizeArrayUnexpectedSize() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        35, 17, 25
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseSizes(json->root()));

    Containers::String out;
    Error redirectError{&out};
    json->root().asSizeArray(4);
    #ifndef CORRADE_TARGET_32BIT
    CORRADE_COMPARE(out,
        "Utility::JsonToken::asUnsignedLongArray(): expected a 4-element array, got 3\n");
    #else
    CORRADE_COMPARE(out,
        "Utility::JsonToken::asUnsignedIntArray(): expected a 4-element array, got 3\n");
    #endif
}

void JsonTest::asStringArray() {
    auto&& data = ParseStringArrayData[testCaseInstanceId()];
    setTestCaseDescription(data.name);

    Containers::Optional<Json> json = Json::fromString({R"([
        "hello", ",\t", "world"
    ])", data.flags}, Json::Option::ParseLiterals|Json::Option::ParseStrings);
    CORRADE_VERIFY(json);

    Containers::StringIterable out = json->root().asStringArray();
    CORRADE_COMPARE_AS(out, (Containers::StringIterable{
        "hello", ",\t", "world"
    }), TestSuite::Compare::Container);

    /* The second string is escaped so it's not global, the others are global
       only if the input was global as well */
    CORRADE_COMPARE(out[0].flags() & ~Containers::StringViewFlag::NullTerminated,
        data.flags & ~Containers::StringViewFlag::NullTerminated);
    CORRADE_COMPARE(out[1].flags(), Containers::StringViewFlag::NullTerminated);
    CORRADE_COMPARE(out[2].flags() & ~Containers::StringViewFlag::NullTerminated,
        data.flags & ~Containers::StringViewFlag::NullTerminated);
}

void JsonTest::asStringArrayNotAllSame() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        "hello", "world", false
    ])", Json::Option::ParseLiterals|Json::Option::ParseStrings);
    CORRADE_VERIFY(json);

    Containers::String out;
    Error redirectError{&out};
    json->root().asStringArray();
    CORRADE_COMPARE(out,
        "Utility::JsonToken::asStringArray(): token 2 is a parsed Utility::JsonToken::Type::Bool\n");
}

void JsonTest::asStringArrayNotAllParsed() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        "hello", ",\t", "world"
    ])", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);
    CORRADE_VERIFY(json->parseStrings(json->tokens()[1]));
    CORRADE_VERIFY(json->parseStrings(json->tokens()[2]));

    Containers::String out;
    Error redirectError{&out};
    json->root().asStringArray();
    CORRADE_COMPARE(out,
        "Utility::JsonToken::asStringArray(): token 2 is an unparsed Utility::JsonToken::Type::String\n");
}

void JsonTest::asStringArrayUnexpectedSize() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString(R"([
        "hello", ",\t", "world"
    ])", Json::Option::ParseLiterals|Json::Option::ParseStrings);
    CORRADE_VERIFY(json);

    Containers::String out;
    Error redirectError{&out};
    json->root().asStringArray(4);
    CORRADE_COMPARE(out,
        "Utility::JsonToken::asStringArray(): expected a 4-element array, got 3\n");
}

void JsonTest::asTypeArrayNotArray() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString("{}", Json::Option::ParseLiterals);
    CORRADE_VERIFY(json);

    Containers::String out;
    Error redirectError{&out};
    json->root().asBitArray();
    json->root().asDoubleArray();
    json->root().asFloatArray();
    json->root().asUnsignedIntArray();
    json->root().asIntArray();
    json->root().asUnsignedLongArray();
    json->root().asLongArray();
    json->root().asSizeArray();
    json->root().asStringArray();
    const char* expected =
        "Utility::JsonToken::asBitArray(): token is a parsed Utility::JsonToken::Type::Object\n"
        "Utility::JsonToken::asDoubleArray(): token is a parsed Utility::JsonToken::Type::Object\n"
        "Utility::JsonToken::asFloatArray(): token is a parsed Utility::JsonToken::Type::Object\n"
        "Utility::JsonToken::asUnsignedIntArray(): token is a parsed Utility::JsonToken::Type::Object\n"
        "Utility::JsonToken::asIntArray(): token is a parsed Utility::JsonToken::Type::Object\n"
        "Utility::JsonToken::asUnsignedLongArray(): token is a parsed Utility::JsonToken::Type::Object\n"
        "Utility::JsonToken::asLongArray(): token is a parsed Utility::JsonToken::Type::Object\n"
        #ifndef CORRADE_TARGET_32BIT
        "Utility::JsonToken::asUnsignedLongArray(): token is a parsed Utility::JsonToken::Type::Object\n"
        #else
        "Utility::JsonToken::asUnsignedIntArray(): token is a parsed Utility::JsonToken::Type::Object\n"
        #endif
        "Utility::JsonToken::asStringArray(): token is a parsed Utility::JsonToken::Type::Object\n";
    CORRADE_COMPARE(out, expected);
}

void JsonTest::asTypeArrayNotParsed() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Optional<Json> json = Json::fromString("[]");
    CORRADE_VERIFY(json);

    Containers::String out;
    Error redirectError{&out};
    json->root().asBitArray();
    json->root().asDoubleArray();
    json->root().asFloatArray();
    json->root().asUnsignedIntArray();
    json->root().asIntArray();
    json->root().asUnsignedLongArray();
    json->root().asLongArray();
    json->root().asSizeArray();
    json->root().asStringArray();
    const char* expected =
        "Utility::JsonToken::asBitArray(): token is an unparsed Utility::JsonToken::Type::Array\n"
        "Utility::JsonToken::asDoubleArray(): token is an unparsed Utility::JsonToken::Type::Array\n"
        "Utility::JsonToken::asFloatArray(): token is an unparsed Utility::JsonToken::Type::Array\n"
        "Utility::JsonToken::asUnsignedIntArray(): token is an unparsed Utility::JsonToken::Type::Array\n"
        "Utility::JsonToken::asIntArray(): token is an unparsed Utility::JsonToken::Type::Array\n"
        "Utility::JsonToken::asUnsignedLongArray(): token is an unparsed Utility::JsonToken::Type::Array\n"
        "Utility::JsonToken::asLongArray(): token is an unparsed Utility::JsonToken::Type::Array\n"
        #ifndef CORRADE_TARGET_32BIT
        "Utility::JsonToken::asUnsignedLongArray(): token is an unparsed Utility::JsonToken::Type::Array\n"
        #else
        "Utility::JsonToken::asUnsignedIntArray(): token is an unparsed Utility::JsonToken::Type::Array\n"
        #endif
        "Utility::JsonToken::asStringArray(): token is an unparsed Utility::JsonToken::Type::Array\n";
    CORRADE_COMPARE(out, expected);
}

void JsonTest::fromStringFilenameOffsetError() {
    Containers::String out;
    Error redirectError{&out};
    /* Also verify that empty filename behaves the same as no filename passed */
    CORRADE_VERIFY(!Json::fromString("{35: false}"));
    CORRADE_VERIFY(!Json::fromString("{35: false}", ""));
    CORRADE_VERIFY(!Json::fromString("{35: false}", "fail.json"));
    CORRADE_VERIFY(!Json::fromString("{35: false}", "fail.json", 17));
    CORRADE_VERIFY(!Json::fromString("{35: false}", "fail.json", 17, 25));
    CORRADE_COMPARE(out,
        "Utility::Json: expected \" or } but got 3 at <in>:1:2\n"
        "Utility::Json: expected \" or } but got 3 at <in>:1:2\n"
        "Utility::Json: expected \" or } but got 3 at fail.json:1:2\n"
        /* Counting from 1, so the offset doesn't get used as-is */
        "Utility::Json: expected \" or } but got 3 at fail.json:18:2\n"
        "Utility::Json: expected \" or } but got 3 at fail.json:18:27\n");
}

void JsonTest::fromStringFilenameOffsetErrorSubsequentLine() {
    Containers::String out;
    Error redirectError{&out};
    CORRADE_VERIFY(!Json::fromString("{\n  35: false}", "fail.json"));
    CORRADE_VERIFY(!Json::fromString("{\n  35: false}", "fail.json", 17, 25));
    CORRADE_COMPARE(out,
        "Utility::Json: expected \" or } but got 3 at fail.json:2:3\n"
        /* The column offset should get ignored for second and subsequent
           lines */
        "Utility::Json: expected \" or } but got 3 at fail.json:19:3\n");
}

void JsonTest::fromStringFilenameOffsetParseOptionError() {
    Containers::String out;
    Error redirectError{&out};
    CORRADE_VERIFY(!Json::fromString("[-haha]", Json::Option::ParseDoubles, "fail.json"));
    CORRADE_VERIFY(!Json::fromString("[-haha]", Json::Option::ParseDoubles, "fail.json", 17, 25));
    CORRADE_COMPARE(out,
        "Utility::Json::parseDoubles(): invalid floating-point literal -haha at fail.json:1:2\n"
        /* Counting from 1, so the offset doesn't get used as-is */
        "Utility::Json::parseDoubles(): invalid floating-point literal -haha at fail.json:18:27\n");
}

void JsonTest::fromStringFilenameOffsetParseError() {
    /* The filename should get remembered even for subsequent parse() calls */

    Containers::Optional<Json> json = Json::fromString("[-haha]", "fail.json", 17, 25);
    CORRADE_VERIFY(json);
    CORRADE_COMPARE(json->tokens().size(), 2);

    Containers::String out;
    Error redirectError{&out};
    CORRADE_VERIFY(!json->parseDoubles(json->root()));
    CORRADE_VERIFY(!json->parseDouble(json->tokens()[1]));
    CORRADE_COMPARE(out,
        "Utility::Json::parseDoubles(): invalid floating-point literal -haha at fail.json:18:27\n"
        /* Counting from 1, so the offset doesn't get used as-is */
        "Utility::Json::parseDouble(): invalid floating-point literal -haha at fail.json:18:27\n");
}

void JsonTest::fromFile() {
    /* The file has a parse error, but tokenization should succeed */
    Containers::Optional<Json> json = Json::fromFile(Path::join(JSON_TEST_DIR, "parse-error.json"));
    CORRADE_VERIFY(json);
    CORRADE_COMPARE(json->tokens().size(), 2);

    JsonToken array = json->tokens()[0];
    CORRADE_COMPARE(array.data(), "[\n    -haha\n]");
    CORRADE_COMPARE(array.type(), JsonToken::Type::Array);

    JsonToken number = json->tokens()[1];
    CORRADE_COMPARE(number.data(), "-haha");
    CORRADE_COMPARE(number.type(), JsonToken::Type::Number);
}

void JsonTest::fromFileReadError() {
    Containers::String out;
    Error redirectError{&out};
    CORRADE_VERIFY(!Json::fromFile("nonexistent"));
    /* There's an error from Path::read() before */
    CORRADE_COMPARE_AS(out,
        "\nUtility::Json::fromFile(): can't read nonexistent\n",
        TestSuite::Compare::StringHasSuffix);
}

void JsonTest::fromFileOptionReadError() {
    /* The options parameter is a separate file loading code path, test it as
       well */

    Containers::String out;
    Error redirectError{&out};
    CORRADE_VERIFY(!Json::fromFile("nonexistent", Json::Option::ParseStrings));
    /* There's an error from Path::read() before */
    CORRADE_COMPARE_AS(out,
        "\nUtility::Json::fromFile(): can't read nonexistent\n",
        TestSuite::Compare::StringHasSuffix);
}

void JsonTest::fromFileError() {
    Containers::String filename = Path::join(JSON_TEST_DIR, "error.json");

    Containers::String out;
    Error redirectError{&out};
    CORRADE_VERIFY(!Json::fromFile(filename));
    CORRADE_COMPARE(out, format("Utility::Json: expected a value but got ] at {}:3:1\n", filename));
}

void JsonTest::fromFileParseOptionError() {
    Containers::String filename = Path::join(JSON_TEST_DIR, "parse-error.json");

    Containers::String out;
    Error redirectError{&out};
    CORRADE_VERIFY(!Json::fromFile(filename, Json::Option::ParseDoubles));
    CORRADE_COMPARE(out, format("Utility::Json::parseDoubles(): invalid floating-point literal -haha at {}:2:5\n", filename));
}

void JsonTest::fromFileParseError() {
    /* The filename should get remembered even for subsequent parse() calls */

    Containers::String filename = Path::join(JSON_TEST_DIR, "parse-error.json");
    Containers::Optional<Json> json = Json::fromFile(filename);
    CORRADE_VERIFY(json);
    CORRADE_COMPARE(json->tokens().size(), 2);

    Containers::String out;
    Error redirectError{&out};
    CORRADE_VERIFY(!json->parseDoubles(json->root()));
    CORRADE_VERIFY(!json->parseDouble(json->tokens()[1]));
    CORRADE_COMPARE(out, format(
        "Utility::Json::parseDoubles(): invalid floating-point literal -haha at {0}:2:5\n"
        "Utility::Json::parseDouble(): invalid floating-point literal -haha at {0}:2:5\n", filename));
}

#ifndef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
void JsonTest::tokenConstructCopy() {
    CORRADE_VERIFY(std::is_trivially_copyable<JsonToken>{});
}
#endif

void JsonTest::tokenDataConstructInvalid() {
    CORRADE_SKIP_IF_NO_ASSERT();

    JsonTokenOffsetSize offsetSize;

    /* These are all fine */
    JsonTokenData{4503599627370495ull, offsetSize};
    JsonTokenData{4503599627370495ll, offsetSize};
    JsonTokenData{-4503599627370496ll, offsetSize};
    JsonTokenData{JsonToken::Type::Object, (1ull << 48) - 1};
    JsonTokenData{JsonToken::Type::Array, (1ull << 48) - 1};
    JsonTokenData{JsonToken::Type::String, (1ull << 48) - 1};

    Containers::String out;
    Error redirectError{&out};
    /* Invalid floats / doubles, too large 64-bit values. MinGW is stupid and
       has NAN as a double, while it's said to be a float by the standard and
       all other compilers. So have to cast in both cases. */
    JsonTokenData{float(NAN)};
    JsonTokenData{float(INFINITY)};
    JsonTokenData{double(NAN), offsetSize};
    JsonTokenData{double(INFINITY), offsetSize};
    JsonTokenData{4503599627370496ull, offsetSize};
    JsonTokenData{4503599627370496ll, offsetSize};
    JsonTokenData{-4503599627370497ll, offsetSize};
    /* Too many children / too large index */
    JsonTokenData{JsonToken::Type::Object, 1ull << 48};
    JsonTokenData{JsonToken::Type::Array, 1ull << 48};
    JsonTokenData{JsonToken::Type::String, 1ull << 48};
    /* Objects / arrays marked as strings */
    JsonTokenData{JsonToken::Type::Object, 0, true};
    JsonTokenData{JsonToken::Type::Array, 0, true};
    /* A type other than object / array / string passed to object / array /
       string constructor */
    JsonTokenData{JsonToken::Type::Bool, 0};
    /* MSVC (w/o clang-cl) before 2019 shows -nan(ind), clang-cl shows
       -nan(ind) in 2022 17.10 but not in 2022 17.11 */
    #if defined(CORRADE_TARGET_MSVC) && (_MSC_VER < 1920 /* MSVC <2019 */ || (defined(CORRADE_TARGET_CLANG_CL) && _MSC_VER < 1941 /* <17.11 */))
    CORRADE_COMPARE_AS(out,
       "Utility::JsonTokenData: invalid floating-point value -nan(ind)\n"
       "Utility::JsonTokenData: invalid floating-point value inf\n"
       "Utility::JsonTokenData: invalid floating-point value -nan(ind)\n"
       "Utility::JsonTokenData: invalid floating-point value inf\n"
       "Utility::JsonTokenData: too large integer value 4503599627370496\n"
       "Utility::JsonTokenData: too small or large integer value 4503599627370496\n"
       "Utility::JsonTokenData: too small or large integer value -4503599627370497\n"

        "Utility::JsonTokenData: expected child count to fit into 48 bits, got 281474976710656\n"
        "Utility::JsonTokenData: expected child count to fit into 48 bits, got 281474976710656\n"
        "Utility::JsonTokenData: expected escaped string index to be either all 1s or fit into 48 bits but got 281474976710656\n"

        "Utility::JsonTokenData: object or array can't be a key\n"
        "Utility::JsonTokenData: object or array can't be a key\n"

        "Utility::JsonTokenData: expected object, array or a string type but got Utility::JsonToken::Type::Bool\n",
        TestSuite::Compare::String);
    #else
    CORRADE_COMPARE_AS(out,
       "Utility::JsonTokenData: invalid floating-point value nan\n"
       "Utility::JsonTokenData: invalid floating-point value inf\n"
       "Utility::JsonTokenData: invalid floating-point value nan\n"
       "Utility::JsonTokenData: invalid floating-point value inf\n"
       "Utility::JsonTokenData: too large integer value 4503599627370496\n"
       "Utility::JsonTokenData: too small or large integer value 4503599627370496\n"
       "Utility::JsonTokenData: too small or large integer value -4503599627370497\n"

        "Utility::JsonTokenData: expected child count to fit into 48 bits, got 281474976710656\n"
        "Utility::JsonTokenData: expected child count to fit into 48 bits, got 281474976710656\n"
        "Utility::JsonTokenData: expected escaped string index to be either all 1s or fit into 48 bits but got 281474976710656\n"

        "Utility::JsonTokenData: object or array can't be a key\n"
        "Utility::JsonTokenData: object or array can't be a key\n"

        "Utility::JsonTokenData: expected object, array or a string type but got Utility::JsonToken::Type::Bool\n",
        TestSuite::Compare::String);
    #endif
}

#ifndef CORRADE_NO_STD_IS_TRIVIALLY_TRAITS
void JsonTest::tokenDataConstructCopy() {
    CORRADE_VERIFY(std::is_trivially_copyable<JsonTokenData>{});
}

void JsonTest::tokenOffsetSizeConstructCopy() {
    CORRADE_VERIFY(std::is_trivially_copyable<JsonTokenOffsetSize>{});
}
#endif

void JsonTest::constructCopy() {
    CORRADE_VERIFY(!std::is_copy_constructible<Json>{});
    CORRADE_VERIFY(!std::is_copy_assignable<Json>{});
}

void JsonTest::constructMove() {
    Containers::Optional<Json> a = Json::fromString("\"\\\\\"", Json::Option::ParseStrings);
    CORRADE_VERIFY(a);

    Json b = *Utility::move(a);
    CORRADE_COMPARE(b.root().type(), JsonToken::Type::String);
    CORRADE_COMPARE(b.root().data(), "\"\\\\\"");
    CORRADE_VERIFY(b.root().isParsed());
    CORRADE_COMPARE(b.root().asString(), "\\");

    Containers::Optional<Json> c = Json::fromString("{}");
    CORRADE_VERIFY(c);

    c = Utility::move(b);
    CORRADE_COMPARE(c->root().type(), JsonToken::Type::String);
    CORRADE_COMPARE(c->root().data(), "\"\\\\\"");
    CORRADE_VERIFY(c->root().isParsed());
    CORRADE_COMPARE(c->root().asString(), "\\");

    CORRADE_VERIFY(std::is_nothrow_move_constructible<Json>::value);
    CORRADE_VERIFY(std::is_nothrow_move_assignable<Json>::value);
}

void JsonTest::debugTokenType() {
    Containers::String out;
    Debug{&out} << JsonToken::Type::Number << JsonToken::Type(0xde);
    CORRADE_COMPARE(out, "Utility::JsonToken::Type::Number Utility::JsonToken::Type(0xde)\n");
}

void JsonTest::debugTokenParsedType() {
    Containers::String out;
    Debug{&out} << JsonToken::ParsedType::UnsignedInt << JsonToken::ParsedType(0xad);
    CORRADE_COMPARE(out, "Utility::JsonToken::ParsedType::UnsignedInt Utility::JsonToken::ParsedType(0xad)\n");
}

void JsonTest::fromTokenDataNull() {
    Utility::Json json{{"so, hello?"}, {InPlaceInit, {
        JsonTokenData{nullptr}
    }}, {InPlaceInit, {
        {4, 5}
    }}, {}};
    CORRADE_COMPARE(json.tokens().size(), 1);

    JsonToken root = json.root();
    CORRADE_COMPARE(root.type(), JsonToken::Type::Null);
    CORRADE_VERIFY(root.isParsed());
    CORRADE_COMPARE(root.parsedType(), JsonToken::ParsedType::Other);
    CORRADE_COMPARE(root.data(), "hello");
    CORRADE_COMPARE(root.asNull(), nullptr);
}

void JsonTest::fromTokenDataBool() {
    /* Can only test one value at a time because otherwise it'd have to be
       wrapped in an array which I want to test separately */

    {
        Json json{"well, yes!", {InPlaceInit, {
            JsonTokenData{true}
        }}, {InPlaceInit, {
            {6, 3}
        }}, {}};
        CORRADE_COMPARE(json.tokens().size(), 1);

        JsonToken root = json.root();
        CORRADE_COMPARE(root.type(), JsonToken::Type::Bool);
        CORRADE_VERIFY(root.isParsed());
        CORRADE_COMPARE(root.parsedType(), JsonToken::ParsedType::Other);
        CORRADE_COMPARE(root.data(), "yes");
        CORRADE_VERIFY(root.asBool());
    } {
        Json json{"NEIN", {InPlaceInit, {
            JsonTokenData{false}
        }}, {InPlaceInit, {
            {0, 4}
        }}, {}};
        CORRADE_COMPARE(json.tokens().size(), 1);

        JsonToken root = json.root();
        CORRADE_COMPARE(root.type(), JsonToken::Type::Bool);
        CORRADE_VERIFY(root.isParsed());
        CORRADE_COMPARE(root.parsedType(), JsonToken::ParsedType::Other);
        CORRADE_COMPARE(root.data(), "NEIN");
        CORRADE_VERIFY(!root.asBool());
    }
}

void JsonTest::fromTokenDataDouble() {
    /* The 64-bit constructors patch the associated JsonTokenOffsetSize, so I
       have to create the arrays separately to ensure correct initialization
       order */

    Containers::Array<JsonTokenOffsetSize> offsetsSizes{InPlaceInit, {
        {3, 7}
    }};
    Containers::Array<JsonTokenData> tokens{InPlaceInit, {
        JsonTokenData{3.14159, offsetsSizes.front()}
    }};

    Json json{"pi=3.1415926", Utility::move(tokens), Utility::move(offsetsSizes), {}};
    CORRADE_COMPARE(json.tokens().size(), 1);

    JsonToken root = json.root();
    CORRADE_COMPARE(root.type(), JsonToken::Type::Number);
    CORRADE_VERIFY(root.isParsed());
    CORRADE_COMPARE(root.parsedType(), JsonToken::ParsedType::Double);
    CORRADE_COMPARE(root.data(), "3.14159");
    CORRADE_COMPARE(root.asDouble(), 3.14159);
}

void JsonTest::fromTokenDataFloat() {
    Json json{"e = 2.many", {InPlaceInit, {
        JsonTokenData{2.73f}
    }}, {InPlaceInit, {
        {4, 2}
    }}, {}};
    CORRADE_COMPARE(json.tokens().size(), 1);

    JsonToken root = json.root();
    CORRADE_COMPARE(root.type(), JsonToken::Type::Number);
    CORRADE_VERIFY(root.isParsed());
    CORRADE_COMPARE(root.parsedType(), JsonToken::ParsedType::Float);
    CORRADE_COMPARE(root.data(), "2.");
    CORRADE_COMPARE(root.asFloat(), 2.73f);
}

void JsonTest::fromTokenDataUnsignedInt() {
    Json json{"the answer is 42!", {InPlaceInit, {
        JsonTokenData{42u}
    }}, {InPlaceInit, {
        {14, 3}
    }}, {}};
    CORRADE_COMPARE(json.tokens().size(), 1);

    JsonToken root = json.root();
    CORRADE_COMPARE(root.type(), JsonToken::Type::Number);
    CORRADE_VERIFY(root.isParsed());
    CORRADE_COMPARE(root.parsedType(), JsonToken::ParsedType::UnsignedInt);
    CORRADE_COMPARE(root.data(), "42!");
    CORRADE_COMPARE(root.asUnsignedInt(), 42);
}

void JsonTest::fromTokenDataInt() {
    Json json{"-42, is not the answer", {InPlaceInit, {
        JsonTokenData{-42}
    }}, {InPlaceInit, {
        {0, 4}
    }}, {}};
    CORRADE_COMPARE(json.tokens().size(), 1);

    JsonToken root = json.root();
    CORRADE_COMPARE(root.type(), JsonToken::Type::Number);
    CORRADE_VERIFY(root.isParsed());
    CORRADE_COMPARE(root.parsedType(), JsonToken::ParsedType::Int);
    CORRADE_COMPARE(root.data(), "-42,");
    CORRADE_COMPARE(root.asInt(), -42);
}

void JsonTest::fromTokenDataUnsignedLong() {
    /* The 64-bit constructors patch the associated JsonTokenOffsetSize, so I
       have to create the arrays separately to ensure correct initialization
       order */

    Containers::Array<JsonTokenOffsetSize> offsetsSizes{InPlaceInit, {
        {4, 13}
    }};
    Containers::Array<JsonTokenData> tokens{InPlaceInit, {
        JsonTokenData{5000000000ull, offsetsSizes.front()}
    }};

    Json json{"so, five billions?", Utility::move(tokens), Utility::move(offsetsSizes), {}};
    CORRADE_COMPARE(json.tokens().size(), 1);

    JsonToken root = json.root();
    CORRADE_COMPARE(root.type(), JsonToken::Type::Number);
    CORRADE_VERIFY(root.isParsed());
    CORRADE_COMPARE(root.parsedType(), JsonToken::ParsedType::UnsignedLong);
    CORRADE_COMPARE(root.data(), "five billions");
    CORRADE_COMPARE(root.asUnsignedLong(), 5000000000ull);
}

void JsonTest::fromTokenDataLong() {
    /* The 64-bit constructors patch the associated JsonTokenOffsetSize, so I
       have to create the arrays separately to ensure correct initialization
       order */

    Containers::Array<JsonTokenOffsetSize> offsetsSizes{InPlaceInit, {
        {6, 18}
    }};
    Containers::Array<JsonTokenData> tokens{InPlaceInit, {
        JsonTokenData{-5000000000ll, offsetsSizes.front()}
    }};

    Json json{"well, five billions less.", Utility::move(tokens), Utility::move(offsetsSizes), {}};
    CORRADE_COMPARE(json.tokens().size(), 1);

    JsonToken root = json.root();
    CORRADE_COMPARE(root.type(), JsonToken::Type::Number);
    CORRADE_VERIFY(root.isParsed());
    CORRADE_COMPARE(root.parsedType(), JsonToken::ParsedType::Long);
    CORRADE_COMPARE(root.data(), "five billions less");
    CORRADE_COMPARE(root.asLong(), -5000000000ll);
}

void JsonTest::fromTokenDataTheOtherUnsignedLongType() {
    /* Like fromTokenDataUnsignedLong() or fromTokenDataUnsignedInt() but with
       a `ul` literal instead of `u` / `ull` */

    #if ULONG_MAX == ULLONG_MAX
    Containers::Array<JsonTokenOffsetSize> offsetsSizes{InPlaceInit, {
        {4, 7}
    }};
    Containers::Array<JsonTokenData> tokens{InPlaceInit, {
        JsonTokenData{1000000ul, offsetsSizes.front()}
    }};
    Json json{"one million.", Utility::move(tokens), Utility::move(offsetsSizes), {}};
    #else
    Json json{"one million.", {InPlaceInit, {
        JsonTokenData{1000000ul}
    }}, {InPlaceInit, {
        {4, 7}
    }}, {}};
    #endif
    CORRADE_COMPARE(json.tokens().size(), 1);

    JsonToken root = json.root();
    CORRADE_COMPARE(root.type(), JsonToken::Type::Number);
    CORRADE_VERIFY(root.isParsed());
    CORRADE_COMPARE(root.data(), "million");
    #if ULONG_MAX == ULLONG_MAX
    CORRADE_COMPARE(root.parsedType(), JsonToken::ParsedType::UnsignedLong);
    CORRADE_COMPARE(root.asUnsignedLong(), 1000000);
    #else
    CORRADE_COMPARE(root.parsedType(), JsonToken::ParsedType::UnsignedInt);
    CORRADE_COMPARE(root.asUnsignedInt(), 1000000);
    #endif
}

void JsonTest::fromTokenDataTheOtherLongType() {
    /* Like fromTokenDataLong() or fromTokenDataInt() but with a `l` literal
       instead of `` / `ll` */

    #if LONG_MAX == LLONG_MAX
    Containers::Array<JsonTokenOffsetSize> offsetsSizes{InPlaceInit, {
        {4, 8}
    }};
    Containers::Array<JsonTokenData> tokens{InPlaceInit, {
        JsonTokenData{-1000000l, offsetsSizes.front()}
    }};
    Json json{"one -million.", Utility::move(tokens), Utility::move(offsetsSizes), {}};
    #else
    Json json{"one -million.", {InPlaceInit, {
        JsonTokenData{-1000000l}
    }}, {InPlaceInit, {
        {4, 8}
    }}, {}};
    #endif
    CORRADE_COMPARE(json.tokens().size(), 1);

    JsonToken root = json.root();
    CORRADE_COMPARE(root.type(), JsonToken::Type::Number);
    CORRADE_VERIFY(root.isParsed());
    CORRADE_COMPARE(root.data(), "-million");
    #if LONG_MAX == LLONG_MAX
    CORRADE_COMPARE(root.parsedType(), JsonToken::ParsedType::Long);
    CORRADE_COMPARE(root.asLong(), -1000000);
    #else
    CORRADE_COMPARE(root.parsedType(), JsonToken::ParsedType::Int);
    CORRADE_COMPARE(root.asInt(), -1000000);
    #endif
}

void JsonTest::fromTokenDataSize() {
    /* Like fromTokenDataUnsignedLong() or fromTokenDataUnsignedInt() but using
       std::size_t to verify it doesn't cause ambiguities */

    #ifndef CORRADE_TARGET_32BIT
    Containers::Array<JsonTokenOffsetSize> offsetsSizes{InPlaceInit, {
        {4, 7}
    }};
    Containers::Array<JsonTokenData> tokens{InPlaceInit, {
        JsonTokenData{std::size_t{1000000}, offsetsSizes.front()}
    }};
    Json json{"one million.", Utility::move(tokens), Utility::move(offsetsSizes), {}};
    #else
    Json json{"one million.", {InPlaceInit, {
        JsonTokenData{std::size_t{1000000}}
    }}, {InPlaceInit, {
        {4, 7}
    }}, {}};
    #endif
    CORRADE_COMPARE(json.tokens().size(), 1);

    JsonToken root = json.root();
    CORRADE_COMPARE(root.type(), JsonToken::Type::Number);
    CORRADE_VERIFY(root.isParsed());
    CORRADE_COMPARE(root.parsedType(), JsonToken::ParsedType::Size);
    CORRADE_COMPARE(root.data(), "million");
    CORRADE_COMPARE(root.asSize(), 1000000);
}

void JsonTest::fromTokenDataString() {
    Json json{"she said \"no :(\"...", {InPlaceInit, {
        JsonTokenData{JsonToken::Type::String, ~std::uint64_t{}}
    }}, {InPlaceInit, {
        {9, 7}
    }}, {}};
    CORRADE_COMPARE(json.tokens().size(), 1);

    JsonToken root = json.root();
    CORRADE_COMPARE(root.type(), JsonToken::Type::String);
    CORRADE_VERIFY(root.isParsed());
    CORRADE_COMPARE(root.parsedType(), JsonToken::ParsedType::Other);
    CORRADE_COMPARE(root.data(), "\"no :(\"");
    CORRADE_COMPARE(root.asString(), "no :(");
}

void JsonTest::fromTokenDataStringEscaped() {
    Json json{"she said \"\\\"no\\\" :)\"...", {InPlaceInit, {
        JsonTokenData{JsonToken::Type::String, 1}
    }}, {InPlaceInit, {
        {9, 11}
    }}, {InPlaceInit, {
        "YEAH", "\"no\" :)"
    }}};
    CORRADE_COMPARE(json.tokens().size(), 1);

    JsonToken root = json.root();
    CORRADE_COMPARE(root.type(), JsonToken::Type::String);
    CORRADE_VERIFY(root.isParsed());
    CORRADE_COMPARE(root.parsedType(), JsonToken::ParsedType::Other);
    CORRADE_COMPARE(root.data(), "\"\\\"no\\\" :)\"");
    CORRADE_COMPARE(root.asString(), "\"no\" :)");
}

void JsonTest::fromTokenDataArrayEmpty() {
    Json json{"nothing", {InPlaceInit, {
        JsonTokenData{JsonToken::Type::Array, 0}
    }}, {InPlaceInit, {
        {6, 0}
    }}, {}};
    CORRADE_COMPARE(json.tokens().size(), 1);

    JsonToken root = json.root();
    CORRADE_COMPARE(root.type(), JsonToken::Type::Array);
    CORRADE_VERIFY(root.isParsed());
    CORRADE_COMPARE(root.parsedType(), JsonToken::ParsedType::Other);
    CORRADE_COMPARE(root.data(), "");
    CORRADE_COMPARE(root.childCount(), 0);
    CORRADE_VERIFY(root.asArray().begin() == root.asArray().end());
}

void JsonTest::fromTokenDataArray() {
    Json json{"ar01234", {InPlaceInit, {
        JsonTokenData{JsonToken::Type::Array, 4},
        JsonTokenData{false},
        JsonTokenData{3.65f},
        JsonTokenData{nullptr},
        JsonTokenData{13},
    }}, {InPlaceInit, {
        {0, 2},
        {2, 1},
        {3, 1},
        {4, 1},
        {5, 1},
    }}, {}};
    CORRADE_COMPARE(json.tokens().size(), 5);

    JsonToken root = json.root();
    CORRADE_COMPARE(root.type(), JsonToken::Type::Array);
    CORRADE_VERIFY(root.isParsed());
    CORRADE_COMPARE(root.parsedType(), JsonToken::ParsedType::Other);
    CORRADE_COMPARE(root.data(), "ar");
    CORRADE_COMPARE(root.childCount(), 4);

    int i = 0;
    for(JsonArrayItem child: root.asArray()) {
        CORRADE_ITERATION(i);
        CORRADE_COMPARE(child.index(), i);
        char index[]{char('0' + i), '\0'};
        CORRADE_COMPARE(child.value().data(), index);
        CORRADE_COMPARE(JsonToken{child}.childCount(), 0);
        ++i;
    }
    CORRADE_COMPARE(i, 4);
}

void JsonTest::fromTokenDataObjectEmpty() {
    Json json{"nothing", {InPlaceInit, {
        JsonTokenData{JsonToken::Type::Object, 0}
    }}, {InPlaceInit, {
        {6, 0}
    }}, {}};
    CORRADE_COMPARE(json.tokens().size(), 1);

    JsonToken root = json.root();
    CORRADE_COMPARE(root.type(), JsonToken::Type::Object);
    CORRADE_VERIFY(root.isParsed());
    CORRADE_COMPARE(root.parsedType(), JsonToken::ParsedType::Other);
    CORRADE_COMPARE(root.data(), "");
    CORRADE_COMPARE(root.childCount(), 0);
    CORRADE_VERIFY(root.asObject().begin() == root.asObject().end());
}

void JsonTest::fromTokenDataObject() {
    Json json{"{\"a\"25\"X\"0.31\"c\"!", {InPlaceInit, {
        JsonTokenData{JsonToken::Type::Object, 6},
        JsonTokenData{JsonToken::Type::String, ~std::uint64_t{}, true},
        JsonTokenData{2255u},
        /* An escaped key */
        JsonTokenData{JsonToken::Type::String, 1, true},
        JsonTokenData{0.31f},
        JsonTokenData{JsonToken::Type::String, ~std::uint64_t{}, true},
        /* A (data-less) escaped string value as well */
        JsonTokenData{JsonToken::Type::String, 0}
    }}, {InPlaceInit, {
        {1, 15},
        {1, 3},
        {4, 2},
        {6, 3},
        {9, 4},
        {13, 3},
        {},
    }}, {InPlaceInit, {
        "hello",
        "b",
    }}};
    CORRADE_COMPARE(json.tokens().size(), 7);

    JsonToken root = json.root();
    CORRADE_COMPARE(root.type(), JsonToken::Type::Object);
    CORRADE_VERIFY(root.isParsed());
    CORRADE_COMPARE(root.parsedType(), JsonToken::ParsedType::Other);
    CORRADE_COMPARE(root.data(), "\"a\"25\"X\"0.31\"c\"");
    CORRADE_COMPARE(root.childCount(), 6);

    /* Key data should be wrapped in quotes, key values should be a, b, c
       independently of whether it's a direct reference or escaped */
    int i = 0;
    for(JsonObjectItem child: root.asObject()) {
        CORRADE_ITERATION(i);
        CORRADE_COMPARE(JsonToken{child}.data().front(), '"');
        CORRADE_COMPARE(JsonToken{child}.data().back(), '"');
        char key[]{char('a' + i), '\0'};
        CORRADE_COMPARE(child.key(), key);
        CORRADE_COMPARE(JsonToken{child}.childCount(), 1);
        ++i;
    }
    CORRADE_COMPARE(i, 3); /* There's six child tokens which is three keys */

    /* Values */
    JsonToken a = json.tokens()[2];
    CORRADE_COMPARE(a.type(), JsonToken::Type::Number);
    CORRADE_VERIFY(a.isParsed());
    CORRADE_COMPARE(a.parsedType(), JsonToken::ParsedType::UnsignedInt);
    CORRADE_COMPARE(a.data(), "25");
    CORRADE_COMPARE(a.childCount(), 0);
    CORRADE_COMPARE(a.asUnsignedInt(), 2255);

    JsonToken b = json.tokens()[4];
    CORRADE_COMPARE(b.type(), JsonToken::Type::Number);
    CORRADE_VERIFY(b.isParsed());
    CORRADE_COMPARE(b.parsedType(), JsonToken::ParsedType::Float);
    CORRADE_COMPARE(b.data(), "0.31");
    CORRADE_COMPARE(b.childCount(), 0);
    CORRADE_COMPARE(b.asFloat(), 0.31f);

    JsonToken c = json.tokens()[6];
    CORRADE_COMPARE(c.type(), JsonToken::Type::String);
    CORRADE_VERIFY(c.isParsed());
    CORRADE_COMPARE(c.parsedType(), JsonToken::ParsedType::Other);
    CORRADE_COMPARE(c.data(), "");
    CORRADE_COMPARE(c.childCount(), 0);
    CORRADE_COMPARE(c.asString(), "hello");
}

void JsonTest::fromTokenDataNested() {
    /* Especially verifying nested empty arrays/objects and multiple
       arrays/objects ending at the same token */

    Json json{"01\"A\"a56b\"I\"i82", {InPlaceInit, {
        /* Top-level array at level 0, items 0, 1, 2 */
        JsonTokenData{JsonToken::Type::Array, 12},
        /* Array nested at level 1, empty */
        JsonTokenData{JsonToken::Type::Array, 0},                       /* 0 */
        /* Object nested at level 2, items A, B */
        JsonTokenData{JsonToken::Type::Object, 9},                      /* 1 */
        JsonTokenData{JsonToken::Type::String, ~std::uint64_t{}, true}, /* A */
        /* Array nested at level 3, items 5, 6 */
        JsonTokenData{JsonToken::Type::Array, 2},                       /* a */
        JsonTokenData{false},                                           /* 5 */
        JsonTokenData{-33},                                             /* 6 */
        /* Back to level 2 */
        JsonTokenData{JsonToken::Type::String, 1, true},                /* B */
        /* Object nested at level 3, item I */
        JsonTokenData{JsonToken::Type::Object, 3},                      /* b */
        JsonTokenData{JsonToken::Type::String, ~std::uint64_t{}, true}, /* I */
        /* Array nested at level 4, item 8 */
        JsonTokenData{JsonToken::Type::Array, 1},                       /* i */
        /* Object nested at level 5, empty */
        JsonTokenData{JsonToken::Type::Object, 0},                      /* 8 */
        /* Back to level 4, 3, 2, 1 */
        JsonTokenData{2.5f}                                             /* 2 */
    }}, {InPlaceInit, {
        {},
        {0, 1},  /* 0 */
        {1, 1},  /* 1 */
        {2, 3},  /* "A" */
        {5, 1},  /* a */
        {6, 1},  /* 5 */
        {7, 1},  /* 6 */
        {},
        {8, 1},  /* b */
        {9, 3},  /* "I" */
        {12, 1}, /* i */
        {13, 1}, /* 8 */
        {14, 1}, /* 2 */
    }}, {InPlaceInit, {
        "", /* unused */
        "B"
    }}};
    CORRADE_COMPARE(json.tokens().size(), 13);

    /* Verifying just by iterating over array items and object keys, checking
       that the .data() are as expected */

    /* Top-level array at level 0, items 0, 1, 2 */
    int array0 = 0;
    for(JsonToken i: json.root().asArray()) {
        CORRADE_ITERATION(array0);
        char data[]{char('0' + array0), '\0'};
        CORRADE_COMPARE(i.data(), data);
        ++array0;
    }
    CORRADE_COMPARE(array0, 3);

    /* Array nested at level 1, empty */
    CORRADE_VERIFY(json.root()[0].asArray().begin() ==
                   json.root()[0].asArray().end());

    /* Object nested at level 2, items A, B */
    int objectA = 0;
    for(JsonObjectItem i: json.root()[1].asObject()) {
        CORRADE_ITERATION(objectA);
        char key[]{char('A' + objectA), '\0'};
        char data[]{char('a' + objectA), '\0'};
        CORRADE_COMPARE(i.key(), key);
        CORRADE_COMPARE(i.value().data(), data);
        ++objectA;
    }
    CORRADE_COMPARE(objectA, 2);

    /* Array nested at level 3, items 5, 6 */
    int array5 = 0;
    for(JsonToken i: json.root()[1]["A"].asArray()) {
        CORRADE_ITERATION(array5);
        char data[]{char('5' + array5), '\0'};
        CORRADE_COMPARE(i.data(), data);
        ++array5;
    }
    CORRADE_COMPARE(array5, 2);

    /* Object nested at level 3, item I */
    int objectB = 0;
    for(JsonObjectItem i: json.root()[1]["B"].asObject()) {
        CORRADE_ITERATION(objectB);
        char key[]{char('I' + objectB), '\0'};
        char data[]{char('i' + objectB), '\0'};
        CORRADE_COMPARE(i.key(), key);
        CORRADE_COMPARE(i.value().data(), data);
        ++objectB;
    }
    CORRADE_COMPARE(objectB, 1);

    /* Array nested at level 4, item 8 */
    int array8 = 0;
    for(JsonToken i: json.root()[1]["B"]["I"].asArray()) {
        CORRADE_ITERATION(array8);
        char data[]{char('8' + array8), '\0'};
        CORRADE_COMPARE(i.data(), data);
        ++array8;
    }
    CORRADE_COMPARE(array8, 1);

    /* Object nested at level 5, empty */
    CORRADE_VERIFY(json.root()[1]["B"]["I"][0].asObject().begin() ==
                   json.root()[1]["B"]["I"][0].asObject().end());
}

void JsonTest::fromTokenDataNoTokens() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::String out;
    Error redirectError{&out};
    Utility::Json json{{}, {}, {}, {}};
    CORRADE_COMPARE(out, "Utility::Json: expected at least one token\n");
}

void JsonTest::fromTokenDataInvalidTokenArraySizes() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::String out;
    Error redirectError{&out};
    Json{{}, {InPlaceInit, {
        JsonTokenData{JsonToken::Type::Array, 4},
        JsonTokenData{nullptr},
        JsonTokenData{nullptr},
        JsonTokenData{nullptr},
        JsonTokenData{nullptr},
    }}, {InPlaceInit, {
        {}, {}, {}, {}
    }}, {}};
    CORRADE_COMPARE(out, "Utility::Json: expected token and offset/size arrays to have the same size but got 5 and 4\n");
}

void JsonTest::fromTokenDataTokenOffsetSizeOutOfBounds() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::Array<JsonTokenOffsetSize> offsetsSizes{InPlaceInit, {
        {2, 4}, /* this is fine */
        {},
        {3, 4}  /* this not */
    }};
    Containers::Array<JsonTokenData> tokens{InPlaceInit, {
        JsonTokenData{JsonToken::Type::Array, 2},
        JsonTokenData{nullptr},
        JsonTokenData{35ull, offsetsSizes.back()},
    }};

    Containers::String out;
    Error redirectError{&out};
    /* Small types */
    Json{"0123456", {InPlaceInit, {
        JsonTokenData{JsonToken::Type::Array, 2},
        JsonTokenData{nullptr},
        JsonTokenData{nullptr},
    }}, {InPlaceInit, {
        {2, 5}, /* this is fine */
        {},
        {3, 5}  /* this not */
    }}, {}};
    /* Large types that contain type encoded in the offset/size */
    Json{"012345", Utility::move(tokens), Utility::move(offsetsSizes), {}};
    CORRADE_COMPARE_AS(out,
        "Utility::Json: token 2 offset 3 and size 5 out of range for input of size 7\n"
        "Utility::Json: token 2 offset 3 and size 4 out of range for input of size 6\n",
        TestSuite::Compare::String);
}

void JsonTest::fromTokenDataStringTokenDataSizeTooSmall() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::String out;
    Error redirectError{&out};
    Json{"abcdef", {InPlaceInit, {
        JsonTokenData{JsonToken::Type::Array, 3},
        JsonTokenData{nullptr},
        JsonTokenData{JsonToken::Type::String, ~std::uint64_t{}},
        JsonTokenData{nullptr},
    }}, {InPlaceInit, {
        {}, {}, {3, 1}, {}
    }}, {}};
    CORRADE_COMPARE(out, "Utility::Json: string token 2 should be at least two bytes but got 1\n");
}

void JsonTest::fromTokenDataEscapedStringOutOfBounds() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::String out;
    Error redirectError{&out};
    /* Escaped value */
    Json{{}, {InPlaceInit, {
        JsonTokenData{JsonToken::Type::Array, 2},
        JsonTokenData{JsonToken::Type::String, 4},
        JsonTokenData{nullptr},
    }}, {InPlaceInit, {
        {}, {}, {}
    }}, {InPlaceInit, {
        "yes",
        "hello?",
        "it's",
        "me"
    }}};
    /* Escaped key */
    Json{{}, {InPlaceInit, {
        JsonTokenData{JsonToken::Type::Object, 2},
        JsonTokenData{JsonToken::Type::String, 2, true},
        JsonTokenData{nullptr},
    }}, {InPlaceInit, {
        {}, {}, {}
    }}, {InPlaceInit, {
        "",
        "NEIN"
    }}};
    CORRADE_COMPARE_AS(out,
        "Utility::Json: escaped string token 1 index 4 out of range for 4 escaped strings\n"
        "Utility::Json: escaped string token 1 index 2 out of range for 2 escaped strings\n",
        TestSuite::Compare::String);
}

void JsonTest::fromTokenDataObjectKeyWithNoValue() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::String out;
    Error redirectError{&out};
    Json{{}, {InPlaceInit, {
        JsonTokenData{JsonToken::Type::Array, 7},
        /* Child count accidentally 5 instead of 6, causing the last key to not
           have any value, which is instead treated as an array item */
        JsonTokenData{JsonToken::Type::Object, 5},
        JsonTokenData{JsonToken::Type::String, 0, true},
        /* Verifying that the check correctly handles key child count > 1 */
        JsonTokenData{JsonToken::Type::Array, 2},
        JsonTokenData{35},
        JsonTokenData{false},
        JsonTokenData{JsonToken::Type::String, 1, true}, /* Has no value */
        JsonTokenData{17},
    }}, {InPlaceInit, {
        {}, {}, {}, {},
        {}, {}, {}, {},
    }}, {InPlaceInit, {
        "a",
        "b"
    }}};
    CORRADE_COMPARE(out, "Utility::Json: string key token 6 has no value\n");
}

void JsonTest::fromTokenDataObjectKeyNotString() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::String out;
    Error redirectError{&out};
    Json{{}, {InPlaceInit, {
        JsonTokenData{JsonToken::Type::Array, 8},
        /* Child count accidentally 7 instead of 6, causing the 3.5f token to
           be interpreted as a key instead of an array item */
        JsonTokenData{JsonToken::Type::Object, 7},
        JsonTokenData{JsonToken::Type::String, 0, true},
        /* Verifying that the check correctly handles key child count > 1 */
        JsonTokenData{JsonToken::Type::Array, 2},
        JsonTokenData{35},
        JsonTokenData{false},
        JsonTokenData{JsonToken::Type::String, 1, true},
        JsonTokenData{17},
        JsonTokenData{3.5f}, /* This one is expected to be a key */
    }}, {InPlaceInit, {
        {}, {}, {}, {},
        {}, {}, {}, {},
        {},
    }}, {InPlaceInit, {
        "a",
        "b"
    }}};
    CORRADE_COMPARE(out, "Utility::Json: token 8 expected to be a string key but is Utility::JsonToken::Type::Number\n");
}

void JsonTest::fromTokenDataObjectStringNotKey() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::String out;
    Error redirectError{&out};
    Json{{}, {InPlaceInit, {
        JsonTokenData{JsonToken::Type::Array, 7},
        JsonTokenData{JsonToken::Type::Object, 6},
        JsonTokenData{JsonToken::Type::String, 0, true},
        /* Verifying that the check correctly handles key child count > 1 */
        JsonTokenData{JsonToken::Type::Array, 2},
        JsonTokenData{35},
        JsonTokenData{false},
        JsonTokenData{JsonToken::Type::String, 1}, /* Not marked as a key */
        JsonTokenData{17},
    }}, {InPlaceInit, {
        {}, {}, {}, {},
        {}, {}, {}, {},
    }}, {InPlaceInit, {
        "a",
        "b"
    }}};
    CORRADE_COMPARE(out, "Utility::Json: string token 6 is not marked as a key\n");
}

void JsonTest::fromTokenDataStringValueKey() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::String out;
    Error redirectError{&out};
    /* Escaped */
    Json{{}, {InPlaceInit, {
        JsonTokenData{JsonToken::Type::Array, 4},
        JsonTokenData{35},
        JsonTokenData{JsonToken::Type::String, 1},
        JsonTokenData{JsonToken::Type::String, 0, true}, /* Wrongly marked */
        JsonTokenData{false},
    }}, {InPlaceInit, {
        {}, {}, {}, {},
        {},
    }}, {InPlaceInit, {
        "a",
        "b"
    }}};
    /* Non-escaped */
    Json{"\"b\"", {InPlaceInit, {
        JsonTokenData{JsonToken::Type::Object, 2},
        JsonTokenData{JsonToken::Type::String, 0, true}, /* This is okay */
        JsonTokenData{JsonToken::Type::String, ~std::uint64_t{}, true}
    }}, {InPlaceInit, {
        {}, {}, {0, 3},
    }}, {InPlaceInit, {
        "a"
    }}};
    CORRADE_COMPARE_AS(out,
        "Utility::Json: string token 3 is not expected to be a key\n"
        "Utility::Json: string token 2 is not expected to be a key\n",
        TestSuite::Compare::String);
}

void JsonTest::fromTokenDataInvalidObjectChildCount() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::String out;
    Error redirectError{&out};
    Json{{}, {InPlaceInit, {
        /* Child count larger than the remaining token count (and thus also
           total count). Child count larger than the remaining count but not
           total can only happen for a nested object/array, and in that case
           it'd be also larger than the parent object/array child count. That
           is tested in fromTokenDataInvalidNestedObjectChildCount() below. */
        JsonTokenData{JsonToken::Type::Object, 5},
        JsonTokenData{JsonToken::Type::String, 0, true},
        JsonTokenData{false},
        JsonTokenData{JsonToken::Type::String, 1, true},
        JsonTokenData{35},
    }}, {InPlaceInit, {
        {}, {}, {}, {},
        {},
    }}, {InPlaceInit, {
        "a",
        "b"
    }}};
    CORRADE_COMPARE(out, "Utility::Json: token 0 has 5 children but expected at most 4\n");
}

void JsonTest::fromTokenDataInvalidArrayChildCount() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::String out;
    Error redirectError{&out};
    Json{{}, {InPlaceInit, {
        /* Child count larger than the remaining token count (and thus also
           total count). Child count larger than the remaining count but not
           total can only happen for a nested object/array, and in that case
           it'd be also larger than the parent object/array child count. That
           is tested in fromTokenDataInvalidNestedArrayChildCount() below. */
        JsonTokenData{JsonToken::Type::Array, 4},
        JsonTokenData{false},
        JsonTokenData{JsonToken::Type::String, 0},
        JsonTokenData{-33},
    }}, {InPlaceInit, {
        {}, {}, {}, {},
    }}, {InPlaceInit, {
        "a"
    }}};
    CORRADE_COMPARE(out, "Utility::Json: token 0 has 4 children but expected at most 3\n");
}

void JsonTest::fromTokenDataInvalidNestedObjectChildCount() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::String out;
    Error redirectError{&out};
    Json{{}, {InPlaceInit, {
        JsonTokenData{JsonToken::Type::Array, 10},
        JsonTokenData{true},
        JsonTokenData{false},
        /* If this would be 6 instead of 5, it would be okay */
        JsonTokenData{JsonToken::Type::Array, 5},
        JsonTokenData{35},
        JsonTokenData{JsonToken::Type::Object, 4},
        JsonTokenData{JsonToken::Type::String, 0, true},
        JsonTokenData{-3.5f},
        JsonTokenData{JsonToken::Type::String, 1, true},
        JsonTokenData{nullptr},
        JsonTokenData{17},
    }}, {InPlaceInit, {
        {}, {}, {}, {},
        {}, {}, {}, {},
        {}, {}, {},
    }}, {InPlaceInit, {
        "a",
        "b"
    }}};
    CORRADE_COMPARE(out, "Utility::Json: token 5 has 4 children but expected at most 3\n");
}

void JsonTest::fromTokenDataInvalidNestedArrayChildCount() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::String out;
    Error redirectError{&out};
    Json{{}, {InPlaceInit, {
        JsonTokenData{JsonToken::Type::Array, 9},
        JsonTokenData{true},
        JsonTokenData{JsonToken::Type::Object, 6},
        JsonTokenData{JsonToken::Type::String, 0, true},
        JsonTokenData{35},
        JsonTokenData{JsonToken::Type::String, 1, true},
        /* If this would be 2 instead of 3, it would be okay. Making the
           enclosing object 7-item wouldn't because then there would be no
           key for the extra item. */
        JsonTokenData{JsonToken::Type::Array, 3},
        JsonTokenData{-3.5f},
        JsonTokenData{nullptr},
        JsonTokenData{17},
    }}, {InPlaceInit, {
        {}, {}, {}, {},
        {}, {}, {}, {},
        {}, {},
    }}, {InPlaceInit, {
        "a",
        "b"
    }}};
    CORRADE_COMPARE(out, "Utility::Json: token 6 has 3 children but expected at most 2\n");
}

void JsonTest::fromTokenDataExtraRootTokens() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Containers::String out;
    Error redirectError{&out};
    /* Two values one after each other */
    Json{{}, {InPlaceInit, {
        JsonTokenData{35.6f},
        JsonTokenData{true},
    }}, {InPlaceInit, {
        {}, {}
    }}, {}};
    /* Value after a non-empty array */
    Json{{}, {InPlaceInit, {
        JsonTokenData{JsonToken::Type::Array, 2},
        JsonTokenData{true},
        JsonTokenData{false},
        JsonTokenData{nullptr},
    }}, {InPlaceInit, {
        {}, {}, {}, {}
    }}, {}};
    /* Non-empty array after a value. The array makes the stack non-empty so
       the check has to be done before the stack is added to. */
    Json{{}, {InPlaceInit, {
        JsonTokenData{nullptr},
        JsonTokenData{JsonToken::Type::Array, 2},
        JsonTokenData{true},
        JsonTokenData{false},
    }}, {InPlaceInit, {
        {}, {}, {}, {}
    }}, {}};
    /* Empty array / object one after each other */
    Json{{}, {InPlaceInit, {
        JsonTokenData{JsonToken::Type::Array, 0},
        JsonTokenData{JsonToken::Type::Object, 0},
    }}, {InPlaceInit, {
        {}, {}
    }}, {}};
    CORRADE_COMPARE_AS(out,
        "Utility::Json: extraneous root token 1\n"
        "Utility::Json: extraneous root token 3\n"
        "Utility::Json: extraneous root token 1\n"
        "Utility::Json: extraneous root token 1\n",
        TestSuite::Compare::String);
}

void JsonTest::fromTokenDataParseEmpty() {
    /* All tokens are empty but numeric ones are pointing to a random place in
       the input to test the location appears correctly in the error */
    Containers::Array<JsonTokenOffsetSize> offsetsSizes{InPlaceInit, {
        {}, {}, {}, {},
        {7, 0}, {5, 0}, {8, 0}, {3, 0},
        {}, {}, {},
    }};
    Containers::Array<JsonTokenData> tokens{InPlaceInit, {
        JsonTokenData{JsonToken::Type::Array, 10},  /* 0 */
        JsonTokenData{JsonToken::Type::Object, 0},  /* 1 */
        JsonTokenData{nullptr},                     /* 2 */
        JsonTokenData{true},                        /* 3 */
        JsonTokenData{35.0, offsetsSizes[4]},       /* 4 */
        JsonTokenData{35.0f},                       /* 5 */
        JsonTokenData{35u},                         /* 6 */
        JsonTokenData{-35},                         /* 7 */
        JsonTokenData{35ull, offsetsSizes[8]},      /* 8 */
        JsonTokenData{-35ll, offsetsSizes[9]},      /* 9 */
        JsonTokenData{JsonToken::Type::String, 0},  /* 10 */
    }};

    Json json{"0\n\n\n4567\n8",
        Utility::move(tokens),
        Utility::move(offsetsSizes),
        {InPlaceInit, {"hello"}}};

    /* These are all parsed and cannot be reparsed as a different type, so
       they are no-op and won't fail even though empty */
    CORRADE_VERIFY(json.parseArray(json.tokens()[0]));
    CORRADE_VERIFY(json.parseObject(json.tokens()[1]));
    CORRADE_VERIFY(json.parseNull(json.tokens()[2]));
    CORRADE_VERIFY(json.parseBool(json.tokens()[3]));
    CORRADE_VERIFY(json.parseString(json.tokens()[10]));

    /* Parsing as the same type should be no-op as well */
    CORRADE_VERIFY(json.parseDouble(json.tokens()[4]));
    CORRADE_VERIFY(json.parseFloat(json.tokens()[5]));
    CORRADE_VERIFY(json.parseUnsignedInt(json.tokens()[6]));
    CORRADE_VERIFY(json.parseInt(json.tokens()[7]));
    CORRADE_VERIFY(json.parseUnsignedLong(json.tokens()[8]));
    CORRADE_VERIFY(json.parseLong(json.tokens()[9]));

    /* These should fail predictably. Definitely not succeed, as that would
       mean the original data get lost because there's nothing to parse the
       data from now. */
    {
        Containers::String out;
        Error redirectError{&out};
        CORRADE_VERIFY(!json.parseFloat(json.tokens()[4])); /* was Double */
        CORRADE_VERIFY(!json.parseDouble(json.tokens()[5])); /* was Float */
        CORRADE_VERIFY(!json.parseInt(json.tokens()[6])); /* was UnsignedInt */
        CORRADE_VERIFY(!json.parseUnsignedInt(json.tokens()[7])); /* was Int */
        CORRADE_VERIFY(!json.parseLong(json.tokens()[8])); /* was UnsignedLong */
        CORRADE_VERIFY(!json.parseUnsignedLong(json.tokens()[9])); /* was Long */
        CORRADE_COMPARE_AS(out,
            "Utility::Json::parseFloat(): invalid floating-point literal  at <in>:4:4\n"
            "Utility::Json::parseDouble(): invalid floating-point literal  at <in>:4:2\n"
            "Utility::Json::parseInt(): invalid integer literal  at <in>:4:5\n"
            "Utility::Json::parseUnsignedInt(): invalid unsigned integer literal  at <in>:3:1\n"
            "Utility::Json::parseLong(): invalid integer literal  at <in>:1:1\n"
            "Utility::Json::parseUnsignedLong(): invalid unsigned integer literal  at <in>:1:1\n",
            TestSuite::Compare::String);
    }

    /* They also shouldn't get broken by that, i.e. should stay in the original
       type and with the value unchanged */
    CORRADE_COMPARE(json.tokens()[4].parsedType(), JsonToken::ParsedType::Double);
    CORRADE_COMPARE(json.tokens()[5].parsedType(), JsonToken::ParsedType::Float);
    CORRADE_COMPARE(json.tokens()[6].parsedType(), JsonToken::ParsedType::UnsignedInt);
    CORRADE_COMPARE(json.tokens()[7].parsedType(), JsonToken::ParsedType::Int);
    CORRADE_COMPARE(json.tokens()[8].parsedType(), JsonToken::ParsedType::UnsignedLong);
    CORRADE_COMPARE(json.tokens()[9].parsedType(), JsonToken::ParsedType::Long);

    CORRADE_COMPARE(json.tokens()[4].asDouble(), 35.0);
    CORRADE_COMPARE(json.tokens()[5].asFloat(), 35.0f);
    CORRADE_COMPARE(json.tokens()[6].asUnsignedInt(), 35);
    CORRADE_COMPARE(json.tokens()[7].asInt(), -35);
    CORRADE_COMPARE(json.tokens()[8].asUnsignedLong(), 35);
    CORRADE_COMPARE(json.tokens()[9].asLong(), -35);
}

}}}}

CORRADE_TEST_MAIN(Corrade::Utility::Test::JsonTest)
