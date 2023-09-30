#ifndef SFINAE_HPP_
#define SFINAE_HPP_

#include "limits.hpp"

// Make it value as soon as possible and use constexpr function and avoid type_trait.

namespace efp
{
    // True False type

    struct Unit
    {
    };

    struct True
    {
        static constexpr bool value = true;
    };

    struct False
    {
        static constexpr bool value = false;
    };

    // EnableIfImpl

    template <bool cond, typename A = void>
    struct EnableIfImpl
    {
    };

    template <typename A>
    struct EnableIfImpl<true, A>
    {
        typedef A Type;
    };

    // EnableIf

    template <bool cond, typename A = void>
    using EnableIf = typename EnableIfImpl<cond, A>::Type;

    // ConditionalImpl

    template <bool cond, typename T, typename F>
    struct ConditionalImpl
    {
    };

    template <typename T, typename F>
    struct ConditionalImpl<true, T, F>
    {
        using Type = T;
    };

    template <typename T, typename F>
    struct ConditionalImpl<false, T, F>
    {
        using Type = F;
    };

    // Conditionl_t

    template <bool cond, typename T, typename F>
    using Conditional = typename ConditionalImpl<cond, T, F>::Type;

    // size_of_ptr_v
    constexpr auto size_of_ptr_v = sizeof(void *);

    // ConstParam_t
    // Pass by value if the size of type is leq to machine pointer size.

    // template <typename A>
    // using ConstParam_t = Conditional<
    //     sizeof(A) <= size_of_ptr_v,
    //     A,
    //     // const A &>;
    //     const typename std::remove_const<typename std::remove_reference<A>::Type>::Type &>;

    // eq_v

    template <typename A, typename B>
    constexpr bool eq_v(const A &lhs, const B &rhs)
    {
        return lhs == rhs;
    }

    //  neq_v

    template <typename A, typename B>
    constexpr bool neq_v(const A &lhs, const B &rhs)
    {
        return lhs != rhs;
    }

    //  gt_v

    template <typename A, typename B>
    constexpr bool gt_v(const A &lhs, const B &rhs)
    {
        return lhs > rhs;
    }

    //  lt_v

    template <typename A, typename B>
    constexpr bool lt_v(const A &lhs, const B &rhs)
    {
        return lhs < rhs;
    }

    //  geq_v

    template <typename A, typename B>
    constexpr bool geq_v(const A &lhs, const B &rhs)
    {
        return lhs >= rhs;
    }

    //  leq_v

    template <typename A, typename B>
    constexpr bool leq_v(const A &lhs, const B &rhs)
    {
        return lhs <= rhs;
    }

    // and_v

    constexpr bool and_v(const bool lhs, const bool rhs)
    {
        return lhs && rhs;
    }

    // or_v

    constexpr bool or_v(const bool lhs, const bool rhs)
    {
        return lhs || rhs;
    }

    // max_v

    template <typename A>
    constexpr A max_v(const A &lhs, const A &rhs)
    {
        return lhs > rhs ? lhs : rhs;
    }

    // min_v

    template <typename A>
    constexpr A min_v(const A &lhs, const A &rhs)
    {
        return lhs < rhs ? lhs : rhs;
    }

    // plus_v

    template <typename A, typename B>
    constexpr auto plus_v(const A &lhs, const B &rhs)
        -> decltype(lhs + rhs)
    {
        return lhs + rhs;
    }

    // times_v

    template <typename A>
    constexpr A times_v(const A &lhs, const A &rhs)
    {
        return lhs * rhs;
    }

    // bound_v

    template <typename A, typename B, typename C>
    constexpr auto bound_v(const A &lower, const B &upper, const C &x)
        -> decltype((x > upper) ? (upper) : ((x < lower) ? lower : x))
    {
        return (x > upper) ? (upper) : ((x < lower) ? lower : x);
    }

    // template <typename A, typename B, typename C>
    // constexpr typename C::value_type bound_v(const A &lower, const B &upper, const C &x)
    // {
    //     return (x > upper) ? (upper) : ((x < lower) ? lower : x);
    // }

    // FoldlImpl

    template <template <class, class> class F, typename A, typename... Bs>
    struct FoldlImpl
    {
    };

    template <template <class, class> class F, typename A, typename B>
    struct FoldlImpl<F, A, B> : F<A, B>::Type
    {
    };

    template <template <class, class> class F, typename A, typename B0, typename B1, typename... Bs>
    struct FoldlImpl<F, A, B0, B1, Bs...> : FoldlImpl<F, typename F<A, B0>::Type, B1, Bs...>
    {
    };

    // Foldl

    template <template <class, class> class F, typename A, typename... Bs>
    using Foldl = typename FoldlImpl<F, A, Bs...>::Type;

    // * Maybe just recursive constexpr template function could be enough

    // foldl_v

    template <typename F, typename A>
    constexpr A foldl_v(F f, A a)
    {
        return a;
    }

    template <typename F, typename A, typename B>
    constexpr A foldl_v(F f, A a, B b)
    {
        return f(a, b);
    }

    template <typename F, typename A, typename B, typename... Bs>
    constexpr A foldl_v(F f, A a, B b, Bs... bs)
    {
        return foldl_v(f, f(a, b), bs...);
    }

    // all_v

    template <typename... Args>
    constexpr bool all_v(Args... args)
    {
        return foldl_v(and_v, true, args...);
    }

    // any_v

    template <typename... Args>
    constexpr bool any_v(Args... args)
    {
        return foldl_v(or_v, false, args...);
    }

    // maximum_v
    // cf) since the function is defined as foldr, the result follows the type of first argument.

    template <typename A, typename... As>
    constexpr A maximum_v(A a, As... as)
    {
        return foldl_v(max_v<A>, a, as...);
    }

    // minimum_v
    // cf) since the function is defined as foldr, the result follows the type of first argument.

    template <typename A, typename... As>
    constexpr A minimum_v(A a, As... as)
    {
        return foldl_v(min_v<A>, a, as...);
    }

    // sum_v

    template <typename A, typename... As>
    constexpr A sum_v(A a, As... as)
    {
        return foldl_v(plus_v<A, A>, a, as...);
    }

    // product_v

    template <typename A, typename... As>
    constexpr A product_v(A a, As... as)
    {
        return foldl_v(times_v<A>, a, as...);
    }

    // RemoveReference

    // ! Deprecated

    // template <typename A>
    // using ReferenceRemoved = typename std::remove_reference<A>::Type;

    // CommonImpl

    // ! Deprecated

    // template <typename... Args>
    // using Common = typename std::common_type<Args...>::Type;

    // IsIntegralConstant

    // template <typename A>
    // struct IsIntegralConstant : std::false_type
    // {
    // };

    // template <typename A, A Value>
    // struct IsIntegralConstant<std::integral_constant<A, Value>> : std::true_type
    // {
    // };

    // IntegralConst

    // ! Issue on (int, IntegralConst, -1);

    template <typename A, A a>
    struct IntegralConst
    {
        static constexpr A value = a;
        using value_type = A;
        using Type = IntegralConst;

        constexpr operator value_type() const noexcept { return value; }   // Conversion operator
        constexpr value_type operator()() const noexcept { return value; } // Function call operator
    };

    // IsIntegralConst

    template <typename A>
    struct IsIntegralConst
    {
        static constexpr bool value = false;
    };

    template <typename A, A a>
    struct IsIntegralConst<IntegralConst<A, a>>
    {
        static constexpr bool value = true;
    };

    template <typename A>
    struct IsIntegralConst<A &> : IsIntegralConst<A>
    {
    };

    template <typename A>
    struct IsIntegralConst<A &&> : IsIntegralConst<A>
    {
    };

    // IsSame

    template <typename A, typename B>
    struct IsSame
    {
        static constexpr bool value = false;
    };

    template <typename A>
    struct IsSame<A, A>
    {
        static constexpr bool value = true;
    };

    // PackAtImpl

    template <uint8_t n, typename... Args>
    struct PackAtImpl
    {
        // static_assert(n >= 0, "Index out of range");
        // static_assert(n < sizeof...(Args), "Index out of range");
        using Type = void *;
    };

    template <typename Head, typename... Tail>
    struct PackAtImpl<0, Head, Tail...>
    {
        using Type = Head;
    };

    template <uint8_t n, typename Head, typename... Tail>
    struct PackAtImpl<n, Head, Tail...>
        : PackAtImpl<n - 1, Tail...>
    {
    };

    // PackAt

    template <uint8_t n, typename... Args>
    using PackAt = typename PackAtImpl<n, Args...>::Type;

    // FindHelperValue
    template <uint8_t n>
    struct FindHelperValue
    {
        static constexpr uint8_t value = n;
    };

    // FindHelper

    template <size_t n, template <class> class P, typename... Args>
    struct FindHelper
    {
    };

    template <size_t n, template <class> class P, typename Head, typename... Tail>
    struct FindHelper<n, P, Head, Tail...>
        : Conditional<
              P<Head>::value,
              FindHelperValue<n>,
              FindHelper<n + 1, P, Tail...>>
    {
    };

    // Find

    template <template <class> class P, typename... Args>
    struct Find : FindHelper<0, P, Args...>
    {
    };

    // CallReturnImpl;

    template <typename, typename...>
    struct CallReturnImpl;

    template <typename F, typename... Args>
    struct CallReturnImpl
    {
        using Type = decltype(std::declval<F>()(std::declval<Args>()...));
    };

    template <typename F, typename... Args>
    using CallReturn = typename CallReturnImpl<F, Args...>::Type;

    // IsCallOperator

    template <typename A>
    class IsCallOperator
    {
        typedef char one;
        typedef long two;

        template <typename B>
        static one test(decltype(&B::operator()));

        template <typename B>
        static two test(...);

    public:
        static const bool value = sizeof(test<A>(0)) == sizeof(one);
    };

    // ArgumentsImpl

    template <typename, bool>
    struct ArgumentsImpl
    {
    };

    template <typename F>
    struct ArgumentsImpl<F, true> : ArgumentsImpl<decltype(&F::operator()), false>
    {
        // using Type = typename
    };

    template <typename R, typename... Args>
    struct ArgumentsImpl<R (*)(Args...), false>
    {
        using Type = std::tuple<Args...>;
    };

    template <typename R, typename A, typename... Args>
    struct ArgumentsImpl<R (A::*)(Args...), false>
    {
        using Type = std::tuple<Args...>;
    };

    template <typename R, typename A, typename... Args>
    struct ArgumentsImpl<R (A::*)(Args...) const, false>
    {
        using Type = std::tuple<Args...>;
    };

    // Arguement_t
    // l-value and r-value reference will preserved at the result, but const will be removed.

    template <typename F>
    using Arguments = typename ArgumentsImpl<F, IsCallOperator<F>::value>::Type;

    // ReturnImpl

    template <typename, typename>
    struct ReturnImpl
    {
    };

    template <typename F, typename... Args>
    struct ReturnImpl<F, std::tuple<Args...>>
    {
        using Type = CallReturn<F, Args...>;
    };

    // Return

    template <typename F>
    using Return = typename ReturnImpl<F, Arguments<F>>::Type;

    // IsInvocable

    template <typename F, typename... Args>
    struct IsInvocable
    {
    private:
        template <typename A>
        static auto check(int) -> decltype(std::declval<A>()(std::declval<Args>()...), True());

        template <typename>
        static auto check(...) -> False;

    public:
        static constexpr bool value = decltype(check<F>(0))::value;
    };

    // ReferenceRemovedImpl

    template <typename A>
    struct ReferenceRemovedImpl
    {
        using Type = A;
    };

    template <typename A>
    struct ReferenceRemovedImpl<A &>
    {
        using Type = A;
    };

    template <typename A>
    struct ReferenceRemovedImpl<A &&>
    {
        using Type = A;
    };

    // ReferenceRemoved

    template <typename A>
    using ReferenceRemoved = typename ReferenceRemovedImpl<A>::Type;

    // Commontype

    template <typename... As>
    struct CommonImpl
    {
        using Type = void;
    };

    template <typename A, typename... As>
    struct CommonImpl<A, As...>
    {
        using Type = EnableIf<
            all_v(IsSame<A, As>::value...), A>;
    };

    // Common

    template <typename... As>
    using Common = typename CommonImpl<As...>::Type;

    // template <typename Head, typename... Tail>
    // using Common = EnableIf<
    //     all_v(IsSame<Head, Tail>::value...), Head>;

    // IsConst

    template <typename A>
    struct IsConst : False
    {
    };

    template <typename A>
    struct IsConst<const A> : True
    {
    };

    // Void

    template <typename... Ts>
    struct VoidImpl
    {
        typedef void type;
    };

    template <typename... Ts>
    using Void = typename VoidImpl<Ts...>::Type;
}

#endif