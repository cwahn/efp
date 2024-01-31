#ifndef EFP_TRAIT_HPP_
#define EFP_TRAIT_HPP_

#include "efp/meta.hpp"

// Type-level function traits could be implemneted for external types
// Method requirements could be satisfied by actual implementation or inheritance

namespace efp {
// Element trait

// Rethrive element type from container
template <typename A>
struct ElementImpl {
};

template <typename A>
using Element = typename ElementImpl<Cleaned<A>>::Type;

// Sequence traits
// dyn

constexpr size_t dyn = -1;
using Dyn = Size<dyn>;

// CtSize
// Should be IntegralConstant<size_t> with compile time size.
// dyn for dynamic size

template <typename A>
struct CtSizeImpl {
};

template <typename A>
using CtSize = typename CtSizeImpl<Cleaned<A>>::Type;

// CtCapacity
// Should be IntegralConstant<size_t> with compile time capcity.
// dyn for dynamic size

template <typename A>
struct CtCapacityImpl {
};

template <typename A>
using CtCapacity = typename CtCapacityImpl<Cleaned<A>>::Type;

template <typename As, size_t n, typename = Void<CtSize<As>>>
constexpr auto length(const As& as) -> CtSize<As>;

template <typename As>
constexpr auto length(const As& as) -> size_t;

template <typename As, typename = Void<Element<As>>>
constexpr auto nth(size_t i, const As& as) -> const Element<As>&;

template <typename As, typename = Void<Element<As>>>
constexpr auto nth(size_t i, As& as) -> Element<As>&;

template <typename As, typename = Void<Element<As>>>
constexpr auto data(const As& as) -> const Element<As>*;

template <typename As, typename = Void<Element<As>>>
constexpr auto data(As& as) -> Element<As>*;

template <typename A, typename = void>
struct IsSequenceImplLength : False {
};

template <typename A>
struct IsSequenceImplLength<A, Void<decltype(efp::length(declval<const A>())), decltype(efp::length(declval<A>()))>>
    : Any<
          IsSame<decltype(efp::length(declval<const A>())), CtSize<A>>,
          IsSame<decltype(efp::length(declval<const A>())), size_t>> {
};

// nth
// todo non-const ref check

template <typename A, typename = void>
struct IsSequenceImplNth : False {
};

template <typename A>
struct IsSequenceImplNth<A, Void<decltype(efp::nth(declval<size_t>(), declval<const Cleaned<A>>())), decltype(efp::nth(declval<size_t>(), declval<Cleaned<A>>()))>>
    // : All<
    //       IsSame<decltype(nth(declval<size_t>(), declval<const Cleaned<A>>())), const Element<Cleaned<A>> &>,
    //       IsSame<decltype(nth(declval<size_t>(), declval<Cleaned<A>>())), Element<Cleaned<A>> &>>
    : IsSame<decltype(efp::nth(declval<size_t>(), declval<const Cleaned<A>>())), const Element<Cleaned<A>>&>

{
};

// data

template <typename A, typename = void>
struct IsSequenceImplData : False {
};

template <typename A>
struct IsSequenceImplData<A, Void<decltype(data(declval<const A>())), decltype(data(declval<A>()))>>
    // : All<
    //   IsSame<decltype(data(declval<const A>())), const Element<A> *>,
    //       IsSame<decltype(data(declval<A>())), Element<A> *>>

    : IsSame<decltype(data(declval<const A>())), const Element<A>*> {
};

template <typename A, typename = void>
struct IsSequence : False {
};

template <typename A>
struct IsSequence<A, Void<Element<A>, CtSize<A>, CtCapacity<A>>>
    : All<IsSequenceImplLength<A>,
          IsSequenceImplNth<A>,
          IsSequenceImplData<A>> {
};

// Utility type-level functions for Sequence trait

// IsStaticSize

template <typename A>
using IsStaticSize = Bool<CtSize<A>::value != dyn>;

// IsStaticCapacity

template <typename A>
using IsStaticCapacity = Bool<CtCapacity<A>::value != dyn>;

// // begin

// template <typename A>
// constexpr auto begin(const A &as)
//     -> const Element<A> *
// {
//     static_assert(IsSequence<A>::value, "Argument should be an instance of Sequence trait.");
//     return as.begin();
// }

// template <typename A>
// constexpr auto begin(A &as)
//     -> Element<A> *
// {
//     static_assert(IsSequence<A>::value, "Argument should be an instance of Sequence trait.");
//     return as.begin();
// }

// // end

// template <typename A>
// constexpr auto end(const A &as)
//     -> const Element<A> *
// {
//     static_assert(IsSequence<A>::value, "Argument should be an instance of Sequence trait.");
//     return as.end();
// }

// template <typename A>
// constexpr auto end(A &as)
//     -> Element<A> *
// {
//     static_assert(IsSequence<A>::value, "Argument should be an instance of Sequence trait.");
//     return as.end();
// }

} // namespace efp

#endif