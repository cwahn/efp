#ifndef PRELUDE_HPP_
#define PRELUDE_HPP_

#include <array>
#include <vector>
#include <iterator>
#include <functional>
#include <algorithm>
#include <utility>
#include <numeric>
#include <tuple>

#include "zero_copy.hpp"
#include "sfinae.hpp"

namespace efp
{
    template <typename A>
    A id(const A &a)
    {
        return a;
    }

    // ! WIP
    // Composed

    // template <typename... Fs>
    // struct Composed;

    // Composed

    template <typename F, typename G>
    struct Composed
    {
        const F &f;
        const G &g;

        Composed(const F &f, const G &g) : f(f), g(g) {}

        template <typename Arg>

        auto operator()(Arg &&arg) const -> decltype(f(g(std::forward<Arg>(arg))))
        {
            return f(g(std::forward<Arg>(arg)));
        }
    };

    // template <typename F, typename... Fs>
    // struct Composed<F, Fs...>
    // {
    //     const F &f;
    //     const Composed<Fs...> fs;

    //     Composed(const F &f, const Fs &...fs) : f(f), fs(compose(fs...)) {}

    //     template <typename Arg>

    //     inline auto operator()(Arg &&arg) -> decltype(f(fs(std::forward<Arg>(arg))))
    //     {
    //         return f(fs(std::forward<Arg>(arg)));
    //     }
    // };

    // template <typename F, typename G>
    // inline auto operator*(F f, G g) -> Composed<F, G>
    // {
    //     return compose(f, g);
    // }

    // compose

    template <typename F, typename G>
    auto compose(const F &f, const G &g) -> Composed<F, G>
    {
        return Composed<F, G>(f, g);
    }

    // template <typename F, typename... Fs>
    // auto compose(const F &f, const Fs &...fs) -> Composed<F, Fs...>
    // {
    //     return Composed<F, Fs...>(f, fs...);
    // }

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

    template <typename SeqA>
    constexpr auto min_length(const SeqA &as)
        -> Conditional_t<
            IsStaticLength<SeqA>::value,
            decltype(length(as)),
            size_t>
    {
        return length(as);
    }

    template <typename Head, typename... Tail>
    constexpr auto min_length(const Head &head, const Tail &...tail)
        -> Conditional_t<
            // All<IsIntegralConstant<Head>, IsIntegralConstant<Tail>...>::value,
            all_v(IsIntegralConstant<Head>::value, IsIntegralConstant<Tail>::value...),
            decltype(length(head)),
            size_t>
    {
        return length(head) < min_length(tail...) ? length(head) : min_length(tail...);
    }

    // for_each

    template <typename F, typename... Seqs>
    void for_each(const F &f, const Seqs &...seqs)
    {
        // ? Will it be optimized out to a compile time constatnt?
        const size_t seq_length = min_length(seqs...);

        for (int i = 0; i < seq_length; ++i)
        {
            f(seqs[i]...);
        }
    }

    // MapSequence_t

    template <typename A, typename... Seqs>
    using MapSequence_t =
        Conditional_t<
            all_v(IsStaticCapacity<Seqs>::value...),
            Conditional_t<
                all_v(IsStaticLength<Seqs>::value...),
                StaticArray<A, MinStaticCapacity<Seqs...>::value>,
                StaticVector<A, MinStaticCapacity<Seqs...>::value>>,
            DynamicVector<A>>;

    // MapReturn_t

    template <typename F, typename... Seqs>
    using MapReturn_t = MapSequence_t<CallReturn_t<F, Element_t<Seqs>...>, Seqs...>;

    // map

    template <typename F, typename... Seqs>
    auto map(const F &f, const Seqs &...seqs)
        -> EnableIf_t<
            all_v(IsStaticCapacity<Seqs>::value...) && all_v(IsStaticLength<Seqs>::value...),
            StaticArray<CallReturn_t<F, Element_t<Seqs>...>, MinStaticCapacity<Seqs...>::value>>
    {
        using R = CallReturn_t<F, Element_t<Seqs>...>;

        StaticArray<R, MinStaticCapacity<Seqs...>::value> result;

        for (int i = 0; i < MinStaticCapacity<Seqs...>::value; ++i)
        {
            result[i] = f(seqs[i]...);
        }

        return result;
    }

    template <typename F, typename... Seqs>
    auto map(const F &f, const Seqs &...seqs)
        -> EnableIf_t<
            all_v(IsStaticCapacity<Seqs>::value...) && !all_v(IsStaticLength<Seqs>::value...),
            StaticVector<CallReturn_t<F, Element_t<Seqs>...>, MinStaticCapacity<Seqs...>::value>>
    {
        using R = CallReturn_t<F, Element_t<Seqs>...>;

        const size_t result_length = min_length(seqs...);

        StaticVector<R, MinStaticCapacity<Seqs...>::value> result(result_length);

        for (int i = 0; i < result_length; ++i)
        {
            result[i] = f(seqs[i]...);
        }

        return result;
    }

    template <typename F, typename... Seqs>
    auto map(const F &f, const Seqs &...seqs)
        -> EnableIf_t<
            !all_v(IsStaticCapacity<Seqs>::value...),
            DynamicVector<CallReturn_t<F, Element_t<Seqs>...>>>
    {
        using R = CallReturn_t<F, Element_t<Seqs>...>;

        const size_t result_length = min_length(seqs...);

        DynamicVector<R> result(result_length);

        for (int i = 0; i < result_length; ++i)
        {
            result[i] = f(seqs[i]...);
        }

        return result;
    }

    // MapWithIndexReturn_t

    template <typename F, typename... Seqs>
    using MapWithIndexReturn_t = MapSequence_t<CallReturn_t<F, int, Element_t<Seqs>...>, Seqs...>;

    // FilterReturn_t

    template <typename SeqA>
    using FilterReturn_t =
        Conditional_t<
            IsStaticCapacity<SeqA>::value,
            StaticVector<Element_t<SeqA>, StaticCapacity<SeqA>::value>,
            DynamicVector<Element_t<SeqA>>>;

    // filter

    template <typename F, typename SeqA>
    auto filter(const F &f, const SeqA &as)
        -> EnableIf_t<
            IsStaticCapacity<SeqA>::value,
            StaticVector<Element_t<SeqA>, StaticCapacity<SeqA>::value>>
    {
        StaticVector<Element_t<SeqA>, StaticCapacity<SeqA>::value> result;

        for (int i = 0; i < length(as); ++i)
        {
            const auto a = as[i];
            if (f(a))
            {
                result.push_back(a);
            }
        }

        return result;
    }

    template <typename F, typename SeqA>
    auto filter(const F &f, const SeqA &as)
        -> EnableIf_t<
            !IsStaticCapacity<SeqA>::value,
            DynamicVector<Element_t<SeqA>>>
    {
        const size_t sequance_length = length(as);

        DynamicVector<Element_t<SeqA>> result;
        result.reserve(sequance_length * 2);

        for (int i = 0; i < sequance_length; ++i)
        {
            const auto a = as[i];
            if (f(a))
            {
                result.push_back(a);
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
    using FromFunctionReturn_t = Conditional_t<
        IsIntegralConstant<N>::value,
        StaticArray<CallReturn_t<F, int>, N::value>,
        DynamicVector<CallReturn_t<F, int>>>;

    // from_function

    template <typename N, typename F>
    auto from_function(const N &length, const F &f)
        -> EnableIf_t<
            IsIntegralConstant<N>::value,
            StaticArray<CallReturn_t<F, int>, N::value>>
    {
        StaticArray<CallReturn_t<F, int>, N::value> result;

        for (int i = 0; i < N::value; ++i)
        {
            result[i] = f(i);
        }

        return result;
    }

    template <typename N, typename F>
    auto from_function(const N &length, const F &f)
        -> EnableIf_t<
            !IsIntegralConstant<N>::value,
            DynamicVector<CallReturn_t<F, int>>>
    {
        DynamicVector<CallReturn_t<F, int>> result(length);

        for (int i = 0; i < length; ++i)
        {
            result[i] = f(i);
        }

        return result;
    }

    // for_each_with_index

    template <typename F, typename... Seqs>
    void for_each_with_index(const F &f, const Seqs &...seqs)
    {
        // ? Will it be optimized out to a compile time constatnt?
        const size_t seq_length = min_length(seqs...);

        for (int i = 0; i < seq_length; ++i)
        {
            f(i, seqs[i]...);
        }
    }

    // cartesian_for_each

    template <typename F, typename SeqA>
    void cartesian_for_each(const F &f, const SeqA &as)
    {
        for_each(f, as);
    }

    template <typename F, typename SeqA, typename... Seqs>
    void cartesian_for_each(const F &f, const SeqA &as, const Seqs &...seqs)
    {
        // ? Will it be optimized out to a compile time constatnt?
        const size_t as_length = length(as);
        Element_t<SeqA> a;

        for (int i = 0; i < as_length; ++i)
        {
            a = as[i];
            const auto inner = [=](Element_t<Seqs>... xs)
            {
                f(a, xs...);
            };

            cartesian_for_each<decltype(inner), Seqs...>(inner, seqs...);
        }
    }

    // map_with_index

    template <typename F, typename... Seqs>
    auto map_with_index(const F &f, const Seqs &...seqs)
        -> EnableIf_t<
            all_v(IsStaticCapacity<Seqs>::value...) && all_v(IsStaticLength<Seqs>::value...),
            StaticArray<CallReturn_t<F, int, Element_t<Seqs>...>, MinStaticCapacity<Seqs...>::value>>
    {
        using R = CallReturn_t<F, int, Element_t<Seqs>...>;

        StaticArray<R, MinStaticCapacity<Seqs...>::value> result;

        for (int i = 0; i < MinStaticCapacity<Seqs...>::value; ++i)
        {
            result[i] = f(i, seqs[i]...);
        }

        return result;
    }

    template <typename F, typename... Seqs>
    auto map_with_index(const F &f, const Seqs &...seqs)
        -> EnableIf_t<
            all_v(IsStaticCapacity<Seqs>::value...) && !all_v(IsStaticLength<Seqs>::value...),
            StaticVector<CallReturn_t<F, int, Element_t<Seqs>...>, MinStaticCapacity<Seqs...>::value>>
    {
        using R = CallReturn_t<F, int, Element_t<Seqs>...>;

        const size_t result_length = min_length(seqs...);

        StaticVector<R, MinStaticCapacity<Seqs...>::value> result(result_length);

        for (int i = 0; i < result_length; ++i)
        {
            result[i] = f(i, seqs[i]...);
        }

        return result;
    }

    template <typename F, typename... Seqs>
    auto map_with_index(const F &f, const Seqs &...seqs)
        -> EnableIf_t<
            !all_v(IsStaticCapacity<Seqs>::value...),
            DynamicVector<CallReturn_t<F, int, Element_t<Seqs>...>>>
    {
        using R = CallReturn_t<F, int, Element_t<Seqs>...>;

        const size_t result_length = min_length(seqs...);

        DynamicVector<R> result(result_length);

        for (int i = 0; i < result_length; ++i)
        {
            result[i] = f(i, seqs[i]...);
        }

        return result;
    }

    // cartesian_map
    // ! Maybe need to bechmark and optimize
    template <typename F, typename... Seqs>
    auto cartesian_map(const F &f, const Seqs &...seqs)
        -> EnableIf_t<
            all_v(IsStaticCapacity<Seqs>::value...) && all_v(IsStaticLength<Seqs>::value...),
            StaticArray<CallReturn_t<F, Element_t<Seqs>...>, StaticCapacityProduct<Seqs...>::value>>
    {
        using R = CallReturn_t<F, Element_t<Seqs>...>;

        StaticArray<R, StaticCapacityProduct<Seqs...>::value> result;
        size_t i = 0;

        const auto inner = [&](Element_t<Seqs>... xs)
        {
            result[i++] = f(xs...);
        };

        cartesian_for_each(inner, seqs...);

        return result;
    }

    template <typename F, typename... Seqs>
    auto cartesian_map(const F &f, const Seqs &...seqs)
        -> EnableIf_t<
            all_v(IsStaticCapacity<Seqs>::value...) && !all_v(IsStaticLength<Seqs>::value...),
            StaticVector<CallReturn_t<F, Element_t<Seqs>...>, StaticCapacityProduct<Seqs...>::value>>
    {
        using R = CallReturn_t<F, Element_t<Seqs>...>;

        const size_t result_length = size_t_product(length(seqs)...);

        StaticVector<R, StaticCapacityProduct<Seqs...>::value> result(result_length);
        size_t i = 0;

        const auto inner = [&](Element_t<Seqs>... xs)
        {
            result[i++] = f(xs...);
        };

        cartesian_for_each(inner, seqs...);

        return result;
    }

    template <typename F, typename... Seqs>
    auto cartesian_map(const F &f, const Seqs &...seqs)
        -> EnableIf_t<
            !all_v(IsStaticCapacity<Seqs>::value...),
            DynamicVector<CallReturn_t<F, Element_t<Seqs>...>>>
    {
        using R = CallReturn_t<F, Element_t<Seqs>...>;

        const size_t result_length = size_t_product(length(seqs)...);

        DynamicVector<R> result(result_length);
        size_t i = 0;

        const auto inner = [&](Element_t<Seqs>... xs)
        {
            result[i++] = f(xs...);
        };

        cartesian_for_each(inner, seqs...);

        return result;
    }

}
#endif