#include "include/efp.hpp"

using namespace efp;

bool KKK = true;

Array<int, 3> *array_nrvo_p = nullptr;
ArrVec<int, 3> *arrvec_nrvo_p = nullptr;
Vector<int> *vector_nrvo_p = nullptr;

Array<int, 3> array_rvo()
{
    return Array<int, 3>{1, 2, 3};
};

Array<int, 3> array_nrvo()
{
    Array<int, 3> result{0, 0, 0};
    Array<int, 3> result2{0, 0, 0};

    result[0] = 1;
    result[1] = 2;
    result[2] = 3;
    array_nrvo_p = &result;

    return result;//KKK ? result : result2;
};


Vector<int> vector_rvo()
{
    return Vector<int>{1, 2, 3};
};

Vector<int> vector_nrvo()
{
    Vector<int> result{0, 0, 0};
    Vector<int> result2{0, 0, 0};

    result[0] = 1;
    result[1] = 2;
    result[2] = 3;
    vector_nrvo_p = &result;

    return KKK ? result : result2;
};

int main(int argc, char const *argv[])
{
    // Vector<int> na = vector_nrvo();

    // // CHECK(Vector<int>{1, 2, 3} == vector_rvo());
    // if (&na == vector_nrvo_p)
    //     std::cout << "vector_nrvo" << std::endl;

    // else
    //     std::cout << "f1" << std::endl;




    // if (na == Vector<int>{1, 2, 3})
    //     std::cout << "vector_rvo" << std::endl;

    // else
    //     std::cout << "f2" << std::endl;

    // for (auto a : na)
    //     std::cout <<a << std::endl;

////////////////////////////////////////////////
    Array<int, 3> ana = array_nrvo();

    // CHECK(Vector<int>{1, 2, 3} == vector_rvo());
    if (&ana == array_nrvo_p)
        std::cout << "array_nrvo" << std::endl;

    else
        std::cout << "af1" << std::endl;




    if (ana == Array<int, 3>{1, 2, 3})
        std::cout << "array_rvo" << std::endl;

    else
        std::cout << "af2" << std::endl;

    for (auto a : ana)
        std::cout <<a << std::endl;

    return 0;
}
