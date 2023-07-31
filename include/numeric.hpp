#ifndef NUMERIC_HPP_
#define NUMERIC_HPP_

#include <cmath>

#include "prelude.hpp"

namespace efp
{
    template <typename A, typename B>
<<<<<<< HEAD
    constexpr A max(const A &lhs, const B &rhs)
=======
    constexpr A max(ConstParam_t<A> lhs, ConstParam_t<B> rhs)
>>>>>>> 9e4c10f2d54cdcf0ea90c763f678fc41532f1363
    {
        return max_v(lhs, rhs);
    }

    template <typename A, typename B>
<<<<<<< HEAD
    constexpr A min(const A &lhs, const B &rhs)
=======
    constexpr A min(ConstParam_t<A> lhs, ConstParam_t<B> rhs)
>>>>>>> 9e4c10f2d54cdcf0ea90c763f678fc41532f1363
    {
        return min_v(lhs, rhs);
    }

    template <typename A, typename B>
<<<<<<< HEAD
    constexpr A plus(const A &lhs, const B &rhs)
=======
    constexpr A plus(ConstParam_t<A> lhs, ConstParam_t<B> rhs)
>>>>>>> 9e4c10f2d54cdcf0ea90c763f678fc41532f1363
    {
        return lhs + rhs;
    }

    template <typename A, typename B>
<<<<<<< HEAD
    constexpr A minus(const A &lhs, const B &rhs)
=======
    constexpr A minus(ConstParam_t<A> lhs, ConstParam_t<B> rhs)
>>>>>>> 9e4c10f2d54cdcf0ea90c763f678fc41532f1363
    {
        return lhs - rhs;
    }

    template <typename A, typename B>
<<<<<<< HEAD
    constexpr A times(const A &lhs, const B &rhs)
=======
    constexpr A times(ConstParam_t<A> lhs, ConstParam_t<B> rhs)
>>>>>>> 9e4c10f2d54cdcf0ea90c763f678fc41532f1363
    {
        return lhs * rhs;
    }

    template <typename A, typename B>
<<<<<<< HEAD
    constexpr A devide(const A &lhs, const B &rhs)
=======
    constexpr A devide(ConstParam_t<A> lhs, ConstParam_t<B> rhs)
>>>>>>> 9e4c10f2d54cdcf0ea90c763f678fc41532f1363
    {
        return lhs / rhs;
    }

    template <typename A>
<<<<<<< HEAD
    constexpr A square(const A &a)
=======
    constexpr A square(ConstParam_t<A> a)
>>>>>>> 9e4c10f2d54cdcf0ea90c763f678fc41532f1363
    {
        return a * a;
    }

    template <typename A>
<<<<<<< HEAD
    constexpr A sqrt(const A &a)
=======
    constexpr A sqrt(ConstParam_t<A> a)
>>>>>>> 9e4c10f2d54cdcf0ea90c763f678fc41532f1363
    {
        return std::sqrt(a);
    }

    template <typename A>
<<<<<<< HEAD
    constexpr A sin(const A &a)
=======
    constexpr A sin(ConstParam_t<A> a)
>>>>>>> 9e4c10f2d54cdcf0ea90c763f678fc41532f1363
    {
        return std::sin(a);
    }

    template <typename A>
<<<<<<< HEAD
    constexpr A cos(const A &a)
=======
    constexpr A cos(ConstParam_t<A> a)
>>>>>>> 9e4c10f2d54cdcf0ea90c763f678fc41532f1363
    {
        return std::cos(a);
    }

    template <typename A>
<<<<<<< HEAD
    constexpr A tan(const A &a)
=======
    constexpr A tan(ConstParam_t<A> a)
>>>>>>> 9e4c10f2d54cdcf0ea90c763f678fc41532f1363
    {
        return std::tan(a);
    }

    // Reducing

    template <typename SeqA>
    constexpr Element_t<SeqA> maximum(const SeqA &as)
    {
<<<<<<< HEAD
        return foldl(max, NumericLimits<Element_t<SeqA>>::min(), as);
=======
        return foldl(max<Element_t<SeqA>, Element_t<SeqA>>, NumericLimits<Element_t<SeqA>>::min(), as);
>>>>>>> 9e4c10f2d54cdcf0ea90c763f678fc41532f1363
    }

    template <typename SeqA>
    constexpr Element_t<SeqA> minimum(const SeqA &as)
    {
<<<<<<< HEAD
        return foldl(min, NumericLimits<Element_t<SeqA>>::max(), as);
=======
        return foldl(min<Element_t<SeqA>, Element_t<SeqA>>, NumericLimits<Element_t<SeqA>>::max(), as);
>>>>>>> 9e4c10f2d54cdcf0ea90c763f678fc41532f1363
    }

    template <typename SeqA>
    constexpr Element_t<SeqA> max_min(const SeqA &as)
    {
        return maximum(as) - minimum(as);
    }

    template <typename SeqA>
    constexpr Element_t<SeqA> sum(const SeqA &as)
    {
<<<<<<< HEAD
        return foldl(plus, 0, as);
=======
        return foldl(plus<Element_t<SeqA>, Element_t<SeqA>>, 0, as);
>>>>>>> 9e4c10f2d54cdcf0ea90c763f678fc41532f1363
    }

    template <typename SeqA>
    constexpr Element_t<SeqA> product(const SeqA &as)
    {
<<<<<<< HEAD
        return foldl(times, 1, as);
=======
        return foldl(times<Element_t<SeqA>, Element_t<SeqA>>, 1, as);
>>>>>>> 9e4c10f2d54cdcf0ea90c763f678fc41532f1363
    }
}

#endif