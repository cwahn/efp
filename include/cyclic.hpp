#ifndef CYCLIC_HPP_
#define CYCLIC_HPP_

#include <stdlib.h>
#include <cstdint>
#include <array>

#include "sequence.hpp"

namespace efp
{
    template <typename A, size_t N>
    class Vcb
    {
    public:
        Vcb()
        {
            p_middle_ = buffer_.data() + N;
            p_data_ = buffer_.data();
        }

        void push_back(A value)
        {
            p_data_[0] = value;
            p_data_[N] = value;

            ++p_data_;
            p_data_ -= N * (p_data_ == p_middle_);
        }

        A *data()
        {
            return p_data_;
        }

        size_t size()
        {
            return N;
        }

        bool is_empty()
        {
            return false;
        }

        A *begin()
        {
            return p_data_;
        }

        A *end()
        {
            return p_data_ + N;
        }

    private:
        std::array<A, N * 2> buffer_;
        A *p_middle_;
        A *p_data_;
    };

    template <typename A, size_t N>
    class Vcq
    {
    public:
        Vcq()
            : len_(0)
        {
            p_read_ = buffer_.data();
            p_write_ = buffer_.data();
            p_middle_ = buffer_.data() + N;
        }
        ~Vcq() {}

        void push_back(const A &value)
        {
            p_write_[0] = value;
            p_write_[N] = value;

            ++p_write_;
            p_write_ -= N * (p_write_ == p_middle_);

            if (len_ < N)
            {
                ++len_;
            }
            else
            {
                p_read_++;
                p_read_ -= N * (p_read_ == p_middle_);
            }
        }
        // ! Undefined if empty

        A pop_front()
        {
            A value = *(p_read_);
            len_--;

            p_read_++;
            p_read_ -= N * (p_read_ == p_middle_);

            return value;
        }

        size_t size()
        {
            return len_;
        }

        bool is_empty()
        {
            return len_ == 0;
        }
        A *begin()
        {
            return p_read_;
        }
        A *end()
        {
            return p_write_ > p_read_ ? p_write_ : p_write_ + N;
        }

    private:
        Array<A, N * 2> buffer_;
        A *p_read_;
        A *p_write_;
        A *p_middle_;
        size_t len_;
    };

}

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
