#ifndef NUMERTIC_LIMIT_HPP
#define NUMERTIC_LIMIT_HPP

#include <float.h>

namespace efp
{
    // NumericLimit
    // Per https://en.cppreference.com/w/cpp/types/numeric_limits

    template <typename A>
    struct NumericLimit
    {
        static constexpr A min() { return A(); };
        static constexpr A max() { return A(); };
    };

    template <>
    struct NumericLimit<bool>
    {
        static constexpr bool min() { return false; };
        static constexpr bool max() { return true; };
    };

    template <>
    struct NumericLimit<char>
    {
        static constexpr char min() { return CHAR_MIN; };
        static constexpr char max() { return CHAR_MAX; };
    };

    template <>
    struct NumericLimit<signed char>
    {
        static constexpr signed char min() { return SCHAR_MIN; };
        static constexpr signed char max() { return SCHAR_MAX; };
    };

    template <>
    struct NumericLimit<unsigned char>
    {
        static constexpr unsigned char min() { return 0; };
        static constexpr unsigned char max() { return UCHAR_MAX; };
    };

    template <>
    struct NumericLimit<wchar_t>
    {
        static constexpr wchar_t min() { return WCHAR_MIN; };
        static constexpr wchar_t max() { return WCHAR_MAX; };
    };

    // template <>
    // struct NumericLimit<char8_t>
    // {
    //     static constexpr char8_t min() { return 0; };
    //     static constexpr char8_t max() { return UCHAR_MAX; };
    // };

    template <>
    struct NumericLimit<char16_t>
    {
        static constexpr char16_t min() { return 0; };
        static constexpr char16_t max() { return UINT_LEAST16_MAX; };
    };

    template <>
    struct NumericLimit<char32_t>
    {
        static constexpr char32_t min() { return 0; };
        static constexpr char32_t max() { return UINT_LEAST32_MAX; };
    };

    template <>
    struct NumericLimit<short>
    {
        static constexpr short min() { return SHRT_MIN; };
        static constexpr short max() { return SHRT_MAX; };
    };

    template <>
    struct NumericLimit<unsigned short>
    {
        static constexpr unsigned short min() { return SHRT_MIN; };
        static constexpr unsigned short max() { return SHRT_MAX; };
    };

    template <>
    struct NumericLimit<int>
    {
        static constexpr int min() { return INT_MIN; };
        static constexpr int max() { return INT_MAX; };
    };

    // template <>
    // struct NumericLimit<signed int>
    // {
    //     static constexpr signed int min() { return INT_MIN; };
    //     static constexpr signed int max() { return INT_MAX; };
    // };

    template <>
    struct NumericLimit<unsigned int>
    {
        static constexpr unsigned int min() { return 0; };
        static constexpr unsigned int max() { return UINT_MAX; };
    };

    template <>
    struct NumericLimit<long>
    {
        static constexpr long min() { return LONG_MIN; };
        static constexpr long max() { return LONG_MAX; };
    };

    // template <>
    // struct NumericLimit<signed long>
    // {
    //     static constexpr signed long min() { return LDBL_MIN; };
    //     static constexpr signed long max() { return LONG_MAX; };
    // };

    template <>
    struct NumericLimit<unsigned long>
    {
        static constexpr unsigned long min() { return 0; };
        static constexpr unsigned long max() { return ULONG_MAX; };
    };

    template <>
    struct NumericLimit<long long>
    {
        static constexpr long min() { return LLONG_MIN; };
        static constexpr long max() { return LLONG_MAX; };
    };

    // template <>
    // struct NumericLimit<signed long long>
    // {
    //     static constexpr signed long long min() { return LLONG_MIN; };
    //     static constexpr signed long long max() { return LLONG_MAX; };
    // };

    template <>
    struct NumericLimit<unsigned long long>
    {
        static constexpr unsigned long long min() { return 0; };
        static constexpr unsigned long long max() { return ULLONG_MAX; };
    };

    template <>
    struct NumericLimit<float>
    {
        static constexpr float min() { return FLT_MIN; };
        static constexpr float max() { return FLT_MAX; };
    };

    template <>
    struct NumericLimit<double>
    {
        static constexpr double min() { return DBL_MIN; };
        static constexpr double max() { return DBL_MAX; };
    };
    template <>
    struct NumericLimit<long double>
    {
        static constexpr long double min() { return LDBL_MIN; };
        static constexpr long double max() { return LDBL_MAX; };
    };

}

#endif