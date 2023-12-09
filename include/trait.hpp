#ifndef EFP_TRAIT_HPP_
#define EFP_TRAIT_HPP_

#include "sfinae.hpp"

// Type-level function traits could be implemneted for external types
// Method requirements could be satisfied by actual implementation or inheritance

namespace efp
{
    template <typename A, template <typename> class TypeLevelFunction, typename = void>
    struct ImplTypeLevelFunction : False
    {
    };

    template <typename A, template <typename> class TypeLevelFunction>
    struct ImplTypeLevelFunction<A, TypeLevelFunction, Void<TypeLevelFunction<A>>> : True
    {
    };

    // template <typename F, typename Ret, typename... Args>
    // using SignatureCheck = All<IsInvocable<F, Args...>, IsSame<CallReturn<F, Args...>, Ret>>;

    // template <typename A, typename Ret, typename... Args>
    // using HasSomeFunction = All<
    //     IsSame<Void<decltype(static_cast<Ret (A::*)(Args...)>(&A::some_func))>, void>,
    //     SignatureCheck<decltype(&A::some_func), Ret, Args...>>;

    // // template <typename A>
    // // using HasSomeFunction =
    // //     IsSame<
    // //         Void(decltype(declval<A>().some_func(declval<Arg1>(), declval<Arg2>()))),
    // //         void>;

    // template <typename A>
    // using TraitName = IsSame<decltype(declval<A>().func_name(declval<Arg1>(), declval<Arg2>())), Ret>;

    // template <typename A>
    // void require_func(A a, size_t size)
    // {
    //     IsSame<decltype(a[size]), Ret>;
    // }

    // Element
    // Should get Element type of containter

    template <typename A>
    struct ElementImpl
    {
        using Type = typename A::Element;
    };

    template <typename A>
    using Element = typename ElementImpl<A>::Type;

    // dyn

    constexpr size_t dyn = -1;

    using Dyn = Size<dyn>;

    // CtSize
    // Should be IntegralConstant<size_t> with compile time size.
    // dyn for dynamic size

    template <typename A>
    struct CtSizeImpl
    {
        using Type = typename A::CtSize;
    };

    template <typename A>
    using CtSize = typename CtSizeImpl<A>::Type;

    // CtCapcity
    // Should be IntegralConstant<size_t> with compile time capcity.
    // dyn for dynamic size

    template <typename A>
    struct CtCapcityImpl
    {
        using Type = typename A::CtSize;
    };

    template <typename A>
    using CtCapcity = typename CtCapcityImpl<A>::Type;

    ////////////////////////////////////////////////////////////////////////

    // Utility Functions
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
    using IsStaticCapacity = Bool<CtCapcity<A>::value != dyn>;

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

    using MinStaticCapacity = Min<CtCapcity<As>...>;

    // length
    // ? Maybe need to be at prelude

    template <typename A>
    constexpr auto length(const A &as)
        -> EnableIf<CtSize<A>::value != dyn, CtSize<A>>
    {
        return {};
    }

    template <typename A>
    constexpr auto length(const A &as)
        -> EnableIf<CtSize<A>::value == dyn, size_t>
    {
        return as.size();
    }

    // data

    template <typename A>
    constexpr auto data(const A &as)
        -> const Element<A> *
    {
        return as.data();
    }

    template <typename A>
    constexpr auto data(A &as)
        -> Element<A> *
    {
        return as.data();
    }

    // begin

    template <typename A>
    constexpr auto begin(const A &as)
        -> const Element<A> *
    {
        return as.begin();
    }

    template <typename A>
    constexpr auto begin(A &as)
        -> Element<A> *
    {
        return as.begin();
    }

    // end

    template <typename A>
    constexpr auto end(const A &as)
        -> const Element<A> *
    {
        return as.end();
    }

    template <typename A>
    constexpr auto end(A &as)
        -> Element<A> *
    {
        return as.end();
    }

} // namespace efp

#endif