#ifndef STRING_TEST_HPP_
#define STRING_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "efp.hpp"
#include "test_common.hpp"

using namespace efp;

TEST_CASE("String Construction")
{
    String str = "hello";
    CHECK(str.size() == 5);
}

TEST_CASE("String Copy Construction")
{
    String original("hello");
    String copy = original;
    CHECK(copy == original);
}

TEST_CASE("String Assignment")
{
    String str1("hello");
    String str2;
    str2 = str1;
    CHECK(str1 == str2);
}

TEST_CASE("String Concatenation")
{
    String str1 = "hello";
    String str2 = " world";
    String str3 = "!";

    CHECK(append(str1, str2, str3) == String("hello world!"));
}

TEST_CASE("String c_str()")
{
    String str = "hello";
    CHECK(strcmp(str.c_str(), "hello") == 0);
}

// Include or define the SFINAE tools here...
// struct Nil {...};
// template <typename H, typename T> struct TypeSequence {...};
// ... other definitions ...

TEST_CASE("Type sequences are correctly manipulated", "[format-sfinae]")
{

    SECTION("MakeTypeSeq creates correct type sequences")
    {
        using TestSeq = MakeTypeSeq<int, char, double>;
        CHECK((std::is_same<TestSeq, TypeSeq<int, TypeSeq<char, TypeSeq<double, Nil>>>>::value));
    }

    SECTION("AppendTypeSeq appends types correctly")
    {
        using InitialSeq = MakeTypeSeq<int, char>;
        using AppendedSeq = AppendTypeSeq<InitialSeq, double>;
        CHECK((std::is_same<AppendedSeq, TypeSeq<int, TypeSeq<char, TypeSeq<double, Nil>>>>::value));
    }

    SECTION("TypeSeqHead extracts the correct head type")
    {
        using Seq = MakeTypeSeq<int, char, double>;
        CHECK((std::is_same<TypeSeqHead<Seq>::Type, int>::value));
    }

    SECTION("TypeSeqTail extracts the correct tail type")
    {
        using Seq = MakeTypeSeq<int, char, double>;
        using Tail = TypeSeqTail<Seq>::Type;
        CHECK((std::is_same<Tail, TypeSeq<char, TypeSeq<double, Nil>>>::value));
    }

    SECTION("ContainsTypeSeq checks for type containment")
    {
        using Seq = MakeTypeSeq<int, char, double>;
        CHECK(ContainsTypeSeq<Seq, char>::value);
        CHECK_FALSE(ContainsTypeSeq<Seq, float>::value);
    }

    SECTION("RemoveTypeSeq removes types correctly")
    {
        using Seq = MakeTypeSeq<int, char, double>;
        using RemovedSeq = RemoveTypeSeq<Seq, char>;
        CHECK((std::is_same<RemovedSeq, TypeSeq<int, TypeSeq<double, Nil>>>::value));
    }

    SECTION("IsIntegralType identifies integral types")
    {
        CHECK(IsIntegeralType<int>::value);
        CHECK_FALSE(IsIntegeralType<float>::value);
    }

    SECTION("CharsToString converts type sequence of Char types back to a runtime string")
    {
        using CharSeq = Chars<'H', 'e', 'l', 'l', 'o'>;
        CHECK(std::strcmp(CharsToString<CharSeq>::string(), "Hello") == 0);
    }

    SECTION("TypeToFormat maps types to format specifiers")
    {
        CHECK((std::is_same<TypeToFormat<int>::Type, Chars<'d'>>::value));
        CHECK((std::is_same<TypeToFormat<float>::Type, Chars<'f'>>::value));
    }

    // Additional tests should be written for other constructs...
}
#endif