#ifndef SEQUENCE_HPP_
#define SEQUENCE_HPP_

// Curiously Recurring Template Pattern Sequence
// Check validity on data store

// ct_capacity is compile time bound of length. It does not mean safety of access.
// However, actual capacity does means the length of memory safe to access.

// todo STL only
#include <iostream>

#include "sfinae.hpp"

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

        static_assert(ct_len >= -1, "ct_length must greater or equal than -1.");
        static_assert(ct_cap >= -1, "ct_capacity must greater or equal than -1.");

        Sequence() {}
        Sequence(const Sequence &); // Not emplemented by design for RVO, NRVO enforcement
        Sequence(Sequence &&);      // Not emplemented by design for RVO, NRVO enforcement
        template <typename... Arg>
        Sequence(const Arg &...args)
            : data_{args...} {}

        Sequence &operator=(const Sequence &other)
        {
            if (this != &other)
            {
                memcpy(data_, other.data_, ct_cap);
            }
            return *this;
        }

        Sequence assign_impl(const Sequence &other)
        {
            if (this != &other)
            {
                memcpy(data_, other.data_, ct_cap);
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
            for (int i = 0; i < ct_len; ++i)
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

        bool is_empty() const
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

        static_assert(ct_len >= -1, "ct_length must greater or equal than -1.");
        static_assert(ct_cap >= -1, "ct_capacity must greater or equal than -1.");

        Sequence() : length_{0} {}
        Sequence(const Sequence &); // Not emplemented by design for RVO, NRVO enforcement
        Sequence(Sequence &&);      // Not emplemented by design for RVO, NRVO enforcement
        template <typename... Arg>
        Sequence(const Arg &...args)
            : data_{args...},
              length_(sizeof...(args)) {}

        Sequence &operator=(const Sequence &other)
        {
            if (this != &other)
            {
                memcpy(data_, other.data_, ct_cap);
            }
            return *this;
        }

        Sequence assign_impl(const Sequence &other)
        {
            if (this != &other)
            {
                memcpy(data_, other.data_, ct_cap);
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
            if (length_ != other.length_)
            {
                return false;
            }

            for (int i = 0; i < length_; ++i)
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
            return length_;
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

            length_ = length;
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
            if (length_ >= ct_cap)
            {
                abort();
            }
            else
            {
                data_[length_] = value;
                ++length_;
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
            return data_ + length_;
        }

        const A *end() const
        {
            return data_ + length_;
        }

        bool is_empty() const
        {
            return length_ == 0;
        }

    private:
        A data_[ct_cap];
        int length_;
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

        Sequence() : data_{nullptr}, length_{0}, capacity_{0} {}
        Sequence(const Sequence &); // Not emplemented by design for RVO, NRVO enforcement
        Sequence(Sequence &&);      // Not emplemented by design for RVO, NRVO enforcement
        template <typename... Args>
        Sequence(const Args &...args)
            : data_{new A[sizeof...(args)]},
              capacity_(sizeof...(args)),
              length_(sizeof...(args))
        {
            int i = 0;
            for (auto arg : std::initializer_list<Common<Args...>>{args...})
                data_[i++] = arg;
        }

        ~Sequence()
        {
            delete[] data_;
        }

        Sequence &operator=(const Sequence &other)
        {
            if (this != &other)
            {
                const int other_length = other.size();

                if (capacity_ < other_length)
                {
                    reserve(other_length);
                }

                memcpy(data_, other.data_, other_length);
            }
            return *this;
        }

        Sequence assign_impl(const Sequence &other)
        {
            if (this != &other)
            {
                const int other_length = other.size();

                if (capacity_ < other_length)
                {
                    reserve(other_length);
                }

                memcpy(data_, other.data_, other_length);
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
            if (length_ != other.length_)
            {
                return false;
            }

            for (int i = 0; i < length_; ++i)
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
            return length_;
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

            length_ = length;
        }

        void reserve(int capacity)
        {
            if (capacity > capacity_)
            {
                A *new_data = new A[capacity];
                memcpy(new_data, data_, length_ * sizeof(A));
                delete[] data_;
                data_ = new_data;
                capacity_ = capacity;
            }
        }

        void push_back(const A &value)
        {
            if (length_ >= capacity_)
            {
                reserve(capacity_ == 0 ? 1 : 2 * capacity_);
            }

            data_[length_] = value;
            ++length_;
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
            return data_ + length_;
        }

        const A *end() const
        {
            return data_ + length_;
        }

        bool is_empty() const
        {
            return length_ == 0;
        }

    private:
        A *data_;
        int length_;
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
    class SequenceRef
    {
    };

    template <typename A, int ct_length>
    class SequenceRef<A, ct_length, ct_length>
        : public SequenceBase<SequenceRef<A, ct_length, ct_length>>
    {
    public:
        using Element = A;
        static constexpr int ct_len = ct_length;
        static constexpr int ct_cap = ct_length;

        static_assert(ct_len >= -1, "ct_length must greater or equal than -1.");
        static_assert(ct_cap >= -1, "ct_capacity must greater or equal than -1.");

        SequenceRef() : data_{nullptr} {}
        SequenceRef(const SequenceRef &); // Not emplemented by design for RVO, NRVO enforcement
        SequenceRef(SequenceRef &&);      // Not emplemented by design for RVO, NRVO enforcement
        SequenceRef(A *data)
            : data_{data} {}
        // SequenceRef(const A (&c_array)[ct_len])
        //     : data_{c_array} {}
        // SequenceRef(const std::array<A, ct_len> &stl_array)
        //     : data_{stl_array.data()} {}

        SequenceRef &operator=(const SequenceRef &other)
        {
            if (this != &other)
            {
                data_ = other.data_;
            }
            return *this;
        }

        SequenceRef assign_impl(const SequenceRef &other)
        {
            if (this != &other)
            {
                data_ = other.data_;
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

        bool operator==(const SequenceRef &other) const
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

        bool is_empty() const
        {
            return ct_len == 0;
        }

    private:
        A *data_;
    };

    template <typename A, int ct_length>
    using ArrayView = EnableIf<ct_length != dyn, SequenceRef<A, ct_length, ct_length>>;

    template <typename A, int ct_capacity>
    class SequenceRef<A, dyn, ct_capacity>
        : public SequenceBase<SequenceRef<A, dyn, ct_capacity>>
    {
    public:
        using Element = A;
        static constexpr int ct_len = dyn;
        static constexpr int ct_cap = ct_capacity;

        static_assert(ct_len >= -1, "ct_length must greater or equal than -1.");
        static_assert(ct_cap >= -1, "ct_capacity must greater or equal than -1.");

        SequenceRef() : data_{nullptr}, length_{0} {}
        SequenceRef(const SequenceRef &); // Not emplemented by design for RVO, NRVO enforcement
        SequenceRef(SequenceRef &&);      // Not emplemented by design for RVO, NRVO enforcement
        SequenceRef(A *data) : data_{data} {}
        SequenceRef(A *data, const int length)
            : data_{data}, length_{length}
        {
        }

        SequenceRef &operator=(const SequenceRef &other)
        {
            if (this != &other)
            {
                data_ = other.data_;
                length_ = other.length_;
            }
            return *this;
        }

        SequenceRef assign_impl(const SequenceRef &other)
        {
            if (this != &other)
            {
                data_ = other.data_;
                length_ = other.length_;
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

        bool operator==(const SequenceRef &other) const
        {
            return (data_ == other.data_) &&
                   (length_ == other.length_);
        }

        int size() const
        {
            return length_;
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

            length_ = length;
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
            return data_ + length_;
        }

        const A *end() const
        {
            return data_ + length_;
        }

        bool is_empty() const
        {
            return length_ == 0;
        }

    private:
        A *data_;
        int length_;
    };

    template <typename A, int ct_capacity>
    using ArrVecView = EnableIf<ct_capacity != dyn, SequenceRef<A, dyn, ct_capacity>>;

    template <typename A>
    class SequenceRef<A, dyn, dyn>
        : public SequenceBase<SequenceRef<A, dyn, dyn>>
    {
    public:
        using Element = A;
        static constexpr int ct_len = dyn;
        static constexpr int ct_cap = dyn;

        static_assert(ct_len >= -1, "ct_length must greater or equal than -1.");
        static_assert(ct_cap >= -1, "ct_capacity must greater or equal than -1.");

        SequenceRef() : data_{nullptr}, length_{0}, capacity_{0} {}
        SequenceRef(const SequenceRef &); // Not emplemented by design for RVO, NRVO enforcement
        SequenceRef(SequenceRef &&);      // Not emplemented by design for RVO, NRVO enforcement
        SequenceRef(A *data) : data_{data} {}
        SequenceRef(A *data, const int length, const int capacity)
            : data_{data}, length_{length}, capacity_{capacity} {}
        SequenceRef(const std::vector<A> &stl_vector)
            : data_{stl_vector.data()},
              length_(stl_vector.size()),
              capacity_(stl_vector.capacity()) {}

        SequenceRef &operator=(const SequenceRef &other)
        {
            if (this != &other)
            {
                data_ = other.data_;
                length_ = other.length_;
                capacity_ = other.capacity_;
            }
            return *this;
        }

        SequenceRef assign_impl(const SequenceRef &other)
        {
            if (this != &other)
            {
                data_ = other.data_;
                length_ = other.length_;
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

        bool operator==(const SequenceRef &other) const
        {
            return (data_ == other.data_) &&
                   (length_ == other.length_) &&
                   (capacity_ == other.capacity_);
        }

        int size() const
        {
            return length_;
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

            length_ = length;
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
            return data_ + length_;
        }

        const A *end() const
        {
            return data_ + length_;
        }

        bool is_empty() const
        {
            return length_ == 0;
        }

    private:
        A *data_;
        int length_;
        int capacity_;
    };

    template <typename A>
    using VectorView = SequenceRef<A, dyn, dyn>;

    template <typename A, int ct_length, int ct_capacity>
    class SequenceTrait<SequenceRef<A, ct_length, ct_capacity>>
    {
    public:
        using Element = A;
        static constexpr int ct_len = ct_length;
        static constexpr int ct_cap = ct_capacity;
    };

    // todo STL only

    template <typename A, int ct_length, int ct_capacity>
    std::ostream &
    operator<<(std::ostream &os, const Sequence<A, ct_length, ct_capacity> &seq)
    {
        os << "{ ";
        for (int i = 0; i < seq.size(); ++i)
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
    std::ostream &operator<<(std::ostream &os, const SequenceRef<A, ct_length, ct_capacity> &seq)
    {
        os << "{ ";
        for (int i = 0; i < seq.size(); ++i)
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
        -> EnableIf<A::ct_len != dyn, IntegralConst<int, A::ct_len>>
    {
        return IntegralConst<int, A::ct_len>{};
    }

    template <typename A>
    constexpr auto length(const Seq<A> &as)
        -> EnableIf<A::ct_len == dyn, int>
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

    // Int

    template <int n>
    using Int = IntegralConst<int, n>;

};

#endif