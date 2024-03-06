#ifndef EFP_TRAIT_HPP_
#define EFP_TRAIT_HPP_

#include "efp/meta.hpp"

// Type-level function traits could be implemneted for external types
// Method requirements could be satisfied by actual implementation or inheritance

namespace efp {
// Element trait

// Rethrive element type from container
template<typename A>
struct ElementImpl {
    static_assert(AlwaysFalse<A>::value, "Element trait is not implemented for the type");
    ;
};

template<typename A>
using Element = typename ElementImpl<CVRefRemoved<A>>::Type;

// Sequence traits
// dyn

constexpr size_t dyn = -1;
using Dyn = Size<dyn>;

// CtSize
// Should be IntegralConstant<size_t> with compile time size.
// dyn for dynamic size

template<typename A>
struct CtSizeImpl {
    static_assert(AlwaysFalse<A>::value, "CtSize trait is not implemented for the type");
    ;
};

template<typename A>
using CtSize = typename CtSizeImpl<CVRefRemoved<A>>::Type;

// CtCapacity
// Should be IntegralConstant<size_t> with compile time capcity.
// dyn for dynamic size

template<typename A>
struct CtCapacityImpl {
    static_assert(AlwaysFalse<A>::value, "CtCapacity trait is not implemented for the type");
    ;
};

template<typename A>
using CtCapacity = typename CtCapacityImpl<CVRefRemoved<A>>::Type;

template<typename As, size_t n, typename = Void<CtSize<As>>>
constexpr auto length(const As& as) -> CtSize<As> {
    static_assert(AlwaysFalse<As>::value, "length is not implemented for the type");
    return {};
}

template<typename As>
constexpr auto length(const As& as) -> size_t {
    static_assert(AlwaysFalse<As>::value, "length is not implemented for the type");
    return {};
}

template<typename As, typename = Void<Element<As>>>
constexpr auto nth(size_t i, const As& as) -> const Element<As>& {
    static_assert(AlwaysFalse<As>::value, "nth is not implemented for the type");
    return {};
}

template<typename As, typename = Void<Element<As>>>
constexpr auto nth(size_t i, As& as) -> Element<As>& {
    static_assert(AlwaysFalse<As>::value, "nth is not implemented for the type");
    return {};
}

template<typename As, typename = Void<Element<As>>>
constexpr auto data(const As& as) -> const Element<As>* {
    static_assert(AlwaysFalse<As>::value, "data is not implemented for the type");
    return {};
}

template<typename As, typename = Void<Element<As>>>
constexpr auto data(As& as) -> Element<As>* {
    static_assert(AlwaysFalse<As>::value, "data is not implemented for the type");
    return {};
}

// length

template<typename A, typename = void>
struct IsSequenceImplLength: False {};

template<typename A>
struct IsSequenceImplLength<
    A,
    Void<decltype(efp::length(declval<const A>())), decltype(efp::length(declval<A>()))>>:
    Any<IsSame<decltype(efp::length(declval<const A>())), CtSize<A>>,
        IsSame<decltype(efp::length(declval<const A>())), size_t>> {};

// template<typename A>
// using IsSequenceImplLength =
//     Any<IsInvocableR<CtSize<A>, decltype(length), const A&>,
//         IsInvocableR<size_t, decltype(length), const A&>>;

// nth
// todo non-const ref check

template<typename A, typename = void>
struct IsSequenceImplNth: False {};

template<typename A>
struct IsSequenceImplNth<
    A,
    Void<
        decltype(efp::nth(declval<size_t>(), declval<const CVRefRemoved<A>>())),
        decltype(efp::nth(declval<size_t>(), declval<CVRefRemoved<A>>()))>>
    // : All<
    //       IsSame<decltype(nth(declval<size_t>(), declval<const CVRefRemoved<A>>())), const Element<CVRefRemoved<A>> &>,
    //       IsSame<decltype(nth(declval<size_t>(), declval<CVRefRemoved<A>>())), Element<CVRefRemoved<A>> &>>
    :
    IsSame<
        decltype(efp::nth(declval<size_t>(), declval<const CVRefRemoved<A>>())),
        const Element<CVRefRemoved<A>>&> {};

// template<typename A>
// using IsSequenceImplNth = IsInvocableR<const Element<A>&, decltype(nth), size_t, const A&>;

// data

template<typename A, typename = void>
struct IsSequenceImplData: False {};

template<typename A>
struct IsSequenceImplData<A, Void<decltype(data(declval<const A>())), decltype(data(declval<A>()))>>
    // : All<
    //   IsSame<decltype(data(declval<const A>())), const Element<A> *>,
    //       IsSame<decltype(data(declval<A>())), Element<A> *>>

    : IsSame<decltype(data(declval<const A>())), const Element<A>*> {};

// template<typename A>
// using IsSequenceImplData = IsInvocableR<const Element<A>*, decltype(data), const A&>;

// IsSequence

template<typename A, typename = void>
struct IsSequence: False {};

template<typename A>
struct IsSequence<A, Void<Element<A>, CtSize<A>, CtCapacity<A>>>:
    // All<IsSequenceImplLength<A>, IsSequenceImplNth<A>, IsSequenceImplData<A>> {};
    Bool<_all(
        {IsSequenceImplLength<A>::value, IsSequenceImplNth<A>::value, IsSequenceImplData<A>::value}
    )> {};

// Utility type-level functions for Sequence trait

// IsStaticSize

template<typename A>
using IsStaticSize = Bool<CtSize<A>::value != dyn>;

// IsStaticCapacity

template<typename A>
using IsStaticCapacity = Bool<CtCapacity<A>::value != dyn>;

}  // namespace efp

#endif