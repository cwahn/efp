#ifndef SM_HPP_
#define SM_HPP_

#include "sfinae.hpp"

namespace efp
{
    // template <typename S, typename I, typename O>
    // class SmTrait;

    // template <typename Derived>
    // class SmBase
    // {
    // public:
    //     using Trait = SmTrait<Derived>;

    //     using S = Trait::S;
    //     using I = Trait::I;
    //     using O = Trait::O;

    //     inline Tuple<S, O> react(const S &s, const I &i)
    //     {
    //         return derived().react(s, i);
    //     }

    // private:
    //     inline &derived()
    //     {
    //         return *static_cast<Derived *>(this);
    //     }
    //     inline const Derived &derived() const
    //     {
    //         return *static_cast<const Derived *>(this);
    //     }
    // };

    // template <typename Derived>
    // using Sm = SmBase<Derived>;

    // template <typename>
    // struct IsSm : False
    // {
    // };

    // template <typename Derived>
    // struct IsSm<Sm<Derived>> : True
    // {
    // };

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
    // }

    // template <typename G, typename F, typename = EnableIf < IsSm<G>::value && IsSm<F>::value, void >> class Compose<G, F> : SmBase<Compose<G, F>>
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