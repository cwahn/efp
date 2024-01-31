#ifndef CYCLIC_HPP_
#define CYCLIC_HPP_

#include <stdlib.h>
#include <cstdint>
#include <array>

#include "sequence.hpp"

namespace efp {
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

    Vcb() {
        for (size_t i = 0; i < ct_size * 2; ++i) {
            new (buffer_ + i) A{};
        }
        middle_ = buffer_ + ct_size;
        data_ = buffer_;
    }

    Vcb(const Vcb& other) {
        for (size_t i = 0; i < ct_size * 2; ++i) {
            new (buffer_ + i) A(other.buffer_[i]);
        }
        middle_ = buffer_ + ct_size;
        data_ = buffer_ + (other.data_ - other.buffer_);
    }

    Vcb& operator=(const Vcb& other) {
        for (size_t i = 0; i < ct_size * 2; ++i) {
            (buffer_ + i)->~A();
            new (buffer_ + i) A(other.buffer_[i]);
        }
        middle_ = buffer_ + ct_size;
        data_ = buffer_ + (other.data_ - other.buffer_);
        return *this;
    }

    Vcb(Vcb&& other) {
        for (size_t i = 0; i < ct_size * 2; ++i) {
            new (buffer_ + i) A(std::move(other.buffer_[i]));
        }

        middle_ = buffer_ + ct_size;
        data_ = buffer_ + (other.data_ - other.buffer_);
    }

    Vcb operator=(Vcb&& other) {
        for (size_t i = 0; i < ct_size * 2; ++i) {
            (buffer_ + i)->~A();
            new (buffer_ + i) A(std::move(other.buffer_[i]));
        }
        middle_ = buffer_ + ct_size;
        data_ = buffer_ + (other.data_ - other.buffer_);
        return *this;
    }

    ~Vcb() {
        for (size_t i = 0; i < ct_size * 2; ++i) {
            (buffer_ + i)->~A();
        }
    }

    A& operator[](const SizeType index) { return data_[index]; }

    const A& operator[](const SizeType index) const { return data_[index]; }

    void push_back(A value) {
        data_->~A();
        (data_ + ct_size)->~A();

        new (data_) A{value};
        new (data_ + ct_size) A{value};

        ++data_;
        data_ -= ct_size * (data_ == middle_);
    }

    constexpr SizeType size() const { return ct_size; }

    A* data() { return data_; }

    const A* data() const { return data_; }

    bool empty() const { return false; }

    const A* begin() const { return data_; }

    A* begin() { return data_; }

    const A* end() const { return data_ + ct_size; }

    A* end() { return data_ + ct_size; }

private:
    RawStorage<A, 2 * ct_size> buffer_;
    A* middle_;
    A* data_;
};

// template <typename A, size_t n>
// class SequenceTrait<Vcb<A, n>>
// {
// public:
//     using Element = A;
//     static constexpr size_t ct_size = n;
//     static constexpr size_t ct_capacity = n;
// };

template <typename A, size_t n> struct ElementImpl<Vcb<A, n>> {
    using Type = A;
};

template <typename A, size_t n> struct CtSizeImpl<Vcb<A, n>> {
    using Type = Size<n>;
};

template <typename A, size_t n> struct CtCapacityImpl<Vcb<A, n>> {
    using Type = Size<n>;
};

template <typename A, size_t n> constexpr auto length(const Vcb<A, n>& as) -> Size<n> { return Size<n>{}; }

template <typename A, size_t n> constexpr auto nth(size_t i, const Vcb<A, n>& as) -> const A& { return as[i]; }

template <typename A, size_t n> constexpr auto nth(size_t i, Vcb<A, n>& as) -> A& { return as[i]; }

template <typename A, size_t n> constexpr auto data(const Vcb<A, n>& as) -> const A* { return as.data(); }

template <typename A, size_t n> constexpr auto data(Vcb<A, n>& as) -> A* { return as.data(); }

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

    // Vcq() : buffer_{} {
    //     read_ = buffer_;
    //     write_ = buffer_;
    //     middle_ = buffer_ + ct_capacity;
    // }

    Vcq() {
        // Buffers are uninitialized

        read_ = buffer_;
        write_ = buffer_;
        middle_ = buffer_ + ct_capacity;
    }

    // Vcq(const Vcq& other) {
    //     for (size_t i = 0; i <  * 2; ++i) {
    //         new (buffer_ + i) A(other.buffer_[i]);
    //     }

    //     read_ = buffer_ + (other.read_ - other.buffer_);
    //     write_ = buffer_ + (other.write_ - other.buffer_);
    //     middle_ = buffer_ + ct_capacity;
    // }

    Vcq(const Vcq& other) {
        for (size_t i = other.read_ - other.buffer_; i < other.size_; ++i) {
            const auto j = i < ct_capacity ? i : i - ct_capacity;
            new (buffer_ + j) A(other.buffer_[i]);
            new (middle_ + j) A(other.buffer_[i]);
        }

        read_ = buffer_ + (other.read_ - other.buffer_);
        write_ = buffer_ + (other.write_ - other.buffer_);
        middle_ = buffer_ + ct_capacity;
    }

    // Vcq& operator=(const Vcq& other) {
    //     buffer_ = other.buffer_;
    //     read_ = buffer_ + (other.read_ - other.buffer_);
    //     write_ = buffer_ + (other.write_ - other.buffer_);
    //     middle_ = buffer_ + ct_capacity;
    //     return *this;
    // }

    Vcq& operator=(const Vcq& other) {
        for (size_t i = read_ - buffer_; i < size_; ++i) {
            const auto j = i < ct_capacity ? i : i - ct_capacity;
            (buffer_ + j)->~A();
            (middle_ + j)->~A();
        }

        for (size_t i = other.read_ - other.buffer_; i < other.size_; ++i) {
            const auto j = i < ct_capacity ? i : i - ct_capacity;
            new (buffer_ + j) A(other.buffer_[i]);
            new (middle_ + j) A(other.buffer_[i]);
        }

        read_ = buffer_ + (other.read_ - other.buffer_);
        write_ = buffer_ + (other.write_ - other.buffer_);
        middle_ = buffer_ + ct_capacity; // ?
        return *this;
    }

    // Vcq(Vcq&& other) noexcept : buffer_{std::move(other.buffer_)} {
    //     read_ = buffer_ + (other.read_ - other.buffer_);
    //     write_ = buffer_ + (other.write_ - other.buffer_);
    //     middle_ = buffer_ + ct_capacity;
    // }

    Vcq(Vcq&& other) noexcept {
        for (size_t i = other.read_ - other.buffer_; i < other.size_; ++i) {
            const auto j = i < ct_capacity ? i : i - ct_capacity;
            new (buffer_ + j) A(std::move(other.buffer_[i]));
            new (middle_ + j) A(std::move(other.buffer_[i]));
        }

        read_ = buffer_ + (other.read_ - other.buffer_);
        write_ = buffer_ + (other.write_ - other.buffer_);
        middle_ = buffer_ + ct_capacity;
    }

    // Vcq operator=(Vcq&& other) noexcept {
    //     buffer_ = std::move(other.buffer_);
    //     read_ = buffer_ + (other.read_ - other.buffer_);
    //     write_ = buffer_ + (other.write_ - other.buffer_);
    // }

    Vcq& operator=(Vcq&& other) noexcept {
        for (size_t i = read_ - buffer_; i < size_; ++i) {
            const auto j = i < ct_capacity ? i : i - ct_capacity;
            (buffer_ + j)->~A();
            (middle_ + j)->~A();
        }

        for (size_t i = other.read_ - other.buffer_; i < other.size_; ++i) {
            const auto j = i < ct_capacity ? i : i - ct_capacity;
            new (buffer_ + j) A(std::move(other.buffer_[i]));
            new (middle_ + j) A(std::move(other.buffer_[i]));
        }

        read_ = buffer_ + (other.read_ - other.buffer_);
        write_ = buffer_ + (other.write_ - other.buffer_);
        middle_ = buffer_ + ct_capacity;
        return *this;
    }

    // ~Vcq() {
    //     for (size_t i = 0; i < size_; ++i) {
    //         read_[i].~A();
    //         (read_ + ct_capacity)[i].~A();
    //     }
    // }

    ~Vcq() {
        for (size_t i = read_ - buffer_; i < size_; ++i) {
            const auto j = i < ct_capacity ? i : i - ct_capacity;
            (buffer_ + j)->~A();
            (middle_ + j)->~A();
        }
    }

    A& operator[](const SizeType index) { return read_[index]; }

    const A& operator[](const SizeType index) const { return read_[index]; }

    // void push_back(const A& value) {
    //     // write_[0] = value;
    //     // write_[ct_capacity] = value;

    //     write_->~A();
    //     (write_ + ct_capacity)->~A();

    //     new (write_) A{value};
    //     new (write_ + ct_capacity) A{value};

    //     ++write_;
    //     write_ -= ct_capacity * (write_ == middle_);

    //     if (size_ < ct_capacity) {
    //         ++size_;
    //     } else {
    //         read_++;
    //         read_ -= ct_capacity * (read_ == middle_);
    //     }
    // }

    void push_back(const A& value) {
        if (size_ == ct_capacity) { // Has to destroy the oldest element if the buffer is full
            write_->~A();
            (write_ + ct_capacity)->~A();
        }

        new (write_) A{value};
        new (write_ + ct_capacity) A{value};

        ++write_;
        write_ -= ct_capacity * (write_ == middle_);

        if (size_ < ct_capacity) {
            ++size_;
        } else {
            read_++;
            read_ -= ct_capacity * (read_ == middle_);
        }
    }
    // ! Undefined if empty

    // A pop_front() {
    //     A value = std::move(*read_);
    //     read_->~A();
    //     (read_ + ct_capacity)->~A();
    //     size_--;

    //     read_++;
    //     read_ -= ct_capacity * (read_ == middle_);

    //     return value;
    // }

    A pop_front() {
        A value{std::move(*read_)};

        read_->~A();
        (read_ + ct_capacity)->~A();
        size_--;

        read_++;
        read_ -= ct_capacity * (read_ == middle_);

        return value;
    }

    constexpr SizeType size() const { return size_; }

    bool empty() const { return size_ == 0; }

    A* data() { return read_; }

    const A* data() const { return read_; }

    const A* begin() const { return read_; }

    A* begin() { return read_; }

    const A* end() const { return write_ > read_ ? write_ : write_ + ct_capacity; }

    A* end() { return write_ > read_ ? write_ : write_ + ct_capacity; }

private:
    // Array<A, ct_capacity * 2> buffer_ = {};
    // A buffer_[ct_capacity * 2];
    RawStorage<A, 2 * ct_capacity> buffer_;

    size_t size_ = 0;
    A* read_;
    A* write_;
    A* middle_;
};

// template <typename A, size_t n>
// class SequenceTrait<Vcq<A, n>>
// {
// public:
//     using Element = A;
//     static constexpr size_t ct_size = dyn;
//     static constexpr size_t ct_capacity = n;
// };

template <typename A, size_t n> struct ElementImpl<Vcq<A, n>> {
    using Type = A;
};

template <typename A, size_t n> struct CtSizeImpl<Vcq<A, n>> {
    using Type = Size<dyn>;
};

template <typename A, size_t n> struct CtCapacityImpl<Vcq<A, n>> {
    using Type = Size<n>;
};

template <typename A, size_t n> constexpr auto length(const Vcq<A, n>& as) -> size_t { return as.size(); }

template <typename A, size_t n> constexpr auto nth(size_t i, const Vcq<A, n>& as) -> const A& { return as[i]; }

template <typename A, size_t n> constexpr auto nth(size_t i, Vcq<A, n>& as) -> A& { return as[i]; }

template <typename A, size_t n> constexpr auto data(const Vcq<A, n>& as) -> const A* { return as.data(); }

template <typename A, size_t n> constexpr auto data(Vcq<A, n>& as) -> A* { return as.data(); }

} // namespace efp

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
