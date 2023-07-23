#ifndef SFINAE_HPP_
#define SFINAE_HPP_

namespace efp
{
    // All

    template <typename... Args>
    struct All;

    template <>
    struct All<>
    {
        static constexpr bool value = true;
    };

    template <typename Head, typename... Tail>
    struct All<Head, Tail...>
    {
        static constexpr bool value = Head::value && All<Tail...>::value;
    };

    // Any

    template <typename... Args>
    struct Any;

    template <>
    struct Any<>
    {
        static constexpr bool value = false;
    };

    template <typename Head, typename... Tail>
    struct Any<Head, Tail...>
    {
        static constexpr bool value = Head::value || Any<Tail...>::value;
    };

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