#ifndef SM_HPP_
#define SM_HPP_

#include "sfinae.hpp"

namespace efp
{
    // I found no good reason to make state external.
    // Make state internal and think state machine as state with extra function.
    // State machine will holds sub-state machine as member variable.

    template <typename Derived>
    class SmTrait;

    template <typename Derived>
    class SmBase
    {
    public:
        using Trait = SmTrait<Derived>;

        // using S = Trait::S;
        using I = typename Trait::I;
        using O = typename Trait::O;

        template <typename... Is>
        inline O operator()(const Is &...is)
        {
            return derived()(is...);
        }

    private:
        inline Derived &derived()
        {
            return *static_cast<Derived *>(this);
        }
        inline const Derived &derived() const
        {
            return *static_cast<const Derived *>(this);
        }
    };

    template <typename Derived>
    using Sm = SmBase<Derived>;

    // IsSm

    template <typename>
    struct IsSm : False
    {
    };

    // Pure

    template <typename F, typename I = Arguments<F>>
    class Pure;

    // // Specialization for regular function pointers
    // template <typename Ret, typename... Args>
    // class Pure<Ret (*)(Args...)> : public Sm<Pure<Ret (*)(Args...)>>
    // {
    // public:
    //     using I = Tuple<Args...>; // Assuming Args... are the arguments of the function F.
    //     using O = Ret;

    //     Pure(Ret (*f)(Args...)) : f_{f} {}

    //     O operator()(Args... args)
    //     {
    //         return f_(args...);
    //     }

    // private:
    //     Ret (*f_)(Args...);
    // };

    // template <typename Ret, typename... Args>
    // class SmTrait<Pure<Ret (*)(Args...)>>
    // {
    // public:
    //     using I = Tuple<Args...>;
    //     using O = Ret;
    // };

    // Specialization for lambdas and other callables
    template <typename F, typename... Is>
    class Pure<F, Tuple<Is...>> : public Sm<Pure<F, Tuple<Is...>>>
    {
        // static_assert(IsSame<Arguments<F>, Tuple<FIs...>>::value, "");

    public:
        using I = Arguments<F>;
        using O = Return<F>;

        Pure(const F &f) : f_{f}
        {
            // static_assert(IsDefaultConstructible<F>::value, "Lambda must be stateless");
            // todo check if referentially transparent
        }

        O operator()(const Is &...is)
        {
            return f_(is...);
        }

    private:
        F f_;
    };

    template <typename F>
    class SmTrait<Pure<F>>
    {
    public:
        using I = Arguments<F>;
        using O = Return<F>;
    };

    template <typename F>
    Pure<F> pure_sm(F f)
    {
        return Pure<F>{f};
    }

    template <typename G, typename F, typename I = typename SmTrait<F>::I>
    class Compose
    {
    };

    template <typename G, typename F, typename... Is>
    class Compose<G, F, Tuple<Is...>> : public Sm<Compose<G, F, Tuple<Is...>>>
    {
        // static_assert(IsSame<Arguments<F>, Tuple<FIs...>>::value, "");

    public:
        using I = Arguments<F>;
        using O = Return<G>;

        Compose(const Sm<G> &g, const Sm<F> &f)
            : f_{f}, g_{g}
        {
        }

        O operator()(const Is &...is)
        {
            return g_(f_(is...));
        }

    private:
        Sm<F> f_;
        Sm<G> g_;
    };

    template <typename G, typename F, typename _>
    class SmTrait<Compose<G, F, _>>
    {
    public:
        using I = Arguments<F>;
        using O = Return<G>;
    };

    // typename = EnableIf<IsSm<F>::value && IsSm<G>::value, void>
    template <typename G, typename F>
    Compose<G, F> sm_compose(const Sm<G> &g, const Sm<F> &f)
    {
        return Compose<G, F, typename F::I>{g, f};
    }

    template <typename F, typename G>
    class Concat : public Sm<Concat<F, G>>
    {
    public:
        using I = Tuple<Arguments<F>, Arguments<G>>;
        using O = Tuple<Return<F>, Return<G>>;

        Concat(const G &g, const F &f)
            : f_{f}, g_{g}
        {
        }

        O operator()(const I &i)
        {
            return tuple(f_(get<0>(i)), g_(get<1>(i)));
        }

    private:
        F f_;
        G g_;
    };

    template <typename G, typename F>
    class SmTrait<Concat<G, F>>
    {
    public:
        using I = Tuple<Arguments<F>, Arguments<G>>;
        using O = Tuple<Return<F>, Return<G>>;
    };

    // IsSm

    // template <typename>
    // struct IsSm : False
    // {
    // };

    template <typename Derived>
    struct IsSm<Sm<Derived>> : True
    {
    };

    template <typename A>
    struct IsSm<A &> : IsSm<A>
    {
    };

    template <typename A>
    struct IsSm<A &&> : IsSm<A>
    {
    };

    template <typename A>
    struct IsSm<const A> : IsSm<A>
    {
    };

    // template <typename F>
    // class Pure : SmBase<Pure<F>>
    // {
    // public:
    //     using S = Tuple<>;
    //     using I = Arguments<F>;
    //     using O = Return<F>;

    //     Pure(const F &f) : f_{f}
    //     {
    //     }

    //     Tuple<S, I> react(const S &s, const I &i)
    //     {
    //         return tuple(tuple(), f_(i));
    //     }

    // private:
    //     F f_;
    // };

    // template <typename G, typename F, typename = EnableIf<IsSm<G>::value && IsSm<F>::value, void>>
    // class Compose<G, F> : SmBase<Compose<G, F>>
    // {
    // public:
    //     using S = Tuple<typename F::S, typename G::S>;
    //     using I = typename F::I;
    //     using O = typename F::O;

    //     Tuple<S, O> react(const S &s, const I &i)
    //     {
    //         const auto f_ = f(get<0>(s), i);
    //         const auto g_ = f(get<1>(s), get<1>(f_));
    //         return tuple(tuple(get<0>(f_), get<0>(g_)), get<1>(g_));
    //     }
    // };

    // template <typename F, typename G, typename = EnableIf<IsSm<G>::value && IsSm<F>::value, void>>
    // class Concat<F, G> : SmBase<Concat<F, G>>
    // {
    // public:
    //     using S = Tuple<typename F::S, typename G::S>;
    //     using I = Tuple<typename F::I, typename G::I>;
    //     using O = Tuple<typename F::O, typename G::O>;

    //     Tuple<S, O> react(const S &s, const I &i)
    //     {
    //         const auto f_ = f(get<0>(s), get<0>(i));
    //         const auto g_ = f(get<1>(s), get<1>(i));
    //         return tuple(tuple(get<0>(f_), get<0>(g_)), tuple(get<1>(f_), get<1>(g_)));
    //     }
    // }

}

#endif