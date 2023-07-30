#ifndef ZERO_COPY_TEST_HPP_
#define ZERO_COPY_TEST_HPP_

#include "efp.hpp"

using namespace efp;

TEST_CASE("initialization")
{
    SECTION("Array")
    {
        Array<int, 3> a{1, 2, 3};
        CHECK(a == Array<int, 3>{1, 2, 3});
        CHECK(a.size() == 3);
    }

    SECTION("ArrayVector")
    {
        ArrayVector<int, 4> a = {1, 2, 3};

        CHECK(a.size() == 3);
        CHECK(a.capacity() == 4);
    }

    SECTION("Vector")
    {
        Vector<int> a = {0, 0, 0};
        for (int i = 0; i < a.size(); ++i)
        {
            a[i] = i + 1;
        }

        Vector<int> b = {1, 2, 3};

        CHECK(a == b);
        CHECK(a.size() == 3);
        CHECK(b.size() == 3);
        CHECK(a.capacity() == 3);
        CHECK(b.capacity() == 3);
    }
}

Array<int, 3> array_rvo()
{
    return Array<int, 3>{1, 2, 3};
};

Array<int, 3> array_nrvo()
{
    Array<int, 3> result{0, 0, 0};

    result[0] = 1;
    result[1] = 2;
    result[2] = 3;

    return result;
};

ArrayVector<int, 3> array_vector_rvo()
{
    return ArrayVector<int, 3>{1, 2, 3};
};

ArrayVector<int, 3> array_vector_nrvo()
{
    ArrayVector<int, 3> result{0, 0, 0};

    result[0] = 1;
    result[1] = 2;
    result[2] = 3;

    return result;
};

Vector<int> vector_rvo()
{
    return Vector<int>{1, 2, 3};
};

Vector<int> vector_nrvo()
{
    Vector<int> result{0, 0, 0};

    result[0] = 1;
    result[1] = 2;
    result[2] = 3;

    return result;
};

TEST_CASE("copy elision")
{
    SECTION("Array")
    {
        CHECK(Array<int, 3>{1, 2, 3} == array_rvo());
        CHECK(Array<int, 3>{1, 2, 3} == array_nrvo());
    }

    SECTION("ArrayVector")
    {
        CHECK(ArrayVector<int, 3>{1, 2, 3} == array_vector_rvo());
        CHECK(ArrayVector<int, 3>{1, 2, 3} == array_vector_nrvo());
    }

    SECTION("Vector")
    {
        CHECK(Vector<int>{1, 2, 3} == vector_rvo());
        CHECK(Vector<int>{1, 2, 3} == vector_nrvo());
    }
}

TEST_CASE("assignment")
{
    SECTION("Array")
    {
        Array<double, 3> a{1., 2., 3.};
        Array<double, 3> b;

        b = a;
        CHECK(a == b);
    }

    SECTION("ArrayVector")
    {
        ArrayVector<double, 3> a{1., 2., 3.};
        ArrayVector<double, 3> b;

        b = a;
        CHECK(a == b);
    }

    SECTION("Vector")
    {
        Vector<double> a{1., 2., 3.};
        Vector<double> b;

        b = a;
        CHECK(a == b);
    }
}

#endif