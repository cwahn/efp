#ifndef MAYBE_HPP_
#define MAYBE_HPP_

#include "sfinae.hpp"
#include "trait.hpp"
#include "enum_type.hpp"

namespace efp {
struct Nothing {
};

constexpr Nothing nothing;

// Specialization of Enum for Maybe

template <typename A>
class Enum<Nothing, A> : public detail::EnumBase<Nothing, A> {
public:
    using Base = detail::EnumBase<Nothing, A>;
    using Base::Base;

    bool is_nothing() const {
        return Base::index() == 0;
    }

    bool has_value() const {
        return Base::index() == 1;
    }

    // ! Will crash if empty
    A value() const {
        return Base::template get<A>();
    }

    A&& move() {
        return Base::template move<A>();
    }

    const A&& move() const {
        return Base::template move<A>();
    }

    explicit operator bool() const {
        return has_value();
    }

    template <typename F>
    auto fmap(const F& f)
        -> Enum<Nothing, CallReturn<F, A>> {
        if (has_value())
            return f(value());
        else
            return nothing;
    }
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
auto pure(const Element<A>& a)
    -> EnableIf<
        IsMaybe<A>::value, A> {
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

// #define try_(expr)          \
//     auto&& _tmp_ = (expr);  \
//     if (!_tmp_.has_value()) \
//         return nothing;     \
//     _tmp_.value()

} // namespace efp

#endif