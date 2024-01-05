#ifndef NUMERIC_HPP_
#define NUMERIC_HPP_

// todo remove STL
#include <complex>

#ifdef _MSC_VER
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#endif

#include "prelude.hpp"

namespace efp {
template <typename A>
using Complex = typename std::complex<A>;

template <typename A>
struct IsComplex : False {
};

template <typename A>
struct IsComplex<Complex<A>> : True {
};

template <typename A>
struct ComplexBaseType {
};

template <typename A>
struct ComplexBaseType<Complex<A>> {
    using Type = A;
};

template <typename A>
using ComplexBase_t = typename ComplexBaseType<A>::Type;

template <typename A>
struct AssertComplex {
    using Type = Complex<A>;
};

template <typename A>
struct AssertComplex<Complex<A>> {
    using Type = Complex<A>;
};

template <typename A>
using AssertComplex_t = typename AssertComplex<A>::Type;

template <typename A, typename B>
constexpr A max(const A& lhs, const B& rhs) {
    return max_v(lhs, rhs);
}

template <bool to_complex, typename A>
auto complex_cast(const A& a)
    -> EnableIf<!to_complex && IsComplex<A>::value, ComplexBase_t<A>> {
    return a.real();
}

template <bool to_complex, typename A>
auto complex_cast(const A& a)
    -> EnableIf<to_complex && !IsComplex<A>::value, Complex<A>> {
    return Complex<A>{a, 0};
}

template <bool to_complex, typename A>
auto complex_cast(const A& a)
    -> EnableIf<to_complex == IsComplex<A>::value, A> {
    return a;
}

template <typename A, typename B>
constexpr A min(const A& lhs, const B& rhs) {
    return min_v(lhs, rhs);
}

template <typename A, typename B>
constexpr A plus(const A& lhs, const B& rhs) {
    return lhs + rhs;
}

template <typename A, typename B>
constexpr A minus(const A& lhs, const B& rhs) {
    return lhs - rhs;
}

template <typename A, typename B>
constexpr A times(const A& lhs, const B& rhs) {
    return lhs * rhs;
}

template <typename A, typename B>
constexpr A devide(const A& lhs, const B& rhs) {
    return lhs / rhs;
}

template <typename A>
constexpr A square(const A& a) {
    return a * a;
}

template <typename A>
constexpr A sqrt(const A& a) {
    return ::sqrt(a);
}

template <typename A>
constexpr A sin(const A& a) {
    return ::sin(a);
}

template <typename A>
constexpr A cos(const A& a) {
    return ::cos(a);
}

template <typename A>
constexpr A tan(const A& a) {
    return ::tan(a);
}

template <typename A>
constexpr A abs(const A& a) {
    return a < 0 ? -a : a;
}

template <typename A, typename B>
constexpr A is_approx(const A& lhs, const B& rhs) {
    using MorePrecise_t = Conditional<(sizeof(A) > sizeof(B)), A, B>;
    return abs(static_cast<MorePrecise_t>(lhs) - static_cast<MorePrecise_t>(rhs)) <= NumericLimits<MorePrecise_t>::epsilon();
}

template <typename A>
constexpr Maybe<A> real_from_complex(const Complex<A>& a) {
    return is_approx(abs(a.imag()), 0)
               ? Maybe<A>{a.real()}
               : Maybe<A>{nothing};
}

// Reducings

template <typename As>
constexpr Element<As> max_elem(const As& as) {
    return foldl(max, NumericLimits<Element<As>>::min(), as);
}

template <typename As>
constexpr Element<As> min_elem(const As& as) {
    return foldl(min, NumericLimits<Element<As>>::max(), as);
}

template <typename As>
constexpr Element<As> max_min(const As& as) {
    return max_elem(as) - min_elem(as);
}

template <typename As>
constexpr Element<As> sum(const As& as) {
    return foldl(plus, 0, as);
}

template <typename As>
constexpr Element<As> product(const As& as) {
    return foldl(times, 1, as);
}
} // namespace efp

#endif