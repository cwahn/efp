#ifndef EFP_FORMAT_HPP_
#define EFP_FORMAT_HPP_

#define FMT_HEADER_ONLY
#include "efp/fmt/core.h"

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

// Do the same for efp::Vector and efp::ArrVec
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

#endif