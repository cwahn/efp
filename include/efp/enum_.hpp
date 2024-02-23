#ifndef ENUM_HPP_
#define ENUM_HPP_

#include "efp/meta.hpp"

namespace efp {
// Is WildCard

template<typename F>
using IsWildCard = IsSame<Arguments<ReferenceRemoved<F>>, Tuple<>>;

// WildCardWrapper

template<typename F>
class WildCardWrapper: public F {
public:
    // Explicitly define a constructor to accept a lambda or any callable
    template<typename Functor>
    WildCardWrapper(Functor&& functor) : F(std::forward<Functor>(functor)) {}

    // Overload operator() to forward arguments to the callable's operator()
    template<typename... Args>
    auto operator()(Args&&... args) const -> decltype(std::declval<F>()()) {
        return F::operator()();
    }
};

template<typename F>
struct detail::ReturnImpl<WildCardWrapper<F>> {
    using Type = decltype(declval<F>()());
};

// MatchBranchFImpl
namespace detail {
    template<typename F, typename = void>
    struct MatchBranchFImpl {
        using Type = CVRefRemoved<F>;
    };

    template<typename F>
    struct MatchBranchFImpl<F, EnableIf<IsWildCard<F>::value>> {
        using Type = WildCardWrapper<CVRefRemoved<F>>;
    };
}  // namespace detail

// MatchBranchF

template<typename F>
using MatchBranchF = typename detail::MatchBranchFImpl<F>::Type;

// Overloaded

// template<typename... Fs>
// struct Overloaded {};

// template<typename F>
// struct Overloaded<F>: F {
//     using F::operator();

//     template<typename G>
//     Overloaded(G&& g) : F {MatchBranchF<G> {forward<G>(g)}} {}
// };

// template<class F, class... Fs>
// struct Overloaded<F, Fs...>: F, Overloaded<Fs...> {
//     using F::operator();
//     using Overloaded<Fs...>::operator();

//     template<class G, class... Gs>
//     Overloaded(G&& g, Gs&&... gs)
//         : F {MatchBranchF<G> {forward<G>(g)}}, Overloaded<Fs...>(forward<Gs>(gs)...) {}
// };

template<typename... Fs>
struct Overloaded {};

template<typename F>
struct Overloaded<F>: F {
    using F::operator();

    Overloaded(F&& g) : F {MatchBranchF<F> {forward<F>(g)}} {}
};

template<class F, class... Fs>
struct Overloaded<F, Fs...>: F, Overloaded<Fs...> {
    using F::operator();
    using Overloaded<Fs...>::operator();

    Overloaded(F&& g, Fs&&... fs)
        : F {MatchBranchF<F> {forward<F>(g)}}, Overloaded<Fs...>(forward<Fs>(fs)...) {}
};

namespace detail {
    constexpr uint8_t power_2_ceiling(uint8_t n, uint8_t power = 2) {
        return (power >= n) ? power : power_2_ceiling(n, power * 2);
    }

#define COPY_CASE(i) \
    case i: { \
        using Variant = PackAt < i<sizeof...(As) ? i : sizeof...(As) - 1, As...>; \
        new (dest._storage) Variant(src.template get<Variant>()); \
        break; \
    }

#define MOVE_CASE(i) \
    case i: { \
        using Variant = PackAt < i<sizeof...(As) ? i : sizeof...(As) - 1, As...>; \
        new (dest._storage) Variant(efp::move(src.template move<Variant>())); \
        break; \
    }

#define DESTROCTOR_CASE(i) \
    case i: { \
        using Variant = PackAt < i<sizeof...(As) ? i : sizeof...(As) - 1, As...>; \
        reinterpret_cast<Variant*>(self._storage)->~Variant(); \
        break; \
    }

#define MATCH_CASE(i) \
    case i: { \
        return overloaded( \
            *(reinterpret_cast \
              < const PackAt<i<sizeof...(As) ? i : sizeof...(As) - 1, As...>*>(outer->_storage)) \
        ); \
        break; \
    }

#define EQUALITY_CASE(i) \
    case i: { \
        using Variant = PackAt < i<sizeof...(As) ? i : sizeof...(As) - 1, As...>; \
        return *(reinterpret_cast<const Variant*>(self._storage)) \
            == *(reinterpret_cast<const Variant*>(other._storage)); \
        break; \
    }

#define STAMP2(n, x) x(n) x(n + 1)

#define STAMP4(n, x) \
    STAMP2(n, x) \
    STAMP2(n + 2, x)

#define STAMP8(n, x) \
    STAMP4(n, x) \
    STAMP4(n + 4, x)

#define STAMP16(n, x) \
    STAMP8(n, x) \
    STAMP8(n + 8, x)

#define STAMP32(n, x) \
    STAMP16(n, x) \
    STAMP16(n + 16, x)

#define STAMP64(n, x) \
    STAMP32(n, x) \
    STAMP32(n + 32, x)

#define STAMP128(n, x) \
    STAMP64(n, x) \
    STAMP64(n + 64, x)

#define STAMP256(n, x) \
    STAMP128(n, x) \
    STAMP128(n + 128, x)

    template<size_t n, typename... As>
    struct MatchImpl {};

    template<size_t n, typename... As>
    struct CopyImpl {};

    template<size_t n, typename... As>
    struct MoveImpl {};

    template<size_t n, typename... As>
    struct DestroctorImpl {};

    template<size_t n, typename... As>
    struct EqualityImpl {};

    template<typename... As>
    class EnumBase {
    public:
        template<uint8_t stamp_n, template<uint8_t> typename BranchF, typename... Args>
        struct SwitchFunction<stamp_n, BranchF, Args...>;

        constexpr static uint8_t alt_num = sizeof...(As);
        constexpr static uint8_t switch_size = power_2_ceiling(alt_num);

        // friend struct Switch<switch_size>;

        template<uint8_t i>
        friend struct CCBranch;

        template<uint8_t i>
        friend struct CABranch;

        template<uint8_t i>
        friend struct MCBranch;

        template<uint8_t i>
        friend struct MACBranch;

        // friend struct CopyImpl<switch_size, As...>;
        // friend struct MoveImpl<switch_size, As...>;
        // friend struct DestroctorImpl<switch_size, As...>;
        // friend struct MatchImpl<switch_size, As...>;
        // friend struct EqualityImpl<switch_size, As...>;

        template<typename A>
        struct IsSameUnary {
            template<typename B>
            struct Binded {
                static const bool value = IsSame<A, B>::value;
            };
        };

        template<typename A>
        struct VariantIndex: Find<IsSameUnary<A>::template Binded, As...> {};

        // Default constructor will construct the first variant if the first
        // variant is default constructible
        EnumBase() : _index {0} {
            using Variant = PackAt<0, As...>;
            new (reinterpret_cast<Variant*>(_storage)) Variant {};
        }

        // EnumBase(const EnumBase& other) : _index(other._index) {
        //     detail::CopyImpl<switch_size, As...>::impl(*this, other);
        // }

        EnumBase(const EnumBase& other) : _index(other._index) {
            FunctionSwitch<switch_size, CopyImpl, EnumBase, const EnumBase&>::f(*this, other);
            
        }

        EnumBase& operator=(const EnumBase& other) {
            if (this != &other) {
                // Should distroy the current variant
                detail::DestroctorImpl<switch_size, As...>::impl(*this);

                _index = other._index;
                detail::CopyImpl<switch_size, As...>::impl(*this, other);
            }
            return *this;
        }

        EnumBase(EnumBase&& other) noexcept : _index(other._index) {
            detail::MoveImpl<switch_size, As...>::impl(*this, efp::move(other));
            // Destructor calling will be done for the moved object.
            // Behavior is up to each variant.
        }

        EnumBase& operator=(EnumBase&& other) noexcept {
            if (this != &other) {
                // Should distroy the current variant
                detail::DestroctorImpl<switch_size, As...>::impl(*this);

                _index = other._index;
                detail::MoveImpl<switch_size, As...>::impl(*this, efp::move(other));
                // Destructor calling will be done for the moved object.
                // Behavior is up to each
            }
            return *this;
        }

        ~EnumBase() {
            detail::DestroctorImpl<switch_size, As...>::impl(*this);
        }

        template<typename A>
        EnumBase(const A& a) : _index(VariantIndex<A>::value) {
            static_assert(
                Any<IsSame<A, As>...>::value,
                "Argument type should be one of the variant types"
            );

            new (reinterpret_cast<A*>(_storage)) A(a);
        }

        template<typename A>
        EnumBase(A&& a) : _index(VariantIndex<A>::value) {
            static_assert(
                Any<IsSame<A, As>...>::value,
                "Argument type should be one of the variant types"
            );

            new (reinterpret_cast<A*>(_storage)) A(efp::move(a));
        }

        bool operator==(const EnumBase& other) const {
            if (_index != other._index) {
                return false;
            }

            return detail::EqualityImpl<switch_size, As...>::impl(*this, other);
        }

        bool operator!=(const EnumBase& other) const {
            return !(*this == other);
        }

        uint8_t index() const {
            return _index;
        }

        template<typename A>
        auto get() const -> A {
            static_assert(Any<IsSame<A, As>...>::value, "Invalid variant type");

            if (_index != VariantIndex<A>::value) {
                throw std::runtime_error("Wrong variant index");
            }

            return *(reinterpret_cast<const A*>(_storage));
        }

        template<uint8_t n>
        auto get() const -> PackAt<n, As...> {
            static_assert(n < sizeof...(As), "Invalid variant index");

            if (_index != n) {
                throw std::runtime_error("Wrong variant index");
            }

            return *(reinterpret_cast<const PackAt<n, As...>*>(_storage));
        }

        template<typename A>
        auto move() const -> A {
            static_assert(Any<IsSame<A, As>...>::value, "Invalid variant type");

            if (_index != VariantIndex<A>::value) {
                throw std::runtime_error("Wrong variant index");
            }

            return efp::move(*(reinterpret_cast<const A*>(_storage)));
        }

        template<typename A>
        auto move() -> A {
            static_assert(Any<IsSame<A, As>...>::value, "Invalid variant type");

            if (_index != VariantIndex<A>::value) {
                throw std::runtime_error("Wrong variant index");
            }

            return efp::move(*(reinterpret_cast<A*>(_storage)));
        }

        template<uint8_t n>
        auto move() const -> PackAt<n, As...> {
            static_assert(n < sizeof...(As), "Invalid variant index");

            if (_index != n) {
                throw std::runtime_error("Wrong variant index");
            }

            return efp::move(*(reinterpret_cast<PackAt<n, As...>*>(_storage)));
        }

        template<uint8_t n>
        auto move() -> PackAt<n, As...> {
            static_assert(n < sizeof...(As), "Invalid variant index");

            if (_index != n) {
                throw std::runtime_error("Wrong variant index");
            }

            return efp::move(*(reinterpret_cast<PackAt<n, As...>*>(_storage)));
        }

        // todo emplace

        // * Test if all of the branchs have same return type by Common.
        // * Check if each F has at least one matching with As. (RelevantBranchF)
        // * Check if all the variants are matched at least one of Fs or there
        // is wildcard at last (Exhaustiveness)
        // * Compile time function uint8_t -> uint8_t, which is variant index to
        // branch index.

        // * Test if matchs by
        // * 1. is there is_same operator (Strip ref of arg type.)
        // * 2. is invocable
        // * 3. is void arg type

        // * Need type level maybe
        // * Need type level find (Type -> bool) -> Types -> int
        // * Need type level indexing

        // Arguments implementation will automatically remove the const
        // qualifier if there is.

        template<typename F>
        struct IsWildCard: IsSame<Tuple<>, Arguments<F>> {};

        template<typename F>
        using IsRelevantBranchF = Any<IsWildCard<F>, IsInvocable<F, As>...>;

        template<typename A, typename... Fs>
        using IsVariantCovered = Any<IsInvocable<Fs, A>...>;

        template<typename... Fs>
        using IsExhaustive = Any<IsWildCard<Fs>..., All<IsVariantCovered<As, Fs...>...>>;

        template<typename... Fs>
        struct IsWellFormed {
            static constexpr bool value = true;
            // todo one or zero argument, and each branch should be handled only once.
        };

        struct MatchImpl {
            template<uint8_t i>
            auto operator()()

        }

        template<typename F, typename... Fs>
        auto match(F&& f, Fs&&... fs) const -> Return<F> {
            static_assert(
                All<IsRelevantBranchF<F>, IsRelevantBranchF<Fs>...>::value,
                "Not all branches are relevant"
            );

            static_assert(IsExhaustive<F, Fs...>::value, "Not all variants are matched");

            return Switch<switch_size>::template f<
                Overloaded<MatchBranchF<F>, MatchBranchF<Fs>...>>(
                efp::move<Overloaded<MatchBranchF<F>, MatchBranchF<Fs>...>>(
                    Overloaded<MatchBranchF<F>, MatchBranchF<Fs>...> {
                        efp::forward<F>(f),
                        efp::forward<Fs>(fs)...
                    }
                ),
                *this
            );
        }

    private:
        // #define CASE(i) \
//     case i: \
//         return o( \
//             self.template get < (i < sizeof...(As)) ? i : sizeof...(As) - 1 > (), \
//             efp::forward(args)... \
//         );

// * Has to be more generic
// * It will take index and this pointer with extra arguments. o, self, args..
#define FUNCTION_CASE(i) \
    case i: { \
        return BranchF<i>(self, efp::forward<Args>(args)...);

        template<template<uint8_t> typename BranchF, typename... Args>
        struct SwitchFunction<2, BranchF, Args...> {
            static auto operator()(const EnumBase* self, Args&&... args)
                -> CallReturn<O, PackAt<0, As...>> {
                switch (self._index) {
                    STAMP2(0, FUNCTION_CASE)
                    default:
                        throw std::runtime_error("Wrong variant index");
                }
            }

            static auto operator()(EnumBase* self, Args&&... args)
                -> CallReturn<O, PackAt<0, As...>> {
                switch (self._index) {
                    STAMP2(0, FUNCTION_CASE)
                    default:
                        throw std::runtime_error("Wrong variant index");
                }
            }
        };

        template<template<uint8_t> typename BranchF, typename... Args>
        struct SwitchFunction<4, BranchF, Args...> {
            static auto operator()(const EnumBase* self, Args&&... args)
                -> CallReturn<O, PackAt<0, As...>> {
                switch (self._index) {
                    STAMP4(0, FUNCTION_CASE)
                    default:
                        throw std::runtime_error("Wrong variant index");
                }
            }

            static auto operator()(EnumBase* self, Args&&... args)
                -> CallReturn<O, PackAt<0, As...>> {
                switch (self._index) {
                    STAMP4(0, FUNCTION_CASE)
                    default:
                        throw std::runtime_error("Wrong variant index");
                }
            }
        };

        template<template<uint8_t> typename BranchF, typename... Args>
        struct SwitchFunction<8, BranchF, Args...> {
            static auto operator()(const EnumBase* self, Args&&... args)
                -> CallReturn<O, PackAt<0, As...>> {
                switch (self._index) {
                    STAMP8(0, FUNCTION_CASE)
                    default:
                        throw std::runtime_error("Wrong variant index");
                }
            }

            static auto operator()(EnumBase* self, Args&&... args)
                -> CallReturn<O, PackAt<0, As...>> {
                switch (self._index) {
                    STAMP8(0, FUNCTION_CASE)
                    default:
                        throw std::runtime_error("Wrong variant index");
                }
            }
        };

        template<uint8_t i>
        struct CCBranch {
            static void operator()(EnumBase* self, EnumBase* other) {
                using Variant = PackAt < i<sizeof...(As) ? i : sizeof...(As) - 1, As...>;

                new (self->_storage) Variant(*reinterpret_cast<Variant*>(other->_storage));
            }
        }

        template<uint8_t i>
        struct CABranch {
            static void operator()(EnumBase* self, const EnumBase* other) {
                using Variant = PackAt < i<sizeof...(As) ? i : sizeof...(As) - 1, As...>;

                *reinterpret_cast<Variant*>(self->_storage).~Variant();

                new (self->_storage) Variant(*reinterpret_cast<Variant*>(other->_storage));
            }
        }

        template<uint8_t i>
        struct MCBranch {
            static void operator()(EnumBase* self, EnumBase* other) {
                using Variant = PackAt < i<sizeof...(As) ? i : sizeof...(As) - 1, As...>;

                new (self->_storage)
                    Variant(efp::move(*reinterpret_cast<Variant*>(other->_storage)));
            }
        }

        template<uint8_t i>
        struct MACBranch {
            static void operator()(EnumBase* self, const EnumBase* other) {
                using Variant = PackAt < i<sizeof...(As) ? i : sizeof...(As) - 1, As...>;

                *reinterpret_cast<Variant*>(self->_storage).~Variant();

                new (self->_storage)
                    Variant(efp::move(*reinterpret_cast<Variant*>(other->_storage)));
            }
        }

        // Need to speparate match case from others

        // todo More

        // Member variables
        alignas(_maximum(alignof(As)...)) uint8_t _storage[_maximum(sizeof(As)...)];

        uint8_t _index;  // Current maxinum variant is 256
    };

    // template specialization could not be in the class scope.
    // CopyImpl

    template<typename... As>
    struct CopyImpl<2, As...> {
        static void impl(EnumBase<As...>& dest, const EnumBase<As...>& src) {
            switch (src._index) {
                STAMP2(0, COPY_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template<typename... As>
    struct CopyImpl<4, As...> {
        static void impl(EnumBase<As...>& dest, const EnumBase<As...>& src) {
            switch (src._index) {
                STAMP4(0, COPY_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template<typename... As>
    struct CopyImpl<8, As...> {
        static void impl(EnumBase<As...>& dest, const EnumBase<As...>& src) {
            switch (src._index) {
                STAMP8(0, COPY_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template<typename... As>
    struct CopyImpl<16, As...> {
        static void impl(EnumBase<As...>& dest, const EnumBase<As...>& src) {
            switch (src._index) {
                STAMP16(0, COPY_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template<typename... As>
    struct CopyImpl<32, As...> {
        static void impl(EnumBase<As...>& dest, const EnumBase<As...>& src) {
            switch (src._index) {
                STAMP32(0, COPY_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template<typename... As>
    struct CopyImpl<64, As...> {
        static void impl(EnumBase<As...>& dest, const EnumBase<As...>& src) {
            switch (src._index) {
                STAMP64(0, COPY_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template<typename... As>
    struct CopyImpl<128, As...> {
        static void impl(EnumBase<As...>& dest, const EnumBase<As...>& src) {
            switch (src._index) {
                STAMP128(0, COPY_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template<typename... As>
    struct CopyImpl<256, As...> {
        static void impl(EnumBase<As...>& dest, const EnumBase<As...>& src) {
            switch (src._index) {
                STAMP256(0, COPY_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    // MoveImpl
    template<typename... As>
    struct MoveImpl<2, As...> {
        static void impl(EnumBase<As...>& dest, EnumBase<As...>&& src) {
            switch (src._index) {
                STAMP2(0, MOVE_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template<typename... As>
    struct MoveImpl<4, As...> {
        static void impl(EnumBase<As...>& dest, EnumBase<As...>&& src) {
            switch (src._index) {
                STAMP4(0, MOVE_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template<typename... As>
    struct MoveImpl<8, As...> {
        static void impl(EnumBase<As...>& dest, EnumBase<As...>&& src) {
            switch (src._index) {
                STAMP8(0, MOVE_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template<typename... As>
    struct MoveImpl<16, As...> {
        static void impl(EnumBase<As...>& dest, EnumBase<As...>&& src) {
            switch (src._index) {
                STAMP16(0, MOVE_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template<typename... As>
    struct MoveImpl<32, As...> {
        static void impl(EnumBase<As...>& dest, EnumBase<As...>&& src) {
            switch (src._index) {
                STAMP32(0, MOVE_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template<typename... As>
    struct MoveImpl<64, As...> {
        static void impl(EnumBase<As...>& dest, EnumBase<As...>&& src) {
            switch (src._index) {
                STAMP64(0, MOVE_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template<typename... As>
    struct MoveImpl<128, As...> {
        static void impl(EnumBase<As...>& dest, EnumBase<As...>&& src) {
            switch (src._index) {
                STAMP128(0, MOVE_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template<typename... As>
    struct MoveImpl<256, As...> {
        static void impl(EnumBase<As...>& dest, EnumBase<As...>&& src) {
            switch (src._index) {
                STAMP256(0, MOVE_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    // DestroctorImpl

    template<typename... As>
    struct DestroctorImpl<2, As...> {
        static void impl(EnumBase<As...>& self) {
            switch (self._index) {
                STAMP2(0, DESTROCTOR_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template<typename... As>
    struct DestroctorImpl<4, As...> {
        static void impl(EnumBase<As...>& self) {
            switch (self._index) {
                STAMP4(0, DESTROCTOR_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template<typename... As>
    struct DestroctorImpl<8, As...> {
        static void impl(EnumBase<As...>& self) {
            switch (self._index) {
                STAMP8(0, DESTROCTOR_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template<typename... As>
    struct DestroctorImpl<16, As...> {
        static void impl(EnumBase<As...>& self) {
            switch (self._index) {
                STAMP16(0, DESTROCTOR_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template<typename... As>
    struct DestroctorImpl<32, As...> {
        static void impl(EnumBase<As...>& self) {
            switch (self._index) {
                STAMP32(0, DESTROCTOR_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template<typename... As>
    struct DestroctorImpl<64, As...> {
        static void impl(EnumBase<As...>& self) {
            switch (self._index) {
                STAMP64(0, DESTROCTOR_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template<typename... As>
    struct DestroctorImpl<128, As...> {
        static void impl(EnumBase<As...>& self) {
            switch (self._index) {
                STAMP128(0, DESTROCTOR_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template<typename... As>
    struct DestroctorImpl<256, As...> {
        static void impl(EnumBase<As...>& self) {
            switch (self._index) {
                STAMP256(0, DESTROCTOR_CASE)
                default:
                    throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    // MatchImpl

    // template<typename... As>
    // struct MatchImpl<2, As...> {
    //     template<typename... Fs>
    //     static auto impl(const Overloaded<Fs...>& overloaded, const EnumBase<As...>* outer)
    //         -> Common<Return<Fs>...> {
    //         switch (outer->_index) {
    //             STAMP2(0, MATCH_CASE)
    //             default:
    //                 throw std::runtime_error("Invalied Enum variant index");
    //         }
    //     }
    // };

    // template<typename... As>
    // struct MatchImpl<4, As...> {
    //     template<typename... Fs>
    //     static auto impl(const Overloaded<Fs...>& overloaded, const EnumBase<As...>* outer)
    //         -> Common<Return<Fs>...> {
    //         switch (outer->_index) {
    //             STAMP4(0, MATCH_CASE)
    //             default:
    //                 throw std::runtime_error("Invalied Enum variant index");
    //         }
    //     }
    // };

    // template<typename... As>
    // struct MatchImpl<8, As...> {
    //     template<typename... Fs>
    //     static auto impl(const Overloaded<Fs...>& overloaded, const EnumBase<As...>* outer)
    //         -> Common<Return<Fs>...> {
    //         switch (outer->_index) {
    //             STAMP8(0, MATCH_CASE)
    //             default:
    //                 throw std::runtime_error("Invalied Enum variant index");
    //         }
    //     }
    // };

    // template<typename... As>
    // struct MatchImpl<16, As...> {
    //     template<typename... Fs>
    //     static auto impl(const Overloaded<Fs...>& overloaded, const EnumBase<As...>* outer)
    //         -> Common<Return<Fs>...> {
    //         switch (outer->_index) {
    //             STAMP16(0, MATCH_CASE)
    //             default:
    //                 throw std::runtime_error("Invalied Enum variant index");
    //         }
    //     }
    // };

    // template<typename... As>
    // struct MatchImpl<32, As...> {
    //     template<typename... Fs>
    //     static auto impl(const Overloaded<Fs...>& overloaded, const EnumBase<As...>* outer)
    //         -> Common<Return<Fs>...> {
    //         switch (outer->_index) {
    //             STAMP32(0, MATCH_CASE)
    //             default:
    //                 throw std::runtime_error("Invalied Enum variant index");
    //         }
    //     }
    // };

    // template<typename... As>
    // struct MatchImpl<64, As...> {
    //     template<typename... Fs>
    //     static auto impl(const Overloaded<Fs...>& overloaded, const EnumBase<As...>* outer)
    //         -> Common<Return<Fs>...> {
    //         switch (outer->_index) {
    //             STAMP64(0, MATCH_CASE)
    //             default:
    //                 throw std::runtime_error("Invalied Enum variant index");
    //         }
    //     }
    // };

    // template<typename... As>
    // struct MatchImpl<128, As...> {
    //     template<typename... Fs>
    //     static auto impl(const Overloaded<Fs...>& overloaded, const EnumBase<As...>* outer)
    //         -> Common<Return<Fs>...> {
    //         switch (outer->_index) {
    //             STAMP128(0, MATCH_CASE)
    //             default:
    //                 throw std::runtime_error("Invalied Enum variant index");
    //         }
    //     }
    // };

    // template<typename... As>
    // struct MatchImpl<256, As...> {
    //     template<typename... Fs>
    //     static auto impl(const Overloaded<Fs...>& overloaded, const EnumBase<As...>* outer)
    //         -> Common<Return<Fs>...> {
    //         switch (outer->_index) {
    //             STAMP256(0, MATCH_CASE)
    //             default:
    //                 throw std::runtime_error("Invalied Enum variant index");
    //         }
    //     }
    // };

    // EqualityImpl

    template<typename... As>
    struct EqualityImpl<2, As...> {
        static bool impl(const EnumBase<As...>& self, const EnumBase<As...>& other) {
            switch (self._index) {
                STAMP2(0, EQUALITY_CASE)
                default:
                    throw std::runtime_error("Wrong variant index");
            }
        }
    };

    template<typename... As>
    struct EqualityImpl<4, As...> {
        static bool impl(const EnumBase<As...>& self, const EnumBase<As...>& other) {
            switch (self._index) {
                STAMP4(0, EQUALITY_CASE)
                default:
                    throw std::runtime_error("Wrong variant index");
            }
        }
    };

    template<typename... As>
    struct EqualityImpl<8, As...> {
        static bool impl(const EnumBase<As...>& self, const EnumBase<As...>& other) {
            switch (self._index) {
                STAMP8(0, EQUALITY_CASE)
                default:
                    throw std::runtime_error("Wrong variant index");
            }
        }
    };

    template<typename... As>
    struct EqualityImpl<16, As...> {
        static bool impl(const EnumBase<As...>& self, const EnumBase<As...>& other) {
            switch (self._index) {
                STAMP16(0, EQUALITY_CASE)
                default:
                    throw std::runtime_error("Wrong variant index");
            }
        }
    };

    template<typename... As>
    struct EqualityImpl<32, As...> {
        static bool impl(const EnumBase<As...>& self, const EnumBase<As...>& other) {
            switch (self._index) {
                STAMP32(0, EQUALITY_CASE)
                default:
                    throw std::runtime_error("Wrong variant index");
            }
        }
    };

    template<typename... As>
    struct EqualityImpl<64, As...> {
        static bool impl(const EnumBase<As...>& self, const EnumBase<As...>& other) {
            switch (self._index) {
                STAMP64(0, EQUALITY_CASE)
                default:
                    throw std::runtime_error("Wrong variant index");
            }
        }
    };

    template<typename... As>
    struct EqualityImpl<128, As...> {
        static bool impl(const EnumBase<As...>& self, const EnumBase<As...>& other) {
            switch (self._index) {
                STAMP128(0, EQUALITY_CASE)
                default:
                    throw std::runtime_error("Wrong variant index");
            }
        }
    };

    template<typename... As>
    struct EqualityImpl<256, As...> {
        static bool impl(const EnumBase<As...>& self, const EnumBase<As...>& other) {
            switch (self._index) {
                STAMP256(0, EQUALITY_CASE)
                default:
                    throw std::runtime_error("Wrong variant index");
            }
        }
    };

#define DESTROCTOR_CASE(i) \
    case i: { \
        using Variant = PackAt < i<sizeof...(As) ? i : sizeof...(As) - 1, As...>; \
        reinterpret_cast<Variant*>(self._storage)->~Variant(); \
        break; \
    }

#define MATCH_CASE(i) \
    case i: { \
        return overloaded( \
            *(reinterpret_cast \
              < const PackAt<i<sizeof...(As) ? i : sizeof...(As) - 1, As...>*>(outer->_storage)) \
        ); \
        break; \
    }

#undef MATCH_CASE
#undef STAMP2
#undef STAMP4
#undef STAMP8
#undef STAMP16
#undef STAMP32
#undef STAMP64
#undef STAMP128
#undef STAMP256
}  // namespace detail

template<typename... As>
class Enum: public detail::EnumBase<As...> {
    using Base = detail::EnumBase<As...>;
    using Base::Base;
};

template<typename... As, typename... Fs>
auto match(const Enum<As...>& x, const Fs&... fs) -> EnableIf<
    _all(Enum<As...>::template IsRelevantBranchF<Fs>::value...)
        && Enum<As...>::template IsExhaustive<Fs...>::value
        && Enum<As...>::template IsWellFormed<Fs...>::value,
    Common<Return<Fs>...>> {
    return Enum<As...>::template MatchImpl<sizeof...(As)>::impl(Overloaded<Fs...> {fs...}, &x);
}

// EnumAt

namespace detail {
    template<size_t n, typename A>
    struct EnumAtImpl {};

    template<size_t n, typename... As>
    struct EnumAtImpl<n, Enum<As...>> {
        using Type = PackAt<n, As...>;
    };
}  // namespace detail

template<size_t n, typename A>
using EnumAt = typename detail::EnumAtImpl<n, A>::Type;

}  // namespace efp

#endif