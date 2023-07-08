#ifndef NUMERIC_HPP_
#define NUMERIC_HPP_

#include "prelude.hpp"

template <typename A>
A max(A lhs, A rhs)
{
    return std::max(lhs, rhs);
}

template <typename A>
A min(A lhs, A rhs)
{
    return std::min(lhs, rhs);
}

template <typename A>
A plus(A lhs, A rhs)
{
    return lhs + rhs;
}

template <typename A>
A minus(A lhs, A rhs)
{
    return lhs - rhs;
}

template <typename A>
A times(A lhs, A rhs)
{
    return lhs * rhs;
}

template <typename A>
A devide(A lhs, A rhs)
{
    return lhs / rhs;
}

template <typename A>
A square(A x)
{
    return x * x;
}

template <typename A>
A sqrt(A x)
{
    return sqrt(x);
}

template <typename A>
A sin(A x)
{
    return std::sin(x);
}

template <typename A>
A cos(A x)
{
    return std::cos(x);
}

template <typename A>
A tan(A x)
{
    return std::tan(x);
}

template <typename A>
ElementType_t<A> maximum(A &&xs)
{
    return foldl(max<ElementType_t<A>>, std::numeric_limits<ElementType_t<A>>::min(), xs);
}

template <typename A>
ElementType_t<A> minimum(A &&xs)
{
    return foldl(min<ElementType_t<A>>, std::numeric_limits<ElementType_t<A>>::max(), xs);
}

template <typename A>
ElementType_t<A> max_min(A &&xs)
{
    return maximum(xs) - minimum(xs);
}

template <typename A>
ElementType_t<A> sum(A &&xs)
{
    return foldl(plus<ElementType_t<A>>, 0, xs);
}

template <typename A>
ElementType_t<A> product(A &&xs)
{
    return foldl(times<ElementType_t<A>>, 1, xs);
}

template <typename A>
double mean(A &&xs)
{
    if constexpr (IsArray<A>::value)
    {
        return sum(xs) / double(ArrayLength<A>::value);
    }
    else
    {
        return sum(xs) / double(get_length(xs));
    }
}
#endif