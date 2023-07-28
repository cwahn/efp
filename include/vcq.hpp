#ifndef VCQ_HPP_
#define VCQ_HPP_

#include <stdlib.h>
#include <cstdint>
#include <array>

#include "zero_copy.hpp"

namespace efp
{
    template <typename A, size_t N>
    class Vcq
    {
    public:
        Vcq()
            : len(0)
        {
            p_read = buffer.data();
            p_write = buffer.data();
            p_middle = buffer.data() + N;
        }
        ~Vcq(){}

        void push_back(const A &value)
        {
            p_write[0] = value;
            p_write[N] = value;

            p_write++;
            p_write -= N * (p_write == p_middle);

            if (len < N)
            {
                len++;
            }
            else
            {
                p_read++;
                p_read -= N * (p_read == p_middle);
            }
        }
        // ! Undefined if empty

        A pop_front()
        {
            A value = *(p_read);
            len--;

            p_read++;
            p_read -= N * (p_read == p_middle);

            return value;
        }

        size_t size()
        {
            return len;
        }

        bool is_empty()
        {
            return len == 0;
        }
        A *begin()
        {
            return p_read;
        }
        A *end()
        {
            return p_write > p_read ? p_write : p_write + N;
        }

    private:
        Array<A, N * 2> buffer;
        A *p_read;
        A *p_write;
        A *p_middle;
        size_t len;
    };

}

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
