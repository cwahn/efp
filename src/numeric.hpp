#ifndef NUMERIC_HPP_
#define NUMERIC_HPP_

#include "prelude.hpp"
// ! temp
// #include "arduino_debug.hpp"

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

// Reducing

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
    if constexpr (IsStatic<A>::value)
    {
        return sum(xs) / double(StaticCapacity<A>::value);
    }
    else
    {
        return sum(xs) / double(length(xs));
    }
}

// Generating
// ! Temp no constexpr
template <typename A>
size_t arange_length_(A start, A end, A step)
{
    if (start < end && step > 0)
        return (end - start + step - 1) / step;
    else if (start > end && step < 0)
        return (start - end - step - 1) / (-step);
    else
        return 0;
}

template <typename A>
auto arange_iterable(size_t length) -> A // Internal data could be unpredictable
{
    if constexpr (IsStatic_v<A>)
    {
        if constexpr (IsStaticVector_v<A>)
        {
            return A(length);
        }
        else
        {
            return A();
        }
    }
    else
    {
        return A(length);
    }
}

template <typename A>
A arange(ElementType_t<A> start, ElementType_t<A> end, ElementType_t<A> step)
{
    size_t result_length = arange_length_(start, end, step);

    A result = arange_iterable<A>(result_length);
    ElementType_t<A> value = start;

    for (int i = 0; i < result_length; ++i)
    {
        result[i] = value;
        value += step;
    }

    return result;
}
#endif