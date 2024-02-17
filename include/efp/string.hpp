#ifndef STRING_HPP_
#define STRING_HPP_

// todo Hosted only

#if defined(__STDC_HOSTED__)
    #include <cstring>
#endif

#include "efp/prelude.hpp"
#include "efp/sequence.hpp"

namespace efp {

// if freestanding manual implementation of strlen, memcpy and strncmp
#if !defined(__STDC_HOSTED__)
static size_t std::strlen(const Char* c_str) {
    size_t len = 0;
    while (c_str[len] != '\0') {
        ++len;
    }
    return len;
}

static void std::memcpy(Char* dest, const Char* src, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        dest[i] = src[i];
    }
}

static int std::strncmp(const Char* str1, const Char* str2, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if (str1[i] != str2[i]) {
            return str1[i] - str2[i];
        }
    }
    return 0;
}
#endif

// BasicString

template<typename Char, typename Traits, typename Allocator>
class Vector<Char, Allocator, Traits, EnableIf<detail::IsCharType<Char>::value>>:
    public detail::VectorBase<Char, Allocator> {
public:
    using Base = detail::VectorBase<Char>;
    using Base::Base;

    using traits_type = Traits;

    Vector(const Char* c_str) {
        Base::_capacity = std::strlen(c_str);
        Base::_data = static_cast<Char*>(::operator new[](Base::_capacity * sizeof(Char)));
        Base::_size = Base::_capacity;
        std::memcpy(Base::_data, c_str, Base::_size);
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
        if (std::strncmp(Base::_data, c_str, Base::_size) != 0)
            return false;

        // Check if the character at the position _size in c_str is the null character
        return c_str[Base::_size] == '\0';
    }

    // todo at
    Char at(size_t pos) const {
        if (pos >= Base::_size) {
            // throw std::out_of_range("Index out of range");
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

    // todo append(size_type n, CharT c)

    // todo assign(const CharT* s, size_type n)

    // todo insert(size_type pos, const CharT* s)

    // todo insert(size_type pos, const CharT* s, size_type n)

    // todo replace(size_type pos, size_type len, const CharT* s)

    // todo replace(size_type pos, size_type len, const CharT* s, size_type n)

    // todo substr(size_type pos = 0, size_type len = npos) const

    // todo copy(CharT* dest, size_type n, size_type pos = 0) const

    // todo find, rfind, find_first_of, find_last_of, find_first_not_of, find_last_not_of

    // todo compare(const basic_string& str) const

    // todo compare(size_type pos, size_type len, const basic_string& str) const

    // todo Implicit conversion operators to std::basic_string_view<CharT, Traits>

    const Char* c_str() const {
        Base::_data[Base::_size] = '\0';
        return Base::_data;
    }

    // todo Interface with StringView
    // void append_mut(const Vector& string) {
    //     for_each([&](Char c) { Base::push_back(c); }, string);
    // }

    // todo STL only
    operator std::string() const {
        return std::string(Base::_data, Base::_size);
    }
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

template<typename Char>
class VectorView<Char, EnableIf<detail::IsCharType<Char>::value>>:
    public detail::VectorViewBase<Char> {
public:
    using Base = detail::VectorViewBase<Char>;
    using Base::Base;

    VectorView(const Char* c_str) {
        Base::_size = std::strlen(c_str);
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
        if (std::strncmp(Base::_data, c_str, Base::_size) != 0)
            return false;

        // Check if the character at the position _size in c_str is the null character
        return c_str[Base::_size] == '\0';
    }

    // todo STL only
    operator std::string() const {
        return std::string(Base::_data, Base::_size);
    }

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