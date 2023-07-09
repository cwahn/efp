#ifndef ZERO_COPY_TEST_HPP_
#define ZERO_COPY_TEST_HPP_

#include "zero_copy.hpp"

TEST_CASE("initialization")
{
    SECTION("StaticArray")
    {
        StaticArray<int, 3> a{1, 2, 3};
        CHECK(a == StaticArray<int, 3>{1, 2, 3});
        CHECK(a.size() == 3);
    }

    SECTION("StaticVector")
    {
        StaticVector<int, 4> a{1, 2, 3};
        StaticVector<int, 5> b{1, 2, 3};

        CHECK(a == b);
        CHECK(a.size() == 3);
        CHECK(b.size() == 3);
        CHECK(a.capacity() == 4);
        CHECK(b.capacity() == 5);
    }

    SECTION("DynamicVector")
    {
        DynamicVector<int> a{0, 0, 0};
        for (int i = 0; i < a.size(); ++i)
        {
            a[i] = i + 1;
        }

        DynamicVector<int> b{1, 2, 3};

        CHECK(a == b);
        CHECK(a.size() == 3);
        CHECK(b.size() == 3);
        CHECK(a.capacity() == 6);
        CHECK(b.capacity() == 6);
    }
}

StaticArray<int, 3> static_array_rvo()
{
    return StaticArray<int, 3>{1, 2, 3};
};

StaticArray<int, 3> static_array_nrvo()
{
    StaticArray<int, 3> result{0, 0, 0};

    result[0] = 1;
    result[1] = 2;
    result[2] = 3;

    return result;
};

StaticVector<int, 3> static_vector_rvo()
{
    return StaticVector<int, 3>{1, 2, 3};
};

StaticVector<int, 3> static_vector_nrvo()
{
    StaticVector<int, 3> result{0, 0, 0};

    result[0] = 1;
    result[1] = 2;
    result[2] = 3;

    return result;
};

DynamicVector<int> dynamic_vector_rvo()
{
    return DynamicVector<int>{1, 2, 3};
};

DynamicVector<int> dynamic_vector_nrvo()
{
    DynamicVector<int> result{0, 0, 0};

    result[0] = 1;
    result[1] = 2;
    result[2] = 3;

    return result;
};

TEST_CASE("copy elision")
{
    SECTION("StaticArray")
    {
        CHECK(StaticArray<int, 3>{1, 2, 3} == static_array_rvo());
        CHECK(StaticArray<int, 3>{1, 2, 3} == static_array_nrvo());
    }

    SECTION("StaticVector")
    {
        CHECK(StaticVector<int, 3>{1, 2, 3} == static_vector_rvo());
        CHECK(StaticVector<int, 3>{1, 2, 3} == static_vector_nrvo());
    }

    SECTION("DynamicVector")
    {
        CHECK(DynamicVector<int>{1, 2, 3} == dynamic_vector_rvo());
        CHECK(DynamicVector<int>{1, 2, 3} == dynamic_vector_nrvo());
    }
}

#endif