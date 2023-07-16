#ifndef NUMERIC_HPP_
#define NUMERIC_HPP_

#include "prelude.hpp"
// ! temp
// #include "arduino_debug.hpp"

namespace efp
{
    template <typename A>
    constexpr A max(const A lhs, const A rhs)
    {
        return std::max(lhs, rhs);
    }

    template <typename A>
    constexpr A min(const A lhs, const A rhs)
    {
        return std::min(lhs, rhs);
    }

    template <typename A>
    constexpr A plus(const A lhs, const A rhs)
    {
        return lhs + rhs;
    }

    template <typename A>
    constexpr A minus(const A lhs, const A rhs)
    {
        return lhs - rhs;
    }

    template <typename A>
    constexpr A times(const A lhs, const A rhs)
    {
        return lhs * rhs;
    }

    template <typename A>
    constexpr A devide(const A lhs, const A rhs)
    {
        return lhs / rhs;
    }

    template <typename A>
    constexpr A square(const A a)
    {
        return a * a;
    }

    template <typename A>
    constexpr A sqrt(const A a)
    {
        return std::sqrt(a);
    }

    template <typename A>
    constexpr A sin(const A a)
    {
        return std::sin(a);
    }

    template <typename A>
    constexpr A cos(const A a)
    {
        return std::cos(a);
    }

    template <typename A>
    constexpr A tan(const A a)
    {
        return std::tan(a);
    }

    // Reducing

    template <typename SeqA>
    constexpr Element_t<SeqA> maximum(const SeqA &as)
    {
        return foldl(max<Element_t<SeqA>>, std::numeric_limits<Element_t<SeqA>>::min(), as);
    }

    template <typename SeqA>
    constexpr Element_t<SeqA> minimum(const SeqA &as)
    {
        return foldl(min<Element_t<SeqA>>, std::numeric_limits<Element_t<SeqA>>::max(), as);
    }

    template <typename SeqA>
    constexpr Element_t<SeqA> max_min(const SeqA &as)
    {
        return maximum(as) - minimum(as);
    }

    template <typename SeqA>
    constexpr Element_t<SeqA> sum(const SeqA &as)
    {
        return foldl(plus<Element_t<SeqA>>, 0, as);
    }

    template <typename SeqA>
    constexpr Element_t<SeqA> product(const SeqA &as)
    {
        return foldl(times<Element_t<SeqA>>, 1, as);
    }

    template <typename SeqA>
    constexpr double mean(const SeqA &as)
    {
        return sum(as) / double(length(as));
    }
}

#endif