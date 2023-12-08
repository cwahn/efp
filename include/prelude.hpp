#ifndef PRELUDE_HPP_
#define PRELUDE_HPP_

#include "sfinae.hpp"
#include "enum_type.hpp"
#include "maybe.hpp"
#include "sequence.hpp"

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

    // Composed

    template <typename... Fs>
    struct Composed
    {
    };

    template <typename F>
    struct Composed<F>
    {
        const F &f;

        Composed(const F &f) : f(f) {}

        template <typename... Arg>
        inline auto operator()(Arg &&...arg) const
            -> decltype(f(forward<Arg>(arg)...))
        {
            return f(forward<Arg>(arg)...);
        }
    };

    template <typename F, typename... Fs>
    struct Composed<F, Fs...>
    {
        const F &f;
        const Composed<Fs...> fs;

        Composed(const F &f, const Fs &...fs) : f(f), fs(Composed<Fs...>(fs...)) {}

        template <typename... Arg>
        inline auto operator()(Arg &&...arg) const
            -> decltype(f(fs(forward<Arg>(arg)...)))
        {
            return f(fs(forward<Arg>(arg)...));
        }
    };

    // compose

    template <typename F, typename G>
    auto compose(const F &f, const G &g) -> Composed<F, G>
    {
        return Composed<F, G>(f, g);
    }

    template <typename F, typename... Fs>
    auto compose(const F &f, const Fs &...fs) -> Composed<F, Fs...>
    {
        return Composed<F, Fs...>(f, fs...);
    }

    // // template <typename F, typename G>
    // // inline auto operator*(F f, G g) -> Composed<F, G>
    // // {
    // //     return compose(f, g);
    // // }

    // execute_pack

    template <typename... Args>
    void execute_pack(Args... args) {}

    // AppendReturn

    template <typename... As>
    using AppendReturn = Conditional<
        AreAllStaticLength<As...>::value,
        Sequence<Common<Element<As>...>, sum_v(As::ct_len...), sum_v(As::ct_len...)>,
        Conditional<
            AreAllStaticCapacity<As...>::value,
            Sequence<Common<Element<As>...>, dyn, sum_v(As::ct_cap...)>,
            Sequence<Common<Element<As>...>, dyn, dyn>>>;

    // append

    template <typename A, typename B>
    Unit append_impl(size_t &idx, Seq<A> &result, const Seq<B> &seq)
    {
        const auto seq_length = length(seq);

        for (size_t i = 0; i < seq_length; ++i)
        {
            result[idx] = seq[i];
            idx++;
        }

        return unit;
    }

    template <typename H, typename... As>
    auto append(const Seq<H> &head, const Seq<As> &...tail)
        -> AppendReturn<H, As...>
    {
        AppendReturn<H, As...> result{};

        if (AppendReturn<H, As...>::ct_len == dyn)
        {
            result.resize(sum_v(static_cast<size_t>(length(head)), length(tail)...));
        }

        size_t idx{0};
        execute_pack(append_impl(idx, result, head),
                     append_impl(idx, result, tail)...);

        return result;
    }

    // min_length

    template <typename A, typename... As>
    size_t min_length(const Seq<A> &as, const Seq<As> &...seqs)
    {
        return minimum_v(static_cast<size_t>(length(as)), length(seqs)...);
    }

    // for_each

    template <typename... As, typename F = void (*)(const Element<As> &...)>
    void for_each(const F &f, const Seq<As> &...seqs)
    {
        const size_t seq_length = min_length(seqs...);

        for (size_t i = 0; i < seq_length; ++i)
        {
            f(seqs[i]...);
        }
    }

    // for_eachi

    template <typename... As, typename F = void (*)(Element<As> &...)>
    void for_eachi(const F &f, Seq<As> &...seqs)
    {
        const size_t seq_length = min_length(seqs...);

        for (size_t i = 0; i < seq_length; ++i)
        {
            f(seqs[i]...);
        }
    }

    // NAryReturn

    template <typename R, typename... As>
    using NAryReturn = Sequence<
        R,
        AreAllStaticLength<As...>::value ? MinStaticLength<As...>::value : dyn,
        AreAllStaticCapacity<As...>::value ? MinStaticCapacity<As...>::value : dyn>;

    // MapReturn

    template <typename F, typename... As>
    using MapReturn = NAryReturn<CallReturn<F, Element<As>...>, As...>;

    // map

    template <typename F, typename... As>
    auto map(const F &f, const Seq<As> &...seqs)
        -> MapReturn<F, As...>
    {
        MapReturn<F, As...> result{};
        const size_t bounded_n = min_length(seqs...);
        if (MapReturn<F, As...>::ct_len == dyn)
        {
            result.resize(bounded_n);
        }

        for (size_t i = 0; i < bounded_n; ++i)
        {
            result[i] = f(seqs[i]...);
        }

        return result;
    }

    // FilterReturn

    template <typename A>
    using FilterReturn = Sequence<Element<A>, dyn, A::ct_cap>;

    // filter

    template <typename A, typename F = bool (*)(Element<A> &)>
    auto filter(const F &f, const Seq<A> &as)
        -> FilterReturn<A>
    {
        FilterReturn<A> result{};
        const auto input_length = length(as);

        for (size_t i = 0; i < input_length; ++i)
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

        for (size_t i = 0; i < length(as); ++i)
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

        for (size_t i = length(as) - 1; i != -1; --i)
        {
            result = f(as[i], result);
        }

        return result;
    }

    // todo size_t
    // FromFunctionReturnImpl

    template <typename N, typename F>
    struct FromFunctionReturnImpl
    {
        using Type = Sequence<CallReturn<F, N>, dyn, dyn>;
    };

    template <size_t n, typename F>
    struct FromFunctionReturnImpl<Size<n>, F>
    {
        using Type = Sequence<CallReturn<F, size_t>, n, n>;
    };

    // template <typename F>
    // struct FromFunctionReturnImpl<size_t, F>
    // {
    //     using Type = Sequence<CallReturn<F, size_t>, dyn, dyn>;
    // };

    // FromFunctionReturn

    template <typename N, typename F>
    using FromFunctionReturn = typename FromFunctionReturnImpl<N, F>::Type;

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

        for (size_t i = 0; i < length; ++i)
        {
            result[i] = f(i);
        }

        return result;
    }

    // for_index

    template <typename F = void (*)(size_t)>
    void for_index(const F &f, const size_t i)
    {
        for (size_t j = 0; j < i; ++j)
        {
            f(j);
        }
    }

    // for_each_with_index

    template <typename... As, typename F = void (*)(size_t, const Element<As> &...)>
    void for_each_with_index(const F &f, const Seq<As> &...seqs)
    {
        const auto min_length_ = min_length(seqs...);

        for (size_t i = 0; i < min_length_; ++i)
        {
            f(i, seqs[i]...);
        }
    }

    // for_each_with_indexi

    template <typename... As, typename F = void (*)(size_t, Element<As> &...)>
    void for_each_with_indexi(const F &f, Seq<As> &...seqs)
    {
        const size_t min_length_ = min_length(seqs...);

        for (size_t i = 0; i < min_length_; ++i)
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

    template <typename A, typename... As, typename F = void (*)(const Element<A> &, const Element<As> &...)>
    void cartesian_for_each(const F &f, const Seq<A> &as, const Seq<As> &...seqs)
    {
        const auto as_length = length(as);

        for (size_t i = 0; i < as_length; ++i)
        {
            const auto a = as[i];
            const auto inner = [&](Element<As>... xs)
            { f(a, xs...); };

            cartesian_for_each<As..., decltype(inner)>(inner, seqs...);
        }
    }

    // cartesian_for_eachi

    template <typename A, typename F = void (*)(const Element<A> &)>
    void cartesian_for_eachi(const F &f, Seq<A> &as)
    {
        for_eachi(f, as);
    }

    template <typename A, typename... As, typename F = void (*)(const Element<A> &, const Element<As> &...)>
    void cartesian_for_eachi(const F &f, Seq<A> &as, Seq<As> &...seqs)
    {
        const auto as_length = length(as);

        for (size_t i = 0; i < as_length; ++i)
        {
            const auto a = as[i];
            const auto inner = [&](Element<As>... xs)
            { f(a, xs...); };

            cartesian_for_eachi<As..., decltype(inner)>(inner, seqs...);
        }
    }

    // MapWithIndexRetrun

    template <typename F, typename... As>
    using MapWithIndexRetrun = Sequence<
        CallReturn<F, size_t, Element<As>...>,
        AreAllStaticCapacity<As...>::value ? MinStaticCapacity<As...>::value : dyn,
        AreAllStaticLength<As...>::value ? MinStaticLength<As...>::value : dyn>;

    // map_with_index

    template <typename... As, typename F = void (*)(size_t, const Element<As> &...)>
    auto map_with_index(const F &f, const Seq<As> &...seqs)
        -> MapWithIndexRetrun<F, As...>
    {
        MapWithIndexRetrun<F, As...> result;
        const auto bounded_n = min_length(seqs...);

        if (MapWithIndexRetrun<F, As...>::ct_len == dyn)
        {
            result.resize(bounded_n);
        }

        for (size_t i = 0; i < bounded_n; ++i)
        {
            result[i] = f(i, seqs[i]...);
        }

        return result;
    }

    // CartesianMapReturn

    template <typename F, typename... As>
    using CartesianMapReturn = Sequence<
        CallReturn<F, Element<As>...>,
        AreAllStaticLength<As...>::value ? product_v(StaticLength<As>::value...) : dyn,
        AreAllStaticCapacity<As...>::value ? product_v(StaticCapacity<As>::value...) : dyn>;

    // cartesian_map

    // ! Maybe need to bechmark and optimize
    template <typename... As, typename F = void (*)(const Element<As> &...)>
    auto cartesian_map(const F &f, const Seq<As> &...seqs)
        -> CartesianMapReturn<F, As...>
    {
        CartesianMapReturn<F, As...> result;
        if (CartesianMapReturn<F, As...>::ct_len == dyn)
        {
            result.resize(product_v(static_cast<size_t>(length(seqs))...));
        }

        size_t i = 0;
        const auto inner = [&](Element<As>... xs)
        { result[i++] = f(xs...); };

        cartesian_for_each(inner, seqs...);

        return result;
    }

    // cartesian_for_index

    template <typename F = void (*)(size_t)>
    void cartesian_for_index(const F &f, size_t i)
    {
        for_index(f, i);
    }

    template <typename... Ints, typename F = void (*)(size_t)>
    void cartesian_for_index(const F &f, size_t i, const Ints &...is)
    {
        for (size_t i_ = 0; i_ < i; ++i_)
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
        return TailReturn<A, true>{data(as) + 1, length(as) - Size<1>{}};
    }

    template <typename A>
    auto tail(Seq<A> &as)
        -> TailReturn<A, false>
    {
        return TailReturn<A, false>{data(as) + 1, length(as) - Size<1>{}};
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
        return InitReturn<A, true>{data(as), length(as) - Size<1>{}};
    }

    // ! Partial function. Make sure non empty
    template <typename A>
    auto init(Seq<A> &as)
        -> InitReturn<A, false>
    {
        return InitReturn<A, false>{data(as), length(as) - Size<1>{}};
    }

    // last

    // ! Partial function. Make sure non
    template <typename A>
    Element<A> last(const Seq<A> &as)
    {
        return as[length(as) - 1];
    }

    // is_null

    template <typename A>
    bool is_null(const Seq<A> &as)
    {
        return length(as) == 0;
    }

    // TakeReturnImpl

    template <typename N, typename A, bool is_const>
    struct TakeReturnImpl
    {
        using Type = Conditional<
            IsStaticCapacity<A>::value,
            SequenceView<Conditional<is_const, const Element<A>, Element<A>>, dyn, A::ct_cap>,
            SequenceView<Conditional<is_const, const Element<A>, Element<A>>, dyn, dyn>>;
    };

    template <size_t n, typename A, bool is_const>
    struct TakeReturnImpl<Size<n>, A, is_const>
    {
        using Type = Conditional<
            IsStaticLength<A>::value,
            SequenceView<Conditional<is_const, const Element<A>, Element<A>>, n, n>,
            Conditional<
                IsStaticCapacity<A>::value,
                SequenceView<Conditional<is_const, const Element<A>, Element<A>>, dyn, A::ct_cap>,
                SequenceView<Conditional<is_const, const Element<A>, Element<A>>, dyn, dyn>>>;
    };

    // TakeReturn

    template <typename N, typename A, bool is_const>
    using TakeReturn = typename TakeReturnImpl<N, A, is_const>::Type;

    // take

    // !Should not put n longer than the length. Check should be done by the caller
    template <typename N, typename A>
    auto take(N n, const Seq<A> &as)
        -> TakeReturn<N, A, true>
    {
        return TakeReturn<N, A, true>(data(as), n);
    }

    template <typename N, typename A>
    auto take(N n, Seq<A> &as)
        -> TakeReturn<N, A, false>
    {

        return TakeReturn<N, A, true>(data(as), n);
    }

    // DropReturnImpl

    template <typename N, typename A, bool is_const>
    struct DropReturnImpl
    {
        using Type = Conditional<
            IsStaticCapacity<A>::value,
            SequenceView<Conditional<is_const, const Element<A>, Element<A>>, dyn, A::ct_cap>,
            SequenceView<Conditional<is_const, const Element<A>, Element<A>>, dyn, dyn>>;
    };

    template <size_t n, typename A, bool is_const>
    struct DropReturnImpl<Size<n>, A, is_const>
    {
        using Type = Conditional<
            IsStaticLength<A>::value,
            SequenceView<Conditional<is_const, const Element<A>, Element<A>>, Size<n>{}, Size<n>{}>,
            Conditional<
                IsStaticCapacity<A>::value,
                SequenceView<Conditional<is_const, const Element<A>, Element<A>>, dyn, A::ct_cap>,
                SequenceView<Conditional<is_const, const Element<A>, Element<A>>, dyn, dyn>>>;
    };

    // DropReturn

    template <typename N, typename A, bool is_const>
    using DropReturn = typename DropReturnImpl<N, A, is_const>::Type;

    // drop

    template <typename N, typename A>
    auto drop(N n, const Seq<A> &as)
        -> DropReturn<N, A, true>
    {
        return DropReturn<N, A, true>(data(as) + n, length(as) - n);
    }

    template <typename N, typename A>
    auto drop(N n, Seq<A> &as)
        -> DropReturn<N, A, false>
    {
        return DropReturn<N, A, false>(data(as) + n, length(as) - n);
    }

    // SliceReturn

    template <typename S, typename E, typename A, bool is_const>
    using SliceReturn = TakeReturn<decltype(efp::declval<E>() - efp::declval<S>()), DropReturn<S, A, is_const>, is_const>;

    // slice

    // todo Optimization
    template <typename S, typename E, typename A>
    auto slice(S start, E end, const Seq<A> &as)
        -> SliceReturn<S, E, A, true>
    {
        return SliceReturn<S, E, A, true>(data(as) + start, end - start);
    }

    template <typename S, typename E, typename A>
    auto slice(S start, E end, Seq<A> &as)
        -> SliceReturn<S, E, A, false>
    {
        return SliceReturn<S, E, A, false>(data(as) + start, end - start);
    }

    // todo Add test for take_while and drop_while

    // take_while

    template <typename A>
    using TakeWhileReturn = FilterReturn<A>;

    template <typename A, typename F = bool (*)(const Element<A> &)>
    auto take_while(const F &f, const A &as)
        -> TakeWhileReturn<A>
    {
        TakeWhileReturn<A> result{};
        const auto input_length = length(as);

        size_t i = 0;
        while (i < input_length && f(as[i]))
        {
            ++i;
        }

        return take(i, as);
    }

    // drop_while

    template <typename A>
    using DropWhileReturn = FilterReturn<A>;

    template <typename A, typename F = bool (*)(const Element<A> &)>
    auto drop_while(const F &f, const A &as)
        -> DropWhileReturn<A>
    {
        DropWhileReturn<A> result{};
        const auto input_length = length(as);

        size_t i = 0;
        while (i < input_length && f(as[i]))
        {
            ++i;
        }

        return drop(i, as);
    }

    // elem

    template <typename A>
    bool elem(const Element<A> &a, const Seq<A> &as)
    {
        const auto as_length = length(as);

        for (size_t i = 0; i < as_length; ++i)
        {
            if (as[i] == a)
                return true;
        }

        return false;
    }

    // elem_index

    template <typename A>
    Maybe<size_t> elem_index(const Element<A> &a, const Seq<A> &as)
    {
        const auto as_length = length(as);

        for (size_t i = 0; i < as_length; ++i)
        {
            if (as[i] == a)
                return i;
        }

        return nothing;
    }

    // ElemIndicesReturn

    template <typename A>
    using ElemIndicesReturn = Conditional<
        IsStaticCapacity<A>::value,
        Sequence<size_t, dyn, A::ct_cap>,
        Sequence<size_t, dyn, dyn>>;

    // elem_indices

    template <typename A>
    auto elem_indices(const Element<A> &a, const Seq<A> &as)
        -> ElemIndicesReturn<A>
    {
        ElemIndicesReturn<A> result{};
        const auto as_length = length(as);

        for (size_t i = 0; i < as_length; ++i)
        {
            if (a == as[i])
                result.push_back(i);
        }

        return result;
    }

    // find

    template <typename A, typename F = void (*)(const Element<A> &)>
    bool find(const F &f, const Seq<A> &as)
    {
        const auto as_length = length(as);

        for (size_t i = 0; i < as_length; ++i)
        {
            if (f(as[i]))
                return true;
        }

        return false;
    }

    // find_index

    template <typename A, typename F = void (*)(const Element<A> &)>
    auto find_index(const F &f, const Seq<A> &as)
        -> Maybe<size_t>
    {
        const auto as_length = length(as);

        for (size_t i = 0; i < as_length; ++i)
        {
            if (f(as[i]))
                return i;
        }

        return nothing;
    }

    // FindIndicesReturn

    template <typename A>
    using FindIndicesReturn = Conditional<
        IsStaticCapacity<A>::value,
        Sequence<size_t, dyn, A::ct_cap>,
        Sequence<size_t, dyn, dyn>>;

    // find_indices

    template <typename A, typename F = void (*)(const Element<A> &)>
    auto find_indices(const F &f, const A &as)
        -> FindIndicesReturn<A>
    {
        FindIndicesReturn<A> result{};
        const auto as_length = length(as);

        for (size_t i = 0; i < as_length; ++i)
        {
            if (f(as[i]))
                result.push_back(i);
        }

        return result;
    }
}
#endif