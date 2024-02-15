#ifndef META_HPP_
#define META_HPP_

#include "cpp_core.hpp"

// Make it value as soon as possible and use constexpr function and avoid type_trait.

namespace efp {
// True False type

struct Unit {};

constexpr Unit unit;

inline bool operator==(const Unit&, const Unit&) {
    return true;
}

inline bool operator!=(const Unit&, const Unit&) {
    return false;
}

// CtConst

// ! deprecated direct implementation
// template<typename A, A a>
// struct CtConst {
//     static constexpr A value = a;
//     using value_type = A;
//     using Type = CtConst;

//     constexpr operator value_type() const noexcept {
//         return value;
//     }  // Conversion operator

//     constexpr value_type operator()() const noexcept {
//         return value;
//     }  // Function call operator
// };

template<typename A, A v>
struct CtConst: std::integral_constant<A, v> {
    using ValueType = typename std::integral_constant<A, v>::value_type;
    using Type = CtConst;

    static constexpr A value = std::integral_constant<A, v>::value;

    constexpr operator ValueType() const noexcept {
        return value;
    }

    constexpr ValueType operator()() const noexcept {
        return value;
    }
};

// template<typename A, A a>
// using CtConst = std::integral_constant<A, a>;

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

// EnableIfImpl

namespace detail {
    template<bool cond, typename A = void>
    struct EnableIfImpl {};

    template<typename A>
    struct EnableIfImpl<true, A> {
        typedef A Type;
    };
}  // namespace detail

// EnableIf

template<bool cond, typename A = void>
using EnableIf = typename detail::EnableIfImpl<cond, A>::Type;

// Conditionl

namespace detail {
    template<bool cond, typename T, typename F>
    struct ConditionalImpl {};

    template<typename T, typename F>
    struct ConditionalImpl<true, T, F> {
        using Type = T;
    };

    template<typename T, typename F>
    struct ConditionalImpl<false, T, F> {
        using Type = F;
    };
}  // namespace detail

template<bool cond, typename T, typename F>
using Conditional = typename detail::ConditionalImpl<cond, T, F>::Type;

// All

template<typename... Args>
struct All {};

// Base case: When no types are left, return true.
template<>
struct All<>: True {};

// Recursive case: Check the first type, and recurse for the rest.
template<typename Head, typename... Tail>
struct All<Head, Tail...>: CtConst<bool, Head::value && All<Tail...>::value> {};

// Any

template<typename... Args>
struct Any {};

// Base case: When no types are left, return false.
template<>
struct Any<>: False {};

// Recursive case: Check the first type, and recurse for the rest.
template<typename Head, typename... Tail>
struct Any<Head, Tail...>: CtConst<bool, Head::value || Any<Tail...>::value> {};

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

// ConstParam_t
// Pass by value if the size of type is leq to machine posize_ter size.

// template <typename A>
// using ConstParam_t = Conditional<
//     sizeof(A) <= size_of_ptr_v,
//     A,
//     // const A &>;
//     const typename std::remove_const<typename std::remove_reference<A>::Type>::Type &>;

// eq_v

template<typename A, typename B>
constexpr bool eq_v(const A& lhs, const B& rhs) {
    return lhs == rhs;
}

//  neq_v

template<typename A, typename B>
constexpr bool neq_v(const A& lhs, const B& rhs) {
    return lhs != rhs;
}

//  gt_v

template<typename A, typename B>
constexpr bool gt_v(const A& lhs, const B& rhs) {
    return lhs > rhs;
}

//  lt_v

template<typename A, typename B>
constexpr bool lt_v(const A& lhs, const B& rhs) {
    return lhs < rhs;
}

//  geq_v

template<typename A, typename B>
constexpr bool geq_v(const A& lhs, const B& rhs) {
    return lhs >= rhs;
}

//  leq_v

template<typename A, typename B>
constexpr bool leq_v(const A& lhs, const B& rhs) {
    return lhs <= rhs;
}

// and_v

constexpr bool and_v(const bool lhs, const bool rhs) {
    return lhs && rhs;
}

// or_v

constexpr bool or_v(const bool lhs, const bool rhs) {
    return lhs || rhs;
}

// max_v

template<typename A>
constexpr A max_v(const A& lhs, const A& rhs) {
    return lhs > rhs ? lhs : rhs;
}

// min_v

template<typename A>
constexpr A min_v(const A& lhs, const A& rhs) {
    return lhs < rhs ? lhs : rhs;
}

// plus_v

template<typename A, typename B>
constexpr auto plus_v(const A& lhs, const B& rhs) -> decltype(lhs + rhs) {
    return lhs + rhs;
}

// times_v

template<typename A>
constexpr A times_v(const A& lhs, const A& rhs) {
    return lhs * rhs;
}

// bound_v

template<typename A, typename B, typename C>
constexpr auto bound_v(const A& lower, const B& upper, const C& x)
    -> decltype((x > upper) ? (upper) : ((x < lower) ? lower : x)) {
    return (x > upper) ? (upper) : ((x < lower) ? lower : x);
}

// template <typename A, typename B, typename C>
// constexpr typename C::value_type bound_v(const A &lower, const B &upper, const C &x)
// {
//     return (x > upper) ? (upper) : ((x < lower) ? lower : x);
// }

// Foldl

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

// * Maybe just recursive constexpr template function could be enough

// foldl_v

template<typename F, typename A>
constexpr A foldl_v(F f, A a) {
    return a;
}

template<typename F, typename A, typename B>
constexpr A foldl_v(F f, A a, B b) {
    return f(a, b);
}

template<typename F, typename A, typename B, typename... Bs>
constexpr A foldl_v(F f, A a, B b, Bs... bs) {
    return foldl_v(f, f(a, b), bs...);
}

// all_v

template<typename... Args>
constexpr bool all_v(Args... args) {
    return foldl_v(and_v, true, args...);
}

// any_v

template<typename... Args>
constexpr bool any_v(Args... args) {
    return foldl_v(or_v, false, args...);
}

// maximum_v
// cf) since the function is defined as foldr, the result follows the type of first argument.

template<typename A, typename... As>
constexpr A maximum_v(A a, As... as) {
    return foldl_v(max_v<A>, a, as...);
}

// minimum_v
// cf) since the function is defined as foldr, the result follows the type of first argument.

template<typename A, typename... As>
constexpr A minimum_v(A a, As... as) {
    return foldl_v(min_v<A>, a, as...);
}

// sum_v

template<typename A, typename... As>
constexpr A sum_v(A a, As... as) {
    return foldl_v(plus_v<A, A>, a, as...);
}

// product_v

template<typename A, typename... As>
constexpr A product_v(A a, As... as) {
    return foldl_v(times_v<A>, a, as...);
}

// IsSame

template<typename A, typename B>
struct IsSame {
    static constexpr bool value = false;
};

template<typename A>
struct IsSame<A, A> {
    static constexpr bool value = true;
};

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

// FindHelperValue
template<uint8_t n>
struct FindHelperValue {
    static constexpr uint8_t value = n;
};

// FindHelper

template<size_t n, template<class> class P, typename... Args>
struct FindHelper {};

template<size_t n, template<class> class P, typename Head, typename... Tail>
struct FindHelper<n, P, Head, Tail...>:
    Conditional<P<Head>::value, FindHelperValue<n>, FindHelper<n + 1, P, Tail...>> {};

// Find

template<template<class> class P, typename... Args>
struct Find: FindHelper<0, P, Args...> {};

namespace detail {
    template<class T>
    struct TypeIdentity {
        using Type = T;
    };  // or use std::TypeIdentity (since C++20)

    template<class T>  // Note that `cv void&` is a substitution failure
    auto TryAddLvalueReference(int) -> TypeIdentity<T&>;
    template<class T>  // Handle T = cv void case
    auto TryAddLvalueReference(...) -> TypeIdentity<T>;

    template<class T>
    auto TryAddRvalueReference(int) -> TypeIdentity<T&&>;
    template<class T>
    auto TryAddRvalueReference(...) -> TypeIdentity<T>;
}  // namespace detail

// AddLvalueReference

template<class T>
struct AddLvalueReference: decltype(detail::TryAddLvalueReference<T>(0)) {};

// AddRvalueReference

template<class T>
struct AddRvalueReference: decltype(detail::TryAddRvalueReference<T>(0)) {};

template<typename T>
typename AddRvalueReference<T>::Type declval() noexcept {
    static_assert(AlwaysFalse<T>::value, "declval not allowed in an evaluated context");
}

// CallReturn

namespace detail {
    template<typename, typename...>
    struct CallReturnImpl;

    template<typename F, typename... Args>
    struct CallReturnImpl {
        using Type = decltype(declval<F>()(declval<Args>()...));
    };
}  // namespace detail

template<typename F, typename... Args>
using CallReturn = typename detail::CallReturnImpl<F, Args...>::Type;

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

// IsFunction
// Base template
template<typename T>
struct IsFunction: False {};

// Specializations for all kinds of function types
template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...)>: True {};

// Add specializations for const, volatile, and const volatile member functions if needed
template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) const>: True {};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) volatile>: True {};

template<typename Ret, typename... Args>
struct IsFunction<Ret(Args...) const volatile>: True {};

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

// IndexSequence

// TupleLeaf

template<size_t index, typename A>
class TupleLeaf {
  public:
    using Element = A;
    static constexpr size_t idx = index;

    TupleLeaf() {}

    TupleLeaf(const A& value) : value_ {value} {}

    TupleLeaf(A&& value) : value_ {value} {}

    ~TupleLeaf() {}

    const A& get() const {
        return value_;
    }

    A& get() {
        return value_;
    }

  private:
    A value_;
};

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

namespace detail {
    template<typename A>
    struct PointerRemovedImpl {
        using Type = A;
    };

    template<typename A>
    struct PointerRemovedImpl<A*> {
        using Type = A;
    };
}  // namespace detail

template<typename A>
using PointerRemoved = typename detail::PointerRemovedImpl<A>::Type;

// ReferenceRemoved

namespace detail {
    template<typename A>
    struct ReferenceRemovedImpl {
        using Type = A;
    };

    template<typename A>
    struct ReferenceRemovedImpl<A&> {
        using Type = A;
    };

    template<typename A>
    struct ReferenceRemovedImpl<A&&> {
        using Type = A;
    };
}  // namespace detail

template<typename A>
using ReferenceRemoved = typename detail::ReferenceRemovedImpl<A>::Type;

// ConstRemoved

namespace detail {
    template<typename A>
    struct ConstRemovedImpl {
        using Type = A;
    };

    template<typename A>
    struct ConstRemovedImpl<const A> {
        using Type = A;
    };
}  // namespace detail

template<typename A>
using ConstRemoved = typename detail::ConstRemovedImpl<A>::Type;

// VoletileRemovedImpl

namespace detail {
    template<typename A>
    struct VoletileRemovedImpl {
        using Type = A;
    };

    template<typename A>
    struct VoletileRemovedImpl<const A> {
        using Type = A;
    };
}  // namespace detail

// VoletileRemoved

template<typename A>
using VoletileRemoved = typename detail::VoletileRemovedImpl<A>::Type;

// CVRemoved

template<typename A>
using CVRemoved = VoletileRemoved<ConstRemoved<A>>;

// Cleaned

template<typename A>
using Cleaned = CVRemoved<ReferenceRemoved<A>>;

// todo Decay

// Common

namespace detail {
    template<typename... As>
    struct CommonImpl {
        using Type = void;
    };

    template<typename A, typename... As>
    struct CommonImpl<A, As...> {
        using Type = EnableIf<all_v(IsSame<A, As>::value...), A>;
    };
}  // namespace detail

template<typename... As>
using Common = typename detail::CommonImpl<As...>::Type;

// IsConst

template<typename A>
struct IsConst: False {};

template<typename A>
struct IsConst<const A>: True {};

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
A&& forward(ReferenceRemoved<A>& a) noexcept {
    return static_cast<A&&>(a);
}

template<typename A>
A&& forward(ReferenceRemoved<A>&& a) noexcept {
    static_assert(!IsLvalueReference<A>::value, "Cannot forward an rvalue as an lvalue.");
    return static_cast<A&&>(a);
}

// IsDefaultConstructible

template<typename A, typename = void>
struct IsDefaultConstructible: False {};

template<typename A>
struct IsDefaultConstructible<A, decltype(A())>: True {};

// move

template<typename A>
ReferenceRemoved<A>&& move(A&& a) {
    return static_cast<ReferenceRemoved<A>&&>(a);
}

// InitializerList

// template <class A>
// class InitializerList
// {
// public:
//     using value_type = A;
//     using reference = const A &;
//     using const_reference = const A &;
//     using size_type = size_t;

//     using iterator = const A *;
//     using const_iterator = const A *;

//     constexpr InitializerList() noexcept : array(nullptr), len(0) {}

//     // Number of elements
//     constexpr size_type size() const noexcept { return len; }

//     // First element
//     constexpr const_iterator begin() const noexcept { return array; }

//     // One past the last element
//     constexpr const_iterator end() const noexcept { return array + len; }

// private:
//     iterator array;
//     size_type len;

//     // The constructor is private and can only be called by the compiler
//     // which will create an InitializerList using an array temporary.
//     constexpr InitializerList(const_iterator a, size_type l) : array(a), len(l) {}
// };

template<typename A>
struct DebugType;  // Intentionally undefined

// namespace detail {
//     template <typename, typename A, typename... Args>
//     struct IsConstructibleImpl : False {};

//     // Specialization that tests if the constructor exists.
//     template <typename A, typename... Args>
//     struct IsConstructibleImpl<Void<decltype(A(declval<Args>()...))>, A, Args...> : True {};
// } // namespace detail

// // Public interface to check if A can be constructed with Args...
// template <typename A, typename... Args>
// using IsConstructible = detail::IsConstructibleImpl<A, Args...>;

// template <typename, typename A, typename... Args>
// struct IsConstructible : False {};

// // Specialization that tests if the constructor exists.
// template <typename A, typename... Args>
// struct IsConstructible<Void<decltype(A(declval<Args>()...))>, A, Args...> : True {};

// Helper to test if a type T can be constructed with arguments Args...
// template <typename T, typename... Args>
// struct IsConstructibleImpl {
// private:
//     // Test function to check constructibility
//     template <typename U, typename... UArgs,
//               typename = decltype(U(std::declval<UArgs>()...))>
//     static True test(int);

//     // Fallback function when the above test fails
//     template <typename, typename...>
//     static False test(...);

// public:
//     static constexpr bool value = decltype(test<T, Args...>(0))::value;
// };

// // Public interface for IsConstructible
// template <typename T, typename... Args>
// struct IsConstructible : CtConst<bool, IsConstructibleImpl<T, Args...>::value> {};

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