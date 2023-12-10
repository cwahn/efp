#ifndef EFP_TRAIT_HPP_
#define EFP_TRAIT_HPP_

#include "sfinae.hpp"

// Type-level function traits could be implemneted for external types
// Method requirements could be satisfied by actual implementation or inheritance

namespace efp
{
    template <template <typename> class TypeLevelFunction, typename A, typename = void>
    struct ImplTypeLevelFunction : False
    {
    };

    template <template <typename> class TypeLevelFunction, typename A>
    struct ImplTypeLevelFunction<TypeLevelFunction, A, Void<TypeLevelFunction<A>>> : True
    {
    };

    template <typename A>
    struct ElementImpl
    {
    };

    template <typename A>
    using Element = typename ElementImpl<Cleaned<A>>::Type;

    // Sequence specific traits

    // dyn

    constexpr size_t dyn = -1;

    using Dyn = Size<dyn>;

    // CtSize
    // Should be IntegralConstant<size_t> with compile time size.
    // dyn for dynamic size

    template <typename A>
    struct CtSizeImpl
    {
    };

    template <typename A>
    using CtSize = typename CtSizeImpl<Cleaned<A>>::Type;

    // CtCapacity
    // Should be IntegralConstant<size_t> with compile time capcity.
    // dyn for dynamic size

    template <typename A>
    struct CtCapacityImpl
    {
    };

    template <typename A>
    using CtCapacity = typename CtCapacityImpl<Cleaned<A>>::Type;

    template <typename A, typename = void>
    struct IsSequenceImplLength : False
    {
    };

    template <typename A>
    struct IsSequenceImplLength<A, Void<decltype(length(declval<const A>())), decltype(length(declval<A>()))>>
        : Any<
              IsSame<decltype(length(declval<const A>())), CtSize<A>>,
              IsSame<decltype(length(declval<const A>())), size_t>>
    {
    };

    // nth

    template <typename A, typename = void>
    struct IsSequenceImplNth : False
    {
    };

    template <typename A>
    struct IsSequenceImplNth<A, Void<decltype(nth(declval<size_t>(), declval<const Cleaned<A>>())), decltype(nth(declval<size_t>(), declval<Cleaned<A>>()))>>
        // : All<
        //       IsSame<decltype(nth(declval<size_t>(), declval<const Cleaned<A>>())), const Element<Cleaned<A>> &>,
        //       IsSame<decltype(nth(declval<size_t>(), declval<Cleaned<A>>())), Element<Cleaned<A>> &>>
        : IsSame<decltype(nth(declval<size_t>(), declval<const Cleaned<A>>())), const Element<Cleaned<A>> &>
    // : IsSame<decltype(nth(declval<size_t>(), declval<Cleaned<A>>())), Element<Cleaned<A>> &>

    {
    };

    // data

    template <typename A, typename = void>
    struct IsSequenceImplData : False
    {
    };

    template <typename A>
    struct IsSequenceImplData<A, Void<decltype(data(declval<const A>())), decltype(data(declval<A>()))>>
        // : All<
        //   IsSame<decltype(data(declval<const A>())), const Element<A> *>,
        //       IsSame<decltype(data(declval<A>())), Element<A> *>>

        : IsSame<decltype(data(declval<const A>())), const Element<A> *>
    {
    };

    template <typename A>
    using IsSequence = All<
        ImplTypeLevelFunction<Element, A>,
        ImplTypeLevelFunction<CtSize, A>,
        ImplTypeLevelFunction<CtCapacity, A>,
        IsSequenceImplLength<A>,
        IsSequenceImplNth<A>,
        IsSequenceImplData<A>>;

    ////////////////////////////////////////////////////////////////////////

    // Utility Functions
    // IsStaticSize

    template <typename A>
    using IsStaticSize = Bool<CtSize<A>::value != dyn>;

    // todo Remove AreAlls and Min Max x

    // AreAllStaticSize

    template <typename... As>
    using AreAllStaticSize = All<IsStaticSize<As>...>;

    // IsStaticCapacity

    template <typename A>
    using IsStaticCapacity = Bool<CtCapacity<A>::value != dyn>;

    // AreAllStaticCapacity

    template <typename... As>
    using AreAllStaticCapacity = All<IsStaticCapacity<As>...>;

    // MinStaticLength

    template <typename... As>
    using MinStaticLength = Min<CtSize<As>...>;

    // MinStaticCapacity

    template <typename... As>
    using MinStaticCapacity = Min<CtCapacity<As>...>;

    // // begin

    // template <typename A>
    // constexpr auto begin(const A &as)
    //     -> const Element<A> *
    // {
    //     static_assert(IsSequence<A>::value, "Argument should be an instance of sequence trait.");
    //     return as.begin();
    // }

    // template <typename A>
    // constexpr auto begin(A &as)
    //     -> Element<A> *
    // {
    //     static_assert(IsSequence<A>::value, "Argument should be an instance of sequence trait.");
    //     return as.begin();
    // }

    // // end

    // template <typename A>
    // constexpr auto end(const A &as)
    //     -> const Element<A> *
    // {
    //     static_assert(IsSequence<A>::value, "Argument should be an instance of sequence trait.");
    //     return as.end();
    // }

    // template <typename A>
    // constexpr auto end(A &as)
    //     -> Element<A> *
    // {
    //     static_assert(IsSequence<A>::value, "Argument should be an instance of sequence trait.");
    //     return as.end();
    // }

} // namespace efp

#endif