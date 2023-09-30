#ifndef SEQUENCE_TEST_HPP_
#define SEQUENCE_TEST_HPP_

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

    SECTION("ArrVec")
    {
        ArrVec<int, 4> a = {1, 2, 3};

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

ArrVec<int, 3> array_vector_rvo()
{
    return ArrVec<int, 3>{1, 2, 3};
};

ArrVec<int, 3> array_vector_nrvo()
{
    ArrVec<int, 3> result{0, 0, 0};

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

    SECTION("ArrVec")
    {
        CHECK(ArrVec<int, 3>{1, 2, 3} == array_vector_rvo());
        CHECK(ArrVec<int, 3>{1, 2, 3} == array_vector_nrvo());
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

    SECTION("ArrVec")
    {
        ArrVec<double, 3> a{1., 2., 3.};
        ArrVec<double, 3> b;

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

TEST_CASE("ArrayView")
{
    SECTION("construction")
    {
        int a = 0;
        const int a_const = 0;

        // const pointer will create read-only view.

        ArrayView<int, 1> a_view0{&a};
        ArrayView<const int, 1> a_view1{&a_const};
        const ArrayView<int, 1> a_view2{&a};
        const ArrayView<const int, 1> a_view3{&a_const};
        // ArrayView<int, 1> a_view4{&a_const};
        ArrayView<const int, 1> a_view5{&a};
        // const ArrayView<int, 1> a_view6{&a_const};
        const ArrayView<const int, 1> a_view7{&a};
    }

    SECTION("copy construction")
    {
        ArrayView<const double, 3> view_1{data(array_3)};
        auto view_2 = view_1;
        CHECK(view_1 == view_2);
    }
}

TEST_CASE("VectorView")
{
    SECTION("copy construction")
    {
        VectorView<const double> view_1{data(array_3), 3};
        auto view_2 = view_1;
        CHECK(view_1 == view_2);
    }
}

TEST_CASE("Element_t")
{
    CHECK(IsSame<double, Element_t<decltype(array_3)>>::value == true);
    //  Element_t is not catching const of container.
    CHECK(IsSame<const double, Element_t<decltype(array_3)>>::value == false);
}

#endif