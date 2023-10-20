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

                inline O operator()(const I &i)
        {
            return derived()(i);
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

    template <typename>
    struct IsSm : False
    {
    };

    template <typename Derived>
    struct IsSm<Sm<Derived>> : True
    {
    };

    template <typename F>
    class Pure;

    // Specialization for regular function pointers
    template <typename Ret, typename... Args>
    class Pure<Ret (*)(Args...)>
    {
    public:
        using I = Tuple<Args...>; // Assuming Args... are the arguments of the function F.
        using O = Ret;

        Pure(Ret (*f)(Args...)) : f_{f} {}

        O operator()(Args... args)
        {
            return f_(args...);
        }

    private:
        Ret (*f_)(Args...);
    };

    // Specialization for lambdas and other callables
    template <typename F>
    class Pure
    {
    public:
        using I = Arguments<F>;
        using O = Return<F>;

        Pure(const F &f) : f_{f}
        {
            static_assert(IsDefaultConstructible<F>::value, "Lambda must be stateless");
        }

        O operator()(const I &i)
        {
            return f_(i);
        }

    private:
        F f_;
    };

    template <typename F>
    auto pure_sm(F f)
    {
        return Pure<F>{f};
    }

    template <typename G, typename F>
    class Compose : Sm<Compose<G, F>>
    {
    public:
        using I = Arguments<F>;
        using O = Return<G>;

        Compose(const G &g, const F &f)
            : f_{f}, g_{g}
        {
        }

        O operator()(const I &i)
        {
            return g_(f_(i));
        }

    private:
        F f_;
        G g_;
    };

    template <typename F, typename G>
    class Concat : Sm<Concat<F, G>>
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