// #ifndef SUM_TYPE_HPP_
// #define SUM_TYPE_HPP_

// #include "sfinae.hpp"

// template <typename... As>
// class Enum
// {
// public:
//     ~Enum();

//     template <typename A, typename = EnableIf_t<any_v(IsSame<A, As>::value...)>>
//     Enum(const A& a)
//     : tag_(IndexOf<>)
//     {

//     }

//     // * Test if all of the branchs have same return type by Common_t.
//     // * Check if each F has at least one matching with As.
//     // * Check if all the variants are matched at least one of Fs.
//     // * Compile time function uint8_t -> uint8_t, which is variant index to branch index.

//     // * Test if matchs by
//     // * 1. is there is_same operator (Strip ref of arg type.)
//     // * 2. is invocable
//     // * 3. is void arg type

//     // * Need type level maybe
//     // * Need type level find (Type -> bool) -> Types -> int
//     // * Need type level indexing

//     // template <typename... Fs>
//     // auto match(const Fs &...fs)
//     //     -> Common_t<Return_t<Fs>...>
//     // {
//     //     pack_index(matched_index<As...><Fs...>(tag_), fs...)(??? a)
//     // }

// private:
//     Enum();

//     uint8_t tag_;
//     uint8_thar data_[maximum_v(sizeof(As)...)];
// };

// #endif