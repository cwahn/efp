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

        // Implicit conversion to const char*
        operator const char *() const
        {
            return ptr_;
        }

        // Move constructor
        CString(CString &&other) noexcept : ptr_(other.ptr_)
        {
            other.ptr_ = nullptr;
        }

        // Move assignment operator
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

        // Deleted copy constructor and copy assignment operator to prevent copying
        CString(const CString &) = delete;
        CString &operator=(const CString &) = delete;
    };

    // String

    class String : public Vector<char>
    {
    public:
        String() : Vector<char>() {}

        String(const char *c_str)
        {
            for (size_t i = 0; c_str[i] != '\0'; ++i)
            {
                push_back(c_str[i]);
            }
        }

        ~String() {}

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

        void append_mut(const String &string)
        {
            for_each([&](char c)
                     { push_back(c); },
                     string);
        }
    };

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