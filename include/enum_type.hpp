#ifndef SUM_TYPE_HPP_
#define SUM_TYPE_HPP_

#include "sfinae.hpp"

namespace efp
{
    template <typename... Fs>
    struct Overloaded;

    template <typename F>
    struct Overloaded<F> : F
    {
        using F::operator();

        template <class G>
        Overloaded(G &&g)
            : F(std::forward<G>(g))
        {
        }
    };

    template <class F, class... Fs>
    struct Overloaded<F, Fs...> : F, Overloaded<Fs...>
    {
        using F::operator();
        using Overloaded<Fs...>::operator();

        template <class G, class... Gs>
        Overloaded(G &&g, Gs &&...gs)
            : F(std::forward<G>(g)),
              Overloaded<Fs...>(std::forward<Gs>(gs)...)
        {
        }
    };

    constexpr uint8_t power_2_ceiling(uint8_t n, uint8_t power = 2)
    {
        return (power >= n) ? power : power_2_ceiling(n, power * 2);
    }

    template <int n, typename... As>
    struct Match
    {
    };

    template <typename... As>
    class Enum
    {
    public:
        template <int n, typename... Bs>
        friend struct Match;

        template <typename A>
        struct IsSameUnary
        {
            template <typename B>
            struct Binded
            {
                static const bool value = std::is_same<A, B>::value;
            };
        };

        template <typename A>
        struct VariantIndex
            : Find<IsSameUnary<A>::template Binded, As...>
        {
        };

        template <typename A, typename = EnableIf<any_v(IsSame<A, As>::value...)>>
        Enum(const A &a)
            : index_(VariantIndex<A>::value)
        {
            *(reinterpret_cast<A *>(storage_)) = a;
        }

        template <typename A, typename = EnableIf<any_v(IsSame<A, As>::value...)>>
        Enum(A &&a)
            : index_(VariantIndex<A>::value)
        {
            *(reinterpret_cast<A *>(storage_)) = a;
        }

        ~Enum() {}

        // bool operator==(const Enum &other) const
        // {
        //     if (index_ == other.index_)
        //     {
        //         switch index_{
        //             // todo macro
        //         }
        //     }
        //     return false;
        // }

        uint8_t index() const
        {
            return index_;
        }

        template <typename A>
        auto get() const
            -> EnableIf<any_v(IsSame<A, As>::value...), A>
        {
            if (index_ != VariantIndex<A>::value)
            {
                abort();
            }

            return *(reinterpret_cast<const A *>(storage_));
        }

        template <uint8_t n>
        auto get()
            -> EnableIf<lt_v(n, sizeof...(As)), PackAt<n, As...>>
        {
            if (index_ != n)
            {
                abort();
            }

            return *(reinterpret_cast<PackAt<n, As...> *>(storage_));
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

        // Arguments implementation will auto matically remove the const qualifier if there is.

#define CASE(i)                                                                                \
    case i:                                                                                    \
    {                                                                                          \
        return overloaded(*(reinterpret_cast<const PackAt<i, As...> *>(outer->storage_))); \
        break;                                                                                 \
    }

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

        template <typename F>
        struct IsRelevantBranch
        {
            static constexpr bool value =
                any_v(IsInvocable<F, As>::value...) || IsSame<std::tuple<>, Arguments<F>>::value;
        };

        template <typename F>
        struct IsWildCard : IsSame<std::tuple<>, Arguments<F>>
        {
        };

        template <typename A, typename... Fs>
        struct IsVariantCovered
        {
            static constexpr bool value =
                any_v(IsWildCard<Fs>::value...) || any_v(IsInvocable<Fs, A>::value...);
        };

        template <typename... Fs>
        struct IsExhaustive : False
        {
            static constexpr bool value = all_v(IsVariantCovered<As, Fs...>::value...);
        };

        template <typename... Fs>
        struct IsWellFormed
        {
            static constexpr bool value = true;
            // todo count and get last
            // (!any_v(IsWildCard<Init>::value...) && IsWildCard<Last>::value) ||
            // !any_v(IsWildCard<Fs...>::value...);
        };

        template <typename... Fs>
        auto match(const Fs &...fs)
            -> EnableIf<
                all_v(IsRelevantBranch<Fs>::value...) &&
                    IsExhaustive<Fs...>::value &&
                    IsWellFormed<Fs...>::value,
                Common<Return<Fs>...>>
        {
            return Match<power_2_ceiling(sizeof...(As)), As...>::impl(Overloaded<Fs...>{fs...}, this);
        }

    private:
        Enum()
        {
        }

        alignas(maximum_v(alignof(As)...)) uint8_t storage_[maximum_v(sizeof(As)...)];
        uint8_t index_;
    };

    template <typename... As>
    struct Match<2, As...>
    {
        template <typename... Fs>
        static auto impl(
            const Overloaded<Fs...> &overloaded,
            const Enum<As...> *outer)
            -> Common<Return<Fs>...>
        {
            switch (outer->index_)
            {
                STAMP2(0, CASE)

            default:
                // assert(0);
                abort();
            }
        }
    };

    template <typename... As>
    struct Match<4, As...>
    {
        template <typename... Fs>
        static auto impl(
            const Overloaded<Fs...> &overloaded,
            const Enum<As...> *outer)
            -> Common<Return<Fs>...>
        {
            switch (outer->index_)
            {
                STAMP4(0, CASE)

            default:
                // assert(0);
                abort();
            }
        }
    };

    template <typename... As>
    struct Match<8, As...>
    {
        template <typename... Fs>
        static auto impl(
            const Overloaded<Fs...> &overloaded,
            const Enum<As...> *outer)
            -> Common<Return<Fs>...>
        {
            switch (outer->index_)
            {
                STAMP8(0, CASE)

            default:
                // assert(0);
                abort();
            }
        }
    };

    template <typename... As>
    struct Match<16, As...>
    {
        template <typename... Fs>
        static auto impl(
            const Overloaded<Fs...> &overloaded,
            const Enum<As...> *outer)
            -> Common<Return<Fs>...>
        {
            switch (outer->index_)
            {
                STAMP16(0, CASE)

            default:
                // assert(0);
                abort();
            }
        }
    };

    template <typename... As>
    struct Match<32, As...>
    {
        template <typename... Fs>
        static auto impl(
            const Overloaded<Fs...> &overloaded,
            const Enum<As...> *outer)
            -> Common<Return<Fs>...>
        {
            switch (outer->index_)
            {
                STAMP32(0, CASE)

            default:
                // assert(0);
                abort();
            }
        }
    };

    template <typename... As>
    struct Match<64, As...>
    {
        template <typename... Fs>
        static auto impl(
            const Overloaded<Fs...> &overloaded,
            const Enum<As...> *outer)
            -> Common<Return<Fs>...>
        {
            switch (outer->index_)
            {
                STAMP64(0, CASE)

            default:
                // assert(0);
                abort();
            }
        }
    };

    template <typename... As>
    struct Match<128, As...>
    {
        template <typename... Fs>
        static auto impl(
            const Overloaded<Fs...> &overloaded,
            const Enum<As...> *outer)
            -> Common<Return<Fs>...>
        {
            switch (outer->index_)
            {
                STAMP128(0, CASE)

            default:
                // assert(0);
                abort();
            }
        }
    };

    template <typename... As>
    struct Match<256, As...>
    {
        template <typename... Fs>
        static auto impl(
            const Overloaded<Fs...> &overloaded,
            const Enum<As...> *outer)
            -> Common<Return<Fs>...>
        {
            switch (outer->index_)
            {
                STAMP256(0, CASE)

            default:
                // assert(0);
                abort();
            }
        }
    };

    template <typename... As, typename... Fs>
    auto match(const Enum<As...> &x, const Fs &...fs)
        -> EnableIf<
            all_v(Enum<As...>::template IsRelevantBranch<Fs>::value...) &&
                Enum<As...>::template IsExhaustive<Fs...>::value &&
                Enum<As...>::template IsWellFormed<Fs...>::value,
            Common<Return<Fs>...>>
    {
        return Enum<As...>::template Match<sizeof...(As)>::impl(Overloaded<Fs...>{fs...}, &x);
    }

}

#endif