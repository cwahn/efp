#ifndef CYCLIC_HPP_
#define CYCLIC_HPP_

#include <stdlib.h>
#include <cstdint>
#include <array>

#include "sequence.hpp"

namespace efp
{
    template <typename A, size_t n>
    class Vcb
    // : public SequenceBase<Vcb<A, n>>
    {
    public:
        using Element = A;
        using CtSize = Size<n>;
        using CtCapcity = Size<n>;
        using SizeType = size_t;

        static constexpr size_t ct_size = n;
        static constexpr size_t ct_capacity = n;

        // static_assert(ct_size >= 0, "ct_size must greater or equal than 0.");
        // static_assert(ct_capacity >= 0, "ct_capacity must greater or equal than 0.");

        Vcb()
            : buffer_{}
        {
            middle_ = buffer_.data() + ct_size;
            data_ = buffer_.data();
        }

        A &operator[](const SizeType index)
        {
            return data_[index];
        }

        const A &operator[](const SizeType index) const
        {
            return data_[index];
        }

        void push_back(A value)
        {
            data_[0] = value;
            data_[ct_size] = value;

            ++data_;
            data_ -= ct_size * (data_ == middle_);
        }

        constexpr SizeType size() const
        {
            return ct_size;
        }

        A *data()
        {
            return data_;
        }

        const A *data() const
        {
            return data_;
        }

        bool empty() const
        {
            return false;
        }

        const A *begin() const
        {
            return data_;
        }

        A *begin()
        {
            return data_;
        }

        const A *end() const
        {
            return data_ + ct_size;
        }

        A *end()
        {
            return data_ + ct_size;
        }

    private:
        Array<A, ct_size * 2> buffer_;
        A *middle_;
        A *data_;
    };

    // template <typename A, size_t n>
    // class SequenceTrait<Vcb<A, n>>
    // {
    // public:
    //     using Element = A;
    //     static constexpr size_t ct_size = n;
    //     static constexpr size_t ct_capacity = n;
    // };

    template <typename A, size_t n>
    class Vcq
    // : public SequenceBase<Vcq<A, n>>
    {
    public:
        using Element = A;
        using CtSize = Size<dyn>;
        using CtCapcity = Size<n>;
        using SizeType = size_t;

        static constexpr size_t ct_size = dyn;
        static constexpr size_t ct_capacity = n;

        // static_assert(ct_size >= -1, "ct_size must greater or equal than -1.");
        // static_assert(ct_capacity >= -1, "ct_capacity must greater or equal than -1.");

        Vcq()
            : buffer_{}
        {
            read_ = buffer_.data();
            write_ = buffer_.data();
            middle_ = buffer_.data() + ct_capacity;
        }
        ~Vcq() {}

        A &operator[](const SizeType index)
        {
            return read_[index];
        }

        const A &operator[](const SizeType index) const
        {
            return read_[index];
        }

        void push_back(const A &value)
        {
            write_[0] = value;
            write_[ct_capacity] = value;

            ++write_;
            write_ -= ct_capacity * (write_ == middle_);

            if (size_ < ct_capacity)
            {
                ++size_;
            }
            else
            {
                read_++;
                read_ -= ct_capacity * (read_ == middle_);
            }
        }
        // ! Undefined if empty

        A pop_front()
        {
            A value = *(read_);
            size_--;

            read_++;
            read_ -= ct_capacity * (read_ == middle_);

            return value;
        }

        constexpr SizeType size() const
        {
            return size_;
        }

        bool empty() const
        {
            return size_ == 0;
        }

        A *data()
        {
            return read_;
        }

        const A *data() const
        {
            return read_;
        }

        const A *begin() const
        {
            return read_;
        }

        A *begin()
        {
            return read_;
        }

        const A *end() const
        {
            return write_ > read_ ? write_ : write_ + ct_capacity;
        }

        A *end()
        {
            return write_ > read_ ? write_ : write_ + ct_capacity;
        }

    private:
        Array<A, ct_capacity * 2> buffer_ = {};
        size_t size_ = 0;
        A *read_;
        A *write_;
        A *middle_;
    };

    // template <typename A, size_t n>
    // class SequenceTrait<Vcq<A, n>>
    // {
    // public:
    //     using Element = A;
    //     static constexpr size_t ct_size = dyn;
    //     static constexpr size_t ct_capacity = n;
    // };

}

// // BufferArrVec

// template <typename A, size_t N>
// class BufferArrVec
// {
// public:
//     using Element = A;
//     using SizeType = size_t;

//     BufferArrVec()
//         : buffer_{}, size_{0}
//     {
//         middle_ = buffer_.data() + N;
//         data_ = buffer_.data();
//     }

//     A &operator[](const SizeType index)
//     {
//         return data()[index];
//     }

//     const A &operator[](const SizeType index) const
//     {
//         return data()[index];
//     }

//     void push_back(A value)
//     {
//         data_[0] = value;
//         data_[N] = value;

//         data_++;
//         data_ -= N * (data_ == middle_);
//         size_ = size_ + (size_ < N);
//     }

//     constexpr size_t size() const
//     {
//         return size_;
//     }

//     A *data()
//     {
//         return data_;
//     }

//     const A *data() const
//     {
//         return data_;
//     }

//     bool empty()
//     {
//         return false;
//     }

//     A *begin()
//     {
//         return data_;
//     }

//     A *end()
//     {
//         return data_ + N;
//     }

// private:
//     std::array<A, N * 2> buffer_;
//     A *middle_;
//     A *data_;
//     SizeType size_;
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

//         // for (size_t i = 0; i < in_length; i++)
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
