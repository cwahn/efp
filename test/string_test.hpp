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
    String str1("hello");
    String str2(" world");
    String str3("!");
    // String result;

    // Use the assumed append function for Seq<char>
    const auto result = append(str1, str2, str3);

    CHECK(result == String("hello world!"));
}

TEST_CASE("String c_str()")
{
    String str("hello");
    const char *cstr = str.c_str();
    CHECK(strcmp(cstr, "hello") == 0);
}

#endif