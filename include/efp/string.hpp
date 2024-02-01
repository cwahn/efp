#ifndef STRING_HPP_
#define STRING_HPP_

#include "efp/prelude.hpp"
#include "efp/sequence.hpp"

namespace efp {
// CString
// Owned C-style null operator including const char*
// Not safe to build from regular const char *
class CString {
  private:
    const char* ptr_;

  public:
    // const char pointer with nullptr.
    CString(const char* ptr) : ptr_(ptr) {}

    CString(const CString&) = delete;
    CString& operator=(const CString&) = delete;

    CString(CString&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    CString& operator=(CString&& other) noexcept {
        if (this != &other) {
            delete[] ptr_;

            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    ~CString() {
        delete[] ptr_;
    }

    operator const char*() const {
        return ptr_;
    }

    // Equality operator for comparing with another CString
    bool operator==(const CString& other) const {
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
    bool operator==(const char* c_str) const {
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

template<>
// class Sequence<char, dyn, dyn>
class Vector<char>: public detail::VectorBase<char> {
  public:
    using Base = detail::VectorBase<char>;
    using Base::Base;

    // using Element = char;
    // static constexpr size_t ct_size = dyn;
    // static constexpr size_t ct_capacity = dyn;

    // static_assert(ct_size >= -1, "ct_size must greater or equal than -1.");
    // static_assert(ct_capacity >= -1, "ct_capacity must greater or equal than -1.");

    // Vector() : _data {nullptr}, _size {0}, _capacity {0} {}

    // Vector(const Vector& other) : _data {nullptr}, _size {0}, _capacity {0} {
    //     if (other._data) {
    //         _size = other._size;
    //         _capacity = other._capacity;
    //         _data = new char[_capacity];

    //         // memcpy(_data, other._data, sizeof(char) * _size);
    //         for (size_t i = 0; i < other._size; ++i) {
    //             new (&_data[i]) char(other._data[i]);
    //         }
    //     }
    // }

    // Vector(Vector&& other) : _data {other._data}, _size {other._size}, _capacity {other._capacity} {
    //     other._data = nullptr;
    //     other._size = 0;
    //     other._capacity = 0;
    // }

    // ! Varadic constructor is not working for String
    // template <typename... Args>
    // Sequence(const Args &...args)
    //     : _data{new char[sizeof...(args)]},
    //       _capacity(sizeof...(args)),
    //       _size(sizeof...(args))
    // {
    //     size_t i = 0;
    //     for (auto arg : std::initializer_list<char>{args...})
    //         _data[i++] = arg;
    // }

    // Vector(const char* c_str)
    //     : _size {strlen(c_str)}, _capacity {_size},
    //       _data {static_cast<char*>(::operator new[](_capacity * sizeof(char)))} {
    //     // for (size_t i = 0; c_str[i] != '\0'; ++i) {
    //     //     push_back(c_str[i]);
    //     // }
    //     memcpy(_data, c_str, _size);
    // }

    Vector(const char* c_str) {
        _capacity = strlen(c_str);
        _data = static_cast<char*>(::operator new[](_capacity * sizeof(char)));
        _size = _capacity;
        memcpy(_data, c_str, _size);
    }

    // ~Vector() {
    //     if (_data) {
    //         delete[] _data;
    //         _data = nullptr;
    //     }
    // }

    // // todo copy_and_swap
    // Vector& operator=(const Vector& other) noexcept {
    //     if (this != &other) {
    //         char* newData = nullptr;
    //         if (other._size > 0) {
    //             newData = new char[other._capacity];

    //             for (size_t i = 0; i < other._size; ++i) {
    //                 newData[i] = other._data[i];
    //             }
    //         }

    //         _data = newData;
    //         _size = other._size;
    //         _capacity = other._capacity;
    //     }
    //     return *this;
    // }

    // Vector& operator=(Vector&& other) noexcept {
    //     if (this != &other) {
    //         delete[] _data;

    //         _data = other._data;
    //         _size = other._size;
    //         _capacity = other._capacity;

    //         other._data = nullptr;
    //         other._size = 0;
    //         other._capacity = 0;
    //     }
    //     return *this;
    // }

    // Vector assign_impl(const Vector& other) {
    //     if (this != &other) {
    //         char* newData = nullptr;
    //         if (other._size > 0) {
    //             newData = new char[other._capacity];

    //             for (size_t i = 0; i < other._size; ++i) {
    //                 newData[i] = other._data[i];
    //             }
    //         }

    //         _data = newData;
    //         _size = other._size;
    //         _capacity = other._capacity;
    //     }
    //     return *this;
    // }

    // char& operator[](int index) {
    //     return _data[index];
    // }

    // const char& operator[](int index) const {
    //     return _data[index];
    // }

    // bool operator==(const Vector& other) const {
    //     if (_size != other._size) {
    //         return false;
    //     }

    //     for (size_t i = 0; i < _size; ++i) {
    //         if (_data[i] != other._data[i]) {
    //             return false;
    //         }
    //     }

    //     return true;
    // }

    bool operator==(const Vector& other) const {
        return Base::operator==(other);
    }

    // Specialized equality comparison operator with const char *
    bool operator==(const char* c_str) const {
        // Check if both are null pointers
        if (_data == nullptr && c_str == nullptr)
            return true;

        // If one is null and the other is not, they can't be equal
        if (_data == nullptr || c_str == nullptr)
            return false;

        // Compare the contents up to the size of the SequenceView
        if (strncmp(_data, c_str, _size) != 0)
            return false;

        // Check if the character at the position _size in c_str is the null character
        return c_str[_size] == '\0';
    }

    // size_t size() const {
    //     return _size;
    // }

    // size_t capacity() const {
    //     return _capacity;
    // }

    // void resize(int length) {
    //     if (length < 0) {
    //         throw std::runtime_error("length must be greater or equal than 0.");
    //     }

    //     if (length > _capacity) {
    //         reserve(length);
    //     }

    //     _size = length;
    // }

    // void reserve(int new_capacity) {
    //     if (new_capacity > _capacity) {
    //         char* new_data = new char[new_capacity];

    //         for (size_t i = 0; i < _size; ++i) {
    //             new_data[i] = _data[i];
    //         }

    //         delete[] _data;

    //         _data = new_data;
    //         _capacity = new_capacity;
    //     }
    // }

    // void push_back(const char& value) {
    //     if (_size >= _capacity) {
    //         reserve(_capacity == 0 ? 1 : 2 * _capacity);
    //     }

    //     new (&_data[_size]) char {value};
    //     ++_size;
    // }

    // void push_back(char&& value) {
    //     if (_size >= _capacity) {
    //         reserve(_capacity == 0 ? 1 : 2 * _capacity);
    //     }

    //     new (&_data[_size]) char(efp::move(value));
    //     ++_size;
    // }

    // void erase(int index) {
    //     if (index < 0 || index >= _size) {
    //         throw std::runtime_error("Vector<char>::erase: index out of range");
    //     }

    //     for (size_t i = index; i < _size - 1; ++i) {
    //         new (&_data[i]) char(efp::move(_data[i + 1]));
    //     }

    //     --_size;
    // }

    const CString c_str() const {
        const size_t _size = size();
        char* extended_buffer = new char[_size + 1];
        memcpy(extended_buffer, data(), _size);
        extended_buffer[_size] = '\0';
        return CString(extended_buffer);  // Mark for deletion.
    }

    // String append(const String &string) const
    // {
    //     return efp::append(*this, string);
    // }

    // todo Interface with StringView

    void append_mut(const Vector& string) {
        for_each([&](char c) { push_back(c); }, string);
    }

    // const char* data() const {
    //     return _data;
    // }

    // char* data() {
    //     return _data;
    // }

    // char* begin() {
    //     return _data;
    // }

    // const char* begin() const {
    //     return _data;
    // }

    // char* end() {
    //     return _data + _size;
    // }

    // const char* end() const {
    //     return _data + _size;
    // }

    // bool empty() const {
    //     return _size == 0;
    // }

    //   private:
    //     char* _data;
    //     size_t _size;
    //     size_t _capacity;
};

// using String = Sequence<char, dyn, dyn>;
using String = Vector<char>;

// VectorView<const char> specialization for StringView
template<>
// class SequenceView<const char, dyn, dyn>
class VectorView<const char>
// : public SequenceBase<SequenceView<const char, dyn, dyn>>
{
  public:
    using Element = const char;
    static constexpr size_t ct_size = dyn;
    static constexpr size_t ct_capacity = dyn;

    VectorView() : _data(nullptr), _size(0), _capacity(0) {}

    VectorView(Element* data, size_t size) : _data(data), _size(size), _capacity(size) {
        // Ensure that data is not nullptr for a non-empty view.
        if (size > 0 && _data == nullptr) {
            throw std::runtime_error("VectorView<const char>::VectorView: data is nullptr");
        }
    }

    // StringView could be constructed from string literal
    VectorView(Element* data) : _data(data), _size(strlen(data)), _capacity(_size) {
        // Ensure that data is not nullptr for a non-empty view.
    }

    VectorView& operator=(const VectorView& other) {
        if (this != &other) {
            _data = other._data;
            _size = other._size;
            _capacity = other._capacity;
        }
        return *this;
    }

    Element& operator[](size_t index) {
        return _data[index];
    }

    const Element& operator[](size_t index) const {
        return _data[index];
    }

    bool operator==(const VectorView& other) const {
        return (_data == other._data) && (_size == other._size) && (_capacity == other._capacity);
    }

    // Specialized equality comparison operator with const char *
    bool operator==(const char* c_str) const {
        // Check if both are null pointers
        if (_data == nullptr && c_str == nullptr)
            return true;

        // If one is null and the other is not, they can't be equal
        if (_data == nullptr || c_str == nullptr)
            return false;

        // Compare the contents up to the size of the SequenceView
        if (strncmp(_data, c_str, _size) != 0)
            return false;

        // Check if the character at the position _size in c_str is the null character
        return c_str[_size] == '\0';
    }

    const CString c_str() const {
        const size_t _size = size();
        char* extended_buffer = new char[_size + 1];
        memcpy(extended_buffer, _data, _size);
        extended_buffer[_size] = '\0';
        return CString(extended_buffer);
    }

    size_t size() const {
        return _size;
    }

    size_t capacity() const {
        return _capacity;
    }

    const Element* data() const {
        return _data;
    }

    Element* data() {
        return _data;
    }

    Element* begin() {
        return _data;
    }

    const Element* begin() const {
        return _data;
    }

    Element* end() {
        return _data + _size;
    }

    const Element* end() const {
        return _data + _size;
    }

    bool empty() const {
        return _size == 0;
    }

  private:
    Element* _data;
    size_t _size;
    size_t _capacity;
};

// using StringView = SequenceView<const char, dyn, dyn>;
using StringView = VectorView<const char>;

// inline String join(const String &delimeter, const Vector<String> &strings)
// {
//     String result{};

//     const size_t string_num = length(strings);
//     const auto join_ = [&](int i, const String &s)
//     {
//         result.append_mut(s);
//         if (i < string_num - 1)
//             result.append_mut(delimeter);
//     };

//     for_each_with_index(join_, strings);

//     return result;
// }

// todo STL only

// inline std::ostream& operator<<(std::ostream& os, const String& string) {
//     for_each([&](char c) { os << c; }, string);
//     return os;
// }
};  // namespace efp

#endif