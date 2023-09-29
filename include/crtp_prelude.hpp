#ifndef CRTP_PRELUDE_HPP_
#define CRTP_PRELUDE_HPP_

#include <array>
#include <vector>
#include <tuple>
#include <optional>

#include "sfinae.hpp"
#include "enum_type.hpp"
#include "crtp_sequence.hpp"
#include "maybe.hpp"

// Trust compiler for length calculation

namespace efp
{
    // struct Unit
    // {
    // };

    template <typename A>
    constexpr A id(const A &a)
    {
        return a;
    }

    // // ! WIP
    // // Composed

    // // template <typename... Fs>
    // // struct Composed;

    // // Composed

    // template <typename F, typename G>
    // struct Composed
    // {
    //     const F &f;
    //     const G &g;

    //     Composed(const F &f, const G &g) : f(f), g(g) {}

    //     template <typename Arg>

    //     auto operator()(Arg &&arg) const -> decltype(f(g(std::forward<Arg>(arg))))
    //     {
    //         return f(g(std::forward<Arg>(arg)));
    //     }
    // };

    // // template <typename F, typename... Fs>
    // // struct Composed<F, Fs...>
    // // {
    // //     const F &f;
    // //     const Composed<Fs...> fs;

    // //     Composed(const F &f, const Fs &...fs) : f(f), fs(compose(fs...)) {}

    // //     template <typename Arg>

    // //     inline auto operator()(Arg &&arg) -> decltype(f(fs(std::forward<Arg>(arg))))
    // //     {
    // //         return f(fs(std::forward<Arg>(arg)));
    // //     }
    // // };

    // // template <typename F, typename G>
    // // inline auto operator*(F f, G g) -> Composed<F, G>
    // // {
    // //     return compose(f, g);
    // // }

    // // compose

    // template <typename F, typename G>
    // auto compose(const F &f, const G &g) -> Composed<F, G>
    // {
    //     return Composed<F, G>(f, g);
    // }

    // // template <typename F, typename... Fs>
    // // auto compose(const F &f, const Fs &...fs) -> Composed<F, Fs...>
    // // {
    // //     return Composed<F, Fs...>(f, fs...);
    // // }

    // // execute_pack

    // template <typename... Args>
    // void execute_pack(Args... args)
    // {
    // }

    // // AppendSequence_t

    // template <typename A, typename... Seqs>
    // using AppendSequence_t =
    //     Conditional<
    //         all_v(IsStaticCapacity<Seqs>::value...),
    //         Conditional<
    //             all_v(IsStaticLength<Seqs>::value...),
    //             Array<A, sum_v(StaticCapacity<Seqs>::value...)>,
    //             ArrVec<A, sum_v(StaticCapacity<Seqs>::value...)>>,
    //         Vector<A>>;

    // // AppendReturn_t
    // template <typename Head, typename... Tail>
    // using AppendReturn_t = AppendSequence_t<Element<Head>, Head, Tail...>;

    // // append

    // template <typename A, typename B>
    // Unit append_(A &result, int &idx, const B &seq)
    // {
    //     for (auto elem : seq)
    //     {
    //         result[idx] = elem;
    //         idx++;
    //     }
    //     return Unit{};
    // }

    // template <typename Head, typename... Tail>
    // auto append(const Head &head, const Tail &...tail)
    //     -> EnableIf<
    //         all_v(IsStaticCapacity<Head>::value, IsStaticCapacity<Tail>::value...) && all_v(IsStaticLength<Head>::value, IsStaticLength<Tail>::value...),
    //         AppendReturn_t<Head, Tail...>>
    // {
    //     AppendReturn_t<Head, Tail...> result;
    //     int idx{0};

    //     execute_pack(append_(result, idx, head), append_(result, idx, tail)...);

    //     return result;
    // }

    // template <typename Head, typename... Tail>
    // auto append(const Head &head, const Tail &...tail)
    //     -> EnableIf<
    //         !all_v(IsStaticLength<Head>::value, IsStaticLength<Tail>::value...),
    //         AppendReturn_t<Head, Tail...>>
    // {
    //     // ! Error on int
    //     const size_t bounded_n = sum_v((int)length(head), (int)length(tail)...);

    //     AppendReturn_t<Head, Tail...> result(bounded_n);
    //     int idx{0};

    //     execute_pack(append_(result, idx, head), append_(result, idx, tail)...);

    //     return result;
    // }

    // min_length

    template <typename A, typename... Ts>
    int min_length(const Seq<A> &as, const Seq<Ts> &...seqs)
    {
        return minimum_v(static_cast<int>(length(as)), length(seqs)...);
    }

    // for_each

    template <typename... As, typename F = void (*)(const Element<As> &...)>
    void for_each(const F &f, const Seq<As> &...seqs)
    {
        const int seq_length = min_length(seqs...);

        for (int i = 0; i < seq_length; ++i)
        {
            f(seqs[i]...);
        }
    }

    // for_eachi

    template <typename... As, typename F = void (*)(Element<As> &...)>
    void for_eachi(const F &f, Seq<As> &...seqs)
    {
        const int seq_length = min_length(seqs...);

        for (int i = 0; i < seq_length; ++i)
        {
            f(seqs[i]...);
        }
    }

    // MapReturn

    template <typename F, typename... As>
    using MapReturn = Sequence<
        CallReturn<F, Element<As>...>,
        AreAllStaticCapacity<As...>::value ? MinStaticCapacity<As...>::value : dyn,
        AreAllStaticLength<As...>::value ? MinStaticLength<As...>::value : dyn>;

    // map

    template <typename F, typename... As>
    auto map(const F &f, const Seq<As> &...seqs)
        -> MapReturn<F, As...>
    {
        MapReturn<F, As...> result{};
        const int bounded_n = min_length(seqs...);
        if (MapReturn<F, As...>::ct_len == dyn)
        {
            result.resize(bounded_n);
        }

        for (int i = 0; i < bounded_n; ++i)
        {
            result[i] = f(seqs[i]...);
        }

        return result;
    }

    // FilterReturn

    template <typename F, typename A>
    using FilterReturn = Sequence<Element<A>, dyn, A::ct_cap>;

    // filter

    template <typename A, typename F = bool (*)(Element<A> &)>
    auto filter(const F &f, const Seq<A> &as)
        -> FilterReturn<F, A>
    {
        FilterReturn<F, A> result{};
        const auto input_length = length(as);

        for (int i = 0; i < input_length; ++i)
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

    template <typename A, typename R, typename F = R (*)(const R &, const Element<A> &)>
    R foldl(const F &f, const R &init, const Seq<A> &as)
    {
        R result = init;

        for (int i = 0; i < length(as); ++i)
        {
            result = f(result, as[i]);
        }

        return result;
    }

    // foldr

    template <typename A, typename R, typename F = R (*)(const Element<A> &, const R &)>
    R foldr(const F &f, const R &init, const Seq<A> &as)
    {
        R result = init;

        for (int i = length(as) - 1; i > -1; --i)
        {
            result = f(as[i], result);
        }

        return result;
    }

    // FromFunctionReturnImpl

    template <typename N, typename F>
    struct FromFunctionReturnImpl
    {
    };

    template <int n, typename F>
    struct FromFunctionReturnImpl<IntegralConst<int, n>, F>
    {
        using Type = Sequence<CallReturn<F, int>, n, n>;
    };

    template <typename F>
    struct FromFunctionReturnImpl<int, F>
    {
        using Type = Sequence<CallReturn<F, int>, dyn, dyn>;
    };

    // FromFunctionReturn

    template <typename N, typename F>
    using FromFunctionReturn = typename FromFunctionReturnImpl<N, F>::Type;

    // template <typename N, typename F>
    // using FromFunctionReturn = Conditional<IsIntegralConst<N>::value,
    //                                        Sequence<CallReturn<F, int>, N::value, N::value>,
    //                                        Sequence<CallReturn<F, int>, dyn, dyn>>;

    // from_function

    template <typename N, typename F>
    auto from_function(const N &length, const F &f)
        -> FromFunctionReturn<N, F>
    {
        FromFunctionReturn<N, F> result{};
        if (FromFunctionReturn<N, F>::ct_len == dyn)
        {
            result.resize(length);
        }

        for (int i = 0; i < length; ++i)
        {
            result[i] = f(i);
        }

        return result;
    }

    // template <typename N, typename F>
    // auto from_function(const N &length, const F &f)
    //     -> EnableIf<IsIntegralConst<N>::value,
    //                 Sequence<CallReturn<F, int>, N::value, N::value>>
    // {
    //     Sequence<CallReturn<F, int>, N::value, N::value> result{};
    //     result.resize(length);

    //     for (int i = 0; i < length; ++i)
    //     {
    //         result[i] = f(i);
    //     }

    //     return result;
    // }

    // template <typename N, typename F>
    // auto from_function(const N &length, const F &f)
    //     -> EnableIf<!IsIntegralConst<N>::value,
    //                 Sequence<CallReturn<F, int>, dyn, dyn>>
    // {
    //     Sequence<CallReturn<F, int>, dyn, dyn> result{};
    //     result.resize(length);

    //     for (int i = 0; i < length; ++i)
    //     {
    //         result[i] = f(i);
    //     }

    //     return result;
    // }

    // for_index

    template <typename F = void (*)(const int &)>
    void for_index(const F &f, const int i)
    {
        for (int j = 0; j < i; ++j)
        {
            f(j);
        }
    }

    // for_each_with_index

    template <typename... Ts, typename F = void (*)(const int &, const Element<Ts> &...)>
    void for_each_with_index(const F &f, const Seq<Ts> &...seqs)
    {
        const auto min_length_ = min_length(seqs...);

        for (int i = 0; i < min_length_; ++i)
        {
            f(i, seqs[i]...);
        }
    }

    // for_each_with_indexi

    template <typename... Ts, typename F = void (*)(const int &, Element<Ts> &...)>
    void for_each_with_indexi(const F &f, Seq<Ts> &...seqs)
    {
        const int min_length_ = min_length(seqs...);

        for (int i = 0; i < min_length_; ++i)
        {
            f(i, seqs[i]...);
        }
    }

    // cartesian_for_each

    template <typename A, typename F = void (*)(const Element<A> &)>
    void cartesian_for_each(const F &f, const Seq<A> &as)
    {
        for_each(f, as);
    }

    template <typename A, typename... Ts, typename F = void (*)(const Element<A> &, const Element<Ts> &...)>
    void cartesian_for_each(const F &f, const Seq<A> &as, const Seq<Ts> &...seqs)
    {
        const auto as_length = length(as);

        for (int i = 0; i < as_length; ++i)
        {
            const auto a = as[i];
            const auto inner = [&](Element<Ts>... xs)
            { f(a, xs...); };

            cartesian_for_each<Ts..., decltype(inner)>(inner, seqs...);
        }
    }

    // cartesian_for_eachi

    template <typename A, typename F = void (*)(const Element<A> &)>
    void cartesian_for_eachi(const F &f, Seq<A> &as)
    {
        for_eachi(f, as);
    }

    template <typename A, typename... Ts, typename F = void (*)(const Element<A> &, const Element<Ts> &...)>
    void cartesian_for_eachi(const F &f, Seq<A> &as, Seq<Ts> &...seqs)
    {
        const auto as_length = length(as);

        for (int i = 0; i < as_length; ++i)
        {
            const auto a = as[i];
            const auto inner = [&](Element<Ts>... xs)
            { f(a, xs...); };

            cartesian_for_eachi<Ts..., decltype(inner)>(inner, seqs...);
        }
    }

    // MapWithIndexRetrun

    template <typename F, typename... Ts>
    using MapWithIndexRetrun = Sequence<
        CallReturn<F, int, Element<Ts>...>,
        AreAllStaticCapacity<Ts...>::value ? MinStaticCapacity<Ts...>::value : dyn,
        AreAllStaticLength<Ts...>::value ? MinStaticLength<Ts...>::value : dyn>;

    // map_with_index

    template <typename... Ts, typename F = void (*)(const int &, const Element<Ts> &...)>
    auto map_with_index(const F &f, const Seq<Ts> &...seqs)
        -> MapWithIndexRetrun<F, Ts...>
    {
        MapWithIndexRetrun<F, Ts...> result;
        const auto bounded_n = min_length(seqs...);

        if (MapWithIndexRetrun<F, Ts...>::ct_len == dyn)
        {
            result.resize(bounded_n);
        }

        for (int i = 0; i < bounded_n; ++i)
        {
            result[i] = f(i, seqs[i]...);
        }

        return result;
    }

    // CartesianMapReturn

    template <typename F, typename... Ts>
    using CartesianMapReturn = Sequence<
        CallReturn<F, Element<Ts>...>,
        AreAllStaticLength<Ts...>::value ? product_v(StaticLength<Ts>::value...) : dyn,
        AreAllStaticCapacity<Ts...>::value ? product_v(StaticCapacity<Ts>::value...) : dyn>;

    // cartesian_map

    // ! Maybe need to bechmark and optimize
    template <typename... Ts, typename F = void (*)(const Element<Ts> &...)>
    auto cartesian_map(const F &f, const Seq<Ts> &...seqs)
        -> CartesianMapReturn<F, Ts...>
    {
        CartesianMapReturn<F, Ts...> result;
        if (CartesianMapReturn<F, Ts...>::ct_len == dyn)
        {
            result.resize(product_v(static_cast<int>(length(seqs))...));
        }

        int i = 0;
        const auto inner = [&](Element<Ts>... xs)
        { result[i++] = f(xs...); };

        cartesian_for_each(inner, seqs...);

        return result;
    }

    // cartesian_for_index

    template <typename F = void (*)(const int &)>
    void cartesian_for_index(const F &f, const int &i)
    {
        for_index(f, i);
    }

    template <typename... Ints, typename F = void (*)(const int &)>
    void cartesian_for_index(const F &f, const int &i, const Ints &...is)
    {
        for (int i_ = 0; i_ < i; ++i_)
        {
            const auto inner = [&](const Ints &...is)
            { f(i_, is...); };

            cartesian_for_index(inner, is...);
        }
    }

    // // // todo begin

    // // // todo end

    // head

    // ! Partial function. Make sure non empty
    template <typename A>
    Element<A> head(const Seq<A> &as)
    {
        return as[0];
    }

    // TailReturn

    template <typename A, bool is_const>
    using TailReturn = EnableIf<A::ct_len != 0 && A::ct_cap != 0,
                                SequenceView<Conditional<is_const, const Element<A>, Element<A>>,
                                             IsStaticLength<A>::value ? A::ct_len - 1 : dyn,
                                             IsStaticCapacity<A>::value ? A::ct_cap - 1 : dyn>>;

    // tail
    // ! Partial function. Application on empty list is abortion.
    template <typename A>
    auto tail(const Seq<A> &as)
        -> TailReturn<A, true>
    {
        TailReturn<A, true> result{p_data(as) + 1};

        if (A::ct_len == dyn)
        {
            const auto as_length = length(as);
            if (as_length == 0)
            {
                abort();
            }

            result.resize(as_length - 1);
        }

        return result;
    }

    template <typename A>
    auto tail(Seq<A> &as)
        -> TailReturn<A, false>
    {
        TailReturn<A, false> result{p_data(as) + 1};

        if (A::ct_len == dyn)
        {
            const auto as_length = length(as);
            if (as_length == 0)
            {
                abort();
            }

            result.resize(as_length - 1);
        }

        return result;
    }

    // InitReturn

    template <typename A, bool is_const>
    using InitReturn = EnableIf<A::ct_len != 0 && A::ct_cap != 0,
                                SequenceView<Conditional<is_const, const Element<A>, Element<A>>,
                                             IsStaticLength<A>::value ? A::ct_len - 1 : dyn,
                                             IsStaticCapacity<A>::value ? A::ct_cap - 1 : dyn>>;

    // init

    // ! Zero length input will envoke abort.
    template <typename A>
    auto init(const Seq<A> &as)
        -> InitReturn<A, true>
    {
        InitReturn<A, true> result{p_data(as)};

        if (A::ct_len == dyn)
        {
            const auto as_length = length(as);
            if (as_length == 0)
            {
                abort();
            }

            result.resize(as_length - 1);
        }

        return result;
    }

    // ! Partial function. Make sure non empty
    template <typename A>
    auto init(Seq<A> &as)
        -> InitReturn<A, false>
    {
        InitReturn<A, false> result{p_data(as)};

        if (A::ct_len == dyn)
        {
            const auto as_length = length(as);
            if (as_length == 0)
            {
                abort();
            }

            result.resize(as_length - 1);
        }

        return result;
    }

    // last

    // ! Partial function. Make sure non
    template <typename A>
    Element<A> last(const Seq<A> &as)
    {
        return as[length(as) - 1];
    }

    // todo is_null

    // TakeReturnImpl
    template <typename N, typename A, bool is_const>
    struct TakeReturnImpl
    {
    };

    template <int n, typename A, bool is_const>
    struct TakeReturnImpl<IntegralConst<int, n>, A, is_const>
    {
        using Type = Conditional<
            IsStaticLength<A>::value,
            SequenceView<Conditional<is_const, const Element<A>, Element<A>>, bound_v(0, A::ct_len, n), bound_v(0, A::ct_len, n)>,
            Conditional<
                IsStaticCapacity<A>::value,
                SequenceView<Conditional<is_const, const Element<A>, Element<A>>, dyn, A::ct_cap>,
                SequenceView<Conditional<is_const, const Element<A>, Element<A>>, dyn, dyn>>>;
    };

    template <typename A, bool is_const>
    struct TakeReturnImpl<int, A, is_const>
    {
        using Type = Conditional<
            IsStaticCapacity<A>::value,
            SequenceView<Conditional<is_const, const Element<A>, Element<A>>, dyn, A::ct_cap>,
            SequenceView<Conditional<is_const, const Element<A>, Element<A>>, dyn, dyn>>;
    };

    // TakeReturn

    template <typename N, typename A, bool is_const>
    using TakeReturn = typename TakeReturnImpl<N, A, is_const>::Type;

    // take

    template <typename N, typename A>
    auto take(const N &n, const Seq<A> &as)
        -> TakeReturn<N, A, true>
    {
        TakeReturn<N, A, true> result{p_data(as)};

        if (TakeReturn<N, A, true>::ct_len == dyn)
        {
            result.resize(bound_v(0, length(as), n));
        }

        return result;
    }

    template <typename N, typename A>
    auto take(const N &n, Seq<A> &as)
        -> TakeReturn<N, A, false>
    {
        TakeReturn<N, A, false> result{p_data(as)};

        if (TakeReturn<N, A, false>::ct_len == dyn)
        {
            result.resize(bound_v(0, length(as), n));
        }

        return result;
    }

    // DropReturnImpl

    template <typename N, typename A, bool is_const>
    struct DropReturnImpl
    {
    };

    template <int n, typename A, bool is_const>
    struct DropReturnImpl<IntegralConst<int, n>, A, is_const>
    {
        using Type = Conditional<
            IsStaticLength<A>::value,
            SequenceView<Conditional<is_const, const Element<A>, Element<A>>, bound_v(0, A::ct_len, A::ct_len - n), bound_v(0, A::ct_len, A::ct_len - n)>,
            Conditional<
                IsStaticCapacity<A>::value,
                SequenceView<Conditional<is_const, const Element<A>, Element<A>>, dyn, A::ct_cap>,
                SequenceView<Conditional<is_const, const Element<A>, Element<A>>, dyn, dyn>>>;
    };

    template <typename A, bool is_const>
    struct DropReturnImpl<int, A, is_const>
    {
        using Type = Conditional<
            IsStaticCapacity<A>::value,
            SequenceView<Conditional<is_const, const Element<A>, Element<A>>, dyn, A::ct_cap>,
            SequenceView<Conditional<is_const, const Element<A>, Element<A>>, dyn, dyn>>;
    };

    // DropReturn

    template <typename N, typename A, bool is_const>
    using DropReturn = typename DropReturnImpl<N, A, is_const>::Type;

    // drop

    template <typename N, typename A>
    auto drop(const N &n, const Seq<A> &as)
        -> DropReturn<N, A, true>
    {
        const auto as_length = length(as);
        const auto bounded_n = bound_v(0, as_length, n);
        DropReturn<N, A, true> result{p_data(as) + bounded_n};

        if (DropReturn<N, A, true>::ct_len == dyn)
        {
            result.resize(as_length - bounded_n);
        }

        return result;
    }

    template <typename N, typename A>
    auto drop(const N &n, Seq<A> &as)
        -> DropReturn<N, A, false>
    {
        const auto as_length = length(as);
        const auto bounded_n = bound_v(0, as_length, n);
        DropReturn<N, A, false> result{p_data(as) + bounded_n};

        if (DropReturn<N, A, false>::ct_len == dyn)
        {
            result.resize(as_length - bounded_n);
        }

        return result;
    }

    // elem

    template <typename A>
    bool elem(const Element<A> &a, const Seq<A> &as)
    {
        const int as_length = length(as);

        for (int i = 0; i < as_length; ++i)
        {
            if (as[i] == a)
            {
                return true;
            }
        }

        return false;
    }

    // elem_index

    template <typename A>
    Maybe<int> elem_index(const Element<A> &a, const Seq<A> &as)
    {
        const int as_length = length(as);

        for (int i = 0; i < as_length; ++i)
        {
            if (as[i] == a)
            {
                return i;
            }
        }

        return Nothing{};
    }

    // IndicesReturn_t

    template <typename SeqA>
    using IndicesReturn_t =
        Conditional<
            IsStaticCapacity<SeqA>::value,
            ArrVec<int, StaticCapacity<SeqA>::value>,
            Vector<int>>;

    template <typename A>
    using IndicesReturn = Conditional<
        IsStaticCapacity<A>::value,
        Sequence<int, dyn, A::ct_cap>,
        Sequence<int, dyn, dyn>>;

    // elem_indices

    template <typename A>
    auto elem_indices(const Element<A> &a, const Seq<A> &as)
        -> IndicesReturn<A>
    {
        IndicesReturn<A> result{};
        const auto as_length = length(as);

        for (int i = 0; i < as_length; ++i)
        {
            if (a == as[i])
            {
                result.push_back(i);
            }
        }

        return result;
    }

    // find

    template <typename A, typename F = void (*)(const Element<A> &)>
    bool find(const F &f, const Seq<A> &as)
    {
        const auto as_length = length(as);

        for (int i = 0; i < as_length; ++i)
        {
            if (f(as[i]))
            {
                return true;
            }
        }

        return false;
    }

    // // find_index

    // template <typename SeqA, typename F = void (*)(const Element<SeqA> &)>
    // auto find_index(const F &f, const SeqA &as)
    //     -> Maybe<int>
    // {
    //     for (int i = 0; i < length(as); ++i)
    //     {
    //         if (f(as[i]) == true)
    //         {
    //             return i;
    //         }
    //     }

    //     return Nothing{};
    // }

    // // find_indices

    // template <typename SeqA, typename F = void (*)(const Element<SeqA> &)>
    // auto find_indices(const F &f, const SeqA &as)
    //     -> EnableIf<
    //         IsStaticCapacity<SeqA>::value,
    //         IndicesReturn_t<SeqA>>
    // {
    //     const int length_as = length(as);
    //     IndicesReturn_t<SeqA> result;

    //     for (int i = 0; i < length_as; ++i)
    //     {
    //         if (f(as[i]))
    //         {
    //             result.push_back(i);
    //         }
    //     }

    //     return result;
    // }

    // template <typename SeqA, typename F = void (*)(const Element<SeqA> &)>
    // auto find_indices(const F &f, const SeqA &as)
    //     -> EnableIf<
    //         !IsStaticCapacity<SeqA>::value,
    //         IndicesReturn_t<SeqA>>
    // {
    //     const int as_length = length(as);

    //     IndicesReturn_t<SeqA> result;
    //     result.reserve(as_length);

    //     for (int i = 0; i < as_length; ++i)
    //     {
    //         if (f(as[i]))
    //         {
    //             result.push_back(i);
    //         }
    //     }

    //     return result;
    // }
}
#endif