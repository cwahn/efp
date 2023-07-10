#ifndef PRELUDE_HPP_
#define PRELUDE_HPP_

#include <array>
#include <vector>
#include <iterator>
#include <functional>
#include <algorithm>
#include <iterator>
#include <utility>
#include <type_traits>
#include <numeric>
#include <tuple>

#include "zero_copy.hpp"

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

template <typename... Args>
constexpr bool All_v = All<Args...>::value;

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

template <typename... Args>
constexpr bool Any_v = Any<Args...>::value;

// RemoveReference
template <typename A>
using RemoveReference_t = typename std::remove_reference<A>::type;

// CommonType
template <typename... Args>
using CommonType_t = typename std::common_type<Args...>::type;

// ? What if not either Static and Dynamic?
// IsStatic

template <typename T>
struct IsStatic : std::false_type
{
};

template <typename T, size_t N>
struct IsStatic<T[N]> : std::true_type
{
};

template <typename T, size_t N>
struct IsStatic<std::array<T, N>> : std::true_type
{
};

template <typename T, size_t N>
struct IsStatic<StaticArray<T, N>> : std::true_type
{
};

template <typename T, size_t N>
struct IsStatic<StaticVector<T, N>> : std::true_type
{
};

template <typename T>
struct IsStatic<T &> : IsStatic<T>
{
};

template <typename T>
struct IsStatic<T &&> : IsStatic<T>
{
};

template <typename T>
constexpr bool IsStatic_v = IsStatic<T>::value;

// AreAllStatic

template <typename... Args>
struct AreAllStatic
{
    static constexpr bool value = All_v<IsStatic<Args>...>;
};

template <typename... Args>
constexpr bool AreAllStatic_v = AreAllStatic<Args...>::value;

// IsStaticVector
template <typename T>
struct IsStaticVector : std::false_type
{
};

template <typename T, size_t N>
struct IsStaticVector<StaticVector<T, N>> : std::true_type
{
};

template <typename T>
struct IsStaticVector<T &> : IsStaticVector<T>
{
};

template <typename T>
struct IsStaticVector<T &&> : IsStaticVector<T>
{
};

template <typename T>
constexpr bool IsStaticVector_v = IsStaticVector<T>::value;

// StaticCapacity

template <typename T>
struct StaticCapacity
{
    static constexpr size_t value = 0;
};

template <typename T, size_t N>
struct StaticCapacity<T[N]>
{
    static constexpr size_t value = N;
};

template <typename T, size_t N>
struct StaticCapacity<std::array<T, N>>
{
    static constexpr size_t value = N;
};

template <typename T, size_t N>
struct StaticCapacity<StaticArray<T, N>>
{
    static constexpr size_t value = N;
};

template <typename T, size_t N>
struct StaticCapacity<StaticVector<T, N>>
{
    static constexpr size_t value = N;
};

template <typename T>
struct StaticCapacity<T &> : StaticCapacity<T>
{
};

template <typename T>
struct StaticCapacity<T &&> : StaticCapacity<T>
{
};

template <typename T>
constexpr size_t StaticCapacity_v = StaticCapacity<T>::value;

// min_value

template <typename A>
constexpr A min_value(const A &value)
{
    return value;
}

template <typename Head, typename... Tail>
constexpr Head min_value(const Head &head, const Tail &...tail)
{
    return head < min_value(tail...) ? head : min_value(tail...);
}

// MinStaticCapacity

template <typename... Args>
struct MinStaticCapacity;

template <typename Arg>
struct MinStaticCapacity<Arg>
{
    static constexpr size_t value = StaticCapacity_v<Arg>;
};

template <typename Head, typename... Tail>
struct MinStaticCapacity<Head, Tail...>
{
    static constexpr size_t head = StaticCapacity_v<Head>;
    static constexpr size_t tail = min_value(StaticCapacity_v<Tail>...);
    static constexpr size_t value = head < tail ? head : tail;
};

template <typename... Args>
constexpr size_t MinStaticCapacity_v = MinStaticCapacity<Args...>::value;

// get_length

template <typename A>
size_t get_length(const A &iterable)
{
    return iterable.size();
}

template <typename A, size_t N>
constexpr size_t get_length(const A (&)[N])
{
    return N;
}

template <typename A, size_t N>
constexpr size_t get_length(A(&&)[N])
{
    return N;
}

// min_length

template <typename... Args>
constexpr size_t min_length(Args &...args)
{
    // return min_value(get_length(args)...);
    return min_value(get_length(args)...);
}

template <typename F, typename... Args>
void for_each(F f, Args &&...args)
{
    size_t length = min_value(get_length(args)...);

    for (int i = 0; i < length; ++i)
    {
        f(args[i]...);
    }
}

template <typename A, typename... Args>
using FmapIterable_t = typename std::conditional<All_v<IsStatic<Args>...>,
                                                 typename std::conditional<Any_v<IsStaticVector<Args>...>,
                                                                           StaticVector<A, MinStaticCapacity_v<Args...>>,
                                                                           StaticArray<A, MinStaticCapacity_v<Args...>>>::type,
                                                 DynamicVector<A>>::type;

// initilize_result

template <typename R, typename... Args>
FmapIterable_t<R, Args...> fmap_iterable(Args &...args) // Internal data could be unpredictable
{
    if constexpr (All_v<IsStatic<Args>...>)
    {
        if constexpr (Any_v<IsStaticVector<Args>...>)
        {
            return StaticVector<R, MinStaticCapacity_v<Args...>>(min_length(args...));
        }
        else
        {
            return StaticArray<R, MinStaticCapacity_v<Args...>>();
        }
    }
    else
    {
        return DynamicVector<R>(min_length(args...));
    }
}

// ElementType

template <typename A>
struct ElementType
{
    using type = typename A::value_type;
};

template <typename A, size_t N>
struct ElementType<A[N]>
{
    using type = A;
};

template <typename A>
struct ElementType<A &> : ElementType<A>
{
};

template <typename A>
struct ElementType<A &&> : ElementType<A>
{
};

template <typename A>
using ElementType_t = typename ElementType<A>::type;

// FunctionReturnType;

template <typename, typename...>
struct FunctionReturnType;

template <typename F, typename... Args>
struct FunctionReturnType
{
    using type = decltype(std::declval<F>()(std::declval<Args>()...));
};

template <typename F, typename... Args>
using FunctionReturnType_t = typename FunctionReturnType<F, Args...>::type;

// FmapReturnType_t

template <typename F, typename... Args>
using FmapReturnType_t = FmapIterable_t<FunctionReturnType_t<F, ElementType_t<Args>...>, Args...>;

template <typename F, typename... Args>
auto fmap(F f, Args &&...args) -> FmapReturnType_t<F, Args...>
{
    using R = FunctionReturnType_t<F, ElementType_t<Args>...>;

    auto result = fmap_iterable<R, Args...>(args...);

    if constexpr (All_v<IsStatic<Args>...>)
    {
        if constexpr (Any_v<IsStaticVector<Args>...>)
        {
            for (int i = 0; i < min_length(args...); ++i)
            {
                result[i] = f(args[i]...);
            }
        }
        else
        {
            for (int i = 0; i < MinStaticCapacity_v<Args...>; ++i)
            {
                result[i] = f(args[i]...);
            }
        }
    }
    else
    {
        for (int i = 0; i < min_length(args...); ++i)
        {
            result[i] = f(args[i]...);
        }
    }

    return result;
}

template <typename A, typename Arg>
using FilterIterable_t = typename std::conditional<IsStatic_v<Arg>,
                                                   StaticVector<A, StaticCapacity_v<Arg>>,
                                                   DynamicVector<A>>::type;

template <typename R, typename Arg>
auto filter_iterable(Arg &arg) -> FilterIterable_t<R, Arg> // Internal data could be unpredictable, with size 0;
{
    if constexpr (IsStatic_v<Arg>)
    {
        return StaticVector<R, StaticCapacity_v<Arg>>();
    }
    else
    {
        auto result = DynamicVector<R>();
        result.reserve(get_length(arg) * 2);
        return result;
    }
}

template <typename F, typename Arg>
auto filter(F f, Arg &arg) -> FilterIterable_t<ElementType_t<Arg>, Arg>
{
    auto result = filter_iterable<ElementType_t<Arg>, Arg>(arg);

    for (int i = 0; i < get_length(arg); ++i)
    {
        if (f(arg[i]))
        {
            result.push_back(arg[i]);
        }
    }

    return result;
}

template <typename F, typename R, typename It>
R foldl(F f, R initial_value, It &iterable)
{
    R result = initial_value;

    for (int i = 0; i < get_length(iterable); ++i)
    {
        result = f(result, iterable[i]);
    }

    return result;
}

template <typename It>
using RIters_t = std::tuple<std::reverse_iterator<ElementType_t<It> *>, std::reverse_iterator<ElementType_t<It> *>>;

template <typename It>
RIters_t<It> make_riters(It &iterable);

template <typename A, size_t N>
RIters_t<A[N]> make_riters(A (&iterable)[N])
{
    auto length = std::end(iterable) - std::begin(iterable);

    auto rbegin = std::reverse_iterator<A *>(iterable + length);
    auto rend = std::reverse_iterator<A *>(iterable);

    return std::make_tuple(rbegin, rend);
}

template <typename A, size_t N>
RIters_t<std::array<A, N>> make_riters(std::array<A, N> &iterable)
{
    return std::make_tuple(
        std::reverse_iterator<A *>(iterable.end()),
        std::reverse_iterator<A *>(iterable.begin()));
}

template <typename F, typename R, typename It>
R foldr(F f, R initial_value, It &iterable)
{
    RIters_t<It> riters = make_riters(iterable);

    return std::accumulate(
        std::get<0>(riters),
        std::get<1>(riters),
        initial_value,
        f);
}

template <typename F, typename R, typename A>
R foldr(F f, R initial_value, std::vector<A> &iterable)
{
    R result = initial_value;

    for (int i = get_length(iterable) - 1; i > -1; --i)
    {
        result = f(iterable[i], result);
    }

    return result;
}

#endif