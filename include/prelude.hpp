#ifndef PRELUDE_HPP_
#define PRELUDE_HPP_

#include <array>
#include <vector>
#include <tuple>
#include <optional>

#include "zero_copy.hpp"
#include "sfinae.hpp"

namespace efp
{
    struct Unit
    {
    };

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
    auto length(const Array<A, N> &)
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

    // execute_pack

    template <typename... Args>
    void execute_pack(Args... args)
    {
    }

    // AppendSequence_t

    template <typename A, typename... Seqs>
    using AppendSequence_t =
        Conditional_t<
            all_v(IsStaticCapacity<Seqs>::value...),
            Conditional_t<
                all_v(IsStaticLength<Seqs>::value...),
                Array<A, sum_v(StaticCapacity<Seqs>::value...)>,
                ArrayVector<A, sum_v(StaticCapacity<Seqs>::value...)>>,
            Vector<A>>;

    // AppendReturn_t
    template <typename Head, typename... Tail>
    using AppendReturn_t = AppendSequence_t<Element_t<Head>, Head, Tail...>;

    // append

    template <typename A, typename B>
    Unit append_(A &result, size_t &idx, const B &seq)
    {
        for (auto elem : seq)
        {
            result[idx] = elem;
            idx++;
        }
        return Unit{};
    }

    template <typename Head, typename... Tail>
    auto append(const Head &head, const Tail &...tail)
        -> EnableIf_t<
            all_v(IsStaticCapacity<Head>::value, IsStaticCapacity<Tail>::value...) && all_v(IsStaticLength<Head>::value, IsStaticLength<Tail>::value...),
            AppendReturn_t<Head, Tail...>>
    {
        AppendReturn_t<Head, Tail...> result;
        size_t idx{0};

        execute_pack(append_(result, idx, head), append_(result, idx, tail)...);

        return result;
    }

    template <typename Head, typename... Tail>
    auto append(const Head &head, const Tail &...tail)
        -> EnableIf_t<
            !all_v(IsStaticLength<Head>::value, IsStaticLength<Tail>::value...),
            AppendReturn_t<Head, Tail...>>
    {
        const size_t result_length = sum_v((size_t)length(head), (size_t)length(tail)...);

        AppendReturn_t<Head, Tail...> result(result_length);
        size_t idx{0};

        execute_pack(append_(result, idx, head), append_(result, idx, tail)...);

        return result;
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

    // for_eachi

    template <typename F, typename... Seqs>
    void for_eachi(const F &f, Seqs &...seqs)
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
                Array<A, MinStaticCapacity<Seqs...>::value>,
                ArrayVector<A, MinStaticCapacity<Seqs...>::value>>,
            Vector<A>>;

    // MapReturn_t

    template <typename F, typename... Seqs>
    using MapReturn_t = MapSequence_t<CallReturn_t<F, Element_t<Seqs>...>, Seqs...>;

    // map

    template <typename F, typename... Seqs>
    auto map(const F &f, const Seqs &...seqs)
        -> EnableIf_t<
            all_v(IsStaticCapacity<Seqs>::value...) && all_v(IsStaticLength<Seqs>::value...),
            Array<CallReturn_t<F, Element_t<Seqs>...>, MinStaticCapacity<Seqs...>::value>>
    {
        using R = CallReturn_t<F, Element_t<Seqs>...>;

        Array<R, MinStaticCapacity<Seqs...>::value> result;

        for (int i = 0; i < MinStaticCapacity<Seqs...>::value; ++i)
        {
            result[i] = f(seqs[i]...);
        }

        return result;
    }

    // template <typename F, typename... Seqs>
    // auto map(const F &f, const Seqs &...seqs)
    //     -> EnableIf_t<
    //         all_v(IsStaticCapacity<Seqs>::value...) && !all_v(IsStaticLength<Seqs>::value...),
    //         ArrayVector<CallReturn_t<F, Element_t<Seqs>...>, MinStaticCapacity<Seqs...>::value>>
    // {
    //     using R = CallReturn_t<F, Element_t<Seqs>...>;

    //     const size_t result_length = min_length(seqs...);

    //     ArrayVector<R, MinStaticCapacity<Seqs...>::value> result(result_length);

    //     for (int i = 0; i < result_length; ++i)
    //     {
    //         result[i] = f(seqs[i]...);
    //     }

    //     return result;
    // }

    // template <typename F, typename... Seqs>
    // auto map(const F &f, const Seqs &...seqs)
    //     -> EnableIf_t<
    //         !all_v(IsStaticCapacity<Seqs>::value...),
    //         Vector<CallReturn_t<F, Element_t<Seqs>...>>>
    // {
    //     using R = CallReturn_t<F, Element_t<Seqs>...>;

    //     const size_t result_length = min_length(seqs...);

    //     Vector<R> result(result_length);

    //     for (int i = 0; i < result_length; ++i)
    //     {
    //         result[i] = f(seqs[i]...);
    //     }

    //     return result;
    // }

    template <typename F, typename... Seqs>
    auto map(const F &f, const Seqs &...seqs)
        -> EnableIf_t<
            !all_v(IsStaticLength<Seqs>::value...),
            MapReturn_t<F, Seqs...>>
    {
        const size_t result_length = min_length(seqs...);

        MapReturn_t<F, Seqs...> result(result_length);

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
            ArrayVector<Element_t<SeqA>, StaticCapacity<SeqA>::value>,
            Vector<Element_t<SeqA>>>;

    // filter

    template <typename F, typename SeqA>
    auto filter(const F &f, const SeqA &as)
        -> EnableIf_t<
            IsStaticCapacity<SeqA>::value,
            ArrayVector<Element_t<SeqA>, StaticCapacity<SeqA>::value>>
    {
        ArrayVector<Element_t<SeqA>, StaticCapacity<SeqA>::value> result;

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
            Vector<Element_t<SeqA>>>
    {
        const size_t sequance_length = length(as);

        Vector<Element_t<SeqA>> result;
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
        Array<CallReturn_t<F, int>, N::value>,
        Vector<CallReturn_t<F, int>>>;

    // from_function

    template <typename N, typename F>
    auto from_function(const N &length, const F &f)
        -> EnableIf_t<
            IsIntegralConstant<N>::value,
            Array<CallReturn_t<F, int>, N::value>>
    {
        Array<CallReturn_t<F, int>, N::value> result;

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
            Vector<CallReturn_t<F, int>>>
    {
        Vector<CallReturn_t<F, int>> result(length);

        for (int i = 0; i < length; ++i)
        {
            result[i] = f(i);
        }

        return result;
    }

    // for_index

    template <typename F>
    void for_index(const F &f, const int i)
    {
        for (int i_ = 0; i_ < i; ++i_)
        {
            f(i_);
        }
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

    // for_each_with_indexi

    template <typename F, typename... Seqs>
    void for_each_with_indexi(const F &f, Seqs &...seqs)
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

    // cartesian_for_eachi

    template <typename F, typename SeqA>
    void cartesian_for_eachi(const F &f, SeqA &as)
    {
        for_eachi(f, as);
    }

    template <typename F, typename SeqA, typename... Seqs>
    void cartesian_for_eachi(const F &f, SeqA &as, Seqs &...seqs)
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

            cartesian_for_eachi<decltype(inner), Seqs...>(inner, seqs...);
        }
    }

    // map_with_index

    template <typename F, typename... Seqs>
    auto map_with_index(const F &f, const Seqs &...seqs)
        -> EnableIf_t<
            all_v(IsStaticCapacity<Seqs>::value...) && all_v(IsStaticLength<Seqs>::value...),
            Array<CallReturn_t<F, int, Element_t<Seqs>...>, MinStaticCapacity<Seqs...>::value>>
    {
        using R = CallReturn_t<F, int, Element_t<Seqs>...>;

        Array<R, MinStaticCapacity<Seqs...>::value> result;

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
            ArrayVector<CallReturn_t<F, int, Element_t<Seqs>...>, MinStaticCapacity<Seqs...>::value>>
    {
        using R = CallReturn_t<F, int, Element_t<Seqs>...>;

        const size_t result_length = min_length(seqs...);

        ArrayVector<R, MinStaticCapacity<Seqs...>::value> result(result_length);

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
            Vector<CallReturn_t<F, int, Element_t<Seqs>...>>>
    {
        using R = CallReturn_t<F, int, Element_t<Seqs>...>;

        const size_t result_length = min_length(seqs...);

        Vector<R> result(result_length);

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
            Array<CallReturn_t<F, Element_t<Seqs>...>, StaticCapacityProduct<Seqs...>::value>>
    {
        using R = CallReturn_t<F, Element_t<Seqs>...>;

        Array<R, StaticCapacityProduct<Seqs...>::value> result;
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
            ArrayVector<CallReturn_t<F, Element_t<Seqs>...>, StaticCapacityProduct<Seqs...>::value>>
    {
        using R = CallReturn_t<F, Element_t<Seqs>...>;

        const size_t result_length = size_t_product(length(seqs)...);

        ArrayVector<R, StaticCapacityProduct<Seqs...>::value> result(result_length);
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
            Vector<CallReturn_t<F, Element_t<Seqs>...>>>
    {
        using R = CallReturn_t<F, Element_t<Seqs>...>;

        const size_t result_length = size_t_product(length(seqs)...);

        Vector<R> result(result_length);
        size_t i = 0;

        const auto inner = [&](Element_t<Seqs>... xs)
        {
            result[i++] = f(xs...);
        };

        cartesian_for_each(inner, seqs...);

        return result;
    }

    // // todo No std
    // template <typename SeqA>
    // std::optional<size_t> elem_index(const Element_t<SeqA> a, const SeqA &as)
    // {
    //     for (size_t i = 0; i < length(as); ++i)
    //     {
    //         if (as[i] == a)
    //         {
    //             return i;
    //         }
    //     }

    //     return std::nullopt;
    // }

    // ! temp -1 on fail
    template <typename SeqA>
    int elem_index(const Element_t<SeqA> a, const SeqA &as)
    {
        for (size_t i = 0; i < length(as); ++i)
        {
            if (as[i] == a)
            {
                return i;
            }
        }

        return -1;
    }
}
#endif