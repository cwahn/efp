#ifndef FORMAT_TEST_HPP_
#define FORMAT_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "format.hpp"
#include "test_common.hpp"

// using namespace efp;

// TEST_CASE("String Construction")
// {
//     String str = "hello";
//     CHECK(str.size() == 5);
// }

// TEST_CASE("String Copy Construction")
// {
//     String original("hello");
//     String copy = original;
//     CHECK(copy == original);
// }

// TEST_CASE("String Assignment")
// {
//     String str1("hello");
//     String str2;
//     str2 = str1;
//     CHECK(str1 == str2);
// }

// TEST_CASE("String Concatenation")
// {
//     String str1 = "hello";
//     String str2 = " world";
//     String str3 = "!";

//     CHECK(append(str1, str2, str3) == String("hello world!"));
// }

// TEST_CASE("String c_str()")
// {
//     String str = "hello";
//     CHECK(strcmp(str.c_str(), "hello") == 0);
// }

// // Include or define the SFINAE tools here...
// // struct Nil {...};
// // template <typename H, typename T> struct TypeSequence {...};
// // ... other definitions ...

// TEST_CASE("Type sequences are correctly manipulated", "[format-sfinae]")
// {

//     SECTION("MakeTypeSeq creates correct type sequences")
//     {
//         using TestSeq = MakeTypeSeq<int, char, double>;
//         CHECK((IsSame<TestSeq, TypeSeq<int, TypeSeq<char, TypeSeq<double, Nil>>>>::value));
//     }

//     SECTION("AppendTypeSeq appends types correctly")
//     {
//         using InitialSeq = MakeTypeSeq<int, char>;
//         using AppendedSeq = AppendTypeSeq<InitialSeq, double>;
//         CHECK((IsSame<AppendedSeq, TypeSeq<int, TypeSeq<char, TypeSeq<double, Nil>>>>::value));
//     }

//     SECTION("TypeSeqHead extracts the correct head type")
//     {
//         using Seq = MakeTypeSeq<int, char, double>;
//         CHECK((IsSame<TypeSeqHead<Seq>::Type, int>::value));
//     }

//     SECTION("TypeSeqTail extracts the correct tail type")
//     {
//         using Seq = MakeTypeSeq<int, char, double>;
//         using Tail = TypeSeqTail<Seq>::Type;
//         CHECK((IsSame<Tail, TypeSeq<char, TypeSeq<double, Nil>>>::value));
//     }

//     SECTION("ContainsTypeSeq checks for type containment")
//     {
//         using Seq = MakeTypeSeq<int, char, double>;
//         CHECK(ContainsTypeSeq<Seq, char>::value);
//         CHECK_FALSE(ContainsTypeSeq<Seq, float>::value);
//     }

//     SECTION("RemoveTypeSeq removes types correctly")
//     {
//         using Seq = MakeTypeSeq<int, char, double>;
//         using RemovedSeq = RemoveTypeSeq<Seq, char>;
//         CHECK((IsSame<RemovedSeq, TypeSeq<int, TypeSeq<double, Nil>>>::value));
//     }

//     SECTION("IsIntegralType identifies size_tegral types")
//     {
//         CHECK(IsIntegeralType<int>::value);
//         CHECK_FALSE(IsIntegeralType<float>::value);
//     }

//     SECTION("CharsToString converts type sequence of Char types back to a runtime string")
//     {
//         using CharSeq = Chars<'H', 'e', 'l', 'l', 'o'>;
//         CHECK(std::strcmp(CharsToString<CharSeq>::string(), "Hello") == 0);
//     }

//     SECTION("TypeToFormat maps types to format specifiers")
//     {
//         CHECK((IsSame<TypeToFormat<int>::Type, Chars<'d'>>::value));
//         CHECK((IsSame<TypeToFormat<float>::Type, Chars<'f'>>::value));
//     }
// }

// TEST_CASE("auto_format works as expected", "[format-sfinae]")
// {

//     CHECK(strcmp(auto_format(""), "") == 0);

//     CHECK(strcmp(auto_format("%%"), "%%") == 0);
//     CHECK(strcmp(auto_format("{} %f", 123, 1.23f), "%d %f") == 0);
//     CHECK(strcmp(auto_format("%f {}", 1.23f, 123), "%f %d") == 0);

//     CHECK(strcmp(auto_format(" \\{ "), " { ") == 0);
//     CHECK(strcmp(auto_format("\\{}"), "{}") == 0);
//     CHECK(strcmp(auto_format(" \\{ {} } ", 123), " { %d } ") == 0);

//     CHECK(strcmp(auto_format("{}", nullptr), "%p") == 0);
//     // CHECK(strcmp(auto_format("{}", reinterpret_cast<void *>(0)), "%p") == 0);

//     CHECK(strcmp(auto_format("{}", "str"), "%p") == 0);
//     CHECK(strcmp(auto_format("{s}", "str"), "%s") == 0);

//     CHECK(strcmp(auto_format("{}", static_cast<char>(123)), "%c") == 0);

//     CHECK(strcmp(auto_format("{}", static_cast<short>(123)), "%d") == 0);
//     CHECK(strcmp(auto_format("{}", 123), "%d") == 0);
//     CHECK(strcmp(auto_format("{}", 123l), "%ld") == 0);
//     CHECK(strcmp(auto_format("{}", 123ll), "%lld") == 0);

//     CHECK(strcmp(auto_format("{}", 123u), "%u") == 0);
//     CHECK(strcmp(auto_format("{}", 123ul), "%lu") == 0);
//     CHECK(strcmp(auto_format("{}", 123ull), "%llu") == 0);

//     CHECK(strcmp(auto_format("{x}", 123u), "%x") == 0);
//     CHECK(strcmp(auto_format("{x}", 123ul), "%lx") == 0);
//     CHECK(strcmp(auto_format("{x}", 123ull), "%llx") == 0);

//     CHECK(strcmp(auto_format("{}", true), "%d") == 0);

//     CHECK(strcmp(auto_format("{}", 1.0f), "%f") == 0);
//     CHECK(strcmp(auto_format("{}", 1.0), "%lf") == 0);

//     CHECK(strcmp(auto_format("{10}", 123), "%10d") == 0);
//     CHECK(strcmp(auto_format("{10x}", 123u), "%10x") == 0);
//     CHECK(strcmp(auto_format("{#10x}", 123u), "%#10x") == 0);

//     CHECK(strcmp(auto_format("{x}", 123), "%x") == 0);
//     CHECK(strcmp(auto_format("{x}", 123l), "%lx") == 0);
//     CHECK(strcmp(auto_format("{x}", 123ll), "%llx") == 0);
// }

TEST_CASE("format")
{
    CHECK(efp::format("This is {}", 42) == "This is 42");
}

#endif