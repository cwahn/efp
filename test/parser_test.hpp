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