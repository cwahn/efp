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

        String &operator+=(const String &other)
        {
            for (int i = 0; i < other.size(); ++i)
            {
                push_back(other[i]);
            }
            return *this;
        }

        // Will return smart pointer of maybe owning the data if the buffer is full and can't append "/0" at the end
        const CString c_str() const
        {
            const int size_ = size();

            char *extended_buffer = new char[size_ + 1];
            memcpy(extended_buffer, data(), size_);
            extended_buffer[size_] = '\0';
            return CString(extended_buffer); // Mark for deletion.
        }

        bool operator==(const String &other) const
        {
            return static_cast<const Vector<char> &>(*this) == static_cast<const Vector<char> &>(other);
        }

        // Other string-specific functions...
    };

};

#endif