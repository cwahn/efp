#ifndef STRING_TEST_HPP_
#define STRING_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "efp.hpp"
#include "test_common.hpp"

using namespace efp;

TEST_CASE("String class functionality", "[String]") {
    SECTION("Default Constructor") {
        String str;
        CHECK(str.empty());
        CHECK(str.size() == 0);
        CHECK(str.capacity() == 0);
    }

    SECTION("Constructor from C-Style String") {
        String str("Hello");
        CHECK(str.size() == 5);
        CHECK(std::strcmp(str.data(), "Hello") == 0);
    }

    SECTION("Copy Constructor") {
        String original("Hello");
        String copy = original;
        CHECK(copy == original);
        CHECK(copy.size() == original.size());
    }

    SECTION("Move Constructor") {
        String original("Hello");
        String moved(efp::move(original));
        CHECK(std::strcmp(moved.data(), "Hello") == 0);
        CHECK(original.empty());
    }

    SECTION("Copy Assignment Operator") {
        String str1("Hello");
        String str2;
        str2 = str1;
        CHECK(str2 == str1);
        CHECK(str2.size() == str1.size());
    }

    SECTION("Move Assignment Operator") {
        String str1("Hello");
        String str2;
        str2 = efp::move(str1);
        CHECK(std::strcmp(str2.data(), "Hello") == 0);
        CHECK(str1.empty());
    }

    SECTION("Destruction") {
        // This tests whether destruction happens without errors.
        { String str("Temporary String"); }  // str goes out of scope here and should be destroyed
        CHECK(true);  // If we reach this point, destruction didn't cause a crash
    }

    SECTION("String::operator[]") {
        String str("Hello");
        CHECK(str[0] == 'H');
        CHECK(str[4] == 'o');
    }

    SECTION("String::at") {
        String str("Hello");
        CHECK(str.at(0) == 'H');
        CHECK(str.at(4) == 'o');
    }

    SECTION("String::front") {
        String str("Hello");
        CHECK(str.front() == 'H');
    }

    SECTION("String::back") {
        String str("Hello");
        CHECK(str.back() == 'o');
    }

    SECTION("String::resize") {
        String str("Hello");
        str.resize(10);
        CHECK(str.size() == 10);
    }

    SECTION("String::reserve") {
        String str("Hello");
        str.reserve(10);
        CHECK(str.capacity() >= 10);
    }

    SECTION("String::push_back") {
        String str;
        str.push_back('H');
        str.push_back('i');
        CHECK(str.size() == 2);
        CHECK(std::strcmp(str.data(), "Hi") == 0);
    }

    SECTION("String::pop_back") {
        String str("Hello");
        str.erase(0);  // Remove 'H'
        CHECK(str.size() == 4);
        CHECK(str == "ello");
    }

    SECTION("String::c_str") {
        String str("Hello");
        CHECK(std::strcmp(str.c_str(), "Hello") == 0);
    }

    SECTION("String::operator+=") {
        String str("Hello");
        str += " World";
        CHECK(str == "Hello World");
    }

    SECTION("String::append(const Char*)") {
        String str("Hello");
        str.append(" World");
        CHECK(str == "Hello World");
    }

    SECTION("String::append(size_t, Char)") {
        String str("Hello");
        str.append(3, '!');
        CHECK(str == "Hello!!!");
    }

    SECTION("String::assign") {
        String str("Hello");
        str.assign("World");
        CHECK(str == "World");
    }

    SECTION("Equality with C-Style String") {
        String str("Hello");
        CHECK(str == "Hello");
        CHECK_FALSE(str == "Hi");
    }
}

TEST_CASE("StringView class functionality", "[StringView]") {
    SECTION("Construction from C-Style String Literal") {
        const char* str = "Hello, World!";
        StringView view(str);
        CHECK(view.size() == strlen(str));
        CHECK(std::strncmp(view.data(), str, view.size()) == 0);
    }

    SECTION("Default Construction") {
        StringView view;
        CHECK(view.empty());
        CHECK(view.size() == 0);
    }

    SECTION("Copy Construction") {
        StringView original("Hello");
        StringView copy = original;
        CHECK(copy == original);
    }

    SECTION("Element Access") {
        StringView view("Hello");
        CHECK(view[0] == 'H');
        CHECK(view[4] == 'o');
    }

    SECTION("Equality with Another StringView") {
        StringView view1("Hello");
        StringView view2("Hello");
        StringView view3("World");
        CHECK(view1 == view2);
        CHECK_FALSE(view1 == view3);
    }

    SECTION("Equality with C-Style String") {
        StringView view("Hello");
        CHECK(view == "Hello");
        CHECK_FALSE(view == "World");
    }

    // ! StringView does not have a c_str method
    // SECTION("Conversion to CString") {
    //     StringView view("Hello");
    //     CHECK(std::strcmp(view.c_str(), "Hello") == 0);
    // }

    SECTION("Begin and End Iterators") {
        StringView view("Hello");
        CHECK(*view.begin() == 'H');
        CHECK(*(view.end() - 1) == 'o');
    }
}

TEST_CASE("Strings are joined with a delimiter", "[string]") {
    SECTION("Joining empty list returns an empty string") {
        Vector<String> empty_strings;
        String delimiter(",");
        String result = intercalate(delimiter, empty_strings);
        CHECK(result.size() == 0);
    }

    SECTION("Joining with an empty delimiter") {
        Vector<String> strings = {String("Hello"), String("World")};
        String delimiter("");
        String result = intercalate(delimiter, strings);
        CHECK(result == String("HelloWorld"));
    }

    SECTION("Joining with a space delimiter") {
        Vector<String> strings = {String("Hello"), String("World")};
        String delimiter(" ");
        String result = intercalate(delimiter, strings);
        CHECK(result == String("Hello World"));
    }

    SECTION("Joining with a comma delimiter") {
        Vector<String> strings = {String("apple"), String("banana"), String("cherry")};
        String delimiter(", ");
        String result = intercalate(delimiter, strings);
        CHECK(result == String("apple, banana, cherry"));
    }

    SECTION("Joining single element should not append delimiter") {
        Vector<String> strings = {String("Hello")};
        String delimiter(", ");
        String result = intercalate(delimiter, strings);
        CHECK(result == String("Hello"));
    }
}

#endif