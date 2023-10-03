// #ifndef TUPLE_HPP_
// #define TUPLE_HPP_

// template <int index, typename A>
// class TupleLeaf
// {
// public:
//     using Element = A;
//     static constexpr int idx = index;

//     TupleLeaf() {}
//     TupleLeaf(const A &value) : value_{value} {}
//     TupleLeaf(A &&value) : value_{value} {}

//     ~TupleLeaf() {}

//     A get()
//     {
//         return value_;
//     }

// private:
//     A value_;
// };

// template <int... ns>
// struct IndexSequence
// {
// };

// template <int n, int... ns>
// struct MakeIndexSequenceImpl
//     : MakeIndexSequenceImpl<n - 1, n - 1, ns...>
// {
// };

// template <int... ns>
// struct MakeIndexSequenceImpl<0, ns...>
// {
//     using Type = IndexSequence<ns...>;
// };

// template <int n>
// using MakeIndexSequence = typename MakeIndexSequenceImpl<n>::Type;

// template <typename... Ts>
// using IndexSequenceFor = MakeIndexSequence<sizeof...(Ts)>;

// template <typename... As, typename IndexSequence>
// class TupleImpl
// {
// };

// template <typename... As, int... idxs>
// class TupleImpl<As..., IndexSequence<idxs...>>
//     : public TupleLeaf<As, idxs>...
// {
// public:
//     TupleImpl(const As &...as)
//         : TupleLeaf<As, idxs>{as}...
//     {
//     }
// };

// template <typename... As>
// class Tuple
//     : public TupleImpl<As..., IndexSequenceFor<As...>>
// {
// public:
//     Tuple(const As &...as)
//         : TupleImpl<As..., IndexSequenceFor<As...>>{as...}
//     {
//     }

//     template <int idx>
//     auto get()
//     {
//         return
//     }

// private:
// }

// #endif