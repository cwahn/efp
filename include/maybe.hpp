#ifndef MAYBE_HPP_
#define MAYBE_HPP_

#include "sfinae.hpp"
#include "enum_type.hpp"

namespace efp
{
    struct Nothing
    {
    };

    template <typename A>
    class Maybe : public Enum<Nothing, A>
    {
    public:
        using Enum<Nothing, A>::Enum;

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
        if (ma.is_nothing())
        {
            return Nothing{};
        }

        return f(ma.value());

        // return ma.match(
        //     [](Nothing)
        //     { return Nothing{}; },
        //     [&](A a)
        //     { return f(a); });
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
            return Nothing{};
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
            return Nothing{};
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
            return Nothing{};
        }

        return f(ma.value());
    }

}

#endif