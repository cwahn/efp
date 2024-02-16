#ifndef META_HPP_
#define META_HPP_

#include "cpp_core.hpp"

namespace efp {

struct Unit {
    constexpr bool operator==(const Unit&) const noexcept {
        return true;
    }

    constexpr bool operator!=(const Unit&) const noexcept {
        return false;
    }
};

constexpr Unit unit;

// CtConst

template<typename A, A v>
using CtConst = std::integral_constant<A, v>;

template<typename A, A lhs, A rhs>
constexpr CtConst<bool, lhs == rhs> operator==(CtConst<A, lhs>, CtConst<A, rhs>) {
    return {};
}

template<typename A, A lhs, A rhs>
constexpr CtConst<bool, lhs != rhs> operator!=(CtConst<A, lhs>, CtConst<A, rhs>) {
    return {};
}

template<typename A, A lhs, A rhs>
constexpr CtConst<A, lhs + rhs> operator+(CtConst<A, lhs>, CtConst<A, rhs>) {
    return {};
}

template<typename A, A lhs, A rhs>
constexpr CtConst<A, lhs - rhs> operator-(CtConst<A, lhs>, CtConst<A, rhs>) {
    return {};
}

template<typename A, A lhs, A rhs>
constexpr CtConst<A, lhs * rhs> operator*(CtConst<A, lhs>, CtConst<A, rhs>) {
    return {};
}

template<typename A, A lhs, A rhs>
constexpr CtConst<A, lhs / rhs> operator/(CtConst<A, lhs>, CtConst<A, rhs>) {
    return {};
}

// Bool

template<bool b>
using Bool = CtConst<bool, b>;

// True

using True = CtConst<bool, true>;

// False

using False = CtConst<bool, false>;

// Int

template<size_t n>
using Int = CtConst<int, n>;

// Size

template<size_t n>
using Size = CtConst<std::size_t, n>;

// IsCtConst

template<typename A>
struct IsCtConst {
    static constexpr bool value = false;
};

template<typename A, A a>
struct IsCtConst<CtConst<A, a>> {
    static constexpr bool value = true;
};

template<typename A>
struct IsCtConst<A&>: IsCtConst<A> {};

template<typename A>
struct IsCtConst<A&&>: IsCtConst<A> {};

// AlwaysFalse

template<typename T>
struct AlwaysFalse: False {};

// EnableIf

template<bool cond, typename A = void>
using EnableIf = typename std::enable_if<cond, A>::type;

// Conditionl

template<bool cond, typename T, typename F>
using Conditional = typename std::conditional<cond, T, F>::type;

// All

template<typename... Args>
struct All {};

// Base case: When no types are left, return true.
template<>
struct All<>: True {};

// Recursive case: Check the first type, and recurse for the rest.
template<typename Head, typename... Tail>
struct All<Head, Tail...>: Bool<Head::value && All<Tail...>::value> {};

// Any

template<typename... Args>
struct Any {};

// Base case: When no types are left, return false.
template<>
struct Any<>: False {};

// Recursive case: Check the first type, and recurse for the rest.
template<typename Head, typename... Tail>
struct Any<Head, Tail...>: Bool<Head::value || Any<Tail...>::value> {};

// Min

template<typename Head, typename... Tail>
struct Min: Min<Head, Min<Tail...>> {};

template<typename Head, typename Tail>
struct Min<Head, Tail>: Conditional<Head::value <= Tail::value, Head, Tail> {};

template<typename Head>
struct Min<Head>: Head {};

// Max

template<typename Head, typename... Tail>
struct Max: Max<Head, Max<Tail...>> {};

template<typename Head, typename Tail>
struct Max<Head, Tail>: Conditional<Head::value >= Tail::value, Head, Tail> {};

template<typename Head>
struct Max<Head>: Head {};

// size_of_ptr_v

constexpr auto size_of_ptr_v = sizeof(void*);

// op_neg

template<typename A>
constexpr A op_neg(const A& a) {
    return -a;
}

// op_eq

// template<typename A, typename B>
// constexpr bool op_eq(const A& lhs, const B& rhs) {
//     return lhs == rhs;
// }

template<typename A>
constexpr bool op_eq(const A& lhs, const A& rhs) {
    return lhs == rhs;
}

// op_neq

// template<typename A, typename B>
// constexpr bool op_neq(const A& lhs, const B& rhs) {
//     return lhs != rhs;
// }

template<typename A>
constexpr bool op_neq(const A& lhs, const A& rhs) {
    return lhs != rhs;
}

// op_gt

// template<typename A, typename B>
// constexpr bool op_gt(const A& lhs, const B& rhs) {
//     return lhs > rhs;
// }

template<typename A>
constexpr bool op_gt(const A& lhs, const A& rhs) {
    return lhs > rhs;
}

// op_lt

// template<typename A, typename B>
// constexpr bool op_lt(const A& lhs, const B& rhs) {
//     return lhs < rhs;
// }

template<typename A>
constexpr bool op_lt(const A& lhs, const A& rhs) {
    return lhs < rhs;
}

// op_geq

// template<typename A, typename B>
// constexpr bool op_geq(const A& lhs, const B& rhs) {
//     return lhs >= rhs;
// }

template<typename A>
constexpr bool op_geq(const A& lhs, const A& rhs) {
    return lhs >= rhs;
}

// op_leq

// template<typename A, typename B>
// constexpr bool op_leq(const A& lhs, const B& rhs) {
//     return lhs <= rhs;
// }

template<typename A>
constexpr bool op_leq(const A& lhs, const A& rhs) {
    return lhs <= rhs;
}

// op_not

constexpr bool op_not(const bool b) {
    return !b;
}

// op_and

constexpr bool op_and(const bool lhs, const bool rhs) {
    return lhs && rhs;
}

// op_or

constexpr bool op_or(const bool lhs, const bool rhs) {
    return lhs || rhs;
}

// op_add

// template<typename A, typename B>
// constexpr A op_add(const A& lhs, const B& rhs) {
//     return lhs + rhs;
// }

template<typename A>
constexpr A op_add(const A& lhs, const A& rhs) {
    return lhs + rhs;
}

// op_sub

// template<typename A, typename B>
// constexpr A op_sub(const A& lhs, const B& rhs) {
//     return lhs - rhs;
// }

template<typename A>
constexpr A op_sub(const A& lhs, const A& rhs) {
    return lhs - rhs;
}

// op_mul

// template<typename A, typename B>
// constexpr A op_mul(const A& lhs, const B& rhs) {
//     return lhs * rhs;
// }

template<typename A>
constexpr A op_mul(const A& lhs, const A& rhs) {
    return lhs * rhs;
}

// op_div

// template<typename A, typename B>
// constexpr A op_div(const A& lhs, const B& rhs) {
//     return lhs / rhs;
// }

template<typename A>
constexpr A op_div(const A& lhs, const A& rhs) {
    return lhs / rhs;
}

// op_mod

// template<typename A, typename B>
// constexpr A op_mod(const A& lhs, const B& rhs) {
//     return lhs % rhs;
// }

template<typename A>
constexpr A op_mod(const A& lhs, const A& rhs) {
    return lhs % rhs;
}

// bound_v

template<typename A, typename B, typename C>
constexpr auto bound_v(const A& lower, const B& upper, const C& x)
    -> decltype((x > upper) ? (upper) : ((x < lower) ? lower : x)) {
    return (x > upper) ? (upper) : ((x < lower) ? lower : x);
}

// max

template<typename A>
constexpr A max(const A& lhs, const A& rhs) {
    return lhs > rhs ? lhs : rhs;
}

// min

template<typename A>
constexpr A min(const A& lhs, const A& rhs) {
    return lhs < rhs ? lhs : rhs;
}

// Foldl
// Maybe just recursive constexpr template function could be enough

namespace detail {
    template<template<class, class> class F, typename A, typename... Bs>
    struct FoldlImpl {};

    template<template<class, class> class F, typename A, typename B>
    struct FoldlImpl<F, A, B>: F<A, B>::Type {};

    template<template<class, class> class F, typename A, typename B0, typename B1, typename... Bs>
    struct FoldlImpl<F, A, B0, B1, Bs...>: FoldlImpl<F, typename F<A, B0>::Type, B1, Bs...> {};
}  // namespace detail

template<template<class, class> class F, typename A, typename... Bs>
using Foldl = typename detail::FoldlImpl<F, A, Bs...>::Type;

// IsSame

template<typename A, typename B>
using IsSame = std::is_same<A, B>;

// PackAt

namespace detail {
    template<uint8_t n, typename... Args>
    struct PackAtImpl {
        using Type = void*;
    };

    template<typename Head, typename... Tail>
    struct PackAtImpl<0, Head, Tail...> {
        using Type = Head;
    };

    template<uint8_t n, typename Head, typename... Tail>
    struct PackAtImpl<n, Head, Tail...>: PackAtImpl<n - 1, Tail...> {};
}  // namespace detail

template<uint8_t n, typename... Args>
using PackAt = typename detail::PackAtImpl<n, Args...>::Type;

// Find

namespace detail {
    // FindHelperValue

    template<uint8_t n>
    struct FindHelperValue {
        static constexpr uint8_t value = n;
    };

    // FindImpl

    template<size_t n, template<class> class P, typename... Args>
    struct FindImpl {};

    template<size_t n, template<class> class P, typename Head, typename... Tail>
    struct FindImpl<n, P, Head, Tail...>:
        Conditional<P<Head>::value, FindHelperValue<n>, FindImpl<n + 1, P, Tail...>> {};
}  // namespace detail

template<template<class> class P, typename... Args>
struct Find: detail::FindImpl<0, P, Args...> {};

// LvalueRefAdded

template<class T>
using LvalueRefAdded = typename std::add_lvalue_reference<T>::type;

// RvalueRefAdded

template<class T>
using RvalueRefAdded = typename std::add_rvalue_reference<T>::type;

// declval

template<typename T>
RvalueRefAdded<T> declval() noexcept;

// CallReturn

// Check the C++ standard version
#if __cplusplus >= 201703L  // C++17 or later, use std::invoke_result
template<typename F, typename... Args>
using CallReturn = typename std::invoke_result<F, Args...>::type;

#else  // Before C++17, use the custom implementation

namespace detail {

    template<typename F, typename... Args>
    struct CallReturnImpl {
        using Type = decltype(declval<F>()(declval<Args>()...));
    };
}  // namespace detail

template<typename F, typename... Args>
using CallReturn = typename detail::CallReturnImpl<F, Args...>::Type;

#endif

// HasCallOperator

template<typename A>
class HasCallOperator {
    typedef char one;
    typedef long two;

    template<typename B>
    static one test(decltype(&B::operator()));

    template<typename B>
    static two test(...);

  public:
    static const bool value = sizeof(test<A>(0)) == sizeof(one);
};

// IsInvocable

#if __cplusplus >= 201703L  // If C++17 or later, use std::is_invocable

template<typename F, typename... Args>
using IsInvocable = std::is_invocable<F, Args...>;

#else  // If earlier than C++17, use custom IsInvocable

template<typename F, typename... Args>
struct IsInvocable {
  private:
    template<typename A>
    static auto check(int) -> decltype(declval<A>()(declval<Args>()...), True());

    template<typename>
    static auto check(...) -> False;

  public:
    static constexpr bool value = decltype(check<F>(0))::value;
};

#endif

// IsFunction

template<typename T>
using IsFunction = std::is_function<T>;

// FuncToFuncPtr

namespace detail {

    // Base template
    template<typename A>
    struct FuncToFuncPtrImpl {
        using Type = A;  // Leave non-function types unchanged
    };

    // Specialization for regular and constexpr free functions
    template<typename Ret, typename... Args>
    struct FuncToFuncPtrImpl<Ret(Args...)> {
        using Type = Ret (*)(Args...);
    };

    // Specializations for member functions
    template<typename Ret, typename ClassType, typename... Args>
    struct FuncToFuncPtrImpl<Ret (ClassType::*)(Args...)> {
        using Type = Ret (ClassType::*)(Args...);
    };

    template<typename Ret, typename ClassType, typename... Args>
    struct FuncToFuncPtrImpl<Ret (ClassType::*)(Args...) const> {
        using Type = Ret (ClassType::*)(Args...) const;
    };

    template<typename Ret, typename ClassType, typename... Args>
    struct FuncToFuncPtrImpl<Ret (ClassType::*)(Args...) volatile> {
        using Type = Ret (ClassType::*)(Args...) volatile;
    };

    template<typename Ret, typename ClassType, typename... Args>
    struct FuncToFuncPtrImpl<Ret (ClassType::*)(Args...) const volatile> {
        using Type = Ret (ClassType::*)(Args...) const volatile;
    };

}  // namespace detail

template<typename A>
using FuncToFuncPtr = typename detail::FuncToFuncPtrImpl<A>::Type;

// TupleLeaf

template<std::size_t index, typename A>
class TupleLeaf {
  public:
    using Element = A;
    static constexpr size_t idx = index;

    TupleLeaf() : _value {} {}

    // Lvalue constructor
    TupleLeaf(const A& value) : _value(value) {}

    // Rvalue constructor, using efp::move to ensure the object is moved
    TupleLeaf(A&& value) : _value(move(value)) {}

    ~TupleLeaf() {}

    const A& get() const {
        return _value;
    }

    A& get() {
        return _value;
    }

  private:
    A _value;
};

// IndexSequence

template<int... ns>
struct IndexSequence {};

// MakeIndexSequenceImpl

template<size_t n, int... ns>
struct MakeIndexSequenceImpl: MakeIndexSequenceImpl<n - 1, n - 1, ns...> {};

template<int... ns>
struct MakeIndexSequenceImpl<0, ns...> {
    using Type = IndexSequence<ns...>;
};

// MakeIndexSequence

template<size_t n>
using MakeIndexSequence = typename MakeIndexSequenceImpl<n>::Type;

// IndexSequenceFor

template<typename... Ts>
using IndexSequenceFor = MakeIndexSequence<sizeof...(Ts)>;

// Tuple
namespace detail {
    template<typename IndexSequence, typename... As>
    class TupleImpl {};

    template<int... idxs, typename... As>
    class TupleImpl<IndexSequence<idxs...>, As...>: public TupleLeaf<idxs, As>... {
      public:
        // Function name or function type will be automatically converted to function pointer type
        TupleImpl(const As&... as) : TupleLeaf<idxs, As> {as}... {}

      protected:
        template<typename F>
        auto match_impl(const F& f) const
            -> EnableIf<IsInvocable<F, As...>::value, CallReturn<F, As...>> {
            return f(TupleLeaf<idxs, PackAt<idxs, As...>>::get()...);
        }
    };
}  // namespace detail

// Tuple

template<typename... As>
class Tuple: public detail::TupleImpl<IndexSequenceFor<As...>, As...> {
  public:
    Tuple(const As&... as) : detail::TupleImpl<IndexSequenceFor<As...>, As...> {as...} {}

    template<size_t idx>
    auto get() const -> const PackAt<idx, As...>& {
        return TupleLeaf<idx, PackAt<idx, As...>>::get();
    }

    template<size_t idx>
    auto get() -> PackAt<idx, As...>& {
        return TupleLeaf<idx, PackAt<idx, As...>>::get();
    }

    template<typename F>
    auto match(const F& f) const -> EnableIf<IsInvocable<F, As...>::value, CallReturn<F, As...>> {
        return detail::TupleImpl<IndexSequenceFor<As...>, As...>::match_impl(f);
    }

  private:
};

// Pair

template<typename A, typename B>
using Pair = Tuple<A, B>;

template<size_t index, typename... As>
auto get(const Tuple<As...>& tpl) -> const PackAt<index, As...>& {
    return tpl.template get<index>();
}

template<size_t index, typename... As>
auto get(Tuple<As...>& tpl) -> PackAt<index, As...>& {
    return tpl.template get<index>();
}

// Projection operator for the Tuple the same with get

template<size_t index, typename... As>
auto p(const Tuple<As...>& tpl) -> const PackAt<index, As...>& {
    return tpl.template get<index>();
}

template<size_t index, typename... As>
auto p(Tuple<As...>& tpl) -> PackAt<index, As...>& {
    return tpl.template get<index>();
}

// fst

template<typename... As>
auto fst(const Tuple<As...>& tpl) -> const PackAt<0, As...>& {
    return tpl.template get<0>();
}

template<typename... As>
auto fst(Tuple<As...>& tpl) -> PackAt<0, As...>& {
    return tpl.template get<0>();
}

// snd

template<typename... As>
auto snd(const Tuple<As...>& tpl) -> const PackAt<1, As...>& {
    return tpl.template get<1>();
}

template<typename... As>
auto snd(Tuple<As...>& tpl) -> PackAt<1, As...>& {
    return tpl.template get<1>();
}

// Equality operator for Tuple

namespace detail {
    template<size_t index>
    struct TupleLeafComparatorImpl {
        template<typename... As>
        static bool compare(const Tuple<As...>& lhs, const Tuple<As...>& rhs) {
            if (lhs.template get<index>() != rhs.template get<index>())
                return false;
            return TupleLeafComparatorImpl<index - 1>::compare(lhs, rhs);
        }
    };

    template<>
    struct TupleLeafComparatorImpl<0> {
        template<typename... As>
        static bool compare(const Tuple<As...>& lhs, const Tuple<As...>& rhs) {
            return lhs.template get<0>() == rhs.template get<0>();
        }
    };

    template<>
    struct TupleLeafComparatorImpl<static_cast<size_t>(-1)> {
        template<typename... As>
        static bool compare(const Tuple<As...>&, const Tuple<As...>&) {
            return true;
        }
    };
}  // namespace detail

template<typename... As>
bool operator==(const Tuple<As...>& lhs, const Tuple<As...>& rhs) {
    return detail::TupleLeafComparatorImpl<sizeof...(As) - 1>::compare(lhs, rhs);
}

template<typename... As>
bool operator!=(const Tuple<As...>& lhs, const Tuple<As...>& rhs) {
    return !(lhs == rhs);
}

// tuple

template<typename... As>
auto tuple(const As&... as) -> Tuple<FuncToFuncPtr<As>...> {
    return Tuple<FuncToFuncPtr<As>...> {as...};
}

// TupleAt

namespace detail {
    template<size_t n, typename Tpl>
    struct TupleAtImpl {};

    template<size_t n, typename... As>
    struct TupleAtImpl<n, Tuple<As...>> {
        using Type = PackAt<n, As...>;
    };
}  // namespace detail

template<size_t n, typename Tpl>
using TupleAt = typename detail::TupleAtImpl<n, Tpl>::Type;

// Arguements
// l-value and r-value reference will preserved at the result, but const will be removed.

namespace detail {
    template<typename, bool>
    struct ArgumentsImpl {};

    template<typename F>
    struct ArgumentsImpl<F, true>: ArgumentsImpl<decltype(&F::operator()), false> {
        // using Type = typename
    };

    template<typename R, typename... Args>
    struct ArgumentsImpl<R (*)(Args...), false> {
        using Type = Tuple<Args...>;
    };

    template<typename R, typename A, typename... Args>
    struct ArgumentsImpl<R (A::*)(Args...), false> {
        using Type = Tuple<Args...>;
    };

    template<typename R, typename A, typename... Args>
    struct ArgumentsImpl<R (A::*)(Args...) const, false> {
        using Type = Tuple<Args...>;
    };
}  // namespace detail

template<typename F>
using Arguments = typename detail::ArgumentsImpl<F, HasCallOperator<F>::value>::Type;

// ReturnFromArgument

namespace detail {
    template<typename, typename>
    struct ReturnFromArgumentImpl {};

    template<typename F, typename... Args>
    struct ReturnFromArgumentImpl<F, Tuple<Args...>> {
        using Type = CallReturn<F, Args...>;
    };
}  // namespace detail

template<typename F>
using ReturnFromArgument = typename detail::ReturnFromArgumentImpl<F, Arguments<F>>::Type;

// FunctionReturn

namespace detail {
    template<typename F>
    struct FunctionReturnImpl {};

    // Specialization for function posize_ters
    template<typename R, typename... Args>
    struct FunctionReturnImpl<R (*)(Args...)> {
        using Type = R;
    };

    // Specialization for member function posize_ters
    template<typename R, typename C, typename... Args>
    struct FunctionReturnImpl<R (C::*)(Args...)> {
        using Type = R;
    };

    template<typename R, typename C, typename... Args>
    struct FunctionReturnImpl<R (C::*)(Args..., ...)> {
        using Type = R;
    };

    // Specialization for member function posize_ters with const qualifier
    template<typename R, typename C, typename... Args>
    struct FunctionReturnImpl<R (C::*)(Args...) const> {
        using Type = R;
    };

    template<typename R, typename C, typename... Args>
    struct FunctionReturnImpl<R (C::*)(Args..., ...) const> {
        using Type = R;
    };

    // Specialization for member function posize_ters with volatile qualifier
    template<typename R, typename C, typename... Args>
    struct FunctionReturnImpl<R (C::*)(Args...) volatile> {
        using Type = R;
    };

    template<typename R, typename C, typename... Args>
    struct FunctionReturnImpl<R (C::*)(Args..., ...) volatile> {
        using Type = R;
    };

    // Specialization for member function posize_ters with const volatile qualifier
    template<typename R, typename C, typename... Args>
    struct FunctionReturnImpl<R (C::*)(Args...) const volatile> {
        using Type = R;
    };

    template<typename R, typename C, typename... Args>
    struct FunctionReturnImpl<R (C::*)(Args..., ...) const volatile> {
        using Type = R;
    };
}  // namespace detail

template<typename F>
using FunctionReturn = typename detail::FunctionReturnImpl<F>::Type;

// Return

namespace detail {
    template<typename F, typename Enable = void>
    struct ReturnImpl: FunctionReturnImpl<F> {};

    template<typename F>
    struct ReturnImpl<F, EnableIf<HasCallOperator<F>::value, void>>:
        FunctionReturnImpl<decltype(&F::operator())> {};
}  // namespace detail

template<typename F>
using Return = typename detail::ReturnImpl<F>::Type;

// apply

namespace detail {
    template<typename F, typename... As, int... indices>
    Return<F> apply_impl(const F& f, const Tuple<As...>& tpl, IndexSequence<indices...>) {
        return f(get<indices>(tpl)...);
    }
}  // namespace detail

template<
    typename F,
    typename... As,
    typename = EnableIf<IsSame<Arguments<F>, Tuple<As...>>::value, void>>
Return<F> apply(const F& f, const Tuple<As...>& tpl) {
    return detail::apply_impl(f, tpl, IndexSequenceFor<As...> {});
}

// PointerRemoved

template<typename A>
using PointerRemoved = typename std::remove_pointer<A>::type;

// ReferenceRemoved

template<typename A>
using ReferenceRemoved = typename std::remove_reference<A>::type;

// ConstRemoved

template<typename A>
using ConstRemoved = typename std::remove_const<A>::type;

// VoletileRemoved

template<typename A>
using VoletileRemoved = typename std::remove_volatile<A>::type;

// CVRemoved

template<typename A>
using CVRemoved = VoletileRemoved<ConstRemoved<A>>;

// CVRefRemoved

template<typename A>
using CVRefRemoved = CVRemoved<ReferenceRemoved<A>>;

// todo Decay

// IsConst

template<typename A>
using IsConst = std::is_const<A>;

// Void

namespace detail {
    template<typename... Ts>
    struct VoidImpl {
        using Type = void;
    };
}  // namespace detail

template<typename... Ts>
using Void = typename detail::VoidImpl<Ts...>::Type;

// IsLvalueReference

template<typename A>
struct IsLvalueReference: False {};

template<typename A>
struct IsLvalueReference<A&>: True {};

// Forward

template<typename A>
constexpr A&& forward(ReferenceRemoved<A>& a) noexcept {
    return static_cast<A&&>(a);
}

template<typename A>
constexpr A&& forward(ReferenceRemoved<A>&& a) noexcept {
    static_assert(!IsLvalueReference<A>::value, "Cannot forward an rvalue as an lvalue.");
    return static_cast<A&&>(a);
}

// move

template<typename A>
constexpr ReferenceRemoved<A>&& move(A&& a) {
    return static_cast<ReferenceRemoved<A>&&>(a);
}

// IsDefaultConstructible

template<typename A, typename = void>
struct IsDefaultConstructible: False {};

template<typename A>
struct IsDefaultConstructible<A, decltype(A())>: True {};

// _foldl

template<typename F, typename A>
constexpr A _foldl(F f, A a) {
    return a;
}

#if __cplusplus >= 201703L
// C++17 or later, use a loop for foldl
template<typename F, typename A, typename... Bs>
constexpr A _foldl(F f, A a, Bs... bs) {
    // Convert parameter pack to array for iteration
    A arr[] = {static_cast<A>(bs)...};

    for (auto& element : arr) {
        a = f(a, element);
    }

    return a;
}
#else
// Before C++17, recursive implementation
template<typename F, typename A, typename B, typename... Bs>
constexpr A _foldl(F f, A a, B b, Bs... bs) {
    return _foldl(f, f(forward<A>(a), forward<B>(b)), forward<Bs>(bs)...);
}
#endif

// _all

template<typename... Args>
constexpr bool _all(Args... args) {
    return _foldl(op_and, true, args...);
}

// _any

template<typename... Args>
constexpr bool _any(Args... args) {
    return _foldl(op_or, false, args...);
}

// _maximum
// cf) since the function is defined as foldr, the result follows the type of first argument.

template<typename A, typename... As>
constexpr A _maximum(A a, As... as) {
    return _foldl(max<A>, a, as...);
}

// _minimum
// cf) since the function is defined as foldr, the result follows the type of first argument.

template<typename A, typename... As>
constexpr A _minimum(A a, As... as) {
    return _foldl(min<A>, a, as...);
}

// _sum

template<typename A, typename... As>
constexpr A _sum(A a, As... as) {
    return _foldl(op_add<A>, a, as...);
}

// _product

template<typename A, typename... As>
constexpr A _product(A a, As... as) {
    return _foldl(op_mul<A>, a, as...);
}

// InitializerList

template<typename A>
using InitializerList = std::initializer_list<A>;

// Common

template<typename... As>
using Common = typename std::common_type<As...>::type;

// DebugType

template<typename A>
struct DebugType;  // Intentionally undefined

// IsConstructible

template<typename A, typename... Args>
using IsConstructible = Bool<std::is_constructible<A, Args...>::value>;

// AlignedStorage

// template <size_t Len, size_t Align>
// struct AlignedStorage {
//     alignas(Align) char data[Len];
// };

// Storage

template<typename A, size_t n>
struct RawStorage {
    static_assert(sizeof(A) % alignof(A) == 0, "Size of A must be a multiple of its alignment");

    alignas(alignof(A)) char _data[n * sizeof(A)];

    inline const A& operator[](size_t i) const {
        return reinterpret_cast<const A*>(_data)[i];
    }

    inline A& operator[](size_t i) {
        return reinterpret_cast<A*>(_data)[i];
    }

    // use name of the instance as pointer like c-style array without data()
    inline operator const A*() const {
        return reinterpret_cast<const A*>(_data);
    }

    inline operator A*() {
        return reinterpret_cast<A*>(_data);
    }

    inline const A* data() const {
        return reinterpret_cast<const A*>(_data);
    }

    inline A* data() {
        return reinterpret_cast<A*>(_data);
    }
};

}  // namespace efp

#endif