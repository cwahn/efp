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

template <typename A>
constexpr Element_t<A> maximum(const A &xs)
{
    return foldl(max<Element_t<A>>, std::numeric_limits<Element_t<A>>::min(), xs);
}

template <typename A>
constexpr Element_t<A> minimum(const A &xs)
{
    return foldl(min<Element_t<A>>, std::numeric_limits<Element_t<A>>::max(), xs);
}

template <typename A>
constexpr Element_t<A> max_min(const A &xs)
{
    return maximum(xs) - minimum(xs);
}

template <typename A>
constexpr Element_t<A> sum(const A &xs)
{
    return foldl(plus<Element_t<A>>, 0, xs);
}

template <typename A>
constexpr Element_t<A> product(const A &xs)
{
    return foldl(times<Element_t<A>>, 1, xs);
}

template <typename A>
constexpr double mean(const A &xs)
{
    if constexpr (IsStaticCapacity<A>::value)
    {
        return sum(xs) / double(StaticCapacity<A>::value);
    }
    else
    {
        return sum(xs) / double(length(xs));
    }
}

// Generating
template <typename A>
constexpr size_t arange_length_(const A start, const A end, const A step)
{
    if (start < end && step > 0)
        return (end - start + step - 1) / step;
    else if (start > end && step < 0)
        return (start - end - step - 1) / (-step);
    else
        return 0;
}

template <typename A>
constexpr A arange_iterable(const size_t length) // Internal data could be unpredictable
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
A arange(const Element_t<A> start, const Element_t<A> end, const Element_t<A> step)
{
    const size_t result_length = arange_length_(start, end, step);

    A result = arange_iterable<A>(result_length);
    Element_t<A> value = start;

    for (int i = 0; i < result_length; ++i)
    {
        result[i] = value;
        value += step;
    }

    return result;
}
#endif