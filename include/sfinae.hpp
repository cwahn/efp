#ifndef SFINAE_HPP_
#define SFINAE_HPP_

#include "limits.hpp"

namespace efp
{
    // EnableIfType

    template <bool cond, typename A = void>
    struct EnableIfType
    {
    };

    template <typename A>
    struct EnableIfType<true, A>
    {
        typedef A type;
    };

    // EnableIf_t

    template <bool cond, typename A = void>
    using EnableIf_t = typename EnableIfType<cond, A>::type;

    // ConditionalType

    template <bool cond, typename T, typename F>
    struct ConditionalType
    {
    };

    template <typename T, typename F>
    struct ConditionalType<true, T, F>
    {
        using type = T;
    };

    template <typename T, typename F>
    struct ConditionalType<false, T, F>
    {
        using type = F;
    };

    // Conditionl_t

    template <bool cond, typename T, typename F>
    using Conditional_t = typename ConditionalType<cond, T, F>::type;

    // eq_v

    template <typename A, typename B>
    constexpr bool eq_v(const A lhs, const B rhs)
    {
        return lhs == rhs;
    }

    //  neq_v

    template <typename A, typename B>
    constexpr bool neq_v(const A lhs, const B rhs)
    {
        return lhs != rhs;
    }

    //  gt_v

    template <typename A, typename B>
    constexpr bool gt_v(const A lhs, const B rhs)
    {
        return lhs > rhs;
    }

    //  lt_v

    template <typename A, typename B>
    constexpr bool lt_v(const A lhs, const B rhs)
    {
        return lhs < rhs;
    }

    //  geq_v

    template <typename A, typename B>
    constexpr bool geq_v(const A lhs, const B rhs)
    {
        return lhs >= rhs;
    }

    //  leq_v

    template <typename A, typename B>
    constexpr bool leq_v(const A lhs, const B rhs)
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
    constexpr A max_v(const A lhs, const A rhs)
    {
        return lhs > rhs ? lhs : rhs;
    }

    // min_v

    template <typename A>
    constexpr A min_v(const A lhs, const A rhs)
    {
        return lhs < rhs ? lhs : rhs;
    }

    // Foldl

    template <template <class, class> class F, typename A, typename... Bs>
    struct Foldl
    {
    };

    template <template <class, class> class F, typename A, typename B>
    struct Foldl<F, A, B> : F<A, B>::type
    {
    };

    template <template <class, class> class F, typename A, typename B0, typename B1, typename... Bs>
    struct Foldl<F, A, B0, B1, Bs...> : Foldl<F, typename F<A, B0>::type, B1, Bs...>
    {
    };

    // Foldl_t

    template <template <class, class> class F, typename A, typename... Bs>
    using Foldl_t = typename Foldl<F, A, Bs...>::type;

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

    // RemoveReference

    template <typename A>
    using RemoveReference_t = typename std::remove_reference<A>::type;

    // CommonType

    template <typename... Args>
    using Common_t = typename std::common_type<Args...>::type;

    // IsIntegralConstant

    template <typename T>
    struct IsIntegralConstant : std::false_type
    {
    };

    template <typename T, T Value>
    struct IsIntegralConstant<std::integral_constant<T, Value>> : std::true_type
    {
    };

    // CallReturnType;

    template <typename, typename...>
    struct CallReturnType;

    template <typename F, typename... Args>
    struct CallReturnType
    {
        using type = decltype(std::declval<F>()(std::declval<Args>()...));
    };

    template <typename F, typename... Args>
    using CallReturn_t = typename CallReturnType<F, Args...>::type;

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

    // ArgumentHelper

    template <typename, bool>
    struct ArgumentHelper
    {
    };

    template <typename F>
    struct ArgumentHelper<F, true> : ArgumentHelper<decltype(&F::operator()), false>
    {
        // using type = typename
    };

    template <typename R, typename... Args>
    struct ArgumentHelper<R (*)(Args...), false>
    {
        using type = std::tuple<Args...>;
    };

    template <typename R, typename A, typename... Args>
    struct ArgumentHelper<R (A::*)(Args...), false>
    {
        using type = std::tuple<Args...>;
    };

    template <typename R, typename A, typename... Args>
    struct ArgumentHelper<R (A::*)(Args...) const, false>
    {
        using type = std::tuple<Args...>;
    };

    // Arguement_t

    template <typename F>
    using Argument_t = typename ArgumentHelper<F, IsCallOperator<F>::value>::type;

    // ReturnTypeHelper

    template <typename, typename>
    struct ReturnTypeHelper
    {
    };

    template <typename F, typename... Args>
    struct ReturnTypeHelper<F, std::tuple<Args...>>
    {
        using type = CallReturn_t<F, Args...>;
    };

    // Return_t

    template <typename F>
    using Return_t = typename ReturnTypeHelper<F, Argument_t<F>>::type;

}

#endif