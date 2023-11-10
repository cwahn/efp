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

    // CtListImpl
    // Inspired by printpp

    // Nil type represents the end of the type list
    struct Nil
    {
    };

    // TypeSequence is a template struct that holds a head type and a tail type
    template <typename H, typename T>
    struct TypeSequence
    {
        using Head = H;
        using Tail = T;
    };

    // Head type extraction
    template <typename TypeSeq>
    struct TypeSeqHead
    {
        using Type = typename TypeSeq::Head;
    };

    // Tail type extraction
    template <typename TypeSeq>
    struct TypeSeqTail
    {
        using Type = typename TypeSeq::Tail;
    };

    // MakeTypeSequence creates a type list from a set of types
    template <typename... As>
    struct MakeTypeSequenceImpl
    {
    };

    template <>
    struct MakeTypeSequenceImpl<>
    {
        using Type = Nil;
    };

    template <typename H, typename... Ts>
    struct MakeTypeSequenceImpl<H, Ts...>
    {
        using Type = TypeSequence<H, typename MakeTypeSequenceImpl<Ts...>::Type>;
    };

    // Alias template for easier use of MakeTypeSequenceImpl
    template <typename... As>
    using MakeTypeSequence = typename MakeTypeSequenceImpl<As...>::Type;

    // AppendTypeSequenceImpl appends a type to a type list
    template <typename As, typename A>
    struct AppendTypeSequenceImpl
    {
    };

    template <>
    struct AppendTypeSequenceImpl<Nil, Nil>
    {
        using Type = Nil;
    };

    template <typename A>
    struct AppendTypeSequenceImpl<Nil, A>
    {
        using Type = MakeTypeSequence<A>;
    };

    template <typename H, typename T, typename A>
    struct AppendTypeSequenceImpl<TypeSequence<H, T>, A>
    {
        using Type = TypeSequence<H, typename AppendTypeSequenceImpl<T, A>::Type>;
    };

    // Alias template for easier use of AppendTypeSequenceImpl
    template <typename As, typename A>
    using AppendTypeSequence = typename AppendTypeSequenceImpl<As, A>::Type;

    // ContainsTypeSequence checks if a type is contained sin a type list
    template <typename TypeSeq, typename T>
    struct ContainsTypeSequence : False
    {
    };

    template <typename T, typename Tail>
    struct ContainsTypeSequence<TypeSequence<T, Tail>, T> : False
    {
    };

    template <typename H, typename Tail, typename T>
    struct ContainsTypeSequence<TypeSequence<H, Tail>, T>
    {
        static constexpr bool value = ContainsTypeSequence<Tail, T>::value;
    };

    // RemoveTypeSequenceImpl removes a type from a type list
    template <typename TypeSeq, typename T>
    struct RemoveTypeSequenceImpl
    {
        using Type = TypeSeq;
    };

    template <typename T, typename Tail>
    struct RemoveTypeSequenceImpl<TypeSequence<T, Tail>, T>
    {
        using Type = typename RemoveTypeSequenceImpl<Tail, T>::Type;
    };

    template <typename Head, typename Tail, typename T>
    struct RemoveTypeSequenceImpl<TypeSequence<Head, Tail>, T>
    {
        using Type = TypeSequence<Head, typename RemoveTypeSequenceImpl<Tail, T>::Type>;
    };

    // Alias template for easier use of RemoveTypeSequenceImpl
    template <typename TypeSeq, typename T>
    using RemoveTypeSequence = typename RemoveTypeSequenceImpl<TypeSeq, T>::Type;

    // SubstituteTypeSequenceImpl substitutes one type for another in a type list
    template <typename TypeSeq, typename T, typename Ts>
    struct SubstituteTypeSequenceImpl
    {
        using Type = TypeSeq;
    };

    template <typename T, typename Tail, typename Ts>
    struct SubstituteTypeSequenceImpl<TypeSequence<T, Tail>, T, Ts>
    {
        using Type = TypeSequence<Ts, typename SubstituteTypeSequenceImpl<Tail, T, Ts>::Type>;
    };

    template <typename Head, typename Tail, typename T, typename Ts>
    struct SubstituteTypeSequenceImpl<TypeSequence<Head, Tail>, T, Ts>
    {
        using Type = TypeSequence<Head, typename SubstituteTypeSequenceImpl<Tail, T, Ts>::Type>;
    };

    // Alias template for easier use of SubstituteTypeSequenceImpl
    template <typename TypeSeq, typename T, typename Ts>
    using SubstituteTypeSequence = typename SubstituteTypeSequenceImpl<TypeSeq, T, Ts>::Type;

    // Define a typelist of integer types
    using IntegeralTypes = MakeTypeSequence<
        char, signed char, int, long, long long,
        unsigned char, unsigned, unsigned long, unsigned long long>;

    // IsIntegeralType checks if a type T is in the list of integer types
    template <typename T>
    struct IsIntegeralType
    {
        static constexpr bool value = ContainsTypeSequence<IntegeralTypes, T>::value;
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
    struct Chars
    {
        using Type = TypeSequence<Char<c>, typename Chars<cs...>::Type>;
    };

    template <char c>
    struct Chars<c>
    {
        using Type = TypeSequence<Char<c>, Nil>;
    };

    template <char... cs>
    using CharsType = typename Chars<cs...>::Type;

    // StringToCharsImpl converts a compile-time string to a type list of CharType types
    template <typename Str, size_t pos, char c>
    struct StringToCharsImpl
    {
        using NextPiece = typename StringToCharsImpl<Str, pos + 1, Str::string()[pos + 1]>::Type;
        using Type = TypeSequence<Char<c>, NextPiece>;
    };

    template <typename Str, size_t pos>
    struct StringToCharsImpl<Str, pos, '\0'>
    {
        using Type = Nil;
    };

    template <typename Str>
    using StringToChars = typename StringToCharsImpl<Str, 0, Str::string()[0]>::Type;

    // CharsToString converts a type list of CharType types back to a runtime string
    template <typename TypeSequence, char... cs>
    struct CharsToString;

    template <char c, typename Rests, char... cs>
    struct CharsToString<TypeSequence<Char<c>, Rests>, cs...>
        : public CharsToString<Rests, cs..., c>
    {
    };

    template <>
    struct CharsToString<Nil>
    {
        static constexpr const char *string() { return ""; }
    };

    template <char... cs>
    struct CharsToString<Nil, cs...>
    {
        using List = CharsType<cs...>;

        static const char *string()
        {
            static const char string[] = {cs..., '\0'};
            return string;
        }
    };

    template <typename T>
    struct AlwaysFalse : False
    {
    };

    template <typename T>
    struct TypeToFormat;

    template <>
    struct TypeToFormat<char>
    {
        using Type = Chars<'c'>;
    };
    template <>
    struct TypeToFormat<short>
    {
        using Type = Chars<'d'>;
    };
    template <>
    struct TypeToFormat<int>
    {
        using Type = Chars<'d'>;
    };
    template <>
    struct TypeToFormat<long int>
    {
        using Type = Chars<'l', 'd'>;
    };
    template <>
    struct TypeToFormat<long long int>
    {
        using Type = Chars<'l', 'l', 'd'>;
    };
    template <>
    struct TypeToFormat<signed char>
    {
        using Type = Chars<'h', 'h', 'd'>;
    };
    template <>
    struct TypeToFormat<unsigned char>
    {
        using Type = Chars<'u'>;
    };
    template <>
    struct TypeToFormat<unsigned short>
    {
        using Type = Chars<'u'>;
    };
    template <>
    struct TypeToFormat<unsigned>
    {
        using Type = Chars<'u'>;
    };
    template <>
    struct TypeToFormat<unsigned long>
    {
        using Type = Chars<'l', 'u'>;
    };
    template <>
    struct TypeToFormat<unsigned long long>
    {
        using Type = Chars<'l', 'l', 'u'>;
    };

    template <>
    struct TypeToFormat<bool>
    {
        using Type = Chars<'d'>;
    };

    template <>
    struct TypeToFormat<float>
    {
        using Type = Chars<'f'>;
    };
    template <>
    struct TypeToFormat<double>
    {
        using Type = Chars<'l', 'f'>;
    };

    template <>
    struct TypeToFormat<nullptr_t>
    {
        using Type = Chars<'p'>;
    };

    template <typename T>
    struct TypeToFormat<T *>
    {
        using Type = Chars<'p'>;
    };

    template <typename T, typename FL>
    struct FormatString
    {
        using RawType = CVRemoved<T>;

        static constexpr bool is_format_s = ContainsTypeSequence<FL, Char<'s'>>::value;
        static constexpr bool is_string =
            IsSame<char, CVRemoved<PointerRemoved<RawType>>>::value;

        static constexpr bool is_int = IsIntegeralType<RawType>::value;
        static constexpr bool has_format_x = ContainsTypeSequence<FL, Char<'x'>>::value;

        using RawFormat = typename TypeToFormat<T>::Type;

        using UIntXFormat = Conditional<
            is_int && has_format_x,
            SubstituteTypeSequence<
                SubstituteTypeSequence<RawFormat, Char<'d'>, Char<'x'>>,
                Char<'u'>, Char<'x'>>,
            RawFormat>;

        using FormatType = Conditional<
            is_format_s && is_string,
            SubstituteTypeSequence<RawFormat, Char<'p'>, Char<'s'>>,
            UIntXFormat>;

        using FilteredFL = RemoveTypeSequence<
            RemoveTypeSequence<FL, Char<'x'>>,
            Char<'s'>>;

        using Type = AppendTypeSequence<FilteredFL, FormatType>;
    };

    template <class InList, class OutList, size_t Counter>
    struct FindBrace;

    // Specialization for finding the closing brace
    template <class InList, class OutList>
    struct FindBrace<TypeSequence<Char<'}'>, InList>, OutList, 1>
    {
        using Before = OutList;
        using After = InList;
    };

    // General case: iterating through the list
    template <char C, class InList, class OutList, size_t N>
    struct FindBrace<TypeSequence<Char<C>, InList>, OutList, N>
        : public FindBrace<InList, AppendTypeSequence<OutList, Char<C>>, N>
    {
        static_assert(C != '{', "Found nested braces: {...{...}...}! Maybe you want to mask the outer one?");
    };

    // Error case: missing closing brace
    template <class OutList, size_t N>
    struct FindBrace<Nil, OutList, N>
    {
        static_assert(N + 1 == N, "Missing } after {.");
    };

    template <typename SL, typename TL>
    struct AutoFormat;

    template <>
    struct AutoFormat<Nil, Nil>
    {
        using Type = Nil;
    };

    template <typename TL>
    struct AutoFormat<Nil, TL>
    {
        using Type = Nil;
        static_assert(AlwaysFalse<TL>::value, "There are more vars than format tokens!");
    };

    template <typename SL, typename TL>
    struct AutoFormat<TypeSequence<Char<'%'>, TypeSequence<Char<'%'>, SL>>, TL>
    {
        using Type = TypeSequence<Char<'%'>, TypeSequence<Char<'%'>, typename AutoFormat<SL, TL>::Type>>;
    };

    template <typename SL, typename T, typename TL>
    struct AutoFormat<TypeSequence<Char<'%'>, SL>, TypeSequence<T, TL>>
    {
        using Type = TypeSequence<Char<'%'>, typename AutoFormat<SL, TL>::Type>;
    };

    template <typename SL, typename TL>
    struct AutoFormat<TypeSequence<Char<'\\'>, TypeSequence<Char<'{'>, SL>>, TL>
    {
        using Type = TypeSequence<Char<'{'>, typename AutoFormat<SL, TL>::Type>;
    };

    template <typename SL, typename TL>
    struct AutoFormat<TypeSequence<Char<'{'>, SL>, TL>
    {
        using OtherBrace = FindBrace<SL, Nil, 1>;
        using FormatBlock = typename OtherBrace::Before;
        using RestString = typename OtherBrace::After;

        static_assert(!std::is_same<TL, Nil>::value, "There are more {} than arguments to print");
        using T = typename TL::Head;
        using FormatString = typename FormatString<T, FormatBlock>::Type;

        using Type = TypeSequence<Char<'%'>, AppendTypeSequence<FormatString, typename AutoFormat<RestString, typename TL::Tail>::Type>>;
    };

    template <typename C, typename SL, typename TL>
    struct AutoFormat<TypeSequence<C, SL>, TL>
    {
        using Type = TypeSequence<C, typename AutoFormat<SL, TL>::Type>;
    };

    template <typename StringProvider, typename PtList>
    using AutoFormatType = CharsToString<
        typename AutoFormat<StringToChars<StringProvider>, PtList>::Type>;

    template <typename... Ts>
    MakeTypeSequence<Ts...> TieTypes(Ts...);

#define AUTOFORMAT(s, ...)                                     \
    [] {                                                       \
        struct StringProvider                                  \
        {                                                      \
            static constexpr const char *string()              \
            {                                                  \
                return static_cast<const char *>(s);           \
            }                                                  \
        };                                                     \
        using ParamTypes = decltype(TieTypes(__VA_ARGS__));    \
        using AF = AutoFormatType<StringProvider, ParamTypes>; \
        return AF::string();                                   \
    }()

#define pprintf(s, ...) printf(AUTOFORMAT(s, ##__VA_ARGS__), ##__VA_ARGS__);

};

#endif