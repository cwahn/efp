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
    //         return Tuple<>
    //     }

    // private:
    // }

}

#endif