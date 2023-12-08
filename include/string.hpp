#ifndef STRING_HPP_
#define STRING_HPP_

#include "prelude.hpp"
#include "sequence.hpp"

namespace efp
{
    // CString
    // Owned C-style null operator including const char*
    // Not safe to build from regular const char *
    class CString
    {
    private:
        const char *ptr_;

    public:
        // const char pointer with nullptr.
        CString(const char *ptr)
            : ptr_(ptr) {}

        CString(const CString &) = delete;
        CString &operator=(const CString &) = delete;

        CString(CString &&other) noexcept : ptr_(other.ptr_)
        {
            other.ptr_ = nullptr;
        }

        CString &operator=(CString &&other) noexcept
        {
            if (this != &other)
            {
                delete[] ptr_;

                ptr_ = other.ptr_;
                other.ptr_ = nullptr;
            }
            return *this;
        }

        ~CString()
        {
            delete[] ptr_;
        }

        operator const char *() const
        {
            return ptr_;
        }

        // Equality operator for comparing with another CString
        bool operator==(const CString &other) const
        {
            // If both are null, they are considered equal
            if (ptr_ == nullptr && other.ptr_ == nullptr)
                return true;

            // If one is null and the other is not, they can't be equal
            if (ptr_ == nullptr || other.ptr_ == nullptr)
                return false;

            // Use strcmp to compare the strings
            return strcmp(ptr_, other.ptr_) == 0;
        }

        // Equality operator for comparing with a C-style string
        bool operator==(const char *c_str) const
        {
            // If both are null, they are considered equal
            if (ptr_ == nullptr && c_str == nullptr)
                return true;

            // If one is null and the other is not, they can't be equal
            if (ptr_ == nullptr || c_str == nullptr)
                return false;

            // Use strcmp to compare the strings
            return strcmp(ptr_, c_str) == 0;
        }
    };

    template <>
    class Sequence<char, dyn, dyn>
        : public SequenceBase<Sequence<char, dyn, dyn>>
    {
    public:
        using Element = char;
        static constexpr size_t ct_len = dyn;
        static constexpr size_t ct_cap = dyn;

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
                data_ = new char[capacity_];

                // memcpy(data_, other.data_, sizeof(char) * size_);
                for (size_t i = 0; i < other.size_; ++i)
                {
                    new (&data_[i]) char(other.data_[i]);
                }
            }
        }

        Sequence(Sequence &&other)
            : data_{other.data_}, size_{other.size_}, capacity_{other.capacity_}
        {
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }

        // ! Varadic constructor is not working for String
        // template <typename... Args>
        // Sequence(const Args &...args)
        //     : data_{new char[sizeof...(args)]},
        //       capacity_(sizeof...(args)),
        //       size_(sizeof...(args))
        // {
        //     size_t i = 0;
        //     for (auto arg : std::initializer_list<char>{args...})
        //         data_[i++] = arg;
        // }

        Sequence(const char *c_str)
            : data_{nullptr}, size_{0}, capacity_{0}
        {
            for (size_t i = 0; c_str[i] != '\0'; ++i)
            {
                push_back(c_str[i]);
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
                char *newData = nullptr;
                if (other.size_ > 0)
                {
                    newData = new char[other.capacity_];

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
                char *newData = nullptr;
                if (other.size_ > 0)
                {
                    newData = new char[other.capacity_];

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

        char &operator[](int index)
        {
            return data_[index];
        }

        const char &operator[](int index) const
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

        // Specialized equality comparison operator with const char *
        bool operator==(const char *c_str) const
        {
            // Check if both are null pointers
            if (data_ == nullptr && c_str == nullptr)
                return true;

            // If one is null and the other is not, they can't be equal
            if (data_ == nullptr || c_str == nullptr)
                return false;

            // Compare the contents up to the size of the SequenceView
            if (strncmp(data_, c_str, size_) != 0)
                return false;

            // Check if the character at the position size_ in c_str is the null character
            return c_str[size_] == '\0';
        }

        size_t size() const
        {
            return size_;
        }

        size_t capacity() const
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
                char *new_data = new char[new_capacity];

                for (size_t i = 0; i < size_; ++i)
                {
                    new_data[i] = data_[i];
                }

                delete[] data_;

                data_ = new_data;
                capacity_ = new_capacity;
            }
        }

        void push_back(const char &value)
        {
            if (size_ >= capacity_)
            {
                reserve(capacity_ == 0 ? 1 : 2 * capacity_);
            }

            new (&data_[size_]) char{value};
            ++size_;
        }

        void push_back(char &&value)
        {
            if (size_ >= capacity_)
            {
                reserve(capacity_ == 0 ? 1 : 2 * capacity_);
            }

            new (&data_[size_]) char(efp::move(value));
            ++size_;
        }

        void erase(int index)
        {
            if (index < 0 || index >= size_)
            {
                abort();
            }

            for (size_t i = index; i < size_ - 1; ++i)
            {
                new (&data_[i]) char(efp::move(data_[i + 1]));
            }

            --size_;
        }

        const CString c_str() const
        {
            const size_t size_ = size();
            char *extended_buffer = new char[size_ + 1];
            memcpy(extended_buffer, data(), size_);
            extended_buffer[size_] = '\0';
            return CString(extended_buffer); // Mark for deletion.
        }

        // String append(const String &string) const
        // {
        //     return efp::append(*this, string);
        // }

        // todo Interface with StringView

        void append_mut(const Sequence &string)
        {
            for_each([&](char c)
                     { push_back(c); },
                     string);
        }

        const char *data() const
        {
            return data_;
        }

        char *data()
        {
            return data_;
        }

        char *begin()
        {
            return data_;
        }

        const char *begin() const
        {
            return data_;
        }

        char *end()
        {
            return data_ + size_;
        }

        const char *end() const
        {
            return data_ + size_;
        }

        bool empty() const
        {
            return size_ == 0;
        }

    private:
        char *data_;
        size_t size_;
        size_t capacity_;
    };

    using String = Sequence<char, dyn, dyn>;

    // VectorView<const char> specialization for StringView
    template <>
    class SequenceView<const char, dyn, dyn>
        : public SequenceBase<SequenceView<const char, dyn, dyn>>
    {
    public:
        using Element = const char;
        static constexpr size_t ct_len = dyn;
        static constexpr size_t ct_cap = dyn;

        SequenceView()
            : data_(nullptr), size_(0), capacity_(0) {}

        SequenceView(Element *data, size_t size)
            : data_(data), size_(size), capacity_(size)
        {
            // Ensure that data is not nullptr for a non-empty view.
            if (size > 0 && data_ == nullptr)
            {
                abort();
            }
        }

        // StringView could be constructed from string literal
        SequenceView(Element *data)
            : data_(data), size_(strlen(data)), capacity_(size_)
        {
            // Ensure that data is not nullptr for a non-empty view.
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

        // Specialized equality comparison operator with const char *
        bool operator==(const char *c_str) const
        {
            // Check if both are null pointers
            if (data_ == nullptr && c_str == nullptr)
                return true;

            // If one is null and the other is not, they can't be equal
            if (data_ == nullptr || c_str == nullptr)
                return false;

            // Compare the contents up to the size of the SequenceView
            if (strncmp(data_, c_str, size_) != 0)
                return false;

            // Check if the character at the position size_ in c_str is the null character
            return c_str[size_] == '\0';
        }

        const CString c_str() const
        {
            const size_t size_ = size();
            char *extended_buffer = new char[size_ + 1];
            memcpy(extended_buffer, data_, size_);
            extended_buffer[size_] = '\0';
            return CString(extended_buffer);
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

    using StringView = SequenceView<const char, dyn, dyn>;

    inline String join(const String &delimeter, const Vector<String> &strings)
    {
        String result{};

        const size_t string_num = length(strings);
        const auto join_ = [&](int i, const String &s)
        {
            result.append_mut(s);
            if (i < string_num - 1)
                result.append_mut(delimeter);
        };

        for_each_with_index(join_, strings);

        return result;
    }

// todo STL only
#include <iostream>
    inline std::ostream &operator<<(std::ostream &os, const String &string)
    {
        for_each([&](char c)
                 { os << c; },
                 string);
        return os;
    }
};

#endif