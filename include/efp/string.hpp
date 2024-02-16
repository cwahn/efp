#ifndef STRING_HPP_
#define STRING_HPP_

#include <cstring>

#include "efp/prelude.hpp"
#include "efp/sequence.hpp"

namespace efp {
// CString
// Owned C-style null operator including const char*
// Not safe to build from regular const char *
class CString {
  public:
    // const char pointer with nullptr.
    CString(const char* ptr) : _data(ptr) {}

    CString(const CString&) = delete;
    CString& operator=(const CString&) = delete;

    CString(CString&& other) noexcept : _data(other._data) {
        other._data = nullptr;
    }

    CString& operator=(CString&& other) noexcept {
        if (this != &other) {
            delete[] _data;

            _data = other._data;
            other._data = nullptr;
        }
        return *this;
    }

    ~CString() {
        delete[] _data;
    }

    operator const char*() const {
        return _data;
    }

    // Equality operator for comparing with another CString
    bool operator==(const CString& other) const {
        // If both are null, they are considered equal
        if (_data == nullptr && other._data == nullptr)
            return true;

        // If one is null and the other is not, they can't be equal
        if (_data == nullptr || other._data == nullptr)
            return false;

        // Use strcmp to compare the strings
        return strcmp(_data, other._data) == 0;
    }

    // Equality operator for comparing with a C-style string
    bool operator==(const char* c_str) const {
        // If both are null, they are considered equal
        if (_data == nullptr && c_str == nullptr)
            return true;

        // If one is null and the other is not, they can't be equal
        if (_data == nullptr || c_str == nullptr)
            return false;

        // Use strcmp to compare the strings
        return strcmp(_data, c_str) == 0;
    }

  private:
    const char* _data;
};

namespace detail {
    template<typename T>
    struct IsCharType: False {};

    // Specializations for character types
    template<>
    struct IsCharType<char>: True {};

    template<>
    struct IsCharType<wchar_t>: True {};

    template<>
    struct IsCharType<char16_t>: True {};

    template<>
    struct IsCharType<char32_t>: True {};

#if __cplusplus >= 202002L
    template<>
    struct IsCharType<char8_t>: True {};  // C++20 char8_t support
#endif
}  // namespace detail

template<typename Char>
class Vector<Char, EnableIf<detail::IsCharType<Char>::value>>: public detail::VectorBase<Char> {
  public:
    using Base = detail::VectorBase<Char>;
    using Base::Base;

    Vector(const Char* c_str) {
        Base::_capacity = strlen(c_str);
        Base::_data = static_cast<Char*>(::operator new[](Base::_capacity * sizeof(Char)));
        Base::_size = Base::_capacity;
        memcpy(Base::_data, c_str, Base::_size);
    }

    bool operator==(const Vector& other) const {
        return Base::operator==(other);
    }

    // Specialized equality comparison operator with const char *
    bool operator==(const Char* c_str) const {
        // Check if both are null pointers
        if (Base::_data == nullptr && c_str == nullptr)
            return true;

        // If one is null and the other is not, they can't be equal
        if (Base::_data == nullptr || c_str == nullptr)
            return false;

        // Compare the contents up to the size of the SequenceView
        if (strncmp(Base::_data, c_str, Base::_size) != 0)
            return false;

        // Check if the character at the position _size in c_str is the null character
        return c_str[Base::_size] == '\0';
    }

    // todo STL only
    operator std::string() const {
        return std::string(Base::_data, Base::_size);
    }

    // todo Change to hold one extra capacity for null character
    const CString c_str() const {
        Char* extended_buffer = new char[Base::_size + 1];
        memcpy(extended_buffer, Base::_data, Base::_size);
        extended_buffer[Base::_size] = '\0';
        return CString(extended_buffer);  // Mark for deletion.
    }

    // String append(const String &string) const
    // {
    //     return efp::append(*this, string);
    // }

    // todo Interface with StringView
    void append_mut(const Vector& string) {
        for_each([&](Char c) { Base::push_back(c); }, string);
    }
};

template<typename Char, typename = EnableIf<detail::IsCharType<Char>::value>>
using BasicString = Vector<Char>;

using String = BasicString<char>;

using WString = BasicString<wchar_t>;

using U16String = BasicString<char16_t>;

using U32String = BasicString<char32_t>;

#if __cplusplus >= 202002L
using U8String = BasicString<char8_t>;
#endif

// VectorView<const char> specialization for StringView
template<>
class VectorView<const char> {
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