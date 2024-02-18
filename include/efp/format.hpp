#ifndef EFP_FORMAT_HPP_
#define EFP_FORMAT_HPP_

#define FMT_HEADER_ONLY
#include "efp/fmt/core.h"
#include "efp/sequence.hpp"
#include "efp/string.hpp"
#include "efp/cyclic.hpp"
#include "efp/enum.hpp"

namespace efp {

template<typename... Args>
using FormatString = fmt::format_string<Args...>;

template<typename... Args>
inline auto format(FormatString<Args...> fmt, Args&&... args) -> String {
    return fmt::vformat(fmt, fmt::make_format_args(args...));
}

template<typename... Args>
inline void print(FormatString<Args...> fmt, Args&&... args) {
    const auto& vargs = fmt::make_format_args(args...);
    return fmt::detail::is_utf8() ? fmt::vprint(fmt, vargs)
                                  : fmt::detail::vprint_mojibake(stdout, fmt, vargs);
}

// template<typename... Args>
// inline void print(std::FILE* f, fmt::format_string<Args...> fmt, Args&&... args) {
//     const auto& vargs = fmt::make_format_args(args...);
//     return fmt::detail::is_utf8() ? fmt::vprint(f, fmt, vargs)
//                                   : fmt::detail::vprint_mojibake(f, fmt, vargs);
// }

template<typename... Args>
inline void println(FormatString<Args...> fmt, Args&&... args) {
    return fmt::println(stdout, fmt, efp::forward<Args>(args)...);
}

// template<typename... Args>
// inline void println(std::FILE* f, fmt::format_string<Args...> fmt, Args&&... args) {
//     return fmt::print(f, "{}\n", fmt::format(fmt, efp::forward<Args>(args)...));
// }

}  // namespace efp

// Specialize fmt::formatter for efp::Array
template<typename T, size_t ct_size>
struct fmt::formatter<efp::Array<T, ct_size>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) const -> format_parse_context::iterator {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const efp::Array<T, ct_size>& arr, FormatContext& ctx) -> format_context::iterator {
        return fmt::format_to(ctx.out(), "[{}]", fmt::join(arr.begin(), arr.end(), ", "));
    }
};

// Specialize fmt::formatter for efp::ArrVec
template<typename T, size_t ct_cap>
struct fmt::formatter<efp::ArrVec<T, ct_cap>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) const -> format_parse_context::iterator {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const efp::ArrVec<T, ct_cap>& arrvec, FormatContext& ctx)
        -> format_context::iterator {
        return fmt::format_to(ctx.out(), "[{}]", fmt::join(arrvec.begin(), arrvec.end(), ", "));
    }
};

// Specialize fmt::formatter for efp::Vector
template<typename T, typename Allocator, typename Traits>
struct fmt::formatter<efp::Vector<T, Allocator, Traits>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) const -> format_parse_context::iterator {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const efp::Vector<T, Allocator, Traits>& vec, FormatContext& ctx)
        -> format_context::iterator {
        return fmt::format_to(ctx.out(), "[{}]", fmt::join(vec.begin(), vec.end(), ", "));
    }
};

// Sepecialize fmt::formatter for efp::Vcb
template<typename T, size_t ct_size>
struct fmt::formatter<efp::Vcb<T, ct_size>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) const -> format_parse_context::iterator {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const efp::Vcb<T, ct_size>& vcb, FormatContext& ctx) -> format_context::iterator {
        return fmt::format_to(ctx.out(), "[{}]", fmt::join(vcb.begin(), vcb.end(), ", "));
    }
};

// Specialize fmt::formatter for efp::Vcq
template<typename T, size_t ct_cap>
struct fmt::formatter<efp::Vcq<T, ct_cap>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) const -> format_parse_context::iterator {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const efp::Vcq<T, ct_cap>& vcq, FormatContext& ctx) -> format_context::iterator {
        return fmt::format_to(ctx.out(), "[{}]", fmt::join(vcq.begin(), vcq.end(), ", "));
    }
};

// todo enum support
// Specialize fmt::formatter for efp::Enum
// template<typename... As>
// struct fmt::formatter<efp::Enum<As...>> {
//     template<typename ParseContext>
//     constexpr auto parse(ParseContext& ctx) const -> format_parse_context::iterator {
//         return ctx.begin();
//     }

//     template<typename FormatContext>
//     auto format(const efp::Enum<As...>& enm, FormatContext& ctx) -> format_context::iterator {
//         return fmt::format_to(ctx.out(), "{}", enm.to_string());
//     }
// };

#endif