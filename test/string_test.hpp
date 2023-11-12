#ifndef STRING_TEST_HPP_
#define STRING_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "efp.hpp"
#include "test_common.hpp"

using namespace efp;

TEST_CASE("Strings are joined with a delimiter", "[string]")
{
    SECTION("Joining empty list returns an empty string")
    {
        Vector<String> empty_strings;
        String delimiter(",");
        String result = join(delimiter, empty_strings);
        CHECK(result.size() == 0);
    }

    SECTION("Joining with an empty delimiter")
    {
        Vector<String> strings = {String("Hello"), String("World")};
        String delimiter("");
        String result = join(delimiter, strings);
        CHECK(result == String("HelloWorld"));
    }

    SECTION("Joining with a space delimiter")
    {
        Vector<String> strings = {String("Hello"), String("World")};
        String delimiter(" ");
        String result = join(delimiter, strings);
        CHECK(result == String("Hello World"));
    }

    SECTION("Joining with a comma delimiter")
    {
        Vector<String> strings = {String("apple"), String("banana"), String("cherry")};
        String delimiter(", ");
        String result = join(delimiter, strings);
        CHECK(result == String("apple, banana, cherry"));
    }

    SECTION("Joining single element should not append delimiter")
    {
        Vector<String> strings = {String("Hello")};
        String delimiter(", ");
        String result = join(delimiter, strings);
        CHECK(result == String("Hello"));
    }
}

#endif