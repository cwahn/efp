#ifndef NUMERIC_HPP_
#define NUMERIC_HPP_

#include "prelude.hpp"
// ! temp
// #include "arduino_debug.hpp"

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
constexpr A square(const A x)
{
    return x * x;
}

template <typename A>
constexpr A sqrt(const A x)
{
    return sqrt(x);
}

template <typename A>
constexpr A sin(const A x)
{
    return std::sin(x);
}

template <typename A>
constexpr A cos(const A x)
{
    return std::cos(x);
}

template <typename A>
constexpr A tan(const A x)
{
    return std::tan(x);
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

#endif