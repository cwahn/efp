#ifndef MAYBE_HPP_
#define MAYBE_HPP_

#include "sfinae.hpp"
#include "trait.hpp"
#include "enum_type.hpp"

namespace efp {
struct Nothing {
};

constexpr Nothing nothing;

// template <typename A>
// class Maybe : public Enum<Nothing, A>
// {
// public:
//     template <typename T, typename = EnableIf<any_v(IsSame<T, Nothing>::value, IsSame<T, A>::value)>>
//     Maybe(const T &a) : Enum<Nothing, A>(a)
//     {
//     }

//     template <typename T, typename = EnableIf<any_v(IsSame<T, Nothing>::value, IsSame<T, A>::value)>>
//     Maybe(T &&a) : Enum<Nothing, A>(efp::forward<T>(a))
//     {
//     }

//     bool is_nothing() const
//     {
//         return Enum<Nothing, A>::index() == 0;
//     }

//     bool has_value() const
//     {
//         return Enum<Nothing, A>::index() == 1;
//     }

//     // ! Will crash if empty
//     A value() const
//     {
//         return Enum<Nothing, A>::template get<A>();
//     }

//     A &&move()
//     {
//         return Enum<Nothing, A>::template move<A>();
//     }

//     const A &&move() const
//     {
//         return Enum<Nothing, A>::template move<A>();
//     }

//     explicit operator bool() const
//     {
//         return has_value();
//     }
// };

// Specialization of Enum for Maybe

template <typename T>
class Enum<Nothing, T> {
public:
    template <size_t n, typename... Bs>
    friend struct Match;

    using Element = T;

    template <typename A>
    struct IsSameUnary {
        template <typename B>
        struct Binded {
            static const bool value = IsSame<A, B>::value;
        };
    };

    template <typename A>
    struct VariantIndex
        : Find<IsSameUnary<A>::template Binded, Nothing, T> {
    };

    Enum() {
    }

    template <typename A, typename = EnableIf<any_v(IsSame<A, Nothing>::value, IsSame<A, T>::value)>>
    Enum(const A& a)
        : index_(VariantIndex<A>::value) {
        new (reinterpret_cast<A*>(storage_)) A(a);
    }

    template <typename A, typename = EnableIf<any_v(IsSame<A, Nothing>::value, IsSame<A, T>::value)>>
    Enum(A&& a)
        : index_(VariantIndex<A>::value) {
        new (reinterpret_cast<A*>(storage_)) A(efp::move(a));
    }

    ~Enum() {}

    uint8_t index() const {
        return index_;
    }

    template <typename A>
    auto get() const
        -> EnableIf<any_v(IsSame<A, Nothing>::value, IsSame<A, T>::value), A> {
        if (index_ != VariantIndex<A>::value) {
            abort();
        }

        return *(reinterpret_cast<const A*>(storage_));
    }

    template <uint8_t n>
    auto get()
        -> EnableIf<lt_v(n, 2), PackAt<n, Nothing, T>> {
        if (index_ != n) {
            abort();
        }

        return *(reinterpret_cast<PackAt<n, Nothing, T>*>(storage_));
    }

    template <typename A = T>
    auto move() const
        -> EnableIf<any_v(IsSame<A, Nothing>::value, IsSame<A, T>::value), const A&&> {
        if (index_ != VariantIndex<A>::value) {
            abort();
        }

        return efp::move(*(reinterpret_cast<const A*>(storage_)));
    }

    template <typename A = T>
    auto move()
        -> EnableIf<any_v(IsSame<A, Nothing>::value, IsSame<A, T>::value), A&&> {
        if (index_ != VariantIndex<A>::value) {
            abort();
        }

        return efp::move(*(reinterpret_cast<A*>(storage_)));
    }

    template <uint8_t n>
    auto move() const
        -> EnableIf<lt_v(n, 2), PackAt<n, Nothing, T> const&&> {
        if (index_ != n) {
            abort();
        }

        return efp::move(*(reinterpret_cast<const PackAt<n, Nothing, T>*>(storage_)));
    }

    template <uint8_t n>
    auto move()
        -> EnableIf<lt_v(n, 2), PackAt<n, Nothing, T>&&> {
        if (index_ != n) {
            abort();
        }

        return efp::move(*(reinterpret_cast<PackAt<n, Nothing, T>*>(storage_)));
    }

    // * Test if all of the branchs have same return type by Common.
    // * Check if each F has at least one matching with As. (RelevantBranch)
    // * Check if all the variants are matched at least one of Fs or there is wildcard at last (Exhaustiveness)
    // * Compile time function uint8_t -> uint8_t, which is variant index to branch index.

    // * Test if matchs by
    // * 1. is there is_same operator (Strip ref of arg type.)
    // * 2. is invocable
    // * 3. is void arg type

    // * Need type level maybe
    // * Need type level find (Type -> bool) -> Types -> int
    // * Need type level indexing

    // Arguments implementation will automatically remove the const qualifier if there is.

    template <typename F>
    struct IsRelevantBranch {
        static constexpr bool value =
            any_v(IsInvocable<F, Nothing>::value, IsInvocable<F, T>::value) || IsSame<Tuple<>, Arguments<F>>::value;
    };

    template <typename... Fs>
    struct AreAllRelevantBranchs {
        static constexpr bool value = all_v(IsRelevantBranch<Fs>::value...);
    };

    template <typename F>
    struct IsWildCard : IsSame<Tuple<>, Arguments<F>> {
    };

    template <typename A, typename... Fs>
    struct IsVariantCovered {
        static constexpr bool value =
            any_v(IsWildCard<Fs>::value...) || any_v(IsInvocable<Fs, A>::value...);
    };

    template <typename... Fs>
    struct IsExhaustive : False {
        static constexpr bool value = all_v(IsVariantCovered<Nothing, Fs...>::value, IsVariantCovered<T, Fs...>::value);
    };

    template <typename... Fs>
    struct IsWellFormed {
        static constexpr bool value = true;
        // todo count and get last
        // (!any_v(IsWildCard<Init>::value...) && IsWildCard<Last>::value) ||
        // !any_v(IsWildCard<Fs...>::value...);
    };

    template <typename... Fs>
    auto match(const Fs&... fs) const
        -> EnableIf<
            AreAllRelevantBranchs<Fs...>::value &&
                IsExhaustive<Fs...>::value &&
                IsWellFormed<Fs...>::value,
            Common<Return<Fs>...>> {
        return Match<power_2_ceiling(2), Nothing, T>::impl(Overloaded<MatchBranch<Fs>...>{fs...}, this);
    }

    // * Specialization for Maybe

    bool is_nothing() const {
        return index_ == 0;
    }

    bool has_value() const {
        return index_ == 1;
    }

    // ! Will crash if empty
    T value() const {
        return get<T>();
    }

    explicit operator bool() const {
        return has_value();
    }

private:
    alignas(maximum_v(alignof(Nothing), alignof(T))) uint8_t storage_[maximum_v(sizeof(Nothing), sizeof(T))];
    uint8_t index_;
};

template <typename A>
using Maybe = Enum<Nothing, A>;

template <typename A>
struct IsMaybe : False {
};

template <typename A>
struct IsMaybe<Maybe<A>> : True {
};

// Element specializetion for Maybe
template <typename A>
struct ElementImpl<Enum<Nothing, A>> {
    using Type = A;
};

// functor

template <typename A, typename F>
auto fmap(const F& f, const Maybe<A>& ma)
    -> Maybe<CallReturn<F, A>> {
    if (ma)
        return f(ma.value());
    else
        return nothing;
}

// applicative
template <typename A>
auto pure(const A& a)
    -> EnableIf<
        IsMaybe<A>::value,
        A> {
    return a;
}

template <typename A, typename F>
auto ap(const Maybe<F>& mf, const Maybe<A>& ma)
    -> Maybe<CallReturn<F, A>> {
    if (mf && ma)
        return mf.value()(ma.value());
    else
        return nothing;
}

// monad

template <typename A, typename F>
auto bind(const Maybe<A>& ma, const F& f)
    -> EnableIf<
        IsMaybe<CallReturn<F, A>>::value,
        CallReturn<F, A>>
// todo test if monadic action
{
    if (ma)
        return f(ma.value());
    else
        return nothing;
}

template <typename A, typename F>
auto operator>>=(const Maybe<A>& ma, const F& f)
    -> EnableIf<
        IsMaybe<CallReturn<F, A>>::value,
        CallReturn<F, A>>
// todo test if monadic action
{
    if (ma)
        return f(ma.value());
    else
        return nothing;
}

// #define try_(expr) \
//     ({ auto&& _tmp_##__COUNTER__ = (expr); _tmp_##__COUNTER__ ? _tmp_##__COUNTER__.value() : return nothing })

#define try_(expr)          \
    auto&& _tmp_ = (expr);  \
    if (!_tmp_.has_value()) \
        return nothing;     \
    _tmp_.value()

} // namespace efp

#endif