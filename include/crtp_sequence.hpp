#ifndef CRTP_SEQUENCE_HPP_
#define CRTP_SEQUENCE_HPP_

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

        // static_assert(ct_len >= -1, "ct_length must greater or equal than -1.");
        // static_assert(ct_cap >= -1, "ct_capacity must greater or equal than -1.");

        Derived &operator=(const Derived &other)
        {
            if (this != other)
            {
                derived().assign_from(other);
            }
            return *this;
        }

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

        int length() const
        {
            return derived().length();
        }

        // ! Not appropreate for ref of sequence
        // int capacity() const
        // {
        //     return derived().capacity();
        // }

        // ! Mut, Not appropreate for ref of sequence
        // void resize(int length)
        // {
        //     derived().resize(length);
        // }

        // ! Mut, Not appropreate for ref of sequence
        // void reserve(int capacity)
        // {
        //     derived().reserve(capacity);
        // }

        const Element *p_data() const
        {
            return derived().p_data();
        }

        Element *p_data()
        {
            return derived().p_data();
        }

        const Element *p_begin() const
        {
            return derived().p_begin();
        }

        Element *p_begin()
        {
            return derived().p_begin();
        }

        const Element *p_end() const
        {
            return derived().p_end();
        }

        Element *p_end()
        {
            return derived().p_end();
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
            : p_data_{args...} {}

        Sequence &operator=(const Sequence &other)
        {
            if (this != &other)
            {
                memcpy(p_data_, other.p_data_, ct_cap);
            }
            return *this;
        }

        Sequence assign_impl(const Sequence &other)
        {
            if (this != &other)
            {
                memcpy(p_data_, other.p_data_, ct_cap);
            }
            return *this;
        }

        A &operator[](int index)
        {
            return p_data_[index];
        }

        const A &operator[](int index) const
        {
            return p_data_[index];
        }

        bool operator==(const Sequence &other) const
        {
            for (int i = 0; i < ct_len; ++i)
            {
                if (p_data_[i] != other.p_data_[i])
                {
                    return false;
                }
            }

            return true;
        }

        int length() const
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

        const A *p_data() const
        {
            return p_data_;
        }

        A *p_data()
        {
            return p_data_;
        }

        A *p_begin()
        {
            return p_data_;
        }

        const A *p_begin() const
        {
            return p_data_;
        }

        A *p_end()
        {
            return p_data_ + ct_len;
        }

        const A *p_end() const
        {
            return p_data_ + ct_len;
        }

        bool is_empty() const
        {
            return ct_len == 0;
        }

    private:
        A p_data_[ct_cap];
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

        Sequence() {}
        Sequence(const Sequence &); // Not emplemented by design for RVO, NRVO enforcement
        Sequence(Sequence &&);      // Not emplemented by design for RVO, NRVO enforcement
        template <typename... Arg>
        Sequence(const Arg &...args)
            : p_data_{args...},
              length_(sizeof...(args)) {}

        Sequence &operator=(const Sequence &other)
        {
            if (this != &other)
            {
                memcpy(p_data_, other.p_data_, ct_cap);
            }
            return *this;
        }

        Sequence assign_impl(const Sequence &other)
        {
            if (this != &other)
            {
                memcpy(p_data_, other.p_data_, ct_cap);
            }
            return *this;
        }

        A &operator[](int index)
        {
            return p_data_[index];
        }

        const A &operator[](int index) const
        {
            return p_data_[index];
        }

        bool operator==(const Sequence &other) const
        {
            if (length_ != other.length_)
            {
                return false;
            }

            for (int i = 0; i < length_; ++i)
            {
                if (p_data_[i] != other.p_data_[i])
                {
                    return false;
                }
            }

            return true;
        }

        int length() const
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
                p_data_[length_] = value;
                ++length_;
            }
        }

        const A *p_data() const
        {
            return p_data_;
        }

        A *p_data()
        {
            return p_data_;
        }

        A *p_begin()
        {
            return p_data_;
        }

        const A *p_begin() const
        {
            return p_data_;
        }

        A *p_end()
        {
            return p_data_ + length_;
        }

        const A *p_end() const
        {
            return p_data_ + length_;
        }

        bool is_empty() const
        {
            return length_ == 0;
        }

    private:
        A p_data_[ct_cap];
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

        Sequence() : p_data_{nullptr}, length_{0}, capacity_{0} {}
        Sequence(const Sequence &); // Not emplemented by design for RVO, NRVO enforcement
        Sequence(Sequence &&);      // Not emplemented by design for RVO, NRVO enforcement
        template <typename... Args>
        Sequence(const Args &...args)
            : p_data_{new A[sizeof...(args)]},
              capacity_(sizeof...(args)),
              length_(sizeof...(args))
        {
            int i = 0;
            for (auto arg : std::initializer_list<typename std::common_type<Args...>::type>{args...})
                p_data_[i++] = arg;
        }

        ~Sequence()
        {
            delete[] p_data_;
        }

        Sequence &operator=(const Sequence &other)
        {
            if (this != &other)
            {
                const int other_length = other.length();

                if (capacity_ < other_length)
                {
                    reserve(other_length);
                }

                memcpy(p_data_, other.p_data_, other_length);
            }
            return *this;
        }

        Sequence assign_impl(const Sequence &other)
        {
            if (this != &other)
            {
                const int other_length = other.length();

                if (capacity_ < other_length)
                {
                    reserve(other_length);
                }

                memcpy(p_data_, other.p_data_, other_length);
            }
            return *this;
        }

        A &operator[](int index)
        {
            return p_data_[index];
        }

        const A &operator[](int index) const
        {
            return p_data_[index];
        }

        bool operator==(const Sequence &other) const
        {
            if (length_ != other.length_)
            {
                return false;
            }

            for (int i = 0; i < length_; ++i)
            {
                if (p_data_[i] != other.p_data_[i])
                {
                    return false;
                }
            }

            return true;
        }

        int length() const
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
                A *p_new_data = new A[capacity];
                memcpy(p_new_data, p_data_, length_ * sizeof(A));
                delete[] p_data_;
                p_data_ = p_new_data;
                capacity_ = capacity;
            }
        }

        void push_back(const A &value)
        {
            if (length_ >= capacity_)
            {
                reserve(capacity_ == 0 ? 1 : 2 * capacity_);
            }

            p_data_[length_] = value;
            ++length_;
        }

        const A *p_data() const
        {
            return p_data_;
        }

        A *p_data()
        {
            return p_data_;
        }

        A *p_begin()
        {
            return p_data_;
        }

        const A *p_begin() const
        {
            return p_data_;
        }

        A *p_end()
        {
            return p_data_ + length_;
        }

        const A *p_end() const
        {
            return p_data_ + length_;
        }

        bool is_empty() const
        {
            return length_ == 0;
        }

    private:
        A *p_data_;
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

        // static_assert(ct_len >= -1, "ct_length must greater or equal than -1.");
        // static_assert(ct_cap >= -1, "ct_capacity must greater or equal than -1.");
    };

    // Should have all these three template parameter not to break static link
    template <typename A, int ct_length, int ct_capacity>
    class SequenceView
    {
    };

    template <typename A, int ct_length>
    class SequenceView<A, ct_length, ct_length>
        : SequenceBase<SequenceView<A, ct_length, ct_length>>
    {
    public:
        using Element = A;
        static constexpr int ct_len = ct_length;
        static constexpr int ct_cap = ct_length;

        static_assert(ct_len >= -1, "ct_length must greater or equal than -1.");
        static_assert(ct_cap >= -1, "ct_capacity must greater or equal than -1.");

        SequenceView() : p_data_{nullptr} {}
        SequenceView(const SequenceView &); // Not emplemented by design for RVO, NRVO enforcement
        SequenceView(SequenceView &&);      // Not emplemented by design for RVO, NRVO enforcement
        SequenceView(A *p_data)
            : p_data_{p_data}
        {
        }

        SequenceView &operator=(const SequenceView &other)
        {
            if (this != &other)
            {
                p_data_ = other.p_data_;
            }
            return *this;
        }

        SequenceView assign_impl(const SequenceView &other)
        {
            if (this != &other)
            {
                p_data_ = other.p_data_;
            }
            return *this;
        }

        A &operator[](int index)
        {
            return p_data_[index];
        }

        const A &operator[](int index) const
        {
            return p_data_[index];
        }

        bool operator==(const SequenceView &other) const
        {
            return p_data_ == other.p_data_;
        }

        int length() const
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

        const A *p_data() const
        {
            return p_data_;
        }

        A *p_data()
        {
            return p_data_;
        }

        A *p_begin()
        {
            return p_data_;
        }

        const A *p_begin() const
        {
            return p_data_;
        }

        A *p_end()
        {
            return p_data_ + ct_len;
        }

        const A *p_end() const
        {
            return p_data_ + ct_len;
        }

        bool is_empty() const
        {
            return ct_len == 0;
        }

    private:
        A *p_data_;
    };

    template <typename A, int ct_length>
    using ArrayView = EnableIf<ct_length != dyn, SequenceView<A, ct_length, ct_length>>;

    template <typename A, int ct_capacity>
    class SequenceView<A, dyn, ct_capacity>
        : SequenceBase<SequenceView<A, dyn, ct_capacity>>
    {
    public:
        using Element = A;
        static constexpr int ct_len = dyn;
        static constexpr int ct_cap = ct_capacity;

        static_assert(ct_len >= -1, "ct_length must greater or equal than -1.");
        static_assert(ct_cap >= -1, "ct_capacity must greater or equal than -1.");

        SequenceView() : p_data_{nullptr}, length_{0} {}
        SequenceView(const SequenceView &); // Not emplemented by design for RVO, NRVO enforcement
        SequenceView(SequenceView &&);      // Not emplemented by design for RVO, NRVO enforcement
        SequenceView(A *p_data) : p_data_{p_data} {}
        SequenceView(A *p_data, const int length)
            : p_data_{p_data}, length_{length}
        {
        }

        SequenceView &operator=(const SequenceView &other)
        {
            if (this != &other)
            {
                p_data_ = other.p_data_;
                length_ = other.length_;
            }
            return *this;
        }

        SequenceView assign_impl(const SequenceView &other)
        {
            if (this != &other)
            {
                p_data_ = other.p_data_;
                length_ = other.length_;
            }
            return *this;
        }

        A &operator[](int index)
        {
            return p_data_[index];
        }

        const A &operator[](int index) const
        {
            return p_data_[index];
        }

        bool operator==(const SequenceView &other) const
        {
            return (p_data_ == other.p_data_) &&
                   (length_ == other.length_);
        }

        int length() const
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

        const A *p_data() const
        {
            return p_data_;
        }

        A *p_data()
        {
            return p_data_;
        }

        A *p_begin()
        {
            return p_data_;
        }

        const A *p_begin() const
        {
            return p_data_;
        }

        A *p_end()
        {
            return p_data_ + length_;
        }

        const A *p_end() const
        {
            return p_data_ + length_;
        }

        bool is_empty() const
        {
            return length_ == 0;
        }

    private:
        A *p_data_;
        int length_;
    };

    template <typename A, int ct_capacity>
    using ArrVecView = EnableIf<ct_capacity != dyn, SequenceView<A, dyn, ct_capacity>>;

    template <typename A>
    class SequenceView<A, dyn, dyn>
        : SequenceBase<SequenceView<A, dyn, dyn>>
    {
    public:
        using Element = A;
        static constexpr int ct_len = dyn;
        static constexpr int ct_cap = dyn;

        static_assert(ct_len >= -1, "ct_length must greater or equal than -1.");
        static_assert(ct_cap >= -1, "ct_capacity must greater or equal than -1.");

        SequenceView() : p_data_{nullptr}, length_{0}, capacity_{0} {}
        SequenceView(const SequenceView &); // Not emplemented by design for RVO, NRVO enforcement
        SequenceView(SequenceView &&);      // Not emplemented by design for RVO, NRVO enforcement
        SequenceView(A *p_data) : p_data_{p_data} {}
        SequenceView(A *p_data, const int length, const int capacity)
            : p_data_{p_data}, length_{length}, capacity_{capacity}
        {
        }

        SequenceView &operator=(const SequenceView &other)
        {
            if (this != &other)
            {
                p_data_ = other.p_data_;
                length_ = other.length_;
                capacity_ = other.capacity_;
            }
            return *this;
        }

        SequenceView assign_impl(const SequenceView &other)
        {
            if (this != &other)
            {
                p_data_ = other.p_data_;
                length_ = other.length_;
                capacity_ = other.capacity_;
            }
            return *this;
        }

        A &operator[](int index)
        {
            return p_data_[index];
        }

        const A &operator[](int index) const
        {
            return p_data_[index];
        }

        bool operator==(const SequenceView &other) const
        {
            return (p_data_ == other.p_data_) &&
                   (length_ == other.length_) &&
                   (capacity_ == other.capacity_);
        }

        int length() const
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

        const A *p_data() const
        {
            return p_data_;
        }

        A *p_data()
        {
            return p_data_;
        }

        A *p_begin()
        {
            return p_data_;
        }

        const A *p_begin() const
        {
            return p_data_;
        }

        A *p_end()
        {
            return p_data_ + length_;
        }

        const A *p_end() const
        {
            return p_data_ + length_;
        }

        bool is_empty() const
        {
            return length_ == 0;
        }

    private:
        A *p_data_;
        int length_;
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

        // static_assert(ct_len >= -1, "ct_length must greater or equal than -1.");
        // static_assert(ct_cap >= -1, "ct_capacity must greater or equal than -1.");
    };

    // todo STL only

    template <typename A, int ct_length, int ct_capacity>
    std::ostream &operator<<(std::ostream &os, const Sequence<A, ct_length, ct_capacity> &seq)
    {
        os << "{ ";
        for (int i = 0; i < seq.length(); ++i)
        {
            os << seq[i];
            if (i != seq.length() - 1)
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
        for (int i = 0; i < seq.length(); ++i)
        {
            os << seq[i];
            if (i != seq.length() - 1)
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
        return as.length();
    }

    // p_data

    template <typename A>
    constexpr auto p_data(const Seq<A> &as)
        -> const Element<A> *
    {
        return as.p_data();
    }

    template <typename A>
    constexpr auto p_data(Seq<A> &as)
        -> Element<A> *
    {
        return as.p_data();
    }

    // p_begin

    template <typename A>
    constexpr auto p_begin(const Seq<A> &as)
        -> const Element<A> *
    {
        return as.p_begin();
    }

    template <typename A>
    constexpr auto p_begin(Seq<A> &as)
        -> Element<A> *
    {
        return as.p_begin();
    }

    // p_end

    template <typename A>
    constexpr auto p_end(const Seq<A> &as)
        -> const Element<A> *
    {
        return as.p_end();
    }

    template <typename A>
    constexpr auto p_end(Seq<A> &as)
        -> Element<A> *
    {
        return as.p_end();
    }

};

#endif