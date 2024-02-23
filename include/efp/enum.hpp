#ifndef EFP_ENUM_HPP
#define EFP_ENUM_HPP

#include "efp/meta.hpp"

namespace efp {

namespace detail {

    constexpr uint8_t power_4_ceiling(uint8_t n, uint8_t power = 4) {
        return (power >= n) ? power : power_4_ceiling(n, power * 4);
    }

    // clang-format off
#define EFP_STAMP4(n, x) x(n) x(n + 1) x(n + 2) x(n + 3)

#define EFP_STAMP16(n, x) EFP_STAMP4(n, x) EFP_STAMP4(n + 4, x) EFP_STAMP4(n + 8, x) EFP_STAMP4(n + 12, x)

#define EFP_STAMP64(n, x) EFP_STAMP16(n, x) EFP_STAMP16(n + 16, x) EFP_STAMP16(n + 32, x) EFP_STAMP16(n + 48, x)

#define EFP_STAMP256(n, x) EFP_STAMP64(n, x) EFP_STAMP64(n + 64, x) EFP_STAMP64(n + 128, x) EFP_STAMP64(n + 192, x)

    // Generic case for pattern matching
    // Bound the index to the maximum alternative index to reduce the number of template instantiations
#define EFP_ENUM_CASE(i) case i: return Case<i < alt_num ? i : alt_num - 1>::call(efp::forward<Args>(args)...); break;

    // clang-format on

    template<uint8_t stamp_n, uint8_t alt_num, template<uint8_t> class Case, typename... Args>
    struct _EnumSwitch {};

    template<uint8_t alt_num, template<uint8_t> class Case, typename... Args>
    struct _EnumSwitch<4, alt_num, Case, Args...> {
        static auto call(uint8_t index, Args&&... args)
            -> decltype(Case<0>::call(std::forward<Args>(args)...)) {
            switch (index) {
                EFP_STAMP4(0, EFP_ENUM_CASE)
                default:
                    throw std::runtime_error("Invalid alternative index");
            }
        }
    };

    template<uint8_t alt_num, template<uint8_t> class Case, typename... Args>
    struct _EnumSwitch<16, alt_num, Case, Args...> {
        static auto call(uint8_t index, Args&&... args)
            -> decltype(Case<0>::call(std::forward<Args>(args)...)) {
            switch (index) {
                EFP_STAMP16(0, EFP_ENUM_CASE)
                default:
                    throw std::runtime_error("Invalid alternative index");
            }
        }
    };

    template<uint8_t alt_num, template<uint8_t> class Case, typename... Args>
    struct _EnumSwitch<64, alt_num, Case, Args...> {
        static auto call(uint8_t index, Args&&... args)
            -> decltype(Case<0>::call(std::forward<Args>(args)...)) {
            switch (index) {
                EFP_STAMP64(0, EFP_ENUM_CASE)
                default:
                    throw std::runtime_error("Invalid alternative index");
            }
        }
    };

    // ! temp
    // template<uint8_t alt_num, template<uint8_t> class Case, typename... Args>
    // struct _EnumSwitch<256, alt_num, Case, Args...> {
    //     static auto call(uint8_t index, Args&&... args)
    //         -> decltype(Case<0>::call(std::forward<Args>(args)...)) {
    //         switch (index) {
    //             EFP_STAMP256(0, EFP_ENUM_CASE)
    //             default:
    //                 throw std::runtime_error("Invalid alternative index");
    //         }
    //     }
    // };

    template<uint8_t alt_num, template<uint8_t> class Case, typename... Args>
    using EnumSwitch = _EnumSwitch<power_4_ceiling(alt_num), alt_num, Case, Args...>;

    // todo Maybe support more than 256 alternatives

    // todo zero copy
    template<typename F>
    using IsWildCard = IsSame<Arguments<ReferenceRemoved<F>>, Tuple<>>;

    // Wrapper for wild card to be callable with any arguments
    template<typename F>
    struct WildCardWrapper: public F {
        // Explicitly define a constructor to accept a lambda or any callable
        template<typename G>
        WildCardWrapper(G&& g) : F {std::forward<G>(g)} {}

        // Overload operator() to forward arguments to the callable's operator()
        template<typename... Args>
        inline auto operator()(Args&&...) const -> decltype(std::declval<F>()()) {
            return F::operator()();
        }
    };

    // Wrap the wild card with WildCardWrapper, otherwise use the original callable
    template<typename F>
    using MatchBranch = Conditional<IsWildCard<F>::value, WildCardWrapper<F>, F>;

    // Overloaded class for pattern matching
    template<typename... Fs>
    struct Overloaded;

    template<typename F>
    struct Overloaded<F>: F {
        using F::operator();

        template<typename G>
        Overloaded(const G& g) : F {MatchBranch<G> {g}} {}
    };

    template<class F, class... Fs>
    struct Overloaded<F, Fs...>: F, Overloaded<Fs...> {
        using F::operator();
        using Overloaded<Fs...>::operator();

        template<class G, class... Gs>
        Overloaded(G&& g, Gs&&... gs)
            : F {MatchBranch<G> {forward<G>(g)}}, Overloaded<Fs...>(forward<Gs>(gs)...) {}
    };

    template<typename A, typename... As>
    class EnumBase {
    public:
        constexpr static uint8_t alt_num = sizeof...(As) + 1;

        // constexpr static uint8_t switch_size = power_4_ceiling(alt_num);

        // Default constructor initializes the first alternative with default constructor
        EnumBase() : _index {0} {
            new (reinterpret_cast<A*>(&_storage)) A {};
        }

        EnumBase(const EnumBase& other) : _index {other._index} {
            EnumSwitch<alt_num, CopyCase, EnumBase*, const EnumBase&>::call(_index, this, other);
        }

        EnumBase& operator=(const EnumBase& other) {
            if (this != &other) {
                EnumSwitch<alt_num, DestroyCase, EnumBase*>::call(_index, this);
                _index = other._index;
                EnumSwitch<alt_num, CopyCase, EnumBase*, const EnumBase&>::call(
                    _index,
                    this,
                    other
                );
            }
            return *this;
        }

        EnumBase(EnumBase&& other) : _index {other._index} {
            EnumSwitch<alt_num, MoveCase, EnumBase*, EnumBase&&>::call(
                _index,
                this,
                efp::move(other)
            );
        }

        EnumBase& operator=(EnumBase&& other) {
            if (this != &other) {
                EnumSwitch<alt_num, DestroyCase, EnumBase*>::call(_index, this);
                _index = other._index;
                EnumSwitch<alt_num, MoveCase, EnumBase*, EnumBase&&>::call(
                    _index,
                    this,
                    efp::move(other)
                );
            }
            return *this;
        }

        ~EnumBase() {
            EnumSwitch<alt_num, DestroyCase, EnumBase*>::call(_index, this);
        }

        template<typename Alt>
        EnumBase(const Alt& alt) : _index {AltIndex<Alt>::value} {
            // Compiler will complain if Alt is not in the list
            new (reinterpret_cast<Alt*>(&_storage)) Alt {alt};
        }

        template<typename Alt>
        EnumBase(Alt&& alt) : _index {AltIndex<Alt>::value} {
            // Compiler will complain if Alt is not in the list
            new (reinterpret_cast<Alt*>(&_storage)) Alt {efp::move(alt)};
        }

        bool operator==(const EnumBase& other) const {
            if (_index != other._index) {
                return false;
            }

            return EnumSwitch<alt_num, EqCase, const EnumBase*, const EnumBase&>::call(
                _index,
                this,
                other
            );
        }

        bool operator!=(const EnumBase& other) const {
            return !(*this == other);
        }

        uint8_t index() const {
            return _index;
        }

        template<typename Alt>
        const Alt& get() const {
            if (_index != AltIndex<Alt>::value) {
                throw std::runtime_error("Invalid alternative index");
            }

            return *reinterpret_cast<const Alt*>(&_storage);
        }

        template<typename Alt>
        Alt& get() {
            if (_index != AltIndex<Alt>::value) {
                throw std::runtime_error("Invalid alternative index");
            }

            return *reinterpret_cast<Alt*>(&_storage);
        }

        template<uint8_t i>
        const PackAt<i, A, As...>& get() const {
            if (_index != i) {
                throw std::runtime_error("Invalid alternative index");
            }

            return *reinterpret_cast<const PackAt<i, A, As...>*>(&_storage);
        }

        template<uint8_t i>
        PackAt<i, A, As...>& get() {
            if (_index != i) {
                throw std::runtime_error("Invalid alternative index");
            }

            return *reinterpret_cast<PackAt<i, A, As...>*>(&_storage);
        }

        template<typename Alt>
        const Alt&& move() const {
            if (_index != AltIndex<Alt>::value) {
                throw std::runtime_error("Invalid alternative index");
            }

            return efp::move(*reinterpret_cast<Alt*>(&_storage));
        }

        template<typename Alt>
        Alt&& move() {
            if (_index != AltIndex<Alt>::value) {
                throw std::runtime_error("Invalid alternative index");
            }

            return efp::move(*reinterpret_cast<Alt*>(&_storage));
        }

        template<uint8_t i>
        const PackAt<i, A, As...>&& move() const {
            if (_index != i) {
                throw std::runtime_error("Invalid alternative index");
            }

            return efp::move(*reinterpret_cast<PackAt<i, A, As...>*>(&_storage));
        }

        template<uint8_t i>
        PackAt<i, A, As...>&& move() {
            if (_index != i) {
                throw std::runtime_error("Invalid alternative index");
            }

            return efp::move(*reinterpret_cast<PackAt<i, A, As...>*>(&_storage));
        }

        // todo Improve signature to reduce copy of branches

        // match
        // Pattern matching
        template<typename F, typename... Fs>
        auto match(const F& f, const Fs&... fs) const
            -> decltype(efp::declval<Overloaded<MatchBranch<F>, MatchBranch<Fs>...>>()(
                efp::declval<A>()
            )) const {
            // ! Not working at the moment
            // static_assert(PatternCheck<F, Fs...>::value, "Pattern is not exhaustive");

            using Pattern = Overloaded<MatchBranch<F>, MatchBranch<Fs>...>;

            return EnumSwitch<
                alt_num,
                Match<Pattern>::template Case,
                const EnumBase*,
                const Pattern&>::call(_index, this, Pattern {f, fs...});
        }

    private:
        template<typename Alt>
        struct IsSameUnary {
            template<typename Alt_>
            struct Binded {
                static const bool value = IsSame<Alt, Alt_>::value;
            };
        };

        template<typename Alt>
        struct AltIndex: Find<IsSameUnary<Alt>::template Binded, A, As...> {};

        // Assume that the index is altready bounded
        template<uint8_t i>
        struct CopyCase {
            using Alt = PackAt<i, A, As...>;

            static inline void call(EnumBase* self, const EnumBase& other) {
                new (self->_storage) Alt {*reinterpret_cast<const Alt*>(&other._storage)};
            }
        };

        template<uint8_t i>
        struct MoveCase {
            using Alt = PackAt<i, A, As...>;

            static inline void call(EnumBase* self, EnumBase&& other) {
                new (self->_storage) Alt {efp::move(*reinterpret_cast<Alt*>(&other._storage))};
            }
        };

        template<uint8_t i>
        struct DestroyCase {
            using Alt = PackAt<i, A, As...>;

            static inline void call(EnumBase* self) {
                reinterpret_cast<Alt*>(&self->_storage)->~Alt();
            }
        };

        template<uint8_t i>
        struct EqCase {
            using Alt = PackAt<i, A, As...>;

            static inline bool call(const EnumBase* self, const EnumBase& other) {
                return *reinterpret_cast<const Alt*>(&self->_storage)
                    == *reinterpret_cast<const Alt*>(&other._storage);
            }
        };

        template<typename Overloaded>
        struct Match {
            template<uint8_t i>
            struct Case {
                using Alt = PackAt<i, A, As...>;

                static inline auto call(const EnumBase* self, const Overloaded& overloaded)
                    -> decltype(efp::declval<Overloaded>()(efp::declval<A>())) {
                    return overloaded(self->get<Alt>());
                }
            };
        };

        // Match branch sanity check

        // Wild card will be considered as a irrelevant branch
        template<typename F>
        using IsRelevantBranch = Any<IsInvocable<F, A>, IsInvocable<F, As>...>;

        // WildCardIffLast
        template<typename... Fs>
        struct WildCardIffLast {};

        template<typename F>
        struct WildCardIffLast<F>: IsWildCard<F> {};

        template<typename F, typename... Fs>
        struct WildCardIffLast<F, Fs...>: WildCardIffLast<Fs...> {};

        // AllButLastAreRelevant
        template<typename... Fs>
        struct AllButLastAreRelevant {};

        template<typename F>
        struct AllButLastAreRelevant<F>: True {};

        template<typename F, typename... Fs>
        struct AllButLastAreRelevant<F, Fs...>:
            Conditional<IsRelevantBranch<F>::value, AllButLastAreRelevant<Fs...>, False> {};

        // todo Make it more stricter only excpeting explicitly invocable branches
        // RemoveFirstInvocable
        template<typename Alt, typename List>
        struct _RemoveFirstInvocable {};

        template<typename Alt>
        struct _RemoveFirstInvocable<Alt, TypeList<>> {
            using Type = TypeList<>;
        };

        template<typename Alt, typename F, typename... Fs>
        struct _RemoveFirstInvocable<Alt, TypeList<F, Fs...>> {
            using Type = Conditional<
                IsInvocable<F, Alt>::value,
                TypeList<Fs...>,
                Prepend<F, typename _RemoveFirstInvocable<Alt, TypeList<Fs...>>::Type>>;
        };

        template<typename Alt, typename List>
        using RemoveFirstInvocable = typename _RemoveFirstInvocable<Alt, List>::Type;

        // template<typename Alt, typename List>
        // struct RemoveFirstInvocable {};

        // template<typename Alt>
        // struct RemoveFirstInvocable<Alt, TypeList<>>: TypeList<> {};

        // template<typename Alt, typename F, typename... Fs>
        // struct RemoveFirstInvocable<Alt, TypeList<F, Fs...>>:
        //     Conditional<
        //         IsInvocable<F, Alt>::value,
        //         TypeList<Fs...>,
        //         Prepend<F, RemoveFirstInvocable<Alt, TypeList<Fs...>>>> {};

        // Mutual Exhaustiveness
        // Check if the alternatives and the branches are mutually exhaustive
        template<typename AltList, typename BranchList>
        struct Me {};

        template<typename Alt, typename F>
        struct Me<TypeList<Alt>, TypeList<F>>: IsInvocable<F, Alt> {};

        template<typename Alt, typename... Alts, typename F, typename... Fs>
        struct Me<TypeList<Alt, Alts...>, TypeList<F, Fs...>>:
            Conditional<
                sizeof...(Alts) == sizeof...(Fs),
                Me<TypeList<Alts...>, RemoveFirstInvocable<Alt, TypeList<F, Fs...>>>,
                False> {};

        // PatternCheck
        template<typename, typename... Fs>
        struct _PatternCheck {};

        // No need to check if all the alternatives are covered
        // Only need to check if all the branches are relevant
        template<typename... Fs>
        struct _PatternCheck<True, Fs...> {
            using Type = AllButLastAreRelevant<Fs...>;
        };

        // If there is no wild card at the last branch, check if the alternatives and the branches are mutually exhaustive
        template<typename... Fs>
        struct _PatternCheck<False, Fs...> {
            using Type = Me<TypeList<A, As...>, TypeList<Fs...>>;
        };

        template<typename... Fs>
        using PatternCheck =
            typename _PatternCheck<Conditional<WildCardIffLast<Fs...>::value, True, False>, Fs...>::
                Type;

        // Private member variables
        alignas(_maximum(alignof(A), alignof(As)...)
        ) uint8_t _storage[_maximum(sizeof(A), sizeof(As)...)];

        uint8_t _index;  // Current maxinum variant is 256
    };

}  // namespace detail

template<typename... As>
class Enum: public detail::EnumBase<As...> {
public:
    using Base = detail::EnumBase<As...>;
    using Base::Base;
};

// EnumAt

namespace detail {
    template<size_t n, typename A>
    struct _EnumAt {};

    template<size_t n, typename... As>
    struct _EnumAt<n, Enum<As...>> {
        using Type = PackAt<n, As...>;
    };
}  // namespace detail

template<size_t n, typename A>
using EnumAt = typename detail::_EnumAt<n, A>::Type;

}  // namespace efp

#endif  // EFP_ENUM_HPP