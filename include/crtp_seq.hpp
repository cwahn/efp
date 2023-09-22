#ifndef CRTP_SEQ_HPP_
#define CRTP_SEQ_HPP_

// Curiously Recurring Template Pattern Sequence
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
        using Element = typename SequenceTrait<Derived>::Element;
        static constexpr int ct_len = SequenceTrait<Derived>::ct_len;
        static constexpr int ct_cap = SequenceTrait<Derived>::ct_cap;

        Derived &operator=(const Derived &other)
        {
            if (this != other)
            {
                derived().assign_from(other);
            }
            return *this;
        }

        Element &operator[](int index)
        {
            return derived()[index];
        }

        const Element &operator[](int index) const
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

        int capacity() const
        {
            return derived().capacity();
        }

        void resize(int length)
        {
            derived().resize(length);
        }

        void reserve(int capacity)
        {
            derived().reserve(capacity);
        }

        const Element *p_data() const
        {
            return derived().p_data();
        }

        Element *p_data()
        {
            return derived().p_data();
        }

        Element *p_begin()
        {
            return derived().p_begin();
        }

        const Element *p_begin() const
        {
            return derived().p_begin();
        }

        Element *p_end()
        {
            return derived().p_end();
        }

        const Element *p_end() const
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
            // if (length_ != other.length_)
            // {
            //     return false;
            // }

            for (int i = 0; i < ct_len; ++i)
            {
                if (data_[i] != other.data_[i])
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

        int capacity
        () const
        {
            return ct_cap;
        }

        void resize(int length)
        {
            if (length > ct_len)
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
            return p_data;
        }

        A *p_data()
        {
            return p_data;
        }

        A *p_begin()
        {
            return data_;
        }

        const A *p_begin() const
        {
            return data_;
        }

        A *p_end()
        {
            return data_ + ct_len;
        }

        const A *p_end() const
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
    using Arr = EnableIf_t<ct_length != dyn, Sequence<A, ct_length, ct_length>>;

    template <typename A, int ct_capacity>
    class Sequence<A, dyn, ct_capacity>
        : public SequenceBase<Sequence<A, dyn, ct_capacity>>
    {
    public:
        using Element = A;
        static constexpr int ct_len = dyn;
        static constexpr int ct_cap = ct_capacity;

        Sequence() {}
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

        int length() const
        {
            return length_;
        }

        int capacity
        () const
        {
            return ct_cap;
        }

        void resize(int length)
        {
            length_ = bound_v(0, ct_cap, length);
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
            return p_data;
        }

        A *p_data()
        {
            return p_data;
        }

        A *p_begin()
        {
            return data_;
        }

        const A *p_begin() const
        {
            return data_;
        }

        A *p_end()
        {
            return data_ + ct_len;
        }

        const A *p_end() const
        {
            return data_ + ct_len;
        }

        bool is_empty() const
        {
            return ct_len == 0;
        }

    private:
        A data_[ct_cap];
        int length_;
    };

    template <typename A, int ct_capacity>
    using ArrayVec = EnableIf_t<ct_capacity != dyn, Sequence<A, dyn, ct_capacity>>;

    template <typename A>
    class Sequence<A, dyn, dyn>
        : public SequenceBase<Sequence<A, dyn, dyn>>
    {
    public:
        using Element = A;
        static constexpr int ct_len = dyn;
        static constexpr int ct_cap = dyn;

        Sequence() : data_(nullptr), length_(0), capacity_(0) {}
        Sequence(const Sequence &); // Not emplemented by design for RVO, NRVO enforcement
        Sequence(Sequence &&);      // Not emplemented by design for RVO, NRVO enforcement
        template <typename... Args>
        Sequence(const Args &...args)
            : data_{new A[sizeof...(args)]},
              capacity_(sizeof...(args)),
              length_(sizeof...(args))
        {
            int i = 0;
            for (auto arg : std::initializer_list<typename std::common_type<Args...>::type>{args...})
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
                const int other_length = other.length();

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
                const int other_length = other.length();

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

        int length() const
        {
            return length_;
        }

        int capacity
        () const
        {
            return capacity_;
        }

        void resize(int length)
        {
            if (length > capacity_)
            {
                reserve(length);
            }
            length_ = max_v(0, length);
        }

        void reserve(int capacity)
        {
            if (capacity > capacity_)
            {
                A *new_data = new A[capacity];
                memcpy(new_data, data_, length_);
                delete[] data_;
                data_ = new_data;
                capacity_ = capacity;
            }
        }

        const A *p_data() const
        {
            return data_;
        }

        A *p_data()
        {
            return data_;
        }

        A *p_begin()
        {
            return data_;
        }

        const A *p_begin() const
        {
            return data_;
        }

        A *p_end()
        {
            return data_ + length_;
        }

        const A *p_end() const
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
    using Vec = Sequence<A, dyn, dyn>;

    template <typename A, int ct_length, int ct_capacity>
    class SequenceTrait<Sequence<A, ct_length, ct_capacity>>
    {
    public:
        using Element = A;
        static constexpr int ct_len = ct_length;
        static constexpr int ct_cap = ct_capacity;
    };

    // template <typename A, int ct_length, int ct_capacity>
    // bool operator==(
    //     const Sequence<A, ct_length, ct_length> &lhs,
    //     const Sequence<A, ct_length, ct_length> &rhs)
    // {
    //     const int lhs_length = lhs.length();
    //     const int rhs_length = rhs.length();

    //     if (lhs_length != rhs_length)
    //     {
    //         return false;
    //     }

    //     for (int i = 0; i < lhs_length; ++i)
    //     {
    //         if (lhs[i] != rhs[i])
    //         {
    //             return false;
    //         }
    //     }

    //     return true;
    // }
};

#endif