#ifndef NUMERTIC_LIMIT_HPP
#define NUMERTIC_LIMIT_HPP

#include <float.h>
#include <climits>

namespace efp
{
    // NumericLimits
    // Per https://en.cppreference.com/w/cpp/types/numeric_limits

    template <typename A>
    struct NumericLimits
    {
        static constexpr A min() { return A(); };
        static constexpr A max() { return A(); };
    };

    template <>
    struct NumericLimits<bool>
    {
        static constexpr bool min() { return false; };
        static constexpr bool max() { return true; };
    };

    template <>
    struct NumericLimits<char>
    {
        static constexpr char min() { return CHAR_MIN; };
        static constexpr char max() { return CHAR_MAX; };
    };

    template <>
    struct NumericLimits<signed char>
    {
        static constexpr signed char min() { return SCHAR_MIN; };
        static constexpr signed char max() { return SCHAR_MAX; };
    };

    template <>
    struct NumericLimits<unsigned char>
    {
        static constexpr unsigned char min() { return 0; };
        static constexpr unsigned char max() { return UCHAR_MAX; };
    };

    template <>
    struct NumericLimits<wchar_t>
    {
        static constexpr wchar_t min() { return WCHAR_MIN; };
        static constexpr wchar_t max() { return WCHAR_MAX; };
    };

    // template <>
    // struct NumericLimits<char8_t>
    // {
    //     static constexpr char8_t min() { return 0; };
    //     static constexpr char8_t max() { return UCHAR_MAX; };
    // };

    template <>
    struct NumericLimits<char16_t>
    {
        static constexpr char16_t min() { return 0; };
        static constexpr char16_t max() { return UINT_LEAST16_MAX; };
    };

    template <>
    struct NumericLimits<char32_t>
    {
        static constexpr char32_t min() { return 0; };
        static constexpr char32_t max() { return UINT_LEAST32_MAX; };
    };

    template <>
    struct NumericLimits<short>
    {
        static constexpr short min() { return SHRT_MIN; };
        static constexpr short max() { return SHRT_MAX; };
    };

    template <>
    struct NumericLimits<unsigned short>
    {
        static constexpr unsigned short min() { return SHRT_MIN; };
        static constexpr unsigned short max() { return SHRT_MAX; };
    };

    template <>
    struct NumericLimits<int>
    {
        static constexpr int min() { return INT_MIN; };
        static constexpr int max() { return INT_MAX; };
    };

    // template <>
    // struct NumericLimits<signed int>
    // {
    //     static constexpr signed int min() { return INT_MIN; };
    //     static constexpr signed int max() { return INT_MAX; };
    // };

    template <>
    struct NumericLimits<unsigned int>
    {
        static constexpr unsigned int min() { return 0; };
        static constexpr unsigned int max() { return UINT_MAX; };
    };

    template <>
    struct NumericLimits<long>
    {
        static constexpr long min() { return LONG_MIN; };
        static constexpr long max() { return LONG_MAX; };
    };

    // template <>
    // struct NumericLimits<signed long>
    // {
    //     static constexpr signed long min() { return LDBL_MIN; };
    //     static constexpr signed long max() { return LONG_MAX; };
    // };

    template <>
    struct NumericLimits<unsigned long>
    {
        static constexpr unsigned long min() { return 0; };
        static constexpr unsigned long max() { return ULONG_MAX; };
    };

    template <>
    struct NumericLimits<long long>
    {
        static constexpr long min() { return LLONG_MIN; };
        static constexpr long max() { return LLONG_MAX; };
    };

    // template <>
    // struct NumericLimits<signed long long>
    // {
    //     static constexpr signed long long min() { return LLONG_MIN; };
    //     static constexpr signed long long max() { return LLONG_MAX; };
    // };

    template <>
    struct NumericLimits<unsigned long long>
    {
        static constexpr unsigned long long min() { return 0; };
        static constexpr unsigned long long max() { return ULLONG_MAX; };
    };

    template <>
    struct NumericLimits<float>
    {
        static constexpr float min() { return FLT_MIN; };
        static constexpr float max() { return FLT_MAX; };
    };

    template <>
    struct NumericLimits<double>
    {
        static constexpr double min() { return DBL_MIN; };
        static constexpr double max() { return DBL_MAX; };
    };
    template <>
    struct NumericLimits<long double>
    {
        static constexpr long double min() { return LDBL_MIN; };
        static constexpr long double max() { return LDBL_MAX; };
    };

}

#endif