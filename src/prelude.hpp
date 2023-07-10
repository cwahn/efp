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

template <typename... Seqs>
struct AreAllStatic
{
    static constexpr bool value = All_v<IsStatic<Seqs>...>;
};

template <typename... Seqs>
constexpr bool AreAllStatic_v = AreAllStatic<Seqs...>::value;

// IsStaticVector
template <typename A>
struct IsStaticVector : std::false_type
{
};

template <typename A, size_t N>
struct IsStaticVector<StaticVector<A, N>> : std::true_type
{
};

template <typename A>
struct IsStaticVector<A &> : IsStaticVector<A>
{
};

template <typename A>
struct IsStaticVector<A &&> : IsStaticVector<A>
{
};

template <typename A>
constexpr bool IsStaticVector_v = IsStaticVector<A>::value;

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

template <typename A>
constexpr size_t StaticCapacity_v = StaticCapacity<A>::value;

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

template <typename... Seqs>
struct MinStaticCapacity;

template <typename Seq>
struct MinStaticCapacity<Seq>
{
    static constexpr size_t value = StaticCapacity_v<Seq>;
};

template <typename Head, typename... Tail>
struct MinStaticCapacity<Head, Tail...>
{
    static constexpr size_t head = StaticCapacity_v<Head>;
    static constexpr size_t tail = min_value(StaticCapacity_v<Tail>...);
    static constexpr size_t value = head < tail ? head : tail;
};

template <typename... Seqs>
constexpr size_t MinStaticCapacity_v = MinStaticCapacity<Seqs...>::value;

// length

template <typename A>
size_t length(const A &seq)
{
    return seq.size();
}

template <typename A, size_t N>
constexpr size_t length(const A (&)[N])
{
    return N;
}

// min_length

template <typename... Seqs>
constexpr size_t min_length(const Seqs &...seqs)
{
    return min_value(length(seqs)...);
}

template <typename F, typename... Seqs>
constexpr void for_each(F f, const Seqs &...seqs)
{
    const size_t seq_length = min_value(length(seqs)...);

    for (int i = 0; i < seq_length; ++i)
    {
        f(seqs[i]...);
    }
}

template <typename A, typename... Seqs>
using FmapSequance_t = typename std::conditional<All_v<IsStatic<Seqs>...>,
                                                 typename std::conditional<Any_v<IsStaticVector<Seqs>...>,
                                                                           StaticVector<A, MinStaticCapacity_v<Seqs...>>,
                                                                           StaticArray<A, MinStaticCapacity_v<Seqs...>>>::type,
                                                 DynamicVector<A>>::type;

// initilize_result

template <typename R, typename... Seqs>
constexpr FmapSequance_t<R, Seqs...> fmap_sequance(const Seqs &...seqs) // Internal data could be unpredictable
{
    if constexpr (All_v<IsStatic<Seqs>...>)
    {
        if constexpr (Any_v<IsStaticVector<Seqs>...>)
        {
            return StaticVector<R, MinStaticCapacity_v<Seqs...>>(min_length(seqs...));
        }
        else
        {
            return StaticArray<R, MinStaticCapacity_v<Seqs...>>();
        }
    }
    else
    {
        return DynamicVector<R>(min_length(seqs...));
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

template <typename F, typename... Seqs>
struct FunctionReturnType
{
    using type = decltype(std::declval<F>()(std::declval<Seqs>()...));
};

template <typename F, typename... Seqs>
using FunctionReturnType_t = typename FunctionReturnType<F, Seqs...>::type;

// FmapReturnType_t

template <typename F, typename... Seqs>
using FmapReturnType_t = FmapSequance_t<FunctionReturnType_t<F, ElementType_t<Seqs>...>, Seqs...>;

template <typename F, typename... Seqs>
constexpr FmapReturnType_t<F, Seqs...> fmap(F f, const Seqs &...seqs)
{
    using R = FunctionReturnType_t<F, ElementType_t<Seqs>...>;

    auto result = fmap_sequance<R, Seqs...>(seqs...);

    if constexpr (All_v<IsStatic<Seqs>...>)
    {
        if constexpr (Any_v<IsStaticVector<Seqs>...>)
        {
            for (int i = 0; i < min_length(seqs...); ++i)
            {
                result[i] = f(seqs[i]...);
            }
        }
        else
        {
            for (int i = 0; i < MinStaticCapacity_v<Seqs...>; ++i)
            {
                result[i] = f(seqs[i]...);
            }
        }
    }
    else
    {
        for (int i = 0; i < min_length(seqs...); ++i)
        {
            result[i] = f(seqs[i]...);
        }
    }

    return result;
}

template <typename R, typename Seq>
using FilterSequence_t = typename std::conditional<IsStatic_v<Seq>,
                                                   StaticVector<R, StaticCapacity_v<Seq>>,
                                                   DynamicVector<R>>::type;

template <typename R, typename Seq>
constexpr FilterSequence_t<R, Seq> filter_sequence(const Seq &seq) // Internal data could be unpredictable, with size 0;
{
    if constexpr (IsStatic_v<Seq>)
    {
        return StaticVector<R, StaticCapacity_v<Seq>>();
    }
    else
    {
        auto result = DynamicVector<R>();
        result.reserve(length(seq) * 2);
        return result;
    }
}

template <typename F, typename Seq>
constexpr FilterSequence_t<ElementType_t<Seq>, Seq> filter(F f, const Seq &seq)
{
    auto result = filter_sequence<ElementType_t<Seq>, Seq>(seq);

    for (int i = 0; i < length(seq); ++i)
    {
        if (f(seq[i]))
        {
            result.push_back(seq[i]);
        }
    }

    return result;
}

template <typename F, typename R, typename Seq>
constexpr R foldl(F f, const R initial_value, const Seq &seq)
{
    R result = initial_value;

    for (int i = 0; i < length(seq); ++i)
    {
        result = f(result, seq[i]);
    }

    return result;
}

template <typename F, typename R, typename Seq>
constexpr R foldr(F f, const R initial_value, const Seq &seq)
{
    R result = initial_value;

    for (int i = length(seq) - 1; i > -1; --i)
    {
        result = f(seq[i], result);
    }

    return result;
}

#endif