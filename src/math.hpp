#ifndef MATH_HPP_
#define MATH_HPP_

#include "pure.hpp"

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
auto sum(A &xs) -> ElementType_t<A>
{
    return foldl(plus<ElementType_t<A>>, 0, xs);
}

template <typename A>
auto sum(A &&xs) -> ElementType_t<A>
{
    return foldl(plus<ElementType_t<A>>, 0, xs);
}

template <typename A>
auto product(A &xs) -> ElementType_t<A>
{
    return foldl(times<ElementType_t<A>>, 1, xs);
}

template <typename A>
auto product(A &&xs) -> ElementType_t<A>
{
    return foldl(times<ElementType_t<A>>, 1, xs);
}

template <typename A>
auto mean(A &xs) -> ElementType_t<A>
{
    if (IsArray<A>::value)
    {
        return sum(xs) / ArrayLength<A>::value;
    }
    else
    {
        return sum(xs) / get_length(xs);
    }
}

template <typename A>
auto mean(A &&xs) -> ElementType_t<A>
{
    if (IsArray<A>::value)
    {
        return sum(xs) / ArrayLength<A>::value;
    }
    else
    {
        return sum(xs) / get_length(xs);
    }
}

#endif