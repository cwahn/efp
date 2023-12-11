#include "sequence.hpp"

#include <iostream>
#include <string>
#include <vector>

// EFP_DEF_IMPL_MEMBER_FUNCTION(A, ImplIndexingOperator, operator(), Element<A>, size_t);

int main()
{
    using namespace efp;

    const Array<int, 3> arr{};

    std::cout << nth(0, arr) << std::endl;
    std::cout << data(arr) << std::endl;

    // static_assert(IsSequence<Array<int, 3>>::value, "Array<int, 3> should satisfy IsSequence");

    // static_assert(ImplTypeLevelFunction<Element, Array<int, 3>>::value, "Element trait should be satisfied by Array");
    // static_assert(ImplTypeLevelFunction<CtSize, Array<int, 3>>::value, "CtSize trait should be satisfied by Array");
    // static_assert(ImplTypeLevelFunction<CtCapacity, Array<int, 3>>::value, "CtCapacity trait should be satisfied by Array");

    // static_assert(IsSequenceImplLength<Array<int, 3>>::value, "IsSequenceImplLength should be satisfied by Array");
    // static_assert(IsSequenceImplNth<Array<int, 3>>::value, "IsSequenceImplNth should be satisfied by Array");
    // static_assert(IsSequenceImplData<Array<int, 3>>::value, "IsSequenceImplData should be satisfied by Array");

    // // ! temp

    // static_assert(IsSequenceImplNth<Array<int, 3>>::value, "IsSequenceImplNth should be satisfied by Array");

    // // Breaking down the checks for IsSequenceImplNth
    // // static_assert(IsInvocable<decltype(nth), size_t, const Array<int, 3> &>::value, "nth function is not invocable with const Array");
    // // static_assert(IsInvocable<decltype(nth), size_t, Array<int, 3> &>::value, "nth function is not invocable with non-const Array");

    // static_assert(IsSame<decltype(nth(0, std::declval<const Array<int, 3> &>())), const int &>::value, "nth for const Array does not return const int&");
    // static_assert(IsSame<decltype(nth(0, std::declval<Array<int, 3> &>())), int &>::value, "nth for non-const Array does not return int&");

    // static_assert(IsSequenceImplData<Array<int, 3>>::value, "IsSequenceImplData should be satisfied by Array");

    // // Breaking down the checks for IsSequenceImplData
    // // static_assert(IsInvocable<decltype(data), const Array<int, 3> &>::value, "data function is not invocable with const Array");
    // // static_assert(IsInvocable<decltype(data), Array<int, 3> &>::value, "data function is not invocable with non-const Array");

    // static_assert(IsSame<decltype(data(std::declval<const Array<int, 3> &>())), const int *>::value, "data for const Array does not return const int*");
    // static_assert(IsSame<decltype(data(std::declval<Array<int, 3> &>())), int *>::value, "data for non-const Array does not return int*");
    return 0;
}
