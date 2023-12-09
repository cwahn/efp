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
#include "trait.hpp"

// todo Move and copy assigment operator sort out

namespace efp
{
    // constexpr size_t dyn = -1;

    // template <typename Derived>
    // class SequenceTrait;

    // template <typename Derived>
    // class SequenceBase
    // {
    // public:
    //     using Trait = SequenceTrait<Derived>;

    //     using Element = typename Trait::Element;
    //     static constexpr size_t ct_size = Trait::ct_size;
    //     static constexpr size_t ct_capacity = Trait::ct_capacity;

    //     const Element &operator[](size_t index) const
    //     {
    //         return derived()[index];
    //     }

    //     // ! Mut
    //     Element &operator[](size_t index)
    //     {
    //         return derived()[index];
    //     }

    //     bool operator==(const SequenceBase &other) const
    //     {
    //         return derived() == other.derived();
    //     }

    //     size_t size() const
    //     {
    //         return derived().size();
    //     }

    //     const Element *data() const
    //     {
    //         return derived().data();
    //     }

    //     Element *data()
    //     {
    //         return derived().data();
    //     }

    //     const Element *begin() const
    //     {
    //         return derived().begin();
    //     }

    //     Element *begin()
    //     {
    //         return derived().begin();
    //     }

    //     const Element *end() const
    //     {
    //         return derived().end();
    //     }

    //     Element *end()
    //     {
    //         return derived().end();
    //     }

    // private:
    //     Derived &derived()
    //     {
    //         return *static_cast<Derived *>(this);
    //     }
    //     const Derived &derived() const
    //     {
    //         return *static_cast<const Derived *>(this);
    //     }
    // };

    // template <typename Derived>
    // using Seq = SequenceBase<Derived>;

    // template <typename A, size_t ct_size, size_t ct_capacity>
    // class Sequence
    // {
    // };

    template <typename A, size_t ct_size>
    // class Sequence<A, ct_size, ct_size>
    class Array
    // : public SequenceBase<Sequence<A, ct_size, ct_size>>
    {
    public:
        using Element = A;
        using CtSize = Size<ct_size>;
        using CtCapcity = Size<ct_size>;

        // static constexpr size_t ct_size = ct_size;
        // static constexpr size_t ct_capacity = ct_size;
        // static_assert(ct_size >= -1, "ct_size must greater or equal than -1.");
        // static_assert(ct_capacity >= -1, "ct_capacity must greater or equal than -1.");

        Sequence()
        {
            for (size_t i = 0; i < ct_size; ++i)
            {
                new (&data_[i]) Element();
            }
        }

        Sequence(const Sequence &other)
        {
            for (size_t i = 0; i < ct_size; ++i)
            {
                new (&data_[i]) Element(other.data_[i]);
            }
        }

        Sequence(Sequence &&other) noexcept
        {
            for (size_t i = 0; i < ct_size; ++i)
            {
                new (&data_[i]) Element(efp::move(other.data_[i])); // Move-construct each element
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
                for (size_t i = 0; i < ct_size; ++i)
                {
                    data_[i] = other.data_[i]; // Use assignment for each element
                }
            }
            return *this;
        }

        Element &operator[](size_t index)
        {
            return data_[index];
        }

        const Element &operator[](size_t index) const
        {
            return data_[index];
        }

        bool operator==(const Sequence &other) const
        {
            for (size_t i = 0; i < ct_size; ++i)
            {
                if (data_[i] != other.data_[i])
                {
                    return false;
                }
            }

            return true;
        }

        size_t size() const
        {
            return ct_size;
        }

        size_t capacity() const
        {
            return ct_size;
        }

        void resize(size_t length)
        {
            if (length != ct_size)
            {
                abort();
            }
        }

        void reserve(size_t capacity)
        {
            if (capacity > ct_size)
            {
                abort();
            }
        }

        const Element *data() const
        {
            return data_;
        }

        Element *data()
        {
            return data_;
        }

        Element *begin()
        {
            return data_;
        }

        const Element *begin() const
        {
            return data_;
        }

        Element *end()
        {
            return data_ + ct_size;
        }

        const Element *end() const
        {
            return data_ + ct_size;
        }

        bool empty() const
        {
            return ct_size == 0;
        }

    private:
        A data_[ct_size];
    };

    // template <typename A, size_t ct_size>
    // using Array = EnableIf<ct_size != dyn, Sequence<A, ct_size, ct_size>>;

    template <typename A, size_t ct_capacity>
    // class Sequence<A, dyn, ct_capacity>
    class ArrVec
    // : public SequenceBase<Sequence<A, dyn, ct_capacity>>
    {
    public:
        using Element = A;
        using CtSize = Size<dyn>;
        using CtCapcity = Size<ct_capacity>;

        // static constexpr size_t ct_size = dyn;
        // static constexpr size_t ct_capacity = ct_capacity;

        // static_assert(ct_size >= -1, "ct_size must greater or equal than -1.");
        // static_assert(ct_capacity >= -1, "ct_capacity must greater or equal than -1.");

        Sequence() : size_{0} {}

        Sequence(const Sequence &other) : size_{other.size_}
        {
            for (size_t i = 0; i < size_; ++i)
            {
                new (&data_[i]) Element(other.data_[i]);
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

        // Constructor from array
        template <size_t ct_size_, typename = EnableIf<ct_capacity >= ct_size_, void>>
        Sequence(const Sequence<Element, ct_size_, ct_size_> &as)
            : size_(ct_size_)
        {
            for (size_t i = 0; i < ct_size_; ++i)
            {
                new (&data_[i]) Element(as[i]);
            }
        }

        ~Sequence()
        {
            for (size_t i = 0; i < size_; ++i)
            {
                data_[i].~Element();
            }
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

        Sequence &operator=(Sequence &&other) noexcept
        {
            if (this != &other)
            {
                // Destroy existing elements
                for (size_t i = 0; i < size_; ++i)
                {
                    data_[i].~Element();
                }

                // Move data from the source object
                size_ = other.size_;
                for (size_t i = 0; i < size_; ++i)
                {
                    new (&data_[i]) Element(std::move(other.data_[i]));
                }

                // Reset the source object
                other.size_ = 0;
            }
            return *this;
        }

        Element &operator[](size_t index)
        {
            return data_[index];
        }

        const Element &operator[](size_t index) const
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

        size_t size() const
        {
            return size_;
        }

        size_t capacity() const
        {
            return ct_capacity;
        }

        void resize(size_t length)
        {
            if (length > ct_capacity || length < 0)
            {
                abort();
            }

            size_ = length;
        }

        void reserve(size_t capacity)
        {
            if (capacity > ct_capacity)
            {
                abort();
            }
        }

        void push_back(const Element &value)
        {
            if (size_ >= ct_capacity)
            {
                abort();
            }
            else
            {
                new (&data_[size_]) Element(value);
                ++size_;
            }
        }

        void push_back(Element &&value)
        {
            if (size_ >= ct_capacity)
            {
                abort();
            }
            else
            {
                new (&data_[size_]) Element(efp::move(value));
                ++size_;
            }
        }

        void insert(size_t index, const Element &value)
        {
            if (index < 0 || index > size_ || size_ == ct_capacity)
            {
                abort();
            }

            for (size_t i = size_; i > index; --i)
            {
                new (&data_[i]) Element(efp::move(data_[i - 1]));
                data_[i - 1].~Element();
            }

            new (&data_[index]) Element(value);

            ++size_;
        }

        void pop_back()
        {
            if (size_ == 0)
            {
                abort();
            }

            data_[size_ - 1].~Element();
            --size_;
        }

        void clear()
        {
            for (size_t i = 0; i < size_; ++i)
            {
                data_[i].~Element();
            }
            size_ = 0;
        }

        void erase(size_t index)
        {
            if (index < 0 || index >= size_)
            {
                abort();
            }
            data_[index].~Element();
            for (size_t i = index; i < size_ - 1; ++i)
            {
                new (&data_[i]) Element(efp::move(data_[i + 1]));
                data_[i + 1].~Element();
            }
            --size_;
        }

        const Element *data() const
        {
            return data_;
        }

        Element *data()
        {
            return data_;
        }

        Element *begin()
        {
            return data_;
        }

        const Element *begin() const
        {
            return data_;
        }

        Element *end()
        {
            return data_ + size_;
        }

        const Element *end() const
        {
            return data_ + size_;
        }

        bool empty() const
        {
            return size_ == 0;
        }

    private:
        Element data_[ct_capacity];
        size_t size_;
    };

    // template <typename A, size_t ct_capacity>
    // using ArrVec = EnableIf<ct_capacity != dyn, Sequence<A, dyn, ct_capacity>>;

    template <typename A>
    // class Sequence<A, dyn, dyn>
    class Vector
    // : public SequenceBase<Sequence<A, dyn, dyn>>
    {
    public:
        using Element = A;
        using CtSize = Size<dyn>;
        using CtCapcity = Size<dyn>;

        // static constexpr size_t ct_size = dyn;
        // static constexpr size_t ct_capacity = dyn;

        // static_assert(ct_size >= -1, "ct_size must greater or equal than -1.");
        // static_assert(ct_capacity >= -1, "ct_capacity must greater or equal than -1.");

        Sequence() : data_{nullptr}, size_{0}, capacity_{0} {}

        Sequence(const Sequence &other)
            : data_{nullptr}, size_{0}, capacity_{0}
        {
            if (other.data_)
            {
                size_ = other.size_;
                capacity_ = other.capacity_;
                data_ = new Element[capacity_];

                // memcpy(data_, other.data_, sizeof(A) * size_);
                for (size_t i = 0; i < other.size_; ++i)
                {
                    new (&data_[i]) Element(other.data_[i]);
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
            : data_{new Element[sizeof...(args)]},
              capacity_(sizeof...(args)),
              size_(sizeof...(args))
        {
            size_t i = 0;
            for (auto arg : std::initializer_list<Common<Args...>>{args...})
                data_[i++] = arg;
        }

        // Constructor from Array
        template <size_t ct_size_>
        Sequence(const Sequence<Element, ct_size_, ct_size_> &as)
            : data_(new Element[ct_size_]),
              size_(ct_size_),
              capacity_(ct_size_)
        {
            for (size_t i = 0; i < size_; ++i)
            {
                new (&data_[i]) Element(as[i]);
            }
        }

        // Constructor from ArrVec
        template <size_t ct_cap_>
        Sequence(const Sequence<Element, dyn, ct_cap_> &as)
            : data_(new Element[as.size()]),
              size_(as.size()),
              capacity_(ct_cap_)
        {
            for (size_t i = 0; i < size_; ++i)
            {
                new (&data_[i]) Element(as[i]);
            }
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
                Element *newData = nullptr;
                if (other.size_ > 0)
                {
                    newData = new Element[other.capacity_];

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

        Element &operator[](size_t index)
        {
            return data_[index];
        }

        const Element &operator[](size_t index) const
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

        size_t size() const
        {
            return size_;
        }

        size_t capacity() const
        {
            return capacity_;
        }

        void resize(size_t length)
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

        void reserve(size_t new_capacity)
        {
            if (new_capacity > capacity_)
            {
                Element *new_data = new Element[new_capacity];

                for (size_t i = 0; i < size_; ++i)
                {
                    new (&new_data[i]) Element(efp::move(data_[i]));
                    data_[i].~Element();
                }

                delete[] data_;

                data_ = new_data;
                capacity_ = new_capacity;
            }
        }

        void push_back(const Element &value)
        {
            if (size_ >= capacity_)
            {
                reserve(capacity_ == 0 ? 1 : 2 * capacity_);
            }

            new (&data_[size_]) Element(value);
            ++size_;
        }

        void push_back(Element &&value)
        {
            if (size_ >= capacity_)
            {
                reserve(capacity_ == 0 ? 1 : 2 * capacity_);
            }

            new (&data_[size_]) Element(efp::move(value));
            ++size_;
        }

        void insert(size_t index, const Element &value)
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
            new (&data_[index]) Element(value);
            ++size_;
        }

        void erase(size_t index)
        {
            if (index < 0 || index >= size_)
            {
                abort();
            }

            data_[index].~Element();
            for (size_t i = index; i < size_ - 1; ++i)
            {
                new (&data_[i]) Element(efp::move(data_[i + 1]));
                data_[i + 1].~Element();
            }

            --size_;
        }

        void clear()
        {
            for (size_t i = 0; i < size_; ++i)
            {
                data_[i].~Element();
            }
            size_ = 0;
        }

        void pop_back()
        {
            if (size_ == 0)
            {
                abort();
            }
            data_[size_ - 1].~Element();
            --size_;
        }

        const Element *data() const
        {
            return data_;
        }

        Element *data()
        {
            return data_;
        }

        Element *begin()
        {
            return data_;
        }

        const Element *begin() const
        {
            return data_;
        }

        Element *end()
        {
            return data_ + size_;
        }

        const Element *end() const
        {
            return data_ + size_;
        }

        bool empty() const
        {
            return size_ == 0;
        }

    private:
        Element *data_;
        size_t size_;
        size_t capacity_;
    };

    // template <typename A>
    // using Vector = Sequence<A, dyn, dyn>;

    // template <typename A, size_t ct_size, size_t ct_capacity>
    // class SequenceTrait<Sequence<A, ct_size, ct_capacity>>
    // {
    // public:
    //     using Element = A;
    //     static constexpr size_t ct_size = ct_size;
    //     static constexpr size_t ct_capacity = ct_capacity;
    // };

    // Should have all these three template parameter not to break static link
    // template <typename A, size_t ct_size, size_t ct_capacity>
    // class SequenceView
    // {
    // };

    template <typename A, size_t ct_size>
    // class SequenceView<A, ct_size, ct_size>
    class ArrayView
    // : public SequenceBase<SequenceView<A, ct_size, ct_size>>
    {
    public:
        using Element = A;
        using CtSize = Size<ct_size>;
        using CtCapcity = Size<ct_size>;
        // static constexpr size_t ct_size = ct_size;
        // static constexpr size_t ct_capacity = ct_size;

        // static_assert(ct_size >= -1, "ct_size must greater or equal than -1.");

        SequenceView()
            : data_(nullptr)
        {
        }

        // ! length will not be used
        SequenceView(Element *data, Size<ct_size> length = Size<ct_size>{})
            : data_(data)
        {
            // Ensure that data is not nullptr for a non-empty view.
            if (ct_size > 0 && data_ == nullptr)
            {
                abort();
            }
        }

        Element &operator[](size_t index)
        {
            return data_[index];
        }

        const Element &operator[](size_t index) const
        {
            return data_[index];
        }

        bool operator==(const SequenceView &other) const
        {
            return data_ == other.data_;
        }

        size_t size() const
        {
            return ct_size;
        }

        size_t capacity() const
        {
            return ct_size;
        }

        void resize(size_t length)
        {
            if (length > ct_size || length < 0)
            {
                abort();
            }
        }

        void reserve(size_t capacity)
        {
            if (capacity > ct_size)
            {
                abort();
            }
        }

        const Element *data() const
        {
            return data_;
        }

        Element *data()
        {
            return data_;
        }

        Element *begin()
        {
            return data_;
        }

        const Element *begin() const
        {
            return data_;
        }

        Element *end()
        {
            return data_ + ct_size;
        }

        const Element *end() const
        {
            return data_ + ct_size;
        }

        bool empty() const
        {
            return ct_size == 0;
        }

    private:
        Element *data_;
    };

    // template <typename A, size_t ct_size>
    // using ArrayView = EnableIf<ct_size != dyn, SequenceView<A, ct_size, ct_size>>;

    template <typename A, size_t ct_capacity>
    // class SequenceView<A, dyn, ct_capacity>
    class ArrVecView
    // : public SequenceBase<SequenceView<A, dyn, ct_capacity>>
    {
    public:
        using Element = A;
        using CtSize = Size<dyn>;
        using CtCapcity = Size<ct_capacity>;

        // static constexpr size_t ct_size = dyn;
        // static constexpr size_t ct_capacity = ct_capacity;

        // static_assert(ct_size >= -1, "ct_size must greater or equal than -1.");
        // static_assert(ct_capacity >= -1, "ct_capacity must greater or equal than -1.");

        SequenceView()
            : data_(nullptr), size_(0)
        {
        }

        SequenceView(Element *data, size_t size)
            : data_(data), size_(size)
        {
            // Ensure that data is not nullptr for a non-empty view.
            if (size > 0 && data_ == nullptr)
            {
                abort();
            }
        }

        // Constructor from ArrayView
        template <size_t ct_size_>
        SequenceView(const SequenceView<Element, ct_size_, ct_size_> &as)
            : data_(as.data()), size_(as.size())
        {
        }

        SequenceView &
        operator=(const SequenceView &other)
        {
            if (this != &other)
            {
                data_ = other.data_;
                size_ = other.size_;
            }
            return *this;
        }

        Element &operator[](size_t index)
        {
            return data_[index];
        }

        const Element &operator[](size_t index) const
        {
            return data_[index];
        }

        bool operator==(const SequenceView &other) const
        {
            return (data_ == other.data_) &&
                   (size_ == other.size_);
        }

        size_t size() const
        {
            return size_;
        }

        size_t capacity() const
        {
            return ct_capacity;
        }

        const Element *data() const
        {
            return data_;
        }

        Element *data()
        {
            return data_;
        }

        Element *begin()
        {
            return data_;
        }

        const Element *begin() const
        {
            return data_;
        }

        Element *end()
        {
            return data_ + size_;
        }

        const Element *end() const
        {
            return data_ + size_;
        }

        bool empty() const
        {
            return size_ == 0;
        }

    private:
        Element *data_;
        size_t size_;
    };

    // template <typename A, size_t ct_capacity>
    // using ArrVecView = EnableIf<ct_capacity != dyn, SequenceView<A, dyn, ct_capacity>>;

    template <typename A>
    // class SequenceView<A, dyn, dyn>
    class VectorView
    // : public SequenceBase<SequenceView<A, dyn, dyn>>
    {
    public:
        using Element = A;
        using CtSize = Size<dyn>;
        using CtCapcity = Size<dyn>;

        // static constexpr size_t ct_size = dyn;
        // static constexpr size_t ct_capacity = dyn;

        // static_assert(ct_size >= -1, "ct_size must greater or equal than -1.");
        // static_assert(ct_capacity >= -1, "ct_capacity must greater or equal than -1.");

        SequenceView()
            : data_{nullptr}, size_{0}, capacity_{0}
        {
        }

        SequenceView(Element *data, size_t size)
            : data_(data), size_(size), capacity_(size)
        {
            // Ensure that data is not nullptr for a non-empty view.
            if (size > 0 && data_ == nullptr)
            {
                abort();
            }
        }

        // Constructor from ArrayView
        template <size_t ct_size_>
        SequenceView(const SequenceView<Element, ct_size_, ct_size_> &as)
            : data_(as.data()), size_(as.size())
        {
        }

        // Constructor from ArrVecView
        template <size_t ct_cap_>
        SequenceView(const SequenceView<Element, dyn, ct_cap_> &as)
            : data_(as.data()), size_(as.size())
        {
        }

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

        Element &operator[](size_t index)
        {
            return data_[index];
        }

        const Element &operator[](size_t index) const
        {
            return data_[index];
        }

        bool operator==(const SequenceView &other) const
        {
            return (data_ == other.data_) &&
                   (size_ == other.size_) &&
                   (capacity_ == other.capacity_);
        }

        size_t size() const
        {
            return size_;
        }

        size_t capacity() const
        {
            return capacity_;
        }

        const Element *data() const
        {
            return data_;
        }

        Element *data()
        {
            return data_;
        }

        Element *begin()
        {
            return data_;
        }

        const Element *begin() const
        {
            return data_;
        }

        Element *end()
        {
            return data_ + size_;
        }

        const Element *end() const
        {
            return data_ + size_;
        }

        bool empty() const
        {
            return size_ == 0;
        }

    private:
        Element *data_;
        size_t size_;
        size_t capacity_;
    };

    template <typename A>
    using VectorView = SequenceView<A, dyn, dyn>;

    // template <typename A, size_t ct_size, size_t ct_capacity>
    // class SequenceTrait<SequenceView<A, ct_size, ct_capacity>>
    // {
    // public:
    //     using Element = A;
    //     static constexpr size_t ct_size = ct_size;
    //     static constexpr size_t ct_capacity = ct_capacity;
    // };

    // todo STL only

    template <typename A, size_t ct_size, size_t ct_capacity>
    std::ostream &operator<<(std::ostream &os, const Sequence<A, ct_size, ct_capacity> &seq)
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

    template <typename A, size_t ct_size, size_t ct_capacity>
    std::ostream &operator<<(std::ostream &os, const SequenceView<A, ct_size, ct_capacity> &seq)
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

    // template <typename A>
    // struct ElementImpl
    // {
    //     using type = typename A::Element;
    // };

    // template <typename A>
    // using Element = typename ElementImpl<A>::type;

    // StaticLength
    //! Deprecated

    // template <typename A>
    // struct StaticLength
    // {
    //     static constexpr size_t value = A::ct_size;
    // };

    // StaticCapacity
    // ! Deprecated
    // template <typename A>
    // struct StaticCapacity
    // {
    //     static constexpr size_t value = A::ct_capacity;
    // };
};

#endif