#ifndef PRELUDE_HPP_
#define PRELUDE_HPP_

#include "sfinae.hpp"
#include "trait.hpp"
#include "enum_type.hpp"
#include "maybe.hpp"
#include "sequence.hpp"

// Trust compiler for length calculation

namespace efp
{

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

    // execute_pack

    template <typename... Args>
    void execute_pack(Args... args) {}

    template <typename... Ass>
    using AppendReturn = Conditional<
        All<IsStaticSize<Ass>...>::value,
        Array<Common<Element<Ass>...>, sum_v(CtSize<Ass>::value...)>,
        Conditional<
            All<IsStaticCapacity<Ass>...>::value,
            ArrVec<Common<Element<Ass>...>, sum_v(CtCapacity<Ass>::value...)>,
            Vector<Common<Element<Ass>...>>>>;

    namespace detail
    {
        template <typename As, typename Bs>
        Unit append_impl(size_t &idx, As &as, const Bs &bs)
        {
            const auto seq_length = bs.size(); // Assuming `size` method is available

            for (size_t i = 0; i < seq_length; ++i)
            {
                nth(idx, as) = nth(i, bs);
                idx++;
            }

            return unit;
        }
    }

    template <typename As, typename... Ass>
    auto append(const As &as, const Ass &...ass) -> AppendReturn<As, Ass...>
    {
        static_assert(All<IsSequence<As>, IsSequence<Ass>...>::value, "All types must be sequence types.");

        AppendReturn<As, Ass...> res{};

        if (CtSize<AppendReturn<As, Ass...>>::value == dyn)
        {
            res.resize(sum_v(static_cast<size_t>(length(as)), length(ass)...));
        }

        size_t idx{0};
        execute_pack(detail::append_impl(idx, res, as),
                     detail::append_impl(idx, res, ass)...);

        return res;
    }

    template <typename As, typename... Ass>
    size_t min_length(const As &as, const Ass &...ass)
    {
        static_assert(All<IsSequence<As>, IsSequence<Ass>...>::value, "All types must be sequence types.");
        return minimum_v(static_cast<size_t>(length(as)), length(ass)...);
    }

    template <typename... Ass, typename F = void (*)(const Element<Ass> &...)>
    void for_each(const F &f, const Ass &...ass)
    {
        static_assert(All<IsSequence<Ass>...>::value, "All types must be sequence types.");
        const size_t res_length = min_length(ass...);

        for (size_t i = 0; i < res_length; ++i)
        {
            f(nth(i, ass)...);
        }
    }

    template <typename... Ass, typename F = void (*)(Element<Ass> &...)>
    void for_each_mut(const F &f, Ass &...ass)
    {
        static_assert(All<IsSequence<Ass>...>::value, "All types must be sequence types.");
        const size_t res_length = min_length(ass...);

        for (size_t i = 0; i < res_length; ++i)
        {
            f(nth(i, ass)...);
        }
    }

    template <typename R, typename... Ass>
    using NAryReturn = Conditional<
        All<IsStaticSize<Ass>...>::value,
        Array<R, Min<CtSize<Ass>...>::value>,
        Conditional<
            All<IsStaticCapacity<Ass>...>::value,
            ArrVec<R, Min<CtCapacity<Ass>...>::value>,
            Vector<R>>>;

    // MapReturn

    template <typename F, typename... Ass>
    using MapReturn = NAryReturn<CallReturn<F, Element<Ass>...>, Ass...>;

    // map

    template <typename F, typename... Ass>
    auto map(const F &f, const Ass &...ass) -> MapReturn<F, Ass...>
    {
        static_assert(All<IsSequence<Ass>...>::value, "All arguments should implement Sequence trait.");

        MapReturn<F, Ass...> res{};
        const size_t res_len = min_length(ass...);
        if (CtSize<MapReturn<F, Ass...>>::value == dyn)
        {
            res.resize(res_len);
        }

        for (size_t i = 0; i < res_len; ++i)
        {
            nth(i, res) = f(nth(i, ass)...);
        }

        return res;
    }

    // FilterReturn

    template <typename As>
    using FilterReturn = Conditional<
        CtCapacity<As>::value != dyn,
        ArrVec<Element<As>, CtCapacity<As>::value>,
        Vector<Element<As>>>;

    // filter

    template <typename As, typename F = bool (*)(const Element<As> &)>
    auto filter(const F &f, const As &as) -> FilterReturn<As>
    {
        static_assert(IsSequence<As>::value, "Argument should implement sequence trait.");

        FilterReturn<As> res{};
        const auto res_len = length(as);

        for (size_t i = 0; i < res_len; ++i)
        {
            const auto &a = nth(i, as);
            if (f(a))
            {
                res.push_back(a);
            }
        }

        return res;
    }

    // foldl

    template <typename As, typename R, typename F = R (*)(const R &, const Element<As> &)>
    R foldl(const F &f, const R &init, const As &as)
    {
        static_assert(IsSequence<As>::value, "Argument should implement sequence trait.");

        R res = init;

        for (size_t i = 0; i < length(as); ++i)
        {
            res = f(res, nth(i, as));
        }

        return res;
    }

    // foldr

    template <typename As, typename R, typename F = R (*)(const Element<As> &, const R &)>
    R foldr(const F &f, const R &init, const As &as)
    {
        static_assert(IsSequence<As>::value, "Argument should implement sequence trait.");

        R res = init;

        for (size_t i = length(as) - 1; i != -1; --i)
        {
            res = f(nth(i, as), res);
        }

        return res;
    }

    // todo size_t
    // FromFunctionReturnImpl

    namespace detail
    {
        template <typename N, typename F>
        struct FromFunctionReturnImpl
        {
            using Type = Vector<CallReturn<F, N>>;
        };

        template <size_t n, typename F>
        struct FromFunctionReturnImpl<Size<n>, F>
        {
            using Type = Array<CallReturn<F, size_t>, n>;
        };
    }

    // FromFunctionReturn

    template <typename N, typename F>
    using FromFunctionReturn = typename detail::FromFunctionReturnImpl<N, F>::Type;

    // from_function

    template <typename N, typename F>
    auto from_function(const N &length, const F &f)
        -> FromFunctionReturn<N, F>
    {
        FromFunctionReturn<N, F> res{};

        if (!IsStaticSize<FromFunctionReturn<N, F>>::value)
        {
            res.resize(length);
        }

        for (size_t i = 0; i < length; ++i)
        {
            nth(i, res) = f(i);
        }

        return res;
    }

    // for_index

    template <typename F = void (*)(size_t)>
    void for_index(const F &f, const size_t n)
    {
        for (size_t i = 0; i < n; ++i)
        {
            f(i);
        }
    }

    // for_each_with_index

    template <typename... Ass, typename F = void (*)(size_t, const Element<Ass> &...)>
    void for_each_with_index(const F &f, const Ass &...seqs)
    {
        static_assert(All<IsSequence<Ass>...>::value, "All arguments should implement Sequence trait.");
        const auto min_len = min_length(seqs...);

        for (size_t i = 0; i < min_len; ++i)
        {
            f(i, nth(i, seqs)...);
        }
    }

    template <typename... Ass, typename F = void (*)(size_t, Element<Ass> &...)>
    void for_each_with_index_mut(const F &f, Ass &...seqs)
    {
        static_assert(All<IsSequence<Ass>...>::value, "All arguments should implement Sequence trait.");
        const size_t min_len = min_length(seqs...);

        for (size_t i = 0; i < min_len; ++i)
        {
            f(i, nth(i, seqs)...);
        }
    }

    // cartesian_for_each

    template <typename As, typename F = void (*)(const Element<As> &)>
    void cartesian_for_each(const F &f, const As &as)
    {
        static_assert(IsSequence<As>::value, "Argument should implement sequence trait.");
        for_each(f, as);
    }

    template <typename As, typename... Ass, typename F = void (*)(const Element<As> &, const Element<Ass> &...)>
    void cartesian_for_each(const F &f, const As &as, const Ass &...ass)
    {
        static_assert(All<IsSequence<As>, IsSequence<Ass>...>::value, "All arguments should be instance of Sequence trait.");
        const auto as_len = length(as);

        for (size_t i = 0; i < as_len; ++i)
        {
            const auto a = nth(i, as);
            const auto inner = [&](const Element<Ass> &...xs)
            { f(a, xs...); };

            cartesian_for_each(inner, ass...);
        }
    }

    template <typename As, typename F = void (*)(Element<As> &)>
    void cartesian_for_each_mut(const F &f, As &as)
    {
        static_assert(IsSequence<As>::value, "Argument should implement sequence trait.");
        for_each_mut(f, as);
    }

    template <typename As, typename... Ass, typename F = void (*)(Element<As> &, Element<Ass> &...)>
    void cartesian_for_each_mut(const F &f, As &as, Ass &...ass)
    {
        static_assert(All<IsSequence<As>, IsSequence<Ass>...>::value, "All arguments should be instance of Sequence trait.");
        const auto as_len = length(as);

        for (size_t i = 0; i < as_len; ++i)
        {
            const auto a = nth(i, as);
            const auto inner = [&](Element<Ass> &...xs)
            { f(a, xs...); };

            cartesian_for_each_mut(inner, ass...);
        }
    }

    // MapWithIndexRetrun

    template <typename F, typename... Ass>
    using MapWithIndexReturn = Conditional<
        All<IsStaticSize<Ass>...>::value,
        Array<CallReturn<F, size_t, Element<Ass>...>, Min<CtSize<Ass>...>::value>,
        Conditional<
            All<IsStaticCapacity<Ass>...>::value,
            ArrVec<CallReturn<F, size_t, Element<Ass>...>, Min<CtCapacity<Ass>...>::value>,
            Vector<CallReturn<F, size_t, Element<Ass>...>>>>;

    // map_with_index

    template <typename... Ass, typename F = void (*)(size_t, const Element<Ass> &...)>
    auto map_with_index(const F &f, const Ass &...ass) -> MapWithIndexReturn<F, Ass...>
    {
        static_assert(All<IsSequence<Ass>...>::value, "All arguments should implement Sequence trait.");

        auto res = MapWithIndexReturn<F, Ass...>{};
        const auto res_len = min_length(ass...);

        if (CtSize<MapWithIndexReturn<F, Ass...>>::value == dyn)
        {
            res.resize(res_len);
        }

        for (size_t i = 0; i < res_len; ++i)
        {
            nth(i, res) = f(i, nth(i, ass)...);
        }

        return res;
    }

    // CartesianMapReturn

    template <typename F, typename... Ass>
    using CartesianMapReturn = Conditional<
        All<IsStaticSize<Ass>...>::value,
        Array<CallReturn<F, Element<Ass>...>, product_v(CtSize<Ass>::value...)>,
        Conditional<
            All<IsStaticCapacity<Ass>...>::value,
            ArrVec<CallReturn<F, Element<Ass>...>, product_v(CtCapacity<Ass>::value...)>,
            Vector<CallReturn<F, Element<Ass>...>>>>;

    // cartesian_map

    // ! Maybe need to bechmark and optimize

    template <typename... Ass, typename F = void (*)(const Element<Ass> &...)>
    auto cartesian_map(const F &f, const Ass &...ass) -> CartesianMapReturn<F, Ass...>
    {
        static_assert(All<IsSequence<Ass>...>::value, "All arguments should implement Sequence trait.");

        auto res = CartesianMapReturn<F, Ass...>{};
        if (CtSize<CartesianMapReturn<F, Ass...>>::value == dyn)
        {
            res.resize(product_v(static_cast<size_t>(length(ass))...));
        }

        size_t i = 0;
        const auto inner = [&](const Element<Ass> &...xss)
        { res[i++] = f(xss...); };

        cartesian_for_each(inner, ass...);

        return res;
    }

    // cartesian_for_index

    template <typename F = void (*)(size_t)>
    void cartesian_for_index(const F &f, size_t n)
    {
        for_index(f, n);
    }

    template <typename... Ints, typename F = void (*)(size_t)>
    void cartesian_for_index(const F &f, size_t n, const Ints &...is)
    {
        for (size_t i = 0; i < n; ++i)
        {
            const auto inner = [&](const Ints &...is)
            { f(i, is...); };

            cartesian_for_index(inner, is...);
        }
    }

    // head

    // ! Partial function. Make sure non empty

    template <typename As>
    auto head(const As &as) -> const Element<As> &
    {
        static_assert(IsSequence<As>::value, "Argument should implement sequence trait.");
        assert(!as.empty()); // Ensure the sequence is not empty.
        return nth(0, as);
    }

    // TailReturn

    template <typename As, bool is_const>
    using TailReturn = EnableIf<
        CtSize<As>::value != 0 && CtCapacity<As>::value != 0,
        Conditional<
            IsStaticSize<As>::value,
            Conditional<
                is_const,
                ArrayView<const Element<As>, CtSize<As>::value - 1>,
                ArrayView<Element<As>, CtSize<As>::value - 1>>,
            Conditional<
                IsStaticCapacity<As>::value,
                Conditional<
                    is_const,
                    ArrVecView<const Element<As>, CtCapacity<As>::value - 1>,
                    ArrVecView<Element<As>, CtCapacity<As>::value - 1>>,
                Conditional<
                    is_const,
                    VectorView<const Element<As>>,
                    VectorView<Element<As>>>>>>;

    // tail
    // ! Partial function. Application on empty list is abortion.

    template <typename A>
    auto tail(const A &as) -> TailReturn<A, true>
    {
        static_assert(IsSequence<A>::value, "Argument should be an instance of Sequence trait.");
        assert(length(as) > 0); // Ensure the sequence is not empty.
        return {data(as) + 1, length(as) - 1};
    }

    template <typename A>
    auto tail(A &as) -> TailReturn<A, false>
    {
        static_assert(IsSequence<A>::value, "Argument should be an instance of Sequence trait.");
        assert(length(as) > 0); // Ensure the sequence is not empty.
        return {data(as) + 1, length(as) - 1};
    }

    // InitReturn

    template <typename As, bool is_const>
    using InitReturn = EnableIf<
        CtSize<As>::value != 0 && CtCapacity<As>::value != 0,
        Conditional<
            IsStaticSize<As>::value,
            Conditional<
                is_const,
                ArrayView<const Element<As>, CtSize<As>::value - 1>,
                ArrayView<Element<As>, CtSize<As>::value - 1>>,
            Conditional<
                IsStaticCapacity<As>::value,
                Conditional<
                    is_const,
                    ArrVecView<const Element<As>, CtCapacity<As>::value - 1>,
                    ArrVecView<Element<As>, CtCapacity<As>::value - 1>>,
                Conditional<
                    is_const,
                    VectorView<const Element<As>>,
                    VectorView<Element<As>>>>>>;
    // init

    // ! Zero length input will envoke abort.÷

    template <typename As>
    auto init(const As &as) -> InitReturn<As, true>
    {
        static_assert(IsSequence<As>::value, "Argument should implement sequence trait.");
        assert(length(as) > 0); // Ensure the sequence is not empty.
        return {data(as), length(as) - 1};
    }

    template <typename As>
    auto init(As &as) -> InitReturn<As, false>
    {
        static_assert(IsSequence<As>::value, "Argument should implement sequence trait.");
        assert(length(as) > 0); // Ensure the sequence is not empty.
        return {data(as), length(as) - 1};
    }

    // last

    // ! Partial function. Make sure non

    template <typename As>
    auto last(const As &as) -> const Element<As> &
    {
        static_assert(IsSequence<As>::value, "Argument should implement sequence trait.");
        assert(length(as) > 0); // Ensure the sequence is not empty.
        return as[length(as) - 1];
    }

    // is_null

    template <typename As>
    bool is_null(const As &as)
    {
        static_assert(IsSequence<As>::value, "Argument should implement sequence trait.");
        return length(as) == 0;
    }

    // TakeUnsafeReturnImpl
    // ! Size must be valid. It must be smaller than compile time and runtime size.
    // ! Invalid size will be undifined behavior
    namespace detail
    {
        template <typename N, typename As, bool is_const>
        struct TakeUnsafeReturnImpl
        {
            using Type = Conditional<
                IsStaticCapacity<As>::value,
                Conditional<is_const, ArrVecView<const Element<As>, CtCapacity<As>::value>, ArrVecView<Element<As>, CtCapacity<As>::value>>,
                Conditional<is_const, VectorView<const Element<As>>, VectorView<Element<As>>>>;
        };

        template <size_t n, typename As, bool is_const>
        struct TakeUnsafeReturnImpl<Size<n>, As, is_const>
        {
            // todo Static assert for invalid size

            using Type = Conditional<
                is_const,
                ArrayView<const Element<As>, n>,
                ArrayView<Element<As>, n>>;
        };
    }

    // TakeUnsafeReturn

    template <typename N, typename As, bool is_const>
    using TakeUnsafeReturn = typename detail::TakeUnsafeReturnImpl<N, As, is_const>::Type;

    // take_unsafe

    // !Should not put n longer than the length. Check should be done by the caller
    // Let's make unsafe version as well
    template <typename N, typename As>
    auto take_unsafe(N n, const As &as) -> TakeUnsafeReturn<N, As, true>
    {
        static_assert(IsSequence<As>::value, "Argument should implement sequence trait.");
        return TakeUnsafeReturn<N, As, true>(data(as), n);
    }

    template <typename N, typename As>
    auto take_unsafe(N n, As &as) -> TakeUnsafeReturn<N, As, false>
    {
        static_assert(IsSequence<As>::value, "Argument should implement sequence trait.");
        return TakeUnsafeReturn<N, As, false>(data(as), n);
    }

    // TakeReturnImpl

    namespace detail
    {
        template <typename N, typename As, bool is_const>
        struct TakeReturnImpl
        {
            using Type = Conditional<
                IsStaticCapacity<As>::value,
                Conditional<
                    is_const,
                    ArrVecView<const Element<As>, CtCapacity<As>::value>,
                    ArrVecView<Element<As>, CtCapacity<As>::value>>,
                Conditional<
                    is_const,
                    VectorView<const Element<As>>,
                    VectorView<Element<As>>>>;
        };

        template <size_t n, typename As, bool is_const>
        struct TakeReturnImpl<Size<n>, As, is_const>
        {
            static constexpr size_t bound_size = min_v(n, CtSize<As>::value);
            static constexpr size_t bound_capacity = min_v(n, CtCapacity<As>::value);

            using Type = Conditional<
                IsStaticSize<As>::value,
                Conditional<
                    is_const,
                    ArrayView<const Element<As>, bound_size>,
                    ArrayView<Element<As>, bound_size>>,

                Conditional<
                    is_const,
                    ArrVecView<const Element<As>, bound_capacity>,
                    ArrVecView<Element<As>, bound_capacity>>>;
        };
    }

    // TakeReturn

    template <typename N, typename As, bool is_const>
    using TakeReturn = typename detail::TakeReturnImpl<N, As, is_const>::Type;

    // take

    // !Should not put n longer than the length. Check should be done by the caller
    // Let's make unsafe version as well
    template <typename N, typename As>
    auto take(N n, const As &as) -> TakeReturn<N, As, true>
    {
        static_assert(IsSequence<As>::value, "Argument should implement sequence trait.");
        return TakeReturn<N, As, true>(data(as), min_v(static_cast<size_t>(n), static_cast<size_t>(length(as)))); // Safeguarding against n > length(as)
    }

    template <typename N, typename As>
    auto take(N n, As &as) -> TakeReturn<N, As, false>
    {
        static_assert(IsSequence<As>::value, "Argument should implement sequence trait.");
        return TakeReturn<N, As, false>(data(as), min_v(static_cast<size_t>(n), static_cast<size_t>(length(as)))); // Safeguarding against n > length(as)
    }

    // Dr)opUnsafeReturnImpl

    namespace detail
    {
        template <typename N, typename As, bool is_const>
        struct DropUnsafeReturnImpl
        {
            using Type = Conditional<
                IsStaticCapacity<As>::value,
                Conditional<
                    is_const,
                    ArrVecView<const Element<As>, CtCapacity<As>::value>,
                    ArrVecView<Element<As>, CtCapacity<As>::value>>,
                Conditional<
                    is_const,
                    VectorView<const Element<As>>,
                    VectorView<Element<As>>>>;
        };

        template <size_t n, typename As, bool is_const>
        struct DropUnsafeReturnImpl<Size<n>, As, is_const>
        {
            using Type = Conditional<
                IsStaticSize<As>::value,
                Conditional<
                    is_const,
                    ArrayView<const Element<As>, CtSize<As>::value - n>,
                    ArrayView<Element<As>, CtSize<As>::value - n>>,
                Conditional<
                    IsStaticCapacity<As>::value,
                    Conditional<
                        is_const,
                        ArrVecView<const Element<As>, CtCapacity<As>::value - n>,
                        ArrVecView<Element<As>, CtCapacity<As>::value - n>>,
                    Conditional<
                        is_const,
                        VectorView<const Element<As>>,
                        VectorView<Element<As>>>>>;
        };
    }

    // DropUnsafeReturn

    template <typename N, typename As, bool is_const>
    using DropUnsafeReturn = typename detail::DropUnsafeReturnImpl<N, As, is_const>::Type;

    // drop_unsafe

    template <typename N, typename As>
    auto drop_unsafe(N n, const As &as) -> DropUnsafeReturn<N, As, true>
    {
        static_assert(IsSequence<As>::value, "Argument should implement sequence trait.");
        return DropUnsafeReturn<N, As, true>(data(as) + n, length(as) - n);
    }

    template <typename N, typename As>
    auto drop_unsafe(N n, As &as) -> DropUnsafeReturn<N, As, false>
    {
        static_assert(IsSequence<As>::value, "Argument should implement sequence trait.");
        return DropUnsafeReturn<N, As, false>(data(as) + n, length(as) - n);
    }

    // DropReturnImpl

    namespace detail
    {
        template <typename N, typename As, bool is_const>
        struct DropReturnImpl
        {
            using Type = Conditional<
                IsStaticCapacity<As>::value,
                Conditional<
                    is_const,
                    ArrVecView<const Element<As>, CtCapacity<As>::value>,
                    ArrVecView<Element<As>, CtCapacity<As>::value>>,
                Conditional<
                    is_const,
                    VectorView<const Element<As>>,
                    VectorView<Element<As>>>>;
        };

        template <size_t n, typename As, bool is_const>
        struct DropReturnImpl<Size<n>, As, is_const>
        {
            static constexpr size_t bound_size = (CtSize<As>::value > n) ? (CtSize<As>::value - n) : 0;
            static constexpr size_t bound_capacity = (CtCapacity<As>::value > n) ? (CtCapacity<As>::value - n) : 0;

            using Type = Conditional<
                IsStaticSize<As>::value,
                Conditional<
                    is_const,
                    ArrayView<const Element<As>, bound_size>,
                    ArrayView<Element<As>, bound_size>>,
                Conditional<
                    IsStaticCapacity<As>::value,
                    Conditional<
                        is_const,
                        ArrVecView<const Element<As>, bound_capacity>,
                        ArrVecView<Element<As>, bound_capacity>>,
                    Conditional<
                        is_const,
                        VectorView<const Element<As>>,
                        VectorView<Element<As>>>>>;
        };
    }

    // DropReturn

    template <typename N, typename As, bool is_const>
    using DropReturn = typename detail::DropReturnImpl<N, As, is_const>::Type;

    // drop

    template <typename N, typename As>
    auto drop(N n, const As &as) -> DropReturn<N, As, true>
    {
        static_assert(IsSequence<As>::value, "Argument should implement sequence trait.");
        const size_t as_len = length(as);
        size_t bound_drop_size = (n > as_len) ? as_len : n; // Ensuring n doesn't exceed the size of as
        return DropReturn<N, As, true>(data(as) + bound_drop_size, as_len - bound_drop_size);
    }

    template <typename N, typename As>
    auto drop(N n, As &as) -> DropReturn<N, As, false>
    {
        static_assert(IsSequence<As>::value, "Argument should implement sequence trait.");
        const size_t as_len = length(as);
        size_t bound_drop_size = (n > as_len) ? as_len : n; // Ensuring n doesn't exceed the size of as
        return DropReturn<N, As, false>(data(as) + bound_drop_size, as_len - bound_drop_size);
    }

    // SliceUnsafeReturn

    template <typename S, typename E, typename As, bool is_const>
    using SliceUnsafeReturn = TakeUnsafeReturn<decltype(efp::declval<E>() - efp::declval<S>()), DropUnsafeReturn<S, As, is_const>, is_const>;

    // slice_unsafe

    // todo Optimization
    template <typename S, typename E, typename As>
    auto slice_unsafe(S start, E end, const As &as)
        -> SliceUnsafeReturn<S, E, As, true>
    {
        return SliceUnsafeReturn<S, E, As, true>(data(as) + start, end - start);
    }

    template <typename S, typename E, typename As>
    auto slice_unsafe(S start, E end, As &as)
        -> SliceUnsafeReturn<S, E, As, false>
    {
        return SliceUnsafeReturn<S, E, As, false>(data(as) + start, end - start);
    }

    // SliceReturn

    template <typename S, typename E, typename As, bool is_const>
    using SliceReturn = TakeReturn<decltype(efp::declval<E>() - efp::declval<S>()), DropReturn<S, As, is_const>, is_const>;

    // slice

    // todo Optimization
    template <typename S, typename E, typename As>
    auto slice(S start, E end, const As &as)
        -> SliceReturn<S, E, As, true>
    {
        return SliceReturn<S, E, As, true>(data(as) + start, end - start);
    }

    template <typename S, typename E, typename As>
    auto slice(S start, E end, As &as)
        -> SliceReturn<S, E, As, false>
    {
        return SliceReturn<S, E, As, false>(data(as) + start, end - start);
    }

    // todo Add test for take_while and drop_while

    // take_while

    template <typename As>
    using TakeWhileReturn = Conditional<
        IsStaticCapacity<As>::value,
        ArrVecView<Element<As>, CtCapacity<As>::value>,
        VectorView<Element<As>>>;

    template <typename As, typename F = bool (*)(const Element<As> &)>
    auto take_while(const F &f, const As &as)
        -> TakeWhileReturn<As>
    {
        const auto as_len = length(as);

        size_t i = 0;
        while (i < as_len && f(nth(i, as)))
        {
            ++i;
        }

        return take(i, as);
    }

    // drop_while

    template <typename As>
    using DropWhileReturn = Conditional<
        IsStaticCapacity<As>::value,
        ArrVecView<Element<As>, CtCapacity<As>::value>,
        VectorView<Element<As>>>;

    template <typename As, typename F = bool (*)(const Element<As> &)>
    auto drop_while(const F &f, const As &as)
        -> DropWhileReturn<As>
    {
        const auto as_len = length(as);

        size_t i = 0;
        while (i < as_len && f(nth(i, as)))
        {
            ++i;
        }

        return drop(i, as);
    }

    // elem

    template <typename As>
    bool elem(const Element<As> &a, const As &as)
    {
        const auto as_len = length(as);

        for (size_t i = 0; i < as_len; ++i)
        {
            if (nth(i, as) == a)
                return true;
        }

        return false;
    }

    // elem_index

    template <typename As>
    Maybe<size_t> elem_index(const Element<As> &a, const As &as)
    {
        const auto as_len = length(as);

        for (size_t i = 0; i < as_len; ++i)
        {
            if (nth(i, as) == a)
                return i;
        }

        return nothing;
    }

    // ElemIndicesReturn

    template <typename As>
    using ElemIndicesReturn = Conditional<
        IsStaticCapacity<As>::value,
        ArrVec<size_t, CtCapacity<As>::value>,
        Vector<size_t>>;

    // elem_indices

    template <typename As>
    auto elem_indices(const Element<As> &a, const As &as)
        -> ElemIndicesReturn<As>
    {
        ElemIndicesReturn<As> res{};
        const auto as_len = length(as);

        for (size_t i = 0; i < as_len; ++i)
        {
            if (a == nth(i, as))
                res.push_back(i);
        }

        return res;
    }

    // find

    template <typename As, typename F = void (*)(const Element<As> &)>
    bool find(const F &f, const As &as)
    {
        const auto as_len = length(as);

        for (size_t i = 0; i < as_len; ++i)
        {
            if (f(nth(i, as)))
                return true;
        }

        return false;
    }

    // find_index

    template <typename As, typename F = void (*)(const Element<As> &)>
    auto find_index(const F &f, const As &as)
        -> Maybe<size_t>
    {
        const auto as_len = length(as);

        for (size_t i = 0; i < as_len; ++i)
        {
            if (f(nth(i, as)))
                return i;
        }

        return nothing;
    }

    // FindIndicesReturn

    template <typename As>
    using FindIndicesReturn = Conditional<
        IsStaticCapacity<As>::value,
        ArrVec<size_t, CtCapacity<As>::value>,
        Vector<size_t>>;

    // find_indices

    template <typename As, typename F = void (*)(const Element<As> &)>
    auto find_indices(const F &f, const As &as)
        -> FindIndicesReturn<As>
    {
        FindIndicesReturn<As> res{};
        const auto as_len = length(as);

        for (size_t i = 0; i < as_len; ++i)
        {
            if (f(nth(i, as)))
                res.push_back(i);
        }

        return res;
    }
}
#endif