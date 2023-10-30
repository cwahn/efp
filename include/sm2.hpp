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

    template <typename Derived, typename I = typename SmTrait<Derived>::I>
    class SmBase
    {
    };

    template <typename Derived, typename... Is>
    class SmBase<Derived, Tuple<Is...>>
    {
    public:
        using Trait = SmTrait<Derived>;

        using I = typename Trait::I;
        using O = typename Trait::O;

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

    // Pure

    template <typename F, typename I = Arguments<F>>
    class Pure;

    // Specialization for regular function pointers
    // template <typename Ret, typename... Is>
    // class Pure<Ret (*)(Is...)> : public Sm<Pure<Ret (*)(Is...)>>
    // {
    // public:
    //     using I = Tuple<Is...>; // Assuming Is... are the arguments of the function F.
    //     using O = Ret;

    //     Pure(Ret (*f)(Is...)) : f_{f} {}

    //     O operator()(Is... is)
    //     {
    //         return f_(is...);
    //     }

    // private:
    //     Ret (*f_)(Is...);
    // };

    template <typename Ret, typename... Is>
    class SmTrait<Pure<Ret (*)(Is...)>>
    {
    public:
        using I = Tuple<Is...>;
        using O = Ret;
    };

    // Specialization for lambdas and other callables
    template <typename F, typename... Is>
    class Pure<F, Tuple<Is...>> : public Sm<Pure<F, Tuple<Is...>>>
    {
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

    // Compose

    // ? Does it needs to have input pointer? No it doesn't have to
    // ? What should be in the update function? Nothing if the f and g already have pointers of there sources.
    // Which means it does not need to do anything on update but calling update of _f and _g
    // Why don't we just make it to hold the pointers of input sms so that it can use it any time.
    // So what should unit sm should holds?
    //

    template <typename G, typename F, typename I = typename F::I>
    class Compose
    {
    };

    template <typename G, typename F, typename... Is>
    class Compose<G, F, Tuple<Is...>> : public Sm<Compose<G, F, Tuple<Is...>>>
    {
    public:
        using I = Arguments<F>;
        using O = Return<G>;

        Compose(const Sm<G> &g, const Sm<F> &f)
            : _f{f}, _g{g}
        {
        }

        // O operator()(const Is &...is)
        // {
        //     return g_(f_(is...));
        // }

        // ! Output types of InputSms should be the same with Is...
        template <typename... ISms>
        inline void init(const ISms &...isms)
        {
            _f.init(i_sms...);
            _g.init(_f);
        }

        inline void update()
        {
            _f.update();
            _g.update();
        }

        inline O value()
        {
            _g.value();
        }

    private:
        Sm<F> _f;
        Sm<G> _g;
    };

    // template <typename G, typename F, typename I = typename F::I>
    // class Compose
    // {
    // };

    // template <typename G, typename F, typename... Is>
    // class Compose<G, F, Tuple<Is...>> : public Sm<Compose<G, F, Tuple<Is...>>>
    // {
    // public:
    //     using I = Arguments<F>;
    //     using O = Return<G>;

    //     Compose(const Sm<G> &g, const Sm<F> &f)
    //         : f_{f}, g_{g}
    //     {
    //     }

    //     O operator()(const Is &...is)
    //     {
    //         return g_(f_(is...));
    //     }

    // private:
    //     Sm<F> f_;
    //     Sm<G> g_;
    // };

    template <typename G, typename F>
    class SmTrait<Compose<G, F>>
    {
    public:
        using I = Arguments<F>;
        using O = Return<G>;
    };

    // typename = EnableIf<IsSm<F>::value && IsSm<G>::value, void>
    template <typename G, typename F>
    Compose<G, F> sm_compose(const Sm<G> &g, const Sm<F> &f)
    {
        return Compose<G, F>{g, f};
    }

    // Concat

    template <typename F, typename G, typename FI = typename F::I, typename GI = typename G::I>
    class Concat
    {
    };

    template <typename F, typename G, typename... FIs, typename... GIs>
    class Concat<F, G, Tuple<FIs...>, Tuple<GIs...>> : public Sm<Concat<F, G, Tuple<FIs...>, Tuple<GIs...>>>
    {
    public:
        using I = Tuple<FIs..., GIs...>;
        using O = Tuple<typename F::O, typename G::O>;

        Concat(const Sm<F> &f, const Sm<G> &g)
            : f_{f}, g_{g}
        {
        }

        O operator()(const FIs &...fis, const GIs &...gis)
        {
            return tuple(f_(fis...), g_(gis...));
        }

    private:
        Sm<F> f_;
        Sm<G> g_;
    };

    template <typename G, typename F>
    class SmTrait<Concat<G, F>>
    {
    public:
        using I = Tuple<Arguments<F>, Arguments<G>>;
        using O = Tuple<Return<F>, Return<G>>;
    };

    template <typename F, typename G>
    Concat<F, G> sm_concat(const Sm<F> &f, const Sm<G> &g)
    {
        return Concat<F, G>{f, g};
    }

}

#endif