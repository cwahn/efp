#ifndef SEQUENCE_HPP_
#define SEQUENCE_HPP_

#include <array>

#include "sfinae.hpp"

namespace efp
{
    template <typename A, size_t N>
    class ArrayProxy_ : public std::array<A, N>
    {
    public:
        using std::array<A, N>::array;
        ArrayProxy_(const ArrayProxy_ &) = delete;
        ArrayProxy_(ArrayProxy_ &&) = delete;

        // Assignment operators
        ArrayProxy_ &operator=(const ArrayProxy_ &) = default;
        ArrayProxy_ &operator=(ArrayProxy_ &&) = default;

        // Variadic constructor for the derived class
        template <typename... Args>
        ArrayProxy_(Args &&...args) : std::array<A, N>{{std::forward<Args>(args)...}} {}
    };

    template <typename A, size_t N>
    class Array : public ArrayProxy_<A, N>
    {
    public:
        using ArrayProxy_<A, N>::ArrayProxy_;
        Array(const Array &); // Not emplemented by design for RVO, NRVO enforcement
        Array(Array &&);      // Not emplemented by design for RVO, NRVO enforcement

        // Assignment operator
        Array &operator=(const Array &) = default;
        Array &operator=(Array &&) = default;

        // Variadic constructor for the derived class
        template <typename... Args>
        Array(Args &&...args) : ArrayProxy_<A, N>{std::forward<Args>(args)...} {}
    };

    // template <typename A, size_t N>
    // class Array : std::array<A, N>
    // {
    // public:
    //     // Type aliases to match std::array
    //     using value_type = A;
    //     using size_type = size_t;

    //     // Default constructor
    //     Array() = default;          // Unpredicatable data_
    //     Array(const Array &); // Not emplemented by design for RVO, NRVO enforcement
    //     Array(Array &&);      // Not emplemented by design for RVO, NRVO enforcement

    //     template <typename... Args>
    //     Array(Args &&...args)
    //         : data_{std::forward<Args>(args)...} {}

    //     // Assignment

    //     Array &operator=(const Array &other)
    //     {
    //         if (this != &other)
    //         {
    //             for (size_t i = 0; i < N; ++i)
    //             {
    //                 data_[i] = other.data_[i];
    //             }
    //         }
    //         return *this;
    //     }

    //     // Accessors
    //     // ! Temp no constexpr
    //     A &operator[](size_type index)
    //     {
    //         return data_[index];
    //     }

    //     constexpr const A &operator[](size_type index) const
    //     {
    //         return data_[index];
    //     }

    //     inline constexpr size_type size() const noexcept
    //     {
    //         return N;
    //     }

    //     // Iterators
    //     A *begin() noexcept
    //     {
    //         return data_;
    //     }

    //     const A *begin() const noexcept
    //     {
    //         return data_;
    //     }

    //     A *end() noexcept
    //     {
    //         return data_ + N;
    //     }

    //     const A *end() const noexcept
    //     {
    //         return data_ + N;
    //     }

    //     bool empty() const noexcept
    //     {
    //         return N == 0;
    //     }

    // private:
    //     A data_[N];
    // };

    // template <typename A, size_t N>
    // bool operator==(const Array<A, N> &lhs, const Array<A, N> &rhs)
    // {
    //     return std::equal(lhs.begin(), lhs.end(), rhs.begin());
    // }

    template <typename A, size_t Capacity>
    class ArrVec
    {
    public:
        using value_type = A;
        using size_type = size_t;

        // Constructors
        ArrVec() : size_(0) {}
        ArrVec(const size_t s) : size_(0) {} // Unpredicatable data_
        ArrVec(const ArrVec &);              // Not emplemented by design for RVO, NRVO enforcement
        ArrVec(ArrVec &&);                   // Not emplemented by design for RVO, NRVO enforcement
        template <typename... Args>
        ArrVec(const Args &...args)
            : data_{args...},
              size_(sizeof...(args)) {}

        // Assignment

        ArrVec &operator=(const ArrVec &other)
        {
            if (this != &other)
            {
                size_ = other.size_;
                for (size_t i = 0; i < size_; ++i)
                {
                    data_[i] = other.data_[i];
                }
            }
            return *this;
        }

        // Accessors
        A &operator[](const size_type index)
        {
            return data_[index];
        }

        const A &operator[](const size_type index) const
        {
            return data_[index];
        }

        // Modifiers
        void push_back(const value_type &value)
        {
            if (size_ < Capacity)
            {
                data_[size_] = value;
                ++size_;
            }
        }

        void pop_back()
        {
            if (size_ > 0)
            {
                --size_;
            }
        }

        // Capacity
        constexpr size_type capacity() const noexcept
        {
            return Capacity;
        }

        // size
        size_type size() const noexcept
        {
            return size_;
        }

        A *data()
        {
            return data_;
        }

        const A *data() const
        {
            return data_;
        }

        // Iterators
        A *begin() noexcept
        {
            return data_;
        }

        const A *begin() const noexcept
        {
            return data_;
        }

        A *end() noexcept
        {
            return data_ + size_;
        }

        const A *end() const noexcept
        {
            return data_ + size_;
        }

        bool empty() const noexcept
        {
            return size_ == 0;
        }

    private:
        A data_[Capacity];
        size_type size_;
    };

    template <typename A, size_t N>
    bool operator==(const ArrVec<A, N> &lhs, const ArrVec<A, N> &rhs)
    {
        return std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    // Vector

    template <typename A>
    class VectorProxy_ : public std::vector<A>
    {
    public:
        using std::vector<A>::vector;
        VectorProxy_(const VectorProxy_ &) = delete;
        VectorProxy_(VectorProxy_ &&) = delete;

        // Assignment operators
        VectorProxy_ &operator=(const VectorProxy_ &) = default;
        VectorProxy_ &operator=(VectorProxy_ &&) = default;
    };

    template <typename A>
    class Vector : public VectorProxy_<A>
    {
    public:
        using VectorProxy_<A>::VectorProxy_;
        Vector(const Vector &); // Not emplemented by design for RVO, NRVO enforcement
        Vector(Vector &&);      // Not emplemented by design for RVO, NRVO enforcement

        // Assignment operators
        Vector &operator=(const Vector &) = default;
        Vector &operator=(Vector &&) = default;
    };

    // template <typename A>
    // class Vector
    // {
    // public:
    //     // Type aliases
    //     using value_type = A;
    //     using size_type = size_t;

    //     // Default constructor
    //     Vector() : data_(nullptr), size_(0), capacity_(0) {}
    //     Vector(const size_t size) : data_(new A[size * 2]), size_(size), capacity_(size * 2) {} // Unpredicatable data_
    //     Vector(const Vector &);                                                          // Not emplemented by design for RVO, NRVO enforcement
    //     Vector(Vector &&);                                                               // Not emplemented by design for RVO, NRVO enforcement
    //     template <typename... Args>
    //     Vector(const Args &...args)
    //         : data_(new A[sizeof...(args) * 2]),
    //           size_(sizeof...(args)),
    //           capacity_(sizeof...(args) * 2)
    //     {
    //         int i = 0;
    //         for (auto arg : std::initializer_list<typename std::common_type<Args...>::type>{args...})
    //             data_[i++] = arg;
    //     }

    //     // Destructor
    //     ~Vector()
    //     {
    //         delete[] data_;
    //     }

    //     // Assignment

    //     Vector &operator=(const Vector &other)
    //     {
    //         if (this != &other)
    //         {
    //             const size_t other_size = other.size();
    //             if (capacity_ < other_size)
    //             {
    //                 reserve(other.capacity());
    //             }

    //             for (size_t i = 0; i < other_size; ++i)
    //             {
    //                 data_[i] = other.data_[i];
    //             }
    //             size_ = other_size;
    //         }
    //         return *this;
    //     }

    //     // Element access
    //     A &operator[](const size_type index)
    //     {
    //         return data_[index];
    //     }

    //     const A &operator[](const size_type index) const
    //     {
    //         return data_[index];
    //     }

    //     // Modifiers
    //     void push_back(const A &value)
    //     {
    //         if (size_ == capacity_)
    //         {
    //             reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    //         }
    //         data_[size_++] = value;
    //     }

    //     void pop_back()
    //     {
    //         if (size_ > 0)
    //         {
    //             --size_;
    //         }
    //     }

    //     // Capacity
    //     size_type capacity() const noexcept
    //     {
    //         return capacity_;
    //     }

    //     // size
    //     size_type size() const noexcept
    //     {
    //         return size_;
    //     }

    //     // Reserve capacity
    //     void reserve(const size_type new_capacity)
    //     {
    //         if (new_capacity > capacity_)
    //         {
    //             A *new_data = new A[new_capacity];
    //             for (size_type i = 0; i < size_; ++i)
    //             {
    //                 new_data[i] = data_[i];
    //             }
    //             delete[] data_;
    //             data_ = new_data;
    //             capacity_ = new_capacity;
    //         }
    //     }

    //     // Resizing the vector
    //     void resize(const size_type new_size)
    //     {
    //         if (new_size > capacity_)
    //         {
    //             reserve(new_size);
    //         }
    //         size_ = new_size;
    //     }

    //     // Iterators
    //     A *begin() noexcept
    //     {
    //         return data_;
    //     }

    //     const A *begin() const noexcept
    //     {
    //         return data_;
    //     }

    //     A *end() noexcept
    //     {
    //         return data_ + size_;
    //     }

    //     const A *end() const noexcept
    //     {
    //         return data_ + size_;
    //     }

    //     bool empty() const noexcept
    //     {
    //         return size_ == 0;
    //     }

    // private:
    //     A *data_;
    //     size_type size_;
    //     size_type capacity_;
    // };

    // template <typename A>
    // bool operator==(const Vector<A> &lhs, const Vector<A> &rhs)
    // {
    //     return std::equal(lhs.begin(), lhs.end(), rhs.begin());
    // }

    // ArrayView

    template <typename A, size_t N>
    class ArrayView
    {
    public:
        using value_type = A;
        using size_type = size_t;
        using pointer_type = A *;

        ArrayView(pointer_type p_data)
            : data_(p_data)
        {
        }

        ~ArrayView()
        {
        }

        A &operator[](size_type index)
        {
            return data_[index];
        }

        constexpr const A &operator[](size_type index) const
        {
            return data_[index];
        }

        constexpr size_type size() const noexcept
        {
            return N;
        }

        A *data()
        {
            return data_;
        }

        const A *data() const
        {
            return data_;
        }

        // Iterators
        A *begin() noexcept
        {
            return data_;
        }

        const A *begin() const noexcept
        {
            return data_;
        }

        A *end() noexcept
        {
            return data_ + N;
        }

        const A *end() const noexcept
        {
            return data_ + N;
        }

        bool empty() const noexcept
        {
            return N == 0;
        }

    private:
        pointer_type data_;
    };

    // VectorView

    template <typename A>
    class VectorView
    {
    public:
        using value_type = A;
        using size_type = size_t;
        using pointer_type = Conditional_t<
            IsConst<A>::value,
            const A *,
            A *>;

        VectorView(pointer_type p_data, const size_t size)
            : data_(p_data), size_(size)
        {
        }
        ~VectorView()
        {
        }

        A &operator[](const size_type index)
        {
            return data_[index];
        }

        const A &operator[](const size_type index) const
        {
            return data_[index];
        }

        // Capacity
        // ? Do I need this?
        size_type capacity() const noexcept
        {
            return size_;
        }

        // size
        size_type size() const noexcept
        {
            return size_;
        }

        A *data()
        {
            return data_;
        }

        const A *data() const
        {
            return data_;
        }

        // Iterators
        A *begin() noexcept
        {
            return data_;
        }

        const A *begin() const noexcept
        {
            return data_;
        }

        A *end() noexcept
        {
            return data_ + size_;
        }

        const A *end() const noexcept
        {
            return data_ + size_;
        }

        bool empty() const noexcept
        {
            return size_ == 0;
        }

    private:
        pointer_type data_;
        size_type size_;
    };

    // IsStaticCapacity

    template <typename SeqA>
    struct IsStaticCapacity : std::false_type
    {
    };

    template <typename A, size_t N>
    struct IsStaticCapacity<Array<A, N>> : std::true_type
    {
    };

    template <typename A, size_t N>
    struct IsStaticCapacity<ArrVec<A, N>> : std::true_type
    {
    };

    template <typename A, size_t N>
    struct IsStaticCapacity<std::array<A, N>> : std::true_type
    {
    };

    template <typename A, size_t N>
    struct IsStaticCapacity<A[N]> : std::true_type
    {
    };

    template <typename T>
    struct IsStaticCapacity<T &> : IsStaticCapacity<T>
    {
    };

    template <typename T>
    struct IsStaticCapacity<T &&> : IsStaticCapacity<T>
    {
    };

    // AreAllStaticCapacity

    template <typename... Seqs>
    struct AreAllStaticCapacity
    {
        static constexpr bool value = all_v(IsStaticCapacity<Seqs>::value...);
    };

    // StaticCapacity

    template <typename A>
    struct StaticCapacity
    {
        static constexpr size_t value = 0;
    };

    template <typename A, size_t N>
    struct StaticCapacity<A[N]>
    {
        static constexpr size_t value = N;
    };

    template <typename A, size_t N>
    struct StaticCapacity<std::array<A, N>>
    {
        static constexpr size_t value = N;
    };

    template <typename A, size_t N>
    struct StaticCapacity<Array<A, N>>
    {
        static constexpr size_t value = N;
    };

    template <typename A, size_t N>
    struct StaticCapacity<ArrVec<A, N>>
    {
        static constexpr size_t value = N;
    };

    template <typename A>
    struct StaticCapacity<A &> : StaticCapacity<A>
    {
    };

    template <typename A>
    struct StaticCapacity<A &&> : StaticCapacity<A>
    {
    };

    // IsStaticLength

    template <typename T>
    struct IsStaticLength : std::false_type
    {
    };

    template <typename A, size_t N>
    struct IsStaticLength<Array<A, N>> : std::true_type
    {
    };

    template <typename A, size_t N>
    struct IsStaticLength<std::array<A, N>> : std::true_type
    {
    };

    template <typename A, size_t N>
    struct IsStaticLength<A[N]> : std::true_type
    {
    };

    template <typename T>
    struct IsStaticLength<T &> : IsStaticLength<T>
    {
    };

    template <typename T>
    struct IsStaticLength<T &&> : IsStaticLength<T>
    {
    };

    // StaticLength

    template <typename SeqA>
    struct StaticLength;

    template <typename A, size_t N>
    struct StaticLength<Array<A, N>>
    {
        static constexpr size_t value = N;
    };

    template <typename A, size_t N>
    struct StaticLength<std::array<A, N>>
    {
        static constexpr size_t value = N;
    };

    template <typename A, size_t N>
    struct StaticLength<A[N]>
    {
        static constexpr size_t value = N;
    };

    template <typename SeqA>
    struct StaticLength<SeqA &> : StaticLength<SeqA>
    {
    };

    template <typename SeqA>
    struct StaticLength<SeqA &&> : StaticLength<SeqA>
    {
    };

    // length

    template <typename SeqA>
    constexpr auto length(const SeqA &as)
        -> EnableIf_t<!IsStaticLength<SeqA>::value,
                      size_t>
    {
        return as.size();
    }

    template <typename SeqA>
    constexpr auto length(const SeqA &as)
        -> EnableIf_t<IsStaticLength<SeqA>::value,
                      IntegralConst<size_t, StaticLength<SeqA>::value>>
    {
        return IntegralConst<size_t, StaticLength<SeqA>::value>{};
    }

    template <typename A, size_t N>
    constexpr auto length(const A (&)[N])
        -> IntegralConst<size_t, N>
    {
        return IntegralConst<size_t, N>{};
    }

    // p_data

    template <typename SeqA>
    constexpr auto p_data(const SeqA &as)
        -> decltype(as.data())
    {
        return as.data();
    }

    template <typename A, size_t N>
    constexpr A *p_data(A (&as)[N])
    {
        return (A *)as;
    }

    template <typename A, size_t N>
    constexpr const A *p_data(const A (&as)[N])
    {
        return (const A *)as;
    }

    // StaticSizeT

    // template <size_t N>
    // using StaticSizeT = IntegralConst<size_t, N>;

    // min_size_v

    template <typename A>
    constexpr A min_size_v(const A &value)
    {
        return value;
    }

    template <typename Head, typename... Tail>
    constexpr auto min_size_v(const Head &head, const Tail &...tail)
        -> typename std::conditional<
            all_v(IsIntegralConst<Head>::value, IsIntegralConst<Tail>::value...),
            Head,
            size_t>::type
    {
        return head < min_size_v(tail...) ? head : min_size_v(tail...);
    }

    // size_v_product

    template <typename A>
    constexpr A size_v_product(const A &value)
    {
        return value;
    }

    template <typename Head, typename... Tail>
    constexpr auto size_v_product(const Head &head, const Tail &...tail)
        -> typename std::conditional<
            all_v(IsIntegralConst<Head>::value, IsIntegralConst<Tail>::value...),
            Head,
            size_t>::type
    {
        return head * size_v_product(tail...);
    }

    // MinStaticCapacity

    template <typename... Seqs>
    struct MinStaticCapacity;

    template <typename Seq>
    struct MinStaticCapacity<Seq>
    {
        static constexpr size_t value = StaticCapacity<Seq>::value;
    };

    template <typename Head, typename... Tail>
    struct MinStaticCapacity<Head, Tail...>
    {
        static constexpr size_t head = StaticCapacity<Head>::value;
        static constexpr size_t tail = min_size_v(StaticCapacity<Tail>::value...);
        static constexpr size_t value = head < tail ? head : tail;
    };

    // StaticCapacityProduct

    template <typename... Seqs>
    struct StaticCapacityProduct;

    template <typename SeqA>
    struct StaticCapacityProduct<SeqA>
    {
        static constexpr size_t value = StaticCapacity<SeqA>::value;
    };

    template <typename Head, typename... Tail>
    struct StaticCapacityProduct<Head, Tail...>
    {
        static constexpr size_t value = StaticCapacity<Head>::value * StaticCapacityProduct<Tail...>::value;
    };

    // ElementType

    template <typename A>
    struct ElementType
    {
        using type = typename A::value_type;
    };

    template <typename A, size_t N>
    struct ElementType<A[N]>
    {
        using type = A;
    };

    template <typename A>
    struct ElementType<A &> : ElementType<A>
    {
    };

    template <typename A>
    struct ElementType<A &&> : ElementType<A>
    {
    };

    // Element_t

    template <typename A>
    using Element_t = typename ElementType<A>::type;

};

#endif