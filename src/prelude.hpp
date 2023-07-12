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
using Common_t = typename std::common_type<Args...>::type;

// ? What if not either Static and Dynamic?
// IsStaticCapacity

template <typename T>
struct IsStaticCapacity : std::false_type
{
};

template <typename T, size_t N>
struct IsStaticCapacity<T[N]> : std::true_type
{
};

template <typename T, size_t N>
struct IsStaticCapacity<std::array<T, N>> : std::true_type
{
};

template <typename T, size_t N>
struct IsStaticCapacity<StaticArray<T, N>> : std::true_type
{
};

template <typename T, size_t N>
struct IsStaticCapacity<StaticVector<T, N>> : std::true_type
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

template <typename T>
constexpr bool IsStatic_v = IsStaticCapacity<T>::value;

// AreAllStaticCapacity

template <typename... Seqs>
struct AreAllStaticCapacity
{
    static constexpr bool value = All_v<IsStaticCapacity<Seqs>...>;
};

// IsDynamicLength

template <typename T>
struct IsDynamicLength : std::false_type
{
};

template <typename A, size_t N>
struct IsDynamicLength<StaticVector<A, N>> : std::true_type
{
};

template <typename A>
struct IsDynamicLength<DynamicVector<A>> : std::true_type
{
};

template <typename A>
struct IsDynamicLength<std::vector<A>> : std::true_type
{
};

template <typename T>
struct IsDynamicLength<T &> : IsDynamicLength<T>
{
};

template <typename T>
struct IsDynamicLength<T &&> : IsDynamicLength<T>
{
};

template <typename... Seqs>
constexpr bool AreAllStatic_v = AreAllStaticCapacity<Seqs...>::value;

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

template <typename SeqA>
size_t length(const SeqA &as)
{
    return as.size();
}

template <typename A, size_t N>
size_t length(const A (&)[N])
{
    return N;
}

// min_length

// * Make it return static value if all static capacity, static length.
template <typename... Seqs>
constexpr size_t min_length(const Seqs &...seqs)
{
    return min_value(length(seqs)...);
}

template <typename F, typename... Seqs>
constexpr void for_each(const F &f, const Seqs &...seqs)
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
    typename std::conditional<All_v<IsStaticCapacity<Seqs>...>,
                              typename std::conditional<Any_v<IsStaticVector<Seqs>...>,
                                                        StaticVector<A, MinStaticCapacity_v<Seqs...>>,
                                                        StaticArray<A, MinStaticCapacity_v<Seqs...>>>::type,
                              DynamicVector<A>>::type;

// initilize_result

// ! argument seqs may be redundant if all the seqs are static -> Sould be done out side of it
template <typename R, typename... Seqs>
constexpr MapSequance_t<R, Seqs...> fmap_sequance(const Seqs &...seqs) // Internal data could be unpredictable
{
    if constexpr (All_v<IsStaticCapacity<Seqs>...>)
    {
        if constexpr (Any_v<IsStaticVector<Seqs>...>)
        {
            return MapSequance_t<R, Seqs...>(min_length(seqs...));
        }
        else
        {
            return MapSequance_t<R, Seqs...>();
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
constexpr MapReturn_t<F, Seqs...> map(const F &f, const Seqs &...seqs)
{
    using R = FunctionReturn_t<F, Element_t<Seqs>...>;

    auto result = fmap_sequance<R, Seqs...>(seqs...);

    for (int i = 0; i < min_length(seqs...); ++i)
    {
        result[i] = f(seqs[i]...);
    }

    return result;
}

template <typename SeqA>
using FilterReturn_t = typename std::conditional<IsStatic_v<SeqA>,
                                                 StaticVector<Element_t<SeqA>, StaticCapacity_v<SeqA>>,
                                                 DynamicVector<Element_t<SeqA>>>::type;

template <typename SeqA>
constexpr FilterReturn_t<SeqA> filter_sequence(const SeqA &as) // Internal data could be unpredictable, with size 0;
{
    if constexpr (IsStatic_v<SeqA>)
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

template <typename F, typename SeqA>
constexpr FilterReturn_t<SeqA> filter(const F &f, const SeqA &as)
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

template <typename F, typename R, typename SeqA>
constexpr R foldl(const F &f, const R &initial_value, const SeqA &as)
{
    R result = initial_value;

    for (int i = 0; i < length(as); ++i)
    {
        result = f(result, as[i]);
    }

    return result;
}

template <typename F, typename R, typename SeqA>
constexpr R foldr(const F &f, const R &initial_value, const SeqA &as)
{
    R result = initial_value;

    for (int i = length(as) - 1; i > -1; --i)
    {
        result = f(as[i], result);
    }

    return result;
}

#endif