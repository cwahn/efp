#ifndef STRING_HPP_
#define STRING_HPP_

#include "sequence.hpp"

namespace efp
{
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

    // template <int ct_length, int ct_capacity>
    // class Str : public Sequence<char, ct_length, ct_capacity>
    // {
    //     // String(const char *c_str)
    //     // {
    //     //     for (size_t i = 0; c_str[i] != '\0'; ++i)
    //     //     {
    //     //         push_back(c_str[i]);
    //     //     }
    //     // }
    // };

    // template <typename Derived, typename = EnableIf<
    //                                 IsSame<RemoveReference<Element<Derived>>, char>::value, void>>
    // using Str = StringBase<Derived>;

    // using String = StringBase<>;

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
    };

};

#endif