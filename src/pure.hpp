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

template <typename F, typename It_A>
void for_each(F f, It_A &iterable)
{
    auto length = std::end(iterable) - std::begin(iterable);

    for (int i = 0; i < length; i++)
    {
        f(iterable[i]);
    }
}

// ! experimental

template <typename A, size_t N>
size_t iterable_length(A (&)[N])
{
    return N;
}

template <typename A, size_t N>
size_t iterable_length(std::array<A, N> &)
{
    return N;
}

template <typename A>
size_t iterable_length(std::vector<A> &vector)
{
    return vector.size();
}

template <typename A, size_t N, typename... Args>
size_t iterable_length(A (&head)[N], Args &...tail)
{
    size_t n_t = iterable_length(tail...);
    return N < n_t ? N : n_t;
}

template <typename A, size_t N, typename... Args>
size_t iterable_length(std::array<A, N> &head, Args &...tail)
{
    size_t n_t = iterable_length(tail...);
    return N < n_t ? N : n_t;
}

template <typename A, typename... Args>
size_t iterable_length(std::vector<A> &head, Args &...tail)
{
    size_t n_h = head.size();
    size_t n_t = iterable_length(tail...);
    return n_h < n_t ? n_h : n_t;
}

template <typename F, typename... Args>
void for_each(F f, Args &...args)
{
    size_t length = iterable_length(args...);

    for (int i = 0; i < length; i++)
    {
        f(args[i]...);
    }

    return;
}

// unary fmap

// template <typename T>
// using element_type_t = typename std::remove_reference<decltype(*std::begin(std::declval<T &>()))>::type;

// template <typename, typename>
// struct fmap_type_t_;

// template <typename A, size_t N, typename B>
// struct fmap_type_t_<A[N], B>
// {
//     using type = std::array<B, N>;
// };

// template <typename A, size_t N, typename B>
// struct fmap_type_t_<std::array<A, N>, B>
// {
//     using type = std::array<B, N>;
// };

// template <typename A, typename B>
// struct fmap_type_t_<std::vector<A>, B>
// {
//     using type = std::vector<B>;
// };

// template <typename T, typename B>
// struct fmap_type_t_<T &, B> : fmap_type_t_<T, B>
// {
// };

// template <typename T, typename B>
// struct fmap_type_t_<T &&, B> : fmap_type_t_<T, B>
// {
// };

// template <typename T, typename B>
// using fmap_type_t = typename fmap_type_t_<T, B>::type;

// template <typename F, typename It_A>
// // A = element_type_t<It_A>;
// // B = decltype(std::declval<F>()(std::declval<A>()));
// // R = fmap_type_t<It_A, B>;
// auto fmap(F f, It_A &iterable)
//     -> fmap_type_t<It_A, decltype(std::declval<F>()(std::declval<element_type_t<It_A>>()))>
// {
//     auto length = std::end(iterable) - std::begin(iterable);

//     fmap_type_t<It_A, decltype(std::declval<F>()(std::declval<element_type_t<It_A>>()))> result;

//     for (int i = 0; i < length; i++)
//     {
//         result[i] = f(iterable[i]);
//     }

//     return result;
// }

// template <typename F, typename A>
// auto fmap(F f, std::vector<A> &iterable)
//     -> std::vector<decltype(std::declval<F>()(std::declval<A>()))>
// {
//     auto length = std::end(iterable) - std::begin(iterable);

//     std::vector<decltype(std::declval<F>()(std::declval<A>()))> result(length);

//     for (int i = 0; i < length; i++)
//     {
//         result[i] = f(iterable[i]);
//     }

//     return result;
// }

// ! Experimental

// Helper function to check if a type is std::array

template <typename T>
struct isArray : std::false_type
{
};

template <typename T, std::size_t N>
struct isArray<T[N]> : std::true_type
{
};

template <typename T, std::size_t N>
struct isArray<std::array<T, N>> : std::true_type
{
};

template <typename T>
struct isArray<T &> : isArray<T>
{
};

template <typename T>
struct isArray<T &&> : isArray<T>
{
};

// Helper function to check if all types are C-style arrays or std::array

template <typename... Args>
struct areAllArrays;

template <typename T>
struct areAllArrays<T> : std::false_type
{
};

template <typename T, std::size_t N>
struct areAllArrays<T[N]> : std::true_type
{
};

template <typename T, std::size_t N>
struct areAllArrays<std::array<T, N>> : std::true_type
{
};

template <typename T>
struct areAllArrays<T &> : areAllArrays<T>
{
};

template <typename T>
struct areAllArrays<T &&> : areAllArrays<T>
{
};

template <typename Head, typename... Tail>
struct areAllArrays<Head, Tail...>
{
    static constexpr bool value = isArray<Head>::value && areAllArrays<Tail...>::value;
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

// min_length

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

template <typename... Args>
constexpr size_t min_length(Args &...args)
{
    return sizeof...(Args) == 0 ? 0 : min_value(get_length(args)...);
}

// initilize_result

template <typename R, typename... Args>
auto initialize_result(Args &...args) -> typename std::conditional<areAllArrays<Args...>::value,
                                                                   std::array<R, constexpr min_length(args...)>,
                                                                   std::vector<R>>::type
{
    if constexpr (areAllArrays<Args...>::value)
    {
        return std::array<R, constexpr min_length(args...)>;
    }
    else
    {
        return std::vector<R>(min_length(args...));
    }
}

// fmapReturnType

template <typename>
struct GetElementType;

template <typename A, std::size_t N>
struct GetElementType<A[N]>
{
    using type = A;
};

template <typename A, std::size_t N>
struct GetElementType<std::array<A, N>>
{
    using type = A;
};

template <typename A>
struct GetElementType<std::vector<A>>
{
    using type = A;
};

// Convenience type alias
template <typename A>
using ElementType_t = typename GetElementType<A>::type;

template <typename F, typename... Args>
struct LambdaReturnType<F, Args...>
{
    using type = decltype(std::declval<F>()(std::declval<Args>()...));
};

template <typename F, typename... Args>
using LambdaReturnType_t = typename LambdaReturnType<F, Args...>::type;

template <typename F, typename... Args>
auto fmap(F f, Args &...args) -> typename std::conditional<areAllArrays<Args...>::value,
                                                           std::array<LambdaReturnType_t<F, Args...>, min_length(args...)>,
                                                           std::vector<LambdaReturnType_t<F, Args...>>>::type
{
    using R = LambdaReturnType_t<F, Args...>;

    auto result = initialize_result<R, Args...>(args...);

    for (int i = 0; i < length; i++)
    {
        result[i] = f(args[i]...);
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
using riters_t = std::tuple<std::reverse_iterator<ElementType_t<It> *>, std::reverse_iterator<ElementType_t<It> *>>;

template <typename It>
riters_t<It> make_riters(It &iterable);

template <typename A, size_t N>
riters_t<A[N]> make_riters(A (&iterable)[N])
{
    auto length = std::end(iterable) - std::begin(iterable);

    auto rbegin = std::reverse_iterator<A *>(iterable + length);
    auto rend = std::reverse_iterator<A *>(iterable);

    return std::make_tuple(rbegin, rend);
}

template <typename A, size_t N>
riters_t<std::array<A, N>> make_riters(std::array<A, N> &iterable)
{
    return std::make_tuple(
        std::reverse_iterator<A *>(iterable.end()),
        std::reverse_iterator<A *>(iterable.begin()));
}

template <typename F, typename R, typename It>
R foldr(F f, R initial_value, It &iterable)
{
    riters_t<It> riters = make_riters(iterable);

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

// ! temp for variadic templates

#include <iostream>

template <typename T>
void print(T arg)
{
    std::cout << arg << std::endl;
}

template <typename T, typename... Types>
void print(T arg, Types... args)
{
    std::cout << arg << ", ";
    print(args...);
}
