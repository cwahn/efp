#ifndef MAYBE_HPP_
#define MAYBE_HPP_

#include "sfinae.hpp"
#include "enum_type.hpp"

namespace efp
{
    struct Nothing
    {
    };

    constexpr Nothing nothing;

    template <typename A>
    class Maybe : public Enum<Nothing, A>
    {
    public:
        template <typename T, typename = EnableIf<any_v(IsSame<T, Nothing>::value, IsSame<T, A>::value)>>
        Maybe(const T &a) : Enum<Nothing, A>(a)
        {
        }

        template <typename T, typename = EnableIf<any_v(IsSame<T, Nothing>::value, IsSame<T, A>::value)>>
        Maybe(T &&a) : Enum<Nothing, A>(efp::forward<T>(a))
        {
        }

        bool is_nothing() const
        {
            return Enum<Nothing, A>::index() == 0;
        }

        bool has_value() const
        {
            return Enum<Nothing, A>::index() == 1;
        }

        // ! Will crash if empty
        A value() const
        {
            return Enum<Nothing, A>::template get<A>();
        }

        A &&move()
        {
            return Enum<Nothing, A>::template move<A>();
        }

        const A &&move() const
        {
            return Enum<Nothing, A>::template move<A>();
        }

        explicit operator bool() const
        {
            return has_value();
        }
    };

    template <typename A>
    struct IsMaybe : False
    {
    };

    template <typename A>
    struct IsMaybe<Maybe<A>> : True
    {
    };

    // functor

    template <typename A, typename F>
    auto fmap(const F &f, const Maybe<A> &ma)
        -> Maybe<CallReturn<F, A>>
    {
        if (ma)
            return f(ma.value());

        return nothing;
    }

    // applicative
    template <typename A>
    auto pure(const A &a)
        -> EnableIf<
            IsMaybe<A>::value,
            A>
    {
        return a;
    }

    template <typename A, typename F>
    auto ap(const Maybe<F> &mf, const Maybe<A> &ma)
        -> Maybe<CallReturn<F, A>>
    {
        if (mf.is_nothing() || ma.is_nothing())
        {
            return nothing;
        }

        return mf.value()(ma.value());
    }

    // monad

    template <typename A, typename F>
    auto bind(const Maybe<A> &ma, const F &f)
        -> EnableIf<
            IsMaybe<CallReturn<F, A>>::value,
            CallReturn<F, A>>
    // todo test if monadic action
    {
        if (ma.is_nothing())
        {
            return nothing;
        }

        return f(ma.value());
    }

    template <typename A, typename F>
    auto operator>>=(const Maybe<A> &ma, const F &f)
        -> EnableIf<
            IsMaybe<CallReturn<F, A>>::value,
            CallReturn<F, A>>
    // todo test if monadic action
    {
        if (ma.is_nothing())
        {
            return nothing;
        }

        return f(ma.value());
    }

    // #define try_(expr) \
//     ({ auto&& _tmp_##__COUNTER__ = (expr); _tmp_##__COUNTER__ ? _tmp_##__COUNTER__.value() : return nothing })

#define try_(expr)          \
    auto &&_tmp_ = (expr);  \
    if (!_tmp_.has_value()) \
        return nothing;     \
    _tmp_.value()

}

#endif