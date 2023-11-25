#ifndef PARSER_TEST_HPP_
#define PARSER_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "efp.hpp"
#include "test_common.hpp"

using namespace efp;
using namespace efp::parser;

TEST_CASE("start_with function works correctly", "[start_with]")
{
    SECTION("Sequence starts with the given prefix")
    {
        CHECK(start_with(StringView{"hello", 5}, StringView{"he", 2}));
    }

    SECTION("Sequence does not start with the given prefix")
    {
        CHECK_FALSE(start_with(StringView{"hello", 5}, StringView{"lo", 2}));
    }
}

TEST_CASE("Ch parser works correctly", "[Ch]")
{
    // Ch c{'a'};

    SECTION("Sequence starts with the character")
    {
        auto result = ch('a')(StringView{"apple", 5});

        CHECK(result);
        if (result)
        {
            CHECK(fst(result.value()).size() == 4); // "pple"
            CHECK(snd(result.value()).size() == 1); // "a"
        }
    }

    SECTION("Sequence does not start with the character")
    {
        auto result = ch('a')(StringView{"banana", 6});
        CHECK_FALSE(result);
    }
}

TEST_CASE("alpha0 parser works correctly", "[alpha0]")
{
    SECTION("Sequence with only alphabetic characters")
    {
        auto result = alpha0(StringView{"abcde", 5});
        CHECK(result);
        CHECK(fst(result.value()).size() == 0); // Remaining input is empty
        CHECK(snd(result.value()).size() == 5); // Parsed "abcde"
    }

    SECTION("Empty sequence")
    {
        auto result = alpha0(StringView{"", 0});
        CHECK(result);
        CHECK(fst(result.value()).size() == 0); // Remaining input is empty
        CHECK(snd(result.value()).size() == 0); // Parsed nothing
    }

    SECTION("Sequence starting with non-alphabetic characters")
    {
        auto result = alpha0(StringView{"123abc", 6});
        CHECK(result);
        CHECK(fst(result.value()).size() == 6); // Remaining input is "123abc"
        CHECK(snd(result.value()).size() == 0); // Parsed nothing
    }
}

TEST_CASE("alpha1 parser works correctly", "[alpha1]")
{
    SECTION("Sequence with only alphabetic characters")
    {
        auto result = alpha1(StringView{"abcde", 5});
        CHECK(result);
        CHECK(fst(result.value()).size() == 0); // Remaining input is empty
        CHECK(snd(result.value()).size() == 5); // Parsed "abcde"
    }

    SECTION("Empty sequence")
    {
        auto result = alpha1(StringView{"", 0});
        CHECK_FALSE(result); // Should fail as there are no characters
    }

    SECTION("Sequence starting with non-alphabetic characters")
    {
        auto result = alpha1(StringView{"123abc", 6});
        CHECK_FALSE(result); // Should fail as the first character is not alphabetic
    }
}

TEST_CASE("numeric0 parser works correctly", "[numeric0]")
{
    SECTION("Sequence with only numeric characters")
    {
        auto result = numeric0(StringView{"12345", 5});
        CHECK(result);
        CHECK(fst(result.value()).size() == 0); // Remaining input is empty
        CHECK(snd(result.value()).size() == 5); // Parsed "12345"
    }

    SECTION("Empty sequence")
    {
        auto result = numeric0(StringView{"", 0});
        CHECK(result);
        CHECK(fst(result.value()).size() == 0); // Remaining input is empty
        CHECK(snd(result.value()).size() == 0); // Parsed nothing
    }

    SECTION("Sequence starting with non-numeric characters")
    {
        auto result = numeric0(StringView{"abc123", 6});
        CHECK(result);
        CHECK(fst(result.value()).size() == 6); // Remaining input is "abc123"
        CHECK(snd(result.value()).size() == 0); // Parsed nothing
    }
}

TEST_CASE("numeric1 parser works correctly", "[numeric1]")
{
    SECTION("Sequence with only numeric characters")
    {
        auto result = numeric1(StringView{"12345", 5});
        CHECK(result);
        CHECK(fst(result.value()).size() == 0); // Remaining input is empty
        CHECK(snd(result.value()).size() == 5); // Parsed "12345"
    }

    SECTION("Empty sequence")
    {
        auto result = numeric1(StringView{"", 0});
        CHECK_FALSE(result); // Should fail as there are no characters
    }

    SECTION("Sequence starting with non-numeric characters")
    {
        auto result = numeric1(StringView{"abc123", 6});
        CHECK_FALSE(result); // Should fail as the first character is not numeric
    }
}

TEST_CASE("alphanum0 parser works correctly", "[alphanum0]")
{
    SECTION("Sequence with only alphanumeric characters")
    {
        auto result = alphanum0(StringView{"abc123", 6});
        CHECK(result);
        CHECK(fst(result.value()).size() == 0); // Remaining input is empty
        CHECK(snd(result.value()).size() == 6); // Parsed "abc123"
    }

    SECTION("Empty sequence")
    {
        auto result = alphanum0(StringView{"", 0});
        CHECK(result);
        CHECK(fst(result.value()).size() == 0); // Remaining input is empty
        CHECK(snd(result.value()).size() == 0); // Parsed nothing
    }

    SECTION("Sequence with non-alphanumeric characters")
    {
        auto result = alphanum0(StringView{"!@#abc123", 9});
        CHECK(result);
        CHECK(fst(result.value()).size() == 9); // Remaining input is "!@#abc123"
        CHECK(snd(result.value()).size() == 0); // Parsed nothing
    }
}

TEST_CASE("alphanum1 parser works correctly", "[alphanum1]")
{
    SECTION("Sequence with only alphanumeric characters")
    {
        auto result = alphanum1(StringView{"abc123", 6});
        CHECK(result);
        CHECK(fst(result.value()).size() == 0); // Remaining input is empty
        CHECK(snd(result.value()).size() == 6); // Parsed "abc123"
    }

    SECTION("Empty sequence")
    {
        auto result = alphanum1(StringView{"", 0});
        CHECK_FALSE(result); // Should fail as there are no characters
    }

    SECTION("Sequence with non-alphanumeric characters")
    {
        auto result = alphanum1(StringView{"!@#abc123", 9});
        CHECK_FALSE(result); // Should fail as the first character is not alphanumeric
    }
}

TEST_CASE("Tag parser works correctly", "[Tag]")
{
    // Tag tag{StringView{"<tag>", 5}};

    SECTION("Sequence starts with the tag")
    {
        auto result = tag("<tag>")(StringView{"<tag>value", 10});
        CHECK(result);
        if (result)
        {
            CHECK(fst(result.value()).size() == 5); // "value"
            CHECK(snd(result.value()).size() == 5); // "<tag>"
        }
    }

    SECTION("Sequence does not start with the tag")
    {
        auto result = tag("<tag>")(StringView{"value<tag>", 10});
        CHECK_FALSE(result);
    }
}

TEST_CASE("Alt parser works correctly", "[Alt]")
{
    // Alt<Ch, Tag> alt{tuple(Ch{'!'}, Tag{StringView{"<tag>", 5}})};

    SECTION("Alternative with character matches")
    {
        auto result = alt(ch('!'), tag("<tag>"))(StringView{"!value", 6});
        CHECK(result);
        if (result)
        {
            CHECK(fst(result.value()).size() == 5); // "value"
            CHECK(snd(result.value()).size() == 1); // "!"
        }
    }

    SECTION("Alternative with tag matches")
    {
        auto result = alt(ch('!'), tag("<tag>"))(StringView{"<tag>value", 10});
        CHECK(result);
        if (result)
        {
            CHECK(fst(result.value()).size() == 5); // "value"
            CHECK(snd(result.value()).size() == 5); // "<tag>"
        }
    }

    SECTION("No alternatives match")
    {
        auto result = alt(ch('!'), tag("<tag>"))(StringView{"value", 5});
        CHECK_FALSE(result);
    }
}

#endif