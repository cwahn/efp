#ifndef PRELUDE_HPP_
#define PRELUDE_HPP_

#include <array>
#include <vector>
#include <iterator>
#include <functional>
#include <algorithm>
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

// ? What if not either Static and Dynamic?
// IsStaticCapacity

template <typename SeqA>
struct IsStaticCapacity : std::false_type
{
};

template <typename A, size_t N>
struct IsStaticCapacity<StaticArray<A, N>> : std::true_type
{
};

template <typename A, size_t N>
struct IsStaticCapacity<StaticVector<A, N>> : std::true_type
{
};

template <typename A, size_t N>
struct IsStaticCapacity<std::array<A, N>> : std::true_type
{
};

template <typename A, size_t N>
struct IsStaticCapacity<A[N]> : std::true_type
{
};

template <typename T>
struct IsStaticCapacity<T &> : IsStaticCapacity<T>
{
};

template <typename T>
struct IsStaticCapacity<T &&> : IsStaticCapacity<T>
{
};

// AreAllStaticCapacity

template <typename... Seqs>
struct AreAllStaticCapacity
{
    static constexpr bool value = All<IsStaticCapacity<Seqs>...>::value;
};

// StaticCapacity

template <typename A>
struct StaticCapacity
{
    static constexpr size_t value = 0;
};

template <typename A, size_t N>
struct StaticCapacity<A[N]>
{
    static constexpr size_t value = N;
};

template <typename A, size_t N>
struct StaticCapacity<std::array<A, N>>
{
    static constexpr size_t value = N;
};

template <typename A, size_t N>
struct StaticCapacity<StaticArray<A, N>>
{
    static constexpr size_t value = N;
};

template <typename A, size_t N>
struct StaticCapacity<StaticVector<A, N>>
{
    static constexpr size_t value = N;
};

template <typename A>
struct StaticCapacity<A &> : StaticCapacity<A>
{
};

template <typename A>
struct StaticCapacity<A &&> : StaticCapacity<A>
{
};

// IsStaticLength

template <typename T>
struct IsStaticLength : std::false_type
{
};

template <typename A, size_t N>
struct IsStaticLength<StaticArray<A, N>> : std::true_type
{
};

template <typename A, size_t N>
struct IsStaticLength<std::array<A, N>> : std::true_type
{
};

template <typename A, size_t N>
struct IsStaticLength<A[N]> : std::true_type
{
};

template <typename T>
struct IsStaticLength<T &> : IsStaticLength<T>
{
};

template <typename T>
struct IsStaticLength<T &&> : IsStaticLength<T>
{
};

// StaticLength
template <typename SeqA>
struct StaticLength;

template <typename A, size_t N>
struct StaticLength<StaticArray<A, N>>
{
    static constexpr size_t value = N;
};

template <typename A, size_t N>
struct StaticLength<std::array<A, N>>
{
    static constexpr size_t value = N;
};

template <typename A, size_t N>
struct StaticLength<A[N]>
{
    static constexpr size_t value = N;
};

template <typename SeqA>
struct StaticLength<SeqA &> : StaticLength<SeqA>
{
};

template <typename SeqA>
struct StaticLength<SeqA &&> : StaticLength<SeqA>
{
};

// IsIntegralConstant

template <typename T>
struct IsIntegralConstant : std::false_type
{
};

template <typename T, T Value>
struct IsIntegralConstant<std::integral_constant<T, Value>> : std::true_type
{
};

// StaticSizeT
template <size_t N>
using StaticSizeT = std::integral_constant<size_t, N>;

// min_value

template <typename A>
constexpr A min_value(const A &value)
{
    return value;
}

// ! Not suitable for type other than min_value
template <typename Head, typename... Tail>
constexpr auto min_value(const Head &head, const Tail &...tail)
    -> typename std::conditional<
        All<IsIntegralConstant<Head>, IsIntegralConstant<Tail>...>::value,
        Head,
        size_t>::type
{
    return head < min_value(tail...) ? head : min_value(tail...);
}

// MinStaticCapacity

template <typename... Seqs>
struct MinStaticCapacity;

template <typename Seq>
struct MinStaticCapacity<Seq>
{
    static constexpr size_t value = StaticCapacity<Seq>::value;
};

template <typename Head, typename... Tail>
struct MinStaticCapacity<Head, Tail...>
{
    static constexpr size_t head = StaticCapacity<Head>::value;
    static constexpr size_t tail = min_value(StaticCapacity<Tail>::value...);
    static constexpr size_t value = head < tail ? head : tail;
};

template <typename... Seqs>
constexpr size_t MinStaticCapacity_v = MinStaticCapacity<Seqs...>::value;

// length

template <typename SeqA>
size_t length(const SeqA &as)
{
    return as.size();
}

template <typename A, size_t N>
auto length(const StaticArray<A, N> &)
    -> std::integral_constant<size_t, N>
{
    return std::integral_constant<size_t, N>{};
}

template <typename A, size_t N>
auto length(const std::array<A, N> &)
    -> std::integral_constant<size_t, N>
{
    return std::integral_constant<size_t, N>{};
}

template <typename A, size_t N>
auto length(const A (&)[N])
    -> std::integral_constant<size_t, N>
{
    return std::integral_constant<size_t, N>{};
}

// min_length

// * Make it return static value if all static capacity, static length.
// template <typename... Seqs>
// constexpr auto min_length(const Seqs &...seqs)
//     -> typename std::conditional<
//         All<IsIntegralConstant<Seqs>...>::value,
//         decltype(),
//         size_t>::type
// {
//     return min_value(length(seqs)...);
// }

template <typename SeqA>
constexpr auto min_length(const SeqA &as)
    -> typename std::conditional<
        IsStaticLength<SeqA>::value,
        decltype(length(as)),
        size_t>::type
{
    return length(as);
}

// ! Not suitable for type other than min_value
template <typename Head, typename... Tail>
constexpr auto min_length(const Head &head, const Tail &...tail)
    -> typename std::conditional<
        All<IsIntegralConstant<Head>, IsIntegralConstant<Tail>...>::value,
        decltype(length(head)),
        size_t>::type
{
    return length(head) < min_length(tail...) ? length(head) : min_length(tail...);
}

template <typename F, typename... Seqs>
void for_each(const F &f, const Seqs &...seqs)
{
    // ? Will it be optimized out to a compile time constatnt?
    const size_t seq_length = min_value(length(seqs)...);

    for (int i = 0; i < seq_length; ++i)
    {
        f(seqs[i]...);
    }
}

template <typename A, typename... Seqs>
using MapSequance_t =
    typename std::conditional<
        All<IsStaticCapacity<Seqs>...>::value,
        typename std::conditional<
            All<IsStaticLength<Seqs>...>::value,
            StaticArray<A, MinStaticCapacity_v<Seqs...>>,
            StaticVector<A, MinStaticCapacity_v<Seqs...>>>::type,
        DynamicVector<A>>::type;

// initilize_result

// ! argument seqs may be redundant if all the seqs are static -> Sould be done out side of it
template <typename R, typename... Seqs>
MapSequance_t<R, Seqs...> map_sequance(const Seqs &...seqs) // Internal data could be unpredictable
{
    if constexpr (All<IsStaticCapacity<Seqs>...>::value)
    {
        if constexpr (All<IsStaticLength<Seqs>...>::value)
        {
            return MapSequance_t<R, Seqs...>();
        }
        else
        {
            return MapSequance_t<R, Seqs...>(min_length(seqs...));
        }
    }
    else
    {
        return MapSequance_t<R, Seqs...>(min_length(seqs...));
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
using Element_t = typename ElementType<A>::type;

// FunctionReturnType;

template <typename, typename...>
struct FunctionReturnType;

template <typename F, typename... Seqs>
struct FunctionReturnType
{
    using type = decltype(std::declval<F>()(std::declval<Seqs>()...));
};

template <typename F, typename... Seqs>
using FunctionReturn_t = typename FunctionReturnType<F, Seqs...>::type;

// MapReturn_t

template <typename F, typename... Seqs>
using MapReturn_t = MapSequance_t<FunctionReturn_t<F, Element_t<Seqs>...>, Seqs...>;

template <typename F, typename... Seqs>
MapReturn_t<F, Seqs...> map(const F &f, const Seqs &...seqs)
{
    using R = FunctionReturn_t<F, Element_t<Seqs>...>;

    auto result = map_sequance<R, Seqs...>(seqs...);

    for (int i = 0; i < min_length(seqs...); ++i)
    {
        result[i] = f(seqs[i]...);
    }

    return result;
}

// MapWithIndexReturn_t

template <typename F, typename... Seqs>
using MapWithIndexReturn_t = MapSequance_t<FunctionReturn_t<F, int, Element_t<Seqs>...>, Seqs...>;

// map_with_index

template <typename F, typename... Seqs>
MapWithIndexReturn_t<F, Seqs...> map_with_index(const F &f, const Seqs &...seqs)
{
    using R = FunctionReturn_t<F, int, Element_t<Seqs>...>;

    auto result = map_sequance<R, Seqs...>(seqs...);

    for (int i = 0; i < min_length(seqs...); ++i)
    {
        result[i] = f(i, seqs[i]...);
    }

    return result;
}

// FilterReturn_t

template <typename SeqA>
using FilterReturn_t =
    typename std::conditional<
        IsStaticCapacity<SeqA>::value,
        StaticVector<Element_t<SeqA>, StaticCapacity<SeqA>::value>,
        DynamicVector<Element_t<SeqA>>>::type;

template <typename SeqA>
constexpr FilterReturn_t<SeqA> filter_sequence(const SeqA &as) // Internal data could be unpredictable, with size 0;
{
    if constexpr (IsStaticCapacity<SeqA>::value)
    {
        return FilterReturn_t<SeqA>();
    }
    else
    {
        auto result = FilterReturn_t<SeqA>();
        result.reserve(length(as) * 2);
        return result;
    }
}

// filter

template <typename F, typename SeqA>
FilterReturn_t<SeqA> filter(const F &f, const SeqA &as)
{
    auto result = filter_sequence<SeqA>(as);

    for (int i = 0; i < length(as); ++i)
    {
        if (f(as[i]))
        {
            result.push_back(as[i]);
        }
    }

    return result;
}

// foldl

template <typename F, typename R, typename SeqA>
R foldl(const F &f, const R &initial_value, const SeqA &as)
{
    R result = initial_value;

    for (int i = 0; i < length(as); ++i)
    {
        result = f(result, as[i]);
    }

    return result;
}

// foldr

template <typename F, typename R, typename SeqA>
R foldr(const F &f, const R &initial_value, const SeqA &as)
{
    R result = initial_value;

    for (int i = length(as) - 1; i > -1; --i)
    {
        result = f(as[i], result);
    }

    return result;
}

// FromFunctionReturn_t

template <typename N, typename F>
using FromFunctionReturn_t = typename std::conditional<
    IsIntegralConstant<N>::value,
    StaticArray<FunctionReturn_t<F, int>, N::value>,
    DynamicVector<FunctionReturn_t<F, int>>>::type;

template <typename N, typename F>
auto from_function(const N &length, const F &f)
    -> typename std::enable_if<
        IsIntegralConstant<N>::value,
        StaticArray<FunctionReturn_t<F, int>, N::value>>::type
{
    StaticArray<FunctionReturn_t<F, int>, N::value> result;

    for (int i = 0; i < N::value; ++i)
    {
        result[i] = f(i);
    }

    return result;
}

template <typename N, typename F>
auto from_function(const N &length, const F &f)
    -> typename std::enable_if<
        !IsIntegralConstant<N>::value,
        DynamicVector<FunctionReturn_t<F, int>>>::type
{
    DynamicVector<FunctionReturn_t<F, int>> result;

    result.reserve(length * 2);

    for (int i = 0; i < length; ++i)
    {
        result[i] = f(i);
    }

    return result;
}
#endif