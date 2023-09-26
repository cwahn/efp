#ifndef CRTP_PRELUDE_HPP_
#define CRTP_PRELUDE_HPP_

#include <array>
#include <vector>
#include <tuple>
#include <optional>

#include "sfinae.hpp"
#include "enum_type.hpp"
#include "crtp_seq.hpp"
#include "maybe.hpp"

// Trust compiler for length calculation

namespace efp
{
    // struct Unit
    // {
    // };

    template <typename A>
    A id(const A &a)
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
    // using AppendReturn_t = AppendSequence_t<Element_t<Head>, Head, Tail...>;

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
    //     const size_t result_length = sum_v((int)length(head), (int)length(tail)...);

    //     AppendReturn_t<Head, Tail...> result(result_length);
    //     int idx{0};

    //     execute_pack(append_(result, idx, head), append_(result, idx, tail)...);

    //     return result;
    // }

    // // ct_length

    // template <typename Seq<A>>
    // constexpr int ct_length(const Seq<A> &as)
    // {
    //     return A::ct_len;
    // }

    // // length

    // template <typename Seq<A>>
    // int length(const Seq<A> &as)
    // {
    //     return as.length();
    // }

    // min_length

    // template <typename A>
    // int min_length(const Seq<A> &as)
    // {
    //     return length(as);
    // }

    // template <typename Head, typename... Tail>
    // int min_length(const Head &head, const Tail &...tail)
    // {
    //     return length(head) < min_length(tail...) ? length(head) : min_length(tail...);
    // }

    template <typename A, typename... Ts>
    int min_length(const Seq<A> &as, const Seq<Ts> &...seqs)
    {
        return minimum_v(static_cast<int>(length(as)), length(seqs)...);
    }

    // // ct_min_length

    // template <typename A>
    // constexpr int ct_min_length(const Seq<A> &as)
    // {
    //     return ct_length(as);
    // }

    // template <typename Head, typename... Tail>
    // constexpr int ct_min_length(const Head &head, const Tail &...tail)
    // {
    //     return ct_length(head) < ct_min_length(tail...) ? ct_length(head) : ct_min_length(tail...);
    // }

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
        const int result_length = min_length(seqs...);
        result.resize(result_length);

        for (int i = 0; i < result_length; ++i)
        {
            result[i] = f(seqs[i]...);
        }

        return result;
    }

    // MapWithIndexReturn_t

    // template <typename F, typename... Seqs>
    // using MapWithIndexReturn_t =
    //     MapSequence_t<CallReturn<F, int, Element_t<Seqs>...>, Seqs...>;

    // FilterReturn

    template <typename F, typename A>
    using FilterReturn = Sequence<Element<A>, dyn, A::ct_cap>;

    // filter

    template <typename A, typename F = bool (*)(Element<A> &)>
    auto filter(const F &f, const Seq<A> &as)
        -> FilterReturn<F, A>
    {
        FilterReturn<F, A> result{};
        // ! Can't determine the length in advance
        const auto input_length = length(as);

        int j = 0;
        for (int i = 0; i < input_length; ++i)
        {
            const auto a = as[i];
            if (f(a))
            {
                result[j] = a;
                ++j;
            }
        }
        result.resize(j);

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

    // // FromFunctionReturn_t

    // template <typename N, typename F>
    // using FromFunctionReturn_t = Conditional<
    //     IsIntegralConst<N>::value,
    //     Array<CallReturn<F, int>, N::value>,
    //     Vector<CallReturn<F, int>>>;

    // FromFunctionReturn

    // using FromFunctionReturn = Sequence<CallReturn<F, int>,
    //                                     IsIntegralConst<N>::value ? N::value : dyn,
    //                                     IsIntegralConst<N>::value ? N::value : dyn>;

    template <typename N, typename F>
    using FromFunctionReturn = Conditional<IsIntegralConst<N>::value,
                                           Sequence<CallReturn<F, int>, N::value, N::value>,
                                           Sequence<CallReturn<F, int>, dyn, dyn>>;

    // from_function

    template <typename N, typename F>
    auto from_function(const N &length, const F &f)
        -> EnableIf<IsIntegralConst<N>::value,
                    Sequence<CallReturn<F, int>, N::value, N::value>>
    {
        Sequence<CallReturn<F, int>, N::value, N::value> result{};
        result.resize(length);

        for (int i = 0; i < length; ++i)
        {
            result[i] = f(i);
        }

        return result;
    }

    template <typename N, typename F>
    auto from_function(const N &length, const F &f)
        -> EnableIf<!IsIntegralConst<N>::value,
                    Sequence<CallReturn<F, int>, dyn, dyn>>
    {
        Sequence<CallReturn<F, int>, dyn, dyn> result{};
        result.resize(length);

        for (int i = 0; i < length; ++i)
        {
            result[i] = f(i);
        }

        return result;
    }

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

    // // cartesian_for_eachi

    // template <typename SeqA, typename F = void (*)(Element_t<SeqA> &)>
    // void cartesian_for_eachi(const F &f, SeqA &as)
    // {
    //     for_eachi(f, as);
    // }

    // template <typename SeqA, typename... Seqs, typename F = void (*)(Element_t<SeqA> &, Element_t<Seqs> &...)>
    // void cartesian_for_eachi(const F &f, SeqA &as, Seqs &...seqs)
    // {
    //     // ? Will it be optimized out to a compile time constatnt?
    //     const int as_length = length(as);
    //     Element_t<SeqA> a;

    //     for (int i = 0; i < as_length; ++i)
    //     {
    //         a = as[i];
    //         const auto inner = [=](Element_t<Seqs>... xs)
    //         {
    //             f(a, xs...);
    //         };

    //         cartesian_for_eachi<decltype(inner), Seqs...>(inner, seqs...);
    //     }
    // }

    // // map_with_index

    // template <typename... Seqs, typename F = void (*)(const int &, const Element_t<Seqs> &...)>
    // auto map_with_index(const F &f, const Seqs &...seqs)
    //     -> EnableIf<
    //         all_v(IsStaticCapacity<Seqs>::value...) && all_v(IsStaticLength<Seqs>::value...),
    //         Array<CallReturn<F, int, Element_t<Seqs>...>, MinStaticCapacity<Seqs...>::value>>
    // {
    //     using R = CallReturn<F, int, Element_t<Seqs>...>;

    //     Array<R, MinStaticCapacity<Seqs...>::value> result;

    //     for (int i = 0; i < MinStaticCapacity<Seqs...>::value; ++i)
    //     {
    //         result[i] = f(i, seqs[i]...);
    //     }

    //     return result;
    // }

    // template <typename... Seqs, typename F = void (*)(const int &, const Element_t<Seqs> &...)>
    // auto map_with_index(const F &f, const Seqs &...seqs)
    //     -> EnableIf<
    //         all_v(IsStaticCapacity<Seqs>::value...) && !all_v(IsStaticLength<Seqs>::value...),
    //         ArrVec<CallReturn<F, int, Element_t<Seqs>...>, MinStaticCapacity<Seqs...>::value>>
    // {
    //     using R = CallReturn<F, int, Element_t<Seqs>...>;

    //     const int result_length = min_length(seqs...);

    //     ArrVec<R, MinStaticCapacity<Seqs...>::value> result(result_length);

    //     for (int i = 0; i < result_length; ++i)
    //     {
    //         result[i] = f(i, seqs[i]...);
    //     }

    //     return result;
    // }

    // template <typename... Seqs, typename F = void (*)(const int &, const Element_t<Seqs> &...)>
    // auto map_with_index(const F &f, const Seqs &...seqs)
    //     -> EnableIf<
    //         !all_v(IsStaticCapacity<Seqs>::value...),
    //         Vector<CallReturn<F, int, Element_t<Seqs>...>>>
    // {
    //     using R = CallReturn<F, int, Element_t<Seqs>...>;

    //     const int result_length = min_length(seqs...);

    //     Vector<R> result(result_length);

    //     for (int i = 0; i < result_length; ++i)
    //     {
    //         result[i] = f(i, seqs[i]...);
    //     }

    //     return result;
    // }

    // // cartesian_map
    // // ! Maybe need to bechmark and optimize
    // template <typename... Seqs, typename F = void (*)(const Element_t<Seqs> &...)>
    // auto cartesian_map(const F &f, const Seqs &...seqs)
    //     -> EnableIf<
    //         all_v(IsStaticCapacity<Seqs>::value...) && all_v(IsStaticLength<Seqs>::value...),
    //         Array<CallReturn<F, Element_t<Seqs>...>, StaticCapacityProduct<Seqs...>::value>>
    // {
    //     using R = CallReturn<F, Element_t<Seqs>...>;

    //     Array<R, StaticCapacityProduct<Seqs...>::value> result;
    //     int i = 0;

    //     const auto inner = [&](Element_t<Seqs>... xs)
    //     {
    //         result[i++] = f(xs...);
    //     };

    //     cartesian_for_each(inner, seqs...);

    //     return result;
    // }

    // // todo Make both case as one.
    // template <typename... Seqs, typename F = void (*)(const Element_t<Seqs> &...)>
    // auto cartesian_map(const F &f, const Seqs &...seqs)
    //     -> EnableIf<
    //         all_v(IsStaticCapacity<Seqs>::value...) && !all_v(IsStaticLength<Seqs>::value...),
    //         ArrVec<CallReturn<F, Element_t<Seqs>...>, StaticCapacityProduct<Seqs...>::value>>
    // {
    //     using R = CallReturn<F, Element_t<Seqs>...>;

    //     const int result_length = size_v_product(length(seqs)...);

    //     ArrVec<R, StaticCapacityProduct<Seqs...>::value> result(result_length);
    //     int i = 0;

    //     const auto inner = [&](Element_t<Seqs>... xs)
    //     {
    //         result[i++] = f(xs...);
    //     };

    //     cartesian_for_each(inner, seqs...);

    //     return result;
    // }

    // template <typename... Seqs, typename F = void (*)(const Element_t<Seqs> &...)>
    // auto cartesian_map(const F &f, const Seqs &...seqs)
    //     -> EnableIf<
    //         !all_v(IsStaticCapacity<Seqs>::value...),
    //         Vector<CallReturn<F, Element_t<Seqs>...>>>
    // {
    //     using R = CallReturn<F, Element_t<Seqs>...>;

    //     const int result_length = size_v_product(length(seqs)...);

    //     Vector<R> result(result_length);
    //     int i = 0;

    //     const auto inner = [&](Element_t<Seqs>... xs)
    //     {
    //         result[i++] = f(xs...);
    //     };

    //     cartesian_for_each(inner, seqs...);

    //     return result;
    // }

    // // cartesian_for_index

    // template <typename F = void (*)(const int &)>
    // void cartesian_for_index(const F &f, const int &i)
    // {
    //     for_index(f, i);
    // }

    // template <typename... Ints, typename F = void (*)(const int &)>
    // void cartesian_for_index(const F &f, const int &i, const Ints &...is)
    // {
    //     for (int i_ = 0; i_ < i; ++i_)
    //     {
    //         const auto inner = [=](const Ints &...is)
    //         { f(i_, is...); };

    //         cartesian_for_index(inner, is...);
    //     }
    // }

    // // // todo begin

    // // // todo end

    // // todo head
    // // ! Partial function. Make sure non empty
    // template <typename SeqA>
    // Element_t<SeqA> head(const SeqA &as)
    // {
    //     return as[0];
    // }

    // // todo tail
    // // ! Partial function. Make sure non empty
    // template <typename SeqA>
    // auto tail(const SeqA &as)
    //     -> VectorView<const Element_t<SeqA>>
    // {
    //     return VectorView<const Element_t<SeqA>>{p_data(as) + 1, length(as) - 1};
    // }

    // // ! Partial function. Make sure non empty
    // template <typename SeqA>
    // auto tail(SeqA &as)
    //     -> VectorView<Element_t<SeqA>>
    // {
    //     return VectorView<Element_t<SeqA>>{p_data(as) + 1, length(as) - 1};
    // }

    // // todo init
    // // ! Partial function. Make sure non empty
    // template <typename SeqA>
    // auto init(const SeqA &as)
    //     -> VectorView<const Element_t<SeqA>>
    // {
    //     return VectorView<const Element_t<SeqA>>{p_data(as), length(as) - 1};
    // }

    // // ! Partial function. Make sure non empty
    // template <typename SeqA>
    // auto init(SeqA &as)
    //     -> VectorView<Element_t<SeqA>>
    // {
    //     return VectorView<Element_t<SeqA>>{p_data(as), length(as) - 1};
    // }

    // // todo last
    // // ! Partial function. Make sure non empty
    // template <typename SeqA>
    // Element_t<SeqA> last(const SeqA &as)
    // {
    //     return as[length(as) - 1];
    // }

    // // todo is_null

    // // using View_t = Conditional<
    // //     template <typename SeqA>
    // //     IsStaticLength<SeqA>::value,
    // //     ArrayView<Element_t<SeqA>>,
    // //     VectorView<Element_t<SeqA>>>;

    // // todo take

    // // template <typename N, typename SeqA>
    // // auto take(const N &n, SeqA &as)
    // //     -> EnableIf<
    // //         IsIntegralConst<N>::value && IsStaticLength<SeqA>::value,
    // //         ArrayView<ViewElement_t<SeqA>, bound_v(0, StaticLength<SeqA>::value, N::value)>>
    // // {
    // //     return ArrayView<ViewElement_t<SeqA>, bound_v(0, StaticLength<SeqA>::value, N::value)>{p_data(as)};
    // // }

    // // template <typename N, typename SeqA>
    // // auto take(const N &n, SeqA &as)
    // //     -> EnableIf<
    // //         !IsIntegralConst<N>::value && IsStaticLength<SeqA>::value,
    // //         VectorView<ViewElement_t<SeqA>>>
    // // {
    // //     return VectorView<ViewElement_t<SeqA>>{p_data(as), (size_t)bound_v(0, length(as), n)};
    // // }

    // template <typename SeqA>
    // auto take(int n, const SeqA &as)
    //     -> VectorView<const Element_t<SeqA>>
    // {
    //     return VectorView<const Element_t<SeqA>>{p_data(as), (size_t)bound_v(0, (int)length(as), n)};
    // }

    // template <typename SeqA>
    // auto take(int n, SeqA &as)
    //     -> VectorView<Element_t<SeqA>>
    // {
    //     return VectorView<Element_t<SeqA>>{p_data(as), (size_t)bound_v(0, (int)length(as), n)};
    // }

    // // drop

    // // template <typename N, typename SeqA>
    // // auto drop(const N &n, SeqA &as)
    // //     -> EnableIf<
    // //         IsIntegralConst<N>::value && IsStaticLength<SeqA>::value,
    // //         ArrayView<ViewElement_t<SeqA>, bound_v(0, StaticLength<SeqA>::value, StaticLength<SeqA>::value - N::value)>>
    // // {
    // //     // ! What if larger than n? maybe last with 0?
    // //     return ArrayView<ViewElement_t<SeqA>, bound_v(0, StaticLength<SeqA>::value, StaticLength<SeqA>::value - N::value)>{p_data(as) + n};
    // // }

    // // template <typename N, typename SeqA>
    // // auto drop(const N &n, SeqA &as)
    // //     -> EnableIf<
    // //         IsIntegralConst<N>::value && IsStaticLength<SeqA>::value,
    // //         ArrayView<Element_t<SeqA>, StaticLength<SeqA>::value - N::value>>
    // // {
    // //     // ! What if larger than n? maybe last with 0?
    // //     return ArrayView<Element_t<SeqA>, StaticLength<SeqA>::value - N::value>{data(as) + n};
    // // }

    // // template <typename N, typename SeqA>
    // // auto drop(const N &n, const SeqA &as)
    // //     -> EnableIf<
    // //         IsIntegralConst<N>::value && IsStaticLength<SeqA>::value,
    // //         ArrayView<const Element_t<SeqA>, StaticLength<SeqA>::value - N::value>>
    // // {
    // //     return ArrayView<const Element_t<SeqA>, StaticLength<SeqA>::value - N::value>{data(as) + n};
    // // }

    // // template <typename N, typename SeqA>
    // // auto drop(const N &n, SeqA &as)
    // //     -> EnableIf<
    // //         !IsIntegralConst<N>::value && IsStaticLength<SeqA>::value,
    // //         VectorView<ViewElement_t<SeqA>>>
    // // {
    // //     // ! What if larger than n? maybe last with 0?
    // //     return VectorView<ViewElement_t<SeqA>>{p_data(as) + n, (size_t)bound_v(0, length(as), length(as) - n)};
    // // }

    // // template <typename N, typename SeqA>
    // // auto drop(const N &n, SeqA &as)
    // //     -> EnableIf<
    // //         !IsIntegralConst<N>::value && IsStaticLength<SeqA>::value,
    // //         VectorView<Element_t<SeqA>>>
    // // {
    // //     // ! What if larger than n? maybe last with 0?
    // //     return VectorView<Element_t<SeqA>>{p_data(as) + n, length(as) - n};
    // // }

    // // template <typename N, typename SeqA>
    // // auto drop(const N &n, const SeqA &as)
    // //     -> EnableIf<
    // //         !IsIntegralConst<N>::value && IsStaticLength<SeqA>::value,
    // //         VectorView<const Element_t<SeqA>>>
    // // {
    // //     return VectorView<const Element_t<SeqA>>{p_data(as) + n, length(as) - n};
    // // }

    // template <typename SeqA>
    // auto drop(int n, const SeqA &as)
    //     -> VectorView<const Element_t<SeqA>>
    // {
    //     auto as_length = length(as);
    //     auto bound_n = (size_t)bound_v(0, (int)as_length, n);
    //     return VectorView<const Element_t<SeqA>>{p_data(as) + bound_n, as_length - bound_n};
    // }

    // template <typename SeqA>
    // auto drop(int n, SeqA &as)
    //     -> VectorView<Element_t<SeqA>>
    // {
    //     auto as_length = length(as);
    //     auto bound_n = (size_t)bound_v(0, (int)as_length, n);
    //     return VectorView<Element_t<SeqA>>{p_data(as) + bound_n, as_length - bound_n};
    // }

    // // elem

    // template <typename SeqA>
    // bool elem(const Element_t<SeqA> &a, const SeqA &as)
    // {
    //     const int length_as = length(as);

    //     for (int i = 0; i < length_as; ++i)
    //     {
    //         if (as[i] == a)
    //         {
    //             return true;
    //         }
    //     }

    //     return false;
    // }

    // // elem_index

    // template <typename SeqA>
    // Maybe<int> elem_index(const Element_t<SeqA> &a, const SeqA &as)
    // {
    //     const int length_as = length(as);

    //     for (int i = 0; i < length_as; ++i)
    //     {
    //         if (as[i] == a)
    //         {
    //             return i;
    //         }
    //     }

    //     return Nothing{};
    // }

    // // IndicesReturn_t

    // template <typename SeqA>
    // using IndicesReturn_t =
    //     Conditional<
    //         IsStaticCapacity<SeqA>::value,
    //         ArrVec<int, StaticCapacity<SeqA>::value>,
    //         Vector<int>>;

    // // elem_indices

    // template <typename SeqA>
    // auto elem_indices(const Element_t<SeqA> &a, const SeqA &as)
    //     -> EnableIf<
    //         IsStaticCapacity<SeqA>::value,
    //         IndicesReturn_t<SeqA>>
    // {
    //     IndicesReturn_t<SeqA> result;

    //     for (int i = 0; i < length(as); ++i)
    //     {
    //         if (a == as[i])
    //         {
    //             result.push_back(i);
    //         }
    //     }

    //     return result;
    // }

    // template <typename SeqA>
    // auto elem_indices(const Element_t<SeqA> &a, const SeqA &as)
    //     -> EnableIf<
    //         !IsStaticCapacity<SeqA>::value,
    //         IndicesReturn_t<SeqA>>
    // {
    //     const int length_as = length(as);

    //     IndicesReturn_t<SeqA> result;
    //     result.reserve(length_as);

    //     for (int i = 0; i < length_as; ++i)
    //     {
    //         if (a == as[i])
    //         {
    //             result.push_back(i);
    //         }
    //     }

    //     return result;
    // }

    // // find

    // template <typename SeqA, typename F = void (*)(const Element_t<SeqA> &)>
    // bool find(const F &f, const SeqA &as)
    // {
    //     for (int i = 0; i < length(as); ++i)
    //     {
    //         const Element_t<SeqA> a = as[i];
    //         if (f(a))
    //         {
    //             return true;
    //         }
    //     }

    //     return false;
    // }

    // // find_index

    // template <typename SeqA, typename F = void (*)(const Element_t<SeqA> &)>
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

    // template <typename SeqA, typename F = void (*)(const Element_t<SeqA> &)>
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

    // template <typename SeqA, typename F = void (*)(const Element_t<SeqA> &)>
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