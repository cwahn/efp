#ifndef TUPLE_HPP_
#define TUPLE_HPP_

namespace efp
{
    // TupleLeaf

    template <int index, typename A>
    class TupleLeaf
    {
    public:
        using Element = A;
        static constexpr int idx = index;

        TupleLeaf() {}
        TupleLeaf(const A &value) : value_{value} {}
        TupleLeaf(A &&value) : value_{value} {}

        ~TupleLeaf() {}

        const A &get() const
        {
            return value_;
        }

        A &get()
        {
            return value_;
        }

    private:
        A value_;
    };

    // IndexSequence

    template <int... ns>
    struct IndexSequence
    {
    };

    // MakeIndexSequenceImpl

    template <int n, int... ns>
    struct MakeIndexSequenceImpl
        : MakeIndexSequenceImpl<n - 1, n - 1, ns...>
    {
    };

    template <int... ns>
    struct MakeIndexSequenceImpl<0, ns...>
    {
        using Type = IndexSequence<ns...>;
    };

    // MakeIndexSequence

    template <int n>
    using MakeIndexSequence = typename MakeIndexSequenceImpl<n>::Type;

    // IndexSequenceFor

    template <typename... Ts>
    using IndexSequenceFor = MakeIndexSequence<sizeof...(Ts)>;

    // TupleImpl
    template <typename IndexSequence, typename... As>
    class TupleImpl
    {
    };

    template <int... idxs, typename... As>
    class TupleImpl<IndexSequence<idxs...>, As...>
        : public TupleLeaf<idxs, As>...
    {
    public:
        TupleImpl(const As &...as)
            : TupleLeaf<idxs, As>{as}...
        {
        }

    protected:
        template <typename F>
        auto match_impl(const F &f) const
            -> EnableIf<
                IsInvocable<F, As...>::value,
                CallReturn<F, As...>>
        {
            return f(TupleLeaf<idxs, PackAt<idxs, As...>>::get()...);
        }
    };

    // Tuple

    template <typename... As>
    class Tuple
        : public TupleImpl<IndexSequenceFor<As...>, As...>
    {
    public:
        Tuple(const As &...as)
            : TupleImpl<IndexSequenceFor<As...>, As...>{as...}
        {
        }

        template <int idx>
        auto get() const
            -> const PackAt<idx, As...> &
        {
            return TupleLeaf<idx, PackAt<idx, As...>>::get();
        }

        template <int idx>
        auto get()
            -> PackAt<idx, As...> &
        {
            return TupleLeaf<idx, PackAt<idx, As...>>::get();
        }

        template <typename F>
        auto match(const F &f) const
            -> EnableIf<
                IsInvocable<F, As...>::value,
                CallReturn<F, As...>>
        {
            return TupleImpl<IndexSequenceFor<As...>, As...>::match_impl(f);
        }

    private:
    };

    template <int index, typename... As>
    auto get(const Tuple<As...> &tpl)
        -> const PackAt<index, As...> &
    {
        return tpl.template get<index>();
    }

    template <int index, typename... As>
    auto get(Tuple<As...> &tpl)
        -> PackAt<index, As...> &
    {
        return tpl.template get<index>();
    }

    template <typename... As>
    auto tuple(const As &...as)
        -> Tuple<As...>
    {
        return Tuple<As...>{as...};
    }

    template <int index>
    struct TupleLeafComparator
    {
        template <typename... As>
        static bool compare(const Tuple<As...> &lhs, const Tuple<As...> &rhs)
        {
            if (lhs.template get<index>() != rhs.template get<index>())
                return false;
            return TupleLeafComparator<index - 1>::compare(lhs, rhs);
        }
    };

    template <>
    struct TupleLeafComparator<0>
    {
        template <typename... As>
        static bool compare(const Tuple<As...> &lhs, const Tuple<As...> &rhs)
        {
            return lhs.template get<0>() == rhs.template get<0>();
        }
    };

    template <>
    struct TupleLeafComparator<-1>
    {
        template <typename... As>
        static bool compare(const Tuple<As...> &, const Tuple<As...> &)
        {
            return true;
        }
    };

    // Equality operator for Tuple

    template <typename... As>
    bool operator==(const Tuple<As...> &lhs, const Tuple<As...> &rhs)
    {
        return TupleLeafComparator<(int)(sizeof...(As)) - 1>::compare(lhs, rhs);
    }

    template <typename... As>
    bool operator!=(const Tuple<As...> &lhs, const Tuple<As...> &rhs)
    {
        return !(lhs == rhs);
    }
}

#endif