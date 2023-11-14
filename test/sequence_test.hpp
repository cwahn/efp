#ifndef SEQUENCE_TEST_HPP_
#define SEQUENCE_TEST_HPP_

#include "catch2/catch_test_macros.hpp"
#include "sequence.hpp"

using namespace efp;

TEST_CASE("Initialization")
{
    SECTION("Array Default Constructor")
    {
        Array<int, 5> a;
        CHECK(a.size() == 5); // Assuming default constructor initializes size to 0
    }

    SECTION("ArrVec Default Constructor")
    {
        ArrVec<int, 5> a;
        CHECK(a.size() == 0);
        CHECK(a.capacity() == 5);
    }

    SECTION("Vector Default Constructor")
    {
        Vector<int> a;
        CHECK(a.size() == 0);
        CHECK(a.capacity() == 0); // Assuming default capacity is 0
    }

    SECTION("Array Initialization")
    {
        Array<int, 3> a{1, 2, 3};
        CHECK(a == Array<int, 3>{1, 2, 3});
        CHECK(a.size() == 3);
    }

    SECTION("ArrVec Initialization")
    {
        ArrVec<int, 4> a = {1, 2, 3};
        CHECK(a.size() == 3);
        CHECK(a.capacity() == 4);
    }

    SECTION("Vector Initialization")
    {
        Vector<int> a = {1, 2, 3};
        CHECK(a.size() == 3);
        CHECK(a.capacity() == 3);
    }

    SECTION("Array Copy Constructor")
    {
        Array<int, 3> a{1, 2, 3};
        Array<int, 3> b = a;
        CHECK(b == Array<int, 3>{1, 2, 3});
    }

    SECTION("ArrVec Copy Constructor")
    {
        ArrVec<int, 4> a = {1, 2, 3};
        ArrVec<int, 4> b = a;
        CHECK(b.size() == 3);
        CHECK(b.capacity() == 4);
    }

    SECTION("Vector Copy Constructor")
    {
        Vector<int> a = {1, 2, 3};
        Vector<int> b = a;
        CHECK(b.size() == 3);
        CHECK(b.capacity() == 3);
    }

    SECTION("Array Assignment")
    {
        Array<int, 3> a{1, 2, 3};
        Array<int, 3> b;
        b = a;
        CHECK(b == Array<int, 3>{1, 2, 3});
    }

    SECTION("ArrVec Assignment")
    {
        ArrVec<int, 4> a = {1, 2, 3};
        ArrVec<int, 4> b;
        b = a;
        CHECK(b.size() == 3);
        CHECK(b.capacity() == 4);
    }

    SECTION("Vector Assignment")
    {
        Vector<int> a = {1, 2, 3};
        Vector<int> b;
        b = a;
        CHECK(b.size() == 3);
        CHECK(b.capacity() == 3);
    }
}

bool runtime_con = true;

Array<int, 3> *array_nrvo_p = nullptr;
ArrVec<int, 3> *arrvec_nrvo_p = nullptr;
Vector<int> *vector_nrvo_p = nullptr;

Array<int, 3> *n_array_nrvo_p = nullptr;
ArrVec<int, 3> *n_arrvec_nrvo_p = nullptr;
Vector<int> *n_vector_nrvo_p = nullptr;

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
    array_nrvo_p = &result;

    return result;
};

Array<int, 3> n_array_nrvo()
{
    Array<int, 3> result{0, 0, 0};
    Array<int, 3> n_result{1, 2, 3};

    result[0] = 1;
    result[1] = 2;
    result[2] = 3;
    n_array_nrvo_p = &result;

    return runtime_con ? result : n_result;
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
    arrvec_nrvo_p = &result;

    return result;
};

ArrVec<int, 3> n_array_vector_nrvo()
{
    ArrVec<int, 3> result{0, 0, 0};
    ArrVec<int, 3> n_result{1, 2, 3};

    result[0] = 1;
    result[1] = 2;
    result[2] = 3;
    n_arrvec_nrvo_p = &result;

    return runtime_con ? result : n_result;
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
    vector_nrvo_p = &result;

    return result;
};

Vector<int> n_vector_nrvo()
{
    Vector<int> result{0, 0, 0};
    Vector<int> n_result{1, 2, 3};

    result[0] = 1;
    result[1] = 2;
    result[2] = 3;
    n_vector_nrvo_p = &result;

    return runtime_con ? result : n_result;
};

TEST_CASE("copy elision")
{
    SECTION("Array")
    {
        Array<int, 3> na = array_nrvo();
        Array<int, 3> nna = n_array_nrvo();

        // CHECK(Array<int, 3>{1, 2, 3} == array_rvo());
        CHECK(&na == array_nrvo_p);
        CHECK(&nna != n_array_nrvo_p);
    }

    SECTION("ArrVec")
    {
        ArrVec<int, 3> na = array_vector_nrvo();
        ArrVec<int, 3> nna = n_array_vector_nrvo();

        // CHECK(ArrVec<int, 3>{1, 2, 3} == array_vector_rvo());
        CHECK(&na == arrvec_nrvo_p);
        CHECK(&nna != n_arrvec_nrvo_p);
    }

    SECTION("Vector")
    {
        Vector<int> na = vector_nrvo();
        Vector<int> nna = n_vector_nrvo();

        // CHECK(Vector<int>{1, 2, 3} == vector_rvo());
        CHECK(&na == vector_nrvo_p);
        CHECK(&nna != n_vector_nrvo_p);
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

TEST_CASE("erase")
{
    SECTION("ArrVec::erase")
    {
        ArrVec<int, 5> arrvec{1, 2, 3, 4, 5};
        arrvec.erase(2); // Erase the element at index 2, which is '3'

        ArrVec<int, 5> expected{1, 2, 4, 5};
        CHECK(arrvec == expected);
    }

    SECTION("Vector::erase")
    {
        Vector<int> vec{1, 2, 3, 4, 5};
        vec.erase(2); // Erase the element at index 2, which is '3'

        Vector<int> expected{1, 2, 4, 5};
        CHECK(vec == expected);
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
        VectorView<const double> view_1{data(array_3), 3, 3};
        auto view_2 = view_1;
        CHECK(view_1 == view_2);
    }
}

TEST_CASE("Vector Copy")
{
    SECTION("Vector copy")
    {
        Vector<double> view_1{3., 3., 3.};
        auto view_2 = view_1;
        CHECK(view_1 == view_2);
    }
}

// TEST_CASE("Element_t")
// {
//     CHECK(IsSame<double, Element_t<decltype(array_3)>>::value == true);
//     //  Element_t is not catching const of container.
//     CHECK(IsSame<const double, Element_t<decltype(array_3)>>::value == false);
// }

#endif