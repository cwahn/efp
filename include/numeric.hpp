#ifndef NUMERIC_HPP_
#define NUMERIC_HPP_

#include <cmath>

#include "prelude.hpp"

namespace efp
{
    template <typename A, typename B>
    constexpr A max(ConstParam_t<A> lhs, ConstParam_t<B> rhs)
    {
        return max_v(lhs, rhs);
    }

    template <typename A, typename B>
    constexpr A min(ConstParam_t<A> lhs, ConstParam_t<B> rhs)
    {
        return min_v(lhs, rhs);
    }

    template <typename A, typename B>
    constexpr A plus(ConstParam_t<A> lhs, ConstParam_t<B> rhs)
    {
        return lhs + rhs;
    }

    template <typename A, typename B>
    constexpr A minus(ConstParam_t<A> lhs, ConstParam_t<B> rhs)
    {
        return lhs - rhs;
    }

    template <typename A, typename B>
    constexpr A times(ConstParam_t<A> lhs, ConstParam_t<B> rhs)
    {
        return lhs * rhs;
    }

    template <typename A, typename B>
    constexpr A devide(ConstParam_t<A> lhs, ConstParam_t<B> rhs)
    {
        return lhs / rhs;
    }

    template <typename A>
    constexpr A square(ConstParam_t<A> a)
    {
        return a * a;
    }

    template <typename A>
    constexpr A sqrt(ConstParam_t<A> a)
    {
        return std::sqrt(a);
    }

    template <typename A>
    constexpr A sin(ConstParam_t<A> a)
    {
        return std::sin(a);
    }

    template <typename A>
    constexpr A cos(ConstParam_t<A> a)
    {
        return std::cos(a);
    }

    template <typename A>
    constexpr A tan(ConstParam_t<A> a)
    {
        return std::tan(a);
    }

    // Reducing

    template <typename SeqA>
    constexpr Element_t<SeqA> maximum(const SeqA &as)
    {
        return foldl(max<Element_t<SeqA>, Element_t<SeqA>>, NumericLimits<Element_t<SeqA>>::min(), as);
    }

    template <typename SeqA>
    constexpr Element_t<SeqA> minimum(const SeqA &as)
    {
        return foldl(min<Element_t<SeqA>, Element_t<SeqA>>, NumericLimits<Element_t<SeqA>>::max(), as);
    }

    template <typename SeqA>
    constexpr Element_t<SeqA> max_min(const SeqA &as)
    {
        return maximum(as) - minimum(as);
    }

    template <typename SeqA>
    constexpr Element_t<SeqA> sum(const SeqA &as)
    {
        return foldl(plus<Element_t<SeqA>, Element_t<SeqA>>, 0, as);
    }

    template <typename SeqA>
    constexpr Element_t<SeqA> product(const SeqA &as)
    {
        return foldl(times<Element_t<SeqA>, Element_t<SeqA>>, 1, as);
    }
}

#endif