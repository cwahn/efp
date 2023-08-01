#ifndef SFINAE_HPP_
#define SFINAE_HPP_

#include "limits.hpp"

// Make it value as soon as possible and use constexpr function and avoid type_trait.

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

    // size_of_ptr_v
    constexpr auto size_of_ptr_v = sizeof(void *);

    // ConstParam_t
    // Pass by value if the size of type is leq to machine pointer size.

    template <typename A>
    using ConstParam_t = Conditional_t<
        sizeof(A) <= size_of_ptr_v,
        A,
        // const A &>;
        const typename std::remove_const<typename std::remove_reference<A>::type>::type &>;

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

    template <typename A>
    using RemoveReference_t = typename std::remove_reference<A>::type;

    // CommonType

    template <typename... Args>
    using Common_t = typename std::common_type<Args...>::type;

    // IsIntegralConstant

    template <typename A>
    struct IsIntegralConstant : std::false_type
    {
    };

    template <typename A, A Value>
    struct IsIntegralConstant<std::integral_constant<A, Value>> : std::true_type
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

    // ArgumentType

    template <typename, bool>
    struct ArgumentType
    {
    };

    template <typename F>
    struct ArgumentType<F, true> : ArgumentType<decltype(&F::operator()), false>
    {
        // using type = typename
    };

    template <typename R, typename... Args>
    struct ArgumentType<R (*)(Args...), false>
    {
        using type = std::tuple<Args...>;
    };

    template <typename R, typename A, typename... Args>
    struct ArgumentType<R (A::*)(Args...), false>
    {
        using type = std::tuple<Args...>;
    };

    template <typename R, typename A, typename... Args>
    struct ArgumentType<R (A::*)(Args...) const, false>
    {
        using type = std::tuple<Args...>;
    };

    // Arguement_t

    template <typename F>
    using Argument_t = typename ArgumentType<F, IsCallOperator<F>::value>::type;

    // ReturnType

    template <typename, typename>
    struct ReturnType
    {
    };

    template <typename F, typename... Args>
    struct ReturnType<F, std::tuple<Args...>>
    {
        using type = CallReturn_t<F, Args...>;
    };

    // Return_t

    template <typename F>
    using Return_t = typename ReturnType<F, Argument_t<F>>::type;

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

    // PackAtType

    template <uint8_t n, typename Head, typename... Tail>
    struct PackAtType
        : PackAtType<n - 1, Tail...>
    {
    };

    template <typename Head, typename... Tail>
    struct PackAtType<0, Head, Tail...>
    {
        using type = Head;
    };

    // PackAt_t

    template <uint8_t n, typename... Args>
    using PackAt_t = typename PackAtType<n, Args...>::type;

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
        : Conditional_t<
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



}

#endif