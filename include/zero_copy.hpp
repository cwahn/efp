#ifndef ZERO_COPY_HPP_
#define ZERO_COPY_HPP_

template <typename A, std::size_t N>
class StaticArray
{
public:
    // Type aliases to match std::array
    using value_type = A;
    using size_type = std::size_t;

    // Default constructor
    StaticArray() = default;          // Unpredicatable data_
    StaticArray(const StaticArray &); // Not emplemented by design for RVO, NRVO enforcement
    StaticArray(StaticArray &&);      // Not emplemented by design for RVO, NRVO enforcement

    template <typename... Args>
    StaticArray(Args &&...args)
        : data_{std::forward<Args>(args)...} {}

    // Assignment

    StaticArray &operator=(const StaticArray &other)
    {
        if (this != &other)
        {
            for (std::size_t i = 0; i < N; ++i)
            {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }

    // Accessors
    // ! Temp no constexpr
    A &operator[](size_type index)
    {
        return data_[index];
    }

    constexpr const A &operator[](size_type index) const
    {
        return data_[index];
    }

    inline constexpr size_type size() const noexcept
    {
        return N;
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
    A data_[N];
};

template <typename A, std::size_t Capacity>
class StaticVector
{
public:
    using value_type = A;
    using size_type = std::size_t;

    // Constructors
    StaticVector() : size_(0) {}
    StaticVector(const size_t s) : size_(0) {} // Unpredicatable data_
    StaticVector(const StaticVector &);        // Not emplemented by design for RVO, NRVO enforcement
    StaticVector(StaticVector &&);             // Not emplemented by design for RVO, NRVO enforcement
    template <typename... Args>
    StaticVector(const Args &...args)
        : data_{args...},
          size_(sizeof...(args)) {}

    // Assignment

    StaticVector &operator=(const StaticVector &other)
    {
        if (this != &other)
        {
            size_ = other.size_;
            for (std::size_t i = 0; i < size_; ++i)
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

template <typename A>
class DynamicVector
{
public:
    // Type aliases
    using value_type = A;
    using size_type = std::size_t;

    // Default constructor
    DynamicVector() : data_(nullptr), size_(0), capacity_(0) {}
    DynamicVector(const size_t size) : data_(new A[size * 2]), size_(size), capacity_(size * 2) {} // Unpredicatable data_
    DynamicVector(const DynamicVector &);                                                          // Not emplemented by design for RVO, NRVO enforcement
    DynamicVector(DynamicVector &&);                                                               // Not emplemented by design for RVO, NRVO enforcement
    template <typename... Args>
    DynamicVector(const Args &...args)
        : data_(new A[sizeof...(args) * 2]),
          size_(sizeof...(args)),
          capacity_(sizeof...(args) * 2)
    {
        int i = 0;
        for (auto arg : std::initializer_list<typename std::common_type<Args...>::type>{args...})
            data_[i++] = arg;
    }

    // Destructor
    ~DynamicVector()
    {
        delete[] data_;
    }

    // Assignment

    DynamicVector &operator=(const DynamicVector &other)
    {
        if (this != &other)
        {
            size_ = other.size();
            if (capacity_ < size_)
            {
                capacity_ = other.capacity();
                reserve(capacity_);
            }

            for (std::size_t i = 0; i < size_; ++i)
            {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }

    // Element access
    A &operator[](const size_type index)
    {
        return data_[index];
    }

    const A &operator[](const size_type index) const
    {
        return data_[index];
    }

    // Modifiers
    void push_back(const A &value)
    {
        if (size_ == capacity_)
        {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[size_++] = value;
    }

    void pop_back()
    {
        if (size_ > 0)
        {
            --size_;
        }
    }

    // Capacity
    size_type capacity() const noexcept
    {
        return capacity_;
    }

    // size
    size_type size() const noexcept
    {
        return size_;
    }

    // Reserve capacity
    void reserve(const size_type new_capacity)
    {
        if (new_capacity > capacity_)
        {
            A *new_data = new A[new_capacity];
            for (size_type i = 0; i < size_; ++i)
            {
                new_data[i] = data_[i];
            }
            delete[] data_;
            data_ = new_data;
            capacity_ = new_capacity;
        }
    }

    // Resizing the vector
    void resize(const size_type new_size)
    {
        if (new_size > capacity_)
        {
            reserve(new_size);
        }
        size_ = new_size;
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
    A *data_;
    size_type size_;
    size_type capacity_;
};

template <typename A, typename B>
bool operator==(A &&a, B &&b)
{
    return std::equal(a.begin(), a.end(), b.begin());
}

#endif