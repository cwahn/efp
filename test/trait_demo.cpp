#include "efp.hpp"

#include <iostream>
#include <string>
#include <vector>

EFP_DEF_IMPL_MEMBER_FUNCTION(A, ImplIndexingOperator, operator(), Element<A>, size_t);

int main()
{
    using namespace efp;

    if (ImplTypeLevelFunction<Array<int, 3>, Element>::value)
        std::cout << "ImplElement : True" << std::endl;
    else
        std::cout << "ImplElement : False" << std::endl;

    if (ImplIndexingOperator<Array<int, 3>>::value)
        std::cout << "ImplIndexing : True" << std::endl;
    else
        std::cout << "ImplIndexing : False" << std::endl;

    return 0;
}
