#ifndef STRING_TEST_HPP_
#define STRING_TEST_HPP_

#include "catch2/catch_test_macros.hpp"

#include "efp.hpp"
#include "test_common.hpp"

using namespace efp;

// TEST_CASE("CString class functionality", "[CString]")
// {
//     SECTION("Construction from dynamically allocated C-Style String")
//     {
//         char *dynamicStr = new char[strlen("Hello, World!") + 1];
//         strcpy(dynamicStr, "Hello, World!");
//         CString cstr(dynamicStr);
//         CHECK(std::strcmp(cstr, "Hello, World!") == 0);
//     }

//     SECTION("Move Constructor")
//     {
//         char *dynamicStr = new char[strlen("Hello, World!") + 1];
//         strcpy(dynamicStr, "Hello, World!");
//         CString original(dynamicStr);
//         CString moved(std::move(original));
//         CHECK(std::strcmp(moved, "Hello, World!") == 0);
//         CHECK(original == nullptr); // After move, original should be null
//     }

//     SECTION("Move Assignment Operator")
//     {
//         char *str1 = new char[strlen("Hello") + 1];
//         strcpy(str1, "Hello");
//         char *str2 = new char[strlen("World") + 1];
//         strcpy(str2, "World");

//         CString cstr1(str1);
//         CString cstr2(str2);

//         cstr2 = std::move(cstr1);

//         CHECK(std::strcmp(cstr2, "Hello") == 0);
//         CHECK(cstr1 == nullptr); // After move, cstr1 should be null
//     }

//     SECTION("Destruction")
//     {
//         char *dynamicStr = new char[strlen("Temporary String") + 1];
//         strcpy(dynamicStr, "Temporary String");
//         {
//             CString cstr(dynamicStr);
//         }            // cstr goes out of scope here and should be destroyed
//         CHECK(true); // If we reach this point, destruction didn't cause a crash
//     }

//     SECTION("Equality with Another CString")
//     {
//         char *str1 = new char[strlen("Test String") + 1];
//         strcpy(str1, "Test String");
//         char *str2 = new char[strlen("Test String") + 1];
//         strcpy(str2, "Test String");
//         char *str3 = new char[strlen("Different") + 1];
//         strcpy(str3, "Different");

//         CString cstr1(str1);
//         CString cstr2(str2);
//         CString cstr3(str3);

//         CHECK(cstr1 == cstr2);
//         CHECK_FALSE(cstr1 == cstr3);
//     }

//     SECTION("Equality with C-Style String")
//     {
//         char *dynamicStr = new char[strlen("Test String") + 1];
//         strcpy(dynamicStr, "Test String");
//         CString cstr(dynamicStr);

//         const char *expected = "Test String"; // Not dynamically allocated, only for comparison
//         const char *different = "Different";  // Not dynamically allocated, only for comparison

//         CHECK(cstr == expected);
//         CHECK_FALSE(cstr == different);
//     }

//     SECTION("Null CString Comparison")
//     {
//         CString cstr(nullptr);
//         CString null_cstr(nullptr);
//         CHECK(cstr == null_cstr);
//     }
// }

TEST_CASE("String class functionality", "[String]")
{
    SECTION("Default Constructor")
    {
        String str;
        CHECK(str.empty());
        CHECK(str.size() == 0);
        CHECK(str.capacity() == 0);
    }

    SECTION("Constructor from C-Style String")
    {
        String str("Hello");
        CHECK(str.size() == 5);
        CHECK(std::strcmp(str.data(), "Hello") == 0);
    }

    SECTION("Copy Constructor")
    {
        String original("Hello");
        String copy = original;
        CHECK(copy == original);
        CHECK(copy.size() == original.size());
    }

    SECTION("Move Constructor")
    {
        String original("Hello");
        String moved(std::move(original));
        CHECK(std::strcmp(moved.data(), "Hello") == 0);
        CHECK(original.empty());
    }

    SECTION("Copy Assignment Operator")
    {
        String str1("Hello");
        String str2;
        str2 = str1;
        CHECK(str2 == str1);
        CHECK(str2.size() == str1.size());
    }

    SECTION("Move Assignment Operator")
    {
        String str1("Hello");
        String str2;
        str2 = std::move(str1);
        CHECK(std::strcmp(str2.data(), "Hello") == 0);
        CHECK(str1.empty());
    }

    SECTION("Element Access")
    {
        String str("Hello");
        CHECK(str[0] == 'H');
        CHECK(str[4] == 'o');
    }

    SECTION("Resize")
    {
        String str("Hello");
        str.resize(10);
        CHECK(str.size() == 10);
    }

    SECTION("Reserve")
    {
        String str("Hello");
        str.reserve(10);
        CHECK(str.capacity() >= 10);
    }

    SECTION("Push Back")
    {
        String str;
        str.push_back('H');
        str.push_back('i');
        CHECK(str.size() == 2);
        CHECK(std::strcmp(str.data(), "Hi") == 0);
    }

    SECTION("Erase")
    {
        String str("Hello");
        str.erase(0); // Remove 'H'
        CHECK(str.size() == 4);
        CHECK(str == "ello");
    }

    SECTION("c_str")
    {
        String str("Hello");
        CHECK(std::strcmp(str.c_str().get(), "Hello") == 0);
    }

    SECTION("Equality with C-Style String")
    {
        String str("Hello");
        CHECK(str == "Hello");
        CHECK_FALSE(str == "Hi");
    }

    SECTION("Destruction")
    {
        // This tests whether destruction happens without errors.
        {
            String str("Temporary String");
        }            // str goes out of scope here and should be destroyed
        CHECK(true); // If we reach this point, destruction didn't cause a crash
    }
}

TEST_CASE("StringView class functionality", "[StringView]")
{
    SECTION("Construction from C-Style String Literal")
    {
        const char *str = "Hello, World!";
        StringView view(str);
        CHECK(view.size() == strlen(str));
        CHECK(std::strncmp(view.data(), str, view.size()) == 0);
    }

    SECTION("Default Construction")
    {
        StringView view;
        CHECK(view.empty());
        CHECK(view.size() == 0);
    }

    SECTION("Copy Construction")
    {
        StringView original("Hello");
        StringView copy = original;
        CHECK(copy == original);
    }

    SECTION("Element Access")
    {
        StringView view("Hello");
        CHECK(view[0] == 'H');
        CHECK(view[4] == 'o');
    }

    SECTION("Equality with Another StringView")
    {
        StringView view1("Hello");
        StringView view2("Hello");
        StringView view3("World");
        CHECK(view1 == view2);
        CHECK_FALSE(view1 == view3);
    }

    SECTION("Equality with C-Style String")
    {
        StringView view("Hello");
        CHECK(view == "Hello");
        CHECK_FALSE(view == "World");
    }

    SECTION("Conversion to CString")
    {
        StringView view("Hello");
        CHECK(std::strcmp(view.c_str().get(), "Hello") == 0);
    }

    SECTION("Begin and End Iterators")
    {
        StringView view("Hello");
        CHECK(*view.begin() == 'H');
        CHECK(*(view.end() - 1) == 'o');
    }
}

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