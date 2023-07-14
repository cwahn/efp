#ifndef SM_HPP_
#define SM_HPP_

#include <stdlib.h>
#include <cstdint>
#include <array>

// template <typename A, size_t N>
// class Vcb
// {
// public:
//     Vcb()
//     {
//         start = buffer.data();
//         data = buffer.data() + N;
//     }

//     void pushFront(A value)
//     {
//         data += N * (data == start);
//         --data;
//         data[0] = value;
//         data[N] = value;
//     }

//     A *data;
//     size_t len = N;

// private:
//     std::array<A, N * 2> buffer;
//     A *start;
// };

// template <typename A, size_t N>
// class DynVcb
// {
// public:
//     DynVcb()
//     {
//         start = buffer.data();
//         data = buffer.data() + N;
//         len = 0;
//     }

//     void pushFront(A value)
//     {
//         data += N * (data == start);
//         --data;
//         data[0] = value;
//         data[N] = value;

//         len++;
//     }

//     void reset_len()
//     {
//         len = 0;
//     }

//     A *data;
//     size_t len;
//     size_t max_len = N;

// private:
//     std::array<A, N * 2> buffer;
//     A *start;
// };


// template <typename A, size_t I_N, size_t O_N>
// class FixedSizeBuffer
// {
// public:
//     FixedSizeBuffer(DynVcb<A, I_N> *in_buffer)
//         : in_buffer(in_buffer)
//     {
//     }

//     void update()
//     {
//         A *in_data = in_buffer->data;
//         size_t in_length = in_buffer->len;

//         // for (int i = 0; i < in_length; i++)
//         // {
//         //     out_buffer.push_back(in_data[i]);
//         // }

//         for_each(
//             [&](A x)
//             { out_buffer.push_back(x) },
//             in_data,
//             in_length)
//     }

//     Vcb<A, O_N> out_buffer;

// private:
//     DynVcb *in_buffer;

// }

// template <typename A, size_t I_N>
// class TrandLine
// {
// public:
//     TrandLine(Vcb<A, I_N> *in_buffer)
//         : in_buffer(in_buffer)
//     {
//     }

//     void update()
//     {
//         A *in_data = in_buffer->data;

//         for
//     }

// private:
//     Vcb<A, I_N> *in_buffer;
// }

#endif
