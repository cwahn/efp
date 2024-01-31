#ifndef FORMAT_HPP_
#define FORMAT_HPP_

#include "meta.hpp"
#include "efp/sequence.hpp"
#include "efp/string.hpp"

namespace efp {
// Nil type represents the end of the type list
struct Nil {
    using Head = Nil;
    using Tail = Nil;
};

// TypeSeq is a template struct that holds a head type and a tail type
template <typename H, typename T>
struct TypeSequence {
    using Head = H;
    using Tail = T;
};

template <typename H, typename T>
using TypeSeq = TypeSequence<H, T>;

// Head type extraction
template <typename TypeSeq>
struct TypeSeqHead {
    using Type = typename TypeSeq::Head;
};

// Tail type extraction
template <typename TypeSeq>
struct TypeSeqTail {
    using Type = typename TypeSeq::Tail;
};

// MakeTypeSeq creates a type list from a set of types
template <typename... As>
struct MakeTypeSeqImpl {
};

template <>
struct MakeTypeSeqImpl<> {
    using Type = Nil;
};

template <typename H, typename... Ts>
struct MakeTypeSeqImpl<H, Ts...> {
    using Type = TypeSeq<H, typename MakeTypeSeqImpl<Ts...>::Type>;
};

// Alias template for easier use of MakeTypeSeqImpl
template <typename... As>
using MakeTypeSeq = typename MakeTypeSeqImpl<As...>::Type;

// AppendTypeSeqImpl appends a type to a type list
template <typename As, typename A>
struct AppendTypeSeqImpl {
};

template <>
struct AppendTypeSeqImpl<Nil, Nil> {
    using Type = Nil;
};

template <typename A>
struct AppendTypeSeqImpl<Nil, A> {
    using Type = MakeTypeSeq<A>;
};

template <typename H, typename T>
struct AppendTypeSeqImpl<Nil, TypeSeq<H, T>> {
    using Type = TypeSeq<H, T>;
};

template <typename H, typename T, typename A>
struct AppendTypeSeqImpl<TypeSeq<H, T>, A> {
    using Type = TypeSeq<H, typename AppendTypeSeqImpl<T, A>::Type>;
};

// Alias template for easier use of AppendTypeSeqImpl
template <typename As, typename A>
using AppendTypeSeq = typename AppendTypeSeqImpl<As, A>::Type;

// ContainsTypeSeq checks if a type is contained sin a type list
template <typename TypeSeq, typename T>
struct ContainsTypeSeq : False {
};

template <typename T, typename Tail>
struct ContainsTypeSeq<TypeSeq<T, Tail>, T> : True {
};

template <typename H, typename Tail, typename T>
struct ContainsTypeSeq<TypeSeq<H, Tail>, T> {
    static constexpr bool value = ContainsTypeSeq<Tail, T>::value;
};

// RemoveTypeSeqImpl removes a type from a type list
template <typename TypeSeq, typename T>
struct RemoveTypeSeqImpl {
    using Type = TypeSeq;
};

template <typename T, typename Tail>
struct RemoveTypeSeqImpl<TypeSeq<T, Tail>, T> {
    using Type = typename RemoveTypeSeqImpl<Tail, T>::Type;
};

template <typename Head, typename Tail, typename T>
struct RemoveTypeSeqImpl<TypeSeq<Head, Tail>, T> {
    using Type = TypeSeq<Head, typename RemoveTypeSeqImpl<Tail, T>::Type>;
};

// Alias template for easier use of RemoveTypeSeqImpl
template <typename TypeSeq, typename T>
using RemoveTypeSeq = typename RemoveTypeSeqImpl<TypeSeq, T>::Type;

// SubstituteTypeSeqImpl substitutes one type for another in a type list
template <typename TypeSeq, typename Target, typename Alt>
struct SubstituteTypeSeqImpl {
    using Type = TypeSeq;
};

template <typename T, typename Tail, typename Ts>
struct SubstituteTypeSeqImpl<TypeSeq<T, Tail>, T, Ts> {
    using Type = TypeSeq<Ts, typename SubstituteTypeSeqImpl<Tail, T, Ts>::Type>;
};

template <typename Head, typename Tail, typename T, typename Ts>
struct SubstituteTypeSeqImpl<TypeSeq<Head, Tail>, T, Ts> {
    using Type = TypeSeq<Head, typename SubstituteTypeSeqImpl<Tail, T, Ts>::Type>;
};

// Alias template for easier use of SubstituteTypeSeqImpl
template <typename TypeSeq, typename T, typename Ts>
using SubstituteTypeSeq = typename SubstituteTypeSeqImpl<TypeSeq, T, Ts>::Type;

// Define a typelist of size_teger types
using IntegeralTypes = MakeTypeSeq<
    char, signed char, int, long, long long,
    unsigned char, unsigned, unsigned long, unsigned long long>;

// IsIntegeralType checks if a type T is in the list of size_teger types
template <typename T>
struct IsIntegeralType {
    static constexpr bool value = ContainsTypeSeq<IntegeralTypes, T>::value;
};

// template <char val>
// struct CharType
// {
//     static const constexpr char Value = val;
// };

template <char value>
using Char = IntegralConst<char, value>;

// Chars creates a type list of CharType types
template <char c, char... cs>
struct CharsImpl {
    using Type = TypeSeq<Char<c>, typename CharsImpl<cs...>::Type>;
};

template <char c>
struct CharsImpl<c> {
    using Type = TypeSeq<Char<c>, Nil>;
};

template <char... cs>
using Chars = typename CharsImpl<cs...>::Type;

//  StringToChars

// StringToCharsImpl converts a compile-time string to a type list of Char types
template <typename Str, int pos, char c>
struct StringToCharsImpl {
    using Tail = typename StringToCharsImpl<Str, pos + 1, Str::string()[pos + 1]>::Type;
    using Type = TypeSeq<Char<c>, Tail>;
};

template <typename Str, int pos>
struct StringToCharsImpl<Str, pos, '\0'> {
    using Type = Nil;
};

template <typename Str>
using StringToChars = typename StringToCharsImpl<Str, 0, Str::string()[0]>::Type;

// CharsToString converts a type list of CharType types back to a runtime string
template <typename TypeSeq, char... cs>
struct CharsToString {
};

template <char c, typename Rests, char... cs>
struct CharsToString<TypeSeq<Char<c>, Rests>, cs...>
    : public CharsToString<Rests, cs..., c> {
};

template <>
struct CharsToString<Nil> {
    static constexpr const char* string() { return ""; }
};

template <char... cs>
struct CharsToString<Nil, cs...> {
    using List = Chars<cs...>;

    static const char* string() {
        static const char string[] = {cs..., '\0'};
        return string;
    }
};

template <typename T>
struct TypeToFormat;

template <>
struct TypeToFormat<char> {
    using Type = Chars<'c'>;
};
template <>
struct TypeToFormat<short> {
    using Type = Chars<'d'>;
};
template <>
struct TypeToFormat<int> {
    using Type = Chars<'d'>;
};
template <>
struct TypeToFormat<long int> {
    using Type = Chars<'l', 'd'>;
};
template <>
struct TypeToFormat<long long int> {
    using Type = Chars<'l', 'l', 'd'>;
};
template <>
struct TypeToFormat<signed char> {
    using Type = Chars<'h', 'h', 'd'>;
};
template <>
struct TypeToFormat<unsigned char> {
    using Type = Chars<'u'>;
};
template <>
struct TypeToFormat<unsigned short> {
    using Type = Chars<'u'>;
};
template <>
struct TypeToFormat<unsigned> {
    using Type = Chars<'u'>;
};
template <>
struct TypeToFormat<unsigned long> {
    using Type = Chars<'l', 'u'>;
};
template <>
struct TypeToFormat<unsigned long long> {
    using Type = Chars<'l', 'l', 'u'>;
};

template <>
struct TypeToFormat<bool> {
    using Type = Chars<'d'>;
};

template <>
struct TypeToFormat<float> {
    using Type = Chars<'f'>;
};
template <>
struct TypeToFormat<double> {
    using Type = Chars<'l', 'f'>;
};

template <>
struct TypeToFormat<nullptr_t> {
    using Type = Chars<'p'>;
};

template <typename T>
struct TypeToFormat<T*> {
    using Type = Chars<'p'>;
};

template <typename T, typename FL>
struct FormatStringImpl {
    using RawType = CVRemoved<T>;

    static constexpr bool is_s_fmt = ContainsTypeSeq<FL, Char<'s'>>::value;
    static constexpr bool is_string =
        IsSame<char, CVRemoved<PointerRemoved<RawType>>>::value;

    static constexpr bool is_size_t = IsIntegeralType<RawType>::value;
    static constexpr bool is_x_fmt = ContainsTypeSeq<FL, Char<'x'>>::value;

    using RawFormat = typename TypeToFormat<T>::Type;

    using UIntXFormat = Conditional<
        is_size_t && is_x_fmt,
        SubstituteTypeSeq<
            SubstituteTypeSeq<RawFormat, Char<'d'>, Char<'x'>>,
            Char<'u'>, Char<'x'>>,
        RawFormat>;

    using FormatType = Conditional<
        is_s_fmt && is_string,
        SubstituteTypeSeq<RawFormat, Char<'p'>, Char<'s'>>,
        UIntXFormat>;

    using FilteredFL = RemoveTypeSeq<
        RemoveTypeSeq<FL, Char<'x'>>,
        Char<'s'>>;

    using Type = AppendTypeSeq<FilteredFL, FormatType>;
};

template <typename T, typename FL>
using FormatString = typename FormatStringImpl<T, FL>::Type;

template <class InList, class OutList, int Counter>
struct FindBrace;

// Specialization for finding the closing brace
template <class InList, class OutList>
struct FindBrace<TypeSeq<Char<'}'>, InList>, OutList, 1> {
    using Before = OutList;
    using After = InList;
};

// General case: iterating through the list
template <char C, class InList, class OutList, int N>
struct FindBrace<TypeSeq<Char<C>, InList>, OutList, N>
    : public FindBrace<InList, AppendTypeSeq<OutList, Char<C>>, N> {
    static_assert(C != '{', "Found nested braces: {...{...}...}! Maybe you want to mask the outer one?");
};

// Error case: missing closing brace
template <class OutList, int N>
struct FindBrace<Nil, OutList, N> {
    static_assert(N + 1 == N, "Missing } after {.");
};

// CFormatStringImpl

template <typename SL, typename Types>
struct CFormatStringImpl;

template <>
struct CFormatStringImpl<Nil, Nil> {
    using Type = Nil;
};

template <typename Types>
struct CFormatStringImpl<Nil, Types> {
    using Type = Nil;
    static_assert(AlwaysFalse<Types>::value, "There are more vars than format tokens!");
};

template <typename SL, typename Types>
struct CFormatStringImpl<TypeSeq<Char<'%'>, TypeSeq<Char<'%'>, SL>>, Types> {
    using Type = TypeSeq<Char<'%'>, TypeSeq<Char<'%'>, typename CFormatStringImpl<SL, Types>::Type>>;
};

template <typename SL, typename T, typename Types>
struct CFormatStringImpl<TypeSeq<Char<'%'>, SL>, TypeSeq<T, Types>> {
    using Type = TypeSeq<Char<'%'>, typename CFormatStringImpl<SL, Types>::Type>;
};

template <typename SL, typename Types>
struct CFormatStringImpl<TypeSeq<Char<'\\'>, TypeSeq<Char<'{'>, SL>>, Types> {
    using Type = TypeSeq<Char<'{'>, typename CFormatStringImpl<SL, Types>::Type>;
};

template <typename SL, typename Types>
struct CFormatStringImpl<TypeSeq<Char<'{'>, SL>, Types> {
    using OtherBrace = FindBrace<SL, Nil, 1>;
    using FormatBlock = typename OtherBrace::Before;
    using RestString = typename OtherBrace::After;

    static_assert(!IsSame<Types, Nil>::value, "There are more {} than arguments to print");
    using T = typename Types::Head;
    using FmtStr = FormatString<T, FormatBlock>;

    using Type = TypeSeq<Char<'%'>, AppendTypeSeq<FmtStr, typename CFormatStringImpl<RestString, typename Types::Tail>::Type>>;
};

template <typename C, typename SL, typename Types>
struct CFormatStringImpl<TypeSeq<C, SL>, Types> {
    using Type = TypeSeq<C, typename CFormatStringImpl<SL, Types>::Type>;
};

template <typename StringProvider, typename PtList>
using CFormatString = typename CFormatStringImpl<StringToChars<StringProvider>, PtList>::Type;

template <typename... Ts>
MakeTypeSeq<Ts...> TieTypes(Ts...);

// Format String :: char * goes in to the StringProvider
// CFormatString takes format_string :: char * as arguement
// CFormatString

// #define AUTOFORMAT(s, ...)                                         \
//     [] {                                                           \
//         struct StringProvider                                      \
//         {                                                          \
//             static constexpr const char *string()                  \
//             {                                                      \
//                 return static_cast<const char *>(s);               \
//             }                                                      \
//         };                                                         \
//         using ParamTypes = decltype(TieTypes(__VA_ARGS__));        \
//         using CFmtStr = CFormatString<StringProvider, ParamTypes>; \
//         return CharsToString<CFmtStr>::string();                   \
//     }()

// #define print(s, ...) printf(auto_format(s, ##__VA_ARGS__), ##__VA_ARGS__);

// String format(const char *format, ...)
// {
//     va_list args;
//     va_start(args, format);

//     const int length = vsnprintf(nullptr, 0, format, args);

//     String buffer;
//     buffer.reserve(length + 1);

//     vsnprintf(buffer.data(), length + 1, format, args);
//     buffer.resize(length);

//     va_end(args);
//     return buffer;
// }

// #define format(s, ...) format_(auto_format(s, ##__VA_ARGS__), ##__VA_ARGS__);

// #define auto_format(s, ...) auto_format(s, ##__VA_ARGS__);

template <typename... Args>
const char* auto_format(const char* s, const Args&... args) {
    return [=] {
        struct StringProvider {
            static constexpr const char* string() {
                return static_cast<const char*>(s);
            }
        };
        // using ParamTypes = decltype(TieTypes(__VA_ARGS__));
        using ParamTypes = MakeTypeSeq<Args...>;
        using CFmtStr = CFormatString<StringProvider, ParamTypes>;
        return CharsToString<CFmtStr>::string();
    }();
}

template <typename... Args>
void print(const char* fmt, const Args&... args) {
    printf(auto_format(fmt, args...));
}

template <typename... Arg>
String format(const char* fmt, const Arg&... args) {
    const auto c_fmt = auto_format(fmt, args...);
    const size_t result_length = vsnprintf(nullptr, 0, c_fmt, args...);

    String buffer;
    buffer.reserve(result_length + 1);

    vsnprintf(buffer.data(), result_length + 1, format, args...);
    buffer.resize(result_length);

    return buffer;
}

}; // namespace efp

#endif