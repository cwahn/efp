#ifndef SUM_TYPE_HPP_
#define SUM_TYPE_HPP_

#include "sfinae.hpp"

namespace efp {
// Is WildCard

template <typename F>
using IsWildCard = IsSame<Arguments<ReferenceRemoved<F>>, Tuple<>>;

// WildCardWrapper

template <typename F>
struct WildCardWrapper {
    const F& f;

    template <typename... Args>
    auto operator()(const Args&...) const
        -> decltype(declval<F>()()) {
        return f();
    }
};

template <typename F>
struct detail::ReturnImpl<WildCardWrapper<F>> {
    using Type = decltype(declval<F>()());
};

// MatchBranchImpl
namespace detail {
    template <typename F, typename = void>
    struct MatchBranchImpl {
        using Type = Cleaned<F>;
    };

    template <typename F>
    struct MatchBranchImpl<F, EnableIf<IsWildCard<F>::value, void>> {
        using Type = WildCardWrapper<Cleaned<F>>;
    };
} // namespace detail
// MatchBranch

template <typename F>
using MatchBranch = typename detail::MatchBranchImpl<F>::Type;

// Overloaded

template <typename... Fs>
struct Overloaded;

template <typename F>
struct Overloaded<F> : F {
    using F::operator();

    template <typename G>
    Overloaded(const G& g)
        : F{MatchBranch<G>{g}} {
    }
};

template <class F, class... Fs>
struct Overloaded<F, Fs...> : F, Overloaded<Fs...> {
    using F::operator();
    using Overloaded<Fs...>::operator();

    template <class G, class... Gs>
    Overloaded(G&& g, Gs&&... gs)
        : F{MatchBranch<G>{forward<G>(g)}},
          Overloaded<Fs...>(forward<Gs>(gs)...) {
    }
};

namespace detail {

    constexpr uint8_t power_2_ceiling(uint8_t n, uint8_t power = 2) {
        return (power >= n) ? power : power_2_ceiling(n, power * 2);
    }

#define COPY_CASE(i)                                                              \
    case i: {                                                                     \
        using Variant = PackAt < i<sizeof...(As) ? i : sizeof...(As) - 1, As...>; \
        new (dest._storage) Variant(src.template get<Variant>());                 \
        break;                                                                    \
    }

#define MOVE_CASE(i)                                                              \
    case i: {                                                                     \
        using Variant = PackAt < i<sizeof...(As) ? i : sizeof...(As) - 1, As...>; \
        new (dest._storage) Variant(efp::move(src.template move<Variant>()));     \
        break;                                                                    \
    }

#define DESTROCTOR_CASE(i)                                                        \
    case i: {                                                                     \
        using Variant = PackAt < i<sizeof...(As) ? i : sizeof...(As) - 1, As...>; \
        reinterpret_cast<Variant*>(self._storage)->~Variant();                    \
        break;                                                                    \
    } // namespace detail

#define MATCH_CASE(i)                                                                                                             \
    case i: {                                                                                                                     \
        return overloaded(*(reinterpret_cast < const PackAt<i<sizeof...(As) ? i : sizeof...(As) - 1, As...>*>(outer->_storage))); \
        break;                                                                                                                    \
    }

#define EQUALITY_CASE(i)                                                                                                  \
    case i: {                                                                                                             \
        using Variant = PackAt < i<sizeof...(As) ? i : sizeof...(As) - 1, As...>;                                         \
        return *(reinterpret_cast<const Variant*>(self._storage)) == *(reinterpret_cast<const Variant*>(other._storage)); \
        break;                                                                                                            \
    } // namespace detail

#define STAMP2(n, x) \
    x(n)             \
        x(n + 1)

#define STAMP4(n, x) \
    STAMP2(n, x)     \
    STAMP2(n + 2, x)

#define STAMP8(n, x) \
    STAMP4(n, x)     \
    STAMP4(n + 4, x)

#define STAMP16(n, x) \
    STAMP8(n, x)      \
    STAMP8(n + 8, x)

#define STAMP32(n, x) \
    STAMP16(n, x)     \
    STAMP16(n + 16, x)

#define STAMP64(n, x) \
    STAMP32(n, x)     \
    STAMP32(n + 32, x)

#define STAMP128(n, x) \
    STAMP64(n, x)      \
    STAMP64(n + 64, x)

#define STAMP256(n, x) \
    STAMP128(n, x)     \
    STAMP128(n + 128, x)

    template <size_t n, typename... As>
    struct MatchImpl {};

    template <size_t n, typename... As>
    struct CopyImpl {};

    template <size_t n, typename... As>
    struct MoveImpl {};

    template <size_t n, typename... As>
    struct DestroctorImpl {};

    template <size_t n, typename... As>
    struct EqualityImpl {};

    template <typename... As>
    class EnumBase {

    public:
        friend struct detail::CopyImpl<power_2_ceiling(sizeof...(As)), As...>;
        friend struct detail::MoveImpl<power_2_ceiling(sizeof...(As)), As...>;
        friend struct detail::DestroctorImpl<power_2_ceiling(sizeof...(As)), As...>;
        friend struct detail::MatchImpl<power_2_ceiling(sizeof...(As)), As...>;
        friend struct detail::EqualityImpl<power_2_ceiling(sizeof...(As)), As...>;

        template <typename A>
        struct IsSameUnary {
            template <typename B>
            struct Binded {
                static const bool value = IsSame<A, B>::value;
            };
        };

        template <typename A>
        struct VariantIndex
            : Find<IsSameUnary<A>::template Binded, As...> {
        };

        // Count how many types in the pack are constructible with Args...
        template <typename... Types>
        struct ConstructibleCount {};

        template <typename T>
        struct ConstructibleCount<T> {
            template <typename... Args>
            using Type = Size<IsConstructible<T, Args...>::value ? 1 : 0>;
        };

        template <typename First, typename... Rest>
        struct ConstructibleCount<First, Rest...> {
            template <typename... Args>
            using Type =
                Size<IsConstructible<First, Args...>::value ? 1 : 0 + ConstructibleCount<Rest...>::template Type<Args...>::value>;
        };

        template <typename... Args>
        using IsUniquelyConstructible = Bool<ConstructibleCount<As...>::template Type<Args...>::value == 1>;

        // Base case: no types are constructible
        template <typename...>
        struct FirstConstructible {
            template <typename... Args>
            using Type = void; // Fallback type if no constructible type is found
        };

        // Specialization for at least one type in the pack
        template <typename First, typename... Rest>
        struct FirstConstructible<First, Rest...> {
            template <typename... Args>
            using Type = typename std::conditional<
                IsConstructible<First, Args...>::value,
                First,
                typename FirstConstructible<Rest...>::template Type<Args...>>::type;
        };

        template <typename... Args>
        using DetermineVariant = EnableIf<
            IsUniquelyConstructible<Args...>::value,
            typename FirstConstructible<As...>::template Type<Args...>>;

        // Default constructor will construct the first variant if the first variant is default constructible
        EnumBase()
            : _index{0} {
            using Variant = PackAt<0, As...>;
            new (reinterpret_cast<Variant*>(_storage)) Variant{};
        }

        // todo Support rule of five
        EnumBase(const EnumBase& other)
            : _index(other._index) {
            detail::CopyImpl<power_2_ceiling(sizeof...(As)), As...>::impl(*this, other);
        }

        EnumBase& operator=(const EnumBase& other) {
            if (this != &other) {
                detail::CopyImpl<power_2_ceiling(sizeof...(As)), As...>::impl(*this, other);
            }
            return *this;
        }

        EnumBase(EnumBase&& other) noexcept
            : _index(other._index) {
            detail::MoveImpl<power_2_ceiling(sizeof...(As)), As...>::impl(*this, std::move(other));
        }

        EnumBase& operator=(EnumBase&& other) noexcept {
            if (this != &other) {
                detail::MoveImpl<power_2_ceiling(sizeof...(As)), As...>::impl(*this, std::move(other));
            }
            return *this;
        }

        ~EnumBase() {
            detail::DestroctorImpl<power_2_ceiling(sizeof...(As)), As...>::impl(*this);
        }

        // Function name or function type will be automatically converted to function pointer type
        template <typename A, typename = EnableIf<any_v(IsSame<FuncToFuncPtr<A>, As>::value...)>>
        EnumBase(const A& a)
            : _index(VariantIndex<FuncToFuncPtr<A>>::value) {
            new (reinterpret_cast<FuncToFuncPtr<A>*>(_storage)) FuncToFuncPtr<A>(a);
        }

        // Function name or function type will be automatically converted to function pointer type
        template <typename A, typename = EnableIf<any_v(IsSame<FuncToFuncPtr<A>, As>::value...)>>
        EnumBase(A&& a)
            : _index(VariantIndex<FuncToFuncPtr<A>>::value) {
            new (reinterpret_cast<FuncToFuncPtr<A>*>(_storage)) FuncToFuncPtr<A>(efp::move(a));
        }

        // Extended constructor
        // Templated constructor for forwarding arguments to the variants' constructors
        template <
            typename Head,
            typename... Tail,
            typename = EnableIf<!(sizeof...(Tail) == 0 && Any<IsSame<As, Head>...>::value) && IsUniquelyConstructible<Head, Tail...>::value, void>>
        EnumBase(Head&& head, Tail&&... args)
            : _index(VariantIndex<DetermineVariant<Head, Tail...>>::value) {
            // Determine the appropriate variant type based on the argument
            using VariantType = DetermineVariant<Head, Tail...>; // Implement this based on your logic

            // Construct the variant in place
            new (reinterpret_cast<VariantType*>(_storage)) VariantType(forward<Head>(head), forward<Tail>(args)...);
        }

        // todo Implement equality operator
        bool operator==(const EnumBase& other) const {
            if (_index != other._index) {
                return false;
            }

            return detail::EqualityImpl<power_2_ceiling(sizeof...(As)), As...>::impl(*this, other);
        }

        bool operator!=(const EnumBase& other) const {
            return !(*this == other);
        }

        uint8_t index() const {
            return _index;
        }

        template <typename A>
        auto get() const
            -> EnableIf<any_v(IsSame<A, As>::value...), A> {
            if (_index != VariantIndex<A>::value) {
                throw std::runtime_error("Wrong variant index");
            }

            return *(reinterpret_cast<const A*>(_storage));
        }

        template <uint8_t n>
        auto get()
            -> EnableIf<lt_v(n, sizeof...(As)), PackAt<n, As...>> {
            if (_index != n) {
                throw std::runtime_error("Wrong variant index");
            }

            return *(reinterpret_cast<PackAt<n, As...>*>(_storage));
        }

        template <typename A>
        auto move() const
            -> EnableIf<any_v(IsSame<A, As>::value...), const A&&> {
            if (_index != VariantIndex<A>::value) {
                throw std::runtime_error("Wrong variant index");
            }

            return efp::move(*(reinterpret_cast<const A*>(_storage)));
        }

        template <typename A>
        auto move()
            -> EnableIf<any_v(IsSame<A, As>::value...), A&&> {
            if (_index != VariantIndex<A>::value) {
                throw std::runtime_error("Wrong variant index");
            }

            return efp::move(*(reinterpret_cast<A*>(_storage)));
        }

        template <uint8_t n>
        auto move() const
            -> EnableIf<lt_v(n, sizeof...(As)), PackAt<n, As...> const&&> {
            if (_index != n) {
                throw std::runtime_error("Wrong variant index");
            }

            return efp::move(*(reinterpret_cast<const PackAt<n, As...>*>(_storage)));
        }

        template <uint8_t n>
        auto move()
            -> EnableIf<lt_v(n, sizeof...(As)), PackAt<n, As...>&&> {
            if (_index != n) {
                throw std::runtime_error("Wrong variant index");
            }

            return efp::move(*(reinterpret_cast<PackAt<n, As...>*>(_storage)));
        }

        // * Test if all of the branchs have same return type by Common.
        // * Check if each F has at least one matching with As. (RelevantBranch)
        // * Check if all the variants are matched at least one of Fs or there is wildcard at last (Exhaustiveness)
        // * Compile time function uint8_t -> uint8_t, which is variant index to branch index.

        // * Test if matchs by
        // * 1. is there is_same operator (Strip ref of arg type.)
        // * 2. is invocable
        // * 3. is void arg type

        // * Need type level maybe
        // * Need type level find (Type -> bool) -> Types -> int
        // * Need type level indexing

        // Arguments implementation will automatically remove the const qualifier if there is.

        template <typename F>
        struct IsRelevantBranch {
            static constexpr bool value =
                any_v(IsInvocable<F, As>::value...) || IsSame<Tuple<>, Arguments<F>>::value;
        };

        template <typename... Fs>
        struct AreAllRelevantBranchs {
            static constexpr bool value = all_v(IsRelevantBranch<Fs>::value...);
        };

        template <typename F>
        struct IsWildCard : IsSame<Tuple<>, Arguments<F>> {
        };

        template <typename A, typename... Fs>
        struct IsVariantCovered {
            static constexpr bool value =
                any_v(IsWildCard<Fs>::value...) || any_v(IsInvocable<Fs, A>::value...);
        };

        template <typename... Fs>
        struct IsExhaustive : False {
            static constexpr bool value = all_v(IsVariantCovered<As, Fs...>::value...);
        };

        template <typename... Fs>
        struct IsWellFormed {
            static constexpr bool value = true;
            // todo count and get last
            // (!any_v(IsWildCard<Init>::value...) && IsWildCard<Last>::value) ||
            // !any_v(IsWildCard<Fs...>::value...);
        };

        template <typename... Fs>
        auto match(const Fs&... fs) const
            -> EnableIf<
                AreAllRelevantBranchs<Fs...>::value &&
                    IsExhaustive<Fs...>::value &&
                    IsWellFormed<Fs...>::value,
                Common<Return<Fs>...>> {
            return detail::MatchImpl<detail::power_2_ceiling(sizeof...(As)), As...>::impl(Overloaded<MatchBranch<Fs>...>{fs...}, this);
        }

    private:
        // Member variables
        alignas(maximum_v(alignof(As)...)) uint8_t _storage[maximum_v(sizeof(As)...)];
        uint8_t _index; // Current maxinum variant is 256
    };

    // template specialization could not be in the class scope.
    // CopyImpl

    template <typename... As>
    struct CopyImpl<2, As...> {
        static void impl(EnumBase<As...>& dest, const EnumBase<As...>& src) {
            switch (src._index) {
                STAMP2(0, COPY_CASE)
            default:
                throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template <typename... As>
    struct CopyImpl<4, As...> {
        static void impl(EnumBase<As...>& dest, const EnumBase<As...>& src) {
            switch (src._index) {
                STAMP4(0, COPY_CASE)
            default:
                throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template <typename... As>
    struct CopyImpl<8, As...> {
        static void impl(EnumBase<As...>& dest, const EnumBase<As...>& src) {
            switch (src._index) {
                STAMP8(0, COPY_CASE)
            default:
                throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template <typename... As>
    struct CopyImpl<16, As...> {
        static void impl(EnumBase<As...>& dest, const EnumBase<As...>& src) {
            switch (src._index) {
                STAMP16(0, COPY_CASE)
            default:
                throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template <typename... As>
    struct CopyImpl<32, As...> {
        static void impl(EnumBase<As...>& dest, const EnumBase<As...>& src) {
            switch (src._index) {
                STAMP32(0, COPY_CASE)
            default:
                throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template <typename... As>
    struct CopyImpl<64, As...> {
        static void impl(EnumBase<As...>& dest, const EnumBase<As...>& src) {
            switch (src._index) {
                STAMP64(0, COPY_CASE)
            default:
                throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template <typename... As>
    struct CopyImpl<128, As...> {
        static void impl(EnumBase<As...>& dest, const EnumBase<As...>& src) {
            switch (src._index) {
                STAMP128(0, COPY_CASE)
            default:
                throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template <typename... As>
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
    template <typename... As>
    struct MoveImpl<2, As...> {
        static void impl(EnumBase<As...>& dest, EnumBase<As...>&& src) {
            switch (src._index) {
                STAMP2(0, MOVE_CASE)
            default:
                throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template <typename... As>
    struct MoveImpl<4, As...> {
        static void impl(EnumBase<As...>& dest, EnumBase<As...>&& src) {
            switch (src._index) {
                STAMP4(0, MOVE_CASE)
            default:
                throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template <typename... As>
    struct MoveImpl<8, As...> {
        static void impl(EnumBase<As...>& dest, EnumBase<As...>&& src) {
            switch (src._index) {
                STAMP8(0, MOVE_CASE)
            default:
                throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template <typename... As>
    struct MoveImpl<16, As...> {
        static void impl(EnumBase<As...>& dest, EnumBase<As...>&& src) {
            switch (src._index) {
                STAMP16(0, MOVE_CASE)
            default:
                throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template <typename... As>
    struct MoveImpl<32, As...> {
        static void impl(EnumBase<As...>& dest, EnumBase<As...>&& src) {
            switch (src._index) {
                STAMP32(0, MOVE_CASE)
            default:
                throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template <typename... As>
    struct MoveImpl<64, As...> {
        static void impl(EnumBase<As...>& dest, EnumBase<As...>&& src) {
            switch (src._index) {
                STAMP64(0, MOVE_CASE)
            default:
                throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template <typename... As>
    struct MoveImpl<128, As...> {
        static void impl(EnumBase<As...>& dest, EnumBase<As...>&& src) {
            switch (src._index) {
                STAMP128(0, MOVE_CASE)
            default:
                throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template <typename... As>
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

    template <typename... As>
    struct DestroctorImpl<2, As...> {
        static void impl(EnumBase<As...>& self) {
            switch (self._index) {
                STAMP2(0, DESTROCTOR_CASE)
            default:
                throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template <typename... As>
    struct DestroctorImpl<4, As...> {
        static void impl(EnumBase<As...>& self) {
            switch (self._index) {
                STAMP4(0, DESTROCTOR_CASE)
            default:
                throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template <typename... As>
    struct DestroctorImpl<8, As...> {
        static void impl(EnumBase<As...>& self) {
            switch (self._index) {
                STAMP8(0, DESTROCTOR_CASE)
            default:
                throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template <typename... As>
    struct DestroctorImpl<16, As...> {
        static void impl(EnumBase<As...>& self) {
            switch (self._index) {
                STAMP16(0, DESTROCTOR_CASE)
            default:
                throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template <typename... As>
    struct DestroctorImpl<32, As...> {
        static void impl(EnumBase<As...>& self) {
            switch (self._index) {
                STAMP32(0, DESTROCTOR_CASE)
            default:
                throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template <typename... As>
    struct DestroctorImpl<64, As...> {
        static void impl(EnumBase<As...>& self) {
            switch (self._index) {
                STAMP64(0, DESTROCTOR_CASE)
            default:
                throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template <typename... As>
    struct DestroctorImpl<128, As...> {
        static void impl(EnumBase<As...>& self) {
            switch (self._index) {
                STAMP128(0, DESTROCTOR_CASE)
            default:
                throw std::runtime_error("Invalid Enum variant index");
            }
        }
    };

    template <typename... As>
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

    template <typename... As>
    struct MatchImpl<2, As...> {
        template <typename... Fs>
        static auto impl(
            const Overloaded<Fs...>& overloaded,
            const EnumBase<As...>* outer)
            -> Common<Return<Fs>...> {
            switch (outer->_index) {
                STAMP2(0, MATCH_CASE)
            default:
                throw std::runtime_error("Invalied Enum variant index");
            }
        }
    };

    template <typename... As>
    struct MatchImpl<4, As...> {
        template <typename... Fs>
        static auto impl(
            const Overloaded<Fs...>& overloaded,
            const EnumBase<As...>* outer)
            -> Common<Return<Fs>...> {
            switch (outer->_index) {
                STAMP4(0, MATCH_CASE)
            default:
                throw std::runtime_error("Invalied Enum variant index");
            }
        }
    };

    template <typename... As>
    struct MatchImpl<8, As...> {
        template <typename... Fs>
        static auto impl(
            const Overloaded<Fs...>& overloaded,
            const EnumBase<As...>* outer)
            -> Common<Return<Fs>...> {
            switch (outer->_index) {
                STAMP8(0, MATCH_CASE)
            default:
                throw std::runtime_error("Invalied Enum variant index");
            }
        }
    };

    template <typename... As>
    struct MatchImpl<16, As...> {
        template <typename... Fs>
        static auto impl(
            const Overloaded<Fs...>& overloaded,
            const EnumBase<As...>* outer)
            -> Common<Return<Fs>...> {
            switch (outer->_index) {
                STAMP16(0, MATCH_CASE)
            default:
                throw std::runtime_error("Invalied Enum variant index");
            }
        }
    };

    template <typename... As>
    struct MatchImpl<32, As...> {
        template <typename... Fs>
        static auto impl(
            const Overloaded<Fs...>& overloaded,
            const EnumBase<As...>* outer)
            -> Common<Return<Fs>...> {
            switch (outer->_index) {
                STAMP32(0, MATCH_CASE)
            default:
                throw std::runtime_error("Invalied Enum variant index");
            }
        }
    };

    template <typename... As>
    struct MatchImpl<64, As...> {
        template <typename... Fs>
        static auto impl(
            const Overloaded<Fs...>& overloaded,
            const EnumBase<As...>* outer)
            -> Common<Return<Fs>...> {
            switch (outer->_index) {
                STAMP64(0, MATCH_CASE)
            default:
                throw std::runtime_error("Invalied Enum variant index");
            }
        }
    };

    template <typename... As>
    struct MatchImpl<128, As...> {
        template <typename... Fs>
        static auto impl(
            const Overloaded<Fs...>& overloaded,
            const EnumBase<As...>* outer)
            -> Common<Return<Fs>...> {
            switch (outer->_index) {
                STAMP128(0, MATCH_CASE)
            default:
                throw std::runtime_error("Invalied Enum variant index");
            }
        }
    };

    template <typename... As>
    struct MatchImpl<256, As...> {
        template <typename... Fs>
        static auto impl(
            const Overloaded<Fs...>& overloaded,
            const EnumBase<As...>* outer)
            -> Common<Return<Fs>...> {
            switch (outer->_index) {
                STAMP256(0, MATCH_CASE)
            default:
                throw std::runtime_error("Invalied Enum variant index");
            }
        }
    };

    // EqualityImpl

    template <typename... As>
    struct EqualityImpl<2, As...> {
        static bool impl(const EnumBase<As...>& self, const EnumBase<As...>& other) {
            switch (self._index) {
                STAMP2(0, EQUALITY_CASE)
            default:
                throw std::runtime_error("Wrong variant index");
            }
        }
    };

    template <typename... As>
    struct EqualityImpl<4, As...> {
        static bool impl(const EnumBase<As...>& self, const EnumBase<As...>& other) {
            switch (self._index) {
                STAMP4(0, EQUALITY_CASE)
            default:
                throw std::runtime_error("Wrong variant index");
            }
        }
    };

    template <typename... As>
    struct EqualityImpl<8, As...> {
        static bool impl(const EnumBase<As...>& self, const EnumBase<As...>& other) {
            switch (self._index) {
                STAMP8(0, EQUALITY_CASE)
            default:
                throw std::runtime_error("Wrong variant index");
            }
        }
    };

    template <typename... As>
    struct EqualityImpl<16, As...> {
        static bool impl(const EnumBase<As...>& self, const EnumBase<As...>& other) {
            switch (self._index) {
                STAMP16(0, EQUALITY_CASE)
            default:
                throw std::runtime_error("Wrong variant index");
            }
        }
    };

    template <typename... As>
    struct EqualityImpl<32, As...> {
        static bool impl(const EnumBase<As...>& self, const EnumBase<As...>& other) {
            switch (self._index) {
                STAMP32(0, EQUALITY_CASE)
            default:
                throw std::runtime_error("Wrong variant index");
            }
        }
    };

    template <typename... As>
    struct EqualityImpl<64, As...> {
        static bool impl(const EnumBase<As...>& self, const EnumBase<As...>& other) {
            switch (self._index) {
                STAMP64(0, EQUALITY_CASE)
            default:
                throw std::runtime_error("Wrong variant index");
            }
        }
    };

    template <typename... As>
    struct EqualityImpl<128, As...> {
        static bool impl(const EnumBase<As...>& self, const EnumBase<As...>& other) {
            switch (self._index) {
                STAMP128(0, EQUALITY_CASE)
            default:
                throw std::runtime_error("Wrong variant index");
            }
        }
    };

    template <typename... As>
    struct EqualityImpl<256, As...> {
        static bool impl(const EnumBase<As...>& self, const EnumBase<As...>& other) {
            switch (self._index) {
                STAMP256(0, EQUALITY_CASE)
            default:
                throw std::runtime_error("Wrong variant index");
            }
        }
    };

#undef MATCH_CASE
#undef STAMP2
#undef STAMP4
#undef STAMP8
#undef STAMP16
#undef STAMP32
#undef STAMP64
#undef STAMP128
#undef STAMP256
} // namespace detail

template <typename... As>
class Enum : public detail::EnumBase<As...> {
    using Base = detail::EnumBase<As...>;
    using Base::Base;
};

template <typename... As, typename... Fs>
auto match(const Enum<As...>& x, const Fs&... fs)
    -> EnableIf<
        all_v(Enum<As...>::template IsRelevantBranch<Fs>::value...) &&
            Enum<As...>::template IsExhaustive<Fs...>::value &&
            Enum<As...>::template IsWellFormed<Fs...>::value,
        Common<Return<Fs>...>> {
    return Enum<As...>::template MatchImpl<sizeof...(As)>::impl(Overloaded<Fs...>{fs...}, &x);
}

// EnumAt

namespace detail {
    template <size_t n, typename A>
    struct EnumAtImpl {
    };

    template <size_t n, typename... As>
    struct EnumAtImpl<n, Enum<As...>> {
        using Type = PackAt<n, As...>;
    };
} // namespace detail

template <size_t n, typename A>
using EnumAt = typename detail::EnumAtImpl<n, A>::Type;

} // namespace efp

#endif