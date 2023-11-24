#ifndef SEQUENCE_HPP_
#define SEQUENCE_HPP_

// Curiously Recurring Template Pattern Sequence
// Check validity on data store

// ct_capacity is compile time bound of length. It does not mean safety of access.
// However, actual capacity does means the length of memory safe to access.

// todo STL only
#include <iostream>
#include <cstring>

#include "sfinae.hpp"

// todo Move and copy assigment operator sort out

namespace efp
{
    constexpr int dyn = -1;

    template <typename Derived>
    class SequenceTrait;

    template <typename Derived>
    class SequenceBase
    {
    public:
        using Trait = SequenceTrait<Derived>;

        using Element = typename Trait::Element;
        static constexpr int ct_len = Trait::ct_len;
        static constexpr int ct_cap = Trait::ct_cap;

        const Element &operator[](int index) const
        {
            return derived()[index];
        }

        // ! Mut
        Element &operator[](int index)
        {
            return derived()[index];
        }

        bool operator==(const SequenceBase &other) const
        {
            return derived() == other.derived();
        }

        int size() const
        {
            return derived().size();
        }

        const Element *data() const
        {
            return derived().data();
        }

        Element *data()
        {
            return derived().data();
        }

        const Element *begin() const
        {
            return derived().begin();
        }

        Element *begin()
        {
            return derived().begin();
        }

        const Element *end() const
        {
            return derived().end();
        }

        Element *end()
        {
            return derived().end();
        }

    private:
        Derived &derived()
        {
            return *static_cast<Derived *>(this);
        }
        const Derived &derived() const
        {
            return *static_cast<const Derived *>(this);
        }
    };

    template <typename Derived>
    using Seq = SequenceBase<Derived>;

    template <typename A, int ct_length, int ct_capacity>
    class Sequence
    {
    };

    template <typename A, int ct_length>
    class Sequence<A, ct_length, ct_length>
        : public SequenceBase<Sequence<A, ct_length, ct_length>>
    {
    public:
        using Element = A;
        static constexpr int ct_len = ct_length;
        static constexpr int ct_cap = ct_length;

        // static_assert(ct_len >= -1, "ct_length must greater or equal than -1.");
        // static_assert(ct_cap >= -1, "ct_capacity must greater or equal than -1.");

        Sequence()
        {
            for (size_t i = 0; i < ct_len; ++i)
            {
                new (&data_[i]) A();
            }
        }

        Sequence(const Sequence &other)
        {
            for (size_t i = 0; i < ct_len; ++i)
            {
                new (&data_[i]) A(other.data_[i]);
            }
        }

        Sequence(Sequence &&other) noexcept
        {
            for (size_t i = 0; i < ct_len; ++i)
            {
                new (&data_[i]) A(efp::move(other.data_[i])); // Move-construct each element
            }
        }

        template <typename... Arg>
        Sequence(const Arg &...args)
            : data_{args...}
        {
        }

        Sequence &operator=(const Sequence &other)
        {
            if (this != &other)
            {
                for (size_t i = 0; i < ct_len; ++i)
                {
                    data_[i] = other.data_[i]; // Use assignment for each element
                }
            }
            return *this;
        }

        Sequence assign_impl(const Sequence &other)
        {
            if (this != &other)
            {
                for (size_t i = 0; i < ct_len; ++i)
                {
                    data_[i] = other.data_[i]; // Use assignment for each element
                }
            }
            return *this;
        }

        A &operator[](int index)
        {
            return data_[index];
        }

        const A &operator[](int index) const
        {
            return data_[index];
        }

        bool operator==(const Sequence &other) const
        {
            for (size_t i = 0; i < ct_len; ++i)
            {
                if (data_[i] != other.data_[i])
                {
                    return false;
                }
            }

            return true;
        }

        int size() const
        {
            return ct_len;
        }

        int capacity() const
        {
            return ct_cap;
        }

        void resize(int length)
        {
            if (length != ct_len)
            {
                abort();
            }
        }

        void reserve(int capacity)
        {
            if (capacity > ct_cap)
            {
                abort();
            }
        }

        const A *data() const
        {
            return data_;
        }

        A *data()
        {
            return data_;
        }

        A *begin()
        {
            return data_;
        }

        const A *begin() const
        {
            return data_;
        }

        A *end()
        {
            return data_ + ct_len;
        }

        const A *end() const
        {
            return data_ + ct_len;
        }

        bool empty() const
        {
            return ct_len == 0;
        }

    private:
        A data_[ct_cap];
    };

    template <typename A, int ct_length>
    using Array = EnableIf<ct_length != dyn, Sequence<A, ct_length, ct_length>>;

    template <typename A, int ct_capacity>
    class Sequence<A, dyn, ct_capacity>
        : public SequenceBase<Sequence<A, dyn, ct_capacity>>
    {
    public:
        using Element = A;
        static constexpr int ct_len = dyn;
        static constexpr int ct_cap = ct_capacity;

        // static_assert(ct_len >= -1, "ct_length must greater or equal than -1.");
        // static_assert(ct_cap >= -1, "ct_capacity must greater or equal than -1.");

        Sequence() : size_{0} {}

        Sequence(const Sequence &other) : size_{other.size_}
        {
            for (size_t i = 0; i < size_; ++i)
            {
                new (&data_[i]) A(other.data_[i]);
            }
        }

        Sequence(Sequence &&other) : size_{other.size_}
        {
            other.size_ = 0;
            for (size_t i = 0; i < size_; ++i)
            {
                data_[i] = efp::move(other.data_[i]);
            }
        }

        template <typename... Arg>
        Sequence(const Arg &...args)
            : data_{args...},
              size_(sizeof...(args))
        {
        }

        Sequence &operator=(const Sequence &other)
        {
            if (this != &other)
            {
                resize(other.size());
                for (size_t i = 0; i < size_; ++i)
                {
                    data_[i] = other.data_[i];
                }
            }
            return *this;
        }

        Sequence assign_impl(const Sequence &other)
        {
            if (this != &other)
            {
                resize(other.size());
                for (size_t i = 0; i < size_; ++i)
                {
                    data_[i] = other.data_[i];
                }
            }
            return *this;
        }

        ~Sequence()
        {
            for (size_t i = 0; i < size_; ++i)
            {
                data_[i].~A();
            }
        }

        A &operator[](int index)
        {
            return data_[index];
        }

        const A &operator[](int index) const
        {
            return data_[index];
        }

        bool operator==(const Sequence &other) const
        {
            if (size_ != other.size_)
            {
                return false;
            }

            for (size_t i = 0; i < size_; ++i)
            {
                if (data_[i] != other.data_[i])
                {
                    return false;
                }
            }

            return true;
        }

        int size() const
        {
            return size_;
        }

        int capacity() const
        {
            return ct_cap;
        }

        void resize(int length)
        {
            if (length > ct_cap || length < 0)
            {
                abort();
            }

            size_ = length;
        }

        void reserve(int capacity)
        {
            if (capacity > ct_cap)
            {
                abort();
            }
        }

        void push_back(const A &value)
        {
            if (size_ >= ct_cap)
            {
                abort();
            }
            else
            {
                new (&data_[size_]) A(value);
                ++size_;
            }
        }

        void push_back(A &&value)
        {
            if (size_ >= ct_cap)
            {
                abort();
            }
            else
            {
                new (&data_[size_]) A(efp::move(value));
                ++size_;
            }
        }

        void insert(int index, const A &value)
        {
            if (index < 0 || index > size_ || size_ == ct_capacity)
            {
                abort();
            }

            for (size_t i = size_; i > index; --i)
            {
                new (&data_[i]) A(efp::move(data_[i - 1]));
                data_[i - 1].~A();
            }

            new (&data_[index]) A(value);

            ++size_;
        }

        void pop_back()
        {
            if (size_ == 0)
            {
                abort();
            }

            data_[size_ - 1].~A();
            --size_;
        }

        void clear()
        {
            for (size_t i = 0; i < size_; ++i)
            {
                data_[i].~A();
            }
            size_ = 0;
        }

        void erase(int index)
        {
            if (index < 0 || index >= size_)
            {
                abort();
            }
            data_[index].~A();
            for (size_t i = index; i < size_ - 1; ++i)
            {
                new (&data_[i]) A(efp::move(data_[i + 1]));
                data_[i + 1].~A();
            }
            --size_;
        }

        const A *data() const
        {
            return data_;
        }

        A *data()
        {
            return data_;
        }

        A *begin()
        {
            return data_;
        }

        const A *begin() const
        {
            return data_;
        }

        A *end()
        {
            return data_ + size_;
        }

        const A *end() const
        {
            return data_ + size_;
        }

        bool empty() const
        {
            return size_ == 0;
        }

    private:
        A data_[ct_cap];
        int size_;
    };

    template <typename A, int ct_capacity>
    using ArrVec = EnableIf<ct_capacity != dyn, Sequence<A, dyn, ct_capacity>>;

    template <typename A>
    class Sequence<A, dyn, dyn>
        : public SequenceBase<Sequence<A, dyn, dyn>>
    {
    public:
        using Element = A;
        static constexpr int ct_len = dyn;
        static constexpr int ct_cap = dyn;

        static_assert(ct_len >= -1, "ct_length must greater or equal than -1.");
        static_assert(ct_cap >= -1, "ct_capacity must greater or equal than -1.");

        Sequence() : data_{nullptr}, size_{0}, capacity_{0} {}

        Sequence(const Sequence &other)
            : data_{nullptr}, size_{0}, capacity_{0}
        {
            if (other.data_)
            {
                size_ = other.size_;
                capacity_ = other.capacity_;
                data_ = new A[capacity_];

                // memcpy(data_, other.data_, sizeof(A) * size_);
                for (size_t i = 0; i < other.size_; ++i)
                {
                    new (&data_[i]) A(other.data_[i]);
                }
            }
        }

        Sequence(Sequence &&other)
            : data_{other.data_}, size_{other.size_}, capacity_{other.capacity_}
        {
            other.data_ = nullptr;
        }

        template <typename... Args>
        Sequence(const Args &...args)
            : data_{new A[sizeof...(args)]},
              capacity_(sizeof...(args)),
              size_(sizeof...(args))
        {
            int i = 0;
            for (auto arg : std::initializer_list<Common<Args...>>{args...})
                data_[i++] = arg;
        }

        ~Sequence()
        {
            if (data_)
            {
                delete[] data_;
                data_ = nullptr;
            }
        }

        // todo copy_and_swap
        Sequence &operator=(const Sequence &other) noexcept
        {
            if (this != &other)
            {
                A *newData = nullptr;
                if (other.size_ > 0)
                {
                    newData = new A[other.capacity_];

                    for (size_t i = 0; i < other.size_; ++i)
                    {
                        newData[i] = other.data_[i];
                    }
                }

                data_ = newData;
                size_ = other.size_;
                capacity_ = other.capacity_;
            }
            return *this;
        }

        Sequence &operator=(Sequence &&other) noexcept
        {
            if (this != &other)
            {
                delete[] data_;

                data_ = other.data_;
                size_ = other.size_;
                capacity_ = other.capacity_;

                other.data_ = nullptr;
                other.size_ = 0;
                other.capacity_ = 0;
            }
            return *this;
        }

        Sequence assign_impl(const Sequence &other)
        {
            if (this != &other)
            {
                A *newData = nullptr;
                if (other.size_ > 0)
                {
                    newData = new A[other.capacity_];

                    for (size_t i = 0; i < other.size_; ++i)
                    {
                        newData[i] = other.data_[i];
                    }
                }

                data_ = newData;
                size_ = other.size_;
                capacity_ = other.capacity_;
            }
            return *this;
        }

        A &operator[](int index)
        {
            return data_[index];
        }

        const A &operator[](int index) const
        {
            return data_[index];
        }

        bool operator==(const Sequence &other) const
        {
            if (size_ != other.size_)
            {
                return false;
            }

            for (size_t i = 0; i < size_; ++i)
            {
                if (data_[i] != other.data_[i])
                {
                    return false;
                }
            }

            return true;
        }

        int size() const
        {
            return size_;
        }

        int capacity() const
        {
            return capacity_;
        }

        void resize(int length)
        {
            if (length < 0)
            {
                abort();
            }

            if (length > capacity_)
            {
                reserve(length);
            }

            size_ = length;
        }

        void reserve(int new_capacity)
        {
            if (new_capacity > capacity_)
            {
                A *new_data = new A[new_capacity];

                for (size_t i = 0; i < size_; ++i)
                {
                    new (&new_data[i]) A(efp::move(data_[i]));
                    data_[i].~A();
                }

                delete[] data_;

                data_ = new_data;
                capacity_ = new_capacity;
            }
        }

        void push_back(const A &value)
        {
            if (size_ >= capacity_)
            {
                reserve(capacity_ == 0 ? 1 : 2 * capacity_);
            }

            new (&data_[size_]) A{value};
            ++size_;
        }

        void push_back(A &&value)
        {
            if (size_ >= capacity_)
            {
                reserve(capacity_ == 0 ? 1 : 2 * capacity_);
            }

            new (&data_[size_]) A(efp::move(value));
            ++size_;
        }

        void insert(int index, const A &value)
        {
            if (index < 0 || index > size_)
            {
                abort();
            }
            if (size_ >= capacity_)
            {
                reserve(capacity_ == 0 ? 1 : 2 * capacity_);
            }
            for (size_t i = size_; i > index; --i)
            {
                data_[i] = efp::move(data_[i - 1]);
            }
            new (&data_[index]) A(value);
            ++size_;
        }

        void erase(int index)
        {
            if (index < 0 || index >= size_)
            {
                abort();
            }

            data_[index].~A();
            for (size_t i = index; i < size_ - 1; ++i)
            {
                new (&data_[i]) A(efp::move(data_[i + 1]));
                data_[i + 1].~A();
            }

            --size_;
        }

        void clear()
        {
            for (size_t i = 0; i < size_; ++i)
            {
                data_[i].~A();
            }
            size_ = 0;
        }

        void pop_back()
        {
            if (size_ == 0)
            {
                abort();
            }
            data_[size_ - 1].~A();
            --size_;
        }

        const A *data() const
        {
            return data_;
        }

        A *data()
        {
            return data_;
        }

        A *begin()
        {
            return data_;
        }

        const A *begin() const
        {
            return data_;
        }

        A *end()
        {
            return data_ + size_;
        }

        const A *end() const
        {
            return data_ + size_;
        }

        bool empty() const
        {
            return size_ == 0;
        }

    private:
        A *data_;
        int size_;
        int capacity_;
    };

    template <typename A>
    using Vector = Sequence<A, dyn, dyn>;

    template <typename A, int ct_length, int ct_capacity>
    class SequenceTrait<Sequence<A, ct_length, ct_capacity>>
    {
    public:
        using Element = A;
        static constexpr int ct_len = ct_length;
        static constexpr int ct_cap = ct_capacity;
    };

    // Should have all these three template parameter not to break static link
    template <typename A, int ct_length, int ct_capacity>
    class SequenceView
    {
    };

    template <typename A, int ct_length>
    class SequenceView<A, ct_length, ct_length>
        : public SequenceBase<SequenceView<A, ct_length, ct_length>>
    {
    public:
        using Element = A;
        static constexpr int ct_len = ct_length;
        static constexpr int ct_cap = ct_length;

        // static_assert(ct_len >= -1, "ct_length must greater or equal than -1.");

        // ! length will not be used
        explicit SequenceView(A *data, Size<ct_len> length = Size<ct_len>{})
            : data_(data)
        {
            // Ensure that data is not nullptr for a non-empty view.
            if (ct_len > 0 && data_ == nullptr)
            {
                abort();
            }
        }

        // ! No need of constructor taking only the pointer
        // explicit SequenceView(A *data)
        //     : data_(data)
        // {
        //     // Ensure that data is not nullptr for a non-empty view.
        //     if (ct_len > 0 && data_ == nullptr)
        //     {
        //         abort();
        //     }
        // }

        // Rule of five: if you define one of the destructor, copy constructor,
        // copy assignment operator, move constructor, or move assignment operator,
        // you should define all of them. Here, the default behavior is size_tended.

        // SequenceView(const SequenceView &) = default;
        // SequenceView(SequenceView &&) noexcept = default;
        // SequenceView &operator=(const SequenceView &) = default;
        // SequenceView &operator=(SequenceView &&) noexcept = default;
        // ~SequenceView() = default;

        A &operator[](int index)
        {
            return data_[index];
        }

        const A &operator[](int index) const
        {
            return data_[index];
        }

        bool operator==(const SequenceView &other) const
        {
            return data_ == other.data_;
        }

        int size() const
        {
            return ct_len;
        }

        int capacity() const
        {
            return ct_cap;
        }

        void resize(int length)
        {
            if (length > ct_cap || length < 0)
            {
                abort();
            }
        }

        void reserve(int capacity)
        {
            if (capacity > ct_cap)
            {
                abort();
            }
        }

        const A *data() const
        {
            return data_;
        }

        A *data()
        {
            return data_;
        }

        A *begin()
        {
            return data_;
        }

        const A *begin() const
        {
            return data_;
        }

        A *end()
        {
            return data_ + ct_len;
        }

        const A *end() const
        {
            return data_ + ct_len;
        }

        bool empty() const
        {
            return ct_len == 0;
        }

    private:
        A *data_;
    };

    template <typename A, int ct_length>
    using ArrayView = EnableIf<ct_length != dyn, SequenceView<A, ct_length, ct_length>>;

    template <typename A, int ct_capacity>
    class SequenceView<A, dyn, ct_capacity>
        : public SequenceBase<SequenceView<A, dyn, ct_capacity>>
    {
    public:
        using Element = A;
        static constexpr int ct_len = dyn;
        static constexpr int ct_cap = ct_capacity;

        // static_assert(ct_len >= -1, "ct_length must greater or equal than -1.");
        // static_assert(ct_cap >= -1, "ct_capacity must greater or equal than -1.");

        // SequenceView() : data_{nullptr}, size_{0} {}
        // SequenceView(const SequenceView &) {}
        // SequenceView(SequenceView &&) {}

        explicit SequenceView(A *data, size_t size)
            : data_(data), size_(size)
        {
            // Ensure that data is not nullptr for a non-empty view.
            if (size > 0 && data_ == nullptr)
            {
                abort();
            }
        }

        SequenceView(A *data) : data_{data}
        {
        }
        // SequenceView(A *data, const int length)
        //     : data_{data}, size_{length}
        // {
        // }

        SequenceView &operator=(const SequenceView &other)
        {
            if (this != &other)
            {
                data_ = other.data_;
                size_ = other.size_;
            }
            return *this;
        }

        // SequenceView assign_impl(const SequenceView &other)
        // {
        //     if (this != &other)
        //     {
        //         data_ = other.data_;
        //         size_ = other.size_;
        //     }
        //     return *this;
        // }

        A &operator[](int index)
        {
            return data_[index];
        }

        const A &operator[](int index) const
        {
            return data_[index];
        }

        bool operator==(const SequenceView &other) const
        {
            return (data_ == other.data_) &&
                   (size_ == other.size_);
        }

        int size() const
        {
            return size_;
        }

        int capacity() const
        {
            return ct_cap;
        }

        void resize(int length)
        {
            if (length > ct_cap || length < 0)
            {
                abort();
            }

            size_ = length;
        }

        void reserve(int capacity)
        {
            if (capacity > ct_cap)
            {
                abort();
            }
        }

        const A *data() const
        {
            return data_;
        }

        A *data()
        {
            return data_;
        }

        A *begin()
        {
            return data_;
        }

        const A *begin() const
        {
            return data_;
        }

        A *end()
        {
            return data_ + size_;
        }

        const A *end() const
        {
            return data_ + size_;
        }

        bool empty() const
        {
            return size_ == 0;
        }

    private:
        A *data_;
        int size_;
    };

    template <typename A, int ct_capacity>
    using ArrVecView = EnableIf<ct_capacity != dyn, SequenceView<A, dyn, ct_capacity>>;

    template <typename A>
    class SequenceView<A, dyn, dyn>
        : public SequenceBase<SequenceView<A, dyn, dyn>>
    {
    public:
        using Element = A;
        static constexpr int ct_len = dyn;
        static constexpr int ct_cap = dyn;

        // static_assert(ct_len >= -1, "ct_length must greater or equal than -1.");
        // static_assert(ct_cap >= -1, "ct_capacity must greater or equal than -1.");

        // SequenceView() : data_{nullptr}, size_{0}, capacity_{0} {}
        // SequenceView(const SequenceView &) {}
        // SequenceView(SequenceView &&) {}

        explicit SequenceView(A *data, size_t size)
            : data_(data), size_(size), capacity_(size)
        {
            // Ensure that data is not nullptr for a non-empty view.
            if (size > 0 && data_ == nullptr)
            {
                abort();
            }
        }

        // SequenceView(A *data) : data_{data}, size_{0}, capacity_{0}
        // {
        // }

        // SequenceView(A *data, const int length, const int capacity)
        //     : data_{data}, size_{length}, capacity_{capacity} {}

        SequenceView &operator=(const SequenceView &other)
        {
            if (this != &other)
            {
                data_ = other.data_;
                size_ = other.size_;
                capacity_ = other.capacity_;
            }
            return *this;
        }

        // SequenceView assign_impl(const SequenceView &other)
        // {
        //     if (this != &other)
        //     {
        //         data_ = other.data_;
        //         size_ = other.size_;
        //         capacity_ = other.capacity_;
        //     }
        //     return *this;
        // }

        A &operator[](int index)
        {
            return data_[index];
        }

        const A &operator[](int index) const
        {
            return data_[index];
        }

        bool operator==(const SequenceView &other) const
        {
            return (data_ == other.data_) &&
                   (size_ == other.size_) &&
                   (capacity_ == other.capacity_);
        }

        int size() const
        {
            return size_;
        }

        int capacity() const
        {
            return capacity_;
        }

        void resize(int length)
        {
            if (length < 0)
            {
                abort();
            }

            if (length > capacity_)
            {
                reserve(length);
            }

            size_ = length;
        }

        void reserve(int capacity)
        {
            if (capacity > capacity_)
            {
                capacity_ = capacity;
            }
        }

        const A *data() const
        {
            return data_;
        }

        A *data()
        {
            return data_;
        }

        A *begin()
        {
            return data_;
        }

        const A *begin() const
        {
            return data_;
        }

        A *end()
        {
            return data_ + size_;
        }

        const A *end() const
        {
            return data_ + size_;
        }

        bool empty() const
        {
            return size_ == 0;
        }

    private:
        A *data_;
        int size_;
        int capacity_;
    };

    template <typename A>
    using VectorView = SequenceView<A, dyn, dyn>;

    template <typename A, int ct_length, int ct_capacity>
    class SequenceTrait<SequenceView<A, ct_length, ct_capacity>>
    {
    public:
        using Element = A;
        static constexpr int ct_len = ct_length;
        static constexpr int ct_cap = ct_capacity;
    };

    // todo STL only

    template <typename A, int ct_length, int ct_capacity>
    std::ostream &operator<<(std::ostream &os, const Sequence<A, ct_length, ct_capacity> &seq)
    {
        os << "{ ";
        for (size_t i = 0; i < seq.size(); ++i)
        {
            os << seq[i];
            if (i != seq.size() - 1)
            {
                os << ", ";
            }
        }
        os << " }";
        return os;
    }

    template <typename A, int ct_length, int ct_capacity>
    std::ostream &operator<<(std::ostream &os, const SequenceView<A, ct_length, ct_capacity> &seq)
    {
        os << "{ ";
        for (size_t i = 0; i < seq.size(); ++i)
        {
            os << seq[i];
            if (i != seq.size() - 1)
            {
                os << ", ";
            }
        }
        os << " }";
        return os;
    }

    // Element

    template <typename A>
    struct ElementImpl
    {
        using type = typename A::Element;
    };

    template <typename A>
    using Element = typename ElementImpl<A>::type;

    // StaticLength

    template <typename A>
    struct StaticLength
    {
        static constexpr int value = A::ct_len;
    };

    // StaticCapacity

    template <typename A>
    struct StaticCapacity
    {
        static constexpr int value = A::ct_cap;
    };

    // IsStaticLength

    template <typename A>
    struct IsStaticLength
    {
        static constexpr bool value = A::ct_len != dyn;
    };

    // AreAllStaticLength

    template <typename... A>
    struct AreAllStaticLength
    {
        static constexpr bool value = all_v(IsStaticLength<A>::value...);
    };

    // IsStaticCapacity

    template <typename A>
    struct IsStaticCapacity
    {
        static constexpr bool value = A::ct_cap != dyn;
    };

    // AreAllStaticCapacity

    template <typename... A>
    struct AreAllStaticCapacity
    {
        static constexpr bool value = all_v(IsStaticCapacity<A>::value...);
    };

    // MinStaticLength

    template <typename... A>
    struct MinStaticLength
    {
        static constexpr int value = minimum_v(StaticLength<A>::value...);
    };

    // MinStaticCapacity

    template <typename... A>
    struct MinStaticCapacity
    {
        static constexpr int value = minimum_v(StaticCapacity<A>::value...);
    };

    // length
    // ? Maybe need to be at prelude

    template <typename A>
    constexpr auto length(const Seq<A> &as)
        -> EnableIf<A::ct_len != dyn, IntegralConst<size_t, A::ct_len>>
    {
        return IntegralConst<size_t, A::ct_len>{};
    }

    template <typename A>
    constexpr auto length(const Seq<A> &as)
        -> EnableIf<A::ct_len == dyn, size_t>
    {
        return as.size();
    }

    // data

    template <typename A>
    constexpr auto data(const Seq<A> &as)
        -> const Element<A> *
    {
        return as.data();
    }

    template <typename A>
    constexpr auto data(Seq<A> &as)
        -> Element<A> *
    {
        return as.data();
    }

    // begin

    template <typename A>
    constexpr auto begin(const Seq<A> &as)
        -> const Element<A> *
    {
        return as.begin();
    }

    template <typename A>
    constexpr auto begin(Seq<A> &as)
        -> Element<A> *
    {
        return as.begin();
    }

    // end

    template <typename A>
    constexpr auto end(const Seq<A> &as)
        -> const Element<A> *
    {
        return as.end();
    }

    template <typename A>
    constexpr auto end(Seq<A> &as)
        -> Element<A> *
    {
        return as.end();
    }
};

#endif