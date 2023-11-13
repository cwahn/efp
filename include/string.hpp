#ifndef STRING_HPP_
#define STRING_HPP_

#include "prelude.hpp"
#include "sequence.hpp"

namespace efp
{
    // CString

    class CString
    {
    private:
        const char *ptr_;

    public:
        CString(const char *ptr)
            : ptr_(ptr) {}

        ~CString()
        {
            delete[] ptr_;
        }

        operator const char *() const
        {
            return ptr_;
        }

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

        CString(const CString &) = delete;
        CString &operator=(const CString &) = delete;
    };

    template <>
    class Sequence<char, dyn, dyn>
        : public SequenceBase<Sequence<char, dyn, dyn>>
    {
    public:
        using Element = char;
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
                data_ = new char[capacity_];

                // memcpy(data_, other.data_, sizeof(char) * size_);
                for (int i = 0; i < other.size_; ++i)
                {
                    new (&data_[i]) char(other.data_[i]);
                }
            }
        }

        Sequence(Sequence &&other)
            : data_{other.data_}, size_{other.size_}, capacity_{other.capacity_}
        {
            other.data_ = nullptr;
        }

        // ! Varadic constructor is not working for String
        // template <typename... Args>
        // Sequence(const Args &...args)
        //     : data_{new char[sizeof...(args)]},
        //       capacity_(sizeof...(args)),
        //       size_(sizeof...(args))
        // {
        //     int i = 0;
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

                    for (int i = 0; i < other.size_; ++i)
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

                    for (int i = 0; i < other.size_; ++i)
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

            for (int i = 0; i < size_; ++i)
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
                char *new_data = new char[new_capacity];

                for (int i = 0; i < size_; ++i)
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

            for (int i = index; i < size_ - 1; ++i)
            {
                new (&data_[i]) char(efp::move(data_[i + 1]));
            }

            --size_;
        }

        const CString c_str() const
        {
            const int size_ = size();
            char *extended_buffer = new char[size_ + 1];
            memcpy(extended_buffer, data(), size_);
            extended_buffer[size_] = '\0';
            return CString(extended_buffer); // Mark for deletion.
        }

        // String append(const String &string) const
        // {
        //     return efp::append(*this, string);
        // }

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

        bool is_empty() const
        {
            return size_ == 0;
        }

    private:
        char *data_;
        int size_;
        int capacity_;
    };

    using String = Sequence<char, dyn, dyn>;

    String join(const String &delimeter, const Vector<String> &strings)
    {
        String result{};

        const int string_num = length(strings);
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
    std::ostream &operator<<(std::ostream &os, const String &string)
    {
        for_each([&](char c)
                 { os << c; },
                 string);
        return os;
    }
};

#endif