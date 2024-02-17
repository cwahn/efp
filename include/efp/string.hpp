#ifndef STRING_HPP_
#define STRING_HPP_

#if defined(__STDC_HOSTED__)
    #include <cstring>
    #include <string>
#endif

#include "efp/prelude.hpp"
#include "efp/sequence.hpp"

namespace efp {

// BasicString

template<typename Char, typename Traits, typename Allocator>
class Vector<Char, Allocator, Traits, EnableIf<detail::IsCharType<Char>::value>>:
    public detail::VectorBase<Char, Allocator> {
public:
    using Base = detail::VectorBase<Char>;
    using Base::Base;

    using traits_type = Traits;
    static const size_t npos = -1;

    Vector(const Char* c_str) {
        Base::_size = Traits::length(c_str);
        Base::_capacity = Base::_size + 1;
        Base::_data = Base::_allocator.allocate(Base::_capacity);
        efp::memcpy(Base::_data, c_str, Base::_size * sizeof(Char));
    }

    Vector(const Char* s, size_t count, const Allocator& alloc = Allocator()) {
        Base::_size = count;
        Base::_capacity = Base::_size + 1;
        Base::_data = Base::_allocator.allocate(Base::_capacity);
        efp::memcpy(Base::_data, s, Base::_size * sizeof(Char));
    }

    Vector(size_t size, Char c) {
        Base::_capacity = size;
        Base::_data = static_cast<Char*>(::operator new[](Base::_capacity * sizeof(Char)));
        Base::_size = size;
        for (size_t i = 0; i < size; ++i) {
            Base::_data[i] = c;
        }
    }

    Vector(const Char* s, size_t size) {
        // Ensure we don't read beyond the end of the provided string
        Base::_capacity = min(std::strlen(s), size);
        Base::_data = static_cast<Char*>(::operator new[](Base::_capacity * sizeof(Char)));
        Base::_size = Base::_capacity;
        std::memcpy(Base::_data, s, Base::_size * sizeof(Char));
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
        if (Traits::compare(Base::_data, c_str, Base::_size) != 0)
            return false;

        // Check if the character at the position _size in c_str is the null character
        return c_str[Base::_size] == '\0';
    }

    // todo at
    Char at(size_t pos) const {
        if (pos >= Base::_size) {
            throw std::runtime_error("Index out of range");
        }

        return Base::_data[pos];
    }

    // todo front
    Char front() const {
        return Base::_data[0];
    }

    // todo back
    Char back() const {
        return Base::_data[Base::_size - 1];
    }

    // todo operator+=
    Vector& operator+=(const Vector& other) {
        for (size_t i = 0; i < other.size(); ++i) {
            Base::push_back(other[i]);
        }

        return *this;
    }

    // todo append(const CharT* s, size_type n)
    Vector& append(const Char* c_str) {
        const size_t len = Traits::length(c_str);

        for (size_t i = 0; i < len; ++i) {
            Base::push_back(c_str[i]);
        }

        return *this;
    }

    // todo append(size_type n, CharT c)
    Vector& append(size_t n, Char c) {
        for (size_t i = 0; i < n; ++i) {
            Base::push_back(c);
        }

        return *this;
    }

    // todo assign(const CharT* s, size_type n)
    Vector& assign(const Char* c_str) {
        Base::clear();
        return append(c_str);
    }

    // todo insert(size_type pos, const CharT* s)
    Vector& insert(size_t pos, const Char* c_str) {
        if (pos > Base::_size) {
            throw std::runtime_error("Index out of range");
        }

        const size_t len = Traits::length(c_str);
        Base::reserve(Base::_size + len);

        for (size_t i = Base::_size; i > pos; --i) {
            Base::_data[i + len - 1] = Base::_data[i - 1];
        }

        for (size_t i = 0; i < len; ++i) {
            Base::_data[pos + i] = c_str[i];
        }

        Base::_size += len;

        return *this;
    }

    // todo insert(size_type pos, const CharT* s, size_type n)
    Vector& insert(size_t pos, const Char* c_str, size_t n) {
        if (pos > Base::_size) {
            throw std::runtime_error("Index out of range");
        }

        // Always reserve one extra space for the null character
        Base::reserve(Base::_size + n + 1);

        for (size_t i = Base::_size; i > pos; --i) {
            Base::_data[i + n - 1] = Base::_data[i - 1];
        }

        for (size_t i = 0; i < n; ++i) {
            Base::_data[pos + i] = c_str[i];
        }

        Base::_size += n;

        return *this;
    }

    // todo replace(size_type pos, size_type len, const CharT* s)

    // todo replace(size_type pos, size_type len, const CharT* s, size_type n)

    // todo substr(size_type pos = 0, size_type len = npos) const
    Vector substr(size_t pos = 0, size_t len = npos) const {
        if (pos > Base::_size) {
            throw std::runtime_error("Index out of range");
        }

        if (len > Base::_size - pos) {
            len = Base::_size - pos;
        }

        return Vector(Base::_data + pos, len);
    }

    // todo size_type copy( CharT* dest, size_type count, size_type pos = 0 ) const
    // The resulting character string is not null-terminated.
    // https://en.cppreference.com/w/cpp/string/basic_string/copy
    size_t copy(Char* dest, size_t count, size_t pos = 0) const {
        if (pos > Base::_size) {
            throw std::runtime_error("Index out of range");
        }

        if (count > Base::_size - pos) {
            count = Base::_size - pos;
        }

        efp::memcpy(dest, Base::_data + pos, count * sizeof(Char));
        return count;
    }

    // todo find, rfind, find_first_of, find_last_of, find_first_not_of, find_last_not_of

    // todo compare(const basic_string& str) const
    int compare(const Vector& other) const {
        return Traits::compare(Base::_data, other.data(), Base::_size);
    }

    // todo compare(size_type pos, size_type len, const basic_string& str) const
    int compare(size_t pos, size_t len, const Vector& other) const {
        if (pos > Base::_size) {
            throw std::runtime_error("Index out of range");
        }

        if (len > Base::_size - pos) {
            len = Base::_size - pos;
        }

        return Traits::compare(Base::_data + pos, other.data(), len);
    }

    // todo Implicit conversion operators to std::basic_string_view<CharT, Traits>

    const Char* c_str() const {
        Base::_data[Base::_size] = '\0';
        return Base::_data;
    }

    // todo Interface with StringView

#if defined(__STDC_HOSTED__)
    operator std::string() const {
        return std::string(Base::_data, Base::_size);
    }
#endif
};

template<
    typename Char,
    typename Traits = detail::DefaultCharTraits<Char>,
    typename Allocator = detail::DefaultAllocator<Char>,
    typename = EnableIf<detail::IsCharType<Char>::value>>
using BasicString = Vector<Char, Allocator, Traits>;

using String = BasicString<char>;

using WString = BasicString<wchar_t>;

using U16String = BasicString<char16_t>;

using U32String = BasicString<char32_t>;

#if __cplusplus >= 202002L
using U8String = BasicString<char8_t>;
#endif

// BasicStringView specialization
// todo Traits

template<typename Char, typename Traits>
class VectorView<Char, Traits, EnableIf<detail::IsCharType<Char>::value>>:
    public detail::VectorViewBase<Char> {
public:
    using Base = detail::VectorViewBase<Char>;
    using Base::Base;

    VectorView(const Char* c_str) {
        Base::_size = Traits::length(c_str);
        Base::_data = c_str;
    }

    bool operator==(const VectorView& other) const {
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
        if (Traits::compare(Base::_data, c_str, Base::_size) != 0)
            return false;

        // Check if the character at the position _size in c_str is the null character
        return c_str[Base::_size] == '\0';
    }

#if defined(__STDC_HOSTED__)
    operator std::string() const {
        return std::string(Base::_data, Base::_size);
    }
#endif

    const Char* c_str() const {
        return Base::_data;
    }
};

// BasicSstringView

template<typename Char, typename = EnableIf<detail::IsCharType<Char>::value>>
using BasicStringView = VectorView<Char>;

using StringView = BasicStringView<char>;

using WStringView = BasicStringView<wchar_t>;

using U16StringView = BasicStringView<char16_t>;

using U32StringView = BasicStringView<char32_t>;

#if __cplusplus >= 202002L
using U8StringView = BasicStringView<char8_t>;
#endif

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

#if defined(__STDC_HOSTED__)

    // inline std::ostream& operator<<(std::ostream& os, const String& string) {
    //     for_each([&](char c) { os << c; }, string);
    //     return os;
    // }

#endif

};  // namespace efp

#endif