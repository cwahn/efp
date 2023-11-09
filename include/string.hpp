#ifndef STRING_HPP_
#define STRING_HPP_

#include "sequence.hpp"

namespace efp
{
    class CStrPtr
    {
    private:
        const char *ptr_;
        bool is_owning_;

    public:
        // CStrPtr(char *ptr, bool is_owning = false)
        //     : ptr_(ptr), is_owning_(is_owning) {}

        CStrPtr(const char *ptr, bool is_owning = false)
            : ptr_(ptr),
              is_owning_(is_owning) {}

        ~CStrPtr()
        {
            if (is_owning_)
            {
                delete[] ptr_;
            }
        }

        // Implicit conversion to const char*
        operator const char *() const
        {
            return ptr_;
        }

        // Implicit conversion to char* (non-const)
        // operator char *()
        // {
        //     return ptr_;
        // }

        // Move constructor
        CStrPtr(CStrPtr &&other) noexcept : ptr_(other.ptr_), is_owning_(other.is_owning_)
        {
            other.ptr_ = nullptr;
            other.is_owning_ = false;
        }

        // Move assignment operator
        CStrPtr &operator=(CStrPtr &&other) noexcept
        {
            if (this != &other)
            {
                if (is_owning_)
                {
                    delete[] ptr_;
                }
                ptr_ = other.ptr_;
                is_owning_ = other.is_owning_;
                other.ptr_ = nullptr;
                other.is_owning_ = false;
            }
            return *this;
        }

        // Deleted copy constructor and copy assignment operator to prevent copying
        CStrPtr(const CStrPtr &) = delete;
        CStrPtr &operator=(const CStrPtr &) = delete;
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

        // String(const String &other) : Vector<char>(static_cast<const Vector<char> &>(other)) {}

        // String(String &&other) : Vector<char>(move(other)) {}

        // String(std::initializer_list<char> ilist) : Vector<char>(ilist) {}

        ~String() {}

        // String &operator=(const String &other)
        // {
        //     Vector<char>::operator=(other);
        //     return *this;
        // }

        // String &operator=(String &&other)
        // {
        //     Vector<char>::operator=(move(other));
        //     return *this;
        // }

        String &operator+=(const String &other)
        {
            for (int i = 0; i < other.size(); ++i)
            {
                push_back(other[i]);
            }
            return *this;
        }

        const CStrPtr c_str() const
        {
            const int size_ = size();
            if (size_ < capacity())
            {
                // There's enough space to add a null terminator without extending the buffer.
                Vector<char> &non_const_this = *const_cast<Vector<char> *>(static_cast<const Vector<char> *>(this));
                non_const_this[size_] = '\0';
                return CStrPtr(data(), false); // No need to delete, part of the Vector's data.
            }
            else
            {
                // The buffer is full or doesn't have a null terminator, need to extend.
                char *extended_buffer = new char[size_ + 1];
                memcpy(extended_buffer, data(), size_);
                extended_buffer[size_] = '\0';
                return CStrPtr(extended_buffer, true); // Mark for deletion.
            }
        }

        bool operator==(const String &other) const
        {
            return static_cast<const Vector<char> &>(*this) == static_cast<const Vector<char> &>(other);
        }

        // Other string-specific functions...
    };

};

#endif