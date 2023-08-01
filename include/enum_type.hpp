#ifndef SUM_TYPE_HPP_
#define SUM_TYPE_HPP_

#include "sfinae.hpp"

namespace efp
{

    template <typename... As>
    class Enum
    {
    public:
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

        template <typename A, typename = EnableIf_t<any_v(IsSame<A, As>::value...)>>
        Enum(const A &a)
            : index_(VariantIndex<A>::value)
        {
            *(reinterpret_cast<A *>(storage_)) = a;
        }

        template <typename A, typename = EnableIf_t<any_v(IsSame<A, As>::value...)>>
        Enum(A &&a)
            : index_(VariantIndex<A>::value)
        {
            *(reinterpret_cast<A *>(storage_)) = a;
        }

        ~Enum() {}

        uint8_t index() const
        {
            return index_;
        }

        template <typename A, typename = EnableIf_t<any_v(IsSame<A, As>::value...)>>
        A get()
        {
            if (index_ != VariantIndex<A>::value)
            {
                abort();
            }

            return *(reinterpret_cast<A *>(storage_));
        }

        template <uint8_t n>
        using NthVariant_t = PackAt_t<n, As...>;

        template <uint8_t n, typename = EnableIf_t<lt_v(n, sizeof...(As))>>
        NthVariant_t<n> get()
        {
            if (index_ != n)
            {
                abort();
            }

            return *(reinterpret_cast<NthVariant_t<n> *>(storage_));
        }

        // * Test if all of the branchs have same return type by Common_t.
        // * Check if each F has at least one matching with As.
        // * Check if all the variants are matched at least one of Fs.
        // * Compile time function uint8_t -> uint8_t, which is variant index to branch index.

        // * Test if matchs by
        // * 1. is there is_same operator (Strip ref of arg type.)
        // * 2. is invocable
        // * 3. is void arg type

        // * Need type level maybe
        // * Need type level find (Type -> bool) -> Types -> int
        // * Need type level indexing

        // Argument_t implementation 

        // template <typename... Fs>
        // auto match(const Fs &...fs)
        //     -> Common_t<Return_t<Fs>...>
        // {
        //     pack_index(matched_index<As...><Fs...>(index_), fs...)(??? a)
        // }

    private:
        Enum();

        alignas(maximum_v(alignof(As)...)) uint8_t storage_[maximum_v(sizeof(As)...)];
        // todo Maybe need to modifiy the type of index
        uint8_t index_;
    };
}

#endif