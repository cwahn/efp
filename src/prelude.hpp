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
    static constexpr bool value = Head::value || All<Tail...>::value;
};

// IsArray

template <typename T>
struct IsArray : std::false_type
{
};

template <typename T, std::size_t N>
struct IsArray<T[N]> : std::true_type
{
};

template <typename T, std::size_t N>
struct IsArray<std::array<T, N>> : std::true_type
{
};

template <typename T>
struct IsArray<T &> : IsArray<T>
{
};

template <typename T>
struct IsArray<T &&> : IsArray<T>
{
};

// AreAllArrays

template <typename... Args>
struct AreAllArrays
{
    static constexpr bool value = All<IsArray<Args>...>::value;
};

// ArrayLength

template <typename T>
struct ArrayLength
{
    static constexpr size_t value = 0;
};

template <typename T, size_t N>
struct ArrayLength<T[N]>
{
    static constexpr size_t value = N;
};

template <typename T, size_t N>
struct ArrayLength<std::array<T, N>>
{
    static constexpr size_t value = N;
};

template <typename T>
struct ArrayLength<T &> : ArrayLength<T>
{
};

template <typename T>
struct ArrayLength<T &&> : ArrayLength<T>
{
};

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

// MinArrayLength

template <typename... Args>
struct MinArrayLength;

template <typename Arg>
struct MinArrayLength<Arg>
{
    static constexpr size_t value = ArrayLength<Arg>::value;
};

template <typename Head, typename... Tail>
struct MinArrayLength<Head, Tail...>
{
    static constexpr size_t head = ArrayLength<Head>::value;
    static constexpr size_t tail = min_value(ArrayLength<Tail>::value...);
    static constexpr size_t value = head < tail ? head : tail;
};

// get_length

template <typename A, size_t N>
constexpr size_t get_length(A (&)[N])
{
    return N;
}

template <typename A, size_t N>
constexpr size_t get_length(std::array<A, N> &)
{
    return N;
}

template <typename A>
size_t get_length(std::vector<A> &vector)
{
    return vector.size();
}

// min_length

template <typename... Args>
constexpr size_t min_length(Args &...args)
{
    return min_value(get_length(args)...);
}

template <typename F, typename... Args>
void for_each(F f, Args &&...args)
{
    if constexpr (AreAllArrays<Args...>::value)
    {
        constexpr size_t length = min_value(ArrayLength<Args>::value...);
        for (int i = 0; i < length; ++i)
        {
            f(args[i]...);
        }
    }
    else
    {
        size_t length = min_length(args...);

        for (int i = 0; i < length; ++i)
        {
            f(args[i]...);
        }
    }
}

// ReturnIterable_t
template <typename A, typename... Args>
using ReturnIterable_t = typename std::conditional<AreAllArrays<Args...>::value,
                                                   std::array<A, MinArrayLength<Args...>::value>,
                                                   std::vector<A>>::type;

// initilize_result

template <typename R, typename... Args>
auto initialize_result(Args &...args) -> ReturnIterable_t<R, Args...>
{
    if constexpr (AreAllArrays<Args...>::value)
    {
        return std::array<R, MinArrayLength<Args...>::value>();
    }
    else
    {
        return std::vector<R>(min_length(args...));
    }
}

// ElementType

template <typename>
struct ElementType;

template <typename A, std::size_t N>
struct ElementType<A[N]>
{
    using type = A;
};

template <typename A, std::size_t N>
struct ElementType<std::array<A, N>>
{
    using type = A;
};

template <typename A>
struct ElementType<std::vector<A>>
{
    using type = A;
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
using FmapReturnType_t = ReturnIterable_t<FunctionReturnType_t<F, ElementType_t<Args>...>, Args...>;

template <typename F, typename... Args>
auto fmap(F f, Args &...args) -> FmapReturnType_t<F, Args...>
{
    using R = FunctionReturnType_t<F, ElementType_t<Args>...>;

    FmapReturnType_t<F, Args...> result = initialize_result<R, Args...>(args...);

    if constexpr (AreAllArrays<Args...>::value)
    {
        for (int i = 0; i < MinArrayLength<Args...>::value; ++i)
        {
            result[i] = f(args[i]...);
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

template <typename F, typename It>
auto filter(F f, It &iterable) -> std::vector<ElementType_t<It>>
{
    std::vector<ElementType_t<It>> result;

    std::copy_if(
        std::begin(iterable),
        std::end(iterable),
        std::back_inserter(result),
        f);

    return result;
}

template <typename F, typename R, typename It>
R foldl(F f, R initial_value, It &iterable)
{
    return std::accumulate(std::begin(iterable), std::end(iterable), initial_value, f);
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
    return std::accumulate(
        iterable.rbegin(),
        iterable.rend(),
        initial_value,
        f);
}

#endif