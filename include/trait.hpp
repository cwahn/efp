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

    // template <typename F, F f, typename R, typaname A>
    // using ImplFunction = IsSame<CallReturn<F, A>, R>;

    // Element
    // Should get Element type of containter

    template <typename A>
    struct ElementImpl
    {
        // using Type = typename A::Element;
    };

    template <typename A>
    using Element = typename ElementImpl<ConstRemoved<ReferenceRemoved<A>>>::Type;

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
        // using Type = typename A::CtSize;
    };

    template <typename A>
    using CtSize = typename CtSizeImpl<ConstRemoved<ReferenceRemoved<A>>>::Type;

    // CtCapacity
    // Should be IntegralConstant<size_t> with compile time capcity.
    // dyn for dynamic size

    template <typename A>
    struct CtCapacityImpl
    {
        // using Type = typename A::CtCapacity;
    };

    template <typename A>
    using CtCapacity = typename CtCapacityImpl<ConstRemoved<ReferenceRemoved<A>>>::Type;

    // length
    // ? Maybe need to be at prelude

    // template <typename A>
    // constexpr auto length(const A &as)
    //     -> EnableIf<CtSize<A>::value != dyn, CtSize<A>>;
    // // {
    // //     static_assert(IsSequence<A>::value, "Argument should be an instance of sequence trait.");
    // //     return {};
    // // }

    // template <typename A>
    // constexpr auto length(const A &as)
    //     -> EnableIf<CtSize<A>::value == dyn, size_t>;
    // // {
    // //     static_assert(IsSequence<A>::value, "Argument should be an instance of sequence trait.");
    // //     return as.size();
    // // }

    // // data

    // template <typename A>
    // constexpr auto data(const A &as) -> const Element<A> *;
    // // {
    // //     static_assert(IsSequence<A>::value, "Argument should be an instance of sequence trait.");
    // //     return as.data();
    // // }

    // template <typename A>
    // constexpr auto data(A &as) -> Element<A> *;
    // // {
    // //     static_assert(IsSequence<A>::value, "Argument should be an instance of sequence trait.");
    // //     return as.data();
    // // }

    // // nth

    // template <typename A>
    // constexpr auto nth(size_t n, const A &as) -> const Element<A> &;
    // // {
    // // }

    // template <typename A>
    // constexpr auto nth(size_t n, A &as) -> Element<A> &;
    // // {
    // // }

    // template <typename A>
    // constexpr auto length(const A &as)
    //     -> EnableIf<CtSize<A>::value != dyn, CtSize<A>>;

    // template <typename A>
    // constexpr auto length(const A &as)
    //     -> EnableIf<CtSize<A>::value == dyn, size_t>;

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

    // template <typename A>
    // using IsSequenceImplLength = Any<
    //     IsSame<decltype(length(declval<const A>())), CtSize<A>>,
    //     IsSame<decltype(length(declval<const A>())), size_t>>;

    // nth

    // template <typename A>
    // constexpr auto nth(size_t n, const A &as) -> const Element<A> &;

    // template <typename A>
    // constexpr auto nth(size_t n, A &as) -> Element<A> &;

    // template <typename A>
    // using IsSequenceImplNth = All<
    //     IsSame<decltype(data(declval<const A>())), const Element<A> *>,
    //     IsSame<decltype(data(declval<A>())), Element<A> *>>;

    template <typename A, typename = void>
    struct IsSequenceImplNth : False
    {
    };

    template <typename A>
    struct IsSequenceImplNth<A, Void<decltype(nth(declval<size_t>(), declval<const A>())), decltype(nth(declval<size_t>(), declval<A>()))>>
        : All<
              IsSame<decltype(nth(declval<size_t>(), declval<const A>())), const Element<A> &>,
              IsSame<decltype(nth(declval<size_t>(), declval<A>())), Element<A> &>>
    {
    };

    // data

    // template <typename A>
    // constexpr auto data(const A &as) -> const Element<A> *;

    // template <typename A>
    // constexpr auto data(A &as) -> Element<A> *;

    template <typename A, typename = void>
    struct IsSequenceImplData : False
    {
    };

    template <typename A>
    struct IsSequenceImplData<A, Void<decltype(data(declval<const A>())), decltype(data(declval<A>()))>>
        : All<
              IsSame<decltype(data(declval<const A>())), const Element<A> *>,
              IsSame<decltype(data(declval<A>())), Element<A> *>>
    {
    };

    // template <typename A>
    // using IsSequenceImplData = All<
    //     IsSame<decltype(data(declval<const A>())), const Element<A> *>,
    //     IsSame<decltype(data(declval<A>())), Element<A> *>>;

    template <typename A>
    using IsSequence = All<
        ImplTypeLevelFunction<Element, A>,
        ImplTypeLevelFunction<CtSize, A>,
        ImplTypeLevelFunction<CtCapacity, A>,
        IsSequenceImplLength<A>
        // ! Trait is not working properlly at the moments
        // IsSequenceImplNth<A>,
        // IsSequenceImplData<A>,
        >;

    ////////////////////////////////////////////////////////////////////////

    // Utility Functionsƒ
    // IsStaticSize

    template <typename A>
    // struct IsStaticSize
    // {
    //     static constexpr bool value = CtSize<A>::value != dyn;
    // };
    using IsStaticSize = Bool<CtSize<A>::value != dyn>;

    // todo Remove AreAlls and Min Max x

    // AreAllStaticSize

    template <typename... As>
    // struct AreAllStaticSize
    // {
    //     static constexpr bool value = all_v(IsStaticSize<A>::value...);
    // };
    using AreAllStaticSize = All<IsStaticSize<As>...>;

    // IsStaticCapacity

    template <typename A>
    // struct IsStaticCapacity
    // {
    //     static constexpr bool value = A::ct_capacity != dyn;
    // };
    using IsStaticCapacity = Bool<CtCapacity<A>::value != dyn>;

    // AreAllStaticCapacity

    template <typename... As>
    // struct AreAllStaticCapacity
    // {
    //     static constexpr bool value = all_v(IsStaticCapacity<A>::value...);
    // };
    using AreAllStaticCapacity = All<IsStaticCapacity<As>...>;

    // MinStaticLength

    template <typename... As>
    // struct MinStaticLength
    // {
    //     static constexpr size_t value = minimum_v(StaticLength<A>::value...);
    // };
    using MinStaticLength = Min<CtSize<As>...>;

    // MinStaticCapacity

    template <typename... As>
    // struct MinStaticCapacity
    // {
    //     static constexpr size_t value = minimum_v(StaticCapacity<A>::value...);
    // };

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